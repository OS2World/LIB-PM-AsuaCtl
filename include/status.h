/* Dodatkowe kontrolki panelu.
 * (c) 1999 Wojciech Gazda
 *
 * ctlmisc.h
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/04/18 22:55:35 $
 * $Name:  $
 * $RCSfile: status.h $
 * $Revision: 1.7 $
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


#ifndef _STATUS_H_
#define _STATUS_H_

#include "cconv.h"

// Definicja klasy okna
#define WC_STATUS        "WC_STATUS"    // Rozszerzone okno statusu



// Definicje komunikat¢w
// Przed plikiem ctlmisc.h mo¾na zdefiniowa† wˆasny indeks pierwszego komuniaktu
// tak, aby unikn¥† konfliktu z innymi komunikatami. Nowy indeks musi by† liczony
// wzgl©dem WM_USER (na przykˆad WM_USER + 5)
#ifndef STAT_FIRST_MSG
#define STAT_FIRST_MSG  (WM_USER + 5)
#endif

// Numer ostatniego komunikatu
#define STAT_LAST_MSG   (STAT_FIRST_MSG + 7)

// Komunikaty umo¾liwiaj¥ce odczyt informacji na temat okna WC_STATUS
// Odczyty szeroko˜ci i wysoko˜ci okna dziaˆaj¥ nawet wtedy, gdy
// okno ma rozmiary 0,0. Zwracane warto˜ci to wymiary potrzebne
// do tego, aby tekst zawarty w oknie zmie˜ciˆ si© w caˆo˜ci. Wymiary
// te nie zale¾¥ od aktualnych rozmiar¢w okna
#define SM_QUERYMAXCHARINC (STAT_FIRST_MSG + 0)   // Odczyt max. szeroko˜ci znaku - lAveCharWidth
#define SM_QUERYTEXTHEIGHT (STAT_FIRST_MSG + 1)   // Odczyt wysoko˜ci tekstu - lMaxBaselineExt
#define SM_QUERYTEXTWIDTH  (STAT_FIRST_MSG + 2)   // Odczyt szeroko˜ci tekstu
#define SM_QUERYHEIGHT     (STAT_FIRST_MSG + 3)   // Odczyt wysoko˜ci okna
#define SM_QUERYHILITE     (STAT_FIRST_MSG + 4)   // Odczyt stanu aktywno˜ci okna
#define SM_QUERYWIDTH      (STAT_FIRST_MSG + 5)   // Odczyt szeroko˜ci okna
#define SM_SETHILITE       (STAT_FIRST_MSG + 6)   // Aktywacja / dezaktywacja okna

// Kody potwierdzeä (usnotifycode) przesyˆne komunikatem WM_CONTROL
// Nast¥piˆa zmiana presentation parameter za pomoc¥ drag&drop,
// ulcontrolspec zawiera kod zmienionego parametru - PP_*
#define SMN_PRESPARAMCHANGED       0x0400



// Definicje kod¢w bˆ©d¢w
#define ERR_PNL_CLASSREG      0x0001    // bˆ¥d rejestracji klasy okna



// Style okna WC_STATUS
// Obramowanie:
#define SS_BORDER             0x000F    // Kompletna ramka dookoˆa okna
#define SS_THINFRAME          0x0080    // ½¥danie rysowania cienkiego obramowania
#define SS_LEFTFRAME          0x0001    // Lewa kraw©d« obramowania
#define SS_RIGHTFRAME         0x0002    // Prawa kraw©d« obramowania
#define SS_TOPFRAME           0x0004    // G¢rna kraw©d« obramowania
#define SS_BOTTOMFRAME        0x0008    // Dolna kraw©d« obramowania

// Pozycjonowanie tekstu w osi X
#define SS_LEFT               0x0000    // Tekst jest rysowany od lewej strony
#define SS_CENTER             0x0020    // Tekst jest ˜rodkowany
#define SS_RIGHT              0x0040    // Tekst jest wyr¢wnywany do prawej
#define SS_AUTOSHIFT          0x0060    // Tekst wyr¢wnany do lewej, zostaje przesuwane w prawo gdy nie mie˜ci si© w oknie

// Automatyczne skalownie
#define SS_XAUTOSCALE         0x0100    // Okno jest automatycznie skalowane w osi poziomej
#define SS_YAUTOSCALE         0x0200    // Okno jest automatycznie skalowane w osi pionowej

// Dodatkowe opcje
#define SS_OWNERDRAW          0x0010    // U¾ytkownik mo¾e samodzielnie przerysowa† tˆo
#define SS_MOUSETRANSPARENT   0x0400    // Okno jest przezroczyste dla komunikat¢w myszy
#define SS_WINSTYLE           0x0800    // Rysowanie uchwytu skalowania okna




// Prototypy funkcji
LONG ASUAAPI CtlStatusInitialize(HAB hab);

#endif // _STATUS_H_
/*
 * $Log: status.h $
 * Revision 1.7  1999/04/18 22:55:35  Wojciech_Gazda
 * Nowy styl: SS_WINSTYLE
 *
 * Revision 1.6  1999/02/28 22:17:33  Wojciech_Gazda
 * Dodano styl SS_MOUSETRANSPARENT
 *
 * Revision 1.5  1999/02/26 23:15:19  Wojciech_Gazda
 * Dodanie komunikat¢w aktywuj¥cych i dezaktywuj¥cych okno
 *
 * Revision 1.4  1999/02/20 19:45:05  Wojciech_Gazda
 * Zmiana struktury OWNERBACK
 *
 * Revision 1.3  1999/02/11 12:42:17  Wojciech_Gazda
 * Dodanie stylu SS_AUTOSHIFT
 *
 * Revision 1.2  1999/02/03 22:25:17  Wojciech_Gazda
 * Pragma - dodano definicj© pakowania struktur
 *
 * Revision 1.1  1999/01/31 15:12:23  Wojciech_Gazda
 * Initial revision
 *
 */
