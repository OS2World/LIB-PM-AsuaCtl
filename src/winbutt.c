/* ==========================================================================
   Win Button (Custom Control)
   ===========================

    $RCSfile: WINBUTT.C $
      Author: Przemyslaw_Dobrowolski <dobrawka@asua.org.pl>
   some code: Wojciech Gazda <gazda@asua.org.pl>

       $Date: 1999/08/14 13:55:35 $
   $Revision: 2.1 $
      $State: Exp $
       Notes: Zast©pnik WC_BUTTON

 Copyright ¸ 1999-2001 Asua OS/2 Programmers Group (http://www.asua.org.pl)
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


#define INCL_GPILOGCOLORTABLE
#define INCL_WINMESSAGEMGR
#define INCL_WINRECTANGLES
#define INCL_GPIPRIMITIVES
#define INCL_WINWINDOWMGR
#define INCL_WINPOINTERS
#define INCL_WINFRAMEMGR
#define INCL_WINDIALOGS
#define INCL_WINBUTTONS
#define INCL_WINTIMER
#define INCL_GPILCIDS
#define INCL_WININPUT
#define INCL_WINSYS
#define INCL_DOSPROCESS

#include <os2.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define  __INTERNAL_USE__

#include "asuintl.h"

#include "winbutt.h"


#define BTN_BACKGROUND      0
#define BTN_FOREGROUND      1
#define BTN_DISABLEDBK      2
#define BTN_DISABLEDFG      3
#define BTN_DEFBORDER       4
#define BTN_LIGHTBORDER     5
#define BTN_DARKBORDER      6
#define BTN_TEXTFLAT        7

#define BTN_MAXCOLOR        8 // Count of max color definition
                              // This definition are usefull for 
                              // changing the system colors.

// Private states of button
#define WBS_UP         0x0001      // normal 
#define WBS_DOWN       0x0002      // clicked down
#define WBS_CAPTURE    0x0004      // button are captured

// WBS_CAPTURE is set, when the mouse button (or space) 
//  is keeping pushing
#define WBS_MOUSEIN    0x0010      // Mouse pointer are in the button

#define BTN_BORDERWIDTH  6         // witdth of border
#define BTN_BORDERHEIGHT 6         // height of border

const PSZ szButtonVersion = "$Revision: 2.1 $"; // Window version

// Window version
static CTLINFO G_ctlButtonVersion =
{ sizeof(CTLINFO),
  NULL,
  2, 0
};

/* Private button structure */
typedef struct _BTN                    
{
  LONG        id;                      // Window ID
  RECTL       rcl;                     // Button Rectangle
  ULONG       flState;                 // Button State
  ULONG       flStyle;                 // Button Style
  HBITMAP     hbm;                     // Default bitmap handle (when active)
  HBITMAP     hbmMask;                 // Handle Bitmap mask for default bitmap
  HBITMAP     hbmDisabled;             // Bitmap handle (when disabled)
  HBITMAP     hbmMaskDisabled;         // Handle Bitmap mask for disabled bitmap
  HBITMAP     hbmPressed;              // Uchwyt do bitmapy z wci˜ni©tym klawiszem
  HBITMAP     hbmMaskPressed;          // Uchwyt do maski z wci˜ni©tym klawiszem
  HBITMAP     hbmMouseIn;              // Uchwyt do bitmapy z myszk¥ na klawiszu
  HBITMAP     hbmMaskMouseIn;          // Uchwyt do maski z myszk¥ na klawiszem
  HPOINTER    hptrArrow;               // Uchwyt do systemowego wska«nika myszy
  ULONG       ulTextWidth;             // Dˆugo˜† tekstu
  ULONG       ulTextHeight;            // Wysoko˜† tekstu
  ULONG       bmpcx;                   // Dˆugo˜† bitmap (najwi©ksza z dw¢ch)
  ULONG       bmpcy;                   // Szeroko˜† bitmapy (najwi©ksza z dw¢ch)
  POINTL      ptlBmp;                  // Prostok¥t w kt¢rym rysujemy bitmape
  RECTL       rclText;                 // Prostok¥t w kt¢rym piszemy tekst
  FONTMETRICS fm;                      // Parametry bie¾¥cej czcionki
  BOOL        bFocused;                // Has control focus?
  ULONG       ulColors[BTN_MAXCOLOR];  // Color on button
  PSZ         szText;                  // Text on button
  WBTNCDATA   ctlData;                 // CTLDATA for our button
} BTN, *PBTN;

// Template for this window control
static PPTEMPLATE G_PresParams[BTN_MAXCOLOR] =
  {{ PP_BACKGROUNDCOLOR,         0L,
     SYSCLR_BUTTONMIDDLE,        0xCCCCCC,
   },
   { PP_FOREGROUNDCOLOR,         0L,
     SYSCLR_MENUTEXT,            0x000000,
   },
   { PP_DISABLEDBACKGROUNDCOLOR, 0L,
     SYSCLR_BUTTONMIDDLE,        0xCCCCCC,
   },
   { PP_DISABLEDFOREGROUNDCOLOR, 0L,
     SYSCLR_BUTTONDARK,          0x808080,
   },
   { PP_BORDERDEFAULTCOLOR,      0L,
     SYSCLR_BUTTONDEFAULT,       0x000000,
   },
   { PP_BORDERLIGHTCOLOR,        0L,
     SYSCLR_BUTTONLIGHT,         0xFFFFFF,
   },
   { PP_BORDERDARKCOLOR,         0L,
     SYSCLR_BUTTONDARK,          0x808080,
   },
   { PP_USER,                    0L,
     0,                          0x0000FF,
   },

 };

// *** Forward Declarations
// *********************************
static VOID    BtnDrawText(HWND hwnd, PBTN pBtn);
static LONG    BtnGetTextHeight(HWND hwnd, PSZ szText);
static LONG    BtnGetTextWidth(HWND hwnd, PSZ szText);
static VOID    BtnDrawBox(HPS hps,LONG xLeft,LONG yBottom,LONG xRight,LONG yTop);
static VOID    BtnTextFilter(HWND hwnd, PSZ szText);
static VOID    BtnStoreFont(HWND hwnd, PBTN pBtn);
static VOID    BtnSizeButton(PBTN pBtn, LONG x, LONG y, LONG cx, LONG cy);
static VOID    BtnStoreWindowText(HWND hwnd, PBTN  pBtn, PSZ text);
static VOID    BtnStoreWindowBitmaps(HWND hwnd, HMODULE hmod, ULONG idEnabled, ULONG idDisabled, ULONG idPressed, ULONG idMouseIn);
static VOID    BtnAutoscale(HWND hwnd, ULONG *pulHeight, ULONG *pulWidth);
static VOID    BtnQueryBitmaps(HPS hps,PBTN pBtn);
static VOID    BtnSetCtlData(HWND hwnd, PBTN pBtn, PVOID pCtlData);
static HBITMAP BtnCreateMonochrome(HWND hwnd, HBITMAP hbm);
static LONG    BtnChangeToMonochrome(BITMAPINFO2 *info2, VOID *bits, ULONG cbPalSize);

MRESULT	EXPENTRY BtnWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

MRESULT BtnOnPaint(HWND hwnd);

