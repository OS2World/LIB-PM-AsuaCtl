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


/* Asuactl.dll
 * Pasek narz©dzi - dodatkowe okna kontrolne (Carrier Controls).
 * (c) 1999 Wojciech Gazda
 *
 * tcontrol.c
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/06/27 12:35:51 $
 * $Name:  $
 * $RCSfile: tcontrol.c $
 * $Revision: 1.1 $
 *
 */

//#define  INCL_DOSDEV
#define  INCL_DOSPROCESS

// Deklaracje OS/2
#define  INCL_GPILOGCOLORTABLE
#define  INCL_GPIPRIMITIVES
#define  INCL_GPIPOLYGON
#define  INCL_WINRECTANGLES
#define  INCL_WINWINDOWMGR
#define  INCL_WINTRACKRECT
#define  INCL_WINFRAMEMGR
#define  INCL_WINPOINTERS
#define  INCL_WINTIMER
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


// Identyfikatory timer¢w
#define TMR_TITLEHILITE       1         // Czasomierz steruj¥cy pod˜wietlaniem paska tytuˆu
#define TMR_TITLETIP          2         // Czasomierz steruj¥cy wy˜wietlaniem podpowiedzi
#define TMR_ROTATEHILITE      3         // Czasomierz steruj¥cy pod˜wietlaniem przycisku obracania




// Prototypy funkcji
MRESULT EXPENTRY MiniTitlebarProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY RotateButtonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);



// Prototypy funkcji lokalnych
static VOID  MtlControlPos(HWND hwnd, SWP *newpos);
static VOID  MtlHiliteControl(HWND hwnd, ULONG msg, ULONG timer);
static BOOL  MtlTestPoint(TOOLCTL *tcl, POINTS *cpos, ULONG mode);
static VOID  MtlMinMaxControl(HWND hwnd, ULONG msgcode, POINTS *cpos);
static VOID  MtlRedraw(HWND hwnd);
static VOID  MtlBeginTrack(HWND hwnd, POINTS *cpos);
static VOID  RtbHiliteControl(HWND hwnd, ULONG msg, ULONG timer);
static VOID  RtbRedraw(HWND hwnd);
static VOID  RtbRotateRequest(HWND hwnd);

// Dodatkowe funkcje usˆugowe
static VOID  DrawBitmap(HPS hps, HBITMAP hbitmap, POINTL *pos, ULONG flMode);
static VOID  DrawTitleButton(HPS hps, RECTL *wsize, ULONG TitleState);
static VOID  DrawTitleFace(HPS hps, RECTL *wsize, ULONG TitleState);
static VOID  DrawTitleFrame(HPS hps, RECTL *wsize, ULONG TitleState);





// Procedura okna steruj¥ca paskiem tytuˆu
//
MRESULT EXPENTRY MiniTitlebarProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
  { case WM_ADJUSTWINDOWPOS:  // Korekta skalowania
      MtlControlPos(hwnd, (SWP *)PVOIDFROMMP(mp1));
      return(0);

    case WM_BUTTON1DBLCLK:    // Obsˆuga minimalizacji/maksymalizacji
    case WM_BUTTON1CLICK:     // w trybie normalnym i full window drag.
      MtlMinMaxControl(hwnd, msg, (POINTS *)&mp1);
      break;

    case WM_BUTTON1DOWN:      // Naci˜ni©cie lewego przycisku myszy
      // Przesˆanie komunikatu do standardowej procedury
      // w celu poprawnej aktywacji gˆ¢wnego okna aplikacji.
      WinDefWindowProc(hwnd, msg, mp1, mp2);
      // Sprawdzenie czy mo¾na rozpocz¥† przesuwanie okna
      MtlBeginTrack(hwnd, (POINTS *)&mp1);
      break;

    case WM_MOUSEMOVE:        // Reakcja na ruch myszy
      // Kontrola pod˜wietlania, wy˜wietlanie tytuˆu w okienku popowiedzi
      MtlHiliteControl(hwnd, msg, 0L);
      break;

    case WM_PAINT:            // Przerysowanie paska tytuˆu
      MtlRedraw(hwnd);
      break;

    case WM_TIMER:            // Sterowanie pod˜wietleniem paska i wy˜wietlaniem tytuˆu
      MtlHiliteControl(hwnd, msg, SHORT1FROMMP(mp1));
      break;
  }

  // Powr¢t do standardowej procedury okna
  return(WinDefWindowProc(hwnd, msg, mp1, mp2));
}





// Procedura obsˆuguj¥ca przycisk obracania okna
//
MRESULT EXPENTRY RotateButtonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
  { case WM_BUTTON1CLICK:     // Wygenerowanie ¾¥dania obr¢cenia okna
      RtbRotateRequest(hwnd);
      break;

    case WM_BUTTON1DOWN:      // Kontrola podswietlania przycisku
    case WM_BUTTON1UP:
    case WM_MOUSEMOVE:
      RtbHiliteControl(hwnd, msg, 0L);
      break;

    case WM_CALCVALIDRECTS:   // Okre˜lenie obszaru nie podlegaj¥cego uniewa¾nieniu
      break;

    case WM_PAINT:            // Przerysowanie przycisku
      RtbRedraw(hwnd);
      return(0);

    case WM_TIMER:            // Sterowanie pod˜wietleniem przycisku
      RtbHiliteControl(hwnd, msg, SHORT1FROMMP(mp1));
      break;
  }

  // Powr¢t do standardowej procedury okna
  return(WinDefWindowProc(hwnd, msg, mp1, mp2));
}





