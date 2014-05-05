/* OS/2 Commander - moduˆy pomocnicze
 * Wspomaganie zarz¥dzaniem czcionkami
 * (c) 1999 Wojciech Gazda
 *
 * fonts.c
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/01/31 15:13:15 $
 * $Name:  $
 * $RCSfile: fonts.c $
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


// Deklaracje OS/2
#define INCL_DEV
#define INCL_GPILCIDS
#define INCL_GPIPRIMITIVES
#define INCL_GPITRANSFORMS
#include <os2.h>

// Funkcje biblioteczne
#include <string.h>
#include <malloc.h>
#include <stdio.h>

// Deklarace lokalne
#include "fonts.h"



// Prototypy funkcji
LONG ASUAAPI FntCreateRasterFont(HPS hps, PSZ szNameSize, LONG lLcid, FONTMETRICS *fm);





// Funkcja tworzy czcionk© w przestrzeni roboczej hps,
// preferuj¥c przy tym czcionki rastrowe.
// Rozmiar czcionki jet podawany w punktach.
// Za po˜rednictwem wska«nika pfm mo¾na odczyta† parametry utworzonej czcionki,
// je˜li wska«nik ten jest r¢wny NULL - parametry nie s¥ odczytywane
//
// Parametry:
//   hps            - [parametr] uchwyt presentation space
//   szNameSize     - [parametr] rozmiar i nazwa czcionki, np. "10.Helv"
//   lLcid          - [parametr] numer identyfikacyjny tworzonej czcionki
//   pfm            - [rezultat] parametry utworzonej czcionki
//
// Powr¢t:
//   0                   - poprawne wykonanie funkcji
//   ERR_FNT_INVPARAM    - bˆedna warto˜† parametru
//   ERR_FNT_HDC         - bˆad odczytu kontekstu urz¥dzenia
//   ERR_FNT_CREATE      - bˆ¥d tworzenia czcionki
//
LONG ASUAAPI FntCreateRasterFont(HPS hps, PSZ szNameSize, LONG lLcid, FONTMETRICS *pfm)
{ FATTRS  fattrs;        // Parametry niezb©dne do utworzenia czcionki
  POINTL  res;           // Pozioma i pionowa rozdzielczo˜† urz¥dzenia
  HDC     hdc;           // Kontekst urz¥dzenia
  PSZ     facename;      // Nazwa czcionki
  LONG    psize;         // Rozmiar w punktach
  LONG    fcount;        // Liczba dost©pnych czcionek
  LONG i, tmp;           // Zmienne pomocnicze

  // Odczyt rozmiaru czcionki
  if(sscanf(szNameSize, "%d.", &psize) != 1)
    return(ERR_FNT_INVPARAM);
  // Ustawienie wska«nika do nazwy czcionki
  facename = strchr(szNameSize, '.');
  if(facename == NULL)
    return(ERR_FNT_INVPARAM);

  // Odczyt rozdzielczo˜ci urz¥dzenia
  hdc = GpiQueryDevice(hps);
  if(hdc == NULLHANDLE) return(ERR_FNT_HDC);
  DevQueryCaps(hdc, CAPS_HORIZONTAL_FONT_RES, 1, &res.x);
  DevQueryCaps(hdc, CAPS_VERTICAL_FONT_RES,   1, &res.y);

  // Inicjacja zmiennych
  facename ++;
  memset(&fattrs, 0, sizeof(FATTRS));
  fattrs.usRecordLength = sizeof(FATTRS);
  strncpy(&fattrs.szFacename, facename, FACESIZE - 1);

  tmp = 0;
  // Odczyt liczby czcionek o podanej nazwie zainstalowanych w systemie
  fcount = GpiQueryFonts(hps, QF_PUBLIC, facename, &tmp, (LONG) sizeof(FONTMETRICS), NULL);

  // Odczyt parametr¢w dost©pnych czcionek
  if(fcount)
  { FONTMETRICS *fm;          // Odczytane parametry czcionki

    // Alokacja pami©ci dla bufora
    fm = malloc(fcount * sizeof(FONTMETRICS));
    if(fm == NULL) return(ERR_FNT_OUTMEM);

    // Odczyt parametr¢w czcionek
    GpiQueryFonts(hps, QF_PUBLIC, facename, &fcount, (LONG) sizeof(FONTMETRICS), fm);

    // Poszukiwanie czcionki o podanych wymiarach
    for(i = 0; i < fcount; ++i)
    { // Sprawdzenie rozmiaru
      if(fm[i].sNominalPointSize == (psize * 10))
        // Sprawdzenie rozdzielczo˜ci urz¥dzenia
        if((fm[i].sXDeviceRes == res.x) && (fm[i].sYDeviceRes == res.y))
          // Sprawdzenie typu czcionki
          if(!(fm[i].fsDefn & FM_DEFN_OUTLINE))
          { fattrs.lAveCharWidth   = fm[i].lAveCharWidth;
            fattrs.lMaxBaselineExt = fm[i].lMaxBaselineExt;
          }
    }

    // Zwolnienie pami©ci
    free(fm);
  }

  // Tworzenie czcionki
  tmp = GpiCreateLogFont(hps, NULL, lLcid, &fattrs);
  // Bˆ¥d tworzenia czcionki
  if(tmp == GPI_ERROR) return(ERR_FNT_CREATE);
  // Wybranie czcionki
  if(tmp == FONT_MATCH) GpiSetCharSet(hps, lLcid);

  // Sprawdzenie czy czcionka wektorowa i ustalenie jej rozmiar¢w
  if(!fattrs.lAveCharWidth || !fattrs.lMaxBaselineExt)
  { POINTL size[2];      // Obliczone rozmiary znaku
    SIZEF  box;          // Rozmiar czcionki

    size[0].x = 0; size[0].y = 0;
    size[1].x = (res.x * psize + 36) / 72;
    size[1].y = (res.y * psize + 36) / 72;
    // Przeksztaˆcenie wsp¢ˆrz©dnych
    GpiConvert(hps, CVTC_DEVICE, CVTC_WORLD, 2L, size);

    // Ustalenie rozmiaru czcionki
    box.cx = (size[0].x - size[1].x) << 16;
    box.cy = (size[0].y - size[1].y) << 16;
    GpiSetCharBox(hps, &box);
  }

  // Odczyt struktury FONTMETRICS
  if(pfm != NULL)
    GpiQueryFontMetrics(hps, sizeof(FONTMETRICS), pfm);

  return(0);
}

/*
 * $Log: fonts.c $
 * Revision 1.1  1999/01/31 15:13:15  Wojciech_Gazda
 * Initial revision
 *
 */
