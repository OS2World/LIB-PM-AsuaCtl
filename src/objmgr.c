/* Asuactl.dll
 * Pasek narz©dzi - object manager
 * (c) 1999 Wojciech Gazda
 *
 * objmgr.c
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/06/27 12:36:00 $
 * $Name:  $
 * $RCSfile: objmgr.c $
 * $Revision: 1.1 $
 *
 */

/*  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License Version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


//#define  INCL_DOSDEV
#define  INCL_DOSPROCESS


// Deklaracje OS/2
#define  INCL_GPILOGCOLORTABLE
#define  INCL_WINRECTANGLES
#define  INCL_WINMESSAGEMGR
#define  INCL_WINWINDOWMGR
#define  INCL_WINFRAMEMGR
#define  INCL_WINPOINTERS
#define  INCL_WINBUTTONS
#define  INCL_WINTIMER
#define  INCL_WININPUT
#define  INCL_WINATOM
#define  INCL_WINSYS
#include <os2.h>

// Funkcje biblioteczne
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

// Deklaracje lokalne
#define  __INTERNAL_USE__
#include "asuintl.h"
#include "toolbar.h"
#include "tooldefs.h"
#include "objmgr.h"


// Deklaracje staˆych
#define TMR_TIPREMOVE  1      // Czasomierz steruj¥cy wyˆaczaniem podpowiedzi
#define TMR_TIPDELAY   2      // czasomierz odmierzaj¥cy czas op¢«nienia podpowidzi


// Prototypy funkcji
MRESULT EXPENTRY ObjectPadProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY ObjectPassThru(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
LONG ObjDeleteObject(HWND hwnd, ULONG objId);
LONG ObjIdFromPosition(HWND hwnd, ULONG pos);
LONG ObjInitialize(TOOLCTL *tcl);
LONG ObjInsertObject(HWND hwnd, TOOLOBJ *tobj, TOOLWIN *twin);
LONG ObjInsertTooltip(HWND hwnd, ULONG objId, PSZ tip);
LONG ObjMoveObject(HWND hwnd, ULONG objId, ULONG newpos, ULONG flAttrs);
HWND ObjQueryHandle(HWND hwnd, ULONG id);
LONG ObjQueryHeight(TOOLCTL *tcl, ULONG flNewState);
LONG ObjQueryObject(HWND hwnd, ULONG objId, TOOLOBJ *tobj);
LONG ObjQueryObjectCount(HWND hwnd);
LONG ObjQueryPosition(HWND hwnd, ULONG objId);
LONG ObjQueryTip(HWND hwnd, ULONG objId, ULONG ulBufSize, PSZ TipText);
LONG ObjQueryWidth(TOOLCTL *tcl, ULONG flNewState);
VOID ObjReleaseResources(TOOLCTL *tcl);
LONG ObjSetObject(HWND hwnd, ULONG objId, ULONG ulOptions, TOOLOBJ *tobj);
LONG ObjShowObject(HWND hwnd, ULONG id, ULONG bNewState);


// Prototypy funkcji lokalnych
static LONG ObjControlObjects(HWND hwnd, ULONG id, ULONG command);
static VOID ObjControlTip(TOOLCTL *tcl, ULONG timer, ULONG id);
static VOID ObjDisplayTip(TOOLCTL *tcl, ULONG id);
static LONG ObjHittest(HWND hwnd, POINTS *cpos);
static VOID ObjPlaceObjects(HWND hwnd);
static VOID ObjRedrawObjects(HWND hwnd);


// Dodatkowe funkcje usˆugowe
static VOID    CalcObjectSize(TOOLCTL *tcl);
static int     CompareObjects(const void *object1, const void *object2);
static LONG    DeletePositionIndex(TOOLCTL *tcl, ULONG objidx);
static LONG    ExpandObjectTable(TOOLCTL *tcl);
static OBJCTL *FindObjectID(TOOLCTL *tcl, ULONG id);
static LONG    InsertPositionIndex(TOOLCTL *tcl, ULONG objidx, ULONG iPosition);
static VOID    PlaceEndcut(TOOLCTL *tcl, RECTL *area);
static VOID    PlaceStandard(TOOLCTL *tcl, RECTL *area);
static VOID    ShrinkObjectTable(TOOLCTL *tcl, ULONG objidx);
static VOID    UpdateAlignment(TOOLCTL *tcl, LONG objidx, ULONG flNewAttrs);
static VOID    UpdateGroupStyle(TOOLCTL *tcl, ULONG ulGroup, ULONG flNewAttrs);
static VOID    DrawSeparator(HPS hps, OBJCTL *obj, RECTL *area, ULONG flState);




// Procedura obsˆuguj¥ca okno przechowuj¥ce obiekty
//
MRESULT EXPENTRY ObjectPadProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
  { case TM_OBJECTCONTROL:      // Sterowanie zachowaniem przycisk¢w i innych obiekt¢w
      return(MRFROMLONG(ObjControlObjects(hwnd, LONGFROMMP(mp1), LONGFROMMP(mp2))));

    case WM_CALCVALIDRECTS:
      WinInvalidateRect(hwnd, NULL, TRUE);
      // DosBeep(1000, 10);
      break;

    case WM_HITTEST:     // Przezroczysto˜† dla komunikat¢w myszy
      return(MRFROMLONG(ObjHittest(hwnd, (POINTS *)&mp1)));

    case WM_PAINT:       // Rysowanie separator¢w i tˆa
      ObjRedrawObjects(hwnd);
      break;

    case WM_SIZE:        // Pozycjonowanie obiekt¢w
      ObjPlaceObjects(hwnd);
      break;

    case WM_TIMER:       // Sterowanie procesem wy˜wietlania podpowiedzi
      { TOOLCTL *tcl;    // Wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi

        // Odczyt adresu struktir kontrolnych
        tcl = ToolLoadData(hwnd, "ObjectPadProc");
        if(tcl == NULL) break;
        // Sterowanie wy˜wietlaniem podpowiedzi
        ObjControlTip(tcl, SHORT1FROMMP(mp1), 0);
      }
      break;
  }

  // Powr¢t do standardowej procedury okna
  return(WinDefWindowProc(hwnd, msg, mp1, mp2));
}





// Procedura dokonuje wst©pnej obr¢bki komunikat¢w napˆywaj¥cych do
// obiektu umieszczonego na pasku narz©dzi, i generuje komunikaty
// steruj¥ce wy˜wietlaniem podpowiedzi i wspomagaj¥ce
// sterowaniem przyciskami.
//
MRESULT EXPENTRY ObjectPassThru(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
  OBJCTL  *obj;     // Adresu struktury kontrolnej obiektu
  ULONG    id;      // Identyfikator okna
  HWND     parent;  // Uchwyt wˆa˜ciciela okna obiektu

  // Odczyt uchwytu okna rodzicielskiego i sprawdzenie jego poprawno˜ci
  { UCHAR classname[OBJ_CLASSIZE];      // Bufor zawieraj¥cy nazw© klasy

    parent = WinQueryWindow(hwnd, QW_PARENT);
    WinQueryClassName(parent, OBJ_CLASSIZE, classname);
    if(strcmp(classname, INTC_OBJECTPAD))
    { // Niewˆa˜ciwa klasa okna rodzicielskiego, powr¢t
      return(WinDefWindowProc(hwnd, msg, mp1, mp2));
    }
  }
  // Odczyt adresu struktury kontrolnej paska narz©dzi
  tcl = ToolLoadData(parent, "ObjectPassThru");
  if(tcl == NULL) return(WinDefWindowProc(hwnd, msg, mp1, mp2));
  // Odczyt wska«nika do obiektu
  id = WinQueryWindowUShort(hwnd, QWS_ID);
  obj = FindObjectID(tcl, id);
  if(obj == NULL) return(WinDefWindowProc(hwnd, msg, mp1, mp2));

  // Analiza komunikat¢w przeznaczonych dla obiekt¢w
  switch(obj->flAttribute & TBO_BASICTYPES)
  { case TBO_BUTTON:          // Przyciski WC_WINBUTTON
    case TBO_STDBUTTON:       // Standardowe przyciski systemowe
      // Uwzgl©dniane s¥ tylko przyciski w grupach TBO_CHECKBOX i TBO_RADIOBUTTON
      if(WinIsWindowEnabled(obj->hwndObject) == TRUE)
        switch(obj->flAttribute & TBO_RADIOBUTTON)
        { case TBO_RADIOBUTTON:
            if(msg == WM_BUTTON1DOWN)
            { // Zwolnienie pozostaˆych przycisk¢w w grupie
              WinPostMsg(parent, TM_OBJECTCONTROL, MPFROMLONG(id), MPFROMLONG(TNC_RELEASEGROUP));
            }

          case TBO_CHECKBOX:
            if(msg == WM_BUTTON1DOWN)
            { // Odczyt aktualnego stanu przycisku i zapami©tanie go
              if((LONG)obj->pfnObjProc(hwnd, BM_QUERYHILITE, 0, 0) == TRUE)
                obj->flAttribute |= TBO_BUTTONPRESS;
              else obj->flAttribute &= ~TBO_BUTTONPRESS;
            }
            if(msg == WM_BUTTON1UP)
            { // Blokada przerysowywania przycisku
              WinEnableWindowUpdate(hwnd, FALSE);
              // Wstawienie do kolejki komunikat¢w okna rodzicielskiego potwierdzenia
              WinPostMsg(parent, TM_OBJECTCONTROL, MPFROMLONG(id), MPFROMLONG(TNC_REDRAWBUTTON));
            }
        }

    case TBO_STDWINDOW:       // Pozostaˆe obiekty + przyciski
      if(msg == WM_MOUSEMOVE)
      { // Wysˆanie komunikatu rozpoczynaj¥cego wy˜wietlanie podpowiedzi
        if(tcl->lActiveTip != id)
          WinPostMsg(parent, TM_OBJECTCONTROL, MPFROMLONG(id), MPFROMLONG(TNC_STARTTIP));
      }
      break;
  }

//// Korekta bˆ©d¢w powstaj¥cych w stanie floating podczas zmiany ogniska
//if(msg == WM_QUERYFOCUSCHAIN)
//{ switch(SHORT1FROMMP(mp1))
//  { case QFC_PARTOFCHAIN:
//      return(MRFROMLONG(FALSE));
//
//  }
//}

  // Powr¢t do oryginalnej procedury okna obiektu
  return(obj->pfnObjProc(hwnd, msg, mp1, mp2));
}





// Usuni©cie obiektu z paska narz©dzi
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   objId     - [parametr] identyfikator obiektu
//
// Powr¢t:
//   FALSE - bˆad wykonania
//   TRUE  - obiekt usuni©to
//
LONG ObjDeleteObject(HWND hwnd, ULONG objId)
{ TOOLCTL *tcl;     // Wska«nik do struktur kontrolnych okna
  OBJCTL  *obj;     // Wska«nik do znalezionego obiektu
  LONG     objidx;  // Indeks usuwanego obiektu

  tcl = ToolLoadData(hwnd, "ObjDeleteObject");
  if(tcl == NULL) return(FALSE);

  // Odczyt wska«nika do obiektu
  obj = FindObjectID(tcl, objId);
  if(obj == NULL) return(FALSE);

  // Usuni©cie okna
  if(obj->hwndObject != NULLHANDLE)
    WinDestroyWindow(obj->hwndObject);
  // Usuni©cie wpisu z tablicy atom¢w (usuni©cie podpowiedzi)
  if(obj->aToolTip != 0)
    WinDeleteAtom(tcl->hatomTips, obj->aToolTip);

  // Reorganizacja tablicy obiekt¢w
  objidx = DeletePositionIndex(tcl, obj->iPosition);
  // Usuni©cie wpisu z tablicy obiekt¢w
  ShrinkObjectTable(tcl, objidx);

  // Aktualizacja rozmiar¢w paska narz©dzi
  if(!(tcl->flToolState & TST_MINIMIZED))
    if((LONG)WinSendMsg(tcl->hwndToolBar, TM_AUTOSIZE, 0, 0) == FALSE)
    { // Rozmieszczenie obiekt¢w
      ObjPlaceObjects(tcl->hwndObjectPad);
      // Wymuszenie przerysowania separator¢w
      WinInvalidateRect(tcl->hwndObjectPad, NULL, FALSE);
    }
  return(FALSE);
}





// Funkcja odczytuje identyfikator obieku na podstawie przekazanej pozycji
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   pos       - [parametr] pozycja obiektu
//
// Powr¢t:
//   0 - bˆ¥d odczytu
//   Identyfikator obiektu.
//
LONG ObjIdFromPosition(HWND hwnd, ULONG pos)
{ TOOLCTL *tcl;     // Wska«nik do struktur kontrolnych paska narz©dzi
  OBJCTL  *objtab;  // Wska«nik do tablicy definicji obiekt¢w

  tcl = ToolLoadData(hwnd, "ObjIdFromPosition");
  if(tcl == NULL) return(0);
  objtab = tcl->objtab;

  if(tcl->nObjects == 0) return(0);
  if(pos >= tcl->nObjects) pos = tcl->nObjects - 1;

  // Odczyt identyfikatora
  return(objtab[pos].id);
}





// Funkcja inicjuje podstawowe struktury danych kontroluj¥ce obiekty
// umieszczane na pasku narz©dzi
//
// Parametry:
//   tcl       - [parametr] adres gˆ¢wnej struktury kontrolnej paska narz©dzi
//
// Powr¢t:
//   FALSE     - poprawne wykonanie funkcji
//   TRUE      - bˆ¥d wykonania
//
LONG ObjInitialize(TOOLCTL *tcl)
{
  // Tworzenie lokalnej tablicy atom¢w dla podpowiedzi
  tcl->hatomTips = WinCreateAtomTable(0, 0);
  if(tcl->hatomTips == NULLHANDLE) return(TRUE);

  // Tworzenie okna przechowuj¥cego obiekty
  tcl->hwndObjectPad = WinCreateWindow(tcl->hwndToolBar, INTC_OBJECTPAD, "", 0,
                         0, 0, 0, 0,
                         tcl->hwndToolBar, HWND_TOP,
                         TCID_OBJECTPAD,
                         NULL, NULL);
  if(tcl->hwndObjectPad == NULLHANDLE) return(TRUE);

  // Zapami©tanie wska«nika do struktur kontrolnych
  WinSetWindowPtr(tcl->hwndObjectPad, 0L, tcl);
  return(FALSE);
}





// Funkcja dodaje okno potomne do paka narz©dzi, ustawiaj¥c jednocze˜nie
// odpowiednie atrybuty.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   tobj      - [parametr] wska«nik do struktury TOOLOBJ zawieraj¥cej atrybuty obiektu
//   twin      - [parametr] wska«nik do struktury TOOLWIN identyfikuj¥cej typ okna
//
// Powr¢t:
//   TRUE  - poprawne wykonanie funkcji
//   FALSE - bˆ¥d wykonania
//
LONG ObjInsertObject(HWND hwnd, TOOLOBJ *tobj, TOOLWIN *twin)
{ TOOLCTL *tcl;          // Wska«nik do gˆ¢wnych struktur kontrolnych okna
  OBJCTL  *obj;          // Wska«nik do struktury zawieraj¥cej dodawany obiekt
  LONG     winstyle;     // Skorygowany styl okna
  LONG     objidx;       // Indeks nowo utworzonego obiektu
  HWND     owner;        // Wˆa˜ciciel noweho okna potomnego

  // Odczyt adresu struktur kontrolnych
  tcl = ToolLoadData(hwnd, "ObjInsertObject");
  if(tcl == NULL) return(FALSE);

  // Sprawdzenie poprawno˜ci danych przekazywanych za po˜rednictwem struktur
  if(tobj == NULL) return(FALSE);
  // Sprawdzenie czy identyfikator nie powtarza si©
  if(FindObjectID(tcl, tobj->id) != NULL) return(FALSE);
  // Je˜li obiekt nie jest separatorem, wymaga dodatkowych struktur
  if((tobj->flAttribute & 0x0F) != TBO_SEPARATOR)
  { // Sprawdzenie czy przekazano wska«nik do tych struktur
    if(twin == NULL) return(FALSE);
  }

  // Alokacja pami©ci dla dodatkowej struktury kontrolnej
  objidx = ExpandObjectTable(tcl);
  if(objidx < 0) return(FALSE);
  // Okre˜lenie poˆo¾enia nowego obiektu
  objidx = InsertPositionIndex(tcl, objidx, tobj->iPosition);
  obj = tcl->objtab + objidx;

  // Wypeˆnianie struktury kontrolnej obiektu
  obj->ulGroup     = tobj->ulGroup;          // Grupa przycisk¢w
  obj->flAttribute = tobj->flAttribute;      // Zapami©tanie atrybut¢w
  obj->cx          = tobj->cx;               // Pocz¥tkowe wymiary okna
  obj->cy          = tobj->cy;
  obj->id          = tobj->id;               // Identyfikator okna
  obj->ulUser      = tobj->ulUser;           // Dane u¾ytkowika

  // Aktualizacja parametr¢w grupy
  UpdateGroupStyle(tcl, obj->ulGroup, tobj->flAttribute);
  // Aktualizacja sposobu wyr¢wnania
  UpdateAlignment(tcl, objidx, tobj->flAttribute);

  // Tworzenie okna potomnego
  if((tobj->flAttribute & TBO_BASICTYPES) != TBO_SEPARATOR)
  { // Okre˜lenie wˆa˜ciciela nowego obiektu
    owner = WinQueryWindow(tcl->hwndToolBar, QW_OWNER);

    // Korekta stylu okna
    winstyle  = twin->flStyle | WS_VISIBLE | WS_CLIPSIBLINGS;
    winstyle &= ~WS_DISABLED;
    if(tobj->flAttribute & TBO_HIDE)    winstyle &= ~WS_VISIBLE;
    if(tobj->flAttribute & TBO_DISABLE) winstyle |=  WS_DISABLED;

    // Tworzenie okna potomnego
    obj->hwndObject = WinCreateWindow(tcl->hwndObjectPad, twin->pszClass, twin->pszName,
                       winstyle,
                       0, 0, 0, 0,
                       owner, HWND_TOP, obj->id,
                       twin->pCtlData, twin->pPresParams);

    // Obsˆuga bˆ©du tworzenia okna
    if(obj->hwndObject == NULLHANDLE)
    { // Zmiana poˆo¾enia pozostaˆych obiekt¢w
      objidx = DeletePositionIndex(tcl, objidx);
      // Usuni©cie struktur kontrolnych
      ShrinkObjectTable(tcl, objidx);
      return(FALSE);
    }

    // Zamiana procedury okna
    obj->pfnObjProc = WinSubclassWindow(obj->hwndObject, ObjectPassThru);
  }
  else obj->hwndObject = NULLHANDLE;

  // Aktualizacja rozmiar¢w okna
  if(!(tcl->flToolState & TST_MINIMIZED))
    if((LONG)WinSendMsg(tcl->hwndToolBar, TM_AUTOSIZE, 0, 0) == FALSE)
    { // Rozmieszczenie obiekt¢w
      ObjPlaceObjects(tcl->hwndObjectPad);
      // Wymuszenie przerysowania separator¢w
      WinInvalidateRect(tcl->hwndObjectPad, NULL, FALSE);
    }
  return(TRUE);
}




// Funkcja dodaje lub usuwa podpowied« do obiektu o podanym identyfikatorze,
// umieszczonym na pasku narz©dzi.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   objId     - [parametr] identyfikator obiektu
//   tip       - [parametr] dodawany ci¥g znak¢w
//
// Powr¢t:
//   TRUE  - poprawne wykonanie
//   FALSE - bˆad wykonanie
//
LONG ObjInsertTooltip(HWND hwnd, ULONG objId, PSZ tip)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury kontrolnej okna
  OBJCTL  *obj;     // Wska«nik do struktury kontrolnej obiektu

  // Inicjacja zmiennych
  tcl = ToolLoadData(hwnd, "ObjInsertToolTip");
  if(tcl == NULL) return(FALSE);
  obj = FindObjectID(tcl, objId);
  if(obj == NULL) return(FALSE);

  // Sprawdzneie czy jest to separator
  if((obj->flAttribute & TBO_BASICTYPES) == TBO_SEPARATOR) return(FALSE);

  // Usuni©cie tekstu podpowiedzi
  if(obj->aToolTip != 0)
  { WinDeleteAtom(tcl->hatomTips, obj->aToolTip);
    obj->aToolTip = 0;
  }

  // Dodanie nowego tekstu podpowiedzi
  if(tip != NULL)
    if(*tip != 0)
      obj->aToolTip = WinAddAtom(tcl->hatomTips, tip);
  return(TRUE);
}





// Funkcja odczytuje uchwyt okna nale¾¥cy do obiektu o podanym identyfikatorze.
//
// Paramery:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   id        - [parametr] identyfikator obiektu
//
// Powr¢t:
//   Uchwyt okna
//   NULLHANDLE - obiekt jest separatorem lub nie istnieje
//
HWND ObjQueryHandle(HWND hwnd, ULONG id)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury kontrolnej okna
  OBJCTL  *obj;     // Wska«nik do definicji ukrywanego obiektu

  tcl = ToolLoadData(hwnd, "ObjQueryHandle");
  if(tcl == NULL) return(NULLHANDLE);
  obj = FindObjectID(tcl, id);
  if(obj == NULL) return(NULLHANDLE);
  return(obj->hwndObject);
}





// Funkcja przenosi obiekt z dotychczasowej pozycji na now¥,
// umo¾liwiaj¥c jednocze˜nie zmian© typu wyr¢wnania. Atrybuty flAttrs
// mog¥ przyjmowa† warto˜ci: TBO_BEGINALIGN, TBO_ENDALIGN lub TBO_CENTERALIGN.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   objId     - [parametr] identyfikator obiektu
//   newpos    - [parametr] nowa pozycja obiektu
//   flAttrs   - [parametr] nowe parametry steruj¥ce wyr¢wnaniem
//
// Powr¢t:
//   TRUE  - poprawne wykonanie
//   FALSE - bˆ¥d
//
LONG ObjMoveObject(HWND hwnd, ULONG objId, ULONG newpos, ULONG flAttrs)
{ TOOLCTL *tcl;     // Adres struktur kontrolnych paska narz©dzi
  OBJCTL  *obj;     // Adres struktury kontrolnej przemieszczanego obiektu
  OBJCTL  *objtab;  // Adres pocz¥tku tablicy definicji obiekt¢w
  LONG     objidx;  // Nowy indeks obiektu powstaˆy w wyniku jego przemieszczenia

  // Inicjacja zmiennych
  tcl = ToolLoadData(hwnd, "ObjMoveObject");
  if(tcl == NULL) return(FALSE);
  obj = FindObjectID(tcl, objId);
  if(obj == NULL) return(FALSE);
  objtab = tcl->objtab;

  // Sprawdzenie czy w tabeli s¥ obiekty
  if(tcl->nObjects == 0) return(FALSE);
  // Korekta pozycji docelowej
  if(newpos >= tcl->nObjects) newpos = tcl->nObjects - 1;

  // Sprawdzenie czy obiekt nie jest ju¾ w pozycji docelowej
  if(newpos != obj->iPosition)
  { // Ustalenie nieprawidˆowej warto˜ci pozycji w tabeli dla przemieszczanego obiektu
    objidx = DeletePositionIndex(tcl, obj->iPosition);
    // Przesuni©cie obiektu na pozycj© docelow¥
    objidx = InsertPositionIndex(tcl, objidx, newpos);
  }

  // Korekta justowania obiektu
  UpdateAlignment(tcl, objidx, flAttrs);

  // Rozmieszczenie obiekt¢w
  if(!(tcl->flToolState & TST_MINIMIZED))
  { ObjPlaceObjects(tcl->hwndObjectPad);
    // Wymuszenie przerysowania separator¢w
    WinInvalidateRect(tcl->hwndObjectPad, NULL, FALSE);
  }
  return(TRUE);
}





// Funkcja odczytuje wysoko˜† obszaru w kt¢rym znajduj¥ si© obiekty,
// niezb©dn¥ do ich umieszczenia.
//
// Parametry:
//   tcl        - [parametr] wska«nik do gˆ¢wnej struktury kontrolnej paska anrz©dzi
//   flNewState - [parametr] stan paska o kt¢ry pytamy
//
// Powr¢t:
//   Szeroko˜† obszaru.
//
LONG ObjQueryHeight(TOOLCTL *tcl, ULONG flNewState)
{ OBJCTL  *objtab;  // Tablica definicji obiekt¢w
  LONG     height;  // Obliczona wysoko˜† okna
  LONG i, tmp;      // Zmienne pomocnicze

  // Inicjacja zmiennych
  objtab = tcl->objtab;
  // Aktualizacja szeroko˜ci obiekt¢w
  CalcObjectSize(tcl);

  for(i = 0, height = 0; i < tcl->nObjects; ++i)
  { if((objtab[i].flAttribute & TBO_BASICTYPES) == TBO_SEPARATOR)
    { // Separator - szeroko˜† zale¾y od poˆo¾enia paska narz©dzi
      tmp = flNewState & TST_ROTATED ? objtab[i].cy : 0;
    }
    else
    { // Pozostaˆe obiekty na pasku narz©dzi
      tmp = objtab[i].cy;
    }
    // Sprawdzenie czy obiekt jest widoczny, i je˜li nie - blokada odczytu
    if(objtab[i].flAttribute & TBO_HIDE) tmp = 0;

    if(flNewState & TST_ROTATED)
    { // Pasek umieszczony pionowo - odczyt sumy wysoko˜ci wszystkich obiekt¢w
      height += tmp;
      if((i < (tcl->nObjects - 1)) && !(objtab[i].flAttribute & TBO_HIDE))
        height += CXY_OBJECTSPACE;
    }
    else
    { // Pasek umieszczony poziomo - odczyt maksymalnej wysoko˜ci
      if(height < tmp) height = tmp;
    }
  }

  // Ustawienie domy˜lnej wysoko˜ci okna (gdy nie ma na nim obiekt¢w)
  if(!height) height = 25;
  // Powr¢t - odczytana wysoko˜† okna
  return(height);
}





// Odczyt parametr¢w obuiektu
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi,
//   objId     - [parametr] identyfikator obiektu,
//   tobj      - [rezultat] wska«nik do struktury informacyjnej obiektu
//
// Powr¢t:
//   FALSE - bˆ¥d odczytu,
//   TRUE  - poprawny odczyt danych
//
LONG ObjQueryObject(HWND hwnd, ULONG objId, TOOLOBJ *tobj)
{ TOOLCTL *tcl;     // Adres struktury kontrolnej paska narz©dzi
  OBJCTL  *obj;     // Wska«nik do struktury kontrolnej obiketu

  // Inicjacja zmiennych
  tcl = ToolLoadData(hwnd, "ObjQueryObject");
  if(tcl == NULL) return(FALSE);
  obj = FindObjectID(tcl, objId);
  if(obj == NULL) return(FALSE);

  // Kopiowanie p¢l struktury
  tobj->iPosition   = obj->iPosition;
  tobj->ulGroup     = obj->ulGroup;

  // Tylko atrybuty przeznaczone dla u¾ytkownika
  tobj->flAttribute = obj->flAttribute & 0xFFFF;
  // Aktualizacja znacznika blokady okna
  tobj->flAttribute &= ~TBO_DISABLE;
  if(obj->hwndObject != NULLHANDLE)
    if(WinIsWindowEnabled(obj->hwndObject) == FALSE)
       tobj->flAttribute |= TBO_DISABLE;

  // Pozostaˆe pola struktury
  tobj->cx          = obj->cx;
  tobj->cy          = obj->cy;
  tobj->id          = obj->id;
  tobj->ulUser      = obj->ulUser;
  return(TRUE);
}





// Funkcja odczytuje liczn© obiekt¢w umieszczonych na pasku narz©dzi.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dz
//
// Powr¢t:
//   Liczba obiekt¢w umieszczonych na pasku
//
LONG ObjQueryObjectCount(HWND hwnd)
{ TOOLCTL *tcl;     // Adres gˆ¢wnej struktury kontrolnej paska narz©dzi

  tcl = ToolLoadData(hwnd, "ObjQueryObjectCout");
  if(tcl == NULL) return(0);
  return(tcl->nObjects);
}





// Funkcja odczytuje poˆo¾enie obiektu na podstawie identyfikatora
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   objId     - [parametr] identyfikator obiektu
//
// Powr¢t:
//   >0 - poˆo¾enie obiektu
//   -1 - bˆ¥d
//
LONG ObjQueryPosition(HWND hwnd, ULONG objId)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
  OBJCTL  *obj;     // Wska«nik do obiektu

  // Inicjacja zmiennych
  tcl = ToolLoadData(hwnd, "ObjQueryPosition");
  if(tcl == NULL) return(-1);
  obj = FindObjectID(tcl, objId);
  if(obj == NULL) return(-1);

  // Odczytana pozycja
  return(obj->iPosition);
}





// Odczyt tekstu podpowiedzi do obiektu
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   objId     - [parametr] identyfikator obiektu
//   ulBufSize - [parametr] rozmiar bufora TipText
//   TipText   - [rezultat] odczytany tekst pdpowiedzi
//
// Rezultaty:
//   Dˆugo˜† tekstu podpowiedzi bez uzgl©dnienia koäcowego znaku 0.
//
LONG ObjQueryTip(HWND hwnd, ULONG objId, ULONG ulBufSize, PSZ TipText)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
  OBJCTL  *obj;     // Adres struktury kontrolnej obiektu
  LONG tip;         // Dˆugo˜† tekstu podpowiedzi

  // Inicjacja zmiennych
  tcl = ToolLoadData(hwnd, "ObjQueryTip");
  if(tcl == NULL) return(0);
  obj = FindObjectID(tcl, objId);
  if(obj == NULL) return(0);

  // Sprawdzenie czy jest tekstu podpowiedzi
  if(obj->aToolTip == 0) return(0);
  // Odczyt dˆugo˜ci tekstu
  tip = WinQueryAtomLength(tcl->hatomTips, obj->aToolTip);
  if(tip && (ulBufSize != 0) && (TipText != NULL))
  { // Odczyt tekstu
    WinQueryAtomName(tcl->hatomTips, obj->aToolTip, TipText, ulBufSize);
  }

  // Odczytana dˆugo˜c tekstu
  return(tip);
}





// Funkcja odczytuje szeroko˜† obszaru w kt¢rym znajduj¥ si© obiekty,
// niezb©dn¥ do ich umieszczenia.
//
// Parametry:
//   tcl        - [parametr] wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
//   flNewState - [parametr] stan paska o kt¢ry pytamy
//
// Powr¢t:
//   Szeroko˜† obszaru.
//
LONG ObjQueryWidth(TOOLCTL *tcl, ULONG flNewState)
{ OBJCTL  *objtab;  // Tablica definicji obiekt¢w
  LONG     width;   // Obliczona szeroko˜† okna
  LONG i, tmp;      // Zmienne pomocnicze

  // Inicjacja zmiennych
  objtab = tcl->objtab;
  // Obliczenie rozmiar¢w obiekt¢w
  CalcObjectSize(tcl);

  for(i = 0, width = 0; i < tcl->nObjects; ++i)
  { if((objtab[i].flAttribute & TBO_BASICTYPES) == TBO_SEPARATOR)
    { // Separator - szeroko˜† zale¾y od poˆo¾enia paska narz©dzi
      tmp = flNewState & TST_ROTATED ? 0 : objtab[i].cx;
    }
    else
    { // Pozostaˆe obiekty na pasku narz©dzi
      tmp = objtab[i].cx;
    }
    // Sprawdzenie czy obiekt jest widoczny, i je˜li nie - blokada odczytu
    if(objtab[i].flAttribute & TBO_HIDE) tmp = 0;

    if(flNewState & TST_ROTATED)
    { // Pasek umieszczony pionowo - odczyt maksymalnej szeroko˜ci
      if(width < tmp) width = tmp;
    }
    else
    { // Pasek umieszczony poziomo - odczyt sumy szeroko˜ci wszystkich obiekt¢w
      width += tmp;
      if((i < (tcl->nObjects - 1)) && !(objtab[i].flAttribute & TBO_HIDE))
        width += CXY_OBJECTSPACE;
    }
  }

  // Ustawienie domy˜lnej szeroko˜ci okna (gdy nie ma na nim obiekt¢w)
  if(!width) width = 25;
  // Powr¢t - odczytana szeroko˜† okna
  return(width);
}





// Funkcja oczyszczaj¥ca struktury danych paska narz©dzi podczas obsˆugi WM_DESTROY.
// Zostaje zwolniona pami©†, tablice atom¢w, oraz zostaj¥ zniszczone okna potomne.
//
// Parametry:
//   tcl       - [parametr] adres gˆ¢wnej struktury kontrolnej paska narz©dzi
//
VOID ObjReleaseResources(TOOLCTL *tcl)
{
  // Usuni©cie tablicy atom¢w przechowuj¥cej podpowiedzi
  if(tcl->hatomTips != NULLHANDLE)
    WinDestroyAtomTable(tcl->hatomTips);

  // Usuni©cie okna przechowuj¥cego obiekty
  if(tcl->hwndObjectPad != NULLHANDLE)
    WinDestroyWindow(tcl->hwndObjectPad);

  // Zwolnienie pami©ci zajmowanej przez tablic© obiekt¢w
  if(tcl->objtab != NULL) free(tcl->objtab);
}





// Funkcja zmienia parametry wybranego obiektu na podstawie danych przekazanych
// za po˜rednictwem struktury TOOLOBJ, oraz zmiennej ulOptions.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   objId     - [parametr] identyfikator obiektu
//   ulOptions - [parametr] opcje umo¾liwiaj¥ce wyb¢r wa¾nych p¢l struktury tobj
//   tobj      - [parametr] struktura zawieraj¥ca moyfikowane parametry
//
// Powr¢t:
//   FALSE - bˆ¥d wykonania
//   TRUE  - zmiana parametr¢w zostaˆa wykonana poprawnie
//
LONG ObjSetObject(HWND hwnd, ULONG objId, ULONG ulOptions, TOOLOBJ *tobj)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
  OBJCTL  *obj;     // Wska«nik do struktury kontrolnej wybranego obiektu

  // Inicjacja zmiennych
  tcl = ToolLoadData(hwnd, "ObjSetObject");
  if(tcl == NULL) return(FALSE);
  obj = FindObjectID(tcl, objId);
  if(obj == NULL) return(FALSE);

  // Zmiana przynale¾no˜ci do grupy
  if((ulOptions & TSO_GROUP) && (obj->ulGroup != tobj->ulGroup))
  { OBJCTL *objtab;      // Adres pocz¥tku tabeli zawieraj¥cej definicje obiekt¢w
    LONG i;

    // Inicjacja zmiennych
    objtab = tcl->objtab;
    obj->flAttribute &= ~TBO_RADIOBUTTON;
    // Odczyt parametr¢w grupy (je˜li s¥)
    for(i = 0; i < tcl->nObjects; ++i)
      if(objtab[i].ulGroup == tobj->ulGroup)
      { // Nadanie parametr¢w obiektowi
        obj->flAttribute |= (objtab[i].ulGroup & TBO_RADIOBUTTON);
        break;
      }
    // Zmiana numeru grupy
    obj->ulGroup = tobj->ulGroup;
  }

  // Zmiana wymiar¢w obiektu
  if(ulOptions & TSO_SIZE)
  { obj->cx = tobj->cx;
    obj->cy = tobj->cy;
    // Korekta wymiar¢w okna
    if(!(tcl->flToolState & TST_MINIMIZED))
      if((LONG)WinSendMsg(tcl->hwndToolBar, TM_AUTOSIZE, 0, 0) == FALSE)
      { // Pozycjonowanie obiekt¢w w przypadku gdy wymiary okna nie zmieniˆy si©
        ObjPlaceObjects(tcl->hwndObjectPad);
        // Wymuszenie przerysowania separator¢w
        WinInvalidateRect(tcl->hwndObjectPad, NULL, FALSE);
      }
  }

  // Zmiana pola u¾ytkownika
  if(ulOptions & TSO_USER)
  { obj->ulUser = tobj->ulUser;
  }
  return(TRUE);
}





// Funkcja ukrywa lub wy˜wietla wybrany obiekt umieszczony na pasku narz©dzi.
// Gdy bNewState = TRUE obiekt jest wy˜wietlany, gdy FALSE - jest ukrywany.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   id        - [parametr] identyfikator obiektu
//   bNewState - [parametr] nowy stan obiektu
//
// Powr¢t:
//   TRUE  - poprawne wykonanie funkcji
//   FALSE - bˆ¥d wykonania
//
LONG ObjShowObject(HWND hwnd, ULONG id, ULONG bNewState)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury kontrolnej okna
  OBJCTL  *obj;     // Wska«nik do definicji ukrywanego obiektu

  tcl = ToolLoadData(hwnd, "ObjShowObject");
  if(tcl == NULL) return(FALSE);
  obj = FindObjectID(tcl, id);
  if(obj == NULL) return(FALSE);

  if(bNewState)
  { // Wy˜wietlenie obiektu
    obj->flAttribute &= ~TBO_HIDE;
    // Aktywacja okna, je˜li poprzednio byˆo ukryte
    if((obj->flAttribute & TBO_BASICTYPES) != TBO_SEPARATOR)
      if(WinIsWindowVisible(obj->hwndObject) == FALSE)
        WinShowWindow(obj->hwndObject, TRUE);
  }
  else
  { // Ukrycie obiektu
    obj->flAttribute |=  TBO_HIDE;
    // Dezaktywacja okna je˜li poprzednio byˆo widoczne
    if((obj->flAttribute & TBO_BASICTYPES) != TBO_SEPARATOR)
      if(WinIsWindowVisible(obj->hwndObject) == TRUE)
        WinShowWindow(obj->hwndObject, FALSE);
  }

  // Aktualizacja rozmiar¢w okna
  if(!(tcl->flToolState & TST_MINIMIZED))
    if((LONG)WinSendMsg(tcl->hwndToolBar, TM_AUTOSIZE, 0, 0) == FALSE)
    { // Rozmieszczenie obiekt¢w
      ObjPlaceObjects(tcl->hwndObjectPad);
      // Wymuszenie przerysowania separator¢w
      WinInvalidateRect(tcl->hwndObjectPad, NULL, FALSE);
    }
  return(TRUE);
}





//*******************/
//* Funkcje lokalne */
//*******************/

