// Program testowy
// (c) 1997-1999  Wojciech Gazda

#define  INCL_WINSHELLDATA

// Standardowe definicje OS/2
#define  INCL_DOSMODULEMGR
#define  INCL_DOSPROCESS
#define  INCL_GPILOGCOLORTABLE
#define  INCL_GPIPRIMITIVES
#define  INCL_WIN
#include <os2.h>

// Funkcje biblioteczene C
#include <stdio.h>
#include <malloc.h>
#include <mem.h>


// Definicje lokalne
#include <asuactl.h>
#include "../../graphics.h"


/* Staˆe */
#define  TEXTSIZE       0x80            // Max rozmiar staˆych tekstowych
#define  SET_MSGQUEUE   0x00000001      // Utworzenie kolejki komunikat¢w
#define  SET_RESOURCE   0x00000002      // Zaˆadowanie zasob¢w
#define  SET_MAINWIN    0x00000004      // Utworzenie okna gˆ¢wnego
#define  SET_PANELINIT  0x00000008      // Zainiciowanie danych paneli


/* Struktury danych - konfiguracja programu */


/* Zmienne globalne */
HMODULE  asuctl;              // Uchwyt biblioteki DLL zawiaraj¥cej okna kontrolne
HWND     winframe;            // Uchwyt ramki gˆ¢wnego okna programu
HWND     mainwindow;          // Uchwyt przestrzeni roboczej gˆ¢wnego okna programu
HWND     toolbar;             // Uchwyt paska narz©dzi
HAB      anchor;              // Wska«nik do obszaru roboczego Presentation Manager
HMQ      mainqueue;           // Wska«nik do kolejki komunikat¢w gˆ¢wnego okna
CHAR     classname[TEXTSIZE]; // Nazwa aplikacji
ULONG    objmove;             // Zmienna struj¥ca przenoszeniem obiekt¢w


// Prototypy funkcji
VOID    APIENTRY exitproc(USHORT usTermCode);
MRESULT EXPENTRY mainwinproc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
VOID    InsertObjects(HWND tool);
VOID    DrawTestBitmaps(HWND hwnd);



