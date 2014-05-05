/* Biblioteka okien kontrolnych - Asua Controls
 * Pomocnicze funkcje graficzne.
 * (c) 1999 Wojciech Gazda & Przemyslaw Dobrowolski
 *
 * graphics.h
 *
 * $Author: $
 * $Date:  $
 * $Name:  $
 * $RCSfile: $
 * $Revision: $
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


#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include "cconv.h"

// Sterowanie rysowaniem ikony
#define DRAW_ICON        0
#define DRAW_MINIICON    1



// Prototypy funkcji
LONG ASUAAPI GrfDrawBitmap(HPS hps, HBITMAP hbmColor, HBITMAP hbmMono, POINTL *pos, BOOL flPattern);

#endif //_GRAPHICS_H_
/*
 * $Log: $
 *
 */
