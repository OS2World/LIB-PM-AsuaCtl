/* Asuactl.dll
 * Pasek narz©dzi - no˜nik
 * (c) 1999 Wojciech Gazda
 *
 * carrier.c
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/06/27 12:35:43 $
 * $Name:  $
 * $RCSfile: carrier.c $
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


#define  INCL_DOSDEV
#define  INCL_DOSPROCESS

// Deklaracje OS/2
#define  INCL_GPILOGCOLORTABLE
#define  INCL_GPIPRIMITIVES
#define  INCL_GPIREGIONS
#define  INCL_WINRECTANGLES
#define  INCL_WINWINDOWMGR
#define  INCL_WINTRACKRECT
#define  INCL_WINFRAMEMGR
#define  INCL_WINPOINTERS
#define  INCL_WININPUT
#define  INCL_WINATOM
#define  INCL_WINSYS
#include <os2.h>


// Deklaracje lokalne
#define  __INTERNAL_USE__
#include "asuintl.h"
#include "toolbar.h"
#include "tooldefs.h"
#include "tcontrol.h"
#include "carrier.h"
#include "objmgr.h"



// Prototypy funkcji
VOID  CrrCalcNewRect(TOOLADJ *tadj);
LONG  CrrInitialize(HWND hwnd, TOOLCTL *tcl);
LONG  CrrMinMaxFrame(HWND hwnd, SWP *newpos);
VOID  CrrMoveWithOwner(HWND hwnd, SWP *newpos, SWP *owner);
VOID  CrrPlaceControls(HWND hwnd);
LONG  CrrQueryHeight(TOOLADJ *tadj);
LONG  CrrQueryWidth(TOOLADJ *tadj);
VOID  CrrRedrawBorder(HWND hwnd);
LONG  CrrRotate(HWND hwnd, ULONG ulOptions);
LONG  CrrTrackRect(HWND hwnd);
VOID  CrrUpdateToolSize(TOOLCTL *tcl);


// Prototypy funkcji lokalnych
static LONG  CrrQueryBorder(TOOLADJ *tadj, ULONG mode);
static LONG  CrrQueryControl(TOOLADJ *tadj, ULONG mode);
static VOID  CrrQueryClientArea(TOOLADJ *tadj);
static LONG  CrrQueryEndMargin(TOOLADJ *tadj);
static VOID  CrrQueryInterior(TOOLADJ *tadj);

// Dodatkowe funkcje usˆugowe
static VOID  FrameOnlyDrag(TOOLCTL *tcl, TOOLTRACK *trc);
static VOID  FullWindowDrag(TOOLCTL *tcl, TOOLTRACK *trc);
static VOID  InvalidateParent(HWND hwnd, HWND oldparent, RECTL *oldpos);
static VOID  PlaceRotateButton(TOOLCTL *tcl, RECTL *rclInter);
static VOID  PlaceTitleBar(TOOLCTL *tcl, RECTL *rclInter);
static VOID  StoreOffset(TOOLCTL *tcl, RECTL *winpos);
static VOID  UpdateBeforeDrag(TOOLCTL *tcl, TOOLTRACK *trc, POINTL *offset);





// Funkcja oblicza proponowan¥ pozycj© i rozmiary paska narz©dzi.
// Struktura TOOLADJ powinna przed wywoˆaniem mie† zinicjowane pola:
// hwndToolBar, hwndParent, flAttrs, flState.
// Pozycja i rozmiary s¥ zwracane w polu rclSizePos, i s¥ one
// liczone wzgl©dem okna rodzicielskiego
//
// Parametry:
//   tadj      - [parametr/rezultat] wska«nik do struktury zawiraj¥cej pytanie
//
VOID CrrCalcNewRect(TOOLADJ *tadj)
{ RECTL rclNew;     // Nowe wymiary paska
  LONG  cx, cy;     // Szeroko˜† i wysoko˜† paska

  // Odczyt proponowanych wymiar¢w paska narz©dzi
  cx = CrrQueryWidth(tadj);
  cy = CrrQueryHeight(tadj);
  // Odczyt poˆo¾enia i wymiar¢w okna rodzicielskiego
  WinQueryWindowRect(tadj->hwndParent, &rclNew);

  if(tadj->flState & TST_FIXED)
  { // Pasek jest zamksymalizowany
    if(tadj->flState & TST_ROTATED)
    { // Pasek jest w pozycji pionowej
      if(tadj->flAttrs & TBA_FIXEDRIGHT) rclNew.xLeft  = rclNew.xRight - cx;
      else                               rclNew.xRight = rclNew.xLeft  + cx;
    }
    else
    { // Pasek jest w pozycji poziomej
      if(tadj->flAttrs & TBA_FIXEDBOTTOM) rclNew.yTop    = rclNew.yBottom + cy;
      else                                rclNew.yBottom = rclNew.yTop    - cy;
    }
  }
  else if(tadj->flState & TST_MINIMIZED)
  { // Pasek jest zminimalizowany
    if(!(tadj->flAttrs & (TBA_FIXEDBOTTOM | TBA_FIXEDRIGHT)))
    { // Lewy g¢rny r¢g okna
      rclNew.xRight  = rclNew.xLeft + cx;
      rclNew.yBottom = rclNew.yTop  - cy;
    }
    else if((tadj->flAttrs & TBA_FIXEDBOTTOM) && (tadj->flAttrs & TBA_FIXEDRIGHT))
    { // Prawy dolny r¢g okna
      rclNew.xLeft   = rclNew.xRight  - cx;
      rclNew.yTop    = rclNew.yBottom + cy;
    }
    else if(tadj->flAttrs & TBA_FIXEDBOTTOM)
    { // Lewy dolny r¢g okna
      rclNew.xRight  = rclNew.xLeft   + cx;
      rclNew.yTop    = rclNew.yBottom + cy;
    }
    else if(tadj->flAttrs & TBA_FIXEDRIGHT)
    { // Prawy g¢rny r¢g okna
      rclNew.xLeft   = rclNew.xRight - cx;
      rclNew.yBottom = rclNew.yTop   - cy;
    }
  }
  else
  { // Pasek jest w stanie "floating"
    WinQueryWindowRect(tadj->hwndToolBar, &rclNew);
    WinMapWindowPoints(tadj->hwndToolBar, tadj->hwndParent, (POINTL *)&rclNew, 2);
    // Krekta wymiar¢w paska
    rclNew.xRight = rclNew.xLeft   + cx;
    rclNew.yTop   = rclNew.yBottom + cy;
  }
  // Zapmi¥tanie nowych wymiar¢w
  tadj->rclSizePos = rclNew;
}





// Inicjacja no˜nika
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   tcl       - [parametr/rezultat] adres gˆ¢wnej struktury kontrolnej okna
//
// Powr¢t:
//   FALSE - poprawne wykonanie funkcji
//   TRUE  - bˆ¥d inicjacji
//
LONG CrrInitialize(HWND hwnd, TOOLCTL *tcl)
{ HAB     hab;           // Uchwyt anchor block PM
//  LONG    rc;            // Kody bˆ©d¢w zwracane przez funkcje systemowe
//  ULONG   winstyle;      // Styl tworzonego okna


  // Odczyt anchor block PM
  hab = WinQueryAnchorBlock(hwnd);

  // Otwarcie nowego okna - paska tytuˆu
  if(tcl->flWinStyle & (TBS_MOVEBUTTON | TBS_FIXBUTTON))
  { tcl->hwndTitleBar = WinCreateWindow(hwnd, INTC_MINITITLE, "",
                          WS_VISIBLE, 0, 0, 0, 0,
                          hwnd, HWND_TOP, TCID_TITLEBAR,
                          NULL, NULL);
    if(tcl->hwndTitleBar == NULLHANDLE) return(TRUE);

    // Aktualizacja parametr¢w steruj¥cych wygl¥dem paska tytuˆu
    if(tcl->flWinStyle & TBS_FIXBUTTON)
      tcl->flTitleState |= STT_DRAWBUTTON;

    // Zapami©tanie wska«nika do struktur kontrolnych paska narz©dzi
    WinSetWindowPtr(tcl->hwndTitleBar, 0L, tcl);
  }

  // Otwarcie nowego okna - przycisku obracania paska narz©dzi
  if(tcl->flWinStyle & TBS_ROTATEBUTTON)
  { tcl->hwndRotate = WinCreateWindow(hwnd, INTC_ROTATEBTN, "",
                        WS_VISIBLE, 0, 0, 0, 0,
                        hwnd, HWND_TOP, TCID_ROTATE,
                        NULL, NULL);
    if(tcl->hwndRotate == NULLHANDLE) return(TRUE);

    // Zapami©tanie wska«nika do struktur kontrolnych paska narz©dzi
    WinSetWindowPtr(tcl->hwndRotate, 0L, tcl);
  }

  // Poprawne wykonanie funcji
  return(FALSE);
}





// Obsˆuga minimalzacji/maksymalizacji i przywr¢cenia stanu -
// komunikat WM_MINMAXFRAME.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   newpos    - [parametr/rezultat] - proponowane wymiary i poˆo¾enie okna
//
// Powr¢t:
//   TRUE - komunikat WM_MINMAXFRAME zostaˆ wykonany
//
LONG CrrMinMaxFrame(HWND hwnd, SWP *newpos)
{ TOOLCTL *tcl;     // Wska«nik do struktur kontrolnych paska narz©dzi
  TOOLADJ  tadj;    // Paarametry przekazywane wˆa˜cicielowi paska w celu dokonania korekt
  RECTL  oldpos;    // Stara pozycja okna
  HWND   oldparent; // Uchwyt okna rodzicielskiego przed dokonanim zmian
  LONG   oldstyle;  // Styl okna przed wywoˆaniem potwierdzenia TN_*
  ULONG  notify;    // Kod potwierdzenia


  tcl = ToolLoadData(hwnd, "ToolControlPos");
  if(tcl == NULL) return(0);
  notify   = 0;

  // Inicjacja struktury TOOLADJ
  tadj.hwndToolBar = hwnd;
  tadj.hwndParent  = WinQueryWindow(hwnd, QW_PARENT);
  tadj.flAttrs     = tcl->flToolAttrs;

  // Zapami©tania aktualnego stanu okna
  tcl->flOldState = tcl->flToolState;
  // Odczyt starej pozycji i rozmiar¢w okna
  WinQueryWindowRect(hwnd, &oldpos);
  WinMapWindowPoints(hwnd, tadj.hwndParent, (POINTL *)&oldpos, 2);
  oldparent = tadj.hwndParent;

  // Okre˜lenie rodzaju operacji
  if((newpos->fl & SWP_MINIMIZE) && !(tcl->flToolState & TST_MINIMIZED))
  { // Okre˜lenie kodu potwierdzenia - minmalizacja
    notify = TN_MINIMIZE;
  }
  else if((newpos->fl & SWP_MAXIMIZE) && !(tcl->flToolState & TST_FIXED))
  { // maksymalizacja okna
    notify = TN_FIXTOOL;
  }
  else if((newpos->fl & SWP_RESTORE) && (tcl->flToolState & (TST_FIXED | TST_MINIMIZED)))
  { // Przywr¢cenie stanu okna
    notify = TN_RESTORE;
  }

  // Wykonanie czynno˜ci zale¾nych od stanu okna
  if(notify)
  { // Korekta wewn©trznych zmiennych okna i struktur kontrolnych
    // Skasowanie znacznik¢w maksymalizacji i minimalizacji
    tcl->flWinStyle  &= ~(WS_MINIMIZED  | WS_MAXIMIZED);
    tcl->flToolState &= ~(TST_MINIMIZED | TST_FIXED);

    // Korekta podczas maksymalizacji
    if(notify == TN_FIXTOOL)
    { tcl->flWinStyle  |= WS_MAXIMIZED;
      tcl->flToolState |= TST_FIXED;
    }
    // Korekta podczas minimalizacji
    if(notify == TN_MINIMIZE)
    { tcl->flWinStyle  |= WS_MINIMIZED;
      tcl->flToolState |= TST_MINIMIZED;
    }

    // Zapami©tanie nowych parametr¢w w strukturach okna
    WinSetWindowULong(hwnd, QWL_STYLE, tcl->flWinStyle);
    // Obliczenie proponowanych wymiar¢w i pozycji paska narz©dzi
    tadj.flState = tcl->flToolState;
    WinSendMsg(hwnd, TM_CALCNEWRECT, MPFROMP(&tadj), 0L);

    // Zapami©tanie pozycji do odtworzenia
    if((notify == TN_MINIMIZE) || (notify == TN_FIXTOOL))
    { if(!(tcl->flOldState & (TST_FIXED | TST_MINIMIZED)))
      { WinSetWindowUShort(hwnd, QWS_XRESTORE, oldpos.xLeft);
        WinSetWindowUShort(hwnd, QWS_YRESTORE, oldpos.yTop);
      }
    }
    else
    { // Korekta proonowanej pozycji okna
      tadj.rclSizePos.xRight -= tadj.rclSizePos.xLeft;
      tadj.rclSizePos.yBottom = tadj.rclSizePos.yTop - tadj.rclSizePos.yBottom;
      // Odczyt pozycji potrzebnej do odtworzenia stanu okna (TN_RESTORE)
      tadj.rclSizePos.xLeft   = WinQueryWindowUShort(hwnd, QWS_XRESTORE);
      tadj.rclSizePos.yTop    = WinQueryWindowUShort(hwnd, QWS_YRESTORE);
      tadj.rclSizePos.xRight += tadj.rclSizePos.xLeft;
      tadj.rclSizePos.yBottom = tadj.rclSizePos.yTop - tadj.rclSizePos.yBottom;
    }

    // Zapami©tanie stylu okna
    oldstyle = WinQueryWindowULong(hwnd, QWL_STYLE);
    // Blokada przerysowywania paska narz©dzi na czas przetwarzania TN_*
    WinEnableWindowUpdate(hwnd, FALSE);

    // Ustawienie znacznika trwania minimalizacji
    tcl->flToolState |= TST_MINMAXCTL;
    // Wysˆanie potwierdzenia do wˆa˜ciciela
    tadj.flState = tcl->flOldState & 0xFFFF;
    ToolSendNotify(hwnd, notify, &tadj);
    // Uwzgl©dnienie zmiany atrybut¢w
    tcl->flToolAttrs = tadj.flAttrs;
    // Skasowanie znacznika trwania minimalizacji
    tcl->flToolState &= ~TST_MINMAXCTL;

    // Sprawdzenie czy okno byˆo widoczne przed wysˆaniem potwierdzenie
    if(oldstyle & WS_VISIBLE)
    { // Wymuszenie przerysowania obszaru okna rodzicielskiego zakrywanego przez pasek narz©dzi
      WinInvalidateRect(oldparent, &oldpos, TRUE);
      // Odblokowanie przerysowywania okna
      WinEnableWindowUpdate(hwnd, TRUE);
      // Obszar uniewa¾niony zostanie obliczony podczas przetwarzania WM_CALCVALIDRECTS
      WinValidateRect(hwnd, NULL, TRUE);
    }

    // Zapami©tanie przesuni©cia paska narz©dzi wzgl©dem wˆa˜ciciela
    if(notify == TN_RESTORE) StoreOffset(tcl, &tadj.rclSizePos);

    // Aktualizacja struktury SWP
    newpos->x   = tadj.rclSizePos.xLeft;
    newpos->y   = tadj.rclSizePos.yBottom;
    newpos->cx  = tadj.rclSizePos.xRight - tadj.rclSizePos.xLeft;
    newpos->cy  = tadj.rclSizePos.yTop   - tadj.rclSizePos.yBottom;
    // Wymuszenie przesuni©cia, skalowania, oraz odblokowanie od˜wie¾ania
    newpos->fl |= (SWP_MOVE | SWP_SIZE);
  }
  return(0);
}





// Pozycjonowanie paska narz©dzi wzgl©dem okna rodzicielskiego,
// w zale¾no˜ci od stylu TBS_MOVEWITHOWNER. Procedura ukrywa pasek
// narz©dzi podczas minimalizacji lub ukrywania okna aplikacji.
// Parametr tool jest wska«nikiem do struktury SWP zawieraj¥cej
// proponowan¥ pozycj© i wymiary paska narz©dzi.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   tool      - [parametr/rezultat] wska«nik struktury SWP zawieraj¥cej nowe poˆo¾enie paska
//   owner     - [parametr] nowa pozycja i wymiary wˆa˜ciciela
//
VOID CrrMoveWithOwner(HWND hwnd, SWP *tool, SWP *owner)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury kontrolnej

  tcl = ToolLoadData(hwnd, "CrrMoveWithOwner");
  if(tcl == NULL) return;

  // Ukrycie okna przy minimalizacji/ukryciu wˆa˜ciciela
  if(owner->fl & (SWP_HIDE | SWP_MINIMIZE))
  { WinShowWindow(hwnd, FALSE);
    return;
  }
  // Wy˜wietlenie paska przy maksymalizacji/przywr¢ceniu okna
  if(owner->fl & (SWP_RESTORE | SWP_MAXIMIZE | SWP_SHOW))
  { WinShowWindow(hwnd, TRUE);
  }

  // Przesuwanie okna wraz z wˆa˜cicielem
  if((tcl->flWinStyle & TBS_MOVEWITHOWNER) && (owner->fl & SWP_MOVE))
  { // Korekta poˆo¾enia paska narz©dzi
    tool->x = owner->x + tcl->ptlOffset.x;
    tool->y = owner->y + owner->cy - tcl->ptlOffset.y;
    tool->fl |= SWP_MOVE;
  }
}





// Pozycjonowanie okien steruj¥cych zachowaniem paska, takich jak:
// pasek tytuˆu, przycisk obracania i przyciski przewijania.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//
VOID CrrPlaceControls(HWND hwnd)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
  TOOLADJ  tadj;    // Struktura zawieraj¥ca pytanie o stan paska narz©dzi

  tcl = ToolLoadData(hwnd, "CrrPlaceControls");
  if(tcl == NULL) return;

  // Odczyt obszaru wewn¥trz obramowania
  tadj.hwndToolBar = hwnd;
  tadj.flAttrs     = tcl->flToolAttrs;
  tadj.flState     = tcl->flToolState;
  CrrQueryInterior(&tadj);

  // Pozycjonowanie paska tytuˆu
  PlaceTitleBar(tcl, &tadj.rclSizePos);
  // Pozycjonowanie przycisku obracania okna
  PlaceRotateButton(tcl, &tadj.rclSizePos);
  // Pozycjonowanie pocz¥tkowego przycisku przesuwania okna


  // Obliczenie wsp¢ˆrz©dnych obszaru okna przechowuj¥cego obiekty
  CrrQueryClientArea(&tadj);
  // Pozycjonowanie okna przechowuj¥cego obiekty
  if(tcl->flToolState & TST_MINIMIZED)
  { // Zgaszenie okna w stanie zminimalizowanym
    WinShowWindow(tcl->hwndObjectPad, FALSE);
  }
  else
  { WinSetWindowPos(tcl->hwndObjectPad, HWND_TOP,
                    tadj.rclSizePos.xLeft,
                    tadj.rclSizePos.yBottom,
                    tadj.rclSizePos.xRight - tadj.rclSizePos.xLeft,
                    tadj.rclSizePos.yTop   - tadj.rclSizePos.yBottom,
                    SWP_MOVE | SWP_SIZE | SWP_ZORDER | SWP_SHOW);
  }
}





// Funkcja oblicza wysoko˜† paska narz©dzi na podstawie danych steruj¥cych
// przekazanych w strukturze TOOLADJ. Powinny zosta† zainicjowane pola:
// hwndToolBar, hwndParent, flState, flAttrs.
//
// Parametry:
//   tadj      - [parametr] struktura zawieraj¥ca pytanie o wysoko˜†
//
// Powr¢t:
//   Proponowana wysoko˜† paska narz©dzi
//
LONG CrrQueryHeight(TOOLADJ *tadj)
{ TOOLCTL *tcl;     // Adres gˆ¢wnej struktury kontrolnej paska narz©dzi
  RECTL    wsize;   // Wymiary okna rodzicielskiego
  LONG     height;  // Wysoko˜† paska narz©dzi
  LONG     minh;    // Minimalna wysoko˜† paska narz©dzi

  // Odczyt afresu struktur kontrolnych
  tcl = ToolLoadData(tadj->hwndToolBar, "CrrQueryHeight");
  if(tcl == NULL) return(0);

  if(tadj->flState & TST_MINIMIZED)
  { // Odczyt wysoko˜ci dla paska zminimalizowanego

    if(tadj->flState & TST_ROTATED)
    { // Pionowy pasek narz©dzi
      height = ObjQueryWidth(tcl, tadj->flState);
      // Powi©kszenie rozmiar¢w o 50%
      height += (height >> 1);
    }
    else
    { // Poziomy pasek narz©dzi
      height = CY_TITLEHEIGHT;
    }

    // Uwzgl©dnienie grubo˜ci ramki
    tadj->flState |= TST_MINIMIZED;
    height += CrrQueryBorder(tadj, TDQ_HEIGHT);
    return(height);
  }
  else
  { if((tadj->flState & TST_FIXED) && (tadj->flState & TST_ROTATED))
    { // Odczyt wysoko˜ci dla paska zmaksymalizowanego i umieszczonego pionowo
      WinQueryWindowRect(tadj->hwndParent, &wsize);
      height = wsize.yTop - wsize.yBottom;
    }
    else
    { // Odczyt wysoko˜ci dla pozostaˆych stan¢w
      height = ObjQueryHeight(tcl, tadj->flState) + CXY_INTERNALMARGIN;
      // Uwzgl©dnienie wymiar¢w ramki
      minh   = CrrQueryBorder(tadj, TDQ_HEIGHT);

      // Tylko pionowy pasek narz©dzi
      if(tadj->flState & TST_ROTATED)
      { // Wysoko˜† tytuˆu i standardowych przycisk¢w steruj¥cych
        minh   += CrrQueryControl(tadj, TDQ_HEIGHT);
        height += minh;

        // Ograniczenie maksymalnej wysoko˜ci w stanie floating
        if((tcl->ulMaxLength > 0) && (height > tcl->ulMaxLength))
        { height = tcl->ulMaxLength;
          if(height < minh) height = minh;
        }
      }
      else height += minh;
    }
  }

  return(height);
}





// Funkcja oblicza poˆo¾enie i rozmiary obszaru zawartego wewn¥trz obramowania.
// Struktura TOOLADJ powinna mie† zainicjowane pola: hwndToolBar, flAttrs i flState.
// Obliczone wsp¢ˆrz©dne obszaru s¥ zwracane w polu rclSizePos, natomiast skorygowane
// atrybuty w polu flAttrs.
//
// Parametry:
//   tadj      - [parametr/rezultat] wska«nik do struktury zawieraj¥cej pytanie
//
VOID CrrQueryInterior(TOOLADJ *tadj)
{
  // Korekta wygl¥du ramki podczas przesuwania paska narz©dzi
  if(tadj->flState & TST_FULLTRCFRAME)
  {
    // Dla zminimalizowanego paska jest rysowane tylko czarne obramowanie
    if(tadj->flState & TST_MINIMIZED)
      tadj->flAttrs &= ~TBA_FRAME;
    // W pozostaˆych przypadkach - peˆne obramowanie 3D
    else tadj->flAttrs |= TBA_FRAME;
  }
  else
  {
    // Korekta atrybut¢w, gdy ramka znajduje si© w stanie floating - peˆne obramowanie
    if(!(tadj->flState & (TST_FIXED | TST_MINIMIZED)))
      tadj->flAttrs |= TBA_FRAME;
  }

  // Obliczenie obszaru wewn¥trz ramki
  // Odczyt peˆnych wymiar¢w okna
  WinQueryWindowRect(tadj->hwndToolBar, &tadj->rclSizePos);
  // Uwzgl©dnienie obramowania rysowanego podczas przesuwania,
  // oraz gdy pasek jest w stanie floating
  if((tadj->flState & TST_FULLTRCFRAME) || !(tadj->flState & (TST_FIXED | TST_MINIMIZED)))
  { tadj->rclSizePos.xLeft  ++; tadj->rclSizePos.yBottom ++;
    tadj->rclSizePos.xRight --; tadj->rclSizePos.yTop    --;
  }

  // Uwzgl©dnienie standardowego obramowania
  if(tadj->flAttrs & TBA_LEFTFRAME)   tadj->rclSizePos.xLeft   ++;
  if(tadj->flAttrs & TBA_RIGHTFRAME)  tadj->rclSizePos.xRight  --;
  if(tadj->flAttrs & TBA_BOTTOMFRAME) tadj->rclSizePos.yBottom ++;
  if(tadj->flAttrs & TBA_TOPFRAME)    tadj->rclSizePos.yTop    --;
}





// Funkcja oblicza szeroko˜† paska narz©dzi na podstawie danych steruj¥cych
// przekazanych w strukturze TOOLADJ. Powinny zosta† zainicjowane pola:
// hwndToolBar, hwndParent, flState, flAttrs.
//
// Parametry:
//   tadj      - [parametr] struktura zawieraj¥ca pytanie o wysoko˜†
//
// Powr¢t:
//   Proponowana szeroko˜† paska narz©dzi
//
LONG CrrQueryWidth(TOOLADJ *tadj)
{ TOOLCTL *tcl;     // Adres struktur kontrolnych paska narz©dzi
  RECTL    wsize;   // Wymiary okna rodzicielskiego
  LONG     width;   // Szeroko˜† paska narz©dzi
  LONG     minw;    // Minimalna szeroko˜† w stanie floating

  // Odczyt adresu struktur kontrolnych
  tcl = ToolLoadData(tadj->hwndToolBar, "CrrQueryWidth");
  if(tcl == NULL) return(0);

  if(tadj->flState & TST_MINIMIZED)
  { // Odczyt szeroko˜ci dla paska zminimalizowanego
    if(tadj->flState & TST_ROTATED)
    { // Pionowy pasek narz©dzi
      width = CX_TITLEWIDTH;
    }
    else
    { // Poziomy pasek narz©dzi
      width = ObjQueryHeight(tcl, tadj->flState);
      // Powi©kszenie rozmiar¢w o 50%
      width += (width >> 1);
    }
    // Uwzgl©dnienie grubo˜ci ramki
    tadj->flState |= TST_MINIMIZED;
    width += CrrQueryBorder(tadj, TDQ_WIDTH);
    return(width);
  }
  else
  { if((tadj->flState & TST_FIXED) && !(tadj->flState & TST_ROTATED))
    { // Odczyt szeroko˜†i dla paska zmaksymalizowanego i nieodwr¢conego
      WinQueryWindowRect(tadj->hwndParent, &wsize);
      width = wsize.xRight - wsize.xLeft;
    }
    else
    { // Odczyt szeroko˜ci dla pozostaˆych stan¢w
      width = ObjQueryWidth(tcl, tadj->flState) + CXY_INTERNALMARGIN;
      // Uwzgl©dnienie wymiar¢w ramki
      minw  = CrrQueryBorder(tadj, TDQ_WIDTH);

      // Tylko poziomy pasek narz©dzi
      if(!(tadj->flState & TST_ROTATED))
      { // Szeroko˜† tytuˆu i standardowych przycisk¢w steruj¥cych
        minw  += CrrQueryControl(tadj, TDQ_WIDTH);
        width += minw;

        // Ograniczenie maksymalnej szeroko˜ci paska w stanie floating
        if((tcl->ulMaxLength > 0) && (width > tcl->ulMaxLength))
        { width = tcl->ulMaxLength;
          if(width < minw) width = minw;
        }
      }
      else width += minw;
    }
  }
  return(width);
}





// Przerysowanie obramowania paska narz©dzi
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//
VOID  CrrRedrawBorder(HWND hwnd)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnej struktury kontrolnej okna
  POINTL   pos;     // Wsp¢ˆrz©dne og¢lnego przeznaczenia
  RECTL    wsize;   // Rozmiary okna
  ULONG    frame;   // Skorygowane atrybuty ramki
  HPS      hps;     // Uchwyt presentation space

  tcl = ToolLoadData(hwnd, "CrrRedrawBorder");
  if(tcl == NULL) return;
  hps = WinBeginPaint(hwnd, NULL, NULLHANDLE);

  // adowanie tablicy kolor¢w
  PpmQueryPresColors(hwnd, TOOL_MAXCOLOR, PPmColor, tcl->colors);
  // Tworzenie logicznej tablicy kolor¢w
  GpiCreateLogColorTable(hps, 0L, LCOLF_CONSECRGB, 0L, TOOL_MAXCOLOR, tcl->colors);
  // Odczyt wymiar¢w paska narz©dzi
  WinQueryWindowRect(tcl->hwndToolBar, &wsize);
  // Zapami©tanie atrybut¢w steruj¥cych wy˜wietlaniem ramki
  frame = tcl->flToolAttrs;

  // **** TEST   ****
  // WinFillRect(hps, &wsize, TOOL_HILITEBACKGROUND);
  // DosSleep(50);
  // **** KONIEC ****

  // Rysowanie ramki u¾ywanej podczas przesuwania okna,
  // i gdy pasek jest w stanie "floating"
  if((tcl->flToolState & TST_FULLTRCFRAME) || !(tcl->flToolState & (TST_MINIMIZED | TST_FIXED)))
  {
    // O˜wietlenie ramki
    GpiSetColor(hps, TOOL_FRAMEDRAGHILITE);
    pos.x = wsize.xLeft; pos.y = wsize.yBottom;
    GpiMove(hps, &pos);
    // Lewa kraw©d«
    pos.y = wsize.yTop - 1;   GpiLine(hps, &pos);
    // G¢rna kraw©d«
    pos.x = wsize.xRight - 2; GpiLine(hps, &pos);

    // Cieä ramki
    GpiSetColor(hps, TOOL_FRAMEDRAGDARK);
    pos.x = wsize.xRight - 1; pos.y = wsize.yTop - 1;
    GpiMove(hps, &pos);
    // Prawa kraw©d«
    pos.y = wsize.yBottom;    GpiLine(hps, &pos);
    // Dolna kraw©d«
    pos.x = wsize.xLeft  + 1; GpiLine(hps, &pos);

    // Korekta wymiar¢w obszaru
    wsize.xLeft  ++; wsize.yBottom ++;
    wsize.xRight --; wsize.yTop    --;
    // Korekta atrybut¢w ramki - rysowanie peˆnej ramki w stanie "floating"
    // i podczas przesuwania paska narz©dzi
    frame |= TBA_FRAME;
  }


  // Rysowanie standardowej ramki
  // O˜wietlenie ramki
  GpiSetColor(hps, TOOL_FRAMEHILITE);
  pos.x = wsize.xLeft; pos.y = wsize.yBottom;
  GpiMove(hps, &pos);
  // Lewa kraw©d«
  pos.y = wsize.yTop - 1;
  if(frame & TBA_LEFTFRAME)
  { GpiLine(hps, &pos);
    wsize.xLeft ++;
  }
  else GpiMove(hps, &pos);
  // G¢rna kraw©d«
  pos.x = wsize.xRight - 1;
  if(frame & TBA_TOPFRAME)
  { GpiLine(hps, &pos);
    wsize.yTop --;
  }
  else GpiMove(hps, &pos);

  // Cieä ramki
  GpiSetColor(hps, TOOL_FRAMEDARK);
  pos.x = wsize.xRight - 1; pos.y = wsize.yTop - 1;
  GpiMove(hps, &pos);
  // Prawa kraw©d«
  pos.y = wsize.yBottom;
  if(frame & TBA_RIGHTFRAME)
  { GpiLine(hps, &pos);
    wsize.xRight --;
  }
  else GpiMove(hps, &pos);
  // Dolna kraw©d«
  pos.x = wsize.xLeft;
  if(frame & TBA_BOTTOMFRAME)
  { GpiLine(hps, &pos);
    wsize.yBottom ++;
  }

  // Wypeˆnienie tˆa
  WinFillRect(hps, &wsize, TOOL_BACKGROUND);
  // Koniec rysowania
  WinEndPaint(hps);
}





// Sterowanie obracaniem paska narz©dzi
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   ulOptions - [parametr] opcje steruj¥ce obrotem
//
// Powr¢t
//   0
//
LONG CrrRotate(HWND hwnd, ULONG ulOptions)
{ TOOLCTL *tcl;          // Wska«nik do gˆ¢wnej struktury kontrolnej okna
  TOOLADJ  tadj;         // Struktura zawieraj¥ca proponowane parametry okna
  RECTL    oldpos;       // Poprzednie poˆo¾enie paska nrz©dzi
  HWND     oldparent;    // Poprzednie okno rodzicielskie
  ULONG    oldstyle;     // Poprzedni styl okna


  tcl = ToolLoadData(hwnd, "CrrRotate");
  if(tcl == NULL) return(0);

  // Zmiana stanu okna na przeciwny
  if(ulOptions == TBR_ROTATE)
  { tcl->flToolState ^= TST_ROTATED;
  }
  else if(ulOptions == TBR_VERTICAL)
  { if(tcl->flToolState & TST_ROTATED) return(0);
    // Obr¢cenie ona do pozycji pionowej
    tcl->flToolState |= TST_ROTATED;
  }
  else if(ulOptions == TBR_HORIZONTAL)
  { if(!(tcl->flToolState & TST_ROTATED)) return(0);
    // Obr¢cenie okna do pozycji poziomej
    tcl->flToolState &= ~TST_ROTATED;
  }
  else return(0);

  // Inicjacja struktury TOOLADJ
  tadj.hwndToolBar = hwnd;
  tadj.hwndParent  = WinQueryWindow(hwnd, QW_PARENT);
  tadj.flAttrs     = tcl->flToolAttrs;
  tadj.flState     = tcl->flToolState & 0xFFFF;

  // Zapamietanie uchwytu okna rodzicielskiego
  oldparent = tadj.hwndParent;
  // Odczyt i zapami©tanie aktualnych wymiar¢w okna
  WinQueryWindowRect(hwnd, &oldpos);
  WinMapWindowPoints(hwnd, oldparent, (POINTL *)&oldpos, 2);

  // Obliczenie nowej pozycji i wymiar¢w okna
  WinSendMsg(hwnd, TM_CALCNEWRECT, MPFROMP(&tadj), 0L);
  // Korekta nowoobliczonych obliczonych wymiar¢w dla stanu "floating"
  if(!(tcl->flToolState & (TST_MINIMIZED | TST_FIXED)))
  { WinOffsetRect(WinQueryAnchorBlock(hwnd), &tadj.rclSizePos,
                  0, oldpos.yTop - tadj.rclSizePos.yTop);
  }

  // Zapami©tanie stylu okna
  oldstyle = WinQueryWindowULong(hwnd, QWL_STYLE);
  // Blokada przerysowywania okna
  WinEnableWindowUpdate(hwnd, FALSE);

  // Wysˆanie potwierdzenie o obracaniu okna
  ToolSendNotify(hwnd, TN_ROTATE, &tadj);
  // Zapami©tanie nowych atrybut¢w
  tcl->flToolAttrs = tadj.flAttrs;

  // Sprawdzenie czy okno byˆo widoczne przed wysˆaniem komunikatu
  if(oldstyle & WS_VISIBLE)
  { // Wymuszenie od˜wie¾enia okna rodzicielskiego
    InvalidateParent(hwnd, oldparent, &oldpos);
    // Aktualizacja zawarto˜ci paska narz©dzi
    WinEnableWindowUpdate(hwnd, TRUE);
    // Obszar uniewa¾niony zostanie obliczony podczas przetwarzania WM_CALCVALIDRECTS
    WinValidateRect(hwnd, NULL, TRUE);
  }

  // Uaktualnienie poˆo¾enia i wymiar¢w paska narz©dzi
  // ale tylko wtedy gdy pasek nie jest wˆa˜nie minimalizowany/maksymalizowany
  // lub prznoszony w stan "floating"
  if(!(tcl->flToolState & TST_MINMAXCTL))
  { WinSetWindowPos(hwnd, HWND_TOP,
                    tadj.rclSizePos.xLeft,
                    tadj.rclSizePos.yBottom,
                    tadj.rclSizePos.xRight - tadj.rclSizePos.xLeft,
                    tadj.rclSizePos.yTop   - tadj.rclSizePos.yBottom,
                    SWP_MOVE | SWP_SIZE);
  }

  // Zapami©tanie offsetu wzgl©dem wˆa˜ciciela
  if(!(tcl->flToolState & (TST_MINIMIZED | TST_FIXED)))
    StoreOffset(tcl, &tadj.rclSizePos);
  return(0);
}





// Przesuwanie paska narz©dzi za pomoc¥ myszy
//
// Parametry:
//   hwnd      - [parametr] uchwyt przesuwanego okna (paska narz©dzi)
//
// Powr¢t:
//   TRUE  - operacja zakoäczona sukcesem
//   FALSE - bˆ¥d wykonania
//
LONG CrrTrackRect(HWND hwnd)
{ TOOLTRACK  trc;   // Struktura kontroluj¥ca przesuwanie paska narz©dzi
  TOOLCTL   *tcl;   // Adres struktury kontrolnej paska narz©dzi
  RECTL oldpos;     // Poprzednia pozycja paska narz©dzi
  HWND  oldparent;  // Uchwyt starego okna rodzicielskiego
  LONG  oldstyle;   // Styl okna przed wywoˆaniem potwierdzenia TN_*


  // Odczyt adresu struktur kontrolnych
  tcl = ToolLoadData(hwnd, "CrrTrackRect");
  if(tcl == NULL) return(FALSE);

  // Wst©pna inicjacja struktury TOOLTRACK
  trc.hwndToolBar = hwnd;
  trc.hwndParent  = WinQueryWindow(hwnd, QW_PARENT);
  trc.flState     = tcl->flToolState & 0xFFFF;
  trc.flAttrs     = tcl->flToolAttrs;
  // Odczyt poˆo¾enia paska wzgl©dem okna rodzicielskiego
  WinQueryWindowRect(hwnd, &trc.rclSizePos);
  WinMapWindowPoints(hwnd, trc.hwndParent, (POINTL *)&trc.rclSizePos, 2);
  // Zapami©tanie starych warto˜ci
  oldparent = trc.hwndParent;
  oldpos    = trc.rclSizePos;

  // Obliczenie obszaru ruchu
  if(tcl->flToolState & (TST_FIXED | TST_MINIMIZED))
  { // Pasek jest przyklejony do okna nadrz©dnego - blokada przesuwu
    trc.rclBoundary = trc.rclSizePos;
  }
  else
  { // Pasek mo¾e si© swobodnie porusza† wewn¥trz okna rodzicielskiego
    WinQueryWindowRect(trc.hwndParent, &trc.rclBoundary);
  }

  // Zapami©tanie stylu okna
  oldstyle = WinQueryWindowULong(hwnd, QWL_STYLE);
  // Blokada od˜wie¾ania okna na czas przetwarzania TN_BEGINTRACK
  WinEnableWindowUpdate(hwnd, FALSE);

  // Wysˆanie komunikatu WM_CONTROL do wˆa˜ciciela
  ToolSendNotify(hwnd, TN_BEGINTRACK, &trc);

  // Sprawdzenie czy okno byˆo widoczne przed wysˆaniem komunikatu
  if(oldstyle & WS_VISIBLE)
  { // Wymuszenie od˜wie¾enia okna rodzicielskiego
    InvalidateParent(hwnd, oldparent, &oldpos);
    // Aktualizacja zawarto˜ci paska narz©dzi
    WinEnableWindowUpdate(hwnd, TRUE);
    // Obszar uniewa¾niony zostanie obliczony podczas przetwarzania WM_CALCVALIDRECTS
    WinValidateRect(hwnd, NULL, TRUE);
  }

  // Ukrycie podpowiedzi przed rozpocz©ciem przesuwania
  if(tcl->hwndToolTip != NULLHANDLE)
    WinShowWindow(tcl->hwndToolTip, FALSE);

  // Sprawdzenie czy przesuwa† tylko ramk©
  if(ulWarpVer >= 40)
  { // Sprawdznie czy jest uaktywniona opcja "Full Window Drag"
    if(WinQuerySysValue(HWND_DESKTOP, SV_TRACKRECTLEVEL) != 0)
      // Przesuwanie caˆego okna w trybie "full window dragging"
      FullWindowDrag(tcl, &trc);
  }
  // Przesuwanie samej ramki
  else FrameOnlyDrag(tcl, &trc);

  // Zapami©tanie aktualnych parametr¢w okna
  oldparent = WinQueryWindow(hwnd, QW_PARENT);
  WinQueryWindowRect(hwnd, &oldpos);
  WinMapWindowPoints(hwnd, oldparent, (POINTL *)&oldpos, 2);
  // Zapami©tanie stylu okna
  oldstyle = WinQueryWindowULong(hwnd, QWL_STYLE);
  // Blokada od˜wie¾ania okna
  WinEnableWindowUpdate(hwnd, FALSE);

  // Wysˆanie komunikatu WM_CONTROL do wˆa˜ciciela
  ToolSendNotify(hwnd, TN_ENDTRACK, &trc);

  // Sprawdzenie czy okno byˆo widoczne przed wysˆaniem komunikatu
  if(oldstyle & WS_VISIBLE)
  { // Wymuszenie od˜wie¾enia okna rodzicielskiego
    InvalidateParent(hwnd, oldparent, &oldpos);
    // Aktualizacja zawarto˜ci paska narz©dzi
    WinEnableWindowUpdate(hwnd, TRUE);
    // Obszar uniewa¾niony zostanie obliczony podczas przetwarzania WM_CALCVALIDRECTS
    WinValidateRect(hwnd, NULL, TRUE);
  }

  // Korekta poˆo¾enia i rozmiar¢w okna na podstawie danych przekazanych w trc
  WinSetWindowPos(hwnd, HWND_TOP,
                  trc.rclSizePos.xLeft,
                  trc.rclSizePos.yBottom,
                  trc.rclSizePos.xRight - trc.rclSizePos.xLeft,
                  trc.rclSizePos.yTop   - trc.rclSizePos.yBottom,
                  SWP_MOVE | SWP_SIZE);

  // Zapami©tanie przemieszcenia okna wzgl©dem wˆa˜ciciela
  if(!(tcl->flToolState & (TST_FIXED | TST_MINIMIZED)))
    StoreOffset(tcl, &trc.rclSizePos);
  return(TRUE);
}





/*******************/
/* Funkcje lokalne */
/*******************/