static MRESULT BtnOnSize             (HWND hwnd);
static MRESULT BtnOnCreate           (HWND hwnd, MPARAM mp1, MPARAM mp2);
static MRESULT BtnOnEnable           (HWND hwnd, MPARAM mp1);
static MRESULT Btn_WM_SETWINDOWPARAMS  (HWND hwnd, MPARAM mp1);
static MRESULT Btn_WM_QUERYWINDOWPARAMS(HWND hwnd, MPARAM mp1);
static MRESULT BtnOnFocusChange         (HWND hwnd, MPARAM mp2);
static MRESULT BtnOnDestroy          (HWND hwnd);
static MRESULT BtnOnChar(HWND hwnd, MPARAM mp1, MPARAM mp2);
static MRESULT BtnOnMouseMove        (HWND hwnd, PBTN pBtn, MPARAM mp1);
static MRESULT BtnMouseUp              (HWND hwnd, MPARAM mp1, BOOL flMouse);
static MRESULT BtnMouseDown            (HWND hwnd);
static MRESULT Btn_WM_MATCHMNEMONIC    (HWND hwnd, PBTN pBtn, MPARAM mp1);
static MRESULT BtnOnTimer            (HWND hwnd, PBTN pBtn);
static MRESULT BtnOnPresParamChanged (HWND hwnd, MPARAM mp1);
static MRESULT Btn_OtherProc           (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
static MRESULT BtnDblClick             (HWND hwnd, PBTN pBtn, POINTL ptl);

// 
// Function:
//           LONG BtnChangeToMonochrome(BITMAPINFO2, VOID, ULONG)
// 
// Description:
// This Function change color bitmap to monochrome. In indexed color mode
// change only description in pallette, for RGB bitmaps all points from bitmap.
//
// Parameters:
//   info2     - [parameter/result] header of bitmap with pallette.
//   bits      - [parameter/result] bits to convert
//
// Returns:
//   0                   - OK. No error.
//   1                   - pˆaszczyzny bitowe nie s¥ obsˆugiwane
//   2                   - invalid count of bytes per pixels.
//
// Note: Function based on informations and code from Wojciech Gazda
//
//
//
LONG BtnChangeToMonochrome(BITMAPINFO2 *info2, VOID *bits, ULONG cbPalSize)
{

// Helpful definitions
#define _getrgbpx(src) (*((ULONG *)(src)) & 0xFFFFFF)
#define _putrgbpx(src, rgb) (*((ULONG *)src) = *((ULONG *)src) & 0xFF000000 | (rgb))
#define _putindexpx(dst, vl)  (*(UCHAR *)dst = (UCHAR)vl), (dst = (VOID *)((UCHAR *)dst + 1))
#define _shiftptr(src, shift) ((VOID *)((UCHAR *)src + (LONG)shift))


  LONG  gray;  // Variable with level of gray
  LONG i, x;   // help variables

  // Get state of bitmap
  if(info2->cPlanes != 1)
  {
    return(1);
  }

  // RGB bitmap
  if(info2->cBitCount == 24)
  { LONG  cxs;      // Count of bytes missed for end of line.
    VOID *tmp;      // helpful pointer to buffer of bitmap
    LONG  rgb;      // Pixel colour

    // Get count of line (using  bytes for return)
    cxs = info2->cx * 3;
    cxs = cxs & 0x03 ? (cxs & 0xFFFFFFFC) + 0x04 : cxs;
    // Count Obliczenie wyr¢wnania do koäca wiersza
    cxs -= (info2->cx * 3);

    // Conversion all lines
    for(i = 0, tmp = bits; i < info2->cy; ++i)
    { // Konwersja all points in line
      for(x = 0; x < info2->cx; ++x)
      { // Get pixel
        rgb = _getrgbpx(tmp);
        // Count of gray from pixel from algorithm:
        //                                    gray = 0.30*R + 0.59*G + 0.11*B
        // Convert red colour
        gray  = (rgb >> 16) * 30;
        // Convert green colour
        gray += (((rgb >> 8) & 0xFF) * 59);
        // Convert blue colour
        gray += ((rgb & 0xFF) * 11);
        // Normalisation (divide by 100)
        gray /= 100;
        // Set description in colour pallette
        rgb = gray | (gray << 8) | (gray << 16);
        // Change pixel
        _putrgbpx(tmp, rgb);
        // Move pointer to next point
        tmp = _shiftptr(tmp, 3);
      }

      // Move pointer for equal to 4 bytes
      tmp = _shiftptr(tmp, cxs);
    }
  }
  // Bitmap with colour pallette
  else if(info2->cBitCount <= 8)
  { RGB2 *palette;  // Begin addressof pallette
    LONG  palsize;  // Count of colours in pallette 

    // get colours in pallette
    palsize = cbPalSize >> 2;
    // get pointer of begin of pallette.
    palette = (RGB2 *)((UCHAR *)info2 + info2->cbFix);
    // convert all description from pallette
    for(i = 0; i < palsize; ++i)
    { // Count of gray from pixel from algorithm:
      //                                    gray = 0.30*R + 0.59*G + 0.11*B
      // Convert red colour
      gray  = ((ULONG)palette[i].bRed) * 30;
        // Convert green colour
      gray += ((ULONG)palette[i].bGreen) * 59;
        // Convert blue colour
      gray += ((ULONG)palette[i].bBlue) * 11;
      // Normalizacja (podzielenie przez 100)
      gray /= 100;
      // Set description in colour pallette
      palette[i].bRed   = gray;
      palette[i].bGreen = gray;
      palette[i].bBlue  = gray;
    }
  }
  else
  {
    return(2);
  }

#undef _getrgbpx
#undef _putrgbpx
#undef _putindexpx
#undef _shiftptr

  // Correct run
  return(0);
}

//
// Stworzenie mapy bitowej z odcieniami szaro˜ci
//
// Parametry:
//       hwnd       - [parametr] Uchwyt do okna z map¥ bitow¥
//        hbm       - [parametr] Pierwowz¢r mapy bitowej
//
// Powr¢t:
//       NULLHANDLE  - Wyst¥piˆ bˆ¥d przy konwersji lub hbm==NULHANDLE
//   [inne warto˜ci] - Uchwyt do nowej mapy bitowej
//
// Nota: Funkcja bazuje si© na informacjach i kodzie Wojciecha Gazdy
//
static HBITMAP BtnCreateMonochrome(HWND hwnd, HBITMAP hbm)
{
  HBITMAP           hbmNew;       // Uchwyt nowo stworzonej mapy bitowej
  PBITMAPINFO2      pbmpi;        // Wska«nik struktury informacyjnej mapy bitowej
  ULONG             cbPalSize;    // Dˆugo˜† palety w pierwowzorze
  ULONG             cbBuffer;     // Dˆugo˜† bit¢w mapy bitowej (bez palety)
  PBYTE             pbBuffer;     // Bufor mapy bitowej
  HDC               hdcMem;       // Pomocniczy kontekst urz¥dzenia pami©ciowego
  HPS               hpsMem;       // Pomocnicza presentation space
  DEVOPENSTRUC      dop  = {NULL, "DISPLAY", NULL, NULL, NULL, NULL, NULL, NULL, NULL};
  SIZEL             size = {0, 0};// Rozmiary nowoutworzonej presentation space
  HAB               hab=WinQueryAnchorBlock(hwnd);

  // Odczytujemy tylko 16 bajt¢w informacji o mapie bitowej
  pbmpi=malloc(16);
  memset(pbmpi,0,16);
  pbmpi->cbFix=16;

  // Pobieramy dane (tylko 16 bajt¢w) o  bitmapie
  GpiQueryBitmapInfoHeader(hbm,(BITMAPINFOHEADER2 *) pbmpi);

  // nale¾y teraz pobra† dˆugo˜† palety pami©taj¥c ¾e powyzej 8 bit¢w
  // nie ma palety (pr¢cz sytuacji gdy jest pole cclrImportat).
  if(pbmpi->cBitCount > 8)
  {
    if(pbmpi->cbFix >= 40L && pbmpi->cclrImportant)
      cbPalSize=pbmpi->cclrImportant * 4L;
    else if(pbmpi->cbFix >= 36L && pbmpi->cclrUsed)
      cbPalSize=pbmpi->cclrUsed * 4L;
    else
      cbPalSize=0;
  }
  else
  {
    cbPalSize = 1;
    cbPalSize <<= pbmpi->cBitCount;
    cbPalSize *= pbmpi->cPlanes;

    // Nie ma palety kolor¢w - wi©cej ni¾ 256 kolor¢w w obrazie
    if(cbPalSize < 257)
    {
      // S¥ jakie˜ ograniczenia rozmiaru palety?
      if(pbmpi->cbFix >= 36 && pbmpi->cclrUsed)
        if(cbPalSize > pbmpi->cclrUsed)
          cbPalSize = pbmpi->cclrUsed;

      cbPalSize *=4;
    }
    else
      cbPalSize=0;
  }

  // Je˜li jest paleta to nale¾y zarezerwowa† pami©† na jej odczyt
  if (cbPalSize>0) pbmpi = realloc(pbmpi, pbmpi->cbFix + cbPalSize);

  // Obliczenie ilo˜ci bit¢w w jednej linii obrazu
  cbBuffer = pbmpi->cx * pbmpi->cBitCount;

  // Obliczenie ilo˜ci bajt¢w w linii
  if(cbBuffer & 0x07L)
    cbBuffer += 0x08;

  cbBuffer >>= 0x03;

  if (cbBuffer & 0x03L)
    cbBuffer=(cbBuffer + 4) & 0xFFFFFFFCL;
  else
    cbBuffer=cbBuffer & 0xFFFFFFFCL;

  // Obliczenie liczby bajt¢w w jednej pˆaszczy«nie bitowej
  cbBuffer *= pbmpi->cy;

  // Obliczenie liczby bajt¢w w caˆym obrazie
  cbBuffer *= pbmpi->cPlanes;

  if (cbBuffer) pbBuffer = malloc(cbBuffer);

  hdcMem = DevOpenDC(hab, OD_MEMORY, "*", 5L, (PDEVOPENDATA)&dop, NULLHANDLE);
  hpsMem = GpiCreatePS(hab, hdcMem, &size, GPIA_ASSOC | PU_PELS);
  if(hpsMem == GPI_ERROR)
  {
    DevCloseDC(hdcMem);
    return(NULLHANDLE);
  }

  if(GpiSetBitmap(hpsMem, hbm) == HBM_ERROR)
  {
    GpiAssociate(hpsMem, NULLHANDLE);
    GpiDestroyPS(hpsMem);
    DevCloseDC(hdcMem);
    return(NULLHANDLE);
  }
  else
  {
    if (GpiQueryBitmapBits(hpsMem, 0L, pbmpi->cy, pbBuffer, pbmpi)== GPI_ALTERROR)
    {
      GpiAssociate(hpsMem, NULLHANDLE);
      GpiDestroyPS(hpsMem);
      DevCloseDC(hdcMem);
      return(NULLHANDLE);
    }
  }
  // Usuni©cie mapy bitowej z presentation space
  GpiSetBitmap(hpsMem, NULLHANDLE);
  GpiAssociate(hpsMem, NULLHANDLE);
  GpiDestroyPS(hpsMem);
  DevCloseDC(hdcMem);

  // Teraz konwertujemy palete do palety monochromatycznej
  // (C) Wojciech Gazda z biblioteki bitmap.dll
  BtnChangeToMonochrome(pbmpi,(VOID *) pbBuffer,cbPalSize);
  hbmNew = GpiCreateBitmap(WinGetPS(HWND_DESKTOP), (BITMAPINFOHEADER2 *)pbmpi, CBM_INIT, pbBuffer, pbmpi);
  free(pbmpi);
  free(pbBuffer);

  return (hbmNew);
}

// Przygotowanie okna WC_WINBUTTON do pracy
//
// Parametry:
//   hab       - [parametr] uchwyt anchor block
//
// Powr¢t:
//   0    - poprawne wykonanie funkcji
//   1    - bˆ¥d rejestracji klasy okna
//
LONG APIENTRY BtnRegister(HAB hab)
{ ULONG winstyle;   // Domy˜lny styl okna
  LONG  rc;         // Kody bˆ©d¢w zwracane przez funkcje systemowe

  // Ustalenie domy˜lnego stylu okna WC_PROGRESSBAR
  winstyle =  CS_SIZEREDRAW; // | CS_HITTEST;CS_PARENTCLIP | CS_SYNCPAINT | CS_SIZEREDRAW;

  // Rejestracja nowej klasy okna - samoskaluj¥ce si© okno statusowe
  rc = WinRegisterClass(hab, WC_WINBUTTON, (PFNWP)BtnWndProc, winstyle, sizeof(VOID *));
  if(!rc) return(1);
  return(0);
}

// ******************
//  Funkcje lokalne
// ******************

//
// Procedura rysuj¥ca i kontroluj¥ca okno statusowe WC_WINBUTTON
//
MRESULT	EXPENTRY BtnWndProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
#define GETPRV() WinQueryWindowPtr(hwnd,0)

  PBTN pBtn; // Pointer of private button structure

  switch ( msg )
  {
    // Create of window control
    case WM_CREATE:
      return(BtnOnCreate(hwnd,mp1,mp2));

    // Size will be change
    case WM_SIZE:
      return(BtnOnSize(hwnd));

    // Change of state - enable/disable
    case WM_ENABLE:
      return(BtnOnEnable(hwnd,mp1));

    // Timer handler
    case WM_TIMER:
      // Get private structure
      pBtn=GETPRV();;
      if (pBtn == NULL)
      {
        // why we have this message without structure?
        // this never happend!
        return ((MPARAM)FALSE); 
      }

      // If button are disabled, it can not handle WM_TIMER message
      if (pBtn->flStyle & WS_DISABLED)
      {
        return ((MPARAM)FALSE); 
      }
      else
      { // Handle this message
        return(BtnOnTimer(hwnd,pBtn));
      }
      // break is not necesserry

    // Mnemonic matches handler
    case WM_MATCHMNEMONIC:
      // Get private structure
      pBtn=GETPRV();;
      if (pBtn == NULL)
      {
        // why we have this message without structure?
        // this never happend!
        return ((MPARAM)FALSE); 
      }

      // If button are disabled, it can not handle WM_MATCHMNEMONIC message
      if (pBtn->flStyle & WS_DISABLED)
      {
        return ((MPARAM)FALSE); 
      }
      else
      {
        return(Btn_WM_MATCHMNEMONIC(hwnd, pBtn, mp1));
      }
      // break is not necesserry

    case WM_SETWINDOWPARAMS :       // Zmiana parametr¢w okna
      return(Btn_WM_SETWINDOWPARAMS(hwnd,mp1));

    case WM_PRESPARAMCHANGED:       // Obsˆuga WinSetPresParam
      return (BtnOnPresParamChanged(hwnd,mp1));

    case WM_QUERYWINDOWPARAMS :
      return(Btn_WM_QUERYWINDOWPARAMS(hwnd,mp1));

    // get version of our control
    case WM_NULL:                  
      AsuQueryCtlInfo(hwnd, msg, mp1, &G_ctlButtonVersion);
      break;

    // left button are pressed
    case WM_BUTTON1DOWN :
      // Get private structure
      pBtn=GETPRV();;
      if (pBtn == NULL)
      {
        // why we have this message without structure?
        // this never happend!
        return ((MPARAM)FALSE); 
      }

      if (pBtn->flStyle & WS_DISABLED)
        break;
      else
        return(BtnMouseDown(hwnd));

    case WM_BUTTON2UP :             // Prawy przycisk zostaˆ wci˜ni©ty
      // Get private structure
      pBtn=GETPRV();;
      if (pBtn == NULL)
      {
        // why we have this message without structure?
        // this never happend!
        return ((MPARAM)FALSE); 
      }

      if (!(pBtn->flStyle & WS_DISABLED))
        WinSendMsg(WinQueryWindow(hwnd, QW_OWNER),WM_CONTROL,MPFROM2SHORT(pBtn->id,BN_BUTTON2CLICKED),NULL);
      return MPFROMLONG(TRUE);

    case WM_BUTTON1DBLCLK:           // Dwumlask lewego przycisku myszy
      // Get private structure
      pBtn=GETPRV();;
      if (pBtn == NULL)
      {
        // why we have this message without structure?
        // this never happend!
        return ((MPARAM)FALSE); 
      }

      if (!(pBtn->flStyle & WS_DISABLED))
      { POINTL ptl;
        ptl.x = (LONG)SHORT1FROMMP(mp1);
        ptl.y = (LONG)SHORT2FROMMP(mp1);

        return(BtnDblClick(hwnd,pBtn,ptl));
      }
      else
        return (MRFROMLONG(FALSE));

    case WM_BUTTON1UP :         // lewy przycisk zostaˆ wci˜ni©ty
      // Get private structure
      pBtn=GETPRV();;
      if (pBtn == NULL)
      {
        // why we have this message without structure?
        // this never happend!
        return ((MPARAM)FALSE); 
      }

      if (!(pBtn->flStyle & WS_DISABLED))
      {
        BtnMouseUp(hwnd,mp1,TRUE);
        // Trzeba wysˆa† notyfikacje!
        WinSendMsg(WinQueryWindow(hwnd, QW_OWNER),WM_CONTROL,MPFROM2SHORT(pBtn->id,BN_CLICKED),NULL);
        return (MRFROMLONG(TRUE));
      }
      return (MRFROMLONG(FALSE));

    case WM_MOUSEMOVE :             // odczyt ruch¢w myszy
      // Get private structure
      pBtn=GETPRV();;
      if (pBtn == NULL)
      {
        // why we have this message without structure?
        // this never happend!
        return ((MPARAM)FALSE); 
      }

      if (pBtn->flStyle & WS_DISABLED)
        break;
      else
        return(BtnOnMouseMove(hwnd,pBtn,mp1));

    case WM_CHAR:                  // wci˜ni©ty klawisz
      // Get private structure
      pBtn=GETPRV();;
      if (pBtn == NULL)
      {
        // why we have this message without structure?
        // this never happend!
        return ((MPARAM)FALSE); 
      }

      if (pBtn->flStyle & WS_DISABLED)
        break;
      else
        return (BtnOnChar(hwnd,mp1,mp2));

    case WM_ERASEBACKGROUND:       // szczerze m¢wi¥c nie mam poj©cia po
                                  // co to jest obsˆugiwane
      return(MRFROMLONG(TRUE));

    case WM_PAINT:                 // od˜wierzane okna
      return(BtnOnPaint(hwnd));

    case WM_SETFOCUS:              // zmiana fokusa
      return(BtnOnFocusChange(hwnd,mp2));

    case WM_DESTROY:
      return(BtnOnDestroy(hwnd));

    case WM_SYSCOLORCHANGE:        // Zmianie ulegˆy kolory systemowe
      // Get private structure
      pBtn=GETPRV();;
      if (pBtn == NULL)
      {
        // why we have this message without structure?
        // this never happend!
        return ((MPARAM)FALSE); 
      }
      // Uaktualnienie kolor¢w
      PpmQueryPresColors(hwnd, BTN_MAXCOLOR, G_PresParams, (LONG *)pBtn->ulColors);
      break;

    // Przej©cie dodatkowych (niestandrdowych) wiadomo˜ci
    case BM_AUTOSIZE:             // Ustaw automatycznie minimalne rozmiary
    case BM_SETDEFAULT:           // Zmiana przycisku na domy˜lny
    case BM_CLICK:                // Fake-click
    case BM_QUERYHILITE:          // W jakim przycisk jest stanie? (wskazuj¥cym ;-)))
    case BM_SETHILITE:            // Zmieä stan (wylecz kaca ;-))
    case BM_QUERYHEIGHT:
    case BM_QUERYWIDTH:
      return(Btn_OtherProc(hwnd,msg,mp1,mp2));
  }
#undef GETPRV
  return(WinDefWindowProc(hwnd, msg, mp1, mp2));
}