//*******************/
//* Funkcje lokalne */
//*******************/

// Kontrola rozmiar¢w okna podczas skalowania
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska tytuˆu
//   newpos    - [parametr/rezultat] pozycja i znaczniki przekazywane przez WinSetWindowPos
//
static VOID  MtlControlPos(HWND hwnd, SWP *newpos)
{ TOOLCTL *tcl;          // Adres gˆ¢wnej struktury kontrolnej okna

  tcl = ToolLoadData(hwnd, "MtlControlPos");
  if(tcl == NULL) return;

  // Okre˜lenie wymiar¢w paska tytuˆu
  if(tcl->flTitleState & STT_ROTATED)
  { // Pasek jest odwr¢cony - rysowany poziomo
    newpos->cy = CY_TITLEHEIGHT;
  }
  else
  { // Pasek jest w pozycji normalnej - rysowany pionowo
    newpos->cx = CX_TITLEWIDTH;
  }
  // Wymuszenie zmiany rozmiaru
  newpos->fl |= SWP_SIZE;
}





// Funkcja steruje pod˜wietlaniem paska tytuˆu, oraz wy˜wietlaniem
// podpowiedzi zawieraj¥cej tytuˆ paska narz©dzi
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska tytuˆu
//   msg       - [parametr] kod komunikatu - WM_MOUSEMOVE lub WM_TIMER
//   timer     - [parametr] numer timera
//
static VOID MtlHiliteControl(HWND hwnd, ULONG msg, ULONG timer)
{ TOOLCTL *tcl;          // Adres gˆ¢wnej struktury kontrolnej paska narz©dzi
  HAB      hab;          // Uchwyt anchor block
  RECTL    wsize;        // Rozmiary okna
  POINTL   pos;          // Pozycja kursora myszy
  LONG     tipdelay;     // Czas op¢«nienia przed wˆ¥czeniem podpowiedzi

  tcl = ToolLoadData(hwnd, "MtlHiliteControl");
  if(tcl == NULL) return;
  hab = WinQueryAnchorBlock(hwnd);

  // Zaˆ¥czenie pod˜wietlenia okna
  if(msg == WM_MOUSEMOVE)
  { // Sprawdzenie czy okno nie jest ju¾ pod˜wietlone
    if(!(tcl->flTitleState & STT_HILITED))
    { // Pod˜wietlenie okna
      tcl->flTitleState |= STT_HILITED;
      // Od˜wie¾enie okna
      WinInvalidateRect(hwnd, NULL, TRUE);
      // Wyzwolenie czasomierza steruj¥cego pod˜wietlaniem
      WinStartTimer(hab, hwnd, TMR_TITLEHILITE, TIME_TIPREMOVE);

      // Odczyt czasu op¢«nienia przed wˆ¥czeniem podpowiedzi
      if(!WinQueryPresParam(hwnd, PP_TOOLTIPDELAY, 0L, NULL, 4, &tipdelay, 0L))
        tipdelay = TIME_TIPDELAY;
      // Wyzwolenie czasomierza steruj¥cego wy˜wietlaniem podpowiedzi
      WinStartTimer(hab, hwnd, TMR_TITLETIP, (ULONG)tipdelay);
    }
  }
  if(msg == WM_TIMER)
  { // Sprawdzenie czy wyˆ¥czy† pod˜wietlenie i podpowied« do paska tytuˆu
    if(timer == TMR_TITLEHILITE)
    { // Sprawdzenie czy kursor znajduje si© w obr©bie okna
      WinQueryPointerPos(HWND_DESKTOP, &pos);
      // Konwersja do wsp¢ˆrz©dnych okna
      WinMapWindowPoints(HWND_DESKTOP, hwnd, &pos, 1);
      // Odczyt wymiar¢w okna
      WinQueryWindowRect(hwnd, &wsize);
      // Sprawdzenie czy kursor jest wewn¥trz okna
      if(WinPtInRect(hab, &wsize, &pos) == FALSE)
      { // Zatrzymanie timer¢w
        WinStopTimer(hab, hwnd, TMR_TITLEHILITE);
        WinStopTimer(hab, hwnd, TMR_TITLETIP);

        // Wyˆ¥czenie podpowiedzi (je˜li jest)
        if(tcl->flTitleState & STT_TOOLTIP)
        { tcl->flTitleState &= ~STT_TOOLTIP;
          // Zgaszenie okna z podpowiedzi¥
          if(tcl->hwndToolTip != NULLHANDLE)
            WinSetWindowPos(tcl->hwndToolTip, HWND_TOP, 0, 0, 0, 0, SWP_HIDE);
        }

        // Wyˆ¥czenie pod˜wietlenia paska
        tcl->flTitleState &= ~STT_HILITED;
        // Od˜wie¾enie okna
        WinInvalidateRect(hwnd, NULL, TRUE);
      }
    }

    // Wy˜wietlenie tytuˆu (podpowiedzi)
    if(timer == TMR_TITLETIP)
    { // Zatrzymanie czasomierza
      WinStopTimer(hab, hwnd, TMR_TITLETIP);
      // Wy˜wietlenie okna z podpowiedzi¥
      ToolDisplayTip(hwnd, hwnd, tcl->szWinText);
      // Uaktualnienie znacznika
      tcl->flTitleState |= STT_TOOLTIP;
    }
  }
}





