/* ==========================================================================
    Progress Bar (Custom Control) for OS/2
   ===========================================

    $RCSfile: progbar.c $
      Author: Przemyslaw_Dobrowolski <dobrawka@asua.org.pl>
       $Date: 1999/04/16 11:15:26 $
   $Revision: 1.2 $
      $State: Exp $
       Notes: Progress Bar (ProcentBar)

 Copyright ¸ 1999 Asua OS/2 Programmers Group (http://www.asua.org.pl)
============================================================================= */

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


#define INCL_DOSMEMMGR
#define INCL_WINWINDOWMGR
#define INCL_WINRECTANGLES
#define INCL_WINSYS
#define INCL_WINMESSAGEMGR
#define INCL_GPIPRIMITIVES
#define INCL_GPILOGCOLORTABLE
#define INCL_GPIREGIONS
#define INCL_GPILCIDS
#include <os2.h>
#include <string.h>
#include <stdio.h>

#define  __INTERNAL_USE__
#include "progbar.h"
#include "pparams.h"

PSZ szProgbarVersion = "$Revision: 1.2 $"; // Wersja kontrolki

// Deklaracje staˆych
#define DEF_STEPSIZE    15   // Domy˜lna szeroko˜†
#define MIN_STEPSIZE    5    // Minimalna szeroko˜† kwadracika (kroku)

#define PBA_FRAMELIGHT        0    // PP_BORDERLIGHTCOLOR        O˜wietlenie ramki
#define PBA_FRAMEDARK         1    // PP_BORDERDARKCOLOR         Cieä ramki
#define PBA_ACTIVEBK          2    // PP_BACKGROUNDCOLOR         Kolor tˆa aktywnej ramki
#define PBA_ACTIVEBAR         3    // PP_FOREGROUNDCOLOR         Kolor tekstu aktywnej ramki

// Liczba kolor¢w
#define PBA_MAXCOLOR          4

static PPTEMPLATE PresParams[PBA_MAXCOLOR] =
  {{ PP_BORDERLIGHTCOLOR,        0L,    // O˜wietlenie ramki
     0,                          0xFFFFFF,
   },
   { PP_BORDERDARKCOLOR,         0L,    // Cieä ramki
     0,                          0x808080,
   },
   { PP_BACKGROUNDCOLOR,         0L,    // Kolor tˆa aktywnej ramki
     0,                          0xCCCCCC,
   },
   { PP_FOREGROUNDCOLOR,         0L,    // Kolor wska«nika
     0,                          0x000080,
   }
  };



typedef struct _PROGBAR
{
  LONG  flStyle;        // Styl kontrolki
  SHORT sMaxRange;      // Maksymalna warto˜†
  SHORT sMinRange;      // Minimalna wartosc
  SHORT sCurPos;        // Warto˜† aktualna
  SHORT sStep;          // Krok (co ile ma zmienia† si© procent-bar)
  SHORT sVisStep;
  SHORT sVisStepWidth;
  SHORT sVisPos;
  SHORT sTotalSteps;        // Liczba wszystkich krok¢w
  ULONG ulColors[PBA_MAXCOLOR];         // Kolor wska«nika
} PROGBAR, *PPROGBAR;


// *** Definicja prototyp¢w
MRESULT EXPENTRY        ProgBarWndProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);

