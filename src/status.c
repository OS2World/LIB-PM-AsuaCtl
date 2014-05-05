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

/* Dodatkowe kontrolki panelu
 * (c) 1999 Wojciech Gazda
 *
 * ctlmisc.c
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/04/18 22:54:59 $
 * $Name:  $
 * $RCSfile: status.c $
 * $Revision: 2.2 $
 *
 */

#define INCL_DOSDEV

// Deklaracje OS/2
#define INCL_DOSMISC
#define INCL_GPILOGCOLORTABLE
#define INCL_GPIPRIMITIVES
#define INCL_GPIREGIONS
#define INCL_GPILCIDS
#define INCL_WINSYS
#define INCL_WINSTDDRAG
#define INCL_WINWINDOWMGR
#define INCL_WINMESSAGEMGR
#include <os2.h>

// Funkcje biblioteczne
#include <malloc.h>
#include <string.h>

// Deklaracje lokalne
#define  __INTERNAL_USE__
#include "asuintl.h"
#include "status.h"


// Deklaracje staˆych
// Indeksy kolejnych kolor¢w okna WC_STATUS
#define STA_FRAMEHILITE       0    // PP_BORDERLIGHTCOLOR        O˜wietlenie ramki
#define STA_FRAMEDARK         1    // PP_BORDERDARKCOLOR         Cieä ramki
#define STA_ACTIVEBKGND       2    // PP_BACKGROUNDCOLOR         Kolor tˆa aktywnej ramki
#define STA_INACTVBKGND       3    // PP_DISABLEDBACKGROUNDCOLOR Kolor tˆa zablokowanej ramki
#define STA_ACTIVETEXT        4    // PP_FOREGROUNDCOLOR         Kolor tekstu aktywnej ramki
#define STA_INACTVTEXT        5    // PP_DISABLEDFOREGROUNDCOLOR Kolor tekstu nieaktywnej ramki
#define STA_FRAMEHILITE2      6    // PP_BORDER2LIGHTCOLOR       Kolor pod˜wietlenia wewn©trznego marginesu
#define STA_FRAMEDARK2        7    // PP_BORDER2DARKCOLOR        Kolor zacienienia wewn©trznego marginesu

// Liczba kolor¢w
#define STA_MAXCOLOR          8

// Rozmiary okna stylu SS_WINSTYLE (staˆe rozmiary)
#define CX_WINSTYLE           12   // Szeroko˜† okna z atrybutem SS_WINSTYLE
#define CY_WINSTYLE           12   // Wysoko˜† okna z atrybutem SS_WINSTYLE



// Typy danych
// Struktura kontrolna okna WC_STATUS
typedef struct
{ FONTMETRICS fm;                  // Parametry bie¾¥cej czcionki
  ULONG ulHilite;                  // Aktualny stan okna
  ULONG ulWinstyle;                // Styl okna
  ULONG ulColor[STA_MAXCOLOR];     // Kolory element¢w okna
  ULONG ulTextWidth;               // Dˆugo˜† tekstu w pikselach
  BOOL  bForceRedraw;              // Znacznik wymuszenia rysowania tˆa dla SS_OWNERDRAW
  PSZ   szWintext;                 // Tekst wy˜wietlany w oknie
} STATUS;



// Zmienne globalne
// Lista parametr¢w prezentacji u¾ytych w obr©bie okna + lista odpowiadaj¥cych im kolor¢w
static PPTEMPLATE PresParams[STA_MAXCOLOR] =
  {{ PP_BORDERLIGHTCOLOR,        0L,    // O˜wietlenie ramki
     0,                          0xFFFFFF,
   },
   { PP_BORDERDARKCOLOR,         0L,    // Cieä ramki
     0,                          0x808080,
   },
   { PP_BACKGROUNDCOLOR,         0L,    // Kolor tˆa aktywnej ramki
     SYSCLR_ENTRYFIELD,          0xFFFFFF,
   },
   { PP_DISABLEDBACKGROUNDCOLOR, 0L,    // Kolor tˆa zablokowanej ramki
     SYSCLR_ENTRYFIELD,          0xFFFFFF,
   },
   { PP_FOREGROUNDCOLOR,         0L,    // Kolor tekstu aktywnej ramki
     SYSCLR_WINDOWTEXT,          0x000000,
   },
   { PP_DISABLEDFOREGROUNDCOLOR, 0L,    // Kolor tekstu nieaktywnej ramki
     0,                          0x808080,
   },
   { PP_BORDER2LIGHTCOLOR,       0L,    // Kolor pod˜wietlenia wewn©trznego marginesu
     0,                          0xCCCCCC,
   },
   { PP_BORDER2DARKCOLOR,        0L,    // Kolor zacienienia wewn©trznego marginesu
     0,                          0x000000,
   },
  };



// Wersja kontrolki
static CTLINFO ctlStatusVersion =
{ sizeof(CTLINFO),
  NULL,
  2, 20
};



// Prototypy funkcji
LONG ASUAAPI CtlStatusInitialize(HAB hab);
MRESULT EXPENTRY WinStatusProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// Prototypy funkcji lokalnych
static MRESULT EXPENTRY WinStatusProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static VOID    StatAutoscale(HWND hwnd, SWP *swp);
static VOID    StatCalcInterior(RECTL *inter, ULONG winstyle);
static VOID    StatDrawFrame(HPS hps, RECTL *size, ULONG winstyle);
static VOID    StatDrawText(HWND hwnd, HPS hps, RECTL *size, STATUS *stat);
static VOID    StatDrawWinstyle(HPS hps, RECTL *size, STATUS *stat);
static LONG    StatLoadWndParams(HWND hwnd, WNDPARAMS *wpr);
static MRESULT StatProcessMouse(HWND hwnd);
static VOID    StatRedraw(HWND hwnd);
static VOID    StatSetPresParam(HWND hwnd, LONG ppm);
static VOID    StatSetText(HWND hwnd, WNDPARAMS *wpr);
static VOID    StatStoreFont(HWND hwnd, STATUS *stat);
static VOID    StatStoreWindowText(HWND hwnd, STATUS *stat, PSZ text);
static LONG    StatUserCommand(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);