// Odczyt wymiar¢w obramowania.
// Struktura TOOLADJ powinna mie† zainicjowane pola flState i flAttrs.
//
// Parametry:
//   tadj      - [parametr] wska«nik do struktury zawieraj¥cej pytanie o rozmiary paska
//   mode      - [parametr] rodzaj odczytywanej informacji
//
static LONG CrrQueryBorder(TOOLADJ *tadj, ULONG mode)
{ LONG thick;  // Grubo˜† obramowania

  // Obliczenie pocz¥tkowej grubo˜ci obramowania
  if(tadj->flState & TST_ROTATED)
  { if(mode & TDQ_HEIGHT) thick = 0;
    else thick = 2 * CXY_INTERNALMARGIN;
  }
  else
  { if(mode & TDQ_HEIGHT) thick = 2 * CXY_INTERNALMARGIN;
    else thick = 0;
  }

  // Pasek jest przesuwany - jest peˆne obramowanie
  if(tadj->flState & TST_FULLTRCFRAME)
  {
    // W stanie zminimalizowanym - cienka ramka
    if(tadj->flState & TST_MINIMIZED) return(2);
    // W pozostaˆych stanach - peˆna ramka
    return(thick + 4);
  }

  // Pasek w stanie floating - peˆna ramka
  if(!(tadj->flState & (TST_MINIMIZED | TST_FIXED))) return(thick + 4);
  // Pasek w stanie zminimalizowanym - usuni©cie marginesu
  if(tadj->flState & TST_MINIMIZED) thick = 0;

  // Odczyt wysoko˜ci
  if(mode & TDQ_HEIGHT)
  { // Dolna kraw©d«
    if(tadj->flAttrs & TBA_BOTTOMFRAME) thick++;
    // G¢rna kraw©d«
    if(tadj->flAttrs & TBA_TOPFRAME)    thick++;
  }
  else
  { // Lewa kraw©d«
    if(tadj->flAttrs & TBA_LEFTFRAME)  thick++;
    // G¢rna kraw©d«
    if(tadj->flAttrs & TBA_RIGHTFRAME) thick++;
  }
  return(thick);
}