// Funkcja steruje zachowaniem przycisk¢w umieszczonych na pasku narz©dzi,
// oraz wy˜wietlaniem tekst¢w podpowiedzi do obiekt¢w
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna przechowuj¥cego obiekty
//   id        - [parametr] identyfikator obiektu
//   command   - [parametr] czynno˜† do wykonania
//
// Rezultaty:
//   0    - stanadardowe zakoäczenie wykonania
//
static LONG ObjControlObjects(HWND hwnd, ULONG id, ULONG command)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
  OBJCTL  *obj;     // Wska«nik do struktury kontrolnej obiektu
  OBJCTL  *objtab;  // Wska«nik do tablicy definicji obiekt¢w
  LONG i;           // Zmienne pomocnicze

  // Inicjacja zmiennych
  tcl = ToolLoadData(hwnd, "ObjControlObjects");
  if(tcl == NULL) return(0);
  obj = FindObjectID(tcl, id);
  if(obj == NULL) return(0);
  objtab = tcl->objtab;

  // Realizacja czynno˜ci
  switch(command)
  { case TNC_REDRAWBUTTON:         // Sterowanie przyciskami
      { BOOL newstate;        // Nowy stan przycisku

        // Zmiana wygl¥du przycisku
        if(obj->flAttribute & TBO_BUTTONPRESS)
        { if((obj->flAttribute & TBO_RADIOBUTTON) == TBO_CHECKBOX)
            newstate = FALSE;
        }
        else newstate = TRUE;

        // Zmiana stanu okna
        WinSendMsg(obj->hwndObject, BM_SETHILITE, MPFROMLONG(newstate), 0);
        // Wymuszenie przerysowania przycisku
        WinEnableWindowUpdate(obj->hwndObject, TRUE);
      }
      break;

    case TNC_RELEASEGROUP:    // Zwolnienie wszystkich przycisk¢w w grupie TBO_RADIOBUTTON
      for(i = 0; i < tcl->nObjects; ++i)
      { if((objtab[i].ulGroup == obj->ulGroup) && (objtab[i].id != obj->id))
          switch(objtab[i].flAttribute & TBO_BASICTYPES)
          { case TBO_STDBUTTON:
            case TBO_BUTTON:
              // Sprawdzenie stanu przycisku
              if((LONG)WinSendMsg(objtab[i].hwndObject, BM_QUERYHILITE, 0, 0) == TRUE)
                // Wyˆ¥czenie przycisku
                WinSendMsg(objtab[i].hwndObject, BM_SETHILITE, MPFROMLONG(FALSE), 0);
          }
      }
      break;

    case TNC_STARTTIP:        // Rozpocz©cie procedury wy˜wietlania podpowiedzi
      ObjControlTip(tcl, 0, id);
      break;
  }
  return(0);
}





