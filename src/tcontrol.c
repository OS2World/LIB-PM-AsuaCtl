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
 * Pasek narz�dzi - dodatkowe okna kontrolne (Carrier Controls).
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


// Identyfikatory timer�w
#define TMR_TITLEHILITE       1         // Czasomierz steruj�cy pod�wietlaniem paska tytu�u
#define TMR_TITLETIP          2         // Czasomierz steruj�cy wy�wietlaniem podpowiedzi
#define TMR_ROTATEHILITE      3         // Czasomierz steruj�cy pod�wietlaniem przycisku obracania




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

// Dodatkowe funkcje us�ugowe
static VOID  DrawBitmap(HPS hps, HBITMAP hbitmap, POINTL *pos, ULONG flMode);
static VOID  DrawTitleButton(HPS hps, RECTL *wsize, ULONG TitleState);
static VOID  DrawTitleFace(HPS hps, RECTL *wsize, ULONG TitleState);
static VOID  DrawTitleFrame(HPS hps, RECTL *wsize, ULONG TitleState);





// Procedura okna steruj�ca paskiem tytu�u
//
MRESULT EXPENTRY MiniTitlebarProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
  { case WM_ADJUSTWINDOWPOS:  // Korekta skalowania
      MtlControlPos(hwnd, (SWP *)PVOIDFROMMP(mp1));
      return(0);

    case WM_BUTTON1DBLCLK:    // Obs�uga minimalizacji/maksymalizacji
    case WM_BUTTON1CLICK:     // w trybie normalnym i full window drag.
      MtlMinMaxControl(hwnd, msg, (POINTS *)&mp1);
      break;

    case WM_BUTTON1DOWN:      // Naci�ni�cie lewego przycisku myszy
      // Przes�anie komunikatu do standardowej procedury
      // w celu poprawnej aktywacji g��wnego okna aplikacji.
      WinDefWindowProc(hwnd, msg, mp1, mp2);
      // Sprawdzenie czy mo�na rozpocz�� przesuwanie okna
      MtlBeginTrack(hwnd, (POINTS *)&mp1);
      break;

    case WM_MOUSEMOVE:        // Reakcja na ruch myszy
      // Kontrola pod�wietlania, wy�wietlanie tytu�u w okienku popowiedzi
      MtlHiliteControl(hwnd, msg, 0L);
      break;

    case WM_PAINT:            // Przerysowanie paska tytu�u
      MtlRedraw(hwnd);
      break;

    case WM_TIMER:            // Sterowanie pod�wietleniem paska i wy�wietlaniem tytu�u
      MtlHiliteControl(hwnd, msg, SHORT1FROMMP(mp1));
      break;
  }

  // Powr�t do standardowej procedury okna
  return(WinDefWindowProc(hwnd, msg, mp1, mp2));
}





// Procedura obs�uguj�ca przycisk obracania okna
//
MRESULT EXPENTRY RotateButtonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
  { case WM_BUTTON1CLICK:     // Wygenerowanie ��dania obr�cenia okna
      RtbRotateRequest(hwnd);
      break;

    case WM_BUTTON1DOWN:      // Kontrola podswietlania przycisku
    case WM_BUTTON1UP:
    case WM_MOUSEMOVE:
      RtbHiliteControl(hwnd, msg, 0L);
      break;

    case WM_CALCVALIDRECTS:   // Okre�lenie obszaru nie podlegaj�cego uniewa�nieniu
      break;

    case WM_PAINT:            // Przerysowanie przycisku
      RtbRedraw(hwnd);
      return(0);

    case WM_TIMER:            // Sterowanie pod�wietleniem przycisku
      RtbHiliteControl(hwnd, msg, SHORT1FROMMP(mp1));
      break;
  }

  // Powr�t do standardowej procedury okna
  return(WinDefWindowProc(hwnd, msg, mp1, mp2));
}





//*******************/
//* Funkcje lokalne */
//*******************/

