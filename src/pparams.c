/* OS/2 Commander - moduˆy pomocnicze
 * obsˆuga Presentation parameters
 * (c) 1999 Wojciech Gazda
 *
 * pparams.c
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/06/27 12:52:41 $
 * $Name:  $
 * $RCSfile: pparams.c $
 * $Revision: 1.2 $
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
#define INCL_WINSYS
#include <os2.h>

// Funkcje biblioteczne
#include <string.h>

// Deklaracje lokalne
#include "pparams.h"


// Prototypy funkcji
LONG ASUAAPI PpmQueryPresColors(HWND hwnd, LONG nColors, PPTEMPLATE *ppm, LONG *lColTable);
LONG ASUAAPI PpmStorePresParams(HWND hwnd, PRESPARAMS *ppm, PPTEMPLATE *ppt, LONG nColors);




// Funkcja odczytuje kolory zapisane w presentation parameters okna i umieszcza
// odpowiednie warto˜ci RGB w tablicy lColTable. Gdy dany parametr nie istnieje,
// odczytywany jest kolor systemowy SYSCLR_*, gdy ten jest zadeklarowany jako 0
// odczytywana jest warto˜† podana w tablicy ppm. Tablica ta zawiera list©
// prsentation parameters, kolor¢w systemowych i kolor¢w zaszytych na staˆe.
// Mo¾na te¾ dla wybranych kolor¢w u¾y† flag QPF_* (na przykˆad QPF_NOINHERIT).
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna dla kt¢rego odczytywane s¥ parametry
//   nColors   - [parametr] liczba odczytywanych kolor¢w
//   ppm       - [parametr] tablica okre˜laj¥ca kt¢re parametry i kolory maj¥ by† czytane
//   lColTable - [rezultat] tablica warto˜ci RGB odpowiadaj¥cych poszczeg¢lnym kolorom
//
// Powr¢t:
//   Obecnie, zawsze 0
//
LONG ASUAAPI PpmQueryPresColors(HWND hwnd, LONG nColors, PPTEMPLATE *ppm, LONG *lColTable)
{ LONG i;      // Zmienne pomocnicze

  // Odczyt kolejnych presentation parameters
  for(i = 0; i < nColors; ++i)
  { if(!WinQueryPresParam(hwnd, ppm[i].ulPresParam, 0L, NULL, sizeof(LONG), lColTable + i, ppm[i].ulPresFlags))
    { // Parametru nie udaˆo si© odczyta† - sprawdzenie czy
      // ma by† odczytany kolor systemowy
      if(ppm[i].ulSysColor)
      { // Odczyt koloru systemowego
        lColTable[i] = WinQuerySysColor(HWND_DESKTOP, ppm[i].ulSysColor, 0L);
      }
      else
      { // Odczyt koloru zaszytego na staˆe
        lColTable[i] = (LONG)ppm[i].ulRgbColor;
      }
    }
  }

  // Poprawne wykonanie funkcji
  return(0);
}





// Funkcja analizuje parametry przekazane za po˜rednictwem struktury ppm
// (np. podczas obsˆugi WM_CREATE) i umieszcza je w strukturach okna.
// Uwzgl©dniane s¥ tylko parametry uwzgl©dnione na li˜cie ppt. Gdy wska«nik
// ppt jest r¢wny NULL - uwzgl©dniane s¥ wszystkie parametry.
//
// Parametry:
//   hwnd      - [parametr] uchwyt okna do kt¢rego zostan¥ przypisane parametry
//   ppm       - [parametr] lista presentation parameters
//   ppt       - [parametr] lista wzorcowych presentation parameters
//   nColors   - [parametr] liczba pozycji w tablicy ppt (ignorowana gdy ppt == NULL)
//
// Powr¢t:
//   Obecnie funkcja zawsze zwraca 0
//
LONG ASUAAPI PpmStorePresParams(HWND hwnd, PRESPARAMS *ppm, PPTEMPLATE *ppt, LONG nColors)
{ PARAM *parm;           // Aktualnie przetwarzany parametr
  LONG   total;          // Caˆkowity rozmiar struktury PRESPARAMS
  LONG   i;              // Zmienne pomocnicze

  // Sprawdzenie czy s¥ jakie˜ parametry
  if(ppm == NULL) return(0);
  // Odczyt ilo˜ci parametr¢w (liczba bajt¢w)
  total = ppm->cb;

  // Odczyt kolejnych parametr¢w
  for(parm = ppm->aparam; total > 0; )
  { // Sprawdzenie czy identyfikator parametru jest poprawny
    if(parm->id == PP_FONTNAMESIZE)
    { // Zapami©tanie rodzaju czcionki
      WinSetPresParam(hwnd, PP_FONTNAMESIZE, strlen(parm->ab), parm->ab);
    }
    else if(ppt != NULL)
    { // Sprawdzenie czy jet to jeden z kolor¢w
      for(i = 0; i < nColors; ++i)
        if(parm->id == ppt[i].ulPresParam)
        { // Zapami©tanie koloru
          WinSetPresParam(hwnd, ppt[i].ulPresParam, sizeof(LONG), parm->ab);
          break;
        }
    }
    else
    { // Zapis wszystkich parametr¢w gdy nie ma struktury PPTEMPLATE
      WinSetPresParam(hwnd, parm->id, parm->cb, parm->ab);
    }

    // Przej˜cie do kolejnego parametru
    total -= (8 + parm->cb);
    parm   = (PARAM *)((UCHAR *)parm + 8 + parm->cb);
  }
  return(0);
}


/*
 * $Log: pparams.c $
 * Revision 1.2  1999/06/27 12:52:41  Wojciech_Gazda
 * Poprawka w funkcji PpmStorePresParams, tak aby funkcja
 * odczytywaˆa parametry bez zdefiniowanej tablicy PPTEMPLATE
 *
 * Revision 1.1  1999/02/03 22:23:24  Wojciech_Gazda
 * Initial revision
 *
 */