// Funkcja sprawdza czy podana pozycja kursora mie˜ci si© w obr©bie
// zadanego obszaru w obr©bie paska tytuˆu.
//
// Parametry:
//   tcl       - [parametr] wska«nik do gˆ¢wnych struktur kontrolnych paska narz©dzi
//   cpos      - [parametr] wsp¢ˆrz©dne kursora liczone wzgl©dem paska tytuˆu
//   mode      - [parametr] identyfikator testowanego obszaru
//
// Powr¢t:
//   FALSE - kursor nie jest nad podanym obszarem
//   TRUE  - kursor jest nad podanym obszarem
//
static BOOL MtlTestPoint(TOOLCTL *tcl, POINTS *cpos, ULONG mode)
{ POINTL   pos;          // Pozycja kursora
  RECTL    area;         // Wsp¢ˆrz©dne tesrowanego obszaru
  HAB      hab;          // Uchwyt anchor block PM

  // Odczyt uchwytu anchor block
  hab = WinQueryAnchorBlock(tcl->hwndTitleBar);
  // Odczyt wymiar¢w okna
  WinQueryWindowRect(tcl->hwndTitleBar, &area);

  // Sprawdzenie czy jest przycisk "przyklejania/odklejania" okna
  if(tcl->flTitleState & STT_DRAWBUTTON)
  { // Korekta wymiar¢w obszaru movebar
    if(tcl->flTitleState & STT_ROTATED)
    { // Pasek tytuˆu jest odwr¢cony - le¾y poziomo
      area.xLeft += (CXY_TITLEBUTTON + 1);
    }
    else
    { // Pasek tytuˆu jest w normalnej pozycji - pionowo
      area.yTop  -= (CXY_TITLEBUTTON + 2);
    }
  }

  // Przepisanie pozycji kursora
  pos.x = cpos->x; pos.y = cpos->y;
  // Sprawdzenie czy kursor jest ponad paskiem przesuwu okna
  if(mode == MTLQ_MOVEBAR)
    return(WinPtInRect(hab, &area, &pos));

  // Sprawdzenie czy kursor jest ponad przyciskiem min/max
  if((mode == MTLQ_FIXBUTTON) && (tcl->flTitleState & STT_DRAWBUTTON))
  { if(WinPtInRect(hab, &area, &pos) == FALSE)
      return(TRUE);
    else return(FALSE);
  }
  return(FALSE);
}





// Funkcja steruje minimalizacj¥ i maksymalizacj¥ paska narz©dzi za pomoc¥ myszy,
// oraz przyklejaniem/odklejaniem paska od okna rodzicielskiego.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska tytuˆu
//   msgcode   - [parametr] Kod komunikatu odpowiedzialnego za wywoˆanie funkcji
//   cpos      - [parametr] pozycja kursora w momencie naciskania przycisku
//
static VOID  MtlMinMaxControl(HWND hwnd, ULONG msgcode, POINTS *cpos)
{ TOOLCTL *tcl;          // Adres struktur kontrolnych paska naz©dzi
  LONG     flswp;        // Zmienna steruj¥ca procesem zmiany stanu okna

  tcl = ToolLoadData(hwnd, "MtlMinMaxControl");
  if(tcl == NULL) return;

  //// Sprawdzenie czy przesuwa† tylko ramk©
  if(ulWarpVer >= 40)
  { // Sprawdznie czy jest uaktywniona opcja "Full Window Drag"
    if(WinQuerySysValue(HWND_DESKTOP, SV_TRACKRECTLEVEL) != 0)
      // Ignorowanie podw¢jnego klikni©cia
      if(msgcode == WM_BUTTON1DBLCLK) return;
  }
  else
    // Przesuwanie samej ramki
    if(msgcode == WM_BUTTON1DOWN) return;

  // Sprawdzenie czy kursor jest ponad uchwytem do przesuwania okna
  // oraz jest odblokowana minimalizacja/maksymalizacja okna
  if(MtlTestPoint(tcl, cpos, MTLQ_MOVEBAR) == TRUE)
  { if(tcl->flWinStyle & TBS_ENABLEMINIMIZE)
    {
      // Odczyt aktualnego stanu okna
      if(tcl->flToolState & TST_MINIMIZED)
        flswp = SWP_MAXIMIZE;
      else flswp = SWP_MINIMIZE;

      // Zmiana stanu okna
      WinSetWindowPos(tcl->hwndToolBar, HWND_TOP, 0, 0, 0, 0, flswp);
    }
  }

  // Sprawdzenie czy kursor jest nad przyciskiem dokowania
  else if(MtlTestPoint(tcl, cpos, MTLQ_FIXBUTTON) == TRUE)
  {
    // Okre˜lenie aktualnego stanu okna
    if(tcl->flToolState & (TST_MINIMIZED | TST_FIXED))
      flswp = SWP_RESTORE;
    else
    { // Odczyt poprzedniego stanu okna
      if(tcl->flOldState & TST_MINIMIZED)
        flswp = SWP_MINIMIZE;
      else flswp = SWP_MAXIMIZE;
    }

    // Zmiana stanu okna
    WinSetWindowPos(tcl->hwndToolBar, HWND_TOP, 0, 0, 0, 0, flswp);
  }
}