// Obliczenie rozmiar¢w standardowych kontrolek paska narz©dzi, takich jak
// pasek tytuˆu, przycisk obracania okna.
//
// Parametry:
//   tadj      - [parametr] wska«nik do struktury zawieraj¥cej informacje o oknie
//   mode      - [parametr] rodzaj odczytywanej informacji: TDQ_HEIGHT, TDQ_WIDTH
//
static LONG  CrrQueryControl(TOOLADJ *tadj, ULONG mode)
{ HWND  rotate;     // Uchwyt przycisku obracania okna
  LONG  winstyle;   // Styl paska narz©dzi
  LONG  thick;      // Dˆugo˜† obszaru przycisk¢w kontrolnych

  // Odczyt stylu okna
  winstyle = WinQueryWindowULong(tadj->hwndToolBar, QWL_STYLE);
  // Odczyt uchwytu przycisku obracania okna
  if(winstyle & TBS_ROTATEBUTTON)
    rotate = WinWindowFromID(tadj->hwndToolBar, TCID_ROTATE);

  // Wst©pne okre˜lenie rozmiar¢w przycisk¢w kontrolnych
  thick = 0;

  if(mode & TDQ_HEIGHT)
  { // Czy jest pasek tytuˆu ?
    if(winstyle & (TBS_MOVEBUTTON | TBS_FIXBUTTON))
      thick += (CY_TITLEHEIGHT + CXY_OBJECTSPACE);

    // Czy jest przycisk obracania ?
    if(winstyle & TBS_ROTATEBUTTON)
      if(WinIsWindowEnabled(rotate))
        thick += (CY_ROTATEBUTTON + 2 * CXY_ROTATESPACE + CXY_OBJECTSPACE);
  }
  else
  { // Czy jest pasek tytuˆu ?
    if(winstyle & (TBS_MOVEBUTTON | TBS_FIXBUTTON))
      thick += (CX_TITLEWIDTH + CXY_OBJECTSPACE);

    // Czy jest przycisk obracania ?
    if(winstyle & TBS_ROTATEBUTTON)
      if(WinIsWindowEnabled(rotate))
        thick += (CX_ROTATEBUTTON + 2 * CXY_ROTATESPACE + CXY_OBJECTSPACE);
  }
  return(thick);
}