// Procedura gˆ¢wna programu
int main(void)
{ QMSG   qmsg;                   // Struktura definiuj¥ca komunikat
  ULONG  framestyle;             // Zmienna kontroluj¥ca wygl¥d okna
  ULONG  classtyle;              // Standardowe atrybuty okien programu
  ULONG  errors;                 // Znaczniki poprawnego wykonania funkcji
  LONG   rc;

  errors = 0L;

  // Inicjacja procedur Presentation Manager
  anchor = WinInitialize(0);
  if(anchor == NULLHANDLE) return(0);

//// Inicjacja biblioteki asuctl.dll
//{ LONG _syscall (*AsuCtlInitialize)(HAB hab);
//
//  asuctl = NULLHANDLE;
//  rc = DosLoadModule(NULL, 0L, "ASUACTL", &asuctl);
//  if(asuctl != NULLHANDLE)
//  { // Odczyt adresu funkcji inicjuj¥cej
//    DosQueryProcAddr(asuctl, 0, "ASUCTLINITIALIZE", &AsuCtlInitialize);
//    if(AsuCtlInitialize != NULL)
        AsuCtlInitialize(anchor);
//  }
//  else
//  { DosBeep(1000, 500);
//    WinTerminate(anchor);
//    return(0);
//  }
//}

  rc = AsuGetComponentVersion(WC_TOOLBAR);
  rc = AsuGetComponentVersion(WC_STATUS);

  // Utworzenie kolejki komunikat¢w
  mainqueue = WinCreateMsgQueue(anchor, 0);
  if(mainqueue == NULLHANDLE) goto error;
  errors |= SET_MSGQUEUE;

  // Rejestracja procedury koäcz¥cej program
  DosExitList(EXLST_ADD,(PFNEXITLIST)exitproc);

  // Zaˆadowanie nazwy programu z zasob¢w
  memset(classname, 0, TEXTSIZE);
  strcpy(classname, "Test paska narz©dzi");

  // Cechy okien programu
  classtyle = 0L;
  // Rejestracja aplikacji w systemie
  if(!WinRegisterClass(anchor,             // Identyfikator obszaru danych PM
                      (PSZ)classname,      // Nazwa aplikacji
                      (PFNWP)mainwinproc,  // Procedura obsˆugi okna
                      classtyle,           // Styl okien potomnych
                      0L))                 // Dodatkowy obszar danych dla ka¾dego okna potomnego
  { goto error;
  }

  // Utworzenie gˆ¢wnego okna programu
  framestyle = FCF_TITLEBAR    | FCF_SYSMENU     |
               FCF_MINMAX      | FCF_TASKLIST   |
               FCF_NOBYTEALIGN | FCF_SIZEBORDER;
  winframe =
   WinCreateStdWindow(HWND_DESKTOP,       // Wska«nik do okna macierzystego
                      WS_VISIBLE |
                      WS_ANIMATE,         // Okno widoczne na ekranie
                      (PVOID)&framestyle, // Wygl¥d okna
                      (PSZ)classname,     // Tytuˆ okna
                      (PSZ)classname,     // Tekst na belce tytuˆowej
                      WS_VISIBLE |
                      WS_CLIPCHILDREN |
                      WS_CLIPSIBLINGS,    // Styl okna potomnego
                      0L,                 // Wska«nik do zasob¢w
                      0L,                 // Identyfikator ikony w menu
                      (PHWND)&mainwindow);// Uchwyt obszaru client (en©trze okna)
  if(!winframe)
  { goto error;
  }
  errors |= SET_MAINWIN;

  // Ustawienie pozycji i rozmiar¢w okna gˆ¢wnego
  WinSetWindowPos(winframe,             // Wska«nik do okna gˆ¢wnego
                  HWND_TOP,             // Okno na wierzchu
                  0, 0,                 // Pozycja x,y na ekranie
                  700, 480,             // Rozmiary h, v
                  SWP_SIZE | SWP_MOVE | // Opcje
                  SWP_SHOW | SWP_ACTIVATE);

  // Utworzenie paska narz©dzi
  { TOOLINFO tnf;

    tnf.cb = sizeof(TOOLINFO);
    tnf.flToolAttrs = TBA_FRAME | TBA_FIXEDTOP;
    tnf.usMaxLength = 500;

    toolbar = WinCreateWindow(mainwindow, WC_TOOLBAR, "Pr¢bny pasek narz©dzi",
                 WS_VISIBLE | WS_MAXIMIZED | TBS_MOVEBUTTON | TBS_FIXBUTTON |
                 TBS_MOVEWITHOWNER | TBS_ENABLEMINIMIZE | TBS_ROTATEBUTTON,
                 0, 0, 0, 0,
                 mainwindow,
                 HWND_TOP,
                 1000,
                 &tnf, NULL);

    // Wyˆ¥czenie przycisku obracania
    WinSendMsg(toolbar, TM_ENABLEROTATE, FALSE, 0);

    // Umieszczenie obiekt¢w na pasku
    InsertObjects(toolbar);

    // Pozycjonowanie paska
    WinSendMsg(toolbar, TM_AUTOSIZE, 0, 0);
  }

  rc = WinQuerySysValue(HWND_DESKTOP, SV_TRACKRECTLEVEL);

  // Gˆ¢wna p©tla programu
  while(WinGetMsg(anchor, (PQMSG)&qmsg, 0, 0, 0))
  { // Standardowa reakcja na komunikat
    WinDispatchMsg(anchor, (PQMSG)&qmsg);
  }

error:
  if(errors & SET_MAINWIN)
  { WinDestroyWindow(toolbar);     // Zamkni©cie paska narz©dzi
    WinDestroyWindow(mainwindow);  // Zamkni©cie okna
  }
  if(errors & SET_MSGQUEUE)
    WinDestroyMsgQueue(mainqueue); // Zlikwidowanie kolejki komunikat¢w
  WinTerminate(anchor);            // Zwolnienie zasob¢w
  return(0);                       // Wykonanie zakoäczone sukcesem
}