// Przerysowanie paska tytuˆu
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska tytuˆu
//
static VOID MtlRedraw(HWND hwnd)
{ TOOLCTL *tcl;                    // Adres gˆ¢wnej struktury kontrolnej paska narz©dzi
  RECTL    wsize;                  // Rozmiar okna
  HPS      hps;                    // Uchwyt presentation space

  tcl = ToolLoadData(hwnd, "MtlRedraw");
  if(tcl == NULL) return;

  // Odczyt rozmiar¢w okna
  WinQueryWindowRect(hwnd, &wsize);
  // Rozpocz©cie rysowania
  hps = WinBeginPaint(hwnd, NULL, NULLHANDLE);
  // adowanie tablicy kolor¢w
  PpmQueryPresColors(tcl->hwndToolBar, TOOL_MAXCOLOR, PPmColor, tcl->colors);
  // Tworzenie logicznej tablicy kolor¢w
  GpiCreateLogColorTable(hps, LCOL_PURECOLOR, LCOLF_CONSECRGB, 0L, TOOL_MAXCOLOR, tcl->colors);

  // Rysowanie obramowania
  DrawTitleFrame(hps, &wsize, tcl->flTitleState);
  // Rysowanie wn©trza przycisku
  DrawTitleFace(hps, &wsize, tcl->flTitleState);
  // Rysowanie przycisku (tr¢jkcika) wewn¥trz paska
  DrawTitleButton(hps, &wsize, tcl->flTitleState);

  // Zakoäczenie rysowania
  WinEndPaint(hps);
}





// Sprawdzenie czy mo¾na wygenerowa† komunikat WM_TRACKRECT
// i je˜li mo¾na, to wysˆanie go do wˆa˜ciciela.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska tytuˆu
//   cpos      - [parametr] pozycja kursora myszy
//
static VOID MtlBeginTrack(HWND hwnd, POINTS *cpos)
{ TOOLCTL *tcl;          // Struktura kontrolna paska narz©dzi
  HWND     owner;        // Wˆa˜ciciel paska tytuˆu
  ULONG    tflags;       // Zmienne steruj¥ce przesuwem


  // Odczyt adresu struktury kontrolnej
  tcl = ToolLoadData(hwnd, "MtlBeginTrack");
  if(tcl == NULL) return;
  // Sprawdzenie czy kursor myszy znajduje si© nad paskiem przesuwania
  if(MtlTestPoint(tcl, cpos, MTLQ_MOVEBAR) == FALSE) return;

  // Odczyt uchwytu wˆa˜ciciela paska tytuˆu
  owner = WinQueryWindow(hwnd, QW_OWNER);
  if(owner != NULLHANDLE)
  { // Zatrzymanie timera wy˜wietlaj¥cego podpowied«
    WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, TMR_TITLETIP);
    // Zaˆ¥czenie pod˜wietlenia
    tcl->flTitleState |= STT_HILITED;
    WinInvalidateRect(hwnd, NULL, FALSE);

    // Okre˜lenie flag steruj¥cych przesuwem
    tflags = TF_MOVE;
    // Sprawdzenie czy okno jest "przymocowane" do okna rodzicielskiego
    if(tcl->flToolState & TST_FIXED) tflags |= TF_ALLINBOUNDARY;
    // Wysˆanie komunikatu WM_TRACKFRAME - rozpocz©cie przesuwu okna
    WinSendMsg(owner, WM_TRACKFRAME, MPFROMLONG(tflags), 0L);

    // Wyˆ¥czenie pod˜wietlenia
    tcl->flTitleState &= ~STT_HILITED;
  }
}





