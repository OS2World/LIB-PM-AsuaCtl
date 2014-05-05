/* Biblioteka okien kontrolnych - Asua Controls
 * (c) 1999 Wojciech Gazda & Przemyslaw Dobrowolski
 *
 * asuctl.c
 *
 * Author: Wojciech_Gazda
 *         Przemyslaw_Dobrowolski
 * $Date: 1999/06/27 12:50:31 $
 * $Name:  $
 * $RCSfile: asuactl.c $
 * $Revision: 1.5 $
 *
 * _Nale¾y_ wszystkie pliki linkowa† z rozmiarem stacks¢w co najmniej 32kB
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


#ifdef __WATCOMC__
// Deklaracja formatu wywoˆania dla LibMain
#pragma aux LibMain "*"    \
            parm caller [] \
            value no8087   \
            modify [eax ecx edx];
#endif

// Deklaracje OS/2
#define INCL_WINMESSAGEMGR
#define INCL_WINWINDOWMGR
#define INCL_WINSYS
#define INCL_DOSPROCESS
#define INCL_DOSMISC
#include <os2.h>

// Deklaracja plik¢w nagˆ¢wkowych kompilatora
#include <string.h>
#include <stdio.h>

// Deklaracje lokalne
#include "asuintl.h"
#include "progbar.h"
#include "status.h"
#include "toolbar.h"
#include "winbutt.h"



// Wersja naszej biblioteki
static PSZ szAsuaVersion = "$Revision: 1.5 $";

// Zmienne globalne importowane
// Wersje kontrolek
extern PSZ szButtonVersion;
extern PSZ szProgbarVersion;

// Zmienne globalne eksportowane
HMODULE hResource = NULLHANDLE;         // Uchwyt moduˆu asuactl.dll
ULONG   ulWarpVer = 30;                 // Wersja systemu operacyjnego (domy˜lnie Warp 3)

// Zmienne globalne wewn¥trzmoduˆowe
static HAB hAnchor = NULLHANDLE;        // Uchwyt Anchor-Block




// Prototypy funkcji
MRESULT AsuQueryCtlInfo(HWND hwnd, ULONG msg, MPARAM mp1, CTLINFO *CtlInfo);
LONG ASUAAPI AsuCtlInitialize(HAB hab);
LONG ASUAAPI AsuGetComponentVersion(PSZ class);
LONG ASUAAPI AsuGetVersion(VOID);

// Prototypy funkcji lokalnych
static LONG AsuRcsToLong(PSZ szRevision);

#if __IBMC__ || __IBMCPP__
void __ctordtorInit( void );
void __ctordtorTerm( void );
int  _CRT_init(void);
void _CRT_term(void);
#endif


#ifdef __WATCOMC__
unsigned LibMain(unsigned hmod, unsigned term)  
#else
unsigned long _System _DLL_InitTerm(unsigned long hmod, unsigned long term)
#endif
{ PPIB  ppib;
  PTIB  ptib;

  switch (term)
  { // Zakoäczenie wykonywania dll-a
    case 1:
#if __IBMC__ || __IBMCPP__
//      __ctordtorTerm();
      _CRT_term();
#endif
      break;

    case 0:  
      // Inicjacja DLL-a
#if __IBMC__ || __IBMCPP__
      _CRT_init();
//      __ctordtorInit();
#endif
      DosGetInfoBlocks(&ptib, &ppib);
      if (ppib->pib_ultype != 3)
      {
        ULONG ulWritten;
        DosWrite(2, "This program must be run from an Presentation Manager!\r\n", 54, &ulWritten);
        DosSleep(1000);
        DosExit(EXIT_PROCESS, 1);
      }
      // musimy pami©ta† moduˆ w kt¢rym s¥ domy˜lne ikony
      hResource = hmod;

      // Odczyt i zapami©tanie wersji systemu operacyjnego
      DosQuerySysInfo(QSV_VERSION_MINOR, QSV_VERSION_MINOR, &ulWarpVer, sizeof(ULONG));
      break;

    default  :
      return 0;
  }
     
  // Poprawne przeprowadzenie inicjalizacji

  return(1);
}





// Funkcja powinna by† umieszczona w gˆ¢wnej procedurze okna kontrolnego.
// Parametr mp1 powinien zostaje przekazany przez asuactl.dll w momencie
// wywoˆania funkcji AsuGetComponentVersion, i zawiera wska«nik do struktury
// CTLINFO odczytywanej p¢«niej przez AsuGetComponentVersion.
// Funkcja przepisuje zawarto˜† struktury CtlInfo dostarczonej przez programist©
// pisz¥cego okno kontrolne do obszaru wskazywanego przez mp1, ale tylko wtedy gdy s¥
// speˆnione nast©puj¥ce warunki:
//   hwnd == NULL, msg == WM_NULL, mp1 != NULL i CtlInfo != NULL.
// Umo¾liwia to odr¢¾nienie specjalnego wywoˆania WM_NULL od wywoˆania systemowego.
//
// Parametry:
//   hwnd      - [parametr] r¢wny NULL przy odczycie wersji
//   msg       - [parametr] r¢wny WM_NULL przy odczycie wersji
//   mp1       - [rezultat] wska«nik do CTLINFO w procedurze AsuGetComponentVersion
//   mp2       - [parametr] struktura CTLINFO dostarczana przez u¾ytkownika
//
// Powr¢t:
//   Zawsze 0
//
MRESULT AsuQueryCtlInfo(HWND hwnd, ULONG msg, MPARAM mp1, CTLINFO *CtlInfo)
{ CTLINFO *tmp;     // Wska«nik pomocniczy
  LONG     dsize;   // Rozmiar kopiowanego obszaru

  // Sprawdzenie warunk¢w odczytu wersji
  if(hwnd != NULLHANDLE) return(0);
  if(msg  != WM_NULL)    return(0);
  if(mp1  == 0)          return(0);
  if(CtlInfo == NULL)    return(0);

  // Przepisanie parametr¢w
  tmp = (CTLINFO *)mp1;
  // Okre˜lenie rozmiaru kopiowanego obszaru
  if(CtlInfo->cbSize > tmp->cbSize)
    dsize = tmp->cbSize;
  else dsize = CtlInfo->cbSize;

  memcpy(tmp, CtlInfo, dsize);
  return(0);
}





// Inicjacja okien kontrolnych zawartych w bibliotece dynamicznej
//
LONG ASUAAPI AsuCtlInitialize(HAB hab)
{ LONG rc;

  // Zapami©tanie uchwytu anchor-block dla p¢«niejszych odczyt¢w wersji
  hAnchor = hab;
  // Ustalenie kodu bˆ©du - poprawne wykonanie
  rc = 0;

  // Inicjacja okna WC_STATUS
  if(CtlStatusInitialize(hab)) rc = 1;

  // Inicjalizacja okna WC_PROGRESSBAR
  if(ProgbarRegister(hab)) rc = 1;

  // Inicjalizacja okna WC_WINBUTTON
  if(BtnRegister(hab)) rc = 1;

  // Inicjalizacja okna WC_TOOLBAR
  if(CtlToolbarInitialize(hab)) rc = 1;

  return(rc);
}





// Odczyt wersji komponentu - kontrolki zawartej w bibliotece asuactl.dll
//
// Parametry:
//   szClass   - [parametr] nazwa klasy
//
// Powr¢t:
//   Mniej zancz¥ce sˆowo    - numer podwersji (minor)
//   Bardziej znacz¥ce sˆowo - numer wersji (major)
//
LONG ASUAAPI AsuGetComponentVersion(PSZ szClass)
{ CLASSINFO cls;    // Struktura informacyjna klasy
  CTLINFO   ctl;    // Struktura w t¢rej kontrolka zwraca numer wersji

  // Zalecana metoda pobierania wersji (zautomatyzowana)
  if(WinQueryClassInfo(hAnchor, szClass, &cls) == TRUE)
  { // Inicjacja struktury ctl;
    ctl.cbSize     = sizeof(CTLINFO);
    ctl.usVerMajor = 0;
    ctl.usVerMinor = 0;

    // Odczyt wersji przez wywoˆanie procedury okna z WM_NULL
    (*cls.pfnWindowProc)(NULL, WM_NULL, MPFROMP(&ctl), 0);

    // Sprawdzenie czy jest okr˜lony numer wersji zapisany binarnie
    if(ctl.usVerMinor | ctl.usVerMajor)
      return(MAKELONG(ctl.usVerMinor, ctl.usVerMajor));

    // Odczyt wersji zapisanej w formacie RCS
    if(ctl.szRCSVersion != NULL)
      return(AsuRcsToLong(ctl.szRCSVersion));
  }

  // Przestarzaˆa metoda pobierania wersji
  if (strcmp(szClass,WC_WINBUTTON)==0)
    return AsuRcsToLong(szButtonVersion);
  if (strcmp(szClass,WC_PROGRESSBAR)==0)
    return AsuRcsToLong(szProgbarVersion);

  return (0);
}





// Odczyt wersji biblioteki dynamicznej
//
// Powr¢t:
//   Mniej zancz¥ce sˆowo    - numer podwersji (minor)
//   Bardziej znacz¥ce sˆowo - numer wersji (major)
//
LONG ASUAAPI AsuGetVersion(VOID)
{
  return AsuRcsToLong(szAsuaVersion);
}





/*******************/
/* Funkcje lokalne */
/*******************/