// Przygotowanie okna WC_STATUS do pracy
//
// Parametry:
//   hab       - [parametr] uchwyt anchor block
//
// Powr¢t:
//   0    - poprawne wykonanie funkcji
//   1    - bˆ¥d rejestracji klasy okna
//
LONG ASUAAPI CtlStatusInitialize(HAB hab)
{ ULONG winstyle;   // Domy˜lny styl okna
  LONG  rc;         // Kody bˆ©d¢w zwracane przez funkcje systemowe

  // Ustalenie domy˜lnego stylu okna WC_STATUS
  winstyle = CS_SIZEREDRAW | CS_HITTEST;
  // Rejestracja nowej klasy okna - samoskaluj¥ce si© okno statusowe
  rc = WinRegisterClass(hab, WC_STATUS, WinStatusProc, winstyle, sizeof(VOID *));
  if(!rc) return(1);

  return(0);
}





/*******************/
/* Funkcje lokalne */
/*******************/

// Procedura rysuj¥ca i kontroluj¥ca okno statusowe WC_STATUS
//
MRESULT EXPENTRY WinStatusProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{ STATUS  *stat;         // Adres struktur kontrolnych okna

  // Wykonywanie komunikat¢w
  switch(msg)
  { case WM_ADJUSTWINDOWPOS:  // Automatyczne skalowanie okna
      StatAutoscale(hwnd, (SWP *)PVOIDFROMMP(mp1));
      break;


    case WM_CREATE:           // Tworzenie okna
      { // Struktura inicjuj¥ca przekazywana przez system
        CREATESTRUCT *pcr;

        // Alokacja pami©ci dla struktury kontrolnej okna
        stat = malloc(sizeof(STATUS));
        if(stat == NULL) return(MRFROMLONG(TRUE));
        memset(stat, 0, sizeof(STATUS));

        // Odczyt adresu struktury inicjuj¥cej
        pcr = (CREATESTRUCT *)PVOIDFROMMP(mp2);
        // Zaˆadowanie presentation parameters ze struktury CREATESTRUCT
        PpmStorePresParams(hwnd, (PRESPARAMS *)pcr->pPresParams, PresParams, STA_MAXCOLOR);
        // Zapami©tanie kolor¢w pobranych z presentation parameters
        PpmQueryPresColors(hwnd, STA_MAXCOLOR, PresParams, (LONG *)stat->ulColor);
        // Zapami©tanie czcionki pobranej z presentation parameters
        StatStoreFont(hwnd, stat);
        // Zapami©tanie tekstu
        StatStoreWindowText(hwnd, stat, pcr->pszText);

        // Zapami©tanie stylu okna
        stat->ulWinstyle = pcr->flStyle;
        // Standardowo, okno jest pod˜wietlone
        stat->ulHilite = TRUE;

        // Zapami©tanie wsk«nika do struktury kontrolnej
        WinSetWindowPtr(hwnd, 0L, stat);
        // Poprawne wykonanie inicjacji
        return(MRFROMLONG(FALSE));
      }
      break;


    case WM_DESTROY:          // Zamykanie okna
      { // Odczyt adresu struktur kontrolnych
        stat = WinQueryWindowPtr(hwnd, 0L);
        // Zwolnienie pami©ci
        if(stat->szWintext != NULL) free(stat->szWintext);
        if(stat != NULL)            free(stat);
        return(0);
      }
      break;


    case WM_ENABLE:           // Zmiana stanu okna - wymuszenie przerysowania
      WinInvalidateRect(hwnd, NULL, FALSE);
      break;


    case WM_HITTEST:               // STerowanie przepˆywem komunikat¢w myszy
      return(StatProcessMouse(hwnd));

    case WM_NULL:                  // Odczyt wersji kontrolki
      AsuQueryCtlInfo(hwnd, msg, mp1, &ctlStatusVersion);
      break;

    case WM_PAINT:                 // Przerysowanie okna
      StatRedraw(hwnd);
      break;


    case WM_PRESPARAMCHANGED:      // Obsˆuga WinSetPesParam
      StatSetPresParam(hwnd, LONGFROMMP(mp1));
      break;


    case WM_QUERYWINDOWPARAMS:     // Odczyt parametr¢w okna
      return(MRFROMLONG(StatLoadWndParams(hwnd, (WNDPARAMS *)PVOIDFROMMP(mp1))));


    case WM_SETWINDOWPARAMS:       // Obsˆuga WinSetWindowText
      StatSetText(hwnd, (WNDPARAMS *)PVOIDFROMMP(mp1));
      break;


    case WM_SYSCOLORCHANGE:        // Zmianie ulegˆy kolory systemowe
      // Zmianie ulegˆy kolory
      { STATUS *stat;     // Adres struktury kontrolnej okna

        // Odczyt adresu struktur kontrolnych
        stat = WinQueryWindowPtr(hwnd, 0L);
        if(stat == NULL) break;
        // Uaktualnienie kolor¢w
        PpmQueryPresColors(hwnd, STA_MAXCOLOR, PresParams, (LONG *)stat->ulColor);
      }
      break;
  }

  // Interpretacja komunikat¢w steruj¥cych
  if((msg >= STAT_FIRST_MSG) && (msg <= STAT_LAST_MSG))
    return(MRFROMLONG(StatUserCommand(hwnd, msg, mp1, mp2)));

  // Przekazanie nieprzetworzonych komunikat¢w do standardowej procedury okna
  return(WinDefWindowProc(hwnd, msg, mp1, mp2));
}