// Funkcja steruje zale¾no˜ciami czasowymi podczas wy˜wietlania tekstu
// podpowiedzi do obiekt¢w.
//
// Parametry:
//   tcl       - [parametr] wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
//   timer     - [parametr] identyfikator czasomierza
//   id        - [parametr] identyfikator obiektu ¾¥daj¥cego podpowiedzi
//
static VOID ObjControlTip(TOOLCTL *tcl, ULONG timer, ULONG id)
{ POINTL cpos;      // Pozycja kursora
  LONG   delay;     // Czas op¢«nienia przed wy˜wietleniem podpowiedzi
  HWND   hwndc;     // Uchwyt okna znajduj¥cego si© pod kursorem
  HAB    hab;       // Uchwyt anchor block PM

  // Inicjacja zmiennych
  hab = WinQueryAnchorBlock(tcl->hwndToolBar);

  // Zgˆoszenie ¾¥dania wy˜wietlenia podpowiedzi
  if(id)
  { // Zapami©tanie identyfikatora
    tcl->lActiveTip = id;
    // Wyzwolenie czasomierza umo¾liwiaj¥cego wykrycie zej˜cia kursora z paska narz©dzi
    WinStartTimer(hab, tcl->hwndObjectPad, TMR_TIPREMOVE, TIME_TIPREMOVE);

    // Odczyt czasu op¢«nienia przed wˆ¥czeniem podpowiedzi
    if(!WinQueryPresParam(tcl->hwndToolBar, PP_TOOLTIPDELAY, 0L, NULL, 4, &delay, 0L))
      delay = TIME_TIPDELAY;
    // Wyzwolenie czasomierza odmierzaj¥cego czas op¢«nienia przed wy˜wietleniem podpowiedzi
    WinStartTimer(hab, tcl->hwndObjectPad, TMR_TIPDELAY, delay);

    // Sprawdzenie czy podpowied« mo¾na wy˜wietli† natychmiast
    if(tcl->flToolState & TST_TIPACTIVATED)
    { // Wy˜wietlenie tekstu podpowiedzi
      ObjDisplayTip(tcl, tcl->lActiveTip);
    }
  }
  else if(timer)
  { // Odczyt pozycji kursora
    WinQueryPointerPos(HWND_DESKTOP, &cpos);
    WinMapWindowPoints(HWND_DESKTOP, tcl->hwndObjectPad, &cpos, 1);

    // Blokada pbsˆugi komunikatu WM_HITTEST
    tcl->flToolState |= TST_HITTEST;
    // Odczyt uchwytu okna znajduj¥cego si© pod kursorem
    hwndc = WinWindowFromPoint(tcl->hwndObjectPad, &cpos, FALSE);
    // Przywr¢cenie normalnej obsˆugi komunikatu WM_HITTEST
    tcl->flToolState &= ~TST_HITTEST;

    if(hwndc == NULLHANDLE)
    { // Wyˆ¥czenie podpowiedzi - kursor znajduje si© poza paskiem narz©dzi
      WinStopTimer(hab, tcl->hwndObjectPad, TMR_TIPREMOVE);
      WinStopTimer(hab, tcl->hwndObjectPad, TMR_TIPDELAY);
      tcl->flToolState &= ~TST_TIPACTIVATED;
      if(tcl->lActiveTip != 0)
      { tcl->lActiveTip   = 0;
        // Wyˆ¥czenie podpowiedzi
        WinShowWindow(tcl->hwndToolTip, FALSE);
      }
    }
    else if(hwndc == tcl->hwndObjectPad)
    { // Kursor znajduje si© na pasku pomi©dzy obiektami
      WinStopTimer(hab, tcl->hwndObjectPad, TMR_TIPDELAY);
      if(tcl->lActiveTip != 0)
      { tcl->lActiveTip = 0;
        // Wyˆ¥czenie podpowiedzi
        WinShowWindow(tcl->hwndToolTip, FALSE);
      }
    }
    else
    { // Kursor znajduje si© nad jednym z obiekt¢w
      if((timer == TMR_TIPDELAY) && !(tcl->flToolState & TST_TIPACTIVATED))
      { tcl->flToolState |= TST_TIPACTIVATED;
        // Wy˜wietlenie tekstu podpowiedzi
        ObjDisplayTip(tcl, tcl->lActiveTip);
      }
    }
  }
}