// Funkcja rysuj¥ca obram¢wk© zaznaczonego przycisku
// (zast©pnik kt¢ry umie rysowa† linie przerywan¥ co piksele)
//
// Parametry:
//   hps                          - [paramter] uchwyt do Presentation Spaece
//   xLeft, yBottom, xRight, yTop - [paramter] rozmiary w jakich powinni˜my rysowa†
//
// Powr¢t:
//   brak
//
static VOID BtnDrawBox(HPS hps,LONG xLeft,LONG yBottom,LONG xRight,LONG yTop)
{
  POINTL ptl;
  LONG   x1;
  LONG   y1;

  for (y1=yBottom;y1<yTop+1;y1=y1+2)
  { ptl.x = xLeft;
    ptl.y = y1;

    GpiSetPel(hps,&ptl);

    ptl.x = xRight;
    ptl.y = y1;

    GpiSetPel(hps,&ptl);
  }

  for (x1=xLeft;x1<xRight;x1=x1+2)
  { ptl.x = x1;
    ptl.y = yTop;

    GpiSetPel(hps,&ptl);

    ptl.x = x1;
    ptl.y = yBottom;

    GpiSetPel(hps,&ptl);
  }
  return;
}


// Funkcja dostosowywuj¥ca zmienne przypisane do okna do danych rozmiar¢w
//
// Parametry:
//   hwnd    - [paramter] uchwyt do kontrolki
//   x, y    - [paramter] miejsca pocz¥tku ukˆadu wsp¢ˆrz©dnych rysowania kontrolki
//  cx, cy   - [paramter] dˆugo˜† i szeroko˜† od x,y
//
// Powr¢t:
//   brak
//
static VOID BtnSizeButton(PBTN pBtn, LONG x, LONG y, LONG cx, LONG cy)
{
  POINTL ptltemp;

  // zapami©tujemy wymiary prostok¥ta przycisku
  pBtn->rcl.xLeft   = x;
  pBtn->rcl.yBottom = y;
  pBtn->rcl.xRight  = x + cx;
  pBtn->rcl.yTop    = y + cy;

  if (pBtn->ulTextWidth < (pBtn->rcl.xRight - pBtn->rcl.xLeft))
    ptltemp.x=(pBtn->rcl.xLeft + pBtn->rcl.xRight - pBtn->ulTextWidth) >> 1;
  else
    ptltemp.x=-1; // Zaznaczamy ¾e jest to miejscie do okre˜lenia p¢«niej

  // if (pBtn->ulTextHeight < (pBtn->rcl.yTop - pBtn->rcl.yBottom))
  ptltemp.y=(pBtn->rcl.yTop + pBtn->rcl.yBottom - pBtn->ulTextHeight) >> 1;
  // else
  // ptltemp.y=-1; // Zaznaczamy ¾e jest to miejscie do okre˜lenia p¢«niej

  // je˜li punkty s¥ znane to nale¾y okre˜li†
  // poˆo¾enie mapy bitowej i tekstu wzgl©dem
  // siebie, ale tylko wtedy gdy jest to styl BS_BITMAP
  // i je˜li s¥ jakie˜ bitmapy do pokazania
  if ((pBtn->flStyle & BS_BITMAP) && (pBtn->hbm))
  {
    switch (pBtn->flStyle & 0x30)
    {
      // Bitmapa jest po prawej stronie tekstu
      case BS_ALIGNRIGHT:
        if ((pBtn->szText) && (strlen(pBtn->szText)>0))
        {
          if (ptltemp.x > 0) // kiedy wyliczony ˜rodek X jest prawidˆowy
          {
            pBtn->rclText.xLeft   = ptltemp.x - (pBtn->bmpcx >> 1) - (pBtn->fm.lMaxCharInc >> 2);
            pBtn->rclText.xRight  = pBtn->rclText.xLeft + pBtn->ulTextWidth;
            pBtn->ptlBmp.x        = pBtn->rclText.xRight + (pBtn->fm.lMaxCharInc >> 1);
          }
          else
          {
            pBtn->rclText.xLeft   = pBtn->rcl.xLeft+4;
            pBtn->rclText.xRight  = pBtn->rcl.xRight-6-pBtn->bmpcx - (pBtn->fm.lMaxCharInc >> 1);
            pBtn->ptlBmp.x        = pBtn->rclText.xRight + (pBtn->fm.lMaxCharInc >> 1);
          }
          pBtn->ptlBmp.y        = (pBtn->rcl.yTop + pBtn->rcl.yBottom - pBtn->bmpcy ) >> 1;
        }
        else
        {
          memset(&pBtn->rclText,0,sizeof(RECTL));
          pBtn->ptlBmp.x        = (pBtn->rcl.xRight + pBtn->rcl.xLeft - pBtn->bmpcx ) >> 1;
          pBtn->ptlBmp.y        = (pBtn->rcl.yTop + pBtn->rcl.yBottom - pBtn->bmpcy ) >> 1;
        }
        break;

      // Bitmapa jest powy¾ej tekstu
      case BS_ALIGNTOP:
        if ((pBtn->szText) && (strlen(pBtn->szText)>0))
        {
          if (ptltemp.x > 0) // kiedy wyliczony ˜rodek X jest prawidˆowy
          {
            pBtn->rclText.xLeft   = ptltemp.x;
            pBtn->rclText.xRight  = pBtn->rclText.xLeft + pBtn->ulTextWidth;
          }
          else
          {
            pBtn->rclText.xLeft   = 4;
            pBtn->rclText.xRight  = pBtn->rcl.xRight-4;
          }

          pBtn->rclText.yBottom = ptltemp.y - (pBtn->bmpcy >> 1)- (pBtn->fm.lMaxBaselineExt >> 4);
          pBtn->rclText.yTop    = pBtn->rclText.yBottom + pBtn->ulTextHeight;

          pBtn->ptlBmp.x        = (pBtn->rcl.xLeft + pBtn->rcl.xRight - pBtn->bmpcy >> 1);
          pBtn->ptlBmp.y        = pBtn->rclText.yTop + (pBtn->fm.lMaxBaselineExt >> 2);
        }
        else
        {
          memset(&pBtn->rclText,0,sizeof(RECTL));
          pBtn->ptlBmp.x        = (pBtn->rcl.xRight + pBtn->rcl.xLeft - pBtn->bmpcx ) >> 1;
          pBtn->ptlBmp.y        = (pBtn->rcl.yTop + pBtn->rcl.yBottom - pBtn->bmpcy ) >> 1;
        }
        break;

      case BS_ALIGNBOTTOM: // Bitmapa poni¾ej tekstu
        if ((pBtn->szText) && (strlen(pBtn->szText)>0)) // Mo¾e zdarzy† si©
                                                        // ¾e nie ma tekstu
        {
          pBtn->ptlBmp.x = (pBtn->rcl.xLeft + pBtn->rcl.xRight - pBtn->bmpcy >> 1);

//          if (ptltemp.y < 0)  // kiedy wyliczony ˜rodek Y jest prawidˆowy
            pBtn->ptlBmp.y = ptltemp.y - (pBtn->bmpcy >> 1)- (pBtn->fm.lMaxBaselineExt >> 4);
//          else
//            pBtn->ptlBmp.y = pBtn->rcl.yTop - (pBtn->bmpcy >> 1)- (pBtn->fm.lMaxBaselineExt >> 4);

          if (ptltemp.x > 0) // kiedy wyliczony ˜rodek X jest prawidˆowy
          {
            pBtn->rclText.xLeft   = ptltemp.x;
            pBtn->rclText.xRight  = pBtn->rclText.xLeft + pBtn->ulTextWidth;
          }
          else
          {
            pBtn->rclText.xLeft   = 4;
            pBtn->rclText.xRight  = pBtn->rcl.xRight-4;
          }

          pBtn->rclText.yBottom = pBtn->ptlBmp.y + pBtn->bmpcy + (pBtn->fm.lMaxBaselineExt >> 2);
          pBtn->rclText.yTop    = pBtn->rclText.yBottom + pBtn->ulTextHeight;
        }
        else
        {
          memset(&pBtn->rclText,0,sizeof(RECTL));
          pBtn->ptlBmp.x        = (pBtn->rcl.xRight + pBtn->rcl.xLeft - pBtn->bmpcx ) >> 1;
          pBtn->ptlBmp.y        = (pBtn->rcl.yTop + pBtn->rcl.yBottom - pBtn->bmpcy ) >> 1;
        }
        break;

      case BS_ALIGNLEFT:  // Bitmapa po lewej stronie tekstu  (styl domy˜lny)
        if ((pBtn->szText) && (strlen(pBtn->szText)>0)) // Mo¾e zdarzy† si©
                                                        // ¾e nie ma tekstu
        {
          if (ptltemp.x > 0) // kiedy wyliczony ˜rodek jest prawidˆowy
            pBtn->ptlBmp.x        = ptltemp.x - (pBtn->bmpcx >> 1) - (pBtn->fm.lMaxCharInc >> 2);
          else
            pBtn->ptlBmp.x        = pBtn->rcl.xLeft+4;

          pBtn->ptlBmp.y        = (pBtn->rcl.yTop + pBtn->rcl.yBottom - pBtn->bmpcy ) >> 1;

          pBtn->rclText.xLeft   = pBtn->ptlBmp.x + pBtn->bmpcx  + (pBtn->fm.lMaxCharInc >> 1);

          if (ptltemp.x > 0) // kiedy wyliczony ˜rodek jest prawidˆowy
            pBtn->rclText.xRight  = pBtn->rclText.xLeft + pBtn->ulTextWidth;
          else
            pBtn->rclText.xRight  = pBtn->rcl.xRight -4;

          pBtn->rclText.yBottom = ptltemp.y;
          pBtn->rclText.yTop    = ptltemp.y+pBtn->ulTextHeight;
        }
        else
        {
          memset(&pBtn->rclText,0,sizeof(RECTL));
          pBtn->ptlBmp.x        = (pBtn->rcl.xRight + pBtn->rcl.xLeft - pBtn->bmpcx ) >> 1;
          pBtn->ptlBmp.y        = (pBtn->rcl.yTop + pBtn->rcl.yBottom - pBtn->bmpcy ) >> 1;
        }
        break;
    }
  }
  else
  {
    pBtn->ptlBmp.x    = 0;
    pBtn->ptlBmp.y    = 0;

    pBtn->rclText = pBtn->rcl;
    pBtn->rclText.xLeft  +=4;
    pBtn->rclText.xRight -=4;
    pBtn->rclText.yTop   -=4;
    pBtn->rclText.yBottom+=4;
  }
}