// Procedura koäcz¥ca program
VOID APIENTRY exitproc(USHORT usTermCode)
{ if(WinIsWindow(anchor, winframe)) // Sprawdzenie czy jest otwarte okno gˆ¢wne
    WinDestroyWindow(winframe);     // Zniszczenie okna gˆ¢wnego je˜li jest
  WinDestroyMsgQueue(mainqueue);    // Zniszczenie kolejki komunikat¢w
  WinTerminate(anchor);             // Zwolnienie zasob¢w Presentation Manager
  // Przej˜cie do kolejnej procedury wyj˜cia zawartej na li˜cie
  DosExitList(EXLST_EXIT, (PFNEXITLIST)exitproc);
  usTermCode;
}




// Procedura obsˆugi gˆ¢wnego okna
MRESULT EXPENTRY mainwinproc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
  switch(msg)
  {
    case WM_COMMAND:
      //DosBeep(1000, 20);
      break;

    case WM_CONTROL:     // Wspomaganie sterowaniem paskiem narz©dzi
      // Sprawdzenie czy operacja dotyczy paska narz©dzi
      if(SHORT1FROMMP(mp1) == 1000)
      {
        // Reakcja na poszczeg¢lne kody potwierdzeä
        switch(SHORT2FROMMP(mp1))
        { case TN_BEGINTRACK:        // mp2 zawiera wska«nik do TOOLTRACK
            { TOOLTRACK *trc;

              trc = PVOIDFROMMP(mp2);
              if(trc->flState & TST_FIXED)
              { // Ustalenie nowego obszaru ograniczaj¥cego
                WinQueryWindowRect(trc->hwndParent, &trc->rclBoundary);
              }
            }
            break;

          case TN_TRACKING:          // mp2 zawiera wska«nik do POINTL
            break;

          case TN_ENDTRACK:          // mp2 zawiera wska«nik do TOOLTRACK
            { TOOLTRACK *trc;
              TOOLADJ    tadj;

              trc = PVOIDFROMMP(mp2);
              if(trc->flState & TST_FIXED)
              {
                // Odczyt nowej pozycji i wymiar¢w okna
                tadj.hwndToolBar = trc->hwndToolBar;
                tadj.hwndParent  = trc->hwndParent;
                tadj.flState     = trc->flState;
                tadj.flAttrs     = trc->flAttrs;
                WinSendMsg(trc->hwndToolBar, TM_CALCNEWRECT, MPFROMP(&tadj), 0L);
                trc->rclSizePos = tadj.rclSizePos;
              }
            }
            break;

          case TN_AUTOSIZE:
            break;

          case TN_MINIMIZE:
            { TOOLADJ *tadj;

              tadj = PVOIDFROMMP(mp2);
              tadj->flAttrs &= ~TBA_FRAME;

              WinSendMsg(toolbar, TM_ENABLEROTATE, MPFROMLONG(FALSE), 0);
              WinSendMsg(toolbar, TM_ROTATE, MPFROMLONG(TBR_HORIZONTAL), 0);

              if(!(tadj->flState & (TST_MINIMIZED | TST_FIXED)))
                WinSetParent(toolbar, hwnd, TRUE);

              tadj->flState &= ~(TST_FIXED | TST_ROTATED);
              tadj->flState |=  TST_MINIMIZED;
              WinSendMsg(toolbar, TM_CALCNEWRECT, MPFROMP(tadj), 0);
            }
            break;

          case TN_FIXTOOL:
            { TOOLADJ *tadj;

              tadj = PVOIDFROMMP(mp2);
              tadj->flAttrs |= TBA_FRAME;

              WinSendMsg(toolbar, TM_ENABLEROTATE, MPFROMLONG(FALSE), 0);
              WinSendMsg(toolbar, TM_ROTATE, MPFROMLONG(TBR_HORIZONTAL), 0);

              if(!(tadj->flState & TST_MINIMIZED))
                WinSetParent(toolbar, hwnd, TRUE);

              tadj->flState &= ~(TST_MINIMIZED | TST_ROTATED);
              tadj->flState |=  TST_FIXED;
              WinSendMsg(toolbar, TM_CALCNEWRECT, MPFROMP(tadj), 0);
            }
            break;

          case TN_RESTORE:
            { TOOLADJ *tadj;
              POINTL   pos;
              RECTL    wsize;

              tadj = PVOIDFROMMP(mp2);
              tadj->flState &= ~(TST_FIXED | TST_MINIMIZED);
              WinSendMsg(toolbar, TM_ENABLEROTATE, MPFROMLONG(TRUE), 0);
              if(tadj->flAttrs & TBA_VERTICAL)
              { // Obr¢cenie okna w przypadku gdy byˆo pionowo w stanie floating
                WinSendMsg(toolbar, TM_ROTATE, MPFROMLONG(TBR_VERTICAL), 0);
              }

              tadj->rclSizePos.xRight  = tadj->rclSizePos.xLeft +
                (LONG)WinSendMsg(toolbar, TM_QUERYWIDTH,  MPFROMLONG(TDQ_CURRENT), 0);
              tadj->rclSizePos.yBottom = tadj->rclSizePos.yTop -
                (LONG)WinSendMsg(toolbar, TM_QUERYHEIGHT, MPFROMLONG(TDQ_CURRENT), 0);

              pos.x = (SHORT)WinQueryWindowUShort(tadj->hwndToolBar, QWS_XRESTORE);
              pos.y = (SHORT)WinQueryWindowUShort(tadj->hwndToolBar, QWS_YRESTORE);
              if((pos.x == 0) && (pos.y == 0))
              { WinQueryWindowRect(tadj->hwndToolBar, &wsize);
                WinMapWindowPoints(tadj->hwndToolBar, HWND_DESKTOP, (POINTL *)&wsize, 2);
                tadj->rclSizePos.xLeft   += wsize.xLeft;
                tadj->rclSizePos.xRight  += wsize.xLeft;
                tadj->rclSizePos.yBottom += wsize.yBottom;
                tadj->rclSizePos.yTop    += wsize.yBottom;
              }
              WinSetParent(toolbar, HWND_DESKTOP, TRUE);
            }
            break;

          case TN_ROTATE:
            { TOOLADJ *tadj;

              tadj = PVOIDFROMMP(mp2);
              if(!(tadj->flState & (TST_MINIMIZED | TST_FIXED)))
              { if(tadj->flState & TST_ROTATED)
                  tadj->flAttrs |= TBA_VERTICAL;
                else tadj->flAttrs &= ~TBA_VERTICAL;
              }
            }
        }
      }
      break;

    case WM_PAINT:
      DrawTestBitmaps(hwnd);
      break;

    case WM_BUTTON1DOWN:
      //WinSetFocus(HWND_DESKTOP, hwnd);
      break;

    case WM_BUTTON2CLICK:
      { HWND rotate;

        rotate = WinWindowFromID(toolbar, TCID_ROTATE);
        if(rotate != NULLHANDLE)
        { if(WinIsWindowEnabled(rotate))
            WinSendMsg(toolbar, TM_ENABLEROTATE, FALSE, 0);
          else
            WinSendMsg(toolbar, TM_ENABLEROTATE, MPFROMLONG(TRUE), 0);
        }
      }
      break;

    case WM_BUTTON3CLICK:
      { ULONG id;

        id = (ULONG)WinSendMsg(toolbar, TM_IDFROMPOSITION, 0, 0);
        WinSendMsg(toolbar, TM_DELETEOBJECT, MPFROMLONG(id), 0);
      }
      break;

    case WM_CHAR:
      if((SHORT1FROMMP(mp1) & KC_VIRTUALKEY) && !(SHORT1FROMMP(mp1) & KC_KEYUP))
      { switch(SHORT2FROMMP(mp2))
        { case VK_F9:
            if(!(SHORT1FROMMP(mp1) & KC_CTRL))
              WinSendMsg(toolbar, TM_SHOWOBJECT, MPFROMLONG(1), MPFROMLONG(FALSE));
            else WinSendMsg(toolbar, TM_SHOWOBJECT, MPFROMLONG(1), MPFROMLONG(TRUE));
            break;

          case VK_F8:
            { HWND hf2;

              hf2 = (HWND)WinSendMsg(toolbar, TM_QUERYHANDLE, MPFROMLONG(2), 0);
              if(hf2 != NULLHANDLE)
              { if(WinIsWindowEnabled(hf2) == TRUE)
                  WinEnableWindow(hf2, FALSE);
                else WinEnableWindow(hf2, TRUE);
              }
            }
            break;

          case VK_F2:
            { switch(objmove)
              { case 0:       // Przeniesienie przycisku F10
                  WinSendMsg(toolbar, TM_MOVEOBJECT, MPFROM2SHORT(4, 1), MPFROMLONG(TBO_BEGINALIGN));
                  objmove ++; break;

                case 1:       // Przeniesienie przycisku F11
                  WinSendMsg(toolbar, TM_MOVEOBJECT, MPFROM2SHORT(5, 0), MPFROMLONG(TBO_BEGINALIGN));
                  objmove ++; break;

                case 2:       // Przeniesienie okna WC_ENTRYFIELD
                  WinSendMsg(toolbar, TM_MOVEOBJECT, MPFROM2SHORT(100, TB_END), MPFROMLONG(TBO_ENDALIGN));
                  objmove ++; break;

                case 3:       // Przeniesienie pierwszego separatora na ˜rodek
                  WinSendMsg(toolbar, TM_MOVEOBJECT, MPFROM2SHORT(200, 6), MPFROMLONG(TBO_CENTERALIGN));
                  objmove ++; break;

                default:
                  DosBeep(1000, 20);
              }
            }
            break;

          case VK_F3:
            { TOOLINFO tnfo;
              WNDPARAMS wnd;

              wnd.fsStatus  = WPM_CTLDATA | WPM_TEXT;
              wnd.pCtlData  = &tnfo;
              wnd.pszText   = "A to inny tytuˆ!";
              tnfo.flToolAttrs = TBA_FRAME | TBA_FIXEDBOTTOM;
              tnfo.usMaxLength = 300;
              WinSendMsg(toolbar, WM_SETWINDOWPARAMS, MPFROMP(&wnd), 0);
            }
            break;
        }
      }
      break;

    case WM_SIZE:
      WinSendMsg(toolbar, TM_AUTOSIZE, 0, 0);
      break;

  }

  // Standardowa interpretacja komunikat¢w ignorowanych przez program
  return(WinDefWindowProc(hwnd, msg, mp1, mp2));
}





