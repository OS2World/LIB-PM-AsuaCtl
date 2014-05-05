/* ==========================================================================
   Win Button (Custom Control)
   ===========================

    $RCSfile: WINBUTT.H $
      Author: Przemyslaw_Dobrowolski <dobrawka@asua.org.pl>
       $Date: 1999/08/14 13:13:11 $
   $Revision: 2.0 $
      $State: Exp $
       Notes: Zast©pnik WC_BUTTON

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


#ifndef __WINBUTT_H__
#define __WINBUTT_H__

#define WC_WINBUTTON "WC_WINBUTTON"

#define BS_FLATBUTTON  0x0001
#define BS_OWNERDRAW   0x0008

#define BS_ALIGNLEFT   0x0000
#define BS_ALIGNRIGHT  0x0010
#define BS_ALIGNTOP    0x0020
#define BS_ALIGNBOTTOM 0x0030

#define BS_THINFRAME   0x0080    // ½¥danie rysowania cienkiego obramowania

#define BS_AUTOTEXT    0x0400    // Gdy tekst si© nie mie˜ci wypisywana
                                 // jest tylko jego cz©˜† a dalej wpisywane s¥
                                 // '...'

// Nowe kody notyfikacji dla przycisku
#define BN_MOUSEENTER       4  // Mysz weszˆa w obr©b przycisku
#define BN_MOUSELEAVE       5  // Mysz zeszˆa z przycisku
#define BN_MOUSECHANGE      6  // Mysz przeskoczyˆa na inny przycisk (Not supported YET!)
#define BN_PRESPARAMCHANGED 7  // Zmieniˆy si© parametry prezentacji
#define BN_BUTTON2CLICKED   8  // Drugi przycisk myszy zostaˆ przyci˜ni©ty

#define BM_QUERYHEIGHT  WM_USER + 25 // Pytanie o minimaln¥ wysoko˜†
#define BM_QUERYWIDTH   WM_USER + 26 // Pytanie o minimaln¥ szeroko˜†

// Definicja bitmap zaszytych w bibliotece
#define BBMP_OK     1
#define BBMP_CANCEL 2
#define BBMP_HELP   3

#ifdef __WATCOMC__
#pragma pack(push, 1)
#else
#pragma pack(1)
#endif

typedef struct _WBTNCDATA    /* btncd */
{
  USHORT  cb;               // dˆugo˜† struktury
  USHORT  idStdBitmap;      // numer identyfikacyjny bitmapy zaszytej w resourcach DLL'a
  USHORT  fsHiliteState;    // Stan przycisku
  LHANDLE hColorImage;      // Uchwyt do bitmapy (styl normalny)
  LHANDLE hColorDisImage;   // Uchwyt do bitmapy stylu zablokowanego
  LHANDLE hColorPressImage; // Uchwyt do bitmapy (styl wci˜ni©ty)
  LHANDLE hColorHiliteImage;// Uchwyt do bitmapy (styl mysz na przycisku)
  LHANDLE hImage;           // Uchwyt mapy bitowej u¾ywanej jako maski
  LHANDLE hImageDis;        // Uchwyt mapy bitowej u¾ywanej jako maski w 
  LHANDLE hImagePress;      // Uchwyt maski bitowej (styl wci˜ni©ty)
  LHANDLE hImageHilite;     // Uchwyt maski bitowej (styl mysz na przycisku)
                            // stanie zablokownym
} WBTNCDATA;
typedef WBTNCDATA *PWBTNCDATA;     
#ifdef __WATCOMC__
#pragma pack(pop)
#else
#pragma pack()
#endif


LONG APIENTRY BtnRegister(HAB hab);

#endif /* __WINBUTT_H__ */
/* ==========================================================================
 *
 * $Log: WINBUTT.H $
 * Revision 2.0  1999/08/14 13:13:11  Przemyslaw_Dobrowolski
 * Prawie nic. Automatyczny RCS.
 *
 * Revision 1.5  1999/06/08 08:38:27  Przemyslaw_Dobrowolski
 * Zmiana WBCDATA:
 * - Dodano uchwyty do map bitowych i mask dla zdarzen
 *   a) przycisk wcisniety
 *   b) na przycisku jest mysz [tylko flatbutton]
 *
 * Revision 1.4  1999/05/19 19:55:28  Przemyslaw_Dobrowolski
 * Dodane BN_BUTTON2CLICKED
 *
 * Revision 1.3  1999/04/25 20:21:46  Przemyslaw_Dobrowolski
 * Dodano: BS_AUTOTEXT
 *
 * Revision 1.2  1999/04/16 11:01:26  Przemyslaw_Dobrowolski
 * Cos bylo zmieniane... Ale nie wiem co?
 *
 * Revision 1.1  1999/03/18 12:45:36  Przemyslaw_Dobrowolski
 * Initial revision
 *
 */