// Tekst przycisku jest podsyˆany wraz z numerami map bitowych, wi©c
// funkcja filtruje dany teskt i odcina wskazania na bitmap©, a bitmapy
// jak i tekst dodaje do prywatnej struktury
//
// Parametry:
//   hwnd   - Uchwyt do przycisku WC_WINBUTTON
//   szText - Wska«nik do tekstu kt¢ry ma znajdowa† si© w przycisku
//
// Powr¢t:
//   brak
static VOID BtnTextFilter(HWND hwnd,PSZ szText)
{
  PBTN   pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0);
  PSZ    szTemp1;
  PSZ    szTemp2;
  PSZ    szNew;
  ULONG  ulBmp[4] = {0};
  ULONG  ulCount = 0;

  szNew=malloc(6);
  *szNew=0;

  // Sprawdzenie czy jest to wpis z bitmapami
  szTemp1 = strstr((char *)szText,"\t");
  if (!szTemp1)
  {
    BtnStoreWindowText(hwnd, pBtn, szText);
    return;
  }
  szTemp1++;

  // Alokacja potrzebnej pami©ci
  szTemp2=(PSZ)malloc(szTemp1-szText+1);
  // Kopiowanie cz©˜ci ˆaäcucha do '\t'
  strncpy(szTemp2,szText,szTemp1-szText);

  // Poszukiwanie bitmap
  while ( szTemp2 && ulCount<4 )
  {
    // poszukiwanie numeru bitmapy
    szTemp1 = strstr((char *)szTemp2+1,"#");

    if (!szTemp1)
      szTemp1 = strstr((char *)szTemp2+1,"\t");

    if (!szTemp1) break;

    // Je˜li bˆ©dna liczba to nie nale¾y dalej kontynuuowa†
    // bo zdarzy† si© mo¾e GPF
    if (szTemp1-szTemp2-1>6) break;

    if (szTemp1-szTemp2-1!=0)
    {
      strncpy(szNew,(char *)szTemp2+1,szTemp1-szTemp2-1);
      ulBmp[ulCount]=atol(szNew); // Konwertujemy wska«nik do liczby
    }
    else
      ulBmp[ulCount]=0;
    // wska«nik szNew nale¾y zwolni†
    ulCount++;
    szTemp2=szTemp1;
  }
  free(szNew); szNew=NULL;
  szNew=strstr((char *)szText,"\t")+1;
  // Je˜li jest najwa¾niejsza bitmapa to j¥ nagrywamy
  if (ulBmp[0]) // Wgrywamy ¾¥dane bitmapy
  {
    BtnStoreWindowBitmaps(hwnd, 0, ulBmp[0], ulBmp[1], ulBmp[2], ulBmp[3]);
    BtnStoreWindowText(hwnd, pBtn, szNew);
  }
  else
    BtnStoreWindowText(hwnd, pBtn, szText);

  return;
}

static MRESULT Btn_WM_MATCHMNEMONIC (HWND hwnd, PBTN pBtn, MPARAM mp1)
{
  PSZ      szTemp;

  // Sprawdzenie czy wska«nik nie jest pusty
  if (!pBtn) return (0);
  if (SHORT1FROMMP(mp1)==0) return (0);
  // Znalezienie pocz¥tku mnemonika czyli '~' w tekscie okna
  szTemp=strstr(pBtn->szText,"~\0");
  if (szTemp)
  {
    // nast©pny znak ma by† przypisany jako mnemonik
    // wi©c sprawdzenie czy mnemonik dotyczy klawisza
    // z naszego przycisku.
    szTemp++;
    if (toupper(*szTemp)==toupper((CHAR)CHAR1FROMMP(mp1)))
    {
      // Przyci˜ni©cie klawisza
      BtnMouseDown(hwnd);
      BtnMouseUp(hwnd,NULL,FALSE);
      return(MRFROMLONG(TRUE));
    }
  }
  return(MRFROMLONG(FALSE));
}

static MRESULT  BtnOnCreate(HWND hwnd, MPARAM mp1, MPARAM mp2)
{
  PBTN             pBtn;
  PCREATESTRUCT    pCreateStr;  // Wska«nik tworzenia okna
  HPS              hps;

  // rezerwacja pami©† na prywatn¥ struktur©
  pBtn=malloc(sizeof(BTN));
  memset(pBtn,0,sizeof(BTN));

  // nagranie adresu wska«nika
  WinSetWindowPtr(hwnd, 0, (PVOID)pBtn);

  // Pobranie informacji o tworzonym oknie
  pCreateStr = (PCREATESTRUCT)PVOIDFROMMP(mp2);

  pBtn->id         = pCreateStr->id; // zachowanie ID przycisku

  // Kiedy przycisk jest typu BS_FLATBUTTON to nie ma TabStop'u
  // wi©c na wszelki wypadek zostaje on usuni©ty
  if ((pCreateStr->flStyle & 0x07) == BS_FLATBUTTON)
  {
    pCreateStr->flStyle &= ~WS_TABSTOP;
    // Powiadomienie systemu o zmianie w stylu przycisku
    WinSetWindowULong(hwnd,QWL_STYLE,pCreateStr->flStyle);
  }

  pBtn->flStyle=pCreateStr->flStyle; // Zachownie stylu okna

  // Sprawdzenie metryk fontu na przycisku
  hps  = WinGetPS(HWND_DESKTOP);
  GpiQueryFontMetrics(hps, sizeof(FONTMETRICS), &pBtn->fm);
  WinReleasePS(hps);

  // Zachownie CTLDATA
  BtnSetCtlData(hwnd, pBtn, PVOIDFROMMP(mp1));

  // Pobranie Presentation Parameters
  PpmStorePresParams(hwnd, (PRESPARAMS *)pCreateStr->pPresParams, G_PresParams, BTN_MAXCOLOR);

  // Zapami©tanie kolor¢w pobranych z presentation parameters
  PpmQueryPresColors(hwnd, BTN_MAXCOLOR, G_PresParams, (LONG *)pBtn->ulColors);

  // Je˜li jest jaki˜ tekst to jest kopiowany do prywatnej struktury
  // z obliczeniem dˆugo˜ci/szeroko˜ci itd.
  if (pCreateStr->pszText)
  {
    BtnTextFilter(hwnd,pCreateStr->pszText);
    BtnStoreFont(hwnd, pBtn);
  }

  // Je¾eli przycisk ma dostosowywa† si© do wielko˜ci
  if (pBtn->flStyle & BS_AUTOSIZE)
    BtnAutoscale(hwnd,NULL,NULL);
  else
    // A je˜li nie to ustalmy rozmiary przycisku
    BtnSizeButton(pBtn, 0, 0, pCreateStr->cx, pCreateStr->cy);

  // Zapami©tanie wska«nika myszy
  pBtn->hptrArrow = WinQuerySysPointer(HWND_DESKTOP, SPTR_ARROW, FALSE);
//  printf("Dla %s Styl -> %d\n",pBtn->szText,pBtn->flStyle & BS_THINFRAME);
  // Od˜wierzenie okna
  WinInvalidateRect(hwnd, (PRECTL) NULL, TRUE);
  return (0);
}


// Obsˆuga WM_PAINT
MRESULT  BtnOnPaint(HWND hwnd)
{
  HPS        hps;       // Uchwyt do Presentation Space
  POINTL     ptl;
  RECTL      rcl;       // Pomocniczy prostok¥t
  PBTN       pBtn;      // Prywatna struktura z danymi
  OWNERBACK  bkgnd;     // Struktura umo¾liwiaj¥ca wˆa˜cicielowi przerysowanie tˆa
  HWND       hwndOwner; // Wˆa˜ciciel przycisku

  // Odczyt wska«nika z prywtn¥ struktur¥
  pBtn= (PBTN)WinQueryWindowPtr(hwnd, 0);

  // rozpocz©cie rysowania
  hps = WinBeginPaint(hwnd, (HPS)NULL, (PRECTL)NULL);
  // Tworzenie palety logicznych kolor¢w (czyli zdefiniowanych na pocz¥tku)
  GpiCreateLogColorTable(hps, 0L, LCOLF_CONSECRGB, 0, BTN_MAXCOLOR, (LONG *)pBtn->ulColors);

  rcl=pBtn->rcl;

  // Gdy przycisk jest rysowany przez wˆa˜ciciela
  if (pBtn->flStyle & BS_OWNERDRAW)
  {
    // Odczyt uchwytu wˆa˜ciciela
    hwndOwner = WinQueryWindow(hwnd, QW_OWNER);
    if(hwndOwner != NULLHANDLE)
    {
      // Inicjacja struktury steruj¥cej przerysowywaniem
      bkgnd.hwnd       = hwnd;
      bkgnd.hps        = hps;
      bkgnd.bound      = rcl;
      bkgnd.refpoint.x = 0;
      bkgnd.refpoint.y = 0;

      // Wysˆanie komunikatu z ¾¥daniem przerysowania tˆa
      WinSendMsg(hwndOwner, WM_DRAWITEM, MPFROMSHORT(pBtn->id), MPFROMP(&bkgnd));
    }
  }
  else
  {
    // wypeˆniemy kolorem tˆa caˆy przycisk
    WinFillRect(hps, &pBtn->rcl,BTN_BACKGROUND);

    if (((pBtn->flStyle & 0x07) != BS_FLATBUTTON)  || ((pBtn->flState & WBS_MOUSEIN) && (!(pBtn->flStyle & WS_DISABLED)))
       || (pBtn->flState & WBS_DOWN))
    {
      // Pierwsza cz©˜† obram¢wki zale¾na od stylu
      GpiSetColor(hps,(pBtn->flState & WBS_DOWN) ? BTN_DARKBORDER : BTN_LIGHTBORDER);

      // Rysowanie obram¢wki wewn©trznej. Przy stanie normalnym lewy bok i g¢ra
      //  (nie dotyczy BS_FLATBUTTON razem ze stylem BS_THINFRAME);
      if (((pBtn->flStyle & 0x07) != BS_FLATBUTTON) && (!(pBtn->flStyle & BS_THINFRAME)))
      {
        ptl.x=rcl.xLeft+2; ptl.y=rcl.yBottom+2;
        GpiMove(hps,&ptl);
        ptl.x=rcl.xLeft+2; ptl.y=rcl.yTop-3;
        GpiLine(hps,&ptl);
        ptl.x=rcl.xRight-3; ptl.y=rcl.yTop-3;
        GpiLine(hps,&ptl);

        GpiSetColor(hps,(pBtn->flState & WBS_DOWN) ? BTN_LIGHTBORDER : BTN_DARKBORDER);
        ptl.x=rcl.xRight-3; ptl.y=rcl.yBottom+2;
        GpiLine(hps,&ptl);
        ptl.x=rcl.xLeft+2; ptl.y=rcl.yBottom+2;
        GpiLine(hps,&ptl);
      }

      GpiSetColor(hps,(pBtn->flState & WBS_DOWN) ? BTN_DARKBORDER : BTN_LIGHTBORDER);

      if (((pBtn->flStyle & 0x07) ==BS_FLATBUTTON) && (pBtn->flStyle & BS_THINFRAME))
      {
        ptl.x=rcl.xLeft; ptl.y=rcl.yBottom;
        GpiMove(hps, &ptl);
        ptl.x=rcl.xLeft; ptl.y=rcl.yTop-1;
        GpiLine(hps, &ptl);
        ptl.x=rcl.xRight-1; ptl.y=rcl.yTop-1;
        GpiLine(hps, &ptl);
        GpiSetColor(hps,(pBtn->flState & WBS_DOWN) ? BTN_LIGHTBORDER : BTN_DARKBORDER);
        ptl.x=rcl.xRight-1; ptl.y=rcl.yBottom;
        GpiLine(hps, &ptl);
        ptl.x=rcl.xLeft; ptl.y=rcl.yBottom;
        GpiLine(hps, &ptl);
      }
      else
      {
        ptl.x=rcl.xLeft+1; ptl.y=rcl.yBottom+1;
        GpiMove(hps,&ptl);
        ptl.x=rcl.xLeft+1; ptl.y=rcl.yTop-2;
        GpiLine(hps,&ptl);
        ptl.x=rcl.xRight-2; ptl.y=rcl.yTop-2;
        GpiLine(hps,&ptl);
        GpiSetColor(hps,(pBtn->flState & WBS_DOWN) ? BTN_LIGHTBORDER : BTN_DARKBORDER);
        ptl.x=rcl.xRight-2; ptl.y=rcl.yBottom+1;
        GpiLine(hps,&ptl);
        ptl.x=rcl.xLeft+1; ptl.y=rcl.yBottom+1;
        GpiLine(hps,&ptl);
        GpiSetColor(hps,(pBtn->flState & WBS_DOWN) ? BTN_LIGHTBORDER : BTN_DEFBORDER);

        ptl.x=rcl.xLeft+1; ptl.y=rcl.yBottom+1;
        GpiMove(hps, &ptl);
        ptl.x=rcl.xRight-2; ptl.y=rcl.yBottom+1;
        GpiLine(hps, &ptl);
        ptl.x=rcl.xRight-2; ptl.y=rcl.yTop-3;
        GpiLine(hps, &ptl);
      }
    }

    // Wypisujemy tekst ale tylko wtedy gdy istnieje
    if ((pBtn->szText) && (strlen(pBtn->szText)>0))
      BtnDrawText(hwnd,pBtn);

    if (pBtn->flStyle & BS_BITMAP)
    {
      if (pBtn->flState & WBS_DOWN) { pBtn->ptlBmp.x ++; pBtn->ptlBmp.y --; }

      //    GpiSetPattern(hps, PATSYM_DEFAULT);
      if (pBtn->flStyle & WS_DISABLED) // je¾eli jest to klawisz zablokowany
        GrfDrawBitmap(hps, pBtn->hbmDisabled, pBtn->hbmMaskDisabled,&pBtn->ptlBmp,TRUE);
      else
      {
        if ((pBtn->flStyle & BS_FLATBUTTON) && (!(pBtn->flState & WBS_MOUSEIN)))
          GrfDrawBitmap(hps, pBtn->hbmDisabled, pBtn->hbmMaskDisabled,&pBtn->ptlBmp,FALSE);
        else
          GrfDrawBitmap(hps, pBtn->hbm, pBtn->hbmMask,&pBtn->ptlBmp,FALSE);
      }

      if (pBtn->flState & WBS_DOWN) { pBtn->ptlBmp.x --; pBtn->ptlBmp.y ++; }
    }
  }
  WinEndPaint(hps);

  return (0);
}