// Kontrola rozmiar�w okna podczas skalowania
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska tytu�u
//   newpos    - [parametr/rezultat] pozycja i znaczniki przekazywane przez WinSetWindowPos
//
static VOID  MtlControlPos(HWND hwnd, SWP *newpos)
{ TOOLCTL *tcl;          // Adres g��wnej struktury kontrolnej okna

  tcl = ToolLoadData(hwnd, "MtlControlPos");
  if(tcl == NULL) return;

  // Okre�lenie wymiar�w paska tytu�u
  if(tcl->flTitleState & STT_ROTATED)
  { // Pasek jest odwr�cony - rysowany poziomo
    newpos->cy = CY_TITLEHEIGHT;
  }
  else
  { // Pasek jest w pozycji normalnej - rysowany pionowo
    newpos->cx = CX_TITLEWIDTH;
  }
  // Wymuszenie zmiany rozmiaru
  newpos->fl |= SWP_SIZE;
}





// Funkcja steruje pod�wietlaniem paska tytu�u, oraz wy�wietlaniem
// podpowiedzi zawieraj�cej tytu� paska narz�dzi
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska tytu�u
//   msg       - [parametr] kod komunikatu - WM_MOUSEMOVE lub WM_TIMER
//   timer     - [parametr] numer timera
//
static VOID MtlHiliteControl(HWND hwnd, ULONG msg, ULONG timer)
{ TOOLCTL *tcl;          // Adres g��wnej struktury kontrolnej paska narz�dzi
  HAB      hab;          // Uchwyt anchor block
  RECTL    wsize;        // Rozmiary okna
  POINTL   pos;          // Pozycja kursora myszy
  LONG     tipdelay;     // Czas op��nienia przed w��czeniem podpowiedzi

  tcl = ToolLoadData(hwnd, "MtlHiliteControl");
  if(tcl == NULL) return;
  hab = WinQueryAnchorBlock(hwnd);

  // Za��czenie pod�wietlenia okna
  if(msg == WM_MOUSEMOVE)
  { // Sprawdzenie czy okno nie jest ju� pod�wietlone
    if(!(tcl->flTitleState & STT_HILITED))
    { // Pod�wietlenie okna
      tcl->flTitleState |= STT_HILITED;
      // Od�wie�enie okna
      WinInvalidateRect(hwnd, NULL, TRUE);
      // Wyzwolenie czasomierza steruj�cego pod�wietlaniem
      WinStartTimer(hab, hwnd, TMR_TITLEHILITE, TIME_TIPREMOVE);

      // Odczyt czasu op��nienia przed w��czeniem podpowiedzi
      if(!WinQueryPresParam(hwnd, PP_TOOLTIPDELAY, 0L, NULL, 4, &tipdelay, 0L))
        tipdelay = TIME_TIPDELAY;
      // Wyzwolenie czasomierza steruj�cego wy�wietlaniem podpowiedzi
      WinStartTimer(hab, hwnd, TMR_TITLETIP, (ULONG)tipdelay);
    }
  }
  if(msg == WM_TIMER)
  { // Sprawdzenie czy wy��czy� pod�wietlenie i podpowied� do paska tytu�u
    if(timer == TMR_TITLEHILITE)
    { // Sprawdzenie czy kursor znajduje si� w obr�bie okna
      WinQueryPointerPos(HWND_DESKTOP, &pos);
      // Konwersja do wsp��rz�dnych okna
      WinMapWindowPoints(HWND_DESKTOP, hwnd, &pos, 1);
      // Odczyt wymiar�w okna
      WinQueryWindowRect(hwnd, &wsize);
      // Sprawdzenie czy kursor jest wewn�trz okna
      if(WinPtInRect(hab, &wsize, &pos) == FALSE)
      { // Zatrzymanie timer�w
        WinStopTimer(hab, hwnd, TMR_TITLEHILITE);
        WinStopTimer(hab, hwnd, TMR_TITLETIP);

        // Wy��czenie podpowiedzi (je�li jest)
        if(tcl->flTitleState & STT_TOOLTIP)
        { tcl->flTitleState &= ~STT_TOOLTIP;
          // Zgaszenie okna z podpowiedzi�
          if(tcl->hwndToolTip != NULLHANDLE)
            WinSetWindowPos(tcl->hwndToolTip, HWND_TOP, 0, 0, 0, 0, SWP_HIDE);
        }

        // Wy��czenie pod�wietlenia paska
        tcl->flTitleState &= ~STT_HILITED;
        // Od�wie�enie okna
        WinInvalidateRect(hwnd, NULL, TRUE);
      }
    }

    // Wy�wietlenie tytu�u (podpowiedzi)
    if(timer == TMR_TITLETIP)
    { // Zatrzymanie czasomierza
      WinStopTimer(hab, hwnd, TMR_TITLETIP);
      // Wy�wietlenie okna z podpowiedzi�
      ToolDisplayTip(hwnd, hwnd, tcl->szWinText);
      // Uaktualnienie znacznika
      tcl->flTitleState |= STT_TOOLTIP;
    }
  }
}





