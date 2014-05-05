/*
 * Biblioteka okien kontrolnych - Asua Controls
 * Generowanie raport�w o b��dach
 * (c) 1999 Wojciech Gazda
 *
 * asudebug.h
 *
 * $Author:$
 * $Date:$
 * $Name:$
 * $RCSfile$
 * $Revision:$
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


#include "cconv.h"

// Deklaracje sta�ych
#define LOGFILE     "asuactl.err"  // Nazwa pliku
#define LOGSIZE     4095           // Maksymalny rozmiar jednej pozycji logu


// Prototypy funkcji
VOID ASUAAPI AsuLogPMError(PSZ classname, ULONG PMerror, PSZ fname, PSZ commnet);


/*
 * $Log:$
 */