// Funkcja koryguje obszar wewn¥trz obramowania, otrzymany za pomoc¥ funkcji
// CrrQueryInterior tak, aby uwzgl©dniaˆ on tylko obszar przeznaczony
// na obiekty umieszczane wewn¥trz paska narz©dzi.
//
// Parametry:
//   tadj      - [parametr/rezultat] struktura zawieraj¥ca dane do skorygowania
//
static VOID CrrQueryClientArea(TOOLADJ *tadj)
{
  if(tadj->flState & TST_ROTATED)
  { // Korekta g¢rnego marginesu
    tadj->rclSizePos.yTop    -= CrrQueryControl(tadj, TDQ_HEIGHT);
    // Korekta dolego marginesu
    if(tadj->flState & TST_FIXED)
     tadj->rclSizePos.yBottom += CrrQueryEndMargin(tadj);
    tadj->rclSizePos.yBottom += CXY_INTERNALMARGIN;
    // Korekta lewego marginesu
    tadj->rclSizePos.xLeft   += CXY_INTERNALMARGIN;
    // Korekta prawego marginesu
    tadj->rclSizePos.xRight  -= CXY_INTERNALMARGIN;
  }
  else
  { // Korekta g¢rnego marginesu
    tadj->rclSizePos.yTop    -= CXY_INTERNALMARGIN;
    // Korekta dolego marginesu
    tadj->rclSizePos.yBottom += CXY_INTERNALMARGIN;
    // Korekta lewego marginesu
    tadj->rclSizePos.xLeft   += CrrQueryControl(tadj, TDQ_WIDTH);
    // Korekta prawego marginesu
    if(tadj->flState & TST_FIXED)
      tadj->rclSizePos.xRight  -= CrrQueryEndMargin(tadj);
    tadj->rclSizePos.xRight  -= CXY_INTERNALMARGIN;
  }
}





