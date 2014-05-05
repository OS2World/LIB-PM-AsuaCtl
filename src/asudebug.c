/*
 * Biblioteka okien kontrolnych - Asua Controls
 * Generowanie raport¢w o bˆ©dach
 * (c) 1999 Wojciech Gazda
 *
 * asudebug.c
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


// Deklaracje OS/2
#define  INCL_WINERRORS
#define  INCL_DOSDATETIME
#define  INCL_DOSFILEMGR
#define  INCL_DOSMEMMGR
#include <os2.h>

// Funkcje biblioteczne C
#include <string.h>
#include <stdio.h>

// Deklaracje lokalne
#include "asudebug.h"


// Prototypy funkcji
VOID ASUAAPI AsuLogPMError(PSZ classname, ULONG PMerror, PSZ fname, PSZ commnet);



// Dodanie wpisu do pliku asuactl.err
//
// Parametry:
//   classname      - [parametr] klasa okna kontrolnego, kt¢rego dotyczy wpis
//   PMerror        - [parametr] kod bˆ©du zwracany przez WinGetLastError (opcjonalny)
//   fname          - [parametr] nazwa funkcji (lub moduˆu) w kt¢rej wyst¥piˆ bˆ¥d
//   comment        - [parametr] kr¢tki komentarz
//
VOID ASUAAPI AsuLogPMError(PSZ classname, ULONG PMerror, PSZ fname, PSZ comment)
{ HFILE  errlog;         // Uchwyt pliku zawieraj¥cego wykaz bˆ©d¢w
  ULONG  action;         // Czynno˜† wykonana przez funkcj© DosOpen
  PSZ    errdesc;        // Opis bˆ©du
  ULONG  errsize;        // Rozmiar opisu
  LONG   rc;             // Kody bˆ©d¢w zwracane przez funkcje

  // Alokacja pami©ci dla opisu bˆ©du
  rc = DosAllocMem(&errdesc, LOGSIZE + 1, PAG_COMMIT | PAG_READ | PAG_WRITE);
  if(rc) return;
  *errdesc = 0; errsize = 0;

  // Zapami©tanie nazwy klasy
  errsize += sprintf(errdesc + errsize, "\nWindow Class: %s\n", classname == NULL ? "unknown" : classname);
  { DATETIME dt;

    // Odczyt daty i czasu
    DosGetDateTime(&dt);
    // Wy˜wietlenie daty
    errsize += sprintf(errdesc + errsize, "Date: %02d:%02d:%04d    ", dt.day, dt.month, dt.year);
    // Wy˜wietlenie czasu
    errsize += sprintf(errdesc + errsize, "Time: %02d:%02d:%02d\n\n", dt.hours, dt.minutes, dt.seconds);
  }

  // Funkcja
  errsize += sprintf(errdesc + errsize, "Module  : %s\n", fname == NULL ? "unknown" : fname);

  // Bˆ¥d PM
  if(PMerror)
  { errsize += sprintf(errdesc + errsize, "PM Error: 0x%04X, ", PMerror & 0xFFFF);
    errsize += sprintf(errdesc + errsize, "severity: 0x%X ",   PMerror >> 16);
    switch(PMerror >> 16)
    { case SEVERITY_NOERROR:       // Bezbˆ©dnie
        errsize += sprintf(errdesc + errsize, "[no error]\n"); break;
      case SEVERITY_WARNING:       // Ostrze¾enie
        errsize += sprintf(errdesc + errsize, "[warning]\n"); break;
      case SEVERITY_ERROR:         // Bˆ¥d - funkcja zostaˆa anulowana
        errsize += sprintf(errdesc + errsize, "[error]\n"); break;
      case SEVERITY_SEVERE:        // Bˆ¥d - funkcja nie zostaˆa anulowana (nie daˆo si©)
        errsize += sprintf(errdesc + errsize, "[severe error]\n"); break;
      case SEVERITY_UNRECOVERABLE: // Crash!
        errsize += sprintf(errdesc + errsize, "[unrecoverable error]\n"); break;
      default:
        errsize += sprintf(errdesc + errsize, "[unknown]\n"); break;
    }
  }

  // Komentarz
  if(comment != NULL)
    errsize += sprintf(errdesc + errsize, "Comment : %s\n", comment);
  // Stopka
  errsize += sprintf(errdesc + errsize, "\n-----------------------------\n", comment);

  // Otwarcie pliku w bie¾¥cym katalogu
  rc = DosOpen(LOGFILE, &errlog, &action, 0L,
               FILE_NORMAL,
               OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
               OPEN_FLAGS_WRITE_THROUGH | OPEN_FLAGS_NO_CACHE | OPEN_FLAGS_NOINHERIT |
               OPEN_SHARE_DENYNONE | OPEN_ACCESS_READWRITE,
               NULL);
  if(!rc)
  { // Przesuni©cie wska«nika na koniec pliku
    DosSetFilePtr(errlog, 0L, FILE_END, &action);
    // Zapisanie zawarto˜ci bufora w pliku
    DosWrite(errlog, errdesc, errsize, &action);
  }

  // Zamkni©cie pliku
  DosClose(errlog);
  // Zwolnienie pami©ci
  DosFreeMem(errdesc);

}

/*
 * $Log:$
 */