// Funkcja przetwarza zapis generowany przez RCS na format binarny
//
// Parametry:
//   szRevision     - [parametr] ci¥g znak¢w z numerem wersji generowany przez RCS
//
// Powr¢t:
//   Mniej zancz¥ce sˆowo    - numer podwersji (minor)
//   Bardziej znacz¥ce sˆowo - numer wersji (major)
//
static LONG AsuRcsToLong(PSZ szRevision)
{ USHORT usMinor = 0;
  USHORT usMajor = 0;

  // Odczyt numeru wersji
  if(sscanf(szRevision, "%*[^0123456789]%d.%d", &usMajor, &usMinor) != 2)
    return(0);
  // Korekta numeru wersji
  return(MAKELONG(usMinor * 10, usMajor));
}


/*
 * $Log: asuactl.c $
 * Revision 1.5  1999/06/27 12:50:31  Wojciech_Gazda
 * Uproszczenie funkcji AsuRcsToLong
 * dodano now¥ metod© odczytu wersji kontrolki, odost©pniono
 * globalne zmienne wewn¥trzbiblioteczne
 *
 * Revision 1.4  1999/04/16 13:18:12  Przemyslaw_Dobrowolski
 * Przy wielu uruchomieniach sprawdzenia wersji moduˆ¢w pojawiaˆy si©
 * gˆupoty. Poprawiono.
 *
 * Revision 1.3  1999/04/16 10:48:45  Przemyslaw_Dobrowolski
 * Dodano funkcje sprawdzania wersji komponent¢w i wersji biblioteki
 *
 * Revision 1.2  1999/03/18 12:51:35  Przemyslaw_Dobrowolski
 * Biblioteka laduje sie tylko w trybie PM.
 * Dodano inicjalizacje WC_WINBUTTON i WC_PROGRESSBAR
 *
 * Revision 1.1  1999/02/28 22:16:16  Wojciech_Gazda
 * Initial revision
 *
 */