static MRESULT  BtnOnSize(HWND hwnd)
{
  RECTL rcl;         // Prostok¥t kontrolki
  PBTN  pBtn;        // Prywatna struktura przycisku

  // Pobranie wska«nika struktury
  pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0);
  // Odczytanie aktualnych rozmiar¢w okna
  WinQueryWindowRect(hwnd, &rcl);
  // Dostosowania si© przycisku do nowych rozmiar¢w
  BtnSizeButton(pBtn,rcl.xLeft,rcl.yBottom,(rcl.xRight - rcl.xLeft),(rcl.yTop - rcl.yBottom));

  WinInvalidateRect(hwnd, (PRECTL)NULL, TRUE); // wymuszenie przerysowania

  return (0);
}

static MRESULT  BtnOnEnable(HWND hwnd, MPARAM mp1)
{
  PBTN pBtn;

  pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0); // Znajd«my wska«nik

  if ((BOOL)SHORT1FROMMP(mp1)) // Odczytajmy stan  i zmieämy go
    pBtn->flStyle &= ~WS_DISABLED;
  else
    pBtn->flStyle |= WS_DISABLED;

  WinInvalidateRect(hwnd, (PRECTL)NULL, TRUE); // wymuszamy przerysowanie
  return (0);
}

static MRESULT  Btn_WM_SETWINDOWPARAMS(HWND hwnd, MPARAM mp1)
{
  PWNDPARAMS pwprm;       // Wska«nik o parametrach okna
  RECTL      rcl;
  PBTN       pBtn;

  // Odczyt wska«nika prywatnej struktury
  pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0);
  // Odczyt parametr¢w prezentacji kt¢re znajduj¥ si© w parametrze 1
  pwprm = (PWNDPARAMS)PVOIDFROMMP(mp1);

  if ( pwprm->fsStatus & WPM_TEXT )
  {
    BtnTextFilter(hwnd, pwprm->pszText);

    if (pBtn->flStyle & BS_AUTOSIZE) // Je¾eli przycisk ma dostosowywa† si© do wielko˜ci
      BtnAutoscale(hwnd,NULL,NULL);
    else
    {
      WinQueryWindowRect(hwnd, &rcl);
      BtnSizeButton(pBtn,rcl.xLeft,rcl.yBottom,(rcl.xRight - rcl.xLeft),(rcl.yTop - rcl.yBottom));
    }
    WinInvalidateRect(hwnd, NULL, FALSE);
    return(MRFROMLONG(TRUE));
  }

  if ( pwprm->fsStatus & WPM_CTLDATA )
  {
    BtnSetCtlData(hwnd,pBtn,pwprm->pCtlData);
    WinInvalidateRect(hwnd, NULL, FALSE);
    return(MRFROMLONG(TRUE));
  }

  if ( pwprm->fsStatus & WPM_CBCTLDATA )
    pBtn->ctlData.cb = (pwprm->cbCtlData > sizeof(WBTNCDATA)) ? sizeof(WBTNCDATA) : pwprm->cbCtlData;

  return(0);
}

static MRESULT  Btn_WM_QUERYWINDOWPARAMS(HWND hwnd, MPARAM mp1)
{
  PWNDPARAMS pwprm;       // Wska«nik o parametrach okna
  PBTN   pBtn;

  pwprm = (PWNDPARAMS)PVOIDFROMMP(mp1);
  pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0);

  if (pwprm->fsStatus & (WPM_TEXT | WPM_CCHTEXT))
  {
    strcpy(pwprm->pszText, pBtn->szText); // podajmy tekst jaki mamy
    pwprm->cchText = (ULONG)strlen(pBtn->szText); // ...i jego dˆugo˜†

    return(MRFROMLONG(TRUE));
  }

  if (pwprm->fsStatus & (WPM_CBCTLDATA | WPM_CTLDATA))
  {
    memcpy(pwprm->pCtlData,&pBtn->ctlData,pBtn->ctlData.cb);
    pwprm->cbCtlData=pBtn->ctlData.cb;
    return(MRFROMLONG(TRUE));
  }

  if ( pwprm->fsStatus & WPM_TEXT )
  {
    strcpy(pwprm->pszText, pBtn->szText);
    return(MRFROMLONG(TRUE));
  }

  if ( pwprm->fsStatus & WPM_CCHTEXT )
  {
    pwprm->cchText = (ULONG)strlen(pBtn->szText);
    return(MRFROMLONG(TRUE));
  }

  if ( pwprm->fsStatus & WPM_CTLDATA )
  {
    memcpy(pwprm->pCtlData,&pBtn->ctlData,pBtn->ctlData.cb);
    return(MRFROMLONG(TRUE));
  }

  if ( pwprm->fsStatus & WPM_CBCTLDATA )
  {
    pwprm->cbCtlData=pBtn->ctlData.cb;
    return(MRFROMLONG(TRUE));
  }

  return(0);
}

static MRESULT  BtnMouseDown(HWND hwnd)
{
  PBTN pBtn;

  pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0);

  if (pBtn->flStyle & WS_DISABLED) // Gdy przycisk jest nieaktywny
    return (0);                    // to ju¾ dalej nic nie musimy
                                   // robi†!

  if ((pBtn->flStyle & 0x07) != BS_FLATBUTTON)
    WinFocusChange(HWND_DESKTOP,hwnd,0);

  pBtn->flState &= ~WBS_UP;
  pBtn->flState |= WBS_DOWN | WBS_CAPTURE; // zmieniamy stan...
  WinSetActiveWindow(HWND_DESKTOP, WinQueryWindow(hwnd, QW_OWNER));
  WinSetCapture(HWND_DESKTOP, hwnd);
  WinInvalidateRect(hwnd, NULL, TRUE); // trzeba przerysowa† przycisk

  return (0);
}

static MRESULT BtnMouseUp(HWND hwnd, MPARAM mp1, BOOL flMouse)
{
  PBTN pBtn;
  POINTL   ptl;

  pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0);

  if ( pBtn->flStyle & WS_DISABLED ) // Gdy przycisk jest nieaktywny
    return (0);                      // to ju¾ dalej nic nie musimy
                                     // robi†

//* PD 99-06-07 _START_
  pBtn->flState &= ~WBS_DOWN;
  pBtn->flState &= ~WBS_CAPTURE;
  pBtn->flState |= WBS_UP;
//* PD 99-06-07 _STOP_

  WinSetCapture(HWND_DESKTOP, (HWND)NULL);
  ptl.x = (LONG)SHORT1FROMMP(mp1);
  ptl.y = (LONG)SHORT2FROMMP(mp1);

  if ( WinPtInRect((HAB)NULL, &pBtn->rcl, &ptl) )
  {
    if (pBtn->flStyle & BS_SYSCOMMAND)
      WinPostMsg(WinQueryWindow(hwnd, QW_OWNER), WM_SYSCOMMAND,MPFROMSHORT(pBtn->id),MPFROM2SHORT(flMouse, CMDSRC_OTHER));
    else if (pBtn->flStyle & BS_HELP)
           WinPostMsg(WinQueryWindow(hwnd, QW_OWNER), WM_HELP,MPFROMSHORT(pBtn->id),MPFROM2SHORT(flMouse, CMDSRC_OTHER));
         else
           WinPostMsg(WinQueryWindow(hwnd, QW_OWNER), WM_COMMAND,MPFROMSHORT(pBtn->id),MPFROM2SHORT(flMouse, CMDSRC_OTHER));
    WinInvalidateRect(hwnd, (PRECTL)NULL, TRUE);
  }
  return (0);
}

static MRESULT BtnOnTimer(HWND hwnd, PBTN pBtn)
{
  POINTL ptl;
  SWP    swp;

  if (!pBtn) return (0);

  // Odczyt pozycji kursora myszy
  WinQueryPointerPos(HWND_DESKTOP, &ptl);
  // Odczyt rozmiar¢w okna
  WinQueryWindowPos(hwnd,&swp);
  WinMapWindowPoints(HWND_DESKTOP, WinQueryWindow(hwnd,QW_PARENT), &ptl, 1);
  // Kiedy na przycisku jest mysz
  if (pBtn->flState & WBS_MOUSEIN)
  { // sprawdzenie czy nadal mysz znajduje si© w prostok¥cie przycisku
    if ((swp.x>ptl.x) || (swp.x+swp.cx<ptl.x) ||
        (swp.y>ptl.y) || (swp.y+swp.cy<ptl.y))
    {
      // Kiedy ju¾ si© nie znajduje wysˆanie o tym notyfikacji
      WinSendMsg(WinQueryWindow(hwnd, QW_OWNER),WM_CONTROL,MPFROM2SHORT(pBtn->id,BN_MOUSELEAVE),NULL);
      // Wykasowanie z prywatnej struktury informacji ¾e mysz jest na przycisku
      pBtn->flState &= ~WBS_MOUSEIN;
      // Wstrzymujemy czasomierz
      WinStopTimer(WinQueryAnchorBlock(hwnd), hwnd, 1);

      // Przerysowanie ale tylko wtedy gdy styl BS_FLATBUTTON ai jest aktywny
      if ((pBtn->flStyle & 0x07) == BS_FLATBUTTON)
        WinInvalidateRect(hwnd, (PRECTL)NULL, TRUE);
    }
  }
  return (0);
}