// Funkcja okre˜la szeroko˜† koäcowego marginesu
// Struktura TOOLADJ powinna mie† zainicjowane pola flState i flAttrs
//
// Parametry:
//   tadj      - [parametr] wska«nik do struktury zawieraj¥cej stan paska
//
static LONG  CrrQueryEndMargin(TOOLADJ *tadj)
{ LONG margin;

  // Sprawdzenie czy trwa przesuwanie paska
  if(tadj->flState & TST_FULLTRCFRAME) return(0);

  // Obliczanie koäcowego margunesu
  margin = CXY_ENDMARGIN;
  // Uwzgl©dnienie poˆo¾enia paska
  if(tadj->flState & TST_ROTATED)
  { if(tadj->flAttrs & TBA_TOPFRAME)    margin--;
    if(tadj->flAttrs & TBA_BOTTOMFRAME) margin--;
  }
  else
  { if(tadj->flAttrs & TBA_LEFTFRAME)   margin--;
    if(tadj->flAttrs & TBA_RIGHTFRAME)  margin--;
  }
  return(margin);
}





/******************************/
/* Dodatkowe funkcje usˆugowe */
/******************************/

// Przesuwanie tylko ramki paska narz©dzi
//
// Parametry:
//   tcl       - wska«nik do struktur kontrolnych paska narz©dzi
//   trc       - parametry przesuwania
//
static VOID FrameOnlyDrag(TOOLCTL *tcl, TOOLTRACK *trc)
{ TRACKINFO tnf;    // Struktura przekazywana funkcji WinTrackRect
  POINTL    pos;    // Koäcowa pozycja podczas przesuwania

  // Inicjalizacja struktury
  tnf.cxBorder    = 2;
  tnf.cyBorder    = 2;
  tnf.cxGrid      = 1;
  tnf.cyGrid      = 1;
  tnf.cxKeyboard  = 1;
  tnf.cyKeyboard  = 1;
  tnf.rclTrack    = trc->rclSizePos;
  tnf.rclBoundary = trc->rclBoundary;
  tnf.ptlMinTrackSize.x = tnf.rclTrack.xRight - tnf.rclTrack.xLeft;
  tnf.ptlMinTrackSize.y = tnf.rclTrack.yTop   - tnf.rclTrack.yBottom;
  tnf.ptlMaxTrackSize   = tnf.ptlMinTrackSize;
  tnf.fs = TF_MOVE;
  if(tcl->flToolState & (TST_FIXED | TST_MINIMIZED))
    tnf.fs |= TF_ALLINBOUNDARY;

  // Przesuwanie ramki
  WinTrackRect(trc->hwndParent, NULLHANDLE, &tnf);
  // Wysˆanie potwierdzenie TN_TRACKING
  pos.x = tnf.rclTrack.xLeft;
  pos.y = tnf.rclTrack.yBottom;
  ToolSendNotify(tcl->hwndToolBar, TN_TRACKING, &pos);

  // Zapami©tanie rezultat¢w
  trc->rclSizePos = tnf.rclTrack;
}