// Funkcja steruje pod˜wietlaniem przycisku obracania okna
//
// Parametry:
//   hwnd      - [parametr] uchwyt przycisku obracania
//   msg       - [parametr] kod komunikatu - WM_MOUSEMOVE lub WM_TIMER
//   timer     - [parametr] numer timera
//
static VOID RtbHiliteControl(HWND hwnd, ULONG msg, ULONG timer)
{ TOOLCTL *tcl;          // Wska«nik do gˆ¢wnej struktury kontrolnej okna
  POINTL   pos;          // Pozycja kursora wewn¥trz okna
  RECTL    wsize;        // Wymiary okna
  HAB      hab;          // Uchwyt anchor block PM

  tcl = ToolLoadData(hwnd, "RtbHiliteControl");
  if(tcl == NULL) return;
  hab = WinQueryAnchorBlock(hwnd);

  if(msg == WM_MOUSEMOVE)
  { // Sprawdzenie czy okno nie jest ju¾ pod˜wietlone,
    // i czy przycisk myszy nie jest w dolnym poˆo¾eniu
    if(!(tcl->flRotateState & (SRB_HILITED | SRB_BUTTONDOWN)))
    { // Pod˜wietlenie okna
      tcl->flRotateState |= SRB_HILITED;
      // Od˜wie¾enie okna
      WinInvalidateRect(hwnd, NULL, TRUE);
      // Wyzwolenie czasomierza steruj¥cego pod˜wietlaniem
      WinStartTimer(hab, hwnd, TMR_ROTATEHILITE, TIME_TIPREMOVE);
    }
  }
  if(msg == WM_TIMER)
  { // Sprawdzenie czy wyˆ¥czy† pod˜wietlenie przycisku
    if(timer == TMR_ROTATEHILITE)
    { // Sprawdzenie czy kursor znajduje si© w obr©bie okna
      WinQueryPointerPos(HWND_DESKTOP, &pos);
      // Konwersja do wsp¢ˆrz©dnych okna
      WinMapWindowPoints(HWND_DESKTOP, hwnd, &pos, 1);
      // Odczyt wymiar¢w okna
      WinQueryWindowRect(hwnd, &wsize);
      // Sprawdzenie czy kursor jest wewn¥trz okna
      if(WinPtInRect(hab, &wsize, &pos) == FALSE)
      { // Zatrzymanie timera
        WinStopTimer(hab, hwnd, TMR_ROTATEHILITE);
        // Wyˆ¥czenie pod˜wietlenia paska
        tcl->flRotateState &= ~SRB_HILITED;
        // Od˜wie¾enie okna
        WinInvalidateRect(hwnd, NULL, TRUE);
      }
    }
  }
  if(msg == WM_BUTTON1DOWN)
  { // Przej©cie kursora myszy
    WinSetCapture(HWND_DESKTOP, hwnd);
    // Sprawdzenie czy wyˆ¥czy† pod˜wietlanie przycisku
    if(tcl->flRotateState & SRB_HILITED)
    { // Wyˆ¥czenie po˜wietlania
      tcl->flRotateState &= ~SRB_HILITED;
      // Zaznaczenie naci˜ni©cia przycisku
      tcl->flRotateState |=  SRB_BUTTONDOWN;
      // Przerysowanie okna
      WinInvalidateRect(hwnd, NULL, TRUE);
    }
  }
  if(msg == WM_BUTTON1UP)
  { // Zwolnienie kursora myszy
    WinSetCapture(HWND_DESKTOP, NULLHANDLE);
    // Usuni©cie znacznika naci˜ni©cia przycisku
    tcl->flRotateState &= ~SRB_BUTTONDOWN;
  }
}





// Przerysowanie przycisku obracania okna
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna przycisku
//
static VOID RtbRedraw(HWND hwnd)
{ TOOLCTL *tcl;          // Wska«nik do struktur kontrolnych paska narz©dzi
  POINTL   pos;          // Wsp¢ˆrz©dne og¢lnego przeznaczenia
  RECTL    wsize;        // Wymiary okna
  HBITMAP  hbitmap;      // Uchwyt mapy bitowej
  HPS      hps;          // Uchwyt presentation space okna


  tcl = ToolLoadData(hwnd, "RtbRedraw");
  if(tcl == NULL) return;

  hps = WinBeginPaint(hwnd, NULLHANDLE, NULL);
  // adowanie tablicy kolor¢w
  PpmQueryPresColors(tcl->hwndToolBar, TOOL_MAXCOLOR, PPmColor, tcl->colors);
  // Tworzenie logicznej tablicy kolor¢w
  GpiCreateLogColorTable(hps, LCOL_PURECOLOR, LCOLF_CONSECRGB, 0L, TOOL_MAXCOLOR, tcl->colors);
  // Okre˜lenie wymiar¢w okna
  WinQueryWindowRect(hwnd, &wsize);

  // Kasowanie tˆa
  GpiSetColor(hps, TOOL_BACKGROUND);
  // Kasowanie wn©trza przycisku
  pos.x = wsize.xLeft; pos.y = wsize.yBottom;
  GpiMove(hps, &pos);
  pos.x = wsize.xRight - 1; pos.y = wsize.yTop - 1;
  GpiBox(hps, DRO_FILL, &pos, 0, 0);

  // Obliczenie poˆo¾enia kopiowanych map bitowych
  pos.x = (wsize.xLeft   + wsize.xRight - CX_ROTATEBITMAP) / 2;
  pos.y = (wsize.yBottom + wsize.yTop   - CY_ROTATEBITMAP) / 2;

  // Rysowanie maski bitowej (wyci©cie tˆa)
  hbitmap  = GpiLoadBitmap(hps, hResource, ROTATE_BACK,    0, 0);
  if(hbitmap != NULLHANDLE)
  { // Ustawienie atrybut¢w - koloru tˆa i punktu
    GpiSetColor(hps, CLR_WHITE);
    GpiSetBackColor(hps, CLR_BLACK);
    // Kopiowanie mapy bitowej
    DrawBitmap(hps, hbitmap, &pos, ROP_SRCAND);
    // Usuni©cie mapy bitowej
    GpiDeleteBitmap(hbitmap);
  }

  // Wypeˆnienie strzaˆki
  hbitmap  = GpiLoadBitmap(hps, hResource, ROTATE_FILL,    0, 0);
  if(hbitmap != NULLHANDLE)
  { // Ustawienie atrybut¢w - koloru tˆa i punktu
    GpiSetColor(hps, tcl->flRotateState & SRB_HILITED ? TOOL_HILITEBACKGROUND : TOOL_FRAMEDARK);
    GpiSetBackColor(hps, CLR_BLACK);
    // Kopiowanie mapy bitowej
    DrawBitmap(hps, hbitmap, &pos, ROP_SRCPAINT);
    // Usuni©cie mapy bitowej
    GpiDeleteBitmap(hbitmap);
  }

  // Naˆo¾enie konturu
  hbitmap  = GpiLoadBitmap(hps, hResource, ROTATE_OUTLINE, 0, 0);
  if(hbitmap != NULLHANDLE)
  { // Kopiowanie mapy bitowej
    DrawBitmap(hps, hbitmap, &pos, ROP_SRCPAINT);
    // Usuni©cie mapy bitowej
    GpiDeleteBitmap(hbitmap);
  }

  WinEndPaint(hps);
}