static MRESULT  BtnOnMouseMove(HWND hwnd, PBTN pBtn, MPARAM mp1)
{
  POINTL  ptl; // pozycja myszki

  // Por¢wnanie stylu. Czy przypadkiem nie jest wci˜ni©ty klawisz myszki lecz
  // nie jest odci˜ni©ty, czy mo¾e jest to styl BS_FLATBUTTON
  if (( pBtn->flState & WBS_CAPTURE ) || (( pBtn->flStyle & 0x07) == BS_FLATBUTTON ))
  {
    // Zapisanie pozycji myszki w czasie tej wiadomo˜ci
    ptl.x = (LONG)SHORT1FROMMP(mp1);
    ptl.y = (LONG)SHORT2FROMMP(mp1);

    // Sprawdzenie czy punkt znajduje si© w prostok¥cie przycisku
    if ( WinPtInRect((HAB)NULL, &pBtn->rcl, &ptl) != TRUE )
    { // Kursor jest poza prostok¥tem

      if (( pBtn->flState & WBS_DOWN ) && ( pBtn->flState & WBS_CAPTURE ))
      {
        // przycisk ju¾ nie powinien by† wci˜ni©ty
        // lecz nale¾y zapami©ta† ¾e jest on WBS_CAPTRUE
        // Zapami©tanie i zmiana stylu w prywatnej strukturze
        pBtn->flState &= ~WBS_DOWN;
        pBtn->flState &= ~WBS_MOUSEIN;
        pBtn->flState |= WBS_UP;
        // Wymuszenie przerysowania
        WinInvalidateRect(hwnd, (PRECTL)NULL, TRUE);
      }
    }
    else // Kursor jest w obr©bie przycisku
    {
      if (!( pBtn->flState & WBS_MOUSEIN )) // Czy nie byˆo wcze˜niej na przycisku mysz?
      {
        // Wysˆanie notyfikacji ¾e kursor wszedˆ na przycisk
        WinSendMsg(WinQueryWindow(hwnd, QW_OWNER),WM_CONTROL,MPFROM2SHORT(pBtn->id,BN_MOUSEENTER),NULL);
        // Zapami©tanie stylu w prywatnej strukturze
        pBtn->flState |= WBS_MOUSEIN;
        // Kiedy jest to FlatButton nale¾y wymusi† przerysowanie
        if ((pBtn->flStyle & 0x07) == BS_FLATBUTTON)
          WinInvalidateRect(hwnd, (PRECTL)NULL, TRUE);
      }

      // Kiedy wchodzimy na przycisk ju¾ z wci˜ni©tym przyciskiem myszy
      if (( pBtn->flState & WBS_UP ) && ( pBtn->flState & WBS_CAPTURE ))
      {
        pBtn->flState &= ~WBS_UP;
        pBtn->flState |= WBS_DOWN | WBS_CAPTURE;
        // Wymuszenie przerysowania
        WinInvalidateRect(hwnd, (PRECTL)NULL, TRUE);
      }
    }
  }
#if 0
  WinSetPointer(HWND_DESKTOP,
                   (HPOINTER)WinSendMsg(WinQueryWindow(hwnd,QW_OWNER),
                                        WM_CONTROLPOINTER,
                                        MPFROMSHORT(pBtn->id),
                                        MPFROMLONG(pBtn->hptrArrow)));
#endif

  // Uruchomienie czasomierza
  WinStartTimer(WinQueryAnchorBlock(hwnd), hwnd, 1, 75);
  return(0);
}

static MRESULT BtnOnChar(HWND hwnd, MPARAM mp1, MPARAM mp2)
{
  PBTN pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0);
  HWND hwndOwner = WinQueryWindow(hwnd, QW_OWNER);

  // Uwaga: styl BS_FLATBUTTON b©dzie reagowaˆ tylko na mnemoniki i
  // na nic wi©cej
  if ( pBtn->flStyle & 0x07 != BS_FLATBUTTON)
    return(WinDefWindowProc(hwnd, WM_CHAR, mp1, mp2));

  if ( SHORT1FROMMP(mp1) & KC_KEYUP )
    if (SHORT2FROMMP(mp2) == VK_SPACE)
    {
      if (SHORT1FROMMP(mp1) & KC_PREVDOWN)
        return 0;
      else
        BtnMouseUp(hwnd,NULL,FALSE);
        return 0;
    }
    else
      return(0);


  // Sprawd«my wirtualne klawisze
  if (SHORT1FROMMP(mp1) & KC_VIRTUALKEY )
    switch (SHORT2FROMMP(mp2))
    {
      case VK_SPACE :
       if (SHORT1FROMMP(mp1) & KC_PREVDOWN)
         return 0;
       else
       {
         BtnMouseDown(hwnd);
         return 0;
        }
      // uwaga VK_NEWLINE to zwykˆy Enter a VK_ENTER to prawy enter!!!
      case VK_NEWLINE :
      case VK_ENTER   :
      if ((pBtn->flStyle & 0x07) != BS_FLATBUTTON)
      {
        WinPostMsg(hwnd,WM_BUTTON1DOWN,NULL,NULL);
        WinPostMsg(hwnd,WM_BUTTON1UP,NULL,NULL);
      }
      return(0);

      case VK_TAB :
        WinSetFocus(HWND_DESKTOP,WinEnumDlgItem(hwndOwner, hwnd,EDI_NEXTTABITEM));
        return(0);

      case VK_RIGHT:
      case VK_DOWN :
        pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0);
        WinSetFocus(HWND_DESKTOP,WinEnumDlgItem(hwndOwner, hwnd,EDI_NEXTGROUPITEM));
        return(0);

      case VK_BACKTAB : // Shift-TAB
        WinSetFocus(HWND_DESKTOP,WinEnumDlgItem(hwndOwner, hwnd,EDI_PREVTABITEM));
        return(0);

      case VK_LEFT :
      case VK_UP :
        WinSetFocus(HWND_DESKTOP,WinEnumDlgItem(hwndOwner, hwnd,EDI_PREVGROUPITEM));
        return(0);
    }
   return(WinDefWindowProc(hwnd, WM_CHAR, mp1, mp2));
}

static MRESULT  BtnOnDestroy(HWND hwnd)
{
  // pobieramy nasz¥ struktur© po raz ostatni ;)
  PBTN pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0);

  // je˜li s¥ mapy bitowe nale¾y je skasowa†
  // ale tylko te kt¢re byˆy tworzone przez kontrolke
  // NIE NALE½Y kasowa† bitmap kt¢re zostaˆy przesˆane poprzez
  // CTLDATA
  if ((pBtn->hbm) && (!(pBtn->ctlData.hColorImage)))
    GpiDeleteBitmap(pBtn->hbm);

  if ((pBtn->hbmDisabled) && (!(pBtn->ctlData.hColorDisImage)))
    GpiDeleteBitmap(pBtn->hbmDisabled);

  free(pBtn->szText);
  pBtn->szText=NULL;

  free((PVOID)pBtn);
  return (0);
}

static MRESULT  BtnOnFocusChange(HWND hwnd, MPARAM mp2)
{
  PBTN pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0);

  pBtn->bFocused=(BOOL)mp2;
  WinInvalidateRect(hwnd, (PRECTL)NULL, TRUE); // przerysujmy przycisk

  return (0);
}

static MRESULT  Btn_OtherProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  PBTN pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0);

  switch ( msg )
  {
    case BM_AUTOSIZE:    // Przycisk b©dzie si© sam ustawiaˆ
      BtnAutoscale(hwnd,NULL,NULL);
      break;

    case BM_CLICK:       // Przycisk wciska si© lub odciska
      if (LONGFROMMP(mp1))
        return(BtnMouseUp(hwnd,NULL,TRUE));
      else
        return(BtnMouseDown(hwnd));

    case BM_QUERYHILITE: // Przycisk zwraca sw¢j stan
      if (((pBtn->flState) & WBS_DOWN))
        return MRFROMLONG(TRUE);
      else
        return MRFROMLONG(FALSE);
     break;

    case BM_SETDEFAULT:    // Przycisk staje si© domy˜lnym
      pBtn->flState = WBS_UP;
      WinInvalidateRect(hwnd, (PRECTL)NULL, TRUE); // wymuszamy przerysowanie
      break;

    case BM_QUERYHEIGHT:
      { ULONG ulHeight;

        BtnAutoscale(hwnd,&ulHeight,NULL);

        return (MPARAM)ulHeight;
      }


    case BM_QUERYWIDTH:
      { ULONG ulWidth;

        BtnAutoscale(hwnd,NULL,&ulWidth);

        return (MPARAM)ulWidth;
      }

    case BM_SETHILITE:     // Przycisk wciska si© lub odciska lecz nie generuje
      if (LONGFROMMP(mp1)) // WM_CONTROL
      {
        pBtn->flState &= ~WBS_DOWN;
        pBtn->flState |= WBS_UP;
      }
      else
      {
        pBtn->flState &= ~WBS_UP;
        pBtn->flState  = WBS_DOWN;
      }
      WinInvalidateRect(hwnd, (PRECTL)NULL, TRUE); // wymuszamy przerysowanie
      break;
  }
  return (0);
}

static LONG BtnGetTextWidth(HWND hwnd, PSZ szText)
{
  POINTL txt[TXTBOX_COUNT]; // Rozmiary tekstu
  HPS    hps;               // Uchwyt pomocniczej presentation space
  LONG   rc;

  // Odczyt pomocniczej presentation space
  hps = WinGetPS(hwnd);

  GpiQueryTextBox(hps, strlen(szText), szText, TXTBOX_COUNT, txt);
  rc = txt[TXTBOX_BOTTOMRIGHT].x - txt[TXTBOX_BOTTOMLEFT].x;

  if (rc < 0) rc = -rc;

  // Zwolnienie pomocniczej presentation space
  hps = WinReleasePS(hps);

  return rc;
}

static LONG BtnGetTextHeight(HWND hwnd, PSZ szText)
{
  POINTL txt[TXTBOX_COUNT]; // Rozmiary tekstu
  HPS    hps;               // Uchwyt pomocniczej presentation space
  LONG   rc;

  // Odczyt pomocniczej presentation space
  hps = WinGetPS(hwnd);

  // Odczyt parametr¢w domy˜lnej czcionki okna
  GpiQueryTextBox(hps, strlen(szText), szText, TXTBOX_COUNT, txt);

  rc= txt[TXTBOX_TOPLEFT].y - txt[TXTBOX_BOTTOMLEFT].y;

  if (rc < 0) rc = -rc;

  // Zwolnienie pomocniczej presentation space
  hps = WinReleasePS(hps);

  return rc;
}


static VOID BtnStoreFont(HWND hwnd, PBTN pBtn)
{
  PSZ    szTemp;            // aäcuch pomocniczy
  PSZ    szNew;             // aäcuch pomocniczy

  // Pomiar dˆugo˜ci tekstu uwzgl©dniaj¥c mnemonici (je˜li istnieje)
  if(pBtn->szText != NULL)
  {
    szNew=malloc(strlen(pBtn->szText)+1);
    *szNew=0; // Na wszelki wypadek wyczy˜†my nasz ˆaäcuch
    // sparwdzamy czy tekst nie jest mnemonic
    szTemp=strstr(pBtn->szText,"~\0");
    if (szTemp)
    {
      if (strlen(szTemp) > 1) // '~' jak jest na samym koäcu to nie powinni˜my go rusza†
      {
        strncpy(szNew, pBtn->szText,szTemp-pBtn->szText); // kopiowanie ˆaäcucha do '~'
        szTemp++; // przesuni©cie pozycji z '~' na nast©pn¥
        strcat(szNew,szTemp);
      }
      else strcpy(szNew,pBtn->szText);
    } else strcpy(szNew,pBtn->szText);

    pBtn->ulTextWidth  = BtnGetTextWidth(hwnd,szNew);
    pBtn->ulTextHeight = BtnGetTextHeight(hwnd,szNew);
  }
  free(szNew);
}