// Funkcja wy˜wietla podpowied« do obiektu
//
// Parametry:
//   tcl       - [parametr] adres gˆ¢wnej struktury kontrolnej paska narz©dzi
//   id        - [parametr] identyfikator obiektu
//
// Powr¢t:
//   NULL      - nie ma tekstu podpowiedzi
//   <>NULL    - wska«nik do ci¥gu ASCIIZ zawieraj¥cego podpowied«
//
static VOID ObjDisplayTip(TOOLCTL *tcl, ULONG id)
{ OBJCTL *obj;      // Adres struktury kontrolnej obiketu
  LONG    txtl;     // Dˆugo˜† odczytywanego tekstu
  PSZ     tiptext;  // Wska«nik do obszaru danych zawieraj¥cego tekst podpowiedzi

  // Odczyt adresu obiektu
  obj = FindObjectID(tcl, id);
  if(obj != NULL)
    // Sprawdzenie czy jest tekst podpowiedzi
    if(obj->aToolTip != 0)
    {
      // Odczyt dˆugo˜ci tekstu podpowiedzi
      txtl = WinQueryAtomLength(tcl->hatomTips, obj->aToolTip);
      if(txtl)
      { // Alokacja pami©ci dla tekstu
        tiptext = malloc(txtl + 1);
        if(tiptext != NULL)
        { // Odczyt tekstu podpowiedzi
          WinQueryAtomName(tcl->hatomTips, obj->aToolTip, tiptext, txtl + 1);
          tiptext[txtl] = 0;

          // Wy˜wietlenie podpowiedzi
          ToolDisplayTip(tcl->hwndToolBar, obj->hwndObject, tiptext);
          // Zwolnienie pami©ci
          free(tiptext);
          return;
        }
      }
    }
  // Usuni©cie podpowiedzi w przypadku niepowodzenia
  WinShowWindow(tcl->hwndToolTip, FALSE);
}