// Automatyczne skalowanie okna - dostosowaie do wymiar¢w tekstu
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna
//   swp       - [parametr/rezultat] modyfikowana struktura, zawieraj¥ca poprawione wsp¢ˆrz©dne
//
static VOID StatAutoscale(HWND hwnd, SWP *swp)
{ STATUS *stat;     // Adres struktury kontrolnej okna
  LONG    border;   // Szeroko˜† obramowania

  // Odczyt adresu struktur kontrolnych
  stat = WinQueryWindowPtr(hwnd, 0L);
  if(stat == NULL) return;

  // Sprawdzenie czy okno jest stylu SS_WINSTYLE
  if(stat->ulWinstyle & SS_WINSTYLE)
  { // Ustalenie staˆych rozmiar¢w
    swp->cx = CX_WINSTYLE;
    swp->cy = CY_WINSTYLE;
  }
  else
  { // Okre˜lenie szeroko˜ci obramowania
    if(stat->ulWinstyle & SS_THINFRAME) border = 1;
    else                                border = 2;

    // Sprawdzenie czy mo¾na skalowa† w poziomie
    if(stat->ulWinstyle & SS_XAUTOSCALE)
    { // Uwzgl©dnienie dˆugo˜ci tekstu
      swp->cx = stat->ulTextWidth + (stat->fm.lMaxCharInc >> 1);
      // Uwzgl©nienie ramki
      if(stat->ulWinstyle & SS_LEFTFRAME)  swp->cx += border;
      if(stat->ulWinstyle & SS_RIGHTFRAME) swp->cx += border;
    }

    // Sprawdzenie czy mo¾na skalowa† w pionie
    if(stat->ulWinstyle & SS_YAUTOSCALE)
    { // Uwzgl©dnienie wysoko˜ci tekstu
      swp->cy = stat->fm.lMaxBaselineExt;
      // Uwzgl©dnienie ramki
      if(stat->ulWinstyle & SS_TOPFRAME)    swp->cy += border;
      if(stat->ulWinstyle & SS_BOTTOMFRAME) swp->cy += border;
    }
  }
}





// Funkcja oblicza rozmiar i poˆo¾enie obszaru wewn¥trz obramowania
//
// Parametry:
//   inter     - [parametr/rezultat] wsp¢ˆrz©dne obszaru «r¢dˆowego i wynikowego
//   winstyle  - [parametr] styl okna
//
static VOID StatCalcInterior(RECTL *inter, ULONG winstyle)
{ LONG border;      // Grubo˜† ramki w pikselach

  // Okre˜lenie grubo˜ci ramki
  if(winstyle & SS_THINFRAME) border = 1;
  else                        border = 2;

  // Odj©cie grubo˜ci lewej ramki
  if(winstyle & SS_LEFTFRAME)
    inter->xLeft += border;

  // Odj©cie grubo˜ci prawej ramki
  if(winstyle & SS_RIGHTFRAME)
    inter->xRight -= border;

  // Odj©cie grubo˜ci g¢rnej ramki
  if(winstyle & SS_TOPFRAME)
    inter->yTop -= border;

  // Odj©cie grubo˜ci dolnejramki
  if(winstyle & SS_BOTTOMFRAME)
    inter->yBottom += border;
}





// Rysowanie obramowania okna, zale¾ne od jego stylu
//
// Parametry:
//   hps       - [parametr] uchwyt presentation space
//   wsize     - [parametr] rozmiary okna
//   winstyle  - [parametr] styl okna
//
static VOID StatDrawFrame(HPS hps, RECTL *size, ULONG winstyle)
{ POINTL pos;       // Wsp¢ˆrz©dne og¢lnego przeznaczenia
  RECTL  inter;     // Granice obszaru wewn¥trz obramowania
  RECTL  wsize;     // Rozmiary okna

  // Inicjacja zmiennych
  wsize = *size;
  inter = *size;

  // Zewn©trzne obramowanie
  GpiSetColor(hps, (winstyle & SS_THINFRAME ? STA_FRAMEDARK2 : STA_FRAMEHILITE));
  // Prawa kraw©d«
  pos.x = wsize.xRight - 1;
  if(winstyle & SS_RIGHTFRAME)
  { pos.y = wsize.yTop - 1;
    GpiMove(hps, &pos);
    pos.y = wsize.yBottom;
    GpiLine(hps, &pos);
    inter.xRight --;
  }
  else
  { pos.y = wsize.yBottom;
    GpiMove(hps, &pos);
  }

  // Dolna kraw©d«
  pos.x = wsize.xLeft;
  if(winstyle & SS_BOTTOMFRAME)
  { GpiLine(hps, &pos);
    inter.yBottom ++;
  }
  else GpiMove(hps, &pos);

  // Lewa kraw©d«
  GpiSetColor(hps, (winstyle & SS_THINFRAME ? STA_FRAMEDARK2 : STA_FRAMEDARK));
  pos.y = wsize.yTop - 1;
  if(winstyle & SS_LEFTFRAME)
  { GpiLine(hps, &pos);
    inter.xLeft ++;
  }
  else GpiMove(hps, &pos);

  // G¢rna kraw©d«
  pos.x = wsize.xRight - 1;
  if(winstyle & SS_TOPFRAME)
  { GpiLine(hps, &pos);
    inter.yTop --;
  }


  // Sprawdzenie czy mo¾na rysowa† wewn©trzne obramowanie
  if(!(winstyle & SS_THINFRAME))
    if((inter.xRight > inter.xLeft) && (inter.yTop > inter.yBottom))
    { // Zapami©tanie nowego rozmiaru okna
      wsize = inter;

      // Wewn©trzne obramowanie
      GpiSetColor(hps, (ulWarpVer > 30 ? STA_FRAMEDARK2 : STA_FRAMEHILITE));
      // G¢rna kraw©d«
      pos.x = wsize.xRight - 1;
      pos.y = wsize.yTop - 1;
      if(winstyle & SS_TOPFRAME)
      { GpiMove(hps, &pos);
        pos.x = wsize.xLeft;
        GpiLine(hps, &pos);
      }
      else
      { pos.x = wsize.xLeft;
        GpiMove(hps, &pos);
      }

      // Lewa kraw©d«
      pos.y = wsize.yBottom;
      if(winstyle & SS_LEFTFRAME)
        GpiLine(hps, &pos);
      GpiMove(hps, &pos);

      // Dolna kraw©d«
      GpiSetColor(hps, (ulWarpVer > 30 ? STA_FRAMEHILITE2 : STA_FRAMEDARK));
      pos.x = wsize.xRight - 1;
      if(winstyle & SS_BOTTOMFRAME)
        GpiLine(hps, &pos);
      else
        GpiMove(hps, &pos);

      // Prawa kraw©d«
      pos.y = wsize.yTop - 1;
      if(winstyle & SS_RIGHTFRAME)
        GpiLine(hps, &pos);

      // Zaokr¥glenie ramki (tylko Warp3)
      if(ulWarpVer < 40)
      { GpiSetColor(hps, STA_ACTIVEBKGND);
        if((winstyle & SS_TOPFRAME) && (winstyle & SS_RIGHTFRAME))
        { pos.x = wsize.xRight; pos.y = wsize.yTop;
          GpiSetPel(hps, &pos);
        }
        if((winstyle & SS_LEFTFRAME) && (winstyle & SS_BOTTOMFRAME))
        { pos.x = wsize.xLeft - 1; pos.y = wsize.yBottom - 1;
          GpiSetPel(hps, &pos);
        }
      }
    }
}





