/* OS/2 Commander - modu�y pomocnicze
 * Wspomaganie zarz�dzaniem czcionkami
 * (c) 1999 Wojciech Gazda
 *
 * fonts.h
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/01/31 15:13:24 $
 * $Name:  $
 * $RCSfile: fonts.h $
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


#include "cconv.h"

// Kody b��d�w
#define ERR_FNT_INVPARAM      1         // B��dna warto�� parametru
#define ERR_FNT_OUTMEM        2         // B��d alokacji pami�ci
#define ERR_FNT_HDC           3         // B��d odczytu kontekstu urz�dzenia
#define ERR_FNT_CREATE        4         // B��d tworzenia czcionki


// Prototypy funkcji
LONG ASUAAPI FntCreateRasterFont(HPS hps, PSZ szNameSize, LONG lLcid, FONTMETRICS *fm);

/*
 * $Log: fonts.h $
 * Revision 1.1  1999/01/31 15:13:24  Wojciech_Gazda
 * Initial revision
 *
 */