// Funkcja nadaje przezroczysto˜c dla komunikat¢w myszy dla okna TCID_OBJECTPAD.
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna TCID_OBJECTPAD
//   cpos      - [parametr] pozycja kursora myszy
//
// Powr¢t:
//   HT_TRANSPARENT - kursor jest nad oknem TCID_OBJECTPAD
//   HT_NORMAL      - kursor jest nad niezablokowanym oknem
//   HT_ERROR       - kursor jest nad zablokownym obiektem
//
static LONG ObjHittest(HWND hwnd, POINTS *cpos)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury steruj¥cej paska narz©dzi
  POINTL   pos;     // Pozycja kursora
  HWND     hobj;    // Uchwyt okna nad kt¢rym jest kursor

  // Odczyt adresu struktury kontrolnej
  tcl = ToolLoadData(hwnd, "ObjHittest");
  if(tcl == NULL) return(HT_NORMAL);

  if(!(tcl->flToolState & TST_HITTEST))
  { // Inicjacja zmiennych
    pos.x = (LONG)((SHORT)cpos->x);
    pos.y = (LONG)((SHORT)cpos->y);

    // Zaznaczenie, ¾e trwa przetwarzanie WinWindowFromPoint
    tcl->flToolState |= TST_HITTEST;
    // Odczyt uchwytu okna nad kt¢rym jest kursor
    hobj = WinWindowFromPoint(tcl->hwndObjectPad, &pos, FALSE);
    // Usuni©cie znacznika
    tcl->flToolState &= ~TST_HITTEST;

    // Kursor jest nad oknem TCID_OBJECTPAD
    if(hobj == hwnd) return(HT_TRANSPARENT);
    // Kursor jest nad jednym z okien potomnych
    if(hobj != NULLHANDLE)
    { // Sprawdzenie czy obiekt jest zablokowany
      if(WinIsWindowEnabled(hobj) == FALSE) return(HT_ERROR);
      else                                  return(HT_NORMAL);
    }
  }
  // Sytuacje nieprzewidziane
  return(HT_NORMAL);
}