// ** Funkcje statyczne
//static MRESULT EXPENTRY ProgBar_WM_PRESPARAMCHANGED(HWND,ULONG,MPARAM,MPARAM);
static MRESULT EXPENTRY ProgBar_WM_CREATE(HWND,ULONG,MPARAM,MPARAM);
static MRESULT EXPENTRY ProgBar_WM_DESTROY(HWND,ULONG,MPARAM,MPARAM);
static MRESULT EXPENTRY ProgBar_WM_PAINT(HWND,ULONG,MPARAM,MPARAM);
static MRESULT EXPENTRY ProgBar_WM_SIZE(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static MRESULT EXPENTRY ProgBarOtherWndProc(HWND,ULONG,MPARAM,MPARAM);
static MRESULT EXPENTRY ProgBarSetRange (HWND hWnd,PPROGBAR pProgBar, SHORT sMinRange, SHORT sMaxRange);

static SHORT DrawStep(HWND hWnd,PPROGBAR pProgBar);
static VOID  RecalcPos(HWND hWnd, PPROGBAR pProgBar);
static VOID  DrawBorder(HWND hWnd,HPS hPS,BOOL bRaised);
static LONG  GuessTextColor(LONG ulBackColor, LONG ulColor);
static VOID DrawTilPos (HPS hPS, HWND hWnd, PPROGBAR pProgBar, SHORT iPos );

// Przygotowanie okna WC_PROGRESSBAR do pracy
//
// Parametry:
//   hab       - [parametr] uchwyt anchor block
//
// Powr¢t:
//   0    - poprawne wykonanie funkcji
//   1    - bˆ¥d rejestracji klasy okna
//
LONG ASUAAPI ProgbarRegister(HAB hab)
{ ULONG winstyle;   // Domy˜lny styl okna
  LONG  rc;         // Kody bˆ©d¢w zwracane przez funkcje systemowe

  // Ustalenie domy˜lnego stylu okna WC_PROGRESSBAR
  winstyle = CS_PARENTCLIP | CS_SYNCPAINT | CS_SIZEREDRAW;

  // Rejestracja nowej klasy okna - samoskaluj¥ce si© okno statusowe
  rc = WinRegisterClass(hab, WC_PROGRESSBAR, (PFNWP)ProgBarWndProc, winstyle, sizeof(VOID *));
  if(!rc) return(1);
  return(0);
}

// * Obsˆuga komunikatu WM_DESTROY
static MRESULT EXPENTRY ProgBar_WM_DESTROY(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  PPROGBAR pProgBar;

  // pobieramy nasz¥ struktur© po raz ostatni ;)
  pProgBar = (PPROGBAR)WinQueryWindowPtr(hWnd, 0);

  DosFreeMem((PVOID)pProgBar);

  return (0);
}

// *
// * Gˆ¢wna procedura steruj¥ca kontrolka WC_PROGRESSBAR
// *
MRESULT EXPENTRY ProgBarWndProc(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch (msg)
  {
    //
    // *** WIADOMOSCI STANDARDOWE
    //
    case WM_CREATE:     // Tworzenie okna
      return (ProgBar_WM_CREATE(hWnd,msg,mp1,mp2));

    case WM_DESTROY:    // Usuwanie okna
      return (ProgBar_WM_DESTROY(hWnd,msg,mp1,mp2));

    case WM_ENABLE:     // Zmiana stanu okna - wymuszenie przerysowania
      WinInvalidateRect(hWnd, NULL, FALSE);
      break;
/*
    case WM_SYSCOLORCHANGE:        // Zmianie ulegˆy kolory systemowe
      // Zmianie ulegˆy kolory
      { PPROGBAR pProgBar;     // Adres struktury kontrolnej okna

        // Odczyt adresu struktur kontrolnych
        pProgBar = WinQueryWindowPtr(hWnd, 0L);
        if (!pProgBar) break;
        // Uaktualnienie kolor¢w
        PpmQueryPresColors(hWnd, PBA_MAXCOLOR, PresParams, (LONG *)pProgBar->ulColors);
      }
      break;
*/
    case WM_PAINT:
      return (ProgBar_WM_PAINT(hWnd,msg,mp1,mp2));

//    case WM_PRESPARAMCHANGED:      // Obsˆuga WinSetPesParam
//      ProgBar_WM_PRESPARAMCHANGED(hwnd, LONGFROMMP(mp1));
//      break;

    case WM_SIZE:
      return (ProgBar_WM_SIZE(hWnd,msg,mp1,mp2));
   }

  // Interpretacja komunikat¢w steruj¥cych
  if((msg >= PROG_FIRST_MSG) && (msg <= PROG_LAST_MSG))
      return (ProgBarOtherWndProc(hWnd,msg,mp1,mp2));

  // Przekazanie nieprzetworzonych komunikat¢w do standardowej procedury okna
  return(WinDefWindowProc(hWnd, msg, mp1, mp2));
}

// *
// * Procedura obsluguj¥ca komunikat WM_CREATE
// *
static MRESULT EXPENTRY ProgBar_WM_CREATE(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  PPROGBAR       pProgBar;
  PCREATESTRUCT  pCreateStr;  // Wska«nik tworzenia okna

  // alokacja potrzebnej pami©ci
  DosAllocMem((PPVOID)(PVOID)&pProgBar, sizeof(PROGBAR), PAG_READ | PAG_WRITE | PAG_COMMIT);

  // nagrynie adresu wska«nika
  WinSetWindowPtr(hWnd, 0, (PVOID)pProgBar);

  // Pobranie informacji o oknie
  pCreateStr =  (PCREATESTRUCT)PVOIDFROMMP(mp2);

  // Pobranie Presentation Parameters
  PpmStorePresParams(hWnd, (PRESPARAMS *)pCreateStr->pPresParams, PresParams, PBA_MAXCOLOR);

  // Zapami©tanie kolor¢w pobranych z presentation parameters
  PpmQueryPresColors(hWnd, PBA_MAXCOLOR, PresParams, (LONG *)pProgBar->ulColors);

  // Ustawienie warto˜ci domy˜lnych
  pProgBar->flStyle    = pCreateStr->flStyle;
  pProgBar->sStep      =  10;
  pProgBar->sMaxRange  = 100;
  pProgBar->sMinRange  =   0;
  pProgBar->sCurPos    =   0;

  return (0);
}

static MRESULT EXPENTRY ProgBar_WM_SIZE(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  PPROGBAR pProgBar;
  RECTL rcl;

  pProgBar = (PPROGBAR)WinQueryWindowPtr(hWnd, 0);

  WinQueryWindowRect(hWnd, &rcl);
  RecalcPos(hWnd,pProgBar);
  WinInvalidateRect ( hWnd, (HWND) NULL, TRUE );
  return (0);
}


// *
// * Procedura obsluguj¥ca komunikat WM_PAINT
// *
static MRESULT EXPENTRY ProgBar_WM_PAINT(HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  PPROGBAR  pProgBar;
  RECTL         rcl;
  HPS           hPS;

  pProgBar = (PPROGBAR)WinQueryWindowPtr(hWnd, 0);

  hPS = WinBeginPaint(hWnd, (HPS)NULL, (PRECTL)NULL);

  WinQueryWindowRect(hWnd, &rcl);

  // Tworzenie logicznej tablicy kolor¢w
  GpiCreateLogColorTable(hPS, 0L, LCOLF_CONSECRGB, 0, PBA_MAXCOLOR, (LONG *)pProgBar->ulColors);

  // Rysowanie ramki
  if ((pProgBar->flStyle & PBS_RAISED) || (pProgBar->flStyle & PBS_DEPRESSED))
    DrawBorder (hWnd, hPS, pProgBar->flStyle & PBS_RAISED ? TRUE : FALSE);

  // Rysowanie ˜rodka kontrolki
  DrawTilPos (hPS, hWnd, pProgBar, pProgBar->sCurPos);
  WinEndPaint(hPS);
  return (0);
}

// *
// * Procedura obsluguj¥ca dodatkowe (niestandardowe) komunikaty
// *
static MRESULT EXPENTRY ProgBarOtherWndProc(HWND hWnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
  PPROGBAR pProgBar;

  pProgBar = (PPROGBAR)WinQueryWindowPtr(hWnd, 0);

  switch (msg)
  {
    case PBM_DELTAPOS:
    {
      SHORT OldPos = pProgBar->sCurPos;

      pProgBar->sCurPos += LONGFROMMP(mp1);
      WinInvalidateRect ( hWnd, (HWND) NULL, TRUE );

      return MRFROMSHORT(OldPos);
    }

    case PBM_SETPOS:
    {
      SHORT OldPos = pProgBar->sCurPos;

      if (LONGFROMMP(mp1)!=pProgBar->sCurPos)
      {
        pProgBar->sCurPos = LONGFROMMP(mp1);
        WinInvalidateRect ( hWnd, (HWND) NULL, TRUE );
      }
      return MRFROMSHORT(OldPos);
    }

    case PBM_SETRANGE:
      return ProgBarSetRange ( hWnd, pProgBar, SHORT2FROMMP(mp2), SHORT1FROMMP(mp2));

    case PBM_SETSTEP:
    {
       SHORT OldStep = pProgBar->sStep;
       pProgBar->sStep = LONGFROMMP(mp1);
       RecalcPos(hWnd,pProgBar);
       WinInvalidateRect ( hWnd, (HWND) NULL, TRUE );
       return MRFROMSHORT(OldStep);
    }

    case PBM_STEPIT:
      DrawStep(hWnd, pProgBar);
      break;

    case PBM_GETRANGE:
      if (mp2)
      {
        ((PPBRANGE)mp2)->sLow  = pProgBar->sMinRange;
        ((PPBRANGE)mp2)->sHigh = pProgBar->sMaxRange;
      }
      return MRFROMSHORT((mp1) ? pProgBar->sMinRange : pProgBar->sMaxRange);

    case PBM_GETPOS:
      return(MRFROMLONG(pProgBar->sCurPos));

    case PBM_SETBARCOLOR:
      break;
    case PBM_SETBKCOLOR:
      break;
  }
  return (0);
}


// *
// * Procedura rysuj¥ca ramk© kontrolki
// *
static VOID DrawBorder(HWND hWnd,HPS hPS,BOOL bRaised)
{
  RECTL    rcl;
  POINTL   ptl;
  HAB      hAB;

  WinQueryWindowRect ( hWnd, &rcl );
  hAB=WinQueryAnchorBlock(hWnd);
  GpiSetColor(hPS,bRaised ? PBA_FRAMELIGHT : PBA_FRAMEDARK);
  ptl.x=rcl.xLeft+1;
  ptl.y=rcl.yBottom+1;
  GpiMove(hPS,&ptl);
  ptl.x=rcl.xLeft+1;
  ptl.y=rcl.yTop;
  GpiLine(hPS,&ptl);
  ptl.x=rcl.xRight;
  ptl.y=rcl.yTop;
  GpiLine(hPS,&ptl);
  GpiSetColor(hPS,bRaised ? PBA_FRAMEDARK : PBA_FRAMELIGHT);
  ptl.x=rcl.xRight;
  ptl.y=rcl.yBottom+1;
  GpiLine(hPS,&ptl);
  ptl.x=rcl.xLeft+1;
  ptl.y=rcl.yBottom+1;
  GpiLine(hPS,&ptl);
}

// *
// * Procedura rysuj¥ca procenty ;-)
// *
static VOID DrawTilPos (HPS hPS, HWND hWnd, PPROGBAR pProgBar, SHORT iPos )
{
  SHORT  i;
  SHORT  k;
  RECTL  rcl;

  if (pProgBar->flStyle & PBS_SMOOTH)
  {
    pProgBar->sCurPos = 0;
    pProgBar->sVisPos = 0;

    for ( i = pProgBar->sMinRange + pProgBar->sStep, k=i; i <= iPos; i += pProgBar->sStep, k += pProgBar->sStep )
      DrawStep (hWnd, pProgBar);
  }
  else
  {
    DrawStep (hWnd, pProgBar);

    // Tekst wy˜wietlany jest tylko wtedy gdy nie ma stylu SMOOTH
    if (pProgBar->flStyle & PBS_TEXT)
    { CHAR        szString[12];
      POINTL      pos;
      POINTL      txt[TXTBOX_COUNT]; // Rzmiary ˆaäcucha tekstowego
      CHARBUNDLE  cbn;               // Atrybuty tekstu
      FONTMETRICS fm;

      // Ustawmy PS tak aby tekst byˆ zawsze widoczny
      // Tworzenie logicznej tablicy kolor¢w
      GpiQueryFontMetrics(hPS, sizeof(FONTMETRICS), &fm);
      GpiCreateLogColorTable(hPS, 0L, LCOLF_CONSECRGB, 0, PBA_MAXCOLOR, (LONG *)pProgBar->ulColors);
      GpiSetColor(hPS,CLR_BLACK);
      GpiSetMix(hPS, FM_NOTXORSRC);
      sprintf(szString, "%d%%",pProgBar->sCurPos );

      // Gdy styl jest pionowy to test musimy odwr¢ci†
      if (pProgBar->flStyle & PBS_VERTICAL)
      {
        cbn.ptlAngle.x = 0;
        cbn.ptlAngle.y = 1;
        GpiSetAttrs(hPS, PRIM_CHAR, CBB_ANGLE , 0L, &cbn);
      }
      WinDrawText(hPS, strlen(szString), szString, &rcl, 0, GuessTextColor(PresParams[PBA_ACTIVEBK].ulRgbColor,PresParams[PBA_ACTIVEBAR].ulRgbColor), DT_TEXTATTRS |DT_VCENTER | DT_CENTER);
    }
  }
}

// Generalnie funkcja sprawdza czy kolor R lub G lub B
// je˜li jest za ciemny to zwraca biaˆy a je˜li za jasny to czarny
// a ¾aden z nich nie pasuje to funkcja zwr¢ci kolor kt¢ry b©dzie widoczny.
static LONG GuessTextColor(LONG ulBackColor, LONG ulColor)
{
  if ((ulBackColor-ulColor<0) || (ulBackColor-ulColor==0xffffff))
    return (ulColor-ulBackColor);

  return (ulBackColor-ulColor);
}

// *
// * Procedura przeliczaj¥ca kroki itp.
// *
static VOID RecalcPos (HWND hWnd, PPROGBAR pProgBar)
{
  RECTL  rcl;
  SHORT  sTemp;

  WinQueryWindowRect (hWnd,&rcl);

  if (pProgBar->flStyle & PBS_VERTICAL)
  {
    rcl.yBottom+=3;
    rcl.yTop-=3;
  }

  rcl.xLeft+=3;
  rcl.xRight-=3;


  pProgBar->sTotalSteps = ( pProgBar->sMaxRange - pProgBar->sMinRange ) / pProgBar->sStep -1;

  if (pProgBar->flStyle & PBS_VERTICAL)
    sTemp= (rcl.yTop - rcl.yBottom) / pProgBar->sTotalSteps;
  else
    sTemp=( rcl.xRight - rcl.xLeft) / pProgBar->sTotalSteps;

  pProgBar->sVisStepWidth = sTemp;
  pProgBar->sVisStep      = pProgBar->sStep;

  if ( pProgBar->sVisStepWidth < MIN_STEPSIZE )
  {
    pProgBar->sVisStepWidth = MIN_STEPSIZE;
    if (pProgBar->flStyle & PBS_VERTICAL)
      pProgBar->sVisStep = ( pProgBar->sMaxRange - pProgBar->sMinRange ) * pProgBar->sVisStepWidth / (rcl.yTop - rcl.yBottom);
    else
      pProgBar->sVisStep = ( pProgBar->sMaxRange - pProgBar->sMinRange ) * pProgBar->sVisStepWidth / ( rcl.xRight - rcl.xLeft );
  }

  if ( pProgBar->sVisStep <= 0 )
    pProgBar->sVisStep = 1;
}

// *
// * Procedura obsluguj¥ca niestandardowy komunikat PBM_SETRANGE
// *
static MRESULT EXPENTRY ProgBarSetRange (HWND hWnd,PPROGBAR pProgBar, SHORT sMinRange, SHORT sMaxRange)
{
  RECTL   rcl;
  LONG    OldRange;

  OldRange = MAKELONG (pProgBar->sMinRange, pProgBar->sMaxRange);

  WinQueryWindowRect ( hWnd, &rcl );

  if (pProgBar->flStyle & PBS_VERTICAL)
  {
    rcl.yBottom+=3;
    rcl.yTop-=3;
  }

  rcl.xLeft+=3;
  rcl.xRight-=3;

  pProgBar->sMinRange = sMinRange;
  pProgBar->sMaxRange = sMaxRange;
  pProgBar->sVisStepWidth = 0; /*( pProgBar->sMaxRange - pProgBar->sMinRange ) / pProgBar->sStep;*/

  return MRFROMLONG (OldRange);
}

// *
// * Procedura rysujaca kwadraciki
// *
static SHORT DrawStep (HWND hWnd,PPROGBAR pProgBar)
{
  SHORT k;
  SHORT inc;
  SHORT sMaxPos;
  RECTL rcl;
  RECTL rclBk;
  HPS   hPS;
  BOOL bRepaintAll = TRUE;
  SHORT sCurVisPos = pProgBar->sVisPos;

  if (pProgBar->sVisStepWidth <= 0)
    RecalcPos (hWnd, pProgBar);

  inc = pProgBar->sVisStepWidth;

  if ( inc > DEF_STEPSIZE )
    inc = DEF_STEPSIZE;

  WinQueryWindowRect ( hWnd, &rcl );
//  WinInflateRect (WinQueryAnchorBlock(hWnd), &rcl, -3, -3 );

  // Nie mo¾emy przekroczy† wymiar¢w
  if (pProgBar->sCurPos > pProgBar->sMaxRange)
    pProgBar->sCurPos=pProgBar->sMaxRange;

  if (pProgBar->flStyle & PBS_VERTICAL)
  {
    rcl.yTop-=3;
    rcl.yBottom+=4;
    rcl.xRight-=3;
    rcl.xLeft+=4;
    sMaxPos = rcl.yTop;
  }
  else
  {
    rcl.yTop-=2;
    rcl.yBottom+=3;
    rcl.xRight-=2;
    rcl.xLeft+=3;
    sMaxPos = rcl.xRight;
  }

  rclBk=rcl; // Zapami©tajmy caˆy prostok¥t bo zaczniemy go zaraz zmienia†

  hPS = WinGetPS (hWnd);

  // Tu to musi by†, ale nie pytajcie dlaczego.
  GpiCreateLogColorTable(hPS, 0L, LCOLF_CONSECRGB, 0, PBA_MAXCOLOR, (LONG *)pProgBar->ulColors);

  // je¾eli ma by† to kontrolka z krokami
  if (pProgBar->flStyle & PBS_SMOOTH)
  {
    if ( pProgBar->sVisStep > pProgBar->sStep )
    {
      SHORT curPos = pProgBar->sCurPos;

      curPos %= pProgBar->sVisStep;

      if ( curPos + pProgBar->sStep >= pProgBar->sVisStep )
      {
        if (pProgBar->flStyle & PBS_VERTICAL) // Gdy jest to wska«nik pionowy
        {
          rcl.yBottom += sCurVisPos;
          rcl.yTop = rcl.yBottom + inc-2;

          if ( rcl.yTop > sMaxPos )
            rcl.yTop = sMaxPos;
          // Obliczmy niewypeˆnione miejsce
          rclBk.yBottom = rcl.yTop;
        }
        else
        {
          rcl.xLeft += sCurVisPos;
          rcl.xRight = rcl.xLeft + inc-2;

          if (rcl.xRight > sMaxPos)
            rcl.xRight = sMaxPos;
          // Obliczmy niewypeˆnione miejsce
          rclBk.xLeft = rcl.xRight;
        }

        // Wypeˆniamy kolorem
        WinFillRect ( hPS, &rcl, PBA_ACTIVEBAR );

        // Musimy te¾ wypeˆni† tˆo, ale _TYLKO_ to kt¢re jest widoczne
        if ( (bRepaintAll) && (rclBk.xLeft!=0) )
          WinFillRect ( hPS, &rclBk, PBA_ACTIVEBK );

        pProgBar->sVisPos += pProgBar->sVisStepWidth;
      }
    }
    else
    {
      if (pProgBar->flStyle & PBS_VERTICAL)
      {
        rcl.yBottom += sCurVisPos - sCurVisPos % inc;
        rcl.yTop = rcl.yBottom + inc-2;
        if ( rcl.yTop > sMaxPos )
          rcl.yTop = sMaxPos;

        // Obliczmy niewypeˆnione miejsce
        rclBk.yBottom = rcl.yTop;
      }
      else
      {
        rcl.xLeft += sCurVisPos - sCurVisPos % inc;
        rcl.xRight = rcl.xLeft + inc-2;
        if ( rcl.xRight > sMaxPos )
          rcl.xRight = sMaxPos;
        // Obliczmy niewypeˆnione miejsce
        rclBk.xLeft = rcl.xRight;
      }
      k = sCurVisPos % inc + pProgBar->sVisStepWidth;
      pProgBar->sVisPos += pProgBar->sVisStepWidth;
      for (;;)
      {
        if ( k >= inc )
        {
          WinFillRect(hPS, &rcl,   PBA_ACTIVEBAR);

          if ((bRepaintAll)&&(rclBk.xLeft!=0)) WinFillRect(hPS, &rclBk, PBA_ACTIVEBK );

          k -= inc;

          if (pProgBar->flStyle & PBS_VERTICAL)
          {
            rcl.yBottom += inc;
            rcl.yTop += inc;

            if ( rcl.yTop > sMaxPos )
              rcl.yTop = sMaxPos;

            // Obliczmy niewypeˆnione miejsce
            rclBk.yBottom = rcl.yTop;
          }
          else
          {
            rcl.xLeft += inc;
            rcl.xRight += inc;

            if ( rcl.xRight > sMaxPos )
              rcl.xRight = sMaxPos;

            // Obliczmy niewypeˆnione miejsce
            rclBk.xLeft = rcl.xRight;
          }

        }
        else
        break;
      }
    }
    pProgBar->sCurPos += pProgBar->sStep;
  }
  else
  {
    if (pProgBar->sCurPos > pProgBar->sMaxRange)
      pProgBar->sCurPos=pProgBar->sMaxRange;

    if (pProgBar->flStyle & PBS_VERTICAL)
    {
      rcl.yTop=(pProgBar->sCurPos * rcl.yTop)/ pProgBar->sMaxRange;
      rclBk.yBottom=rcl.yTop;
    }
    else
    {
      rcl.xRight=(pProgBar->sCurPos * rcl.xRight)/ pProgBar->sMaxRange;
      rclBk.xLeft=rcl.xRight;
    }
    WinFillRect(hPS, &rcl  , PBA_ACTIVEBAR );
    if (rclBk.xLeft!=0)
      WinFillRect(hPS, &rclBk, PBA_ACTIVEBK );
  }
  WinReleasePS(hPS);

  return (0);
}

/* ==========================================================================
 *
 * $Log: progbar.c $
 * Revision 1.2  1999/04/16 11:15:26  Przemyslaw_Dobrowolski
 * Prawie nic pr¢cz scie¾ek
 *
 * Revision 1.1  1999/03/18 12:56:30  Przemyslaw_Dobrowolski
 * Initial revision
 *
 */