/* ==========================================================================
    Progress Bar (Custom Control) for OS/2
   ===========================================

    $RCSfile: progbar.h $
      Author: Przemyslaw_Dobrowolski <dobrawka@asua.org.pl>
       $Date: 1999/04/16 11:15:40 $
   $Revision: 1.1 $
      $State: Exp $
       Notes: Progress Bar (ProcentBar) [prototyp]

 Copyright ¸ 1999 Asua OS/2 Programmers Group (http://www.asua.org.pl)
============================================================================= */

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


#ifndef __APROGBAR_H__
#define __APROGBAR_H__

#include "cconv.h"

// Definicja klasy okna
#define WC_PROGRESSBAR                          "WC_PROGRESSBAR"

#ifndef PROG_FIRST_MSG
#define PROG_FIRST_MSG  (WM_USER + 15)
#endif

// Numer ostatniego komunikatu
#define PROG_LAST_MSG   (PROG_FIRST_MSG + 9)

// Wiadomo˜ci
#define PBM_SETRANGE            (PROG_FIRST_MSG+0)   // Zmiana Rozmiaru
#define PBM_SETPOS              (PROG_FIRST_MSG+1)   // Caˆkowita zmiana pozycji
#define PBM_DELTAPOS            (PROG_FIRST_MSG+2)   // Zmiana aktualnej pozycji (dodawanie)
#define PBM_SETSTEP             (PROG_FIRST_MSG+3)   // Zmiana kroku (tylko PBS_SMOOTH!)
#define PBM_STEPIT              (PROG_FIRST_MSG+4)   
#define PBM_GETRANGE            (PROG_FIRST_MSG+5)   // Odczyt rozmiaru
#define PBM_GETPOS              (PROG_FIRST_MSG+6)   // Odczyt pozycji
#define PBM_SETBARCOLOR         (PROG_FIRST_MSG+7)   // Zmiana koloru wska«nika
#define PBM_SETBKCOLOR          (PROG_FIRST_MSG+8)   // Zmiana tˆa wska«nika

// Style
#define PBS_SMOOTH        0x0001 // Styl ala Windows (kroki)
#define PBS_TEXT          0x0002 // Posiada tekst - nie dotyczy PBS_SMOOTH
#define PBS_VERTICAL      0x0004 // Pionowy wska«nik
#define PBS_RAISED        0x0008 
#define PBS_DEPRESSED     0x0010 

LONG ASUAAPI ProgbarRegister(HAB hAB);

typedef struct
{
    SHORT sLow;
    SHORT sHigh;
} PBRANGE, *PPBRANGE;


#endif /* __APROGBAR_H__ */
/* ============================================================================
 *
 * $Log: progbar.h $
 * Revision 1.1  1999/04/16 11:15:40  Przemyslaw_Dobrowolski
 * Initial revision
 *
 */