// Wy˜wietlenie tekstu wewn¥trz okna
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna
//   hps       - [parametr] uchwyt przestrzeni roboczej
//   size      - [parametr] obszar w obr©bie kt¢rego ma by† rysowany tekst
//   stat      - [parametr] wska«nik do struktury kontrolnej okna
//
static VOID StatDrawText(HWND hwnd, HPS hps, RECTL *size, STATUS *stat)
{ AREABUNDLE abn;             // Atrybuty wypeˆnieä
  CHARBUNDLE cbn;             // Atrybuty tekstu
  POINTL pos;                 // wsp¢ˆrz©dne og¢lnego przeznaczenia
  POINTL txt[TXTBOX_COUNT];   // Rzmiary ˆaäcucha tekstowego
  RECTL  box;                 // Obszar w kt¢rym odbywa si© rysowanie
  LONG   textw;               // Szeroko˜† tekstu

  // Sprawdzenie czy mo¾na rysowa† tekst wewn¥trz okna
  if((size->xRight <= size->xLeft) || (size->yTop <= size->yBottom)) return;
  box = *size;

  // Ustalenie kolor¢w tˆa i tekstu w zale¾no˜ci od stanu okna
  if((WinIsWindowEnabled(hwnd) == TRUE) && (stat->ulHilite == TRUE))
  { // Okno jest aktywne
    GpiSetColor(hps, STA_ACTIVETEXT);
    // Kolor tˆa tekstu
    cbn.lBackColor = STA_ACTIVEBKGND;
    GpiSetAttrs(hps, PRIM_CHAR, CBB_BACK_COLOR, 0L, &cbn);
    // Kolor wypeˆnienia
    abn.lBackColor = STA_ACTIVEBKGND;
    GpiSetAttrs(hps, PRIM_AREA, ABB_BACK_COLOR, 0L, &abn);
  }
  else
  { // Okno jest nieaktywne
    GpiSetColor(hps, STA_INACTVTEXT);
    // Kolor tˆa tekstu
    cbn.lBackColor = STA_INACTVBKGND;
    GpiSetAttrs(hps, PRIM_CHAR, CBB_BACK_COLOR, 0L, &cbn);
    // Kolor wypeˆnienia
    abn.lBackColor = STA_INACTVBKGND;
    GpiSetAttrs(hps, PRIM_AREA, ABB_BACK_COLOR, 0L, &abn);
  }

  // Odczyt rozmiar¢w tekstu
  GpiQueryTextBox(hps, strlen(stat->szWintext), stat->szWintext, TXTBOX_COUNT, txt);
  // Sprawdzenie czy tekst nie jest odwr¢cony
  if(txt[TXTBOX_BOTTOMLEFT].x > txt[TXTBOX_BOTTOMRIGHT].x)
  { // Odwr¢cenie tekstu do wˆa˜ciwej pozycji
    cbn.ptlAngle.x = -1;
    cbn.ptlAngle.y = 0;
    GpiSetAttrs(hps, PRIM_CHAR, CBB_ANGLE , 0L, &cbn);
  }

  if(stat->szWintext != NULL)
  { // Wy˜wietlenie tekstu
    // Obliczenie pocz¥tkowej pozycji pionowej
    pos.y  = (box.yTop + box.yBottom - stat->fm.lMaxBaselineExt) >> 1;
    pos.y += stat->fm.lMaxDescender;

    // Obliczenie pocz¥tkowej pozycji poziomej
    switch(stat->ulWinstyle & 0x60)
    { case SS_LEFT:
        pos.x = box.xLeft + (stat->fm.lMaxCharInc >> 2); break;

      case SS_CENTER:
        pos.x = (box.xRight + box.xLeft - stat->ulTextWidth) >> 1; break;

      case SS_RIGHT:
        pos.x = box.xRight - (stat->fm.lMaxCharInc >> 2) - stat->ulTextWidth; break;

      case SS_AUTOSHIFT:
        // Obliczenie caˆkowitej szeroko˜ci tekstu
        textw = stat->ulTextWidth + ((stat->fm.lMaxCharInc & 0xFFFFFFFC) >> 1);
        if(textw < (box.xRight - box.xLeft))
          pos.x = box.xLeft + (stat->fm.lMaxCharInc >> 2);
        else
          pos.x = box.xRight - (stat->fm.lMaxCharInc >> 2) - stat->ulTextWidth;
    }

    // Korekta wsp¢ˆrz©dnych obszaru ograniczaj¥cego
    box.xRight --; box.yTop --;

    if((stat->ulWinstyle & SS_OWNERDRAW) && (stat->bForceRedraw == TRUE))
    { // Wy˜wietlenie tekstu bez tˆa
      GpiCharStringPosAt(hps, &pos, &box, CHS_CLIP,
                         strlen(stat->szWintext), stat->szWintext, NULL);
    }
    else
    { // Wy˜wietlenie tekstu z tˆem
      GpiCharStringPosAt(hps, &pos, &box, CHS_CLIP | CHS_OPAQUE,
                         strlen(stat->szWintext), stat->szWintext, NULL);
    }

  }
  else
    if(!(stat->ulWinstyle & SS_OWNERDRAW) || (stat->bForceRedraw == FALSE))
    { // Wykasowanie tˆa
      pos.x = box.xLeft;  pos.y = box.yBottom;
      GpiMove(hps, &pos);
      pos.x = box.xRight; pos.y = box.yTop;
      GpiBox(hps, DRO_FILL, &pos, 0, 0);
    }
}