// Przesuwanie caˆego okna paska narz©dzi - "full window dragging"
//
// Parametry:
//   tcl       - wska«nik do struktur kontrolnych paska narz©dzi
//   trc       - parametry przesuwania
//
static VOID FullWindowDrag(TOOLCTL *tcl, TOOLTRACK *trc)
{ POINTL  cpos;          // Aktualna pozycja kursora i po korekcie - paska narz©dzi
  POINTL  delta;         // Poˆo¾enie przesuwanego paska wzgl©dem kursora myszy
  POINTL  offset;        // Korekta poˆo¾enia po zmianie obramowania w UpdateBeforeDrag
  HWND    hwnd;          // Uchwyt paska narz©dzi
  QMSG    smsg;          // Struktura przechowuj¥ca komunikat
  HAB     hab;           // Uchwyt anchor block PM


  hwnd = tcl->hwndToolBar;
  // Przej©cie kursora myszy
  WinSetCapture(HWND_DESKTOP, tcl->hwndTitleBar);
  // Odczyt uchwytu anchor block
  hab = WinQueryAnchorBlock(hwnd);
  // Inicjacja zmiennych
  offset.x = 0; offset.y = 0;

  // Obliczenie przesuni©cia kursora wzgl©dem paska narz©dzi
  WinQueryPointerPos(HWND_DESKTOP, &delta);
  WinMapWindowPoints(HWND_DESKTOP, trc->hwndParent, &delta, 1);
  delta.x -= trc->rclSizePos.xLeft;
  delta.y -= trc->rclSizePos.yBottom;

  // Pomocnicza, wewn©trzna p©tla przetwarzaj¥ca komunikaty
  // podczas przesuwania okna
  do
  { // Sprawdzenie czy w kolejce jest gdzie˜ WM_PAINT,
    // przetworzenie go w pierwszej kolejno˜ci.
    if(WinPeekMsg(hab, &smsg, NULLHANDLE, WM_PAINT, WM_PAINT, PM_REMOVE) == TRUE)
    { // Przerysowanie wybranego okna
      WinDispatchMsg(hab, &smsg);
    }
    // Sprawdzenie czy w kolejce jest WM_QUIT
    else if(WinPeekMsg(hab, &smsg, NULLHANDLE, WM_QUIT, WM_QUIT, PM_NOREMOVE) == TRUE)
    { // Przerwanie p©tli
      break;
    }

    // Pobieranie i rozsyˆanie pozostaˆych komunikat¢w
    else
    { // Pobranie komunikatu z kolejki
      WinGetMsg(hab, &smsg, NULLHANDLE, 0, 0);

      // Dodatkowe przetwarzanie informacji dotycz¥cych przycisku
      if(smsg.hwnd == tcl->hwndTitleBar)
      { // Sprawdzenie czy zwolniono przycisk, koäcz¥c przesuwanie ramki
        if(smsg.msg == WM_BUTTON1UP)
        { // Korekta wygl¥du ramki po zakoäczeniu przesuwania
          tcl->flToolState &= ~TST_FULLTRCFRAME;
          // Przerwanie wewn©trznej p©tli
          break;
        }

        // Sprawdzenie czy rozpocz©to przesuwanie ramki
        if(smsg.msg == WM_BUTTON1MOTIONSTART)
          // Korekta wygl¥du ramki przed rozpcz©ciem przesuwania
          UpdateBeforeDrag(tcl, trc, &offset);
      }

      // Sprawdzenie czy jest to komunikat informuj¥cy o ruchu kursora
      if(smsg.msg == WM_MOUSEMOVE)
      {
        cpos = smsg.ptl;
        // Obliczenie pozycji kursora wyra¾onej we wsp¢ˆrz©dnych okna rodzicielskiego
        WinMapWindowPoints(HWND_DESKTOP, WinQueryWindow(hwnd, QW_PARENT), &cpos, 1);

        // Sprawdzenie zakresu przesuwania okna
        if(trc->flState & (TST_FIXED | TST_MINIMIZED))
        { // Przesuwanie paska - bez wyje¾d¾ania kraw©dzi¥ poza obr©b obszaru ograniczaj¥cego
          // Dodanie przesuni©cia - delta
          cpos.x -= delta.x;
          cpos.y -= delta.y;
          // Sprawdzanie kolejnych kraw©dzi - prawa kraw©d«
          if((cpos.x + trc->rclSizePos.xRight - trc->rclSizePos.xLeft) > trc->rclBoundary.xRight)
            cpos.x = trc->rclBoundary.xRight - trc->rclSizePos.xRight + trc->rclSizePos.xLeft;
          // Dolna kraw©d«
          if(cpos.y < trc->rclBoundary.yBottom)
            cpos.y = trc->rclBoundary.yBottom;
          // Lewa kraw©d«
          if(cpos.x < trc->rclBoundary.xLeft)
            cpos.x = trc->rclBoundary.xLeft;
          // G¢tna kraw©d«
          if((cpos.y + trc->rclSizePos.yTop - trc->rclSizePos.yBottom) > trc->rclBoundary.yTop)
            cpos.y = trc->rclBoundary.yTop - trc->rclSizePos.yTop + trc->rclSizePos.yBottom;
        }
        else
        { // Sprawdzenie czy nowa pozycja zawiera si© w obr©bie okna i ewentualna korekta
          if(cpos.x < trc->rclBoundary.xLeft)   cpos.x = trc->rclBoundary.xLeft;
          if(cpos.y < trc->rclBoundary.yBottom) cpos.y = trc->rclBoundary.yBottom;
          if(cpos.x > trc->rclBoundary.xRight)  cpos.x = trc->rclBoundary.xRight;
          if(cpos.y > trc->rclBoundary.yTop)    cpos.y = trc->rclBoundary.yTop;
          // Dodanie przesuni©cia - delta
          cpos.x -= delta.x;
          cpos.y -= delta.y;
        }

        // Korekta wsp¢ˆrz©dnych zapisanych w strukturze TOOLTRACK
        WinOffsetRect(hab, &trc->rclSizePos,
                            cpos.x - trc->rclSizePos.xLeft,
                            cpos.y - trc->rclSizePos.yBottom);

        // Ustalenie nowej pozycji okna
        WinSetWindowPos(hwnd, HWND_TOP, cpos.x + offset.x, cpos.y + offset.y, 0, 0, SWP_MOVE | SWP_NOADJUST);
        // Wysˆanie do wˆa˜ciciela komunikatu potwierdzaj¥cego zmian© pozycji
        ToolSendNotify(hwnd, TN_TRACKING, &cpos);
        // Usuni©cie nieprzetworzonego WM_MOUSEMOVE z kolejki
        WinPeekMsg(hab, &smsg, NULLHANDLE, WM_MOUSEMOVE, WM_MOUSEMOVE, PM_REMOVE);
      }
      // Przetwarzanie pozostaˆych komunikat¢w
      else WinDispatchMsg(hab, &smsg);
    }
  } while(1);

  // Zwolnienie kursora myszy
  WinSetCapture(HWND_DESKTOP, NULLHANDLE);
}