// Wygenerowanie ¾¥dania obr¢cenia paska narz©dzi
//
// Parametry:
//   hwnd      - [parametr] uchwyt przycisku obracania okna
//
static VOID  RtbRotateRequest(HWND hwnd)
{ TOOLCTL *tcl;     // Wska«nik do gˆ¢wnych struktur kontrolnych okna

  tcl = ToolLoadData(hwnd, "RtbRotateRequest");
  if(tcl == NULL) return;

  // Wysˆanie komunikatu TM_ROTATE do paska narz©dzi
  WinSendMsg(tcl->hwndToolBar, TM_ROTATE, 0L, 0L);
}





/******************************/
/* Dodatkowe funkcje usˆugowe */
/******************************/

// Rysowanie mapy bitowej. Rozmiary s¥ odczytywane automatycznie.
//
// Parametry:
//   hps       - [parametr] presentation space
//   hbitmap   - [parametr] uchwyt rysowanej mapy bitowej
//   pos       - [parametr] poˆo¾enie lewego dolnego rogu
//   flMode    - [parametr] tryb nakˆadania mapy bitowej
//
static VOID DrawBitmap(HPS hps, HBITMAP hbitmap, POINTL *pos, ULONG flMode)
{ BITMAPINFOHEADER2 *bfh;     // Wska«nik do pocz¥tku struktury informacyjnej
  POINTL bmpos[4];            // Pozycja mapy bitowej (wsp¢ˆrz©dzne dla GpiWCBitBlt)
  UCHAR  tmp[16];             // Bufor na pocz¥tkow¥ cz©˜† struktury BITMAPINFOHEADER2

  // Inicjacja zmiennych
  bfh = (BITMAPINFOHEADER2 *)tmp;
  // Odczyt wymiar¢w mapy bitowej
  bfh->cbFix = 16;
  if(GpiQueryBitmapInfoHeader(hbitmap, bfh) == FALSE) return;

  // Obliczenie wsp¢ˆrz©dnych kopiowanych map bitowych
  // Wsp¢ˆrz©dne lewego dolnego rogu obszaru docelowego
  bmpos[0].x = pos->x;
  bmpos[0].y = pos->y;
  // Wsp¢ˆrz©dne prawego g¢rnego rogu obszaru docelowego
  bmpos[1].x = bmpos[0].x + bfh->cx - 1;
  bmpos[1].y = bmpos[0].y + bfh->cy - 1;
  // Wsp¢ˆrz©dne lewego dolnego rogu obszaru «r¢dˆowego
  bmpos[2].x = 0;
  bmpos[2].y = 0;
  // Wsp¢ˆrz©dne prawego g¢rnego rogu obszaru «r¢dˆowego
  bmpos[3].x = bfh->cx;
  bmpos[3].y = bfh->cy;

  // Kopiowanie mapy bitowej
  GpiWCBitBlt(hps, hbitmap, 4L, bmpos, flMode, 0L);
}





// Rysowanie przycisku (tr¢jk¥cika) na pasku tytuˆu
//
// Parametry:
//   hps       - [parametr] uchwyt presentation space
//   wsize     - [parametr] rozmiary okna
//   tcl       - [parametr] wska«nik do struktur kontrolnych paska narz©dzi
//
static VOID DrawTitleButton(HPS hps, RECTL *wsize, ULONG TitleState)
{ POINTL  pos;           // Wsp¢ˆrz©dne og¢lnego przeznaczenia
  HBITMAP hbmMono;       // Uchwyt monochromatycznej maski bitowej
  HBITMAP hbmColor;      // Uchwyt kolorowej mapy bitowej wy˜wietlanej na przycisku

  // Sprawdzenie czy opcje pozawalaj¥ na rysowania
  if(!(TitleState & STT_DRAWBUTTON)) return;

  // Kasowanie tˆa pod przycisk
  // Okre˜lenie koloru tˆa
  GpiSetColor(hps, TitleState & STT_HILITED ? TOOL_HILITEBACKGROUND : TOOL_BACKGROUND);

  // Wsp¢ˆrz©dne lewego dolnego rogu obszaru docelowego
  pos.x = wsize->xLeft + 1;
  pos.y = TitleState & STT_ROTATED ? wsize->yBottom  + 1 : wsize->yTop   - 1 - CXY_TITLEBUTTON;
  GpiMove(hps, &pos);
  // Wsp¢ˆrz©dne prawego g¢rnego rogu obszaru docelowego
  pos.x = TitleState & STT_ROTATED ? CXY_TITLEBUTTON     : wsize->xRight - 2;
  pos.y = wsize->yTop - 2;
  GpiBox(hps, DRO_FILL, &pos, 0L, 0L);

  // Tworzenie map bitowych
  hbmMono  = GpiLoadBitmap(hps, hResource,
                           TitleState & STT_UPTRIANGL ? MONO_UPTRIANGL  : MONO_DOWNTRIANGL,
                           0, 0);
  hbmColor = GpiLoadBitmap(hps, hResource,
                           TitleState & STT_UPTRIANGL ? COLOR_UPTRIANGL : COLOR_DOWNTRIANGL,
                           0, 0);

  // Wsp¢ˆrz©dne lewego dolnego rogu obszaru docelowego
  pos.x = wsize->xLeft + 2;
  pos.y = wsize->yTop - 7;

  // Rysowanie tˆa pod kolorow¥ map© bitow¥
  if(hbmMono != NULLHANDLE)
  { // Ustawienie atrybut¢w - koloru tˆa i punktu
    GpiSetColor(hps, CLR_WHITE);
    GpiSetBackColor(hps, CLR_BLACK);
    // Kopiowanie mapy bitowej
    DrawBitmap(hps, hbmMono, &pos, ROP_SRCAND);
    // Usuni©cie mapy bitowej
    GpiDeleteBitmap(hbmMono);
  }
  // Rysowanie przycisku - kolorowej mapy bitowej
  if(hbmColor != NULLHANDLE)
  { // Kopiowanie mapy bitowej
    DrawBitmap(hps, hbmColor, &pos, ROP_SRCPAINT);
    // Usuni©cie mapy bitowej
    GpiDeleteBitmap(hbmColor);
  }
}