// Rysowanie przycisku skalowania, wy˜wietlanego zwykle w prawym dolnym
// rogu okien.
//
// Parametry:
//   hps       - [parametr] uchwyt presentation space
//   size      - [parametr] wymiary okna
//   stat      - [parametr] wska«nik do struktur kontrolnych okna
//
static VOID StatDrawWinstyle(HPS hps, RECTL *size, STATUS *stat)
{ POINTL xpos;      // Wsp¢ˆrz©dne przy dolnej kraw©dzi
  POINTL ypos;      // Wsp¢ˆrz©dne przy prawej kraw©dzi
  LONG   i;         // Zmienne pomocnicze

  // Rysowanie tˆa w przypadku, gdy okno jest otwarte ze stylem SS_OWNERDRAW
  // i tˆo nie zostaˆo przerysowane podczas obsˆugi WM_DRAWITEM
  if((stat->bForceRedraw == FALSE) || !(stat->ulWinstyle & SS_OWNERDRAW))
  { GpiSetColor(hps, STA_ACTIVEBKGND);
    GpiMove(hps, (POINTL *)size);
    GpiBox(hps, DRO_FILL, ((POINTL *)size) + 1, 0, 0);
  }

  // Rysowanie cieniowanych belek
  GpiSetColor(hps, STA_FRAMEDARK);
  // Wspˆrz©dne pocz¥tkowe
  xpos.x = size->xLeft + 1;
  xpos.y = size->yBottom;
  ypos.x = size->xRight - 1;
  ypos.y = size->yTop - 2;
  for(i = 0; i < 3; ++i)
  { GpiMove(hps, &xpos); GpiLine(hps, &ypos);
    xpos.x ++; ypos.y --;
    GpiMove(hps, &xpos); GpiLine(hps, &ypos);
    xpos.x += 3; ypos.y -= 3;
  }

  // Rysowanie o˜wietlenia belek
  GpiSetColor(hps, STA_FRAMEHILITE);
  // Wspˆrz©dne pocz¥tkowe
  xpos.x = size->xLeft;
  xpos.y = size->yBottom;
  ypos.x = size->xRight- 1;
  ypos.y = size->yTop - 1;
  for(i = 0; i < 3; ++i)
  { GpiMove(hps, &xpos); GpiLine(hps, &ypos);
    xpos.x += 4; ypos.y -= 4;
  }
}





// Funkcja zapewnia dziaˆanie funkcjom:
// WinQueryWindowText, WinQueryWindowTextLength,
// wykonanym z uchwytem okna WC_STATUS
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna
//   wpr       - [parametr/rezultat] zwracane parametry okna
//
// Powr¢t:
//   0    - FALSE bˆ¥d odczytu
//   1    - TRUE  parametr umiesczono w strukturze WNDPARAMS
//
static LONG StatLoadWndParams(HWND hwnd, WNDPARAMS *wpr)
{ STATUS *stat;     // Wska«nik do struktur kontrolnych okna statusowego
  LONG    rc;       // Kod bˆ©du zwracany przez funkcje

  // Odczyt wska«nika do struktur kontrolnych okna statusowego
  stat = WinQueryWindowPtr(hwnd, 0L);
  if(stat == NULL) return(FALSE);
  rc = 0;

  // Wyb¢r odpowiedniego parametru do odczytu
  if(wpr->fsStatus & WPM_CCHTEXT)
  { // Odczyt dˆugo˜ci tekstu
    // Sprawdzenie czy jest tekst
    if(stat->szWintext == NULL)
      wpr->cchText = 0;
    else wpr->cchText = strlen(stat->szWintext);
    rc = 1;
  }

  if(wpr->fsStatus & WPM_TEXT)
  { // Odczyt tekstu
    if(stat->szWintext != NULL)
    { strcpy(wpr->pszText, stat->szWintext);
      rc = 1;
    }
    else rc = 0;
  }

  // Dane class specific niedost©pne, bo ich nie ma
  return(rc);
}