// Zapami©tanie map bitowych w wewn©trznych strukturach okna, odczyt
// ich dˆugo˜ci i szeroko˜ci
static VOID BtnStoreWindowBitmaps(HWND hwnd, HMODULE hmod, ULONG idEnabled, ULONG idDisabled, ULONG idPressed, ULONG idMouseIn)
{
  PBTN             pBtn; // Wewn©trzna struktura informacyjna
  HPS              hps;  // Uchwyt do pomocniczego presentation space

  hps  = WinGetPS(HWND_DESKTOP);
  pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0); // znajdujemy nasz¥ struktur©

  // Sprawd«my czy istniej¥ jakie˜ bitmapy w naszej strukturze, je˜li tak to
  // nale¾y je skasowa†


/////
/////  UWAGA: Nie nale¾y kasowa† bitmap kt¢re zostaˆy pzresˆane jako uchwyty!!!
/////
///// Maski nie kasujemy poniewa¾ jest ona podawana tylko poprzez ctldata
/////
  if (pBtn->hbm)
  {
    if (pBtn->hbm != pBtn->ctlData.hColorImage)
      GpiDeleteBitmap(pBtn->hbm);

    pBtn->hbm=NULLHANDLE;
  }

  if (pBtn->hbmDisabled)
  {
    if (pBtn->hbmDisabled != pBtn->ctlData.hColorDisImage)
      GpiDeleteBitmap(pBtn->hbmDisabled);

    pBtn->hbmDisabled = NULLHANDLE;
  }

  if (pBtn->hbmPressed)
  {
    if (pBtn->hbmPressed != pBtn->ctlData.hColorPressImage)
      GpiDeleteBitmap(pBtn->hbmDisabled);

    pBtn->hbmPressed = NULLHANDLE;
  }

  if (pBtn->hbmMouseIn)
  {
    GpiDeleteBitmap(pBtn->hbmMouseIn);
    if (pBtn->hbmPressed != pBtn->ctlData.hColorHiliteImage)
      GpiDeleteBitmap(pBtn->hbmMouseIn);


    pBtn->hbmMouseIn = NULLHANDLE;
  }

  // Odczytujemy bitmapy
  pBtn->hbm = GpiLoadBitmap(hps, hmod, idEnabled , 0L, 0L);

  // Mo¾e nie znajdowa† si© bitmapa nieaktywnego stanu
  // lecz zawsze j¥ powinno stworzy†
  if (idDisabled)
    pBtn->hbmDisabled = GpiLoadBitmap(hps, hmod, idDisabled, 0L, 0L);
  else
    pBtn->hbmDisabled = BtnCreateMonochrome(hwnd, pBtn->hbm);

  if (idPressed)
    pBtn->hbmPressed = GpiLoadBitmap(hps, hmod, idPressed, 0L, 0L);
  else
    pBtn->hbmPressed = NULLHANDLE;

  if (idMouseIn)
    pBtn->hbmMouseIn = GpiLoadBitmap(hps, hmod, idMouseIn, 0L, 0L);
  else
    pBtn->hbmMouseIn = NULLHANDLE;

  // Zwolnienie pomocniczej presentation space
  BtnQueryBitmaps(hps,pBtn);
  WinReleasePS(hps);
}

static VOID BtnQueryBitmaps(HPS hps,PBTN pBtn)
{
  BITMAPINFOHEADER bmpi1; // Informacyjna struktura o bitmapie
  BITMAPINFOHEADER bmpi2; // Informacyjna struktura o bitmapie

  memset(&bmpi1,0,sizeof(bmpi1));
  memset(&bmpi2,0,sizeof(bmpi2));

  if (pBtn->hbm)
  {
    GpiQueryBitmapParameters(pBtn->hbm, &bmpi1);
    GpiQueryBitmapParameters(pBtn->hbmDisabled, &bmpi2);

    pBtn->bmpcx=(bmpi1.cx > bmpi2.cx) ? bmpi1.cx : bmpi2.cx;
    pBtn->bmpcy=(bmpi1.cy > bmpi2.cy) ? bmpi1.cy : bmpi2.cy;
  }
  else
  {
    pBtn->bmpcx=0;
    pBtn->bmpcy=0;
  }
}

// Zapami©tanie tekstu w wewn©trznych strukturach okna, odczyt
// dˆugo˜ci tekstu w pikselach, dla czcionki u¾ytej w obr©bie okna
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna
//   pBtn      - [parametr/rezultat] struktury kontrolne okna
//   text      - [parametr] wska«nik do zapamietywanego tekstu
//
static VOID BtnStoreWindowText(HWND hwnd, PBTN  pBtn, PSZ text)
{
  // Zapami©tanie tekstu wy˜wietlanego w oknie
  if(text != NULL)
  {
    if(pBtn->szText == NULL)
      pBtn->szText=malloc(strlen(text)+1);
    else
      pBtn->szText=realloc(pBtn->szText,strlen(text)+1);

    if(pBtn->szText != NULL)
      strcpy(pBtn->szText, text);
  }
  else // Gdy przycisk jest pusty nale¾y pami©† zwolni†
   if (pBtn->szText != NULL)
   {
      free(pBtn->szText);
      pBtn->szText=NULL;
   }

  BtnStoreFont(hwnd,pBtn);
}

// Automatyczne skalowanie okna - dostosowaie do wymiar¢w tekstu i bitmapy
// lub te¾ podanie wˆa˜ciwych wymiar¢w
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna przycisku
//   pulHeight - [parametr/powr¢t] wska«nik do zmiennej pytaj¥cej o minimaln¥
//                                 wysoko˜† okna
//   pulWeight - [parametr/powr¢t] wska«nik do zmiennej pytaj¥cej o minimaln¥
//                                 szeroko˜† okna
//
// Powr¢t:
//   Je˜li dwa ostatnie parametry s¥ puste to okno zostaje przeskalowane
//   i od˜wierzone z nowymi wymiarami
//   Je˜li nie podane zostan¥ tylko wymiary minimalne
//
static VOID BtnAutoscale(HWND hwnd, ULONG *pulHeight, ULONG *pulWidth)
{
  PBTN  pBtn;                  // Wska«nik wewn©trznej struktury okna
  ULONG cx=2*BTN_BORDERWIDTH;  // Najmniejsza dˆugo˜† okna
  ULONG cy=2*BTN_BORDERHEIGHT; // Najmniejsza szeroko˜†

  // Odczyt wska«nika
  pBtn = (PBTN)WinQueryWindowPtr(hwnd, 0);

  // Gdy jest tekst powi©kszanie dˆugo˜ci i szeroko˜ci okna
  // o dˆugo˜† i szeroko˜† tekstu
  if (pBtn->szText && strlen(pBtn->szText)>0)
  {
    cx+= pBtn->ulTextWidth;
    cy+= pBtn->ulTextHeight;
  }

  if ((pBtn->flStyle & BS_BITMAP) && (pBtn->hbm))
  {
    if (((pBtn->flStyle & 0x30) == BS_ALIGNTOP) ||
       ((pBtn->flStyle & 0x30) == BS_ALIGNBOTTOM))
    {
      if ((pBtn->szText) && (strlen(pBtn->szText)>0))
        cy += pBtn->bmpcy + (pBtn->fm.lMaxBaselineExt >> 2);
      else
        cy += pBtn->bmpcy;

      // je¾eli wysoko˜† bitmapy jest wi©ksza od wysoko˜ci okna
      if (pBtn->bmpcx > cx)
        cx += pBtn->bmpcx;
    }
    else
    {
      if ((pBtn->szText) && (strlen(pBtn->szText)>0))
        cx += pBtn->bmpcx + (pBtn->fm.lMaxCharInc >> 1);
      else
        cx += pBtn->bmpcx;
      // je¾eli wysoko˜† bitmapy jest wi©ksza od wysoko˜ci okna
      if (pBtn->bmpcy > cy)
        cy += pBtn->bmpcy;
    }

  }

  // Je˜li dwa ostatnie parametry s¥ puste to okno zostaje przeskalowane
  // i od˜wierzone z nowymi wymiarami
  if (pulHeight==NULL || pulWidth==NULL)
  {
    WinSetWindowPos(hwnd, HWND_TOP, 0, 0,
                    cx,
                    cy,
                    SWP_SIZE);
  }
  else
  {
    if ( pulHeight!=NULL )
      *pulHeight=cy;

    if ( pulWidth!=NULL )
      *pulWidth=cx;
  }

  BtnSizeButton(pBtn,0,0,cx,cy);
}

// Aktualizacja wybranego parametru prezentacji
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna
//   mp1       - [parametr] parametr prezentacji
//
static MRESULT  BtnOnPresParamChanged (HWND hwnd, MPARAM mp1)
{
  PBTN  pBtn;     // Wska«nik do struktur kontrolnych okna

  // Odczyt adresu struktur kontrolnych okna
  pBtn = WinQueryWindowPtr(hwnd, 0L);

  if (pBtn == NULL) return (0);

  // Sprawdzenie czy zmianie ulegˆa czcionka
  // lub kilka parametr¢w na raz - ppm = 0
  if ((LONGFROMMP(mp1) == PP_FONTNAMESIZE) || (LONGFROMMP(mp1) == 0))
  { // Auaktualnienie struktur okna
    BtnStoreFont(hwnd, pBtn);

    // Wymuszenie przeskalowania lub przerysowania
    if(pBtn->flStyle & BS_AUTOSIZE)
      BtnAutoscale(hwnd,NULL,NULL);
    else
      { RECTL rcl;

        WinQueryWindowRect(hwnd, &rcl);
        BtnSizeButton(pBtn,rcl.xLeft,rcl.yBottom,(rcl.xRight - rcl.xLeft),(rcl.yTop - rcl.yBottom));
      }

    // Wymuszenie przerysowania
    WinInvalidateRect(hwnd, NULL, FALSE);
  }

  // Sprawdzenie czy zmianie ulegˆy kolory
  // lub kilka parametr¢w na raz - ppm = 0
  if ((LONGFROMMP(mp1) != PP_FONTNAMESIZE) || (LONGFROMMP(mp1) == 0))
  { // Zmianie ulegˆy kolory
    PpmQueryPresColors(hwnd, BTN_MAXCOLOR, G_PresParams, (LONG *)pBtn->ulColors);
    // Wymuszenie przerysowania
    WinInvalidateRect(hwnd, NULL, FALSE);
  }

  // Sprawdzenie czy komunikat WM_PRESPARAMCHANGED zostaˆ wysˆany
  // przez inny w¥tek - systemow¥ palet© czcionek lub kolor¢w
  if((LONGFROMMP(mp1) == PP_FONTNAMESIZE) ||
     (LONGFROMMP(mp1) == PP_BACKGROUNDCOLOR) ||
     (LONGFROMMP(mp1) == PP_FOREGROUNDCOLOR))
     { HWND owner;     // Uchwyt okna wˆa˜ciciela
       LONG winid;     // Identyfikator bie¾¥cego okna
       HAB  hab;       // Uchwyt anchor block

       // Odczyt uchwytu anchor block
       hab = WinQueryAnchorBlock(hwnd);
       // Sprawdzenie czy zmiana parametru jest inicjowana przez inny w¥tek
       if(WinInSendMsg(hab) == TRUE)
       { // Sprawdzenie czy parametr istnieje w obr©bie okna, czy te¾ jest dziedziczony
         if(WinQueryPresParam(hwnd,LONGFROMMP(mp1), 0, NULL, sizeof(ULONG), &winid, QPF_NOINHERIT))
         { // Odczyt uchwytu wˆa˜ciciela
           owner = WinQueryWindow(hwnd, QW_OWNER);

           // Odczyt identyfikatora okna
           winid = WinQueryWindowUShort(hwnd, QWS_ID);

           // Wstawienie potwierdzenia do wˆa˜ciciela okna
           if(owner != NULLHANDLE)
             WinPostMsg(owner, WM_CONTROL,MPFROM2SHORT(winid, BN_PRESPARAMCHANGED),mp1);
        }
      }
    }

  return (0);
}