// Rysowanie wn©trza paska tytuˆu
//
// Parametry:
//   hps       - [parametr] uchwyt presentation space
//   wsize     - [parametr] rozmiary okna
//   tcl       - [parametr] wska«nik do struktur kontrolnych paska narz©dzi
//
static VOID DrawTitleFace(HPS hps, RECTL *wsize, ULONG TitleState)
{ POLYGON poly;          // Definicja wielok¥ta
  POINTL  ptl[4];        // Punkty wielok¥ta
  POINTL  pos;           // Wsp¢ˆrz©dne og¢lnego przenaczenia
  LONG    start, stop;   // Pozycja pocz¥tkowa i koäcowa


  // Okre˜lenie koloru tˆa
  GpiSetColor(hps, TitleState & STT_HILITED ? TOOL_HILITEBACKGROUND : TOOL_BACKGROUND);
  // Rysowanie tˆa
  // Ustawienie punktu pocz¥tkowego
  pos.x = wsize->xRight - 2;
  pos.y = wsize->yTop   - 2;
  if(TitleState & STT_DRAWBUTTON)
    pos.y -= (TitleState & STT_ROTATED ? 0 : CXY_TITLEBUTTON);
  GpiMove(hps, &pos);

  // Okre˜lenie liczby punkt¢w
  poly.ulPoints = 3;

  // Lewy g¢rny r¢g
  ptl[0].x = wsize->xLeft + 1;
  ptl[0].y = wsize->yTop  - 2;
  if(TitleState & STT_DRAWBUTTON)
  { ptl[0].x += (TitleState & STT_ROTATED ? CXY_TITLEBUTTON : 0);
    ptl[0].y -= (TitleState & STT_ROTATED ? 0 : CXY_TITLEBUTTON);
  }

  // Lewy dolny r¢g
  ptl[1].x = wsize->xLeft   + 1;
  ptl[1].y = wsize->yBottom + 1;
  if(TitleState & STT_DRAWBUTTON)
    ptl[1].x += (TitleState & STT_ROTATED ? CXY_TITLEBUTTON : 0);

  // Prawy dolny r¢g
  ptl[2].x = wsize->xRight  - 2;
  ptl[2].y = wsize->yBottom + 1;

  // Obliczenie pozycji punkt¢w podczas rysowania wyci©cia
  if(TitleState & STT_DRAWCUT)
  { // O jeden punkt wi©cej
    poly.ulPoints ++;
    // Korekta pierwszego punktu wyci©cia
    ptl[2].x = wsize->xRight - CXY_TITLECUT - 1;
    // Pozycja drugiego punktu wyci©cia
    ptl[3].x = wsize->xRight - 2;
    ptl[3].y = wsize->yBottom + CXY_TITLECUT;
  }

  // Rysowanie wielok¥ta
  poly.aPointl = ptl;
  GpiPolygons(hps, 1, &poly, 0L, POLYGON_INCL);

  // Rysowanie kropkowanego tˆa
  if(TitleState & STT_ROTATED)
  { // Pasek w poˆo¾eniu poziomym
    // Okre˜lenie pozycji pocz¥tkowej i koäcowej
    start = wsize->xLeft  + 2 + (TitleState & STT_DRAWBUTTON ? CXY_TITLEBUTTON : 0);
    stop  = wsize->xRight - 2 - (TitleState & STT_DRAWCUT    ? CXY_TITLECUT    : 0);

    // Rysowanie pod˜wietlenia
    pos.y = wsize->yTop - 3;
    GpiSetColor(hps, TOOL_FRAMEHILITE);
    for(pos.x = start; pos.x < stop; pos.x += 3)
      GpiSetPel(hps, &pos);
    pos.y = wsize->yTop - 6;
    for(pos.x = start + 1; pos.x < stop; pos.x +=3)
      GpiSetPel(hps, &pos);

    // Rysowanie cienia
    pos.y = wsize->yTop - 4;
    GpiSetColor(hps, TOOL_FRAMEDARK);
    for(pos.x = start + 1; pos.x < stop; pos.x += 3)
      GpiSetPel(hps, &pos);
    pos.y = wsize->yTop - 7;
    for(pos.x = start + 2; pos.x < stop; pos.x += 3)
      GpiSetPel(hps, &pos);
  }
  else
  {  // Pasek w poˆo¾eniu pionowym
     // Okre˜lenie pozycji pocz¥tkowej i koäcowej
     start = wsize->yTop    - 3 - (TitleState & STT_DRAWBUTTON ? CXY_TITLEBUTTON : 0);
     stop  = wsize->yBottom + 1 + (TitleState & STT_DRAWCUT    ? CXY_TITLECUT    : 0);

     // Rysowanie pod˜wietlenia
     pos.x = wsize->xLeft + 2;
     GpiSetColor(hps, TOOL_FRAMEHILITE);
     for(pos.y = start; pos.y > stop; pos.y -= 3)
       GpiSetPel(hps, &pos);
     pos.x = wsize->xLeft + 5;
     for(pos.y = start - 1; pos.y > stop; pos.y -= 3)
       GpiSetPel(hps, &pos);

     // Rysowanie cienia
     pos.x = wsize->xLeft + 3;
     GpiSetColor(hps, TOOL_FRAMEDARK);
     for(pos.y = start - 1; pos.y > stop; pos.y -= 3)
       GpiSetPel(hps, &pos);
     pos.x = wsize->xLeft + 6;
     for(pos.y = start - 2; pos.y > stop; pos.y -= 3)
       GpiSetPel(hps, &pos);
  }
}




