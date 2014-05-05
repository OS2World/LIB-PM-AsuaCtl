/* Asuactl.dll
 * Pasek narz©dzi - no˜nik.
 *
 * (c) 1999 Wojciech Gazda
 *
 * carrier.h
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/06/27 12:36:13 $
 * $Name:  $
 * $RCSfile: carrier.h $
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


// Dodatkowe parametry TDQ_* niedost©pne dla u¾ytkownika
#define TDQ_WIDTH        0x00000000     // ½¥danie odczytu szeroko˜ci
#define TDQ_HEIGHT       0x00010000     // ½¥danie odczytu wysoko˜ci



// Prototypy funkcji - moduˆ carrier.c
VOID  CrrCalcNewRect(TOOLADJ *tadj);
LONG  CrrInitialize(HWND hwnd, TOOLCTL *tcl);
LONG  CrrMinMaxFrame(HWND hwnd, SWP *newpos);
VOID  CrrMoveWithOwner(HWND hwnd, SWP *newpos, SWP *owner);
VOID  CrrPlaceControls(HWND hwnd);
LONG  CrrQueryHeight(TOOLADJ *tadj);
LONG  CrrQueryWidth(TOOLADJ *tadj);
VOID  CrrRedrawBorder(HWND hwnd);
LONG  CrrRotate(HWND hwnd, ULONG ulOptions);
LONG  CrrTrackRect(HWND hwnd);




/*
 * $Log: carrier.h $
 * Revision 1.1  1999/06/27 12:36:13  Wojciech_Gazda
 * Initial revision
 *
 */