// Funkcja przetwarza komunikat WM_HITTEST, dzi©ki czemu komunikaty myszy
// mog¥ by† przekazywane do procedury wˆa˜ciciela, wtedy gdy
// u¾yto stylu SS_MOUSETRANSPARENT.
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna statusu
//
// Powr¢t:
//   HT_ERROR       - okno jest zablokowane (disabled)
//   HT_TRANSPARENT - u¾yto stylu SS_MOUSETRANSPARENT
//   HT_NORMAL      - w przeciwnym wypadku
//
static MRESULT StatProcessMouse(HWND hwnd)
{ STATUS   *stat;        // Wsjka«nik do struktur kontrolnych okna statusowego

  // Sprawdzenie czy okno jest zablokowane
  if(WinIsWindowEnabled(hwnd) == FALSE)
    return(MRFROMLONG(HT_ERROR));

  // Odczyt wska«nika do struktur kontrolnych okna statusowego
  stat = WinQueryWindowPtr(hwnd, 0L);
  if(stat == NULL) return(MRFROMLONG(HT_NORMAL));

  // Sprawdzenie czy okno ma by† przezroczyste dla myszy
  if(stat->ulWinstyle & SS_MOUSETRANSPARENT)
    return(MRFROMLONG(HT_TRANSPARENT));
  return(MRFROMLONG(HT_NORMAL));
}




// Przerysowanie okna statusu
//
// Parametry:
//   hwnd      - [parametr] uchwyt przerysowywanego okna
//
static VOID StatRedraw(HWND hwnd)
{ OWNERBACK bkgnd;       // Struktura umo¾liwiaj¥ca wˆa˜cicielowi przerysowanie tˆa
  STATUS   *stat;        // Wsjka«nik do struktur kontrolnych okna statusowego
  RECTL     wsize;       // Aktualne rozmiary okna
  RECTL     inter;       // Obszar u¾yteczny wewn¥trz obramowania
  HWND      owner;       // Uchwyt wˆa˜ciciela
  LONG      winid;       // Identyfikator okna hwnd
  HPS       hps;         // Uchwyt presentation space okna


  // Odczyt wska«nika do struktur kontrolnych okna statusowego
  stat = WinQueryWindowPtr(hwnd, 0L);
  if(stat == NULL) return;

  // Odczyt wymiar¢w okna, inicjacja zmiennych pomocniczych
  WinQueryWindowRect(hwnd, &wsize);
  // Obliczenie pozycji obszaru wewn¥trz obramowania
  inter = wsize;
  StatCalcInterior(&inter, stat->ulWinstyle);

  // Rozpocz©cie przerysowywania
  hps = WinBeginPaint(hwnd, NULLHANDLE, NULL);
  // Tworzenie logicznej tablicy kolor¢w
  GpiCreateLogColorTable(hps, 0L, LCOLF_CONSECRGB, 0, STA_MAXCOLOR, (LONG *)stat->ulColor);

  // Wysˆanie komunikatu do wˆa˜ciciela, celem przerysowania tˆa
  if(stat->ulWinstyle & SS_OWNERDRAW)
  { // Odczyt uchwytu wˆa˜ciciela
    owner = WinQueryWindow(hwnd, QW_OWNER);
    if(owner != NULLHANDLE)
    { // Odczyt identyfikatora
      winid = WinQueryWindowUShort(hwnd, QWS_ID);

      // Inicjacja struktury steruj¥cej przerysowywaniem
      bkgnd.hwnd       = hwnd;
      bkgnd.hps        = hps;
      bkgnd.bound      = inter;
      bkgnd.refpoint.x = 0;
      bkgnd.refpoint.y = 0;
      // Wysˆanie komunikatu z ¾¥daniem przerysowania tˆa
      stat->bForceRedraw = (BOOL)WinSendMsg(owner, WM_DRAWITEM, MPFROMSHORT(winid), MPFROMP(&bkgnd));
    }
  }

  if(stat->ulWinstyle & SS_WINSTYLE)
  { // Rysowanie przycisku skalowania okna
    StatDrawWinstyle(hps, &wsize, stat);
  }
  else
  { // Rysowanie fragment¢w obramowania (zale¾nie od stylu okna)
    StatDrawFrame(hps, &wsize, stat->ulWinstyle);
    // Rysowanie tekstu (zale¾nie od stylu okna)
    StatDrawText(hwnd, hps, &inter, stat);
  }

  // Zakoäczenie przerysowywania
  WinEndPaint(hps);
}





