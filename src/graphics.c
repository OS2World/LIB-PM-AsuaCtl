/* Biblioteka okien kontrolnych - Asua Controls
 * Pomocnicze funkcje graficzne.
 * (c) 1999 Wojciech Gazda & Przemyslaw Dobrowolski
 *
 * graphics.c
 *
 * $Author: $
 * $Date:  $
 * $Name:  $
 * $RCSfile:  $
 * $Revision: $
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


// Deklaracje OS/2
#define  INCL_GPIBITMAPS
#define INCL_GPIPRIMITIVES
#include <os2.h>

// Funkcje biblioteczne

// Deklarace lokalne
#include "graphics.h"



// Prototypy funkcji
LONG ASUAAPI GrfDrawBitmap(HPS hps, HBITMAP hbmColor, HBITMAP hbmMono, POINTL *pos, BOOL flPattern);






// Rysowanie mapy bitowej z przezroczystym tˆem.
// Rozmiary bitmapy odczytywane s¥ automatycznie.
//
// Parametry:
//   hps       - [parametr] presentation space
//   hbmColor  - [parametr] uchwyt kolorowej mapy bitowej
//   hbmMono   - [parametr] uchwyt maski bitowej, wycinaj¥cej tˆo
//   pos       - [parametr] poˆo¾enie i rozmiary modyfikowanego obszaru
//   flPattern - [parametr] znacznik naˆo¾enia rastru: TRUE - nakˆadany jest raster
//
// Powr¢t:
//    0 - Poprawne wykonanie funkcji
//  <>0 - Bˆ¥d
//
LONG ASUAAPI GrfDrawBitmap(HPS hps, HBITMAP hbmColor, HBITMAP hbmMono, POINTL *pos, BOOL flPattern)
{ BITMAPINFOHEADER2 *bfh;     // Wska«nik do pocz¥tku struktury informacyjnej
  POINTL bmpos[4];            // Pozycja mapy bitowej (wsp¢ˆrz©dzne dla GpiWCBitBlt)
  UCHAR  tmp[16];             // Bufor na pocz¥tkow¥ cz©˜† struktury BITMAPINFOHEADER2
  ULONG  ulMode;              // Tryb rysowania kolorowej mapy bitowej

  if(hbmColor == NULLHANDLE) return(1);
  // Inicjacja zmiennych
  bfh = (BITMAPINFOHEADER2 *)tmp;


  if(hbmMono != NULLHANDLE)
  { // Odczyt wymiar¢w mapy bitowej
    bfh->cbFix = 16;
    if(GpiQueryBitmapInfoHeader(hbmMono, bfh) == FALSE) return(1);

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

    // Ustawienie atrybut¢w - koloru tˆa i punktu
    GpiSetColor(hps, CLR_WHITE);
    GpiSetBackColor(hps, CLR_BLACK);

    // Ustalenie trybu kopiowania maski (czy ma by† nakˆadany raster)
    if(flPattern == TRUE)
      ulMode = 0x8A;
    else ulMode = ROP_SRCAND;

    // Kopiowanie podkˆadowej mapy bitowej
    GpiWCBitBlt(hps, hbmMono, 4L, bmpos, ulMode, 0L);
  }

  // Odczyt wymiar¢w kolorowej mapy bitowej
  bfh->cbFix = 16;
  if(GpiQueryBitmapInfoHeader(hbmColor, bfh) == FALSE) return(1);

  // Ponowne obliczenie wsp¢ˆrz©dnych kopiowanych map bitowych
  // (GpiWCBitBlt modyfikuje wsp¢ˆrz©dne)
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

  // Okre˜lenie trybu nakˆadania mapy bitowej
  if(hbmMono == NULLHANDLE)
  { // Rysowanie tylko mapy kolorowej
    if(flPattern == TRUE)
    { // Uwzgl©dnienie rastra
      ulMode = 0xCA;
    }
    else ulMode = ROP_SRCCOPY;
  }
  else
  { // Rysowanie bitmapy z naˆo¾on¥ przezroczysto˜ci¥
    if(flPattern == TRUE)
    { // Uwzgl©dnienie rastra
      ulMode = 0xEA;
    }
    else ulMode = ROP_SRCPAINT;
  }

  // Rysowanie kolorowej mapy bitowej
  GpiWCBitBlt(hps, hbmColor, 4L, bmpos, ulMode, 0L);
  return(0);
}

/*
 * $Log: $
 *
 */
