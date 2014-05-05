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

/* Asuactl.dll -
 * Pasek narz�dzi plik g��wny
 * (c) 1999 Wojciech Gazda
 *
 * toolbar.c
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/06/27 12:35:32 $
 * $Name:  $
 * $RCSfile: toolbar.c $
 * $Revision: 1.1 $
 *
 */


#define INCL_GPICONTROL
#define INCL_GPIPRIMITIVES
#define INCL_WINRECTANGLES
#define INCL_WINMESSAGEMGR
#define INCL_WINERRORS


// Deklaracje OS/2
#define  INCL_DOSPROCESS
#define  INCL_DOSMISC
#define  INCL_GPILOGCOLORTABLE
#define  INCL_WINWINDOWMGR
#define  INCL_WINFRAMEMGR
#define  INCL_WININPUT
#define  INCL_WINATOM
#define  INCL_WINSYS
#include <os2.h>


// Funkcje biblioteczne
#include <malloc.h>
#include <string.h>

// *** TEST ****
#include <stdio.h>


// Deklaracje lokalne
#define  __INTERNAL_USE__
#include "status.h"
#include "asuintl.h"
#include "toolbar.h"
#include "tooldefs.h"
#include "tcontrol.h"
#include "carrier.h"
#include "objmgr.h"




// Zmienne globalne
// Tablica wzorc�w kolor�w u�ywanych posczas rysowania paska narz�dzi
PPTEMPLATE PPmColor[TOOL_MAXCOLOR] =
{{  PP_BACKGROUNDCOLOR,     QPF_NOINHERIT, // TOOL_BACKGROUND
    SYSCLR_BUTTONMIDDLE,    0xCCCCCC },    //   Kolor t�a nieaktywnego paska tytu�u
 {  PP_ACTIVECOLOR,         QPF_NOINHERIT, // TOOL_HILITEBACKGROUND
    0L,                     0xC7D5FF },    //   Kolor t�a aktywnego paska tytu�u
 {  PP_BORDERLIGHTCOLOR,    QPF_NOINHERIT, // TOOL_FRAMEHILITE
    0L,                     0xFFFFFF },    //   Kolor o�wietlenia
 {  PP_BORDERDARKCOLOR,     QPF_NOINHERIT, // TOOL_FRAMEDARK
    0L,                     0x808080 },    //   Kolor cienia
 {  PP_TOOLTIPBACKGROUND,   QPF_NOINHERIT, // TOOL_TIPBACKGROUND
    0L,                     0xFFFFC8 },    //   Kolor t�a pod tekstem podpowiedzi
 {  PP_TOOLTIPTEXT,         QPF_NOINHERIT, // TOOL_TIPTEXTCOLOR
    SYSCLR_MENUTEXT,        0x000000 },    //   Kolor tekstu w oknie podpowiedzi
 {  PP_TOOLTIPFRAME,        QPF_NOINHERIT, // TOOL_TIPFRAMECOLOR
    0L,                     0x000000 },    //   Kolor ramki otaczaj�cej podpowied�
 {  PP_BORDER2LIGHTCOLOR,   QPF_NOINHERIT, // TOOL_FRAMEDRAGHILITE
    0L,                     0x808080 },    //   Kolor ramki otaczaj�cej pasek podczas przesuwania
 {  PP_BORDER2DARKCOLOR,    QPF_NOINHERIT, // TOOL_FRAMEDRAGDARK
    0L,                     0x000000 },    //   Kolor ramki otaczaj�cej pasek podczas przesuwania
};


// Wersja paska narz�dzi
static CTLINFO ctlToolVersion =
{ sizeof(CTLINFO),
  NULL,
  1,0
};




// Prototypy funkcji
LONG ASUAAPI CtlToolbarInitialize(HAB hab);
VOID     ToolDisplayTip(HWND hwnd, HWND hwndRef, PSZ szTipText);
VOID     ToolDrawSeparator(HWND hwnd, LONG lPos, ULONG ulSize);
TOOLCTL *ToolLoadData(HWND hwnd, PSZ module);
LONG     ToolSendNotify(HWND hwnd, ULONG notify, VOID *ctlspec);
VOID     ToolUpdateSize(TOOLCTL *tcl);