// Pr¢bna procedura dodaj¥ca obiekty do paska narz©dzi
VOID InsertObjects(HWND tool)
{  TOOLOBJ obj;     // Struktura kontrolna dodawanego obiektu
   TOOLWIN win;     // Dodatkowa struktura definiuj¥ca okna

   // Separator na pocz¥tku paska
   obj.iPosition   = 0;
   obj.ulGroup     = 0;       // Grupa 0
   obj.cx          = 0;       // Szeroko˜† pocz¥tkowa
   obj.cy          = 0;       // Wysoko˜† pocz¥tkowa
   obj.id          = 200;     // Identyfikator obiektu
   obj.flAttribute = TBO_SEPARATOR | TBO_BEGINALIGN | SPS_CONCAVE;
   WinSendMsg(tool, TM_INSERTOBJECT, MPFROMP(&obj), 0);

   // Tworzenie przycisk¢w
   // Przyciski justowane od lewej - grupa 0
   obj.iPosition   = TB_END;  // Poˆo¾enie na pocz¥tku paska narz©dzi
   obj.ulGroup     = 0;       // Grupa 0
   obj.cx          = 35;      // Szeroko˜† pocz¥tkowa
   obj.cy          = 25;      // Wysoko˜† pocz¥tkowa
   obj.id          = 1;       // Identyfikator obiektu
   obj.flAttribute = TBO_STDBUTTON | TBO_BEGINALIGN | TBO_PUSHBUTTON;
   win.pszClass    = /* WC_WINBUTTON */ WC_BUTTON;
   win.pszName     = "F1";
   win.flStyle     = /*BS_FLATBUTTON |*/ BS_PUSHBUTTON | BS_NOPOINTERFOCUS | WS_VISIBLE;
   win.pCtlData    = NULL;
   win.pPresParams = NULL;
   WinSendMsg(tool, TM_INSERTOBJECT, MPFROMP(&obj), MPFROMP(&win));

   obj.iPosition   = TB_END;
   obj.id          = 2;
   win.pszName     = "F2";
   WinSendMsg(tool, TM_INSERTOBJECT, MPFROMP(&obj), MPFROMP(&win));

   obj.iPosition   = TB_END;
   obj.id          = 3;
   win.pszName     = "F3";
   WinSendMsg(tool, TM_INSERTOBJECT, MPFROMP(&obj), MPFROMP(&win));

   // Separator po grupie przycisk¢w
   obj.iPosition   = TB_END;
   obj.ulGroup     = 0;       // Grupa 0
   obj.cx          = 0;       // Szeroko˜† pocz¥tkowa
   obj.cy          = 0;       // Wysoko˜† pocz¥tkowa
   obj.id          = 201;     // Identyfikator obiektu
   obj.flAttribute = TBO_SEPARATOR | TBO_BEGINALIGN | SPS_CONCAVE;
   WinSendMsg(tool, TM_INSERTOBJECT, MPFROMP(&obj), 0);



   // Obiekty justowane do prawej - grupa 1
   // Separator przed grup¥ przycisk¢w
   obj.iPosition   = TB_END;
   obj.ulGroup     = 1;
   obj.cx          = 0;       // Szeroko˜† pocz¥tkowa
   obj.cy          = 0;       // Wysoko˜† pocz¥tkowa
   obj.id          = 202;     // Identyfikator obiektu
   obj.flAttribute = TBO_SEPARATOR | TBO_ENDALIGN | SPS_CONCAVE;
   WinSendMsg(tool, TM_INSERTOBJECT, MPFROMP(&obj), 0);

   // Przyciski justowane od prawej,
   obj.iPosition   = TB_END;
   obj.ulGroup     = 1;
   obj.cx          = 35;
   obj.cy          = 25;
   obj.id          = 4;
   obj.flAttribute = TBO_STDBUTTON | TBO_ENDALIGN | TBO_CHECKBOX;
   win.pszClass    = /* WC_WINBUTTON */ WC_BUTTON;
   win.pszName     = "F10";
   win.flStyle     = BS_NOPOINTERFOCUS | BS_PUSHBUTTON | /* BS_FLATBUTTON |*/ WS_VISIBLE;
   win.pCtlData    = NULL;
   win.pPresParams = NULL;
   WinSendMsg(tool, TM_INSERTOBJECT, MPFROMP(&obj), MPFROMP(&win));
   WinSendMsg(tool, TM_INSERTTOOLTIP, MPFROMLONG(obj.id), MPFROMP("Przycisk F10"));


   obj.iPosition   = TB_END;
   obj.id          = 5;
   obj.flAttribute = TBO_STDBUTTON | TBO_ENDALIGN;
   win.pszName     = "F11";
   WinSendMsg(tool, TM_INSERTOBJECT, MPFROMP(&obj), MPFROMP(&win));
   WinSendMsg(tool, TM_INSERTTOOLTIP, MPFROMLONG(obj.id), MPFROMP("Przycisk F11"));

   obj.iPosition   = TB_END;
   obj.id          = 6;
   win.pszName     = "F12";
   win.flStyle     = BS_PUSHBUTTON | BS_NOPOINTERFOCUS | /*BS_FLATBUTTON | BS_THINFRAME |*/ WS_VISIBLE;
   WinSendMsg(tool, TM_INSERTOBJECT, MPFROMP(&obj), MPFROMP(&win));
   WinSendMsg(tool, TM_INSERTTOOLTIP, MPFROMLONG(obj.id), MPFROMP("Przycisk F12"));



   // Obiekty ˜rodkowane, grupa 4
   obj.iPosition   = 5;
   obj.ulGroup     = 4;
   obj.cx          = 120;
   obj.cy          = 25;
   obj.id          = 100;
   obj.flAttribute = TBO_STDWINDOW | TBO_CENTERALIGN /* | TBO_HIDE | TBO_DISABLE */;
   win.pszClass    = WC_ENTRYFIELD;
   win.pszName     = "WC_ENTRYFIELD";
   win.flStyle     = ES_MARGIN;
   win.pCtlData    = NULL;
   win.pPresParams = NULL;
   WinSendMsg(tool, TM_INSERTOBJECT, MPFROMP(&obj), MPFROMP(&win));
   WinSendMsg(tool, TM_INSERTTOOLTIP, MPFROMLONG(obj.id), MPFROMP("Co by tu wprowadzi†?"));

   obj.iPosition   = 6;
   obj.id          = 101;
   obj.cx          = 35;
   obj.cy          = 25;
   obj.flAttribute = TBO_STDBUTTON | TBO_CENTERALIGN | TBO_RADIOBUTTON;
   win.pszClass    = WC_BUTTON /* WC_WINBUTTON */;
   win.pszName     = "F7";
   win.flStyle     = BS_PUSHBUTTON | BS_NOPOINTERFOCUS | /* BS_FLATBUTTON | BS_THINFRAME |*/ WS_VISIBLE;
   WinSendMsg(tool, TM_INSERTOBJECT, MPFROMP(&obj), MPFROMP(&win));

   obj.iPosition   = 7;
   obj.id          = 102;
   obj.flAttribute = TBO_STDBUTTON | TBO_CENTERALIGN;
   win.pszClass    = WC_BUTTON /* WC_WINBUTTON */;
   win.pszName     = "F8";
   win.flStyle     =  BS_PUSHBUTTON | BS_NOPOINTERFOCUS | /* BS_FLATBUTTON | BS_THINFRAME |*/ WS_VISIBLE;
   WinSendMsg(tool, TM_INSERTOBJECT, MPFROMP(&obj), MPFROMP(&win));
}