// Pozycjonowanie okien potomnych
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna przechowuj¥cego obiekty
//
static VOID ObjPlaceObjects(HWND hwnd)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
  OBJCTL  *objtab;  // Wskz«nik do pocz¥tku tablicy obiekt¢w
  RECTL    area;    // Obszar okna
  LONG     length;  // Minimalna dˆugo˜† cz©˜ci u¾ytkowej paska narz©dzi
  LONG i;           // Zmienne pomocnicze

  tcl = ToolLoadData(hwnd, "ObjPlaceObjects");
  if(tcl == NULL) return;
  // Inicjacja struktury TOOLADJ
  objtab = tcl->objtab;
  // Odczyt wymiar¢w okna
  WinQueryWindowRect(hwnd, &area);

  // Odczyt minimalnej dˆugo˜ci u¾ytkowej cz©˜ci paska narz©dzi
  // Funkcje automatycznie uaktualniaj¥ te¾ wymiary obiekt¢w w tablicach
  if(tcl->flToolState & TST_ROTATED)
  { length = ObjQueryHeight(tcl, tcl->flToolState);
    i = area.yTop - area.yBottom;
  }
  else
  { length = ObjQueryWidth(tcl, tcl->flToolState);
    i = area.xRight - area.xLeft;
  }

  // Sprawdzenie czy obiekty mieszcz¥ si© w obszarze *area
  if(length > i)
  { // Standardowe pozycjonowanie niemieszcz¥cych si© obiekt¢w - obci©cie
    PlaceEndcut(tcl, &area);
  }
  else
    // Standardowe rozmieszczenie element¢w (wszystko si© mie˜ci)
    PlaceStandard(tcl, &area);

  // Sprawdzenie czy pasek nie jest zminimalizowany
  if(!(tcl->flToolState & TST_MINIMIZED))
  { // Pozycjonowanie okien na podstawie danych zapisanych w tablicach
    for(i = 0; i < tcl->nObjects; ++i)
      // PD: Hack for WC_SPINBUTTON
      if((objtab[i].flAttribute & TBO_BASICTYPES) != TBO_SEPARATOR)
      { CHAR achClass[64];
        ULONG lRet;

        memset(&achClass,0,sizeof(achClass));
        // Zapytanie jak¥ klas¥ jest obiekt
        lRet=WinQueryClassName(objtab[i].hwndObject, sizeof(achClass), &achClass);
#if 0
        if (!strcmp(achClass,"#32"))  // SPINBUTTON
        {  ULONG ulTemp=(area.yTop - area.yBottom)/2 - 10;
           WinSetWindowPos(objtab[i].hwndObject, HWND_TOP,
                           objtab[i].x,  ulTemp,
                           tcl->flToolState & TST_ROTATED ? area.xRight - area.xLeft : objtab[i].cx,
                           tcl->flToolState & TST_ROTATED ? objtab[i].cy : area.yTop - area.yBottom,
                           SWP_MOVE | SWP_SIZE | SWP_ZORDER | SWP_NOADJUST);
        
        }
        else
#endif
        {

          WinSetWindowPos(objtab[i].hwndObject, HWND_TOP,
                          objtab[i].x,  objtab[i].y,
                          tcl->flToolState & TST_ROTATED ? area.xRight - area.xLeft : objtab[i].cx,
                          tcl->flToolState & TST_ROTATED ? objtab[i].cy : area.yTop - area.yBottom,
                          SWP_MOVE | SWP_SIZE | SWP_ZORDER | SWP_NOADJUST);
       }
     }
  }
}





// Funkcja przerysowuje obiekty (separatory) umieszczone na pasku narz©dzi,
// oraz przerwy mi©dzy obiektami zdefiniowane jako CXY_OBJECTSPACE
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna przechowuj¥cego obiekty paska narz©dzi
//
static VOID ObjRedrawObjects(HWND hwnd)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
  OBJCTL  *objtab;  // Wska«nik do tablicy definicji obiekt¢w
  RECTL    area;    // Wymiary okna
  HPS      hps;     // Uchwyt presentation space
  LONG i;           // Zmienne pomocnicze

  tcl = ToolLoadData(hwnd, "ObjRedrawObjects");
  if(tcl == NULL) return;
  // Obiekty nie s¥ rysowane na zminimalizowanym pasku narz©dzi
  if(tcl->flToolState & TST_MINIMIZED) return;

  // Rozpocz©cie rysowania
  hps = WinBeginPaint(hwnd, NULL, NULLHANDLE);
  // adowanie tablicy kolor¢w
  PpmQueryPresColors(tcl->hwndToolBar, TOOL_MAXCOLOR, PPmColor, tcl->colors);
  // Tworzenie logicznej tablicy kolor¢w
  GpiCreateLogColorTable(hps, 0L, LCOLF_CONSECRGB, 0L, TOOL_MAXCOLOR, tcl->colors);
  // Odczyt wymiar¢w paska narz©dzi
  WinQueryWindowRect(hwnd, &area);

  // Kasowanie tˆa
  GpiSetColor(hps, TOOL_BACKGROUND /*TOOL_TIPBACKGROUND*/);
  GpiMove(hps, (POINTL *)&area);
  GpiBox(hps, DRO_FILL, ((POINTL *)&area) + 1, 0, 0);

  // Inicjacja zmiennych
  objtab = tcl->objtab;
  // Rysowanie separator¢w
  for(i = 0; i < tcl->nObjects; ++i)
    if(!(objtab[i].flAttribute & TBO_HIDE))
      if((objtab[i].flAttribute & TBO_BASICTYPES) == TBO_SEPARATOR)
        // Rysowanie separatora
        DrawSeparator(hps, objtab + i, &area, tcl->flToolState);

  // Koniec rysowania
  WinEndPaint(hps);
}





/******************************/
/* Dodatkowe funkcje usˆugowe */
/******************************/

// Funkcja oblicza pocz¥tkowe rozmiary przycisk¢w WC_WINBUTTON
// oraz separator¢w i umieszcza je w polach cx i cy tablicy struktur OBJCTL.
//
// Parametry:
//   tcl       - [parametr/rezultat] wska«nik do struktur kontrolnych paska narz©dzi
//
static VOID CalcObjectSize(TOOLCTL *tcl)
{ OBJCTL *objtab;   // Wska«nik do pocz¥tku tablicy definicji obiket¢w
  LONG    cx, cy;   // Obliczone wymiary przycisku klasy WC_WINBUTTON
  LONG i, tmp;      // Zmienne pomocnicze

  // Inicjacja zmiennych
  objtab = tcl->objtab;
  cx = 0; cy = 0;

  // Obliczanie wymiar¢w
  for(i = 0; i < tcl->nObjects; ++i)
    switch(objtab[i].flAttribute & TBO_BASICTYPES)
    { case TBO_BUTTON:        // Obiekt klasy WC_WINBUTTON
        //tmp = (LONG)WinSendMsg(objtab[i].hwndObject, BM_QUERYWIDTH,  0, 0);
        tmp = 25;
        if(cx < tmp) cx = tmp;
        //tmp = (LONG)WinSendMsg(objtab[i].hwndObject, BM_QUERYHEIGHT, 0, 0);
        tmp = 25;
        if(cy < tmp) cy = tmp;
        break;

      case TBO_SEPARATOR:     // Separator
        objtab[i].cx = CXY_SEPARATOR;
        objtab[i].cy = CXY_SEPARATOR;
        break;
    }

  // Aktualizacja wymiar¢w okien klasy WC_WINBUTTON
  for(i = 0; i < tcl->nObjects; ++i)
    if((objtab[i].flAttribute & TBO_BASICTYPES) == TBO_BUTTON)
    { objtab[i].cx = cx;
      objtab[i].cy = cy;
    }
}





// Funkcja por¢wnuje dwa obiekty (jest u¾ywana przez funkcj© qsort)
// za kryterium przyjmuj¥c pozycj© okna (pole iPosition struktur kontrolnych)
//
// Parametry:
//   object1   - [parametr] wska«nik do pierwszego obiektu
//   object2   - [parametr] wska«nik do drugiego obiektu
//
static int CompareObjects(const void *object1, const void *object2)
{
   if(((OBJCTL *)object1)->iPosition > ((OBJCTL *)object2)->iPosition) return(1);
   if(((OBJCTL *)object1)->iPosition < ((OBJCTL *)object2)->iPosition) return(-1);
   return(0);
}





// Funkcja zast©puje pole iPosition struktury OBJCTL liczb¥ TB_END.
// Pola iPosition pozostaˆych struktur s¥ tak modyfikowane, aby
// nie powstaˆa "dziura" po usuni©tym obiekcie.
// Po odj©ciu indeksu, tablica struktur kontrolnych jest sortowana.
//
// Parametry:
//   tcl       - [parametr/rezultat] - wska«nik do struktur kontrolnych paska narz©dzi
//   objidx    - [parametr] - indeks obiektu w tablicy struktur
//
// Powr¢t:
//   Nowe poˆo¾enie obiektu w tabeli struktur
//
static LONG DeletePositionIndex(TOOLCTL *tcl, ULONG objidx)
{ OBJCTL *tmp;
  LONG   i;

  // Odczyt adresu obiektu
  if(objidx >= tcl->nObjects) return(-1);
  tmp = tcl->objtab + objidx;
  // Zapisanie nieprawidˆowej pozycji TB_END
  tmp->iPosition = TB_END;

  // Aktualizacja pozostaˆych struktur
  for(i = 0, tmp = tcl->objtab; i < tcl->nObjects; ++i)
    if((tmp[i].iPosition > objidx) && (tmp[i].iPosition != TB_END))
    { // Przesuni©cie obiektu znajduj¥cego si© za usuwanym o jeden w tyˆ
      tmp[i].iPosition --;
    }

  // Sortowanie tablicy obiekt¢w ze wzgl©du na poˆo¾enie
  qsort(tcl->objtab, tcl->nObjects, sizeof(OBJCTL), CompareObjects);
  // Nowe poˆo¾enie obiektu
  return(tcl->nObjects - 1);
}





// Funkcja oblicza pozycj© obieku na podstawie iPosition, aktualizuj¥c jednocze˜nie
// poˆo¾enia pozostaˆych obiekt¢w. Obliczona pozycja jest zapami©tywana
// w strukturze kontrolnej obiektu obj.
// Po obliczeniu nowej pozycji, tablica jest sortowana, a funkcja zwraca
// nowy indeks struktury kontrolnej obiektu
//
// Parametry:
//   tcl       - [parametr/rezultat] wska«nik do struktur kontrolnych paska narz©dzi
//   objidx    - [parametr/rezultat] indeks dodawanego obiektu
//   iPosition - [parametr] proponowana pozycja nowego obiektu (mo¾e te¾ by† TB_END)
//
// Powr¢t:
//   Nowy indeks obiektu w tablicy struktur, lub -1 w przypadku bˆ©du
//
static LONG InsertPositionIndex(TOOLCTL *tcl, ULONG objidx, ULONG iPosition)
{ OBJCTL *tmp;      // Pomocniczy wska«nik do obiektu
  OBJCTL *objtab;   // Wska«nik do tablicy obiekt¢w
  LONG   i;         // Zmienne pomocznicze

  // Odczyt adresu obiektu
  if(objidx >= tcl->nObjects) return(-1);
  tmp = tcl->objtab + objidx;
  // Zapisanie nieprawidˆowej pozycji TB_END
  tmp->iPosition = TB_END;

  // Korekta poˆo¾enia iPosition
  if(iPosition >= tcl->nObjects) iPosition = tcl->nObjects - 1;

  // Aktualizacja poˆo¾enie pozostaˆych obiekt¢w
  for(i = 0, objtab = tcl->objtab; i < tcl->nObjects; ++i)
    if((objtab[i].iPosition >= iPosition) && (objtab[i].iPosition != TB_END))
    { // Przesuni©cie obiektu znajduj¥cego si© za dodawanym o jeden w prz¢d
      objtab[i].iPosition ++;
    }

  // Zapami©tanie pozycji nowego obiektu
  tmp->iPosition = iPosition;
  // Sortowanie tablicy obiekt¢w ze wzgl©du na poˆo¾enie
  qsort(objtab, tcl->nObjects, sizeof(OBJCTL), CompareObjects);
  // Przekazanie nowej pozycji
  return(iPosition);
}





