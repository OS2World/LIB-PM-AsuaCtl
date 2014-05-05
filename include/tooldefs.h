/* Asuactl.dll
 * Pasek narz�dzi, og�lne deklaracje struktur danych i sta�ych
 * paska narz�dzi, niedost�pne dla u�ytkownika.
 *
 * (c) 1999 Wojciech Gazda
 *
 * tooldefs.h
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/06/27 12:36:37 $
 * $Name:  $
 * $RCSfile: tooldefs.h $
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


// Wa�niejsze wymiary
#define CX_TITLEWIDTH         10        // Szeroko�� paska tytu�u
#define CY_TITLEHEIGHT        10        // Wysoko�� paska tytu�u
#define CY_BOTTOMTIPSPACE     10        // Odst�p mi�dzy podpowiedzi� a doln� kraw�dzi� przycisku
#define CY_TOPTIPSPACE        3         // Odst�p mi�dzy podpowiedzi� a g�rn� kraw�dzi� przycisku
#define CX_ROTATEBUTTON       17        // Szeroko�� przycisku obracania paska
#define CY_ROTATEBUTTON       17        // Wysoko�� przycisku obracania paska
#define CX_ROTATEBITMAP       15        // Szeroko�� mapy bitowej u�ywanej do rysowania przycisku obracania
#define CY_ROTATEBITMAP       15        // Wysoko�� mapy bitowej u�ywanej do rysowania przycisku obracania
#define CXY_TITLEBUTTON       8         // Rozmiary przycisku rysowanego na pasku tytu�u
#define CXY_TITLECUT          7         // Szeroko�� �ci�cia
#define CXY_ENDMARGIN         4         // Margines ko�cowy
#define CXY_INTERNALMARGIN    1         // Dodatkowy margines mi�dzy obramowaniem paska a obiektem
#define CXY_OBJECTSPACE       1         // Odst�p mi�dzy obiektami
#define CXY_ROTATESPACE       3         // Odst�p od przycisku obracania (po obydwu stronach)
#define CXY_SEPARATOR         5         // Grubo�� separatora


// Inne definicje
#define TIME_TIPDELAY         1000      // Czas op��nienia przed w��czeniem podpowiedzi
#define TIME_TIPREMOVE        150       // Okres testowania po�o�enia kursora


// Kolory u�ywane przez pasek narz�dzi
#define   TOOL_BACKGROUND           0    // Kolor nieaktywnego t�a paska tytu�u
#define   TOOL_HILITEBACKGROUND     1    // Kolor pod�wietlonego t�a paska tytu�u
#define   TOOL_FRAMEHILITE          2    // Pod�wietlenie
#define   TOOL_FRAMEDARK            3    // Cie�
#define   TOOL_TIPBACKGROUND        4    // Kolor t�a podpowiedzi
#define   TOOL_TIPTEXTCOLOR         5    // Kolor tekstu podpowiedzi
#define   TOOL_TIPFRAMECOLOR        6    // Kolor ramki otaczaj�cej tekst podpowiedzi
#define   TOOL_FRAMEDRAGHILITE      7    // Pod�wietlenie ramki podczas przesuwania
#define   TOOL_FRAMEDRAGDARK        8    // Cie� ramki podczas przesuwania
// Liczba kolor�w
#define   TOOL_MAXCOLOR             9




// Stany paska narz�dzi niedost�pne dla u�ytkownika - flToolState
#define  TST_FULLTRCFRAME  0x00010000   // ��danie rysowania pe�nej ramki (np. podczas przesuwania paska)
#define  TST_MINMAXCTL     0x00020000   // Trwa w�a�nie minimalizacja/maksymalizacja paska
#define  TST_TIPACTIVATED  0x00040000   // Podpowied� zosta�a uaktywniona
#define  TST_HITTEST       0x00080000   // Trwa przetwarzanie komunikatu WM_HITTEST



// Struktury danych
// Struktura kontrolna obiektu umieszczonego na pasku narz�dzi
typedef struct
{ ULONG   iPosition;     // Po�o�enie obiektu na pasku narz�dzi (numer kolejny)
  ULONG   ulGroup;       // Grupa do kt�rej nale�y obiekt (wa�ne tylko dla przycisk�w)
  ULONG   flAttribute;   // Dodatkowe atrybuty obiektu
  LONG    x,  y;         // Po�o�enie obiektu na pasku narz�dzi
  LONG    cx, cy;        // Wymiary obiektu
  ULONG   id;            // Identyfikator obiektu
  ULONG   ulUser;        // Pole do wykorzystania przez u�ytkownika
  HWND    hwndObject;    // Uchwyt okna obiektu
  PFNWP   pfnObjProc;    // Oryginalna procedura okna obiektu
  ATOM    aToolTip;      // Tekst podpowiedzi
} OBJCTL;



// G��wna struktura kontrolna paska narz�dzi
typedef struct
{ ULONG    flWinStyle;             // Style okna
  ULONG    flToolAttrs;            // Atrybuty paska narz�dzi
  ULONG    flToolState;            // Stan paska narz�dzi
  ULONG    flOldState;             // Stan paska tytu�u z przed ostatniej minimalizacji/maksymalizacji
  ULONG    flTitleState;           // Stan paska tytu�u
  ULONG    flRotateState;          // Stan przycisku obracania okna
  ULONG    ulMaxLength;            // Maksymalna d�ugo�� paska w stanie floating
  LONG     lScrollOffset;          // Przesuni�cie element�w paska wzgl�dem pocz�tku
  LONG     lActiveTip;             // Identyfikator obiektu wy�wietlaj�cego podpowied�
  HWND     hwndToolBar;            // Uchwyt paska narz�dzi
  HWND     hwndTitleBar;           // Pasek tytu�u paska narz�dzi
  HWND     hwndToolTip;            // Uchwyt okna wy�wietlaj�cego podpowiedzi
  HWND     hwndRotate;             // Uchwyt przycisku obracania okna
  HWND     hwndObjectPad;          // Uchwyt okna aprzechowuj�cego obiekty
  POINTL   ptlOffset;              // Przesuni�cie paska narz�dzi wzgl�dem w�a�ciciela
  PSZ      szWinText;              // Tytu� okna (wy�wietlany jako podpowied�)
  HATOMTBL hatomTips;              // Podpowiedzi do obiekt�w
  ULONG    nObjects;               // Liczba obiekt�w znajduj�cych si� na pasku narz�dzi
  OBJCTL  *objtab;                 // Tablica definicji obiekt�w
  LONG     colors[TOOL_MAXCOLOR];  // Pomocnicza tablica kolor�w
} TOOLCTL;


// Tablica wzorc�w kolor�w dla paska narz�dzi
extern PPTEMPLATE PPmColor[];
// Uchwyt modu�u zawieraj�cego zasoby i funkcje
extern HMODULE hResource;
// Wersja systemu operacyjnego
extern ULONG   ulWarpVer;




// Prototypy funkcji
VOID     ToolDisplayTip(HWND hwnd, HWND hwndRef, PSZ szTipText);
VOID     ToolDrawSeparator(HWND hwnd, LONG lPos, ULONG ulSize);
TOOLCTL *ToolLoadData(HWND hwnd, PSZ module);
LONG     ToolSendNotify(HWND hwnd, ULONG notify, VOID *ctlspec);
VOID     ToolUpdateSize(TOOLCTL *tcl);

/*
 * $Log: tooldefs.h $
 * Revision 1.1  1999/06/27 12:36:37  Wojciech_Gazda
 * Initial revision
 *
 */