// Testowanie funkcji GrfDrawBitmap
//
VOID  DrawTestBitmaps(HWND hwnd)
{ HPS     hps;           // Uchwyt presentation space
  HBITMAP back;          // Bitmapa rysowana w tle
  HBITMAP color;         // Bitmapa kolorowa
  HBITMAP mono;          // Bitmapa monochromatyczna
  RECTL   wsize;         // Rozmiary okna
  POINTL  pos;           // Wsp¢ˆrz©dne og¢lnego przeznaczenia

  hps = WinBeginPaint(hwnd, NULLHANDLE, NULL);
  GpiErase(hps);
  WinQueryWindowRect(hwnd, &wsize);

  // Wy˜wietlenie tˆa
  back = GpiLoadBitmap(hps, 0, 1000, 0, 0);
  pos.x = wsize.xLeft; pos.y = wsize.yTop - 200;
  WinDrawBitmap(hps, back, NULL, &pos, 0, 0, DBM_NORMAL);

  // Odczyt bitmapy kolorowej
  color = GpiLoadBitmap(hps, 0, 1001, 0, 0);
  // Odczyt bitmapy monochromatycznej
  mono  = GpiLoadBitmap(hps, 0, 1002, 0, 0);

  GpiSetPattern(hps, PATSYM_HALFTONE);
  pos.x += 20; pos.y += 20;
  GrfDrawBitmap(hps, color, mono, &pos, TRUE);


  GpiDeleteBitmap(back);
  GpiDeleteBitmap(color);
  GpiDeleteBitmap(mono);
  WinEndPaint(hps);
}