// Rysowanie obramowania paska tytuˆu
//
// Parametry:
//   hps       - [parametr] uchwyt presentation space
//   wsize     - [parametr] rozmiary okna
//
static VOID DrawTitleFrame(HPS hps, RECTL *wsize, ULONG TitleState)
{ POINTL pos;       // Wsp¢ˆrz©dne og¢lnego przeznaczenia

  // Rysowanie o˜wietlenia
  GpiSetColor(hps, TOOL_FRAMEHILITE);
  pos.y = /*TitleState & STT_ROTATED ?*/ wsize->yBottom /*: wsize->yBottom + 1*/;
  pos.x = wsize->xLeft;      GpiMove(hps, &pos);
  pos.y = wsize->yTop - 1;   GpiLine(hps, &pos);
  pos.x = TitleState & STT_ROTATED ? wsize->xRight - 2  : wsize->xRight - 1;
  GpiLine(hps, &pos);

  // Rysowanie cienia
  GpiSetColor(hps, TOOL_FRAMEDARK);
  // Pocz¥tek dolnego obramowania
  pos.x =/* TitleState & STT_ROTATED ?*/ wsize->xLeft + 1 /*: wsize->xLeft*/;
  pos.y = wsize->yBottom;  GpiMove(hps, &pos);
  // Koniec dolnego obramowania
  pos.x = TitleState & STT_DRAWCUT ? wsize->xRight - 1 - CXY_TITLECUT : wsize->xRight - 1;
  GpiLine(hps, &pos);

  // Rysowanie uko˜nego wyci©cia
  if(TitleState & STT_DRAWCUT)
  { pos.x = wsize->xRight - 1;
    pos.y = wsize->yBottom + CXY_TITLECUT;
    GpiLine(hps, &pos);
    // Rysowanie pogrubienia
    pos.x = wsize->xRight - CXY_TITLECUT;
    pos.y = wsize->yBottom;
    GpiMove(hps, &pos);
    pos.x = wsize->xRight - 1;
    pos.y = wsize->yBottom + CXY_TITLECUT - 1;
    GpiLine(hps, &pos);
  }

  // Rysowanie prawej kraw©dzi
  pos.y = TitleState & STT_ROTATED ? wsize->yTop - 1 : wsize->yTop - 2;
  GpiLine(hps, &pos);

  // Rysowanie tˆa pod wyci©ciem
  if(TitleState & STT_DRAWCUT)
  { POLYGON poly;        // Struktura zawiraj¥ca punkty wielok¥ta
    POINTL  ptl[2];      // Punkty wielok¥ta

    GpiSetColor(hps, TOOL_BACKGROUND);
    // Okre˜lenie punkt¢w wielok¥ta
    ptl[0].x = wsize->xRight - CXY_TITLECUT + 1;
    ptl[0].y = wsize->yBottom;
    ptl[1].x = wsize->xRight - 1;
    ptl[1].y = wsize->yBottom + CXY_TITLECUT - 2;
    // Inicjacja struktury kontrolnej wielok¥ta
    poly.ulPoints = 2;
    poly.aPointl  = ptl;
    // Punkt startowy
    pos.x = wsize->xRight - 1;
    pos.y = wsize->yBottom;
    GpiMove(hps, &pos);
    // Rysowanie wielok¥ta
    GpiPolygons(hps, 1, &poly, 0L, POLYGON_INCL);
  }
}



/*
 * $Log: tcontrol.c $
 * Revision 1.1  1999/06/27 12:35:51  Wojciech_Gazda
 * Initial revision
 *
 */