// Funkcja dodaje struktur© informacyjn¥ do tablicy definicji obiekt¢w.
// W wyniku wykonania funkcji modyfikowana jest zmienna nObjects zawieraj¥ca
// liczb© obiekt¢w obecnych na pasku narz©dzi.
//
// Parametry:
//   tcl       - [parametr/rezultat] wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
//
// Powr¢t:
//   >= 0 - indeks w tabeli definicji obiekt¢w
//     -1 - bˆad alokacji pami©ci
//
static LONG ExpandObjectTable(TOOLCTL *tcl)
{ OBJCTL *tmp;

  if(tcl->objtab != NULL)
  { // Zmiana rozmiaru tablicy
    tmp = realloc(tcl->objtab, (tcl->nObjects + 1) * sizeof(OBJCTL));
    if(tmp != NULL)
    { // Zapami©tanie nowego wska«nika
      tcl->objtab = tmp;
      // Obliczenie adresu nowozaalokowanej struktury
      tmp = tcl->objtab + tcl->nObjects;
      // Zerowanie pami©ci
      memset(tmp, 0, sizeof(OBJCTL));
      // Zapami©tanie nowej liczby struktur
      tcl->nObjects ++;
      // Przekazanie indeksu obiektu
      return(tcl->nObjects - 1);
    }
  }
  else
  { // Pocz¥tkowo jest tylko jeden element
    tcl->nObjects = 1;
    // alokacja tablicy
    tcl->objtab = malloc(sizeof(OBJCTL));
    // Zerowanie zaalokowanego obszaru
    memset(tcl->objtab, 0, sizeof(OBJCTL));
    // Przekazanie indeksu
    if(tcl->objtab != NULL) return(0);
  }

  // Bˆ¥d wykonania
  return(-1);
}





// Funkcja poszukuje obiektu o podanym identyfikatorze.
//
// Parametry:
//   tcl       - [parametr] wska«nik do gˆ¢wnych struktur kontrolnych paska narz©dzi
//   id        - [parametr] identyfikator poszukiwanego obiektu
//
// Powr¢t:
//   Wska«nik do struktury kontrolnej odnalezionego obiektu
//   NULL - nie znaleziono obiektu
//
static OBJCTL *FindObjectID(TOOLCTL *tcl, ULONG id)
{ OBJCTL *objtab;   // Wska«nik do tablicy definicji obiekt¢w
  LONG i;           // Zmienne pomocnicze

  // Odczyt adresu pocz¥tku tablicy
  objtab = tcl->objtab;

  // Przeszukiwanie tablicy obiekt¢w
  for(i = 0; i < tcl->nObjects; ++i)
  { if(objtab[i].id == id)
      return(objtab + i);
  }
  return(NULL);
}





// Funkcja rozmieszcza obiekty na pasku narz©dzi w sytuacji gdy si© one tam nie mieszcz¥.
// Wszystkie obiekty s¥ dosuwane do pocz¥tku paska anrz©dzi, a wystaj¥ce zostaj¥ obci©te.
// Jest uwzgl©dniana zawarto˜† pola lScrollOffset u¾ywanego przez pasek
// z atrybutem TBS_SCROLLABLE
//
// Parametry:
//   tcl       - [parametr] adres gˆ¢wnej struktury kontrolnej paska narz©dzi
//   area      - [parametr] obszar u¾ytkowy
//
static VOID  PlaceEndcut(TOOLCTL *tcl, RECTL *area)
{ OBJCTL *objtab;   // Wska«nik do pocz¥tku tablicy definicji obiekt¢w
  LONG    bpos;     // Pozycja liczona od pocz¥tku paska narz©dzi
  LONG    i;        // Zmienne pomocznicze

  // Obliczenie wymiar¢w przycisk¢w
  CalcObjectSize(tcl);
  // Inicjacja pozostaˆych zmiennych
  objtab = tcl->objtab;

  // Pozycja pocz¥tkowa z uwzgl©dnieniem przesuni©cia
  if(tcl->flToolState & TST_ROTATED)
    bpos = area->yTop + tcl->lScrollOffset;
  else bpos = area->xLeft - tcl->lScrollOffset;

  // Rozmieszczanie obiekt¢w justowanych od lewej
  for(i = 0; i < tcl->nObjects; ++i)
    if(!(objtab[i].flAttribute & TBO_HIDE))
      if(tcl->flToolState & TST_ROTATED)
      { // Pozycjonowanie obiekt¢w na pionowym pasku narz©dzi
        objtab[i].x = area->xLeft;
        objtab[i].y = bpos - objtab[i].cy;
        bpos -= (objtab[i].cy + CXY_OBJECTSPACE);
      }
      else
      { // Pozycjonowanie obiekt¢w na poziomym pasku narz©dzi
        objtab[i].x = bpos;
        objtab[i].y = area->yBottom;
        bpos += (objtab[i].cx + CXY_OBJECTSPACE);
      }
}





// Funkcja rozmieszcza obiekty na pasku narz©dzi w sytuacji gdy wszystkie si©
// tam mieszcz¥. Rozpoznawane s¥ atrybuty steruj¥ce procesem justowania
// obiekt¢w.
//
// Parametry:
//   tcl       - [parametr] adres gˆ¢wnej struktury kontrolnej paska narz©dzi
//   area      - [parametr] obszar u¾ytkowy
//
static VOID PlaceStandard(TOOLCTL *tcl, RECTL *area)
{ OBJCTL *objtab;   // Wska«nik do pocz¥tku tablicy definicji obiekt¢w
  LONG    bpos;     // Pozycja liczona od pocz¥tku paska narz©dzi
  LONG    bidx;     // Indeks liczony od pocz¥tku paska narz©dzi
  LONG    epos;     // Pozycja liczona od koäca paska narz©dzi
  LONG    eidx;     // Indeks liczony od koäca paska narz©dzi
  LONG    i, tmp;   // Zmienne pomocznicze

  // Obliczenie wymiar¢w przycisk¢w
  CalcObjectSize(tcl);
  // Inicjacja pozostaˆych zmiennych
  objtab = tcl->objtab;
  // Pozycja pocz¥tkowa
  bpos = tcl->flToolState & TST_ROTATED ? area->yTop    : area->xLeft;
  bidx = 0;
  epos = tcl->flToolState & TST_ROTATED ? area->yBottom : area->xRight;
  eidx = tcl->nObjects - 1;

  // Rozmieszczanie obiekt¢w justowanych od lewej
  for(i = 0; (i < tcl->nObjects) && ((objtab[i].flAttribute & TBO_JUSTTYPES) == TBO_BEGINALIGN); ++i)
  { if(!(objtab[i].flAttribute & TBO_HIDE))
    { if(tcl->flToolState & TST_ROTATED)
      { // Pozycjonowanie obiekt¢w na pionowym pasku narz©dzi
        objtab[i].x = area->xLeft;
        objtab[i].y = bpos - objtab[i].cy;
        bpos -= (objtab[i].cy + CXY_OBJECTSPACE);
      }
      else
      { // Pozycjonowanie obiekt¢w na poziomym pasku narz©dzi
        objtab[i].x = bpos;
        objtab[i].y = area->yBottom;
        bpos += (objtab[i].cx + CXY_OBJECTSPACE);
      }
    }
    // Zwi©kszenie licznika okien umieszczanych od lewej strony
    bidx ++;
  }

  // Rozmieszczenie obiekt¢w justowanych od prawej
  for(i = eidx; (i >= 0) && ((objtab[i].flAttribute & TBO_JUSTTYPES) == TBO_ENDALIGN); --i)
  { if(!(objtab[i].flAttribute & TBO_HIDE))
      if(tcl->flToolState & TST_ROTATED)
      { // Dodanie marginesu lub odst©pu mi©dzy obiektami
        if(i != (tcl->nObjects - 1)) epos += CXY_OBJECTSPACE;
        // Pozycjonowanie obiekt¢w na pionowym pasku narz©dzi
        objtab[i].x = area->xLeft;
        objtab[i].y = epos;
        epos += objtab[i].cy;
      }
      else
      { // Dodanie marginesu lub odst©pu mi©dzy obiektami
        if(i != (tcl->nObjects - 1)) epos -= CXY_OBJECTSPACE;
        // Pozycjonowanie obiekt¢w na poziomym pasku narz©dzi
        objtab[i].x = epos - objtab[i].cx;
        objtab[i].y = area->yBottom;
        epos -= objtab[i].cx;
      }
    // Zmniejszenie licznika obiekt¢w umieszczanych od prawej strony
    eidx --;
  }

  // Rozmieszczenie obiekt¢w wycentrowanych
  // Obliczenie caˆkowitej szeroko˜ci obiekt¢w
  for(i = bidx, tmp = 0; i <= eidx; ++i)
    if(!(objtab[i].flAttribute & TBO_HIDE))
    { tmp += (tcl->flToolState & TST_ROTATED ? objtab[i].cy : objtab[i].cx);
      if(i < eidx) tmp += CXY_OBJECTSPACE;
    }

  // Obliczenie pozycji pocz¥tkowej
  if(tcl->flToolState & TST_ROTATED)
    bpos -= ((bpos - epos - tmp) / 2);
  else bpos += ((epos - bpos - tmp) / 2);

  // Pozycjonowanie obiekt¢w
  for(i = bidx; i <= eidx; ++i)
    if(!(objtab[i].flAttribute & TBO_HIDE))
      if(tcl->flToolState & TST_ROTATED)
      { // Pozycjonowanie obiekt¢w na pionowym pasku narz©dzi
        objtab[i].x = area->xLeft;
        objtab[i].y = bpos - objtab[i].cy;
        bpos -= (objtab[i].cy + CXY_OBJECTSPACE);
      }
      else
      { // Pozycjonowanie obiekt¢w na poziomym pasku narz©dzi
        objtab[i].x = bpos;
        objtab[i].y = area->yBottom;
        bpos += (objtab[i].cx + CXY_OBJECTSPACE);
      }
}





