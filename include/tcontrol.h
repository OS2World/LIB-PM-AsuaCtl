/* Asuactl.dll
 * Pasek narz©dzi - dodatkowe okna kontrolne (Carrier Controls).
 *
 * (c) 1999 Wojciech Gazda
 *
 * tcontrol.h
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/06/27 12:36:20 $
 * $Name:  $
 * $RCSfile: tcontrol.h $
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


// Dodatkowe klasy
#define INTC_MINITITLE   (PSZ)"INTC_MINITITLE"    // Specjalny pasek tytuˆu
#define INTC_ROTATEBTN   (PSZ)"INTC_ROTATEBTN"    // Przycisk obracania okna



// Definicje map bitpwych
// Przycisk minimaliazacji/maksymalizacji
#define MONO_UPTRIANGL    1900
#define MONO_DOWNTRIANGL  1901
#define COLOR_UPTRIANGL   1902
#define COLOR_DOWNTRIANGL 1903

// Przycisk obracania paska narz©dzi
#define ROTATE_BACK       1904          // Tˆo przycisku (maska bitowa)
#define ROTATE_OUTLINE    1905          // Kontur - efekt 3D
#define ROTATE_FILL       1906          // Wypeˆnienie konturu



// Stany paska tytuˆu - flTitleState
#define STT_ROTATED      0x0001         // Pasek tytuˆu jest rysowany poziomo
#define STT_HILITED      0x0002         // Pasek tytuˆu jest rysowany w stanie pod˜wietlonym
#define STT_DRAWCUT      0x0004         // Rysowanie ˜ci©cia
#define STT_DRAWBUTTON   0x0008         // Rysowanie przycisku
#define STT_UPTRIANGL    0x0010         // Rysowanie tr¢jk¥cika ostrzem w g¢r©
#define STT_TOOLTIP      0x0020         // Znacznik wy˜wietlania podpowiedzi do paska tytuˆu

// Stany przycisku obracania - flRotateState
#define SRB_HILITED      0x0001         // Przycisk jest pod˜wietlony
#define SRB_BUTTONDOWN   0x0002         // Przycisk myszy jest w dolnym poˆo¾eniu



// Tryby pracy funkcji MtlQueryArea
#define MTLQ_MOVEBAR          0         // Sprawdzenie czy kursor jest nad uchwytem do przesuwania okna
#define MTLQ_FIXBUTTON        1         // Sprawdzenie czy kursor jest nad przyciskiem min/max


// Prototypy funkcji - moduˆ tcontrol.c
MRESULT EXPENTRY MiniTitlebarProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT EXPENTRY RotateButtonProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);


/*
 * $Log: tcontrol.h $
 * Revision 1.1  1999/06/27 12:36:20  Wojciech_Gazda
 * Initial revision
 *
 */