// Funkcja sprawdza czy podana pozycja kursora mie�ci si� w obr�bie
// zadanego obszaru w obr�bie paska tytu�u.
//
// Parametry:
//   tcl       - [parametr] wska�nik do g��wnych struktur kontrolnych paska narz�dzi
//   cpos      - [parametr] wsp��rz�dne kursora liczone wzgl�dem paska tytu�u
//   mode      - [parametr] identyfikator testowanego obszaru
//
// Powr�t:
//   FALSE - kursor nie jest nad podanym obszarem
//   TRUE  - kursor jest nad podanym obszarem
//
static BOOL MtlTestPoint(TOOLCTL *tcl, POINTS *cpos, ULONG mode)
{ POINTL   pos;          // Pozycja kursora
  RECTL    area;         // Wsp��rz�dne tesrowanego obszaru
  HAB      hab;          // Uchwyt anchor block PM

  // Odczyt uchwytu anchor block
  hab = WinQueryAnchorBlock(tcl->hwndTitleBar);
  // Odczyt wymiar�w okna
  WinQueryWindowRect(tcl->hwndTitleBar, &area);

  // Sprawdzenie czy jest przycisk "przyklejania/odklejania" okna
  if(tcl->flTitleState & STT_DRAWBUTTON)
  { // Korekta wymiar�w obszaru movebar
    if(tcl->flTitleState & STT_ROTATED)
    { // Pasek tytu�u jest odwr�cony - le�y poziomo
      area.xLeft += (CXY_TITLEBUTTON + 1);
    }
    else
    { // Pasek tytu�u jest w normalnej pozycji - pionowo
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





// Funkcja steruje minimalizacj� i maksymalizacj� paska narz�dzi za pomoc� myszy,
// oraz przyklejaniem/odklejaniem paska od okna rodzicielskiego.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska tytu�u
//   msgcode   - [parametr] Kod komunikatu odpowiedzialnego za wywo�anie funkcji
//   cpos      - [parametr] pozycja kursora w momencie naciskania przycisku
//
static VOID  MtlMinMaxControl(HWND hwnd, ULONG msgcode, POINTS *cpos)
{ TOOLCTL *tcl;          // Adres struktur kontrolnych paska naz�dzi
  LONG     flswp;        // Zmienna steruj�ca procesem zmiany stanu okna

  tcl = ToolLoadData(hwnd, "MtlMinMaxControl");
  if(tcl == NULL) return;

  //// Sprawdzenie czy przesuwa� tylko ramk�
  if(ulWarpVer >= 40)
  { // Sprawdznie czy jest uaktywniona opcja "Full Window Drag"
    if(WinQuerySysValue(HWND_DESKTOP, SV_TRACKRECTLEVEL) != 0)
      // Ignorowanie podw�jnego klikni�cia
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
    // Okre�lenie aktualnego stanu okna
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





// Przerysowanie paska tytu�u
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska tytu�u
//
static VOID MtlRedraw(HWND hwnd)
{ TOOLCTL *tcl;                    // Adres g��wnej struktury kontrolnej paska narz�dzi
  RECTL    wsize;                  // Rozmiar okna
  HPS      hps;                    // Uchwyt presentation space

  tcl = ToolLoadData(hwnd, "MtlRedraw");
  if(tcl == NULL) return;

  // Odczyt rozmiar�w okna
  WinQueryWindowRect(hwnd, &wsize);
  // Rozpocz�cie rysowania
  hps = WinBeginPaint(hwnd, NULL, NULLHANDLE);
  // �adowanie tablicy kolor�w
  PpmQueryPresColors(tcl->hwndToolBar, TOOL_MAXCOLOR, PPmColor, tcl->colors);
  // Tworzenie logicznej tablicy kolor�w
  GpiCreateLogColorTable(hps, LCOL_PURECOLOR, LCOLF_CONSECRGB, 0L, TOOL_MAXCOLOR, tcl->colors);

  // Rysowanie obramowania
  DrawTitleFrame(hps, &wsize, tcl->flTitleState);
  // Rysowanie wn�trza przycisku
  DrawTitleFace(hps, &wsize, tcl->flTitleState);
  // Rysowanie przycisku (tr�jkcika) wewn�trz paska
  DrawTitleButton(hps, &wsize, tcl->flTitleState);

  // Zako�czenie rysowania
  WinEndPaint(hps);
}





// Sprawdzenie czy mo�na wygenerowa� komunikat WM_TRACKRECT
// i je�li mo�na, to wys�anie go do w�a�ciciela.
//
// Parametry:
//   hwnd      - [parametr] uchwyt paska tytu�u
//   cpos      - [parametr] pozycja kursora myszy
//
static VOID MtlBeginTrack(HWND hwnd, POINTS *cpos)
{ TOOLCTL *tcl;          // Struktura kontrolna paska narz�dzi
  HWND     owner;        // W�a�ciciel paska tytu�u
  ULONG    tflags;       // Zmienne steruj�ce przesuwem


  // Odczyt adresu struktury kontrolnej
  tcl = ToolLoadData(hwnd, "MtlBeginTrack");
  if(tcl == NULL) return;
  // Sprawdzenie czy kursor myszy znajduje si� nad paskiem przesuwania
  if(MtlTestPoint(tcl, cpos, MTLQ_MOVEBAR) == FALSE) return;

  // Odczyt uchwytu w�a�ciciela paska tytu�u
  owner = WinQueryWindow(hwnd, QW_OWNER);
  if(owner != NULLHANDLE)
  { // Zatrzymanie timera wy�wietlaj�cego podpowied�
    WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, TMR_TITLETIP);
    // Za��czenie pod�wietlenia
    tcl->flTitleState |= STT_HILITED;
    WinInvalidateRect(hwnd, NULL, FALSE);

    // Okre�lenie flag steruj�cych przesuwem
    tflags = TF_MOVE;
    // Sprawdzenie czy okno jest "przymocowane" do okna rodzicielskiego
    if(tcl->flToolState & TST_FIXED) tflags |= TF_ALLINBOUNDARY;
    // Wys�anie komunikatu WM_TRACKFRAME - rozpocz�cie przesuwu okna
    WinSendMsg(owner, WM_TRACKFRAME, MPFROMLONG(tflags), 0L);

    // Wy��czenie pod�wietlenia
    tcl->flTitleState &= ~STT_HILITED;
  }
}





// Funkcja steruje pod�wietlaniem przycisku obracania okna
//
// Parametry:
//   hwnd      - [parametr] uchwyt przycisku obracania
//   msg       - [parametr] kod komunikatu - WM_MOUSEMOVE lub WM_TIMER
//   timer     - [parametr] numer timera
//
static VOID RtbHiliteControl(HWND hwnd, ULONG msg, ULONG timer)
{ TOOLCTL *tcl;          // Wska�nik do g��wnej struktury kontrolnej okna
  POINTL   pos;          // Pozycja kursora wewn�trz okna
  RECTL    wsize;        // Wymiary okna
  HAB      hab;          // Uchwyt anchor block PM

  tcl = ToolLoadData(hwnd, "RtbHiliteControl");
  if(tcl == NULL) return;
  hab = WinQueryAnchorBlock(hwnd);

  if(msg == WM_MOUSEMOVE)
  { // Sprawdzenie czy okno nie jest ju� pod�wietlone,
    // i czy przycisk myszy nie jest w dolnym po�o�eniu
    if(!(tcl->flRotateState & (SRB_HILITED | SRB_BUTTONDOWN)))
    { // Pod�wietlenie okna
      tcl->flRotateState |= SRB_HILITED;
      // Od�wie�enie okna
      WinInvalidateRect(hwnd, NULL, TRUE);
      // Wyzwolenie czasomierza steruj�cego pod�wietlaniem
      WinStartTimer(hab, hwnd, TMR_ROTATEHILITE, TIME_TIPREMOVE);
    }
  }
  if(msg == WM_TIMER)
  { // Sprawdzenie czy wy��czy� pod�wietlenie przycisku
    if(timer == TMR_ROTATEHILITE)
    { // Sprawdzenie czy kursor znajduje si� w obr�bie okna
      WinQueryPointerPos(HWND_DESKTOP, &pos);
      // Konwersja do wsp��rz�dnych okna
      WinMapWindowPoints(HWND_DESKTOP, hwnd, &pos, 1);
      // Odczyt wymiar�w okna
      WinQueryWindowRect(hwnd, &wsize);
      // Sprawdzenie czy kursor jest wewn�trz okna
      if(WinPtInRect(hab, &wsize, &pos) == FALSE)
      { // Zatrzymanie timera
        WinStopTimer(hab, hwnd, TMR_ROTATEHILITE);
        // Wy��czenie pod�wietlenia paska
        tcl->flRotateState &= ~SRB_HILITED;
        // Od�wie�enie okna
        WinInvalidateRect(hwnd, NULL, TRUE);
      }
    }
  }
  if(msg == WM_BUTTON1DOWN)
  { // Przej�cie kursora myszy
    WinSetCapture(HWND_DESKTOP, hwnd);
    // Sprawdzenie czy wy��czy� pod�wietlanie przycisku
    if(tcl->flRotateState & SRB_HILITED)
    { // Wy��czenie po�wietlania
      tcl->flRotateState &= ~SRB_HILITED;
      // Zaznaczenie naci�ni�cia przycisku
      tcl->flRotateState |=  SRB_BUTTONDOWN;
      // Przerysowanie okna
      WinInvalidateRect(hwnd, NULL, TRUE);
    }
  }
  if(msg == WM_BUTTON1UP)
  { // Zwolnienie kursora myszy
    WinSetCapture(HWND_DESKTOP, NULLHANDLE);
    // Usuni�cie znacznika naci�ni�cia przycisku
    tcl->flRotateState &= ~SRB_BUTTONDOWN;
  }
}





// Przerysowanie przycisku obracania okna
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna przycisku
//
static VOID RtbRedraw(HWND hwnd)
{ TOOLCTL *tcl;          // Wska�nik do struktur kontrolnych paska narz�dzi
  POINTL   pos;          // Wsp��rz�dne og�lnego przeznaczenia
  RECTL    wsize;        // Wymiary okna
  HBITMAP  hbitmap;      // Uchwyt mapy bitowej
  HPS      hps;          // Uchwyt presentation space okna


  tcl = ToolLoadData(hwnd, "RtbRedraw");
  if(tcl == NULL) return;

  hps = WinBeginPaint(hwnd, NULLHANDLE, NULL);
  // �adowanie tablicy kolor�w
  PpmQueryPresColors(tcl->hwndToolBar, TOOL_MAXCOLOR, PPmColor, tcl->colors);
  // Tworzenie logicznej tablicy kolor�w
  GpiCreateLogColorTable(hps, LCOL_PURECOLOR, LCOLF_CONSECRGB, 0L, TOOL_MAXCOLOR, tcl->colors);
  // Okre�lenie wymiar�w okna
  WinQueryWindowRect(hwnd, &wsize);

  // Kasowanie t�a
  GpiSetColor(hps, TOOL_BACKGROUND);
  // Kasowanie wn�trza przycisku
  pos.x = wsize.xLeft; pos.y = wsize.yBottom;
  GpiMove(hps, &pos);
  pos.x = wsize.xRight - 1; pos.y = wsize.yTop - 1;
  GpiBox(hps, DRO_FILL, &pos, 0, 0);

  // Obliczenie po�o�enia kopiowanych map bitowych
  pos.x = (wsize.xLeft   + wsize.xRight - CX_ROTATEBITMAP) / 2;
  pos.y = (wsize.yBottom + wsize.yTop   - CY_ROTATEBITMAP) / 2;

  // Rysowanie maski bitowej (wyci�cie t�a)
  hbitmap  = GpiLoadBitmap(hps, hResource, ROTATE_BACK,    0, 0);
  if(hbitmap != NULLHANDLE)
  { // Ustawienie atrybut�w - koloru t�a i punktu
    GpiSetColor(hps, CLR_WHITE);
    GpiSetBackColor(hps, CLR_BLACK);
    // Kopiowanie mapy bitowej
    DrawBitmap(hps, hbitmap, &pos, ROP_SRCAND);
    // Usuni�cie mapy bitowej
    GpiDeleteBitmap(hbitmap);
  }

  // Wype�nienie strza�ki
  hbitmap  = GpiLoadBitmap(hps, hResource, ROTATE_FILL,    0, 0);
  if(hbitmap != NULLHANDLE)
  { // Ustawienie atrybut�w - koloru t�a i punktu
    GpiSetColor(hps, tcl->flRotateState & SRB_HILITED ? TOOL_HILITEBACKGROUND : TOOL_FRAMEDARK);
    GpiSetBackColor(hps, CLR_BLACK);
    // Kopiowanie mapy bitowej
    DrawBitmap(hps, hbitmap, &pos, ROP_SRCPAINT);
    // Usuni�cie mapy bitowej
    GpiDeleteBitmap(hbitmap);
  }

  // Na�o�enie konturu
  hbitmap  = GpiLoadBitmap(hps, hResource, ROTATE_OUTLINE, 0, 0);
  if(hbitmap != NULLHANDLE)
  { // Kopiowanie mapy bitowej
    DrawBitmap(hps, hbitmap, &pos, ROP_SRCPAINT);
    // Usuni�cie mapy bitowej
    GpiDeleteBitmap(hbitmap);
  }

  WinEndPaint(hps);
}





// Wygenerowanie ��dania obr�cenia paska narz�dzi
//
// Parametry:
//   hwnd      - [parametr] uchwyt przycisku obracania okna
//
static VOID  RtbRotateRequest(HWND hwnd)
{ TOOLCTL *tcl;     // Wska�nik do g��wnych struktur kontrolnych okna

  tcl = ToolLoadData(hwnd, "RtbRotateRequest");
  if(tcl == NULL) return;

  // Wys�anie komunikatu TM_ROTATE do paska narz�dzi
  WinSendMsg(tcl->hwndToolBar, TM_ROTATE, 0L, 0L);
}





/******************************/
/* Dodatkowe funkcje us�ugowe */
/******************************/

// Rysowanie mapy bitowej. Rozmiary s� odczytywane automatycznie.
//
// Parametry:
//   hps       - [parametr] presentation space
//   hbitmap   - [parametr] uchwyt rysowanej mapy bitowej
//   pos       - [parametr] po�o�enie lewego dolnego rogu
//   flMode    - [parametr] tryb nak�adania mapy bitowej
//
static VOID DrawBitmap(HPS hps, HBITMAP hbitmap, POINTL *pos, ULONG flMode)
{ BITMAPINFOHEADER2 *bfh;     // Wska�nik do pocz�tku struktury informacyjnej
  POINTL bmpos[4];            // Pozycja mapy bitowej (wsp��rz�dzne dla GpiWCBitBlt)
  UCHAR  tmp[16];             // Bufor na pocz�tkow� cz��� struktury BITMAPINFOHEADER2

  // Inicjacja zmiennych
  bfh = (BITMAPINFOHEADER2 *)tmp;
  // Odczyt wymiar�w mapy bitowej
  bfh->cbFix = 16;
  if(GpiQueryBitmapInfoHeader(hbitmap, bfh) == FALSE) return;

  // Obliczenie wsp��rz�dnych kopiowanych map bitowych
  // Wsp��rz�dne lewego dolnego rogu obszaru docelowego
  bmpos[0].x = pos->x;
  bmpos[0].y = pos->y;
  // Wsp��rz�dne prawego g�rnego rogu obszaru docelowego
  bmpos[1].x = bmpos[0].x + bfh->cx - 1;
  bmpos[1].y = bmpos[0].y + bfh->cy - 1;
  // Wsp��rz�dne lewego dolnego rogu obszaru �r�d�owego
  bmpos[2].x = 0;
  bmpos[2].y = 0;
  // Wsp��rz�dne prawego g�rnego rogu obszaru �r�d�owego
  bmpos[3].x = bfh->cx;
  bmpos[3].y = bfh->cy;

  // Kopiowanie mapy bitowej
  GpiWCBitBlt(hps, hbitmap, 4L, bmpos, flMode, 0L);
}





// Rysowanie przycisku (tr�jk�cika) na pasku tytu�u
//
// Parametry:
//   hps       - [parametr] uchwyt presentation space
//   wsize     - [parametr] rozmiary okna
//   tcl       - [parametr] wska�nik do struktur kontrolnych paska narz�dzi
//
static VOID DrawTitleButton(HPS hps, RECTL *wsize, ULONG TitleState)
{ POINTL  pos;           // Wsp��rz�dne og�lnego przeznaczenia
  HBITMAP hbmMono;       // Uchwyt monochromatycznej maski bitowej
  HBITMAP hbmColor;      // Uchwyt kolorowej mapy bitowej wy�wietlanej na przycisku

  // Sprawdzenie czy opcje pozawalaj� na rysowania
  if(!(TitleState & STT_DRAWBUTTON)) return;

  // Kasowanie t�a pod przycisk
  // Okre�lenie koloru t�a
  GpiSetColor(hps, TitleState & STT_HILITED ? TOOL_HILITEBACKGROUND : TOOL_BACKGROUND);

  // Wsp��rz�dne lewego dolnego rogu obszaru docelowego
  pos.x = wsize->xLeft + 1;
  pos.y = TitleState & STT_ROTATED ? wsize->yBottom  + 1 : wsize->yTop   - 1 - CXY_TITLEBUTTON;
  GpiMove(hps, &pos);
  // Wsp��rz�dne prawego g�rnego rogu obszaru docelowego
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

  // Wsp��rz�dne lewego dolnego rogu obszaru docelowego
  pos.x = wsize->xLeft + 2;
  pos.y = wsize->yTop - 7;

  // Rysowanie t�a pod kolorow� map� bitow�
  if(hbmMono != NULLHANDLE)
  { // Ustawienie atrybut�w - koloru t�a i punktu
    GpiSetColor(hps, CLR_WHITE);
    GpiSetBackColor(hps, CLR_BLACK);
    // Kopiowanie mapy bitowej
    DrawBitmap(hps, hbmMono, &pos, ROP_SRCAND);
    // Usuni�cie mapy bitowej
    GpiDeleteBitmap(hbmMono);
  }
  // Rysowanie przycisku - kolorowej mapy bitowej
  if(hbmColor != NULLHANDLE)
  { // Kopiowanie mapy bitowej
    DrawBitmap(hps, hbmColor, &pos, ROP_SRCPAINT);
    // Usuni�cie mapy bitowej
    GpiDeleteBitmap(hbmColor);
  }
}





// Rysowanie wn�trza paska tytu�u
//
// Parametry:
//   hps       - [parametr] uchwyt presentation space
//   wsize     - [parametr] rozmiary okna
//   tcl       - [parametr] wska�nik do struktur kontrolnych paska narz�dzi
//
static VOID DrawTitleFace(HPS hps, RECTL *wsize, ULONG TitleState)
{ POLYGON poly;          // Definicja wielok�ta
  POINTL  ptl[4];        // Punkty wielok�ta
  POINTL  pos;           // Wsp��rz�dne og�lnego przenaczenia
  LONG    start, stop;   // Pozycja pocz�tkowa i ko�cowa


  // Okre�lenie koloru t�a
  GpiSetColor(hps, TitleState & STT_HILITED ? TOOL_HILITEBACKGROUND : TOOL_BACKGROUND);
  // Rysowanie t�a
  // Ustawienie punktu pocz�tkowego
  pos.x = wsize->xRight - 2;
  pos.y = wsize->yTop   - 2;
  if(TitleState & STT_DRAWBUTTON)
    pos.y -= (TitleState & STT_ROTATED ? 0 : CXY_TITLEBUTTON);
  GpiMove(hps, &pos);

  // Okre�lenie liczby punkt�w
  poly.ulPoints = 3;

  // Lewy g�rny r�g
  ptl[0].x = wsize->xLeft + 1;
  ptl[0].y = wsize->yTop  - 2;
  if(TitleState & STT_DRAWBUTTON)
  { ptl[0].x += (TitleState & STT_ROTATED ? CXY_TITLEBUTTON : 0);
    ptl[0].y -= (TitleState & STT_ROTATED ? 0 : CXY_TITLEBUTTON);
  }

  // Lewy dolny r�g
  ptl[1].x = wsize->xLeft   + 1;
  ptl[1].y = wsize->yBottom + 1;
  if(TitleState & STT_DRAWBUTTON)
    ptl[1].x += (TitleState & STT_ROTATED ? CXY_TITLEBUTTON : 0);

  // Prawy dolny r�g
  ptl[2].x = wsize->xRight  - 2;
  ptl[2].y = wsize->yBottom + 1;

  // Obliczenie pozycji punkt�w podczas rysowania wyci�cia
  if(TitleState & STT_DRAWCUT)
  { // O jeden punkt wi�cej
    poly.ulPoints ++;
    // Korekta pierwszego punktu wyci�cia
    ptl[2].x = wsize->xRight - CXY_TITLECUT - 1;
    // Pozycja drugiego punktu wyci�cia
    ptl[3].x = wsize->xRight - 2;
    ptl[3].y = wsize->yBottom + CXY_TITLECUT;
  }

  // Rysowanie wielok�ta
  poly.aPointl = ptl;
  GpiPolygons(hps, 1, &poly, 0L, POLYGON_INCL);

  // Rysowanie kropkowanego t�a
  if(TitleState & STT_ROTATED)
  { // Pasek w po�o�eniu poziomym
    // Okre�lenie pozycji pocz�tkowej i ko�cowej
    start = wsize->xLeft  + 2 + (TitleState & STT_DRAWBUTTON ? CXY_TITLEBUTTON : 0);
    stop  = wsize->xRight - 2 - (TitleState & STT_DRAWCUT    ? CXY_TITLECUT    : 0);

    // Rysowanie pod�wietlenia
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
  {  // Pasek w po�o�eniu pionowym
     // Okre�lenie pozycji pocz�tkowej i ko�cowej
     start = wsize->yTop    - 3 - (TitleState & STT_DRAWBUTTON ? CXY_TITLEBUTTON : 0);
     stop  = wsize->yBottom + 1 + (TitleState & STT_DRAWCUT    ? CXY_TITLECUT    : 0);

     // Rysowanie pod�wietlenia
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




// Rysowanie obramowania paska tytu�u
//
// Parametry:
//   hps       - [parametr] uchwyt presentation space
//   wsize     - [parametr] rozmiary okna
//
static VOID DrawTitleFrame(HPS hps, RECTL *wsize, ULONG TitleState)
{ POINTL pos;       // Wsp��rz�dne og�lnego przeznaczenia

  // Rysowanie o�wietlenia
  GpiSetColor(hps, TOOL_FRAMEHILITE);
  pos.y = /*TitleState & STT_ROTATED ?*/ wsize->yBottom /*: wsize->yBottom + 1*/;
  pos.x = wsize->xLeft;      GpiMove(hps, &pos);
  pos.y = wsize->yTop - 1;   GpiLine(hps, &pos);
  pos.x = TitleState & STT_ROTATED ? wsize->xRight - 2  : wsize->xRight - 1;
  GpiLine(hps, &pos);

  // Rysowanie cienia
  GpiSetColor(hps, TOOL_FRAMEDARK);
  // Pocz�tek dolnego obramowania
  pos.x =/* TitleState & STT_ROTATED ?*/ wsize->xLeft + 1 /*: wsize->xLeft*/;
  pos.y = wsize->yBottom;  GpiMove(hps, &pos);
  // Koniec dolnego obramowania
  pos.x = TitleState & STT_DRAWCUT ? wsize->xRight - 1 - CXY_TITLECUT : wsize->xRight - 1;
  GpiLine(hps, &pos);

  // Rysowanie uko�nego wyci�cia
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

  // Rysowanie prawej kraw�dzi
  pos.y = TitleState & STT_ROTATED ? wsize->yTop - 1 : wsize->yTop - 2;
  GpiLine(hps, &pos);

  // Rysowanie t�a pod wyci�ciem
  if(TitleState & STT_DRAWCUT)
  { POLYGON poly;        // Struktura zawiraj�ca punkty wielok�ta
    POINTL  ptl[2];      // Punkty wielok�ta

    GpiSetColor(hps, TOOL_BACKGROUND);
    // Okre�lenie punkt�w wielok�ta
    ptl[0].x = wsize->xRight - CXY_TITLECUT + 1;
    ptl[0].y = wsize->yBottom;
    ptl[1].x = wsize->xRight - 1;
    ptl[1].y = wsize->yBottom + CXY_TITLECUT - 2;
    // Inicjacja struktury kontrolnej wielok�ta
    poly.ulPoints = 2;
    poly.aPointl  = ptl;
    // Punkt startowy
    pos.x = wsize->xRight - 1;
    pos.y = wsize->yBottom;
    GpiMove(hps, &pos);
    // Rysowanie wielok�ta
    GpiPolygons(hps, 1, &poly, 0L, POLYGON_INCL);
  }
}



/*
 * $Log: tcontrol.c $
 * Revision 1.1  1999/06/27 12:35:51  Wojciech_Gazda
 * Initial revision
 *
 */