// Funkcja usuwa obiekt z tablicy danych.
//
// Parametry:
//   tcl       - [parametr/rezultat] wska«nik do gˆ¢wnej struktury kontrolnej okna
//   objidx    - [parametr] numer usuwanego obiektu
//
static VOID ShrinkObjectTable(TOOLCTL *tcl, ULONG objidx)
{ OBJCTL *tmp;

  // Sprawdzenie czy indeks obiektu jest poprawny
  if(tcl->nObjects == 0) return;
  if(objidx >= tcl->nObjects) return;

  // Przesuni©cie obiekt¢w znajduj¥cych si© za usuwanym
  if(objidx < (tcl->nObjects - 1))
    memmove(tcl->objtab + objidx, tcl->objtab + objidx + 1, tcl->nObjects - objidx - 1);
  // Zmniejszenie liczby obiekt¢w
  tcl->nObjects --;

  // Ponowna alokacja pami©ci
  tmp = realloc(tcl->objtab, tcl->nObjects * sizeof(OBJCTL));
  if(tmp != NULL) tcl->objtab = tmp;
}





// Funkcja uaktualnia spos¢b wyr¢wnywania obiekt¢w s¥siaduj¥cych z obiektem
// dodawanym do paska narz©dzi.
// Je¾eli nowy obiekt posiada styl TBO_BEGINALIGN, to wszystkie obiekty po lewej (w g¢r©)
// uzyskuj¥ styl TBO_BEGINALIGN.
// Je¾eli nowy obiekt posiada styl TBO_ENDALIGN, to wszystkie obiekty po prawej (w d¢ˆ)
// uzyskuj¥ styl TBO_ENDALIGN.
// Je¾eli obiekt posiada styl TBO_CENTERALIGN, to wszystkie obiekty po lewej (u g¢ry)
// posiadaj¥ce styl TBO_ENDALIGN uzyskuj¥ TBO_CENTERALIGN, oraz wszystkie obiekty po
// prawej (u doˆu) posiadaj¥ce styl TB_BEGINALIGN uzyskuj¥ TBO_CENTERALIGN.
//
// Parametry:
//   tcl        - [parametr/rezultat] adres gˆ¢wnej struktury kontrolnej paska narz©dzi
//   objidx     - [parametr] indeks dodawanego obiektu
//   flNewAttrs - [parametr] atrybuty nowego obiektu
//
static VOID UpdateAlignment(TOOLCTL *tcl, LONG objidx, ULONG flNewAttrs)
{ OBJCTL *objtab;   // Adres pocz¥tku tablicy definicji obiekt¢w
  LONG i;           // Zmienne pomocnicze

  // Inicjacja zmiennych
  objtab = tcl->objtab;
  // Usuni©cie pozostaˆych tryb¢w justowania
  flNewAttrs &= TBO_JUSTTYPES;

  switch(flNewAttrs)
  { case TBO_BEGINALIGN:      // Korekta po dodaniu obiektu justowanego od lewej
      for(i = objidx; i >= 0; --i)
      { objtab[i].flAttribute &= ~TBO_JUSTTYPES;
        objtab[i].flAttribute |=  TBO_BEGINALIGN;
      }
      break;

    case TBO_ENDALIGN:        // Korekta po dodaniu obiektu justowanego do prawej
      for(i = objidx; i < tcl->nObjects; ++i)
      { objtab[i].flAttribute &= ~TBO_JUSTTYPES;
        objtab[i].flAttribute |=  TBO_ENDALIGN;
      }
      break;

    case TBO_CENTERALIGN:     // Korekta po dodaniu obiektu centrowanego
      // Sprawdzenie obiekt¢w po lewej
      for(i = objidx; i >= 0; --i)
        if((objtab[i].flAttribute & TBO_JUSTTYPES) != TBO_BEGINALIGN)
        { objtab[i].flAttribute &= ~TBO_JUSTTYPES;
          objtab[i].flAttribute |=  TBO_CENTERALIGN;
        }
      // Sprawdzenie obiekt¢w po prawej
      for(i = objidx; i < tcl->nObjects; ++i)
        if((objtab[i].flAttribute & TBO_JUSTTYPES) != TBO_ENDALIGN)
        { objtab[i].flAttribute &= ~TBO_JUSTTYPES;
          objtab[i].flAttribute |=  TBO_CENTERALIGN;
        }
      break;
  }
}





// Funkcja uaktualnia atrybuty w strukturach wszystkich obiekt¢w nale¾¥cych do
// podanej grupy. Uaktualniane jest pola flAttribute struktur OBJCTL,
// ale tylko wtedy gdy zostaˆ u¾yty kt¢ry˜ z tryb¢w:
// TBO_PUSHBUTTON, TBO_RADIOBUTTON lub TBO_CHECKBOX.
// W przeciwnym wypadku style pozostaj¥ bez zmian.
//
// Parametry:
//   tcl        - [parametr/rezultat] wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
//   ulGroup    - [parametr] grupa, dla kt¢rej s¥ modyfikowane parametry
//   flNewAttrs - [parametr] nowy zestaw atrybut¢w
//
static VOID UpdateGroupStyle(TOOLCTL *tcl, ULONG ulGroup, ULONG flNewAttrs)
{ OBJCTL *objtab;   // Adres pocz¥tku tablicy definicji obiekt¢w
  LONG i;           // Zmienne pomocnicze

  // Inicjacja zmiennych
  objtab = tcl->objtab;

  // Sprawdzenie czy parametry grupy s¥ modyfikowane
  if(!(flNewAttrs & TBO_RADIOBUTTON))
  {
    // Je˜li nie, to poszukiwanie domy˜lnego parametru grupy
    for(i = 0; i < tcl->nObjects; ++i)
      if(objtab[i].ulGroup == ulGroup)
        if(objtab[i].flAttribute & TBO_RADIOBUTTON)
        { // Przyj©cie parametr¢w domy˜lnych
          flNewAttrs = objtab[i].flAttribute & TBO_RADIOBUTTON;
          break;
        }
  }

  // Przeszukiwanie tablicy obiekt¢w i korekta parametr¢w grupy
  for(i = 0; i < tcl->nObjects; ++i)
    if(objtab[i].ulGroup == ulGroup)
    { // Aktualizacja parametr¢w grupy
      // Kasowanie zastanego stlu grupy
      objtab[i].flAttribute &= ~TBO_RADIOBUTTON;
      // Dodanie nowego stylu
      objtab[i].flAttribute |= (flNewAttrs & TBO_RADIOBUTTON);
    }
}





// Funkcja rysuje na pasku narz©dzi separator.
// Zmienna flState okre˜la poˆo¾enie paska narz©dzi, bit: TST_ROTATED.
// Zmienna objAttrs okre˜ˆa wygl¥d separatora, staˆe SPS_*.
//
// Parametry:
//   hps       - [parametr] uchwyt presentation space okna
//   obj       - [parametr] wska«nik do struktury definiuj¥cej obiekt
//   area      - [parametr] wska«nik do obszaru przeznaczonego na obiekty
//   flState   - [parametr] stan paska narz©dzi
//
static VOID DrawSeparator(HPS hps, OBJCTL *obj, RECTL *area, ULONG flState)
{ POINTL pos;       // Wsp¢ˆrz©dne og¢lnego przeznaczenia
  RECTL  line;      // Wsp¢ˆrz©dne rysowanej lini
  ULONG  objAttr;   // Atrybuty separatora

  // Okre˜lenie pozycji i wymiar¢w separatora
  if(flState & TST_ROTATED)
  { line.xLeft   = area->xLeft;
    line.yBottom = obj->y;
    line.xRight  = area->xRight - 1;
    line.yTop    = obj->y + obj->cy - 1;
  }
  else
  { line.xLeft   = obj->x;
    line.yBottom = area->yBottom;
    line.xRight  = obj->x + obj->cx - 1;
    line.yTop    = area->yTop - 1;
  }

  // Rysowanie tˆa pod separatorem
  GpiSetColor(hps, TOOL_BACKGROUND);
  GpiMove(hps, (POINTL *)&line);
  GpiBox(hps, DRO_FILL, ((POINTL *)&line) + 1, 0, 0);

  // Obliczenie wsp¢ˆrz©dnych pocz¥tkowych i koäcowych rysowanych linii
  if(flState & TST_ROTATED)
  { line.yBottom += ((obj->cy - 2) / 2);
    line.yTop     = line.yBottom + 1;
  }
  else
  { line.xLeft   += ((obj->cx - 2) / 2);
    line.xRight   = line.xLeft + 1;
  }

  // Okre˜lenie atrybut¢w separatora
  objAttr = obj->flAttribute & SPS_SEPARATORS;

  // Rysowanie separator¢w r¢¾nych typ¢w
  switch(objAttr)
  { case SPS_LINE:       // Pˆaska linia w kolorze PP_BORDERDARKCOLOR
      GpiSetColor(hps, TOOL_FRAMEDARK);
      GpiMove(hps, (POINTL *)&line);
      GpiBox(hps, DRO_FILL, ((POINTL *)&line) + 1, 0, 0);
      break;

    case SPS_CONCAVE:    // Wkl©sˆa linia 3D
    case SPS_CONVEX:     // Wypukˆa linia 3D
      GpiSetColor(hps, objAttr == SPS_CONCAVE ? TOOL_FRAMEDARK : TOOL_FRAMEHILITE );
      pos.x = line.xLeft;  pos.y = line.yBottom;
      GpiMove(hps, &pos);
      pos.y = line.yTop;   GpiLine(hps, &pos);
      pos.x = line.xRight; GpiLine(hps, &pos);

      GpiSetColor(hps, objAttr == SPS_CONCAVE ? TOOL_FRAMEHILITE : TOOL_FRAMEDARK );
      pos.y = flState & TST_ROTATED ? pos.y : pos.y - 1;
      GpiMove(hps, &pos);
      pos.y = line.yBottom; GpiLine(hps, &pos);
      pos.x = flState & TST_ROTATED ? line.xLeft + 1 : line.xLeft;
      GpiLine(hps, &pos);
      break;
  }
}

/*
 * $Log: objmgr.c $
 * Revision 1.1  1999/06/27 12:36:00  Wojciech_Gazda
 * Initial revision
 *
 */