// Funkcja wymusza przerysowanie fragmentu stargego okna rodzicielskiego,
// zakrytego uprzednio przez pasek narz©dzi, kt¢ry zmieniˆ rodzica.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska narz©dzi
//   oldparent - [parametr] uchwyt starego okna rodzicielskiego
//   oldpos    - [parametr] poprzednia pozycja paska narz©dzi, liczona wzgl©dem oldpos
//
static VOID InvalidateParent(HWND hwnd, HWND oldparent, RECTL *oldpos)
{ RECTL new;        // Nowa pozycja paska narz©dzi
  LONG  rc;         // Kody zwracane przez funkcje
  HRGN  update;     // Od˜wie¾any obszar
  HRGN  tmp;        // Tymczasowy obszar pomocniczy
  HPS   hps;        // Uchwyt presentation space okna rodzicielskiego

  hps = WinGetPS(oldparent);
  // Odczyt wsp¢ˆrz©dnych nowej pozycji paska narz©dzi
  WinQueryWindowRect(hwnd, &new);
  WinMapWindowPoints(hwnd, oldparent, (POINTL *)&new, 2);

  // Tworzenie obszaru od˜wie¾ania
  update = GpiCreateRegion(hps, 1, oldpos);
  if(update != RGN_ERROR)
  { // Region pomocniczy
    tmp = GpiCreateRegion(hps, 1, &new);
    if(tmp != RGN_ERROR)
    { // Obliczenie r¢¾nicy: oldpos - new
      rc = GpiCombineRegion(hps, update, update, tmp, CRGN_DIFF);
      if((rc != RGN_NULL) && (rc != RGN_ERROR))
        // Od˜wie¾enie obszaru
        WinInvalidateRegion(oldparent, update, TRUE);

      // Usuni©cie regionu pomocniczego
      if(tmp    != RGN_ERROR) GpiDestroyRegion(hps, tmp);
    }
    // Usuni©cie od˜wie¾anego regionu
    if(update != RGN_ERROR) GpiDestroyRegion(hps, update);
  }
  WinReleasePS(oldparent);
}





