/* Biblioteka okien kontrolnych: Asua Controls
 * (c) 1999 Wojciech Gazda &  Przemysˆaw Dobrowolski
 *
 * Author: Wojciech_Gazda
 *         Przemyslaw_Dobrowolski
 * $Date: 2001/01/01 12:35:05 $
 * $Name:  $
 * $RCSfile: asuactl.h $
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


// Komunikaty zarezerwowane przez system:
// WM_USER + 40 do 42
// WM_USER + 50 do 55
// WM_USER + 1000
//
// Lista komunikat¢w u¾ywanych przez kontrolki:
// WC_STATUS      - od WM_USER + 5  do WM_USER + 11, rezerwa do WM_USER + 14
// WC_PROGRESSBAR - od WM_USER + 15 do WN_USER + 23, rezerwa do WM_USER + 24
// WC_TOOLBAR     - od WM_USER + 60 do WM_USER + 79 na razie.
// WC_WINBUTTON   - systemowe



// Pliki nagˆ¢wkowe okien kontrolnych
#include "progbar.h"                // Okno stanu     WC_PROGRESSBAR
#include "status.h"                 // Okno kontrolne WC_STATUS
#include "toolbar.h"                // Pasek narz©dzi WC_TOOLBAR
#include "winbutt.h"                // Przycisk       WC_WINBUTTON

#ifdef __WATCOMC__
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif

// Struktury danych
// Struktura przekazywana przez okna stylu *_OWNERDRAW podczas
// przetwarzania komunikatu WM_DRAWITEM. Struktura umo¾liwia
// wˆa˜cicielowi kontrolki przerysowanie jej tˆa. Zalecane jest
// aby we wszystkich kontrolkach umo¾liwiaj¥cych u¾ytkownikowi rysowanie tˆa,
// u¾ywa† tej wˆa˜nie struktury.
typedef struct
{ HWND    hwnd;     // Uchwyt przerysowywanego okna
  HPS     hps;      // Uchwyt presentation space, kt¢rego nale¾y u¾y† podczas rysowania
  RECTL   bound;    // Wsp¢ˆrz©dne obszaru, kt¢ry nale¾y przerysowa†
  POINTL  refpoint; // Wsp¢ˆrz©dne punktu odniesienia
} OWNERBACK;

#ifdef __WATCOMC__
#pragma pack(pop)
#else
#pragma pack()
#endif


// Prototypy funkcji
LONG ASUAAPI AsuCtlInitialize(HAB hab);
LONG ASUAAPI AsuGetComponentVersion(PSZ class);
LONG ASUAAPI AsuGetVersion(VOID);

typedef struct _DIRDLG
{
  PSZ pszTitle;
  PSZ pszOKButton;
  PSZ pszCancelButton;
  PSZ pszInitialDir;
  CHAR szFullDirectoryName[CCHMAXPATH];
  LONG lReturn; // DID_OK or DID_CANCEL
  LONG flDialogType;
  BOOL flUseWinButton;
} DIRDLG, *PDIRDLG;

BOOL ASUAAPI AsuDirDlg(HWND hwndParent,HWND hwndOwner, PDIRDLG pdd);


/*
 * $Log: asuactl.h $
 * Revision 1.1  2001/01/01 12:35:05  DOBRAWKA
 * Initial revision
 *
 * Revision 1.4  1999/06/27 12:48:26  Wojciech_Gazda
 * Uproszczenie procesu modyfikacji pliku podczas dodawania nowych kontrolek
 *
 * Revision 1.3  1999/04/16 10:57:23  Przemyslaw_Dobrowolski
 * Dodano funkcje funkcje sprawdzajace wersje
 * Zmiana ustawien katalogow z naglowkami
 *
 * Revision 1.2  1999/03/18 12:54:30  Przemyslaw_Dobrowolski
 * Dostosowanie do WC_PROGRESSBAR i WC_WINBUTTON
 *
 * Revision 1.1  1999/02/28 22:16:20  Wojciech_Gazda
 * Initial revision
 *
 */
