/* Biblioteka okien kontrolnych: Asua Controls
 * (c) 1999 Wojciech Gazda &  Przemys�aw Dobrowolski
 * Deklaracje funkcji i zmiennych wewn�trzbibliotecznych.
 * Do��czenie plik�w *.h z sekcji common.
 *
 * Author: $
 * $Date: 1999/06/27 12:52:14 $
 * $Name:  $
 * $RCSfile: asuintl.h $
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


#ifndef _ASUAINTL_H_
#define _ASUAINTL_H_

// Pliki nag��wkowe zawieraj�ce deklaracje funkcji us�ugowych
#include "graphics.h"    // Dodatkowe funkcje do obs�ugi grafiki
#include "pparams.h"     // Obs�uga presentation parameters


// Zmienne globalne
// Uchwyt modu�u asuactl.dll
extern HMODULE hResource;
// Wersja systemu operacynego
extern ULONG   ulWarpVer;



// Prywatne, pomocnicze struktury danych biblioteki asuactl.dll
//
// Struktura informacyjna obiektu, umo�liwiaj�ca odczyt wersji i innych informacji
#ifdef __WATCOMC__
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif

typedef struct
{ ULONG   cbSize;        // Rozmiar
  PSZ     szRCSVersion;  // Numer wersji w ormacie RCS
  USHORT  usVerMajor;    // G��wny numer wersji
  USHORT  usVerMinor;    // Dodatkowy numer wersji
} CTLINFO;


// Struktury danych wsp�lne dla wszystkich okien kontrolnych,
// dost�pne tak�e dla u�ytkownika
//
// Struktura przekazywana przez okna stylu *_OWNERDRAW podczas
// przetwarzania komunikatu WM_DRAWITEM. Struktura umo�liwia
// w�a�cicielowi kontrolki przerysowanie jej t�a. Zalecane jest
// aby we wszystkich kontrolkach umo�liwiaj�cych u�ytkownikowi rysowanie t�a,
// u�ywa� tej w�a�nie struktury.
typedef struct
{ HWND    hwnd;     // Uchwyt przerysowywanego okna
  HPS     hps;      // Uchwyt presentation space, kt�rego nale�y u�y� podczas rysowania
  RECTL   bound;    // Wsp��rz�dne obszaru, kt�ry nale�y przerysowa�
  POINTL  refpoint; // Wsp��rz�dne punktu odniesienia
} OWNERBACK;

#ifdef __WATCOMC__
#pragma pack(pop)
#else
#pragma pack()
#endif


// Prototypy og�lnych funkcji us�ugowych
MRESULT AsuQueryCtlInfo(HWND hwnd, ULONG msg, MPARAM mp1, CTLINFO *CtlInfo);

// Prototypy funkcji
LONG ASUAAPI AsuCtlInitialize(HAB hab);
LONG ASUAAPI AsuGetComponentVersion(PSZ class);
LONG ASUAAPI AsuGetVersion(VOID);

#endif // _ASUAINTL_H_
/*
 * $Log: asuintl.h $
 * Revision 1.1  1999/06/27 12:52:14  Wojciech_Gazda
 * Initial revision
 *
 */
