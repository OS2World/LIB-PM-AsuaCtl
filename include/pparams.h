/* OS/2 Commander - moduˆy pomocnicze
 * obsˆuga Presentation parameters
 *
 * pparams.h
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/02/03 22:23:28 $
 * $Name:  $
 * $RCSfile: pparams.h $
 * $Revision: 1.1 $
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


#ifndef _PPARAMS_H_
#define _PPARAMS_H_

#ifdef __WATCOMC__
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif

#include "cconv.h"

// Deklaracje typ¢w danych
// Struktura ta u¾ywana jest przez funkcj© tworz¥c¥ tablic© kolor¢w
// na podstawie presentation parameters okna, funkcja: PpmQueryPresColors
typedef struct
{ ULONG ulPresParam;     // Odczytywany presentation parameter
  ULONG ulPresFlags;     // Flagi u¾ywane QPF_* przez funkcj© WinQueryPresParam
  ULONG ulSysColor;      // Indeks SYSCLR_* koloru systemowego, u¾ywanego wtedy gdy parametr nie istnieje
  ULONG ulRgbColor;      // Kolor u¾yty wtedy gdy indeks SYSCLR_* zostaˆ podany jako zero
} PPTEMPLATE;

#ifdef __WATCOMC__
#pragma pack(pop)
#else
#pragma pack()
#endif



// Prototypy funkcji
LONG ASUAAPI PpmQueryPresColors(HWND hwnd, LONG nColors, PPTEMPLATE *ppt, LONG *lColTable);
LONG ASUAAPI PpmStorePresParams(HWND hwnd, PRESPARAMS *ppm, PPTEMPLATE *ppt, LONG nColors);

#endif // _PPARAMS_H_
/*
 * $Log: pparams.h $
 * Revision 1.1  1999/02/03 22:23:28  Wojciech_Gazda
 * Initial revision
 *
 */