// Aktualizacja wybranego parametru prezentacji
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna
//   ppm       - [parametr] identyfikator parametru
//
static VOID  StatSetPresParam(HWND hwnd, LONG ppm)
{ STATUS *stat;     // Wska«nik do struktur kontrolnych okna
  RECTL   wsize;    // Aktualny rozmiar okna

  // Odczyt adresu struktur kontrolnych okna
  stat = WinQueryWindowPtr(hwnd, 0L);
  if(stat == NULL) return;

  // Sprawdzenie czy zmianie ulegˆa czcionka
  // lub kilka parametr¢w na raz - ppm = 0
  if((ppm == PP_FONTNAMESIZE) || (ppm == 0))
  { // Auaktualnienie struktur okna
    StatStoreFont(hwnd, stat);

    // Wymuszenie przeskalowania lub przerysowania
    if(stat->ulWinstyle & (SS_XAUTOSCALE | SS_YAUTOSCALE))
    { // Odczyt aktualnych rozmiar¢w okna
      WinQueryWindowRect(hwnd, &wsize);
      // Wymuszenie skalowania po zmianie czcionki
      WinSetWindowPos(hwnd, HWND_TOP, 0, 0,
                      wsize.xRight - wsize.xLeft,
                      wsize.yTop   - wsize.yBottom,
                      SWP_SIZE);
    }
    else
      // Wymuszenie przerysowania
      WinInvalidateRect(hwnd, NULL, FALSE);
  }

  // Sprawdzenie czy zmianie ulegˆy kolory
  // lub kilka parametr¢w na raz - ppm = 0
  if((ppm != PP_FONTNAMESIZE) || (ppm == 0))
  { // Zmianie ulegˆy kolory
    PpmQueryPresColors(hwnd, STA_MAXCOLOR, PresParams, (LONG *)stat->ulColor);
    // Wymuszenie przerysowania
    WinInvalidateRect(hwnd, NULL, FALSE);
  }

  // Sprawdzenie czy komunikat WM_PRESPARAMCHANGED zostaˆ wysˆany
  // przez inny w¥tek - systemow¥ palet© czcionek lub kolor¢w
  if((ppm == PP_FONTNAMESIZE) || (ppm == PP_BACKGROUNDCOLOR) || (ppm == PP_FOREGROUNDCOLOR))
  { HWND owner;     // Uchwyt okna wˆa˜ciciela
    LONG winid;     // Identyfikator bie¾¥cego okna
    HAB  hab;       // Uchwyt anchor block

    // Odczyt uchwytu anchor block
    hab = WinQueryAnchorBlock(hwnd);
    // Sprawdzenie czy zmiana parametru jest inicjowana przez inny w¥tek
    if(WinInSendMsg(hab) == TRUE)
    { // Sprawdzenie czy parametr istnieje w obr©bie okna, czy te¾ jest dziedziczony
      if(WinQueryPresParam(hwnd, ppm, 0, NULL, sizeof(ULONG), &winid, QPF_NOINHERIT))
      { // Odczyt uchwytu wˆa˜ciciela
        owner = WinQueryWindow(hwnd, QW_OWNER);
        // Odczyt identyfikatora okna
        winid = WinQueryWindowUShort(hwnd, QWS_ID);
        // Wstawienie potwierdzenia do wˆa˜ciciela okna
        if(owner != NULLHANDLE)
          WinPostMsg(owner, WM_CONTROL,
                     MPFROM2SHORT(winid, SMN_PRESPARAMCHANGED),
                     MPFROMLONG(ppm));

      }
    }
  }
}





// Ustawienie nowego tekstu wewn¥trz okna
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna
//   text      - [parametr] zapisywany tekst
//
static VOID StatSetText(HWND hwnd, WNDPARAMS *wpr)
{ STATUS *stat;
  RECTL   wsize;

  // Odczyt adresu struktur kontrolnych okna
  stat = WinQueryWindowPtr(hwnd, 0L);
  if(stat == NULL) return;
  // Odczyt aktualnych rozmiar¢w okna
  WinQueryWindowRect(hwnd, &wsize);

  if(wpr->fsStatus & WPM_TEXT)
  { // Zapami©tanie tekstu
    StatStoreWindowText(hwnd, stat, wpr->pszText);

    // Przeskalowanie okna w trybie AUTOSCALE
    if(stat->ulWinstyle & (SS_XAUTOSCALE | SS_YAUTOSCALE))
    { WinSetWindowPos(hwnd, HWND_TOP, 0, 0,
                      wsize.xRight - wsize.xLeft,
                      wsize.yTop   - wsize.yBottom,
                      SWP_SIZE);
    }
    // Przerysowanie okna
    WinInvalidateRect(hwnd, NULL, FALSE);
  }
}





// Zapami©tanie nazwy i parametr¢w czcionki odczytanej
// z presentation parameters, obliczenie i zapami©tanie
// struktury FONTMETRICS
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna
//   stat      - [parametr/rezultat] wska«nik do struktur kontrolnych okna
//
static VOID StatStoreFont(HWND hwnd, STATUS *stat)
{ POINTL txt[TXTBOX_COUNT];   // Rozmiary tekstu
  HPS    hps;                 // Uchwyt pomocniczej presentation space
  LONG   rc;                  // Kody bˆ©d¢w zwracane przez funkcje systemowe

  // Utworzenie pomocniczej presentation space
  hps = WinGetPS(hwnd);
  // Odczyt parametr¢w domy˜lnej czcionki okna
  GpiQueryFontMetrics(hps, sizeof(FONTMETRICS), &stat->fm);

  // Pomiar dˆugo˜ci tekstu (je˜li istnieje)
  if(stat->szWintext != NULL)
  { GpiQueryTextBox(hps, strlen(stat->szWintext), stat->szWintext, TXTBOX_COUNT, txt);
    rc = txt[TXTBOX_BOTTOMRIGHT].x - txt[TXTBOX_BOTTOMLEFT].x;
    if(rc < 0) rc = -rc;
    stat->ulTextWidth = rc;
  }

  // Zwolnienie pomocniczej presentation space
  hps = WinReleasePS(hps);
}





// Zapami©tanie tekstu w wewn©trznych strukturach okna, odczyt
// dˆugo˜ci tekstu w pikselach, dla czcionki u¾ytej w obr©bie okna
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna
//   stat      - [parametr/rezultat] struktury kontrolne okna
//   text      - [parametr] wska«nik do zapamietywanego tekstu
//
static VOID StatStoreWindowText(HWND hwnd, STATUS *stat, PSZ text)
{ POINTL txt[TXTBOX_COUNT];   // Rozmiary tekstu odczytywane przez GpiQueryTextBox
  HPS hps;                    // Uchwyt pomocniczej presentation space
  PSZ tmp;                    // Wska«nik pomocniczy


  // Zapami©tanie tekstu wy˜wietlanego w oknie
  if(text != NULL)
  { if(stat->szWintext == NULL)
      stat->szWintext = malloc(strlen(text) + 1);
    else
    { tmp = realloc(stat->szWintext, strlen(text) + 1);
      if(tmp == NULL)
      { free(stat->szWintext);
        stat->szWintext = NULL;
      }
      else stat->szWintext = tmp;
    }
    if(stat->szWintext != NULL)
      strcpy(stat->szWintext, text);
  }
  else
   if(stat->szWintext != NULL)
   { free(stat->szWintext);
     stat->szWintext = NULL;
   }

  // Obliczenie dˆugo˜ci tekstu wyra¾onej w pikselach
  hps = WinGetPS(hwnd);

  // Obliczenie i zapami©tanie dˆugo˜ci tekstu w pikselach
  GpiQueryTextBox(hps, strlen(stat->szWintext), stat->szWintext, TXTBOX_COUNT, txt);
  stat->ulTextWidth = txt[TXTBOX_BOTTOMRIGHT].x - txt[TXTBOX_BOTTOMLEFT].x;

  // Zwolnienie pomocniczej presentation space
  WinReleasePS(hps);
}





