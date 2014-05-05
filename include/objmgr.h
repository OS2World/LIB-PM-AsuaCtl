/* Asuactl.dll
 * Pasek narz©dzi - object manager.
 *
 * (c) 1999 Wojciech Gazda
 *
 * objmgr.h
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/06/27 12:36:27 $
 * $Name:  $
 * $RCSfile: objmgr.h $
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
#define INTC_OBJECTPAD   (PSZ)"INTC_OBJECTPAD"    // Okno przechowuj¥ce obiekty
#define OBJ_CLASSIZE     15                       // Dˆugo˜† nazwy klasy


// Typy danych




// Kody potwierdzeä przekazywane za pomoc¥ komunikatu TM_OBJECTCONTROL
#define TNC_REDRAWBUTTON      1    // ½¥danie przerysowania przycisku
#define TNC_RELEASEGROUP      2    // ½¥danie zwolnienia wszystkich przycisk¢w w grupie
#define TNC_STARTTIP          3    // ½adanie rozpocz©cia wy˜wietlania podpowiedzi



// Wewn©trzne atrybuty obiekt¢w niedost©pne dla u¾ytkownika
// pole: flAttribute struktury OBJCTL.
#define TBO_BUTTONPRESS       0x00010000     // Przycisk jest naci˜ni©ty




// Prototypy funkcji
MRESULT EXPENTRY ObjectPadProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
LONG ObjDeleteObject(HWND hwnd, ULONG objId);
LONG ObjIdFromPosition(HWND hwnd, ULONG pos);
LONG ObjInitialize(TOOLCTL *tcl);
LONG ObjInsertObject(HWND hwnd, TOOLOBJ *tobj, TOOLWIN *twin);
LONG ObjInsertTooltip(HWND hwnd, ULONG objId, PSZ tip);
LONG ObjMoveObject(HWND hwnd, ULONG objId, ULONG newpos, ULONG flAttrs);
LONG ObjQueryObject(HWND hwnd, ULONG objId, TOOLOBJ *tobj);
LONG ObjQueryObjectCount(HWND hwnd);
LONG ObjQueryPosition(HWND hwnd, ULONG objId);
LONG ObjQueryTip(HWND hwnd, ULONG objId, ULONG ulBufSize, PSZ TipText);
HWND ObjQueryHandle(HWND hwnd, ULONG id);
LONG ObjQueryWidth(TOOLCTL *tcl, ULONG flNewState);
LONG ObjQueryHeight(TOOLCTL *tcl, ULONG flNewState);
VOID ObjReleaseResources(TOOLCTL *tcl);
LONG ObjSetObject(HWND hwnd, ULONG objId, ULONG ulOptions, TOOLOBJ *tobj);
LONG ObjShowObject(HWND hwnd, ULONG id, ULONG bNewState);


/*
 * $Log: objmgr.h $
 * Revision 1.1  1999/06/27 12:36:27  Wojciech_Gazda
 * Initial revision
 *
 */