// Pozycjonowanie przycisku oracania okna
//
// Parametry:
//   tcl       - [parametr] wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
//   rclInter  - [parametr] wsp¢ˆrz©dne obszaru wewn¥trz obramowania
//
static VOID  PlaceRotateButton(TOOLCTL *tcl, RECTL *rclInter)
{ POINTL pos;       // Wsp¢ˆrz©dne og¢ˆnego przeznaczenia
  LONG   cx, cy;    // Wymiary przycisku

  // Sprawdzenie czy przycisk obracania okna jest
  if(!(tcl->flWinStyle & TBS_ROTATEBUTTON)) return;

  // Ukrycie przycisku obracania w stanie zminimalizowanym
  if(tcl->flToolState & TST_MINIMIZED)
  { // Ukrycie przycisku obracania
    if(tcl->flWinStyle & TBS_ROTATEBUTTON)
      WinShowWindow(tcl->hwndRotate, FALSE);
    return;
  }

  // Sprawdzenie czy przycisk obracania okna jest aktywny
  if(WinIsWindowEnabled(tcl->hwndRotate))
  { if(tcl->flToolState & TST_ROTATED)
    { pos.x = rclInter->xLeft;
      pos.y = rclInter->yTop  - CXY_ROTATESPACE - CY_ROTATEBUTTON;
      if(tcl->flWinStyle & (TBS_MOVEBUTTON | TBS_FIXBUTTON))
        pos.y -= (CY_TITLEHEIGHT + CXY_OBJECTSPACE);
      cx = rclInter->xRight - rclInter->xLeft;
      cy = CY_ROTATEBUTTON;
    }
    else
    { pos.x = rclInter->xLeft + CXY_ROTATESPACE;
      if(tcl->flWinStyle & (TBS_MOVEBUTTON | TBS_FIXBUTTON))
        pos.x += (CX_TITLEWIDTH + CXY_OBJECTSPACE);
      pos.y = rclInter->yBottom;
      cx = CX_ROTATEBUTTON;
      cy = rclInter->yTop - rclInter->yBottom;
    }
    // Pozycjonowanie przycisku obracania okna
    WinSetWindowPos(tcl->hwndRotate, HWND_TOP,
                    pos.x, pos.y, cx, cy,
                    SWP_MOVE | SWP_SIZE | SWP_ZORDER | SWP_SHOW);
  }
  else WinShowWindow(tcl->hwndRotate, FALSE);
}





// Pozycjonowanie paska tytuˆu.
//
// Parametry:
//   tcl       - [parametr] wska«nik do gˆ¢wnej struktury kontrolnej paska narz©dzi
//   rclInter  - [parametr] wsp¢ˆrz©dne obszaru wewn¥trz obramowania
//
static VOID PlaceTitleBar(TOOLCTL *tcl, RECTL *rclInter)
{ POINTL pos;       // Wsp¢ˆrz©dne og¢lnego przeznaczenia
  LONG   length;    // Obliczona dˆugo˜† paska tytuˆu


  // Sprawdzenie czy pasek narz©dzi jest
  if(!(tcl->flWinStyle & (TBS_MOVEBUTTON | TBS_FIXBUTTON))) return;
  // Pozioma wsp¢ˆrz©dna pocz¥tkowa paska tytuˆu
  pos.x = rclInter->xLeft;

  // Sprawdzenie czy pasek narz©dzi jest zminimalizowany
  if(tcl->flToolState & TST_MINIMIZED)
  { // Obliczenie dˆuo˜ci paska tytuˆu i ustalenie poˆo¾enia
    if(tcl->flToolState & TST_ROTATED)
    { length = rclInter->yTop - rclInter->yBottom;
      pos.y  = rclInter->yTop - length;
      tcl->flTitleState &= ~STT_ROTATED;
    }
    else
    { length = rclInter->xRight - rclInter->xLeft;
      pos.y  = rclInter->yBottom;
      tcl->flTitleState |= STT_ROTATED;
    }
    // Rysowanie ˜ci©cia
    tcl->flTitleState |= STT_DRAWCUT;
  }
  // Pozycjonowanie w stanie normalnym
  else
  { // Ustalenie nowego poˆo¾enia paska tytuˆu
    if(tcl->flToolState & TST_ROTATED)
    { length = rclInter->xRight - rclInter->xLeft;
      pos.y  = rclInter->yTop - CY_TITLEHEIGHT;
      tcl->flTitleState |= STT_ROTATED;
    }
    else
    { length = rclInter->yTop - rclInter->yBottom;
      pos.y  = rclInter->yBottom;
      tcl->flTitleState &= ~STT_ROTATED;
    }
    // Wyˆ¥czenie rysowania ˜ci©cia
    tcl->flTitleState &= ~STT_DRAWCUT;
  }

  // Okre˜lenie stanu przycisku minimalizacji i maksymalizacji
  if(tcl->flToolState & (TST_MINIMIZED | TST_FIXED))
    tcl->flTitleState &= ~STT_UPTRIANGL;
  else tcl->flTitleState |= STT_UPTRIANGL;

  // Pozycjonowanie paska tytuˆu
  WinSetWindowPos(tcl->hwndTitleBar, HWND_TOP,
                  pos.x, pos.y, length, length,
                  SWP_MOVE | SWP_SIZE | SWP_ZORDER);
}





// Funkcja zapami©tuje przemieszczenie paska narz©dzi wzgl©dem wˆa˜ciciela,
// w polu ptlOffset gˆ¢wnej struktury kontrolnej.
//
// Parametry:
//    tcl      - [parametr/rezultat] wska«nik do gˆ¢wnych struktur kontrolnych okna
//    winpos   - [parametr] pozycja okna liczona wzgl©dem okna rodzicielskiego
//
static VOID StoreOffset(TOOLCTL *tcl, RECTL *winpos)
{ RECTL tsize;      // Rozmiary paska narz©dzi
  RECTL osize;      // Rozmiary wˆa˜ciciela
  HWND  owner;      // Uchwyt wˆa˜ciciela okna
  HWND  parent;     // Uchwyt okna rodzicielskiego

  // Inicjacja zmiennych
  tsize = *winpos;
  // Odczyt uchwytu frame-window, b©d¥cego wˆa˜cicielem paska narz©dzi
  owner  = WinQueryWindow(tcl->hwndToolBar, QW_FRAMEOWNER);
  // Odczyt rozmiar¢w wˆa˜ciciela
  WinQueryWindowRect(owner, &osize);
  // Odczyt uchwytu okna rodzicielskiego
  parent = WinQueryWindow(tcl->hwndToolBar, QW_PARENT);

  // Mapowanie do wsp¢ˆrz©dnych wˆa˜cicela
  WinMapWindowPoints(parent, owner, (POINTL *)&tsize, 2);
  // Zapami©tanie przesuni©cia
  tcl->ptlOffset.x = tsize.xLeft;
  tcl->ptlOffset.y = osize.yTop - tsize.yBottom;
}





// Funkcja przygotowuje okno do przesuwania, zostaje przerysowane obramowanie
// oraz zmieniony stan okna: dadany TST_FULLTRCFRAME.
//
// Parametr:
//   tcl       - [parametr] wska«nik do struktut kontrolnych paska
//   trc       - [parametr] parametry przesuwu
//   offset    - [rezultat] przesuni©cie koryguj¥ce poˆo¾enie ramki po zmianie obramowania
//
static VOID UpdateBeforeDrag(TOOLCTL *tcl, TOOLTRACK *trc, POINTL *offset)
{ LONG cx, cy;           // Wymiary paska narz©dzi

  // Przygotowanie ramki do przesuwania - ustawienie odpowiedniego bitu stanu
  tcl->flToolState |= TST_FULLTRCFRAME;
  // Okre˜lnie rozmiar¢w paska narz©dzi skorygowanych o grubo˜† ramki
  cx = (LONG)WinSendMsg(tcl->hwndToolBar, TM_QUERYWIDTH,  MPFROMLONG(TDQ_CURRENT), 0L);
  cy = (LONG)WinSendMsg(tcl->hwndToolBar, TM_QUERYHEIGHT, MPFROMLONG(TDQ_CURRENT), 0L);

  // Obliczenie przesuni©cia koryguj¥cego wzrost wymiar¢w ramki
  // (podczas przesuwania jest rysowane peˆne obramowanie 3D)
  offset->x = -((cx - trc->rclSizePos.xRight + trc->rclSizePos.xLeft  ) / 2);
  offset->y = -((cy - trc->rclSizePos.yTop   + trc->rclSizePos.yBottom) / 2);

  // Korekta wymiar¢w i poˆo¾enia paska narz©dzi
  WinSetWindowPos(tcl->hwndToolBar, HWND_TOP,
                  trc->rclSizePos.xLeft   + offset->x,
                  trc->rclSizePos.yBottom + offset->y,
                  cx, cy, SWP_MOVE | SWP_SIZE);
}


/*
 * $Log: carrier.c $
 * Revision 1.1  1999/06/27 12:35:43  Wojciech_Gazda
 * Initial revision
 *
 */