// Prototypy funkcji lokalnych
static MRESULT EXPENTRY WinToolbarProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static MRESULT ToolAutoSize(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static MRESULT ToolCalcRect(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static MRESULT ToolEnableRotate(HWND hwnd, MPARAM mp1);
static MRESULT ToolQueryHeight(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static MRESULT ToolQueryParams(HWND hwnd, MPARAM mp1);
static MRESULT ToolQueryState(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static MRESULT ToolQueryWidth(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static MRESULT ToolSetParams(HWND hwnd, MPARAM mp1);
static VOID  ToolDestroy(HWND hwnd);
static LONG  ToolInit(HWND hwnd, VOID *ctldata, CREATESTRUCT *ctr);

// Lokalne funkcje pomocnicze
static VOID  StoreClassData(TOOLCTL *tcl, TOOLINFO *cdata);
static VOID  StoreWindowText(TOOLCTL *tcl, PSZ text);
static VOID  UpdateTipPParams(HWND ToolBar, HWND ToolTip);





// Przygotowanie okna WC_TOOLBAR do pracy
//
// Parametry:
//   hab       - [parametr] uchwyt anchor block
//
// Powr�t:
//   0    - poprawne wykonanie funkcji
//   1    - b��d rejestracji klasy okna
//
LONG ASUAAPI CtlToolbarInitialize(HAB hab)
{ ULONG winstyle;   // Domy�lny styl okna
  LONG  rc;         // Kody b��d�w zwracane przez funkcje systemowe

  // Ustalenie domy�lnego stylu okna WC_TOOLBAR
  winstyle = CS_CLIPCHILDREN | CS_CLIPSIBLINGS;
  // Rejestracja nowej klasy okna - pasek narz�dzi
  rc = WinRegisterClass(hab, WC_TOOLBAR, WinToolbarProc, winstyle, 16);
  if(!rc) return(1);

  // Rejestracja klasy paska tytu�u - INTC_MINITITLE
  winstyle = CS_SYNCPAINT | CS_SIZEREDRAW;
  rc = WinRegisterClass(hab, INTC_MINITITLE, MiniTitlebarProc, winstyle, sizeof(VOID *));
  if(!rc) return(1);

  // Rejestracja klasy paska tytu�u - INTC_ROTATEBTN
  winstyle = CS_SYNCPAINT | CS_SIZEREDRAW;
  rc = WinRegisterClass(hab, INTC_ROTATEBTN, RotateButtonProc, winstyle, sizeof(VOID *));
  if(!rc) return(1);

  // Rejestracja klasy okna przechowuj�cego obiekty - INTC_OBJECTPAD
  winstyle = CS_CLIPCHILDREN | CS_CLIPSIBLINGS | CS_HITTEST;
  rc = WinRegisterClass(hab, INTC_OBJECTPAD, ObjectPadProc, winstyle, sizeof(VOID *));

  return(0);
}





// Wy�wietlenie okna z podpowiedzi� - okno jest pozycjonowane wzgl�dem
// dolnej kraw�dzi okna odniesienia.
//
// Parametry:
//   hwnd      - [parametr] okno rodzicielskie maj�ce dost�p do struktury TOOLCTL
//   hwndRef   - [parametr] okno odniesienia, wzgl�dem kt�rego jest pozycjonowana podpowied�
//   szTipText - [parametr] tekst podpowiedzi
//
VOID ToolDisplayTip(HWND hwnd, HWND hwndRef, PSZ szTipText)
{ TOOLCTL *tcl;          // Adres g��wnej struktury kontrolnej paska narz�dzi
  RECTL    screen;       // Rozmiary ekranu
  RECTL    refsize;      // Rozmiary okna odniesienia
  LONG     tlcx, tlcy;   // Rozmiary okna podpowiedzi
  LONG     tlx, tly;     // Pozycja okna podpowiedzi


  if(szTipText == NULL) return;
  tcl = ToolLoadData(hwnd, "ToolDispalyTip");
  if(tcl == NULL) return;

  // Tworzenie okna podpowiedzi je�li wcze�niej nie istnia�o
  if(tcl->hwndToolTip == NULLHANDLE)
  { tcl->hwndToolTip = WinCreateWindow(HWND_DESKTOP, WC_STATUS, "",
                         SS_THINFRAME  | SS_BORDER | SS_CENTER |
                         SS_XAUTOSCALE | SS_YAUTOSCALE | SS_MOUSETRANSPARENT |
                         WS_SYNCPAINT,
                         0, 0, 0, 0,
                         tcl->hwndToolBar,
                         HWND_TOP, TCID_TOOLTIP,
                         NULL, NULL);

    // Kopiowanie domy�lnych prametr�w prezentacji
    UpdateTipPParams(tcl->hwndToolBar, tcl->hwndToolTip);
  }

  if(tcl->hwndToolTip)
  { // Za�adowanie tekstu podpowiedzi
    WinSetWindowText(tcl->hwndToolTip, szTipText);
    // Odczyt rozmiar�w okna podpowiedzi
    tlcx = (LONG)WinSendMsg(tcl->hwndToolTip, SM_QUERYWIDTH,  0L, 0L);
    tlcy = (LONG)WinSendMsg(tcl->hwndToolTip, SM_QUERYHEIGHT, 0L, 0L);

    // Obliczenie pozycji okna wzgl�dem lewego dolnego rogu ekranu
    // Odczyt rozmiar�w ekranu
    WinQueryWindowRect(HWND_DESKTOP, &screen);
    // Odczyt rozmiar�w okna odniesienia
    WinQueryWindowRect(hwndRef, &refsize);
    // Konwersja do wsp��rz�dnych ekranowych
    WinMapWindowPoints(hwndRef, HWND_DESKTOP, (POINTL *)&refsize, 2L);

    // Okre�lenie po�o�enia okna podpowiedzi
    tlx  = refsize.xLeft;
    // Korekta po�o�enia poziomego
    if((tlx + tlcx) > screen.xRight)
    { // Odsuni�cie okna od prawej kraw�dzi
      tlx = screen.xRight - tlcx;
    }
    else if(tlx < screen.xLeft)
    { // Dosuni�cie okna do lewej kraw�dzi
      tlx = screen.xLeft;
    }

    tly  = refsize.yBottom - tlcy - CY_BOTTOMTIPSPACE;
    // Korekta po�o�enia pionowego
    if(tly < screen.yBottom)
    { // Przeniesienie podpowiedzi ponad okno
      tly = refsize.yTop + CY_TOPTIPSPACE;
    }

    // Wy�wietlenie okna
    WinSetWindowPos(tcl->hwndToolTip, HWND_TOP,
                    tlx, tly, 0, 0,
                    SWP_MOVE | SWP_ZORDER | SWP_SHOW);
  }
}






// �adowanie wska�nika do struktur kontrolnych okna
// paska narz�dzi i jego okien potomnych
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna
//   module    - [parametr] nazwa funkcji wywo�uj�cej funkcj� (opcjonalnie)
//
// Powr�t:
//   <>NULL    - wska�nik do struktur kontrolnych
//     NULL    - b��d odczytu
//
TOOLCTL *ToolLoadData(HWND hwnd, PSZ module)
{ TOOLCTL *tcl;     // Adres struktur kontrolnych
  //LONG     rc;      // Kod b��du

  // Odczyt adresu struktury kontrolnej
  tcl = WinQueryWindowPtr(hwnd, 0L);
  if(tcl != NULL)
  {
    // Od�wie�enie p�l zawieraj�cych styl i stan paska narz�dzi na podstawie
    // danych zapisanych w systemowych strukturach okna
    tcl->flWinStyle = WinQueryWindowULong(tcl->hwndToolBar, QWL_STYLE);
    if(tcl->flWinStyle & WS_MAXIMIZED) tcl->flToolState |=  TST_FIXED;
    else                               tcl->flToolState &= ~TST_FIXED;
    if(tcl->flWinStyle & WS_MINIMIZED) tcl->flToolState |=  TST_MINIMIZED;
    else                               tcl->flToolState &= ~TST_MINIMIZED;

    return(tcl);
  }

  // Odczyt kodu b��du
  //rc = WinGetLastError(WinQueryAnchorBlock(hwnd));
  // Zapisanie komunikatu do pliku
  //AsuLogPMError(WC_TOOLBAR, rc, module, "WinQueryWindowPtr is unable to get TOOLCTL structure adress.");
  // Powr�t - b��dny wska�nik
  return(NULL);
}





// Wys�anie komunikatu WM_CONTROL do w�a�ciciela okna
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna wysy�aj�cego komunikat
//   notify    - [parametr] kod potwierdzenia
//   ctlspec   - [paramet/rezultat] dane control-specific
//
// Powr�t:
//   Kod zwr�cony przez WinSendMsg()
//
LONG ToolSendNotify(HWND hwnd, ULONG notify, VOID *ctlspec)
{ ULONG winid;      // Identyfikator okna
  HWND  owner;      // Uchwyt w�a�ciciela
  LONG  rc;         // Kod odpowiedzi

  // Odczyt uchwytu w�ascicela
  owner = WinQueryWindow(hwnd, QW_OWNER);
  if(owner == NULLHANDLE) return(0);
  // Odczyt identyfikatora okna
  winid = WinQueryWindowUShort(hwnd, QWS_ID);

  // Wys�anie komunikatu
  rc = (LONG)WinSendMsg(owner, WM_CONTROL, MPFROM2SHORT(winid, notify), MPFROMP(ctlspec));
  return(rc);
}





// Funkcja koryguje rozmiary paska narz�dzi i u�o�enie okien potomnych po dodaniu
// lub odj�ciu obiektu
//
// Parametry:
//   tcl       - [parametr] wska�nik do struktur kontrolnych paska narz�dzi
//
VOID ToolUpdateSize(TOOLCTL *tcl)
{
  // Koniec przetwarzania wtedy gdy komunikat zosta� wywo�any podczas
  // minimalizacji/maksymalizacji lub przej�cia w stan "floating"
  if(tcl->flToolState & TST_MINMAXCTL) return;

  // Korekta rozmiar�w okna
  if((LONG)WinSendMsg(tcl->hwndToolBar, TM_AUTOSIZE, 0, 0) == FALSE)
  {
    // Rozmieszczenie okien kontrolnych
    CrrPlaceControls(tcl->hwndToolBar);
  }
}




/*******************/
/* Funkcje lokalne */
/*******************/

// Procedura rysuj�ca i kontroluj�ca okno statusowe WC_TOOLBAR
//
static MRESULT EXPENTRY WinToolbarProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{

  switch(msg)
  {
    case WM_ADJUSTWINDOWPOS:       // Sterowanie pozycjonowaniem okna
      { SWP *swp;        // Proponowane wymiary i po�o�enie okna

        swp = PVOIDFROMMP(mp1);
        // Sprawdzenie czy jest to minimalizacja/maksymalizacja/odtworzenie stanu
        if(swp->fl & (SWP_MAXIMIZE | SWP_MINIMIZE | SWP_RESTORE))
        { // Wys�anie komunikatu WM_MINMAXFRAME do samego siebie
          return(WinSendMsg(hwnd, WM_MINMAXFRAME, mp1, mp2));
        }
        // Obs�uga pozosta�ych zdarze�
      }
      break;


    // Tu b�dzie wywo�ywana procedura "inteligentnego" od�wie�ania okna.
    case WM_CALCVALIDRECTS:
      { RECTL *old, *new;

        old = PVOIDFROMMP(mp1);
        new = old + 1;

        // **** TEST ****
        memset(old, 0, sizeof(RECTL));
        //WinInvalidateRect(hwnd, NULL, TRUE);
        return(0L);
      }
      break;

    case WM_CREATE:           // Inicjacja okna
      return(MRFROMLONG(ToolInit(hwnd, PVOIDFROMMP(mp1), (CREATESTRUCT *)PVOIDFROMMP(mp2))));

    case WM_DESTROY:          // Zamkni�cie okna
      ToolDestroy(hwnd);
      break;

    case WM_NULL:             // Odczyt wersji kontrolki
      AsuQueryCtlInfo(hwnd, msg, mp1, &ctlToolVersion);
      break;

    case WM_OWNERPOSCHANGE:   // Nast�pi�a zmiana po�o�enia w�a�ciciela
      CrrMoveWithOwner(hwnd, PVOIDFROMMP(mp1), PVOIDFROMMP(mp2));
      return(0);

    case WM_PAINT:            // Przerysowywanie okna
      CrrRedrawBorder(hwnd);
      break;

    case WM_PRESPARAMCHANGED: // Parametry prezentacji uleg�y zmianie, przerysowanie okna
      WinInvalidateRect(hwnd, NULL, TRUE);
      break;

    case WM_QUERYWINDOWPARAMS:  // Odczyt parametr�w okna
      return(ToolQueryParams(hwnd, mp1));

    case WM_SETWINDOWPARAMS:  // Zapami�tanie parametr�w okna
      return(ToolSetParams(hwnd, mp1));

    case WM_SIZE:             // Pozycjonowanie okien potomnych
      CrrPlaceControls(hwnd);
      break;

    case WM_TRACKFRAME:  // Rozpocz�cie przesuwania okna
      return(MRFROMLONG(CrrTrackRect(hwnd)));

    case WM_MINMAXFRAME: // Minimalizacja/maksymalizacja/restore
      return(MRFROMLONG(CrrMinMaxFrame(hwnd, PVOIDFROMMP(mp1))));
  }

  // Obs�uga niestandardowych komunikat�w steruj�cych TM_*
  if((msg >= TOOL_FIRST_MSG) && (msg <= TOOL_LAST_MSG))
  { switch(msg)
    { case TM_AUTOSIZE:       // Automatyczne pozycjonowanie paska
        return(ToolAutoSize(hwnd, msg, mp1, mp2));

      case TM_CALCNEWRECT:    // Obliczenie nowej proponowanej pozycji i wymiar�w
        return(ToolCalcRect(hwnd, msg, mp1, mp2));

      case TM_DELETEOBJECT:   // Usuni�cie obiektu z paska narz�dzi
        return(MRFROMLONG(ObjDeleteObject(hwnd, (ULONG)mp1)));

      case TM_ENABLEROTATE:   // Odblokowanie/zablokowanie przycisku obracania okna
        return(ToolEnableRotate(hwnd, mp1));

      case TM_IDFROMPOSITION: // Odczyt identyfikatora obiektu na podstawie pozycji
        return(MRFROMLONG(ObjIdFromPosition(hwnd, LONGFROMMP(mp1))));

      case TM_INSERTOBJECT:   // Dodanie obiektu do paska narz�dzi
        return(MRFROMLONG(ObjInsertObject(hwnd, (TOOLOBJ *)mp1, (TOOLWIN *)mp2)));

      case TM_INSERTTOOLTIP:  // Dodanie podpowiedzi do paska narz�dzi
        return(MRFROMLONG(ObjInsertTooltip(hwnd, LONGFROMMP(mp1), PVOIDFROMMP(mp2))));

      case TM_MOVEOBJECT:     // Przesuni�cie obiektu zawartego na pasku narz�dzi
        return(MRFROMLONG(ObjMoveObject(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), LONGFROMMP(mp2))));

      case TM_POSITIONFROMID: // Odczyt pozycji obiektu na podstawie identyfikatora
        return(MRFROMLONG(ObjQueryPosition(hwnd, LONGFROMMP(mp1))));

      case TM_QUERYHANDLE:    // Odczyt uchwytu okna obiektu
        return((MRESULT)ObjQueryHandle(hwnd, LONGFROMMP(mp1)));

      case TM_QUERYHEIGHT:    // Odczyt wysoko��i paska
        return(ToolQueryHeight(hwnd, msg, mp1, mp2));

      case TM_QUERYOBJECT:    // Odczyt struktury informacyjnej obiektu
        return(MRFROMLONG(ObjQueryObject(hwnd, LONGFROMMP(mp1), PVOIDFROMMP(mp2))));

      case TM_QUERYOBJECTCOUNT: // Odczyt liczby obiekt�w umieszczonych na pasku narz�dzi
        return(MRFROMLONG(ObjQueryObjectCount(hwnd)));

      case TM_QUERYSTATE:     // Odczyt aktualnego stanu paska narz�dzi
        return(ToolQueryState(hwnd, msg, mp1, mp2));

      case TM_QUERYTIP:       // Odczyt tekstu podpowiedzi
        return(MRFROMLONG(ObjQueryTip(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), PVOIDFROMMP(mp2))));

      case TM_QUERYWIDTH:     // Odczyt szeroko�ci paska
        return(ToolQueryWidth(hwnd, msg, mp1, mp2));

      case TM_ROTATE:         // Obracanie paska narz�dzi
        return(MRFROMLONG(CrrRotate(hwnd, LONGFROMMP(mp1))));

      case TM_SETOBJECT:      // Zmiana parametr�w obiektu
        return(MRFROMLONG(ObjSetObject(hwnd, SHORT1FROMMP(mp1), SHORT2FROMMP(mp1), PVOIDFROMMP(mp2))));

      case TM_SHOWOBJECT:     // Ukrycie/wy�wietlenie obiektu na pasku narz�dzi
        return(MRFROMLONG(ObjShowObject(hwnd, LONGFROMMP(mp1), LONGFROMMP(mp2))));
    }
  }

  // Przekazanie reszty komunikat�w standardowej procedurze okna
  return(WinDefWindowProc(hwnd, msg, mp1, mp2));
}





// Automatyczne skalowanie paska narz�dzi
//
static MRESULT ToolAutoSize(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{ TOOLCTL *tcl;     // Wsk�nik do g��wnej struktury kontrolnej paska
  TOOLADJ  tadj;    // Struktura steruj�ca pozycjonowaniem paska narz�dzi
  RECTL    oldpos;  // Stare rozmiary paska narz�dzi
  ULONG    maxl;    // Maksymalna d�ugo�� paska narz�dzi


  // Odczyt adresu struktur kontrolnych
  tcl = ToolLoadData(hwnd, "ToolAutoSize");
  if(tcl == NULL) return(0);
  // Odczyt max. d�ugo�ci paska
  maxl = LONGFROMMP(mp1);
  // Zapami�tanie aktualnych rozmiar�w paska narz�dzi
  WinQueryWindowRect(tcl->hwndToolBar, &oldpos);

  // Inicjacja struktury TOOLADJ
  tadj.hwndToolBar = hwnd;
  tadj.hwndParent  = WinQueryWindow(hwnd, QW_PARENT);
  tadj.flAttrs     = tcl->flToolAttrs;
  tadj.flState     = tcl->flToolState;

  // Obliczenie proponowanych wymiar�w i pozycji paska narz�dzi
  CrrCalcNewRect(&tadj);
  // Wys�anie potwierdzenia do w�a�ciciela
  ToolSendNotify(hwnd, TN_AUTOSIZE, &tadj);

  // Ustawienie nowej pozycji paska
  WinSetWindowPos(hwnd, HWND_TOP,
                  tadj.rclSizePos.xLeft,
                  tadj.rclSizePos.yBottom,
                  tadj.rclSizePos.xRight - tadj.rclSizePos.xLeft,
                  tadj.rclSizePos.yTop   - tadj.rclSizePos.yBottom,
                  SWP_ZORDER | SWP_MOVE | SWP_SIZE);

  if((tadj.rclSizePos.xRight - tadj.rclSizePos.xLeft) != (oldpos.xRight - oldpos.xLeft))
    return(MRFROMLONG(TRUE));
  if((tadj.rclSizePos.yTop - tadj.rclSizePos.yBottom) != (oldpos.yTop - oldpos.yBottom))
    return(MRFROMLONG(TRUE));
  // Wymiary nie zmieni�y si�
  return(FALSE);
}





// Funkcja umo�liwia obliczenie nowych wymiar�w i pozycji paska narz�dzi na
// podstawie parametr�w przekazanych za po�rednictwem mp1
// (struktura TOOLADJ)
//
static MRESULT ToolCalcRect(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{ TOOLADJ  *tadj;   // Wska�nik do struktury zawieraj�cej proponowane atrybuty
  TOOLCTL  *tcl;    // Wska�nik do g��wnej struktury kontrolnej paska

  // �adowanie danych kontrolnych
  tcl = ToolLoadData(hwnd, "ToolCalcRect");
  if(tcl == NULL) return(0);

  // Odczyt wska�nika do struktury
  tadj = PVOIDFROMMP(mp1);
  // Korekta zawarto�ci p�l struktury
  tadj->hwndToolBar = hwnd;
  tadj->hwndParent  = WinQueryWindow(hwnd, QW_PARENT);
  // Korekta stanu okna - uwzgl�dnienie flag niedost�pnych dla u�ytkownika
  tadj->flState &= 0xFFFF;
  tadj->flState |= (tcl->flToolState & 0xFFFF0000);

  CrrCalcNewRect(tadj);
  return(0);
}






// Odblokowanie/zablokowanie przycisku obracania okna. Stan przycisku
// mo�na odczyta� za pomoc� WinIsWindowEnabled, dla okna o identyfikatorze TCID_ROTATE
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz�dzi
//   mp1       - [parametr] nowy stan przycisku
//
static MRESULT ToolEnableRotate(HWND hwnd, MPARAM mp1)
{ TOOLCTL *tcl;          // Wska�nik do struktur kontrolnych okna

  tcl = ToolLoadData(hwnd, "ToolEnableRestore");
  if(tcl == NULL) return(0);

  // Sterowanie przyciskiem
  if(tcl->flWinStyle & TBS_ROTATEBUTTON)
  { if((BOOL)mp1 == TRUE)
    { // Odblokowanie przycisku obracania okna
      WinEnableWindow(tcl->hwndRotate, TRUE);
    }
    else
    { // Zablokowanie przycisku obracania okna
      WinEnableWindow(tcl->hwndRotate, FALSE);
    }

    // Korekta wymiar�w okna
    ToolUpdateSize(tcl);
  }
  return(0);
}





// Odczyt proponowanej wysoko�ci paska narz�dzi
//
static MRESULT ToolQueryHeight(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{ TOOLCTL *tcl;          // Wska�nik do g��wnej struktury kontrolnej
  TOOLADJ  tadj;         // Struktura wype�niana przez funkcje okre�laj�ce rozmiary
  ULONG    options;      // Opcje steruj�ce odczytem wysoko�ci

  tcl = ToolLoadData(hwnd, "ToolQueryHeight");
  if(tcl == NULL) return(0);
  // Inicjacja struktury TOOLADJ
  tadj.hwndToolBar = hwnd;
  tadj.hwndParent  = WinQueryWindow(hwnd, QW_PARENT);
  tadj.flAttrs = tcl->flToolAttrs;
  tadj.flState = tcl->flToolState;

  // Odczyt i interpretacja opcji
  options = LONGFROMMP(mp1);
  if(options != TDQ_CURRENT)
  { tadj.flState &= ~(TST_MINIMIZED | TST_FIXED);
    if(options == TDQ_MINIMIZED) tadj.flState |= TST_MINIMIZED;
    if(options == TDQ_FIXED)     tadj.flState |= TST_FIXED;
  }

  // Odczyt wysoko�ci paska
  return(MRFROMLONG(CrrQueryHeight(&tadj)));
}





// Obs�uga komunikatu WM_QUERYWINDOWPARAMS
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz�dzi
//   mp1       - [parametr] wska�nik do struktury WNDPARAMS
//
// Powr�t:
//   FALSE - b��d odczytu
//   TRUE  - odczyt wykonany poprawnie
//
static MRESULT ToolQueryParams(HWND hwnd, MPARAM mp1)
{ WNDPARAMS *wpm;   // Adres struktury w kt�rej funkcja zwraca odczytane warto�ci
  TOOLCTL   *tcl;   // Adres g��wnej struktury kontrolnej okna

  // Inicjacja zmiennych
  tcl = ToolLoadData(hwnd, "ToolQueryParams");
  if(tcl == NULL) return(FALSE);
  wpm = (WNDPARAMS *)mp1;

  // Odczyt rozmiaru danych Class-Specific
  if(wpm->fsStatus & WPM_CBCTLDATA)
  { wpm->cbCtlData = sizeof(TOOLINFO);
    wpm->fsStatus &= ~WPM_CBCTLDATA;
  }

  // Odczyt d�ugo�ci tekstu przechowywanego w oknie
  if(wpm->fsStatus & WPM_CCHTEXT)
  { if(tcl->szWinText == NULL)
      wpm->cchText = 0;
    else wpm->cchText = strlen(tcl->szWinText);
    wpm->fsStatus &= ~WPM_CCHTEXT;
  }

  // Odczyt danych Class-Specific
  if(wpm->fsStatus & WPM_CTLDATA)
  { TOOLINFO *tnfo;      // Wska�nik pomocniczy

    tnfo = wpm->pCtlData;
    tnfo->cb          = sizeof(TOOLINFO);
    tnfo->flToolAttrs = tcl->flToolAttrs;
    tnfo->usMaxLength = tcl->ulMaxLength;
    wpm->fsStatus &= ~WPM_CTLDATA;
  }

  // Odczyt tytu�u okna
  if(wpm->fsStatus & WPM_TEXT)
  { if(tcl->szWinText != NULL)
      strcpy(wpm->pszText, tcl->szWinText);
     wpm->fsStatus &= ~WPM_TEXT;
  }

  return(MRFROMLONG(TRUE));
}





// Odczyt stanu paska narz�dzi
//
static MRESULT ToolQueryState(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{ TOOLCTL *tcl;          // Wska�nik do g��wnej struktury kontrolnej

  tcl = ToolLoadData(hwnd, "ToolQueryState");
  if(tcl == NULL) return(0);

  return(MRFROMLONG(tcl->flToolState & 0xFFFF));
}





// Odczyt proponowanej szeroko�ci paska narz�dzi
//
static MRESULT ToolQueryWidth(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{ TOOLCTL *tcl;          // Wska�nik do g��wnej struktury kontrolnej
  TOOLADJ  tadj;         // Struktura wype�niana przez funkcje okre�laj�ce rozmiary
  ULONG    options;      // Opcje steruj�ce odczytem szeroko�ci

  tcl = ToolLoadData(hwnd, "ToolQueryWidth");
  if(tcl == NULL) return(0);
  // Inicjacja struktury TOOLADJ
  tadj.hwndToolBar = hwnd;
  tadj.hwndParent  = WinQueryWindow(hwnd, QW_PARENT);
  tadj.flAttrs = tcl->flToolAttrs;
  tadj.flState = tcl->flToolState;

  // Odczyt opcji
  options = LONGFROMMP(mp1);
  if(options != TDQ_CURRENT)
  { tadj.flState &= ~(TST_MINIMIZED | TST_FIXED);
    if(options == TDQ_MINIMIZED) tadj.flState |= TST_MINIMIZED;
    if(options == TDQ_FIXED)     tadj.flState |= TST_FIXED;
  }

  // Odczyt szeroko��i paska
  return(MRFROMLONG(CrrQueryWidth(&tadj)));
}





// Ustawienie parametr�w okna (obs�uga komunikatu WM_SETWINDOWPARAMS)
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz�dzi
//   mp1       - [parametr] wska�nik do struktury WNDPARAMS
//
// Powr�t:
//   TRUE  - poprawne wykonanie funkcji
//   FALSE - b��d zmiany parametr�w
//
static MRESULT ToolSetParams(HWND hwnd, MPARAM mp1)
{ WNDPARAMS *wpm;   // Wska�nik do struktury zawieraj�cej zmieniane parametry
  TOOLCTL   *tcl;   // Wska�nik do g��wnej struktury kontrolnej okna

  // Inicjacja zmiennych
  tcl = ToolLoadData(hwnd, "ToolSetParams");
  if(tcl == NULL) return(MRFROMLONG(FALSE));
  wpm = (WNDPARAMS *)mp1;

  // �adowanie danych Class-Specific
  if(wpm->fsStatus & WPM_CTLDATA)
  {
    // Zapami�tanie danych
    StoreClassData(tcl, (TOOLINFO *)wpm->pCtlData);
    // Korekta wymiar�w okna
    WinSendMsg(tcl->hwndToolBar, TM_AUTOSIZE, 0, 0);
  }

  // �adowanie tytu�u okna
  if(wpm->fsStatus & WPM_TEXT)
  {
    // Zapami�tanie tekstu
    StoreWindowText(tcl, wpm->pszText);
  }

  return(MRFROMLONG(TRUE));
}





// Usuni�cie struktur kontrolnych paska narz�dzi
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna
//
static VOID ToolDestroy(HWND hwnd)
{ TOOLCTL *tcl;          // G��wna struktura kontrolna paska narz�dzi

  // Odczyt adresu struktury kontrolnej
  tcl = ToolLoadData(hwnd, "ToolDestroy");
  if(tcl == NULL) return;

  // Usuni�cie zasob�w zwi�zanych z obiektami umieszczonymi na pasku
  ObjReleaseResources(tcl);

  // Usuni�cie paska tytu�u
  if(tcl->hwndTitleBar != NULLHANDLE)
    WinDestroyWindow(tcl->hwndTitleBar);
  // Usuni�cie okna podpowiedzi
  if(tcl->hwndToolTip != NULLHANDLE)
    WinDestroyWindow(tcl->hwndToolTip);
  // Usuni�cie przycisku obracania okna
  if(tcl->hwndRotate != NULLHANDLE)
    WinDestroyWindow(tcl->hwndRotate);

  // Zwolnienie pami�ci zajmowanej przez tekst okna
  if(tcl->szWinText != NULL) free(tcl->szWinText);
  // Zwolnienie pami�ci zajmowanej przez struktury kontrolne
  free(tcl);
}





// Inicjacja paska narz�dzi
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna
//   ctldata   - [parametr] dane class-specific
//   ctr       - [parametr] struktura informacyjna przekazywana przy tworzeniu okna
//
// Powr�t:
//   FALSE - poprawne wykonanie funkcji
//   TRUE  - b��d inicjacji
//
static LONG ToolInit(HWND hwnd, VOID *ctldata, CREATESTRUCT *ctr)
{ TOOLCTL *tcl;          // G��wna struktura kontrolna paska narz�dzi


  // Alokacja pami�ci dla struktur kontrolnych
  tcl = malloc(sizeof(TOOLCTL));
  if(tcl == NULL) return(TRUE);
  memset(tcl, 0, sizeof(TOOLCTL));

  // Zapami�tanie wska�nika w zmiennych okna
  WinSetWindowPtr(hwnd, 0L, tcl);

  // Uchwyt okna
  tcl->hwndToolBar = hwnd;
  // Style okna
  tcl->flWinStyle = ctr->flStyle;

  // Zapami�tanie tekstu wy�wietlanego w oknie
  StoreWindowText(tcl, ctr->pszText);
  // Zapami�tanie presentation parameters
  if(ctr->pPresParams != NULL)
    PpmStorePresParams(hwnd, ctr->pPresParams, NULL, 0);
  // Zapami�tanie danych class-specific w strukturach kontrolnych
  StoreClassData(tcl, ctr->pCtlData);

  // Inicjacja no�nika
  if(CrrInitialize(hwnd, tcl)) return(TRUE);
  // Inicjacja modu�u Object Manager
  if(ObjInitialize(tcl) == TRUE) return(TRUE);

  // Poprawne wykonanie funkcji
  return(FALSE);
}





/******************************/
/* Dodatkowe funkcje us�ugowe */
/******************************/

// Zapami�tanie danych class-specific w wewn�trznych strukturach okna
//
// Parametry:
//   tcl       - [rezultat] adres g��wnej struktury kontrolnej paska narz�dzi
//   cdata     - [parametr] wska�nik do danych class-specific paska
//
static VOID  StoreClassData(TOOLCTL *tcl, TOOLINFO *cdata)
{ LONG  dsize;      // Rozmiar danych class-specific

  // Obliczenie rozmiaru struktury danych
  if(cdata == NULL) dsize = 0;
  else              dsize = cdata->cb;

  // Opuszczenie pola okre�laj�cego rozmiar struktury
  dsize -= sizeof(cdata->cb);
  // Odczyt i zapami�tanie atrybut�w paska narz�dzi (je�li s�)
  if(dsize >= (LONG)sizeof(cdata->flToolAttrs))
  { tcl->flToolAttrs = cdata->flToolAttrs;
    dsize -= sizeof(cdata->flToolAttrs);
  }
  else return;
  // Korekta bit�w stanu okna w zale�no�ci od przekazanych atrybut�w
  if(tcl->flToolAttrs & TBA_VERTICAL)
    tcl->flToolState |= TST_ROTATED;

  // Odczyt i zapami�tanie maksymalnej d�ugo�ci paska (je�li jest)
  if(dsize >= (LONG)sizeof(cdata->usMaxLength))
  { tcl->ulMaxLength = cdata->usMaxLength;
    dsize -= sizeof(cdata->usMaxLength);
  }
  else return;

  // ***** TEST *****
  // tcl->flToolState |= TST_FULLTRCFRAME;
}





// Zapami�tanie tekstu w zmiennych kontrolnych okna
//
// Parametry:
//   tcl       - [rezultat] adres g��wnej struktury kontrolnej okna
//   text      - [parametr] wska�nik do zapami�tywanego ci�gu znak�w
//
static VOID StoreWindowText(TOOLCTL *tcl, PSZ text)
{ LONG tlng;        // D�ugo�� �a�cucha tekstowego

  // Odczyt d�ugo�ci �a�cucha tekstowego
  if(text == NULL) tlng = 0;
  else             tlng = strlen(text) + 1;

  if(tlng == 0)
  { // Zwolnienie pami�ci zajmowanej przez tekst
    if(tcl->szWinText != NULL)
      free(tcl->szWinText);
    tcl->szWinText = NULL;
    return;
  }
  else
  { // Alokacja pami�ci "od zera"
    if(tcl->szWinText == NULL)
    { tcl->szWinText = malloc(tlng);
      // B��d alokacji
      if(tcl->szWinText == NULL) return;
    }
    else
    { PSZ tmp;      // Wska�nik pomocniczy

      // Zmiana rozmiaru zaalokowanego obszaru pami�ci
      tmp = realloc(tcl->szWinText, tlng);
      if(tmp == NULL)
      { free(tcl->szWinText);
        tcl->szWinText = NULL;
        return;
      }
      // Zapami�tanie adresu nowego obszaru
      tcl->szWinText = tmp;
    }
  }
  // Zapami�tanie tekstu
  strcpy(tcl->szWinText, text);
}





// Aktualizacja paramer�w prezentacji okna podpowiedzi
//
// Parametry:
//   ToolBar   - [parametr] uchwyt paska narz�dzi - okno zawiera parametry �r�d�owe
//   ToolTip   - [parametr] uchwyt okna podpowiedzi
//
static VOID UpdateTipPParams(HWND ToolBar, HWND ToolTip)
{ LONG  color;     // Odczytana warto�� koloru
  UCHAR font[32];  // Nazwa i rozmiar czcionki
  LONG  rc;        // Kody zwracane przez funkcje systemowe


  // �adowanie kolor�w do pami�ci okna podpowiedzi
  // Zablokowanie od�wie�ania okna
  WinEnableWindowUpdate(ToolTip, FALSE);

  // Za�adowanie nowych kolor�w
  PpmQueryPresColors(ToolBar, 1, PPmColor + TOOL_TIPBACKGROUND, &color);
  WinSetPresParam(ToolTip, PP_BACKGROUNDCOLOR,  4, &color);
  PpmQueryPresColors(ToolBar, 1, PPmColor + TOOL_TIPTEXTCOLOR, &color);
  WinSetPresParam(ToolTip, PP_FOREGROUNDCOLOR,  4, &color);
  PpmQueryPresColors(ToolBar, 1, PPmColor + TOOL_TIPFRAMECOLOR, &color);
  WinSetPresParam(ToolTip, PP_BORDER2DARKCOLOR, 4, &color);

  // Pr�ba odczytu nazwy czcionki
  rc = WinQueryPresParam(ToolBar, PP_TOOLTIPFONT, 0L, NULL, 32L, font, 0L);
  // Programowanie czcionki odczytanej lub domy�lenj
  if(!rc) WinSetPresParam(ToolTip, PP_FONTNAMESIZE, 7, "8.Helv");
  else    WinSetPresParam(ToolTip, PP_FONTNAMESIZE, rc, font);

  // Odblokowanie od�wie�ania okna
  WinEnableWindowUpdate(ToolTip, TRUE);
}

/*
 * $Log: toolbar.c $
 * Revision 1.1  1999/06/27 12:35:32  Wojciech_Gazda
 * Initial revision
 *
 */