// Interpretacja komunikat¢w steruj¥cych oknem
//
static LONG StatUserCommand(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{ STATUS *stat;     // Wska«nik do struktur kontrolnych okna
  LONG border;      // Grubo˜† obramowania
  LONG rc;          // Kod zwracany przez funkcj©

  // Odczyt adresu struktur kontrolnych okna
  stat = WinQueryWindowPtr(hwnd, 0L);
  if(stat == NULL) return(0);

  rc = 0;
  switch(msg)
  { case SM_QUERYMAXCHARINC:   // Odczyt max. szeroko˜ci znaku - lAveCharWidth
      return(stat->fm.lMaxCharInc);

    case SM_QUERYTEXTHEIGHT:   // Odczyt wysoko˜ci tekstu - lMaxBaselineExt
      return(stat->fm.lMaxBaselineExt);

    case SM_QUERYTEXTWIDTH:    // Odczyt szeroko˜ci tekstu
      return(stat->ulTextWidth);

    case SM_QUERYHEIGHT:       // Odczyt wysoko˜ci okna
      if(stat->ulWinstyle & SS_WINSTYLE)
      { rc = CY_WINSTYLE;
      }
      else
      { rc = stat->fm.lMaxBaselineExt;
        border = stat->ulWinstyle & SS_THINFRAME ? 1 : 2;
        if(stat->ulWinstyle & SS_TOPFRAME)    rc += border;
        if(stat->ulWinstyle & SS_BOTTOMFRAME) rc += border;
      }
      break;

    case SM_QUERYHILITE:       // Odczyt aktywno˜ci okna
      return(stat->ulHilite);

    case SM_QUERYWIDTH:        // Odczyt szeroko˜ci okna
      if(stat->ulWinstyle & SS_WINSTYLE)
      { rc = CX_WINSTYLE;
      }
      else
      { rc = stat->ulTextWidth + ((stat->fm.lMaxCharInc & 0xFFFFFFFC) >> 1);
        border = stat->ulWinstyle & SS_THINFRAME ? 1 : 2;
        if(stat->ulWinstyle & SS_LEFTFRAME)  rc += border;
        if(stat->ulWinstyle & SS_RIGHTFRAME) rc += border;
      }
      break;

    case SM_SETHILITE:         // Aktywacja / dezaktywacja okna
      if(LONGFROMMP(mp1) == TRUE)
      { // ½¥danie aktywacji okna
        if(stat->ulHilite == TRUE) return(0);
        // Aktywacja okna
        stat->ulHilite = TRUE;
        WinInvalidateRect(hwnd, NULL, FALSE);
      }
      else
      { // ½¥danie dezaktywacji okna
        if(stat->ulHilite == FALSE) return(0);
        // Dezaktywacja okna
        stat->ulHilite = FALSE;
        WinInvalidateRect(hwnd, NULL, FALSE);
      }
      return(0);
  }

  return(rc);
}

/*
 * $Log: status.c $
 * Revision 2.2  1999/04/18 22:54:59  Wojciech_Gazda
 * Nieudany (na razie) styl SS_WINSTYLE, poprawka komunikatu SM_QUERYWIDTH
 *
 * Revision 2.1  1999/03/20 20:09:09  Wojciech_Gazda
 * Face-lifting - dostosowanie wygl¥du do OS/2 Warp 3
 *
 * Revision 1.9  1999/02/28 22:16:49  Wojciech_Gazda
 * Dodano styl SS_MOUSETRANSPARENT i reakcj© na zmian© kolor¢w systemowych
 *
 * Revision 1.8  1999/02/26 23:14:54  Wojciech_Gazda
 * Dodanie komunikat¢w aktywuj¥cych i dezaktywuj¥cych okno
 *
 * Revision 1.7  1999/02/21 15:35:03  Wojciech_Gazda
 * Usuni©cie r©cznej obsˆugi czcionek - system robi to sam
 *
 * Revision 1.6  1999/02/21 15:30:00  Wojciech_Gazda
 * Potwierdzenie SMN_PRESPARAMCHANGED generowane tylko po upuszczeniu
 * czego˜ z palety systemowej, wstawiane a nie wysyˆane do kolejki wˆa˜ciciela
 *
 * Revision 1.5  1999/02/20 19:44:30  Wojciech_Gazda
 * Zmiana struktury OWNERBACK
 *
 * Revision 1.4  1999/02/11 12:41:53  Wojciech_Gazda
 * Dodanie stylu SS_AUTOSHIFT
 *
 * Revision 1.3  1999/02/03 22:24:18  Wojciech_Gazda
 * Obsˆuga presentation parameters - wyjazd do osobnego pliku
 * dodano obsˆug© WinQueryWindowText i WinQueryWindowTextLength
 *
 * Revision 1.2  1999/02/03 20:33:08  Wojciech_Gazda
 * Poprawiono SM_QUERYWIDTH - zwraca co trzeba
 *
 * Revision 1.1  1999/01/31 15:11:55  Wojciech_Gazda
 * Initial revision
 *
 */