static VOID BtnSetCtlData(HWND hwnd, PBTN pBtn, PVOID pCtlData)
{
  PWBTNCDATA pBtnCData  = (PWBTNCDATA)pCtlData;

  if (pBtnCData == NULL) return; // je˜li nie ma CTLDATA

  if (pBtnCData->cb == 0) return; // Je˜li dˆugo˜† CTLDATA jest r¢wna 0

  // Set no mask
  pBtn->hbmMask         = NULLHANDLE;
  pBtn->hbmMaskDisabled = NULLHANDLE;

  // copy control structure for next usage
  memcpy(&pBtn->ctlData, pBtnCData,(pBtnCData->cb > sizeof(WBTNCDATA)) ? sizeof(WBTNCDATA) : pBtnCData->cb);

  if (pBtn->ctlData.cb > sizeof(WBTNCDATA))
  {
    pBtn->ctlData.cb = sizeof(WBTNCDATA);
  }

  if (pBtn->ctlData.fsHiliteState && (pBtn->ctlData.cb > 4))
  {
    pBtn->flState = WBS_DOWN;
  }
  else
  {
    pBtn->flState = WBS_UP;
  }

  // User want to use internal icons and control is a button with BS_BITMAP
  if ((pBtn->ctlData.idStdBitmap) && (pBtn->flStyle & BS_BITMAP) && (pBtn->ctlData.cb>2))
  {
    // switch of bitmap
    switch (pBtn->ctlData.idStdBitmap)
    {
      // bitmap associated with 'OK' text
      case BBMP_OK:
        BtnStoreWindowBitmaps(hwnd, hResource, 2000, 0, 0, 0);
        break;

      // bitmap associated with 'Cancel' text
      case BBMP_CANCEL:
        BtnStoreWindowBitmaps(hwnd, hResource, 2010, 0, 0, 0);
        break;

      // bitmap associated with 'Help' text
      case BBMP_HELP:   // Je˜li jest to zestaw bitmap "Help"
        BtnStoreWindowBitmaps(hwnd, hResource, 2020, 0, 0, 0);
        break;
    }
  }
  else
  // User wants custom bitmaps and masks
  if (pBtn->ctlData.hColorImage && (pBtn->ctlData.cb > 6))
  {
    // Set handle from user to private structure
    pBtn->hbm=pBtn->ctlData.hColorImage;

    // Set handle from user to private structure
    if (pBtn->ctlData.hColorDisImage && (pBtn->ctlData.cb > 10))
      pBtn->hbmDisabled=pBtn->ctlData.hColorDisImage;

    // Set handle from user to private structure
    if (pBtn->ctlData.hColorImage && (pBtn->ctlData.cb > 14))
      pBtn->hbmMask=pBtn->ctlData.hImage;

    // Set handle from user to private structure
    if (pBtn->ctlData.hColorDisImage && (pBtn->ctlData.cb > 18))
      pBtn->hbmMaskDisabled=pBtn->ctlData.hImageDis;

    BtnQueryBitmaps(WinGetPS(hwnd),pBtn);
  }

}

static VOID BtnDrawText(HWND hwnd, PBTN pBtn)
{
  RECTL rcl;
  HPS   hps;
  PSZ   szTempText = NULL;
  LONG  lColor;
  LONG  lPointsWidth;

  hps = WinGetPS(hwnd);

  // We use RGB not indexed colours
  GpiCreateLogColorTable(hps, 0L, LCOLF_CONSECRGB, 0, BTN_MAXCOLOR, (LONG *)pBtn->ulColors);

  // If text is too long for button set text to '...'
  if (pBtn->flStyle & BS_AUTOTEXT)
  {
    if (pBtn->ulTextWidth > (pBtn->rclText.xRight - pBtn->rclText.xLeft))  // Text si© nie mie˜ci
    {
      // Rezerwujemy pami©† dla tekstu
      szTempText = (PSZ) strdup(pBtn->szText);
      lPointsWidth = BtnGetTextWidth(hwnd, "...");
      szTempText[strlen(szTempText) - 1] = 0;
      while ((BtnGetTextWidth(hwnd, szTempText) + lPointsWidth) > (pBtn->rclText.xRight - pBtn->rclText.xLeft))
      {
        szTempText[strlen(szTempText) - 1] = 0;
      }
      strcat(szTempText, "...");
    }
  }

  // Draw focus line
  if (((pBtn->bFocused)
      && (!(pBtn->flStyle & BS_NOPOINTERFOCUS)))
      || ((pBtn->flStyle & 0x07) == BS_FLATBUTTON))
  { POINTL ptl;

    GpiSetColor(hps, BTN_DEFBORDER);

    if ((pBtn->flStyle & 0x07) != BS_FLATBUTTON)
    {
       BtnDrawBox(hps, 4, 4, pBtn->rcl.xRight - 5, pBtn->rcl.yTop - 5);
    }
    // Flat button
    else 
    if ((((pBtn->flStyle & 0x07) == BS_FLATBUTTON)
       && (pBtn->flState & WBS_MOUSEIN)
       && (!(pBtn->flStyle & BS_THINFRAME))
       && (!(pBtn->flStyle & WS_DISABLED)))
       || (pBtn->bFocused))
    {
      GpiSetColor(hps, BTN_DEFBORDER);
      rcl=pBtn->rcl;
      ptl.x=rcl.xLeft; ptl.y=rcl.yBottom;
      GpiMove(hps,&ptl);
      ptl.x=rcl.xLeft; ptl.y=rcl.yTop-1;
      GpiLine(hps,&ptl);
      ptl.x=rcl.xRight-1; ptl.y=rcl.yTop-1;
      GpiLine(hps,&ptl);
      ptl.x=rcl.xRight-1; ptl.y=rcl.yBottom;
      GpiLine(hps,&ptl);
      ptl.x=rcl.xLeft; ptl.y=rcl.yBottom;
      GpiLine(hps,&ptl);
    }
  }

  if (pBtn->flStyle & WS_DISABLED)
  {
    GpiSetColor(hps, BTN_LIGHTBORDER);

    rcl = pBtn->rclText;
    // Obliczmy idealne poˆo¾enie tekstu cienia
    rcl.xLeft++;
    rcl.yTop --;
    rcl.yBottom -=2;
    rcl.xRight++;
    WinDrawText(hps, -1, pBtn->szText, &rcl, BTN_LIGHTBORDER, CLR_BACKGROUND, DT_CENTER | DT_VCENTER | DT_MNEMONIC);
    lColor = BTN_DISABLEDFG;;
  }
  else
    lColor = BTN_FOREGROUND;

  rcl = pBtn->rclText;
  rcl.yTop --;

  if (pBtn->flState & WBS_DOWN)
  {
    rcl.xLeft++;
    rcl.yBottom -=2;
    rcl.xRight++;
  }

  if (((pBtn->flStyle & 0x07) == BS_FLATBUTTON) && (pBtn->flState & WBS_MOUSEIN) && (!(pBtn->flStyle & WS_DISABLED)))
    lColor = BTN_TEXTFLAT;

  // Gdy tekst jest skr¢cony wy˜wietlamy go a je˜li nie to nie ;-)
  if ((szTempText != NULL) && (pBtn->flStyle & BS_AUTOTEXT))
    WinDrawText(hps, -1, szTempText, &rcl, lColor, CLR_BACKGROUND, DT_CENTER | DT_VCENTER | DT_MNEMONIC);
  else
    WinDrawText(hps, -1, pBtn->szText, &rcl, lColor, CLR_BACKGROUND, DT_CENTER | DT_VCENTER | DT_MNEMONIC);

  if (szTempText != NULL)
    free(szTempText);

  WinReleasePS(hps);
}

static MRESULT BtnDblClick(HWND hwnd, PBTN pBtn, POINTL ptl)
{
  pBtn->flState &= ~WBS_DOWN;
  pBtn->flState &= ~WBS_CAPTURE;
  pBtn->flState |= WBS_UP;

  WinSetCapture(HWND_DESKTOP, (HWND)NULL);

  if ((pBtn->flStyle & 0x07) != BS_FLATBUTTON)
    WinFocusChange(HWND_DESKTOP,hwnd,0);

  pBtn->flState &= ~WBS_UP;
  pBtn->flState |= WBS_DOWN | WBS_CAPTURE; // zmieniamy stan...
  WinSetCapture(HWND_DESKTOP, hwnd);
  // Ponowne przerysowanie przycisku
  WinInvalidateRect(hwnd, NULL, TRUE); // trzeba przerysowa† przycisk

  // Trzeba wysˆa† notyfikacje o dwumlasku!
  WinSendMsg(WinQueryWindow(hwnd, QW_OWNER),WM_CONTROL,MPFROM2SHORT(pBtn->id,BN_DBLCLICKED),NULL);
  return (MRFROMLONG(TRUE));

}

/* ==========================================================================
 *
 * $Log: WINBUTT.C $
 * Revision 2.1  1999/08/14 13:55:35  Przemyslaw_Dobrowolski
 * BM_QUERY* nie zwracal nic :))))
 *
 * Revision 2.0  1999/08/14 13:10:23  Przemyslaw_Dobrowolski
 * Wszystko dziala:
 * - Flatbutton z flaga BS_THINFRAME jest powiekszany o piksela
 * - BM_QUERYHEIGHT i BM_QUERYWIDTH
 * Przypisane zostalo cale Btn_WM_PAINT, BtnDrawText, BtnSizeBtn
 * i wiele, wiele wiecej malych bugow...
 *
 * Revision 1.10  1999/06/08 08:24:39  Przemyslaw_Dobrowolski
 * - Poprawnie dziaˆa dwumlask myszy
 * - Przycisk przy przycisnieciu nie aktywowaˆ rodzica. Poprawiono.
 *
 * Revision 1.9  1999/06/07 20:20:29  Przemyslaw_Dobrowolski
 * dziala klikniecie tak jak powinno dzialac we flatbuttonie
 * zminilem timer na wiekszy aby nie bylo "echa"...
 *
 * Revision 1.8  1999/05/19 19:55:05  Przemyslaw_Dobrowolski
 * Dodane BN_BUTTON2CLICKED
 *
 * Revision 1.7  1999/04/25 20:18:09  Przemyslaw_Dobrowolski
 * Dodane:
 * #define BS_AUTOTEXT    0x0400    // Gdy tekst si© nie mie˜ci wpisujemy tyle ile si© zmie˜ci a dalej wpisujemy '...'
 * Poprawione:
 * - WM_CHAR - ju¾ nie ma problemu z ENTER'em. UWAGA: Styl bs_flatbutton nie reaguje na wi©kszo˜† komunikat¢w klawiatury.
 * - Dziaˆaj¥ mnemoniki
 * - Mimo tego ¾e co˜ si© nie mie˜ci (nie dla stylu BS_AUTOSIZE) jest wy˜wietlane nieprawidˆowo, a nie tak jak we wcze˜niejszej wersji ¾e nic si© nie pojawiaˆo..
 *
 * Revision 1.6  1999/04/16 13:26:57  Przemyslaw_Dobrowolski
 * Gdy wciskaˆo si© Enter przycisk si© nie odciskaˆ.
 * Poprawiono.
 *
 * Revision 1.5  1999/04/16 10:59:08  Przemyslaw_Dobrowolski
 * Poprawiono bardzo wiele bˆ©d¢w.
 * Dodano:
 * Funkcje konwertuj¥c¥ bitmap© do odcieni szaro˜ci
 * Teraz gdy nie poda si© bitmapy dla stanu zablokowanego jest ona
 * samoczynnie tworzona
 *
 * Revision 1.4  1999/03/18 12:44:26  Przemyslaw_Dobrowolski
 * Przez pomyˆk© istniaˆ domy˜lny font 8.Helv
 *
 * Revision 1.3  1999/03/18 12:42:26  Przemyslaw_Dobrowolski
 * Powinno wszystko dziaˆa†:
 * Dodano: BS_OWNERBUTTON
 * Poprawiono: Autoscalowanie i wy˜wietlanie wci˜ni©tej ikony
 * Dodano: Obsˆug© WinSetPresParams
 * Dodano obsˆug© bitmap z zewn¥trz.
 *
 * Revision 1.2  1999/03/17 20:58:52  Przemyslaw_Dobrowolski
 * Poprawiono bˆ¥d w rozpoznawaniu BS_xxxxxxx
 * Dodano Btn_OtherProc do gˆ¢wnej funkcji.
 *
 */