#define INCL_PM
#define INCL_DOSERRORS
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#define INCL_DOSFILEMGR
#define INCL_DOSMISC
#define INCL_DOSPROCESS
#include <os2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "asuactl.h"
#include "dirdlg.h"

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

extern HMODULE hResource;

#define THREADSTACKSIZE 20960 

// User messages
#define WM_ADDTOCNR      WM_USER+1
#define WM_DIRADDEDTOCNR WM_USER+2
#define WM_QUERYSELECTED WM_USER+0

const CHAR  G_acDriveLetters[28] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";

#ifndef POLISH
const PSZ G_pszMyComputer = {"My Computer"};
#else
const PSZ G_pszMyComputer = {"M¢j Komputer"};
#endif

PFNWP G_pfnNormalCnrProc;    // Funkcja normalna dla kontenera

typedef struct _CNRENTRY
{
  RECORDCORE record;         // standardowy wska«nik rekoru w kontainerzez
  PSZ        szFullPathName; // peˆna ˜cie¾ka
} CNRENTRY, *PCNRENTRY;

typedef struct _THRPARAM
{
  ULONG flSendNotify;
  HWND  hwnd;
  ULONG cbDirs;
  PSZ   szDir[1];
} THRPARAM, *PTHRPARAM;

#pragma pack(1)

typedef struct _BPB  
{ 
  USHORT        BytesPerSector;
  UCHAR         SectorsPerCluster;
  USHORT        ReservedSectors;
  UCHAR         NumFATs;
  USHORT        MaxDirEntries;
  USHORT        TotalSectors;
  UCHAR         MediaType;
  USHORT        NumFATSectors;
  USHORT        SectorsPerTrack;
  USHORT        NumHeads;
  ULONG         HiddenSectors;
  ULONG         BigTotalSectors;
  UCHAR         Reserved_1[6];
} BPB, FAR *PBPB, *NPBPB;

typedef struct _DRIVEPARAMS
{
  struct _BPB bpb;
  USHORT  usCylinders; // no. of cylinders
  UCHAR   ucDeviceType;
  // device type; according to the IBM Control
  // Program Reference (see DSK_GETDEVICEPARAMS),
  // this value can be:
  // --  0:  48 TPI low-density diskette drive
  // --  1:  96 TPI high-density diskette drive
  // --  2:  3.5-inch 720KB diskette drive
  // --  3:  8-Inch single-density diskette drive
  // --  4:  8-Inch double-density diskette drive
  // --  5:  Fixed disk
  // --  6:  Tape drive
  // --  7:  Other (includes 1.44MB 3.5-inch diskette drive)
  // --  8:  R/W optical disk
  // --  9:  3.5-inch 4.0MB diskette drive (2.88MB formatted)
  USHORT  usDeviceAttrs;
} DRIVEPARAMS, *PDRIVEPARAMS;
#pragma pack()

static HPOINTER G_hMyComputer;
static HPOINTER G_hFolder;
static HPOINTER G_hDisk;
static HPOINTER G_hCDROM;
static HPOINTER G_hFloppy;

static TID G_tid=0;

static PSZ G_pszSelectedDir = NULL;

MRESULT EXPENTRY DirDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

VOID EnumDrives(PSZ pszBuffer,      // out: drive letters
                    const char *pcszFileSystem,  // in: FS's to match or NULL
                    BOOL fSkipRemoveables); // in: if TRUE, only non-removeable disks will be returned

VOID UtilRemoveFinalBackslash (PSZ p);
VOID _Optlink SearchThread(PVOID ulParams);
void SearchDirectories(HWND hwnd, PSZ szDirectory);
void AddDirsFromFF(HWND hwnd,PSZ szDirectory,FILEFINDBUF3 * pFData, ULONG cbCount);
void AddRecord(HWND hwnd,PCNRENTRY pRecord, PCNRENTRY pParent);
void AddDirToContainer(HWND hwnd, PSZ szFullDirectory, PSZ szPath, PCNRENTRY pParent);
void ExpandTree(HWND hwnd, PCNRENTRY pRecord);
void DestroyTree(HWND hwndCnr, PCNRENTRY pRecord);
BOOL UtilWarpSansPresent(VOID);
VOID UtilCenterWindow(HWND hwnd);
void AddSelectedDir(HWND hwndCnr,PSZ pszSelectedDir, PCNRENTRY pParent);
void dprintf(const char *fmt,...);
VOID DirDlgOnPaint(HWND hwnd);
APIRET UtilQueryDiskParams(ULONG ulLogicalDrive,
                           PDRIVEPARAMS pdp);

ULONG cnrhScrollToRecord(HWND hwndCnr,       // in: container window
                         PRECORDCORE pRec,   // in: record to scroll to
                         ULONG fsExtent,
                         BOOL KeepParent);

#define DIRDLG_TYPE_OK         0
#define DIRDLG_TYPE_OKCANCEL   1


typedef struct _DLGDATA
{
  PDIRDLG   pDlgData;
  PCNRENTRY pRecord;
} DLGDATA, *PDLGDATA;

BOOL APIENTRY AsuDirDlg(HWND hwndParent,HWND hwndOwner, PDIRDLG pdd)
{ HAB hab;
  HMQ hmq;
  QMSG  qmsg;
  BOOL success;
  HPOINTER hTemp;
  POINTERINFO ptrInfo;
  HMODULE asuactl;
  APIRET rc;
  ULONG dialog_res;
  DLGDATA dlgdata;
  hab = WinInitialize(0);
  hmq = WinCreateMsgQueue(hab, 0);

  AsuCtlInitialize(hab);
  
  G_hMyComputer = WinLoadPointer(HWND_DESKTOP, hResource, ICO_MYCOMP);
  G_hFolder     = WinLoadPointer(HWND_DESKTOP, hResource, ICO_FOLDER);
  G_hDisk       = WinLoadPointer(HWND_DESKTOP, hResource, ICO_DISK);
  G_hFloppy     = WinLoadPointer(HWND_DESKTOP, hResource, ICO_FLOPPY);
  G_hCDROM      = WinLoadPointer(HWND_DESKTOP, hResource, ICO_CDROM);

  if (pdd->flDialogType ==  DIRDLG_TYPE_OKCANCEL)
  {
     if (pdd->flUseWinButton)
       dialog_res=DLG_DIRDLG_OKCANCEL_W;
     else
       dialog_res=DLG_DIRDLG_OKCANCEL;
  }
  else
  {
     if (pdd->flUseWinButton)
       dialog_res=DLG_DIRDLG_OK_W;
     else
       dialog_res=DLG_DIRDLG_OK;
  }
  
  dlgdata.pDlgData=pdd;

  success = WinDlgBox( hwndParent,
                       hwndOwner,
                       (PFNWP)DirDlgProc,
                       (HMODULE)hResource,
                       dialog_res,
                       &dlgdata);


  WinDestroyMsgQueue( hmq );            /* and                          */
  WinTerminate( hab );                  /* terminate the application    */


  return 0;
}


LONG AddMainRecordsToContainer(HWND hwndContainer)
{
  ULONG        i;
  CNRINFO      cnrInfo;
  CHAR         szTemp[50];
  CHAR         szDrives[30];
  PRECORDCORE  Tracks,TrackFirst;
  PRECORDCORE   pParent;
  RECORDINSERT recsIn;
  ULONG        cbTracks;
  ULONG        x;
  PCNRENTRY    pEntry, pTempEntry;
  PCNRENTRY    pParentEntry;
  CHAR         szDir[CCHMAXPATH];
  PSZ          pszPath;
  HWND         hwndMain=WinQueryWindow(hwndContainer,QW_PARENT);
  PTHRPARAM    pThrParams;
  ULONG        ulMemSize;
  APIRET       rc;
  DRIVEPARAMS  dp;


  EnumDrives(szDrives,NULL,FALSE);
  i=0;

  cbTracks=strlen(szDrives);

  pParentEntry=(PCNRENTRY)WinSendMsg(hwndContainer,CM_ALLOCRECORD,MPFROMLONG(sizeof(CNRENTRY)-sizeof(RECORDCORE)),MPFROMLONG(1));
  pParentEntry->record.cb       = sizeof(RECORDCORE);

  pParentEntry->szFullPathName=G_pszMyComputer;
  pParentEntry->record.pszIcon=G_pszMyComputer;
  pParentEntry->record.pszText=G_pszMyComputer;
  pParentEntry->record.pszTree=G_pszMyComputer;
  pParentEntry->record.hptrIcon = G_hMyComputer;
  pParentEntry->record.preccNextRecord=NULL;

  recsIn.cb=sizeof(RECORDINSERT);
  recsIn.pRecordParent     = (PRECORDCORE) NULL;
  recsIn.pRecordOrder      = (PRECORDCORE) CMA_END;
  recsIn.zOrder            = CMA_TOP;
  recsIn.fInvalidateRecord = TRUE;
  recsIn.cRecordsInsert    = 1;

  WinSendMsg(hwndContainer,CM_INSERTRECORD,(PRECORDCORE)pParentEntry,&recsIn);

  pEntry=(PCNRENTRY)WinSendMsg(hwndContainer,CM_ALLOCRECORD,MPFROMLONG(sizeof(CNRENTRY)-sizeof(RECORDCORE)),MPFROMLONG(10));

  pTempEntry=pEntry;
  for (x=0;x<cbTracks;x++)
  {   
    pTempEntry->record.cb     = sizeof(RECORDCORE);

    sprintf(szDir,"%c:",szDrives[x]);
    pTempEntry->szFullPathName=strdup(szDir);

    sprintf(szTemp,"Disk %c",szDrives[x]);
    pszPath=strdup(szTemp);
    pTempEntry->record.pszIcon=pszPath;
    pTempEntry->record.pszTree=pszPath;
    pTempEntry->record.pszName=pszPath;
//    pTempEntry->record.hptrIcon = G_hCDROM;
    //  Sprawd« ikon© dla dysku

    rc=UtilQueryDiskParams((ULONG)szDrives[x]-'A'+1,&dp);

    if ((szDrives[x]=='A') || (szDrives[x]=='B'))
    {
      pTempEntry->record.hptrIcon = G_hFloppy;
    }
    else
    {
        
      if ((dp.ucDeviceType == 7) &&	// Na 99.99999% to CD-ROM
          (dp.usDeviceAttrs == 6) &&
          (dp.bpb.NumHeads == 1) && 
          (dp.bpb.SectorsPerTrack == 65535))
      {
        pTempEntry->record.hptrIcon = G_hCDROM;        
      }
      else
      {
        pTempEntry->record.hptrIcon = G_hDisk;
      }
    }
    pTempEntry = (PCNRENTRY) pTempEntry->record.preccNextRecord;
  }

  recsIn.pRecordParent     = (PRECORDCORE) pParentEntry;
  recsIn.cRecordsInsert    = cbTracks;
  WinSendMsg(hwndContainer,CM_INSERTRECORD,(PRECORDCORE)pEntry,&recsIn);

  cnrInfo.flWindowAttr=CV_TREE | CV_ICON | CA_TREELINE | CV_MINI | CA_DRAWICON;

  WinSendMsg(hwndContainer,CM_SETCNRINFO,&cnrInfo,MPFROMLONG(CMA_FLWINDOWATTR/*| CMA_SLBITMAPORICON*/));
  
  ulMemSize=sizeof(THRPARAM)+(sizeof(PSZ)*(cbTracks-1));
//  dprintf("Size = %d", ulMemSize);
  pThrParams=malloc(ulMemSize);

  pTempEntry=pEntry;
  
  i = 0;
  while (pTempEntry != NULL)
  {
    pThrParams->szDir[i] = pTempEntry->szFullPathName;
    pTempEntry           = (PCNRENTRY)pTempEntry->record.preccNextRecord;
    i++;
  }

  pThrParams->hwnd=hwndMain;
  pThrParams->cbDirs=cbTracks;
  pThrParams->flSendNotify=TRUE;
/*
  DosCreateThread(&G_tid, (PFNTHREAD)SearchThread,
                        (ULONG)pThrParams, 0U, 3*96000);
*/
  G_tid=_beginthread(SearchThread, NULL, THREADSTACKSIZE,
                        pThrParams);

  return (0);
}

MRESULT EXPENTRY DirDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{ PNOTIFYRECORDENTER Selected;
  PCNRENTRY pRecord;
  ULONG rc;
  MRESULT mrResult;
  PDLGDATA pData;
  ULONG ulrc;

  switch (msg)
  {
    case WM_INITDLG:
     { HPOINTER hptr = WinQueryPointer(HWND_DESKTOP);
      if (UtilWarpSansPresent())
        WinSetPresParam(hwnd, PP_FONTNAMESIZE,11, "9.WarpSans");
      else
        WinSetPresParam(hwnd, PP_FONTNAMESIZE, 7, "8.Helv");
      UtilCenterWindow(hwnd);

      WinSetPointer(HWND_DESKTOP,
                  WinQuerySysPointer(HWND_DESKTOP,
                                     SPTR_WAIT,
                                     FALSE));   // no copy

      AddMainRecordsToContainer(WinWindowFromID(hwnd,CNR_TREE));

      pData=(PDLGDATA)mp2;
      
      pData->pRecord=NULL;
      ulrc=WinSetWindowULong(hwnd,QWL_USER,(ULONG)pData);
      G_pszSelectedDir=pData->pDlgData->pszInitialDir;

      // Zmiana title
      if (pData->pDlgData->pszTitle)
      {
        WinSetWindowText(hwnd,pData->pDlgData->pszTitle);
      }

      if (pData->pDlgData->pszOKButton)
      {
        WinSetWindowText(WinWindowFromID(hwnd,DID_OK),pData->pDlgData->pszOKButton);
      }

      if (pData->pDlgData->pszCancelButton)
      {
        WinSetWindowText(WinWindowFromID(hwnd,DID_OK),pData->pDlgData->pszCancelButton);
      }
      WinSetPointer(HWND_DESKTOP,hptr);
      }
      break;

    case WM_CONTROL:
      switch (SHORT1FROMMP(mp1))
      {
        case CNR_TREE:
           switch (SHORT2FROMMP(mp1))
           {
              case CN_EXPANDTREE:
                pRecord=mp2;
                ExpandTree(hwnd,(PCNRENTRY)pRecord);
                break;
           }
      }
      break;
    case WM_ADDTOCNR:
      pRecord=(PCNRENTRY)WinSendMsg(WinWindowFromID(hwnd,CNR_TREE), CM_QUERYRECORD, MPFROMP(NULL), MPFROM2SHORT(CMA_FIRST,CMA_ITEMORDER));
      AddDirToContainer(WinWindowFromID(hwnd,CNR_TREE), (PSZ)mp1, (PSZ)mp2,pRecord);
      // nale¾dy zwolni† dwa parametry bo byˆ to strdup
      free(mp1);
      free(mp2);
      break;

    case WM_DIRADDEDTOCNR:
      if (G_pszSelectedDir)
      { PCNRENTRY pEntry=NULL;

        pData=(PDLGDATA)WinQueryWindowULong(hwnd,QWL_USER);
        pEntry=pData->pRecord;
        if (!pEntry)
        {
          pRecord=(PCNRENTRY)WinSendMsg(WinWindowFromID(hwnd,CNR_TREE), CM_QUERYRECORD, MPFROMP(NULL), MPFROM2SHORT(CMA_FIRST,CMA_ITEMORDER));
          pRecord->record.flRecordAttr |= CRA_EXPANDED;

          pRecord->record.flRecordAttr &= ~CRA_COLLAPSED;
          pRecord->record.flRecordAttr &= ~CRA_SELECTED;
          pRecord->record.flRecordAttr &= ~CRA_CURSORED;
          pRecord->record.flRecordAttr &= ~CRA_SOURCE;

          WinSendMsg(WinWindowFromID(hwnd,CNR_TREE), CM_INVALIDATERECORD, MPFROMP(pRecord), MPFROM2SHORT(0,CMA_REPOSITION));
        }
        else
        {
          pRecord=pEntry;
          pRecord->record.flRecordAttr &=~ CRA_CURSORED;
          WinSendMsg(WinWindowFromID(hwnd,CNR_TREE), CM_INVALIDATERECORD, MPFROMP(pRecord), MPFROM2SHORT(0,CMA_REPOSITION));
        }
        AddSelectedDir(WinWindowFromID(hwnd,CNR_TREE),G_pszSelectedDir,pRecord);
      }
      break;

   case WM_QUERYSELECTED:
//      dprintf("WM_QUERYSELECTED");
      if (G_pszSelectedDir)
      {
        pRecord=mp1;
       
        pRecord->record.flRecordAttr |= CRA_CURSORED;
        WinSendMsg(WinWindowFromID(hwnd,CNR_TREE), CM_INVALIDATERECORD, MPFROMP(pRecord), MPFROM2SHORT(0,CMA_REPOSITION));

//        dprintf("Nagrywamy (%s)",pRecord->szFullPathName);
        pData=(PDLGDATA)WinQueryWindowULong(hwnd,QWL_USER);
        pData->pRecord=pRecord;
//        WinSetWindowULong(hwnd,QWL_USER,(ULONG)pRecord);
        WinSendMsg(WinWindowFromID(hwnd,CNR_TREE), CM_EXPANDTREE, MPFROMP(pRecord),  NULL);
//        AddSelectedDir(WinWindowFromID(hwnd,CNR_TREE),G_pszSelectedDir,pRecord);
      }

      break;

    case WM_PAINT:
      mrResult=WinDefDlgProc(hwnd,msg,mp1,mp2);
      DirDlgOnPaint(hwnd);     
      return mrResult;
      break;

    case WM_COMMAND:
      pData=(PDLGDATA)WinQueryWindowULong(hwnd,QWL_USER);
      pData->pDlgData->lReturn=LONGFROMMP(mp1);
      switch (LONGFROMMP(mp1))
      {
        case DID_OK:
//          WinSetWindowULong(hwnd,QWL_USER+1,(ULONG)pData->pDlgData);
          pRecord=(PCNRENTRY)WinSendMsg(WinWindowFromID(hwnd,CNR_TREE), CM_QUERYRECORD, MPFROMP(NULL), MPFROM2SHORT(CMA_FIRST,CMA_ITEMORDER));
          pRecord=(PCNRENTRY)WinSendMsg(WinWindowFromID(hwnd,CNR_TREE),CM_QUERYRECORDEMPHASIS,MPFROMP(pRecord),MPFROMSHORT(CRA_SELECTED));
          if (pRecord)
          {
            strcpy(pData->pDlgData->szFullDirectoryName,pRecord->szFullPathName);
            WinSendMsg(hwnd,WM_CLOSE,0,0);
         }
        break;

        case DID_CANCEL:
          WinPostMsg(hwnd,WM_CLOSE,0,0);
          break;
      }
      break;

    case WM_CLOSE:
      pRecord=(PCNRENTRY)WinSendMsg(WinWindowFromID(hwnd,CNR_TREE), CM_QUERYRECORD, MPFROMP(NULL), MPFROM2SHORT(CMA_FIRST,CMA_ITEMORDER));
      DestroyTree(WinWindowFromID(hwnd,CNR_TREE),pRecord);
/*      rc=*/WinSendMsg(WinWindowFromID(hwnd,CNR_TREE), CM_REMOVERECORD, MPFROMP(pRecord),MPFROM2SHORT(0,CMA_FREE));
      WinPostMsg(hwnd,WM_QUIT,0,0);
      break;

    default:
      return WinDefDlgProc(hwnd,msg,mp1,mp2);
  }
  return (MRESULT)FALSE;
}

VOID EnumDrives(PSZ pszBuffer,      // out: drive letters
                    const char *pcszFileSystem,  // in: FS's to match or NULL
                    BOOL fSkipRemoveables) // in: if TRUE, only non-removeable disks will be returned
{
    CHAR szName[5] = "";
    ULONG ulLogicalDrive = 1, // start with drive A:
          ulFound = 0;        // found drives count
    APIRET arc             = NO_ERROR; // return code

    if (fSkipRemoveables)
        // start with drive C:
        ulLogicalDrive = 3;

    // go thru the drives, start with C: (== 3), stop after Z: (== 26)
    while (ulLogicalDrive <= 26)
    {
        UCHAR nonRemovable=0;
        ULONG parmSize=2;
        ULONG dataLen=1;

        #pragma pack(1)
        struct
        {
            UCHAR dummy,drive;
        } parms;
        #pragma pack()

        parms.drive=(UCHAR)(ulLogicalDrive-1);
        arc = DosDevIOCtl((HFILE)-1,
                          IOCTL_DISK,
                          DSK_BLOCKREMOVABLE,
                          &parms,
                          parmSize,
                          &parmSize,
                          &nonRemovable,
                          1,
                          &dataLen);

        /* _Pmpf(("  ul = %d, Drive %c: arc = %d nonRemoveable = %d",
                    ulLogicalDrive,
                    G_acDriveLetters[ulLogicalDrive],
                    arc,
                    nonRemovable)); */
/*
        if ((!fSkipRemoveables) && (arc == NO_ERROR) && (!nonRemovable))
        {
          pszBuffer[ulFound] = (CHAR)'A'+ulLogicalDrive-1; // drive letter
          ulFound++;
        }
*/

        if (    // fixed disk and non-removeable
                ((arc == NO_ERROR) /*&& (nonRemovable)*/)
                // or network drive:
             || (arc == ERROR_NOT_SUPPORTED)
           )
        {
            ULONG  ulOrdinal       = 0;     // ordinal of entry in name list
            BYTE   fsqBuffer[sizeof(FSQBUFFER2) + (3 * CCHMAXPATH)] = {0};
            ULONG  cbBuffer   = sizeof(fsqBuffer);        // Buffer length)
            PFSQBUFFER2 pfsqBuffer = (PFSQBUFFER2)fsqBuffer;

            szName[0] = G_acDriveLetters[ulLogicalDrive];
            szName[1] = ':';
            szName[2] = '\0';

            DosError(FERR_DISABLEHARDERR | FERR_DISABLEEXCEPTION);
            arc = DosQueryFSAttach(szName,          // logical drive of attached FS
                                   ulOrdinal,       // ignored for FSAIL_QUERYNAME
                                   FSAIL_QUERYNAME, // return data for a Drive or Device
                                   pfsqBuffer,      // returned data
                                   &cbBuffer);      // returned data length
            DosError(FERR_ENABLEHARDERR | FERR_ENABLEEXCEPTION);
            if ((arc == NO_ERROR) || (arc==ERROR_NOT_READY))
            {
                // The data for the last three fields in the FSQBUFFER2
                // structure are stored at the offset of fsqBuffer.szName.
                // Each data field following fsqBuffer.szName begins
                // immediately after the previous item.
                CHAR* pszFSDName = (PSZ)&(pfsqBuffer->szName) + (pfsqBuffer->cbName) + 1;
                if (pcszFileSystem == NULL)
                {
                    // enum-all mode: always copy
                    pszBuffer[ulFound] = szName[0]; // drive letter
                    ulFound++;
                }
                else if (strcmpi(pszFSDName, pcszFileSystem) == 0)
                {
                    pszBuffer[ulFound] = szName[0]; // drive letter
                    ulFound++;
                }
            }
        }
        ulLogicalDrive++;
    } // end while (G_acDriveLetters[ulLogicalDrive] <= 'Z')

    pszBuffer[ulFound] = '\0';
}

void AddDirRecord(HWND hwnd,PSZ szFullDirectory, PSZ szPath,PCNRENTRY pParent)
{
  PCNRENTRY pEntry;
  PCNRENTRY pTempChild;
  CHAR  szDir[CCHMAXPATH];
  PSZ   pszPath;
  ULONG cbMaxLen;
  sprintf(szDir,"%s\\%s",szFullDirectory,szPath);

  // sprawd« czy ju¾ nie zostaˆ dodany
  pTempChild=(PCNRENTRY)WinSendMsg(hwnd, CM_QUERYRECORD, MPFROMP(pParent), MPFROM2SHORT(CMA_FIRSTCHILD,CMA_ITEMORDER));

  while (pTempChild != NULL)
  {
    if (strcmpi(szDir,pTempChild->szFullPathName)==0)
    {
      // JU½ ISTNIEJE TAKI REKORD!!
      return;               
    }
    pTempChild=(PCNRENTRY)WinSendMsg(hwnd, CM_QUERYRECORD, MPFROMP(pTempChild), MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
  }

  pEntry=(PCNRENTRY)WinSendMsg(hwnd,CM_ALLOCRECORD,MPFROMLONG(sizeof(CNRENTRY)-sizeof(RECORDCORE)),MPFROMLONG(1));

  pEntry->szFullPathName=strdup(szDir);
 
  pszPath=strdup(szPath);

  pEntry->record.pszIcon=pszPath;
  pEntry->record.pszTree=pszPath;
  pEntry->record.pszName=pszPath;
  pEntry->record.hptrIcon = G_hFolder;
  pEntry->record.preccNextRecord = NULL;

  AddRecord(hwnd,pEntry, pParent);
}

VOID UtilRemoveFinalBackslash (PSZ p)
{
   for (; *p; ++p)
      ;

   if (*--p == '\\')
   {
      *p = '\0';
   }
}


void AddDirToContainer(HWND hwnd, PSZ szFullDirectory, PSZ szPath, PCNRENTRY pParent)
{ 
  PCNRENTRY pRecord;
  ULONG cbSize;
  // Na samym pocz¥tku nale¾y rozkˆada† katalog na cz©˜ci pierwsze  
  // OK -> jeste˜my w dyskach

  pRecord=(PCNRENTRY)WinSendMsg(hwnd, CM_QUERYRECORD, MPFROMP(pParent), MPFROM2SHORT(CMA_FIRSTCHILD,CMA_ITEMORDER));
  while (pRecord != NULL)
  {
    if (strcmpi(szFullDirectory,pRecord->szFullPathName)==0)
    {
      // OK zapami©taj rekord i zobacz czy ju¾ nie zo
      AddDirRecord(hwnd,szFullDirectory,szPath,pRecord);
      return;
    }
    else
    {
      cbSize=min(strlen(pRecord->szFullPathName),strlen(szFullDirectory));
      if (strnicmp(szFullDirectory,pRecord->szFullPathName, cbSize)==0)
      {
        AddDirToContainer(hwnd,szFullDirectory,szPath,pRecord);
        return;
      }
    }
    pRecord=(PCNRENTRY)WinSendMsg(hwnd, CM_QUERYRECORD, MPFROMP(pRecord), MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
  }
}
#if 0
VOID _System SearchThread(ULONG ulParams)
#endif
VOID _Optlink SearchThread(PVOID ulParams)
{ PTHRPARAM pParam=(PTHRPARAM)ulParams;
  int i;

//  dprintf("JESTE—MY TU !!!!!!!!!!!!!");
  for (i=0;i<pParam->cbDirs;i++)
  {
//    dprintf("JESTE—MY TU %d, %s",pParam->hwnd,pParam->szDir[i]);
    SearchDirectories(pParam->hwnd,pParam->szDir[i]);
  }

  if (pParam->flSendNotify)  
  {
    WinPostMsg(pParam->hwnd,WM_DIRADDEDTOCNR,0,0);
  }

  free(pParam);
  DosExit(0,0);
}

void SearchDirectories(HWND hwnd, PSZ szDirectory)
{
  FILEFINDBUF3 *result,*data;
  ULONG searchCount = 30;
  CHAR  szDir[CCHMAXPATH];
  HDIR hDir = -1;  
  ULONG attrs;
  APIRET rc;
  PCNRENTRY pRecord;

  UtilRemoveFinalBackslash(szDirectory);
  sprintf(szDir,"%s\\*",szDirectory);

  attrs  =   FILE_READONLY | FILE_HIDDEN | FILE_SYSTEM | FILE_DIRECTORY | FILE_ARCHIVED | MUST_HAVE_DIRECTORY;
  result = (FILEFINDBUF3*)malloc(searchCount*sizeof(FILEFINDBUF3));

  DosError(FERR_DISABLEHARDERR | FERR_DISABLEEXCEPTION);
  rc = DosFindFirst(szDir,&hDir,attrs,result,searchCount*sizeof(FILEFINDBUF3),&searchCount,FIL_STANDARD);
  DosError(FERR_ENABLEHARDERR | FERR_ENABLEEXCEPTION);

  if (rc)
  {
    free(result);
    return;
  }

  do
  {
    AddDirsFromFF(hwnd, szDirectory, result, searchCount);
    rc=DosFindNext(hDir, result,searchCount*sizeof(FILEFINDBUF3),&searchCount);
  }
  while (rc != ERROR_NO_MORE_FILES);

  free(result);
  DosFindClose(hDir);
}

void AddDirsFromFF(HWND hwnd,PSZ szDirectory,FILEFINDBUF3 * pFData, ULONG cbCount)
{ FILEFINDBUF3 *data;
  PCNRENTRY pEntry;
  int x;
  PSZ pszDir;
  PSZ pszPath;

  data = pFData;
  for (x = 0;x < cbCount;x++)
  { 
    if ((strcmpi (data->achName, ".")) && (strcmpi (data->achName, "..")))
    { 
      pszPath=strdup(data->achName);
      pszDir=strdup(szDirectory);
      WinPostMsg(hwnd,WM_ADDTOCNR,MPFROMP(pszDir),MPFROMP(pszPath));
    }
    data = (FILEFINDBUF3*)(((ULONG)data)+data->oNextEntryOffset);
  }
}

void ExpandTree(HWND hwnd, PCNRENTRY pParent)
{
  TID tid;
  PCNRENTRY pTempRecord=pParent;
  PCNRENTRY pRecord;
  PTHRPARAM pThrParams;
  ULONG cbDirs=0;
  ULONG ulMemSize;
  HWND hwndCnr=WinWindowFromID(hwnd,CNR_TREE);
  int i;

  if (strcmpi(pParent->szFullPathName,G_pszMyComputer)==0)
  {
     return;
  }
  else
  {
     pRecord=(PCNRENTRY)WinSendMsg(WinWindowFromID(hwnd,CNR_TREE), CM_QUERYRECORD, MPFROMP(pParent), MPFROM2SHORT(CMA_FIRSTCHILD,CMA_ITEMORDER));
  }
  if (pRecord==NULL) 
    return;
  else
    pTempRecord=pRecord;
  // Srawd« ile rekord¢w trzeba zaalokowa†

  while (pTempRecord!=NULL)
  {
    cbDirs++;
    pTempRecord=(PCNRENTRY)WinSendMsg(WinWindowFromID(hwnd,CNR_TREE), CM_QUERYRECORD, MPFROMP(pTempRecord), MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
  }

  i=0;

  pTempRecord=pRecord;
  ulMemSize=sizeof(THRPARAM)+(sizeof(PSZ)*(cbDirs-1));
  pThrParams=malloc(ulMemSize);
  for (i=0;i<cbDirs;i++)
  {
    pThrParams->szDir[i]=pTempRecord->szFullPathName;
    pTempRecord=(PCNRENTRY)WinSendMsg(WinWindowFromID(hwnd,CNR_TREE), CM_QUERYRECORD, MPFROMP(pTempRecord), MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
  }

  pThrParams->cbDirs=cbDirs;
  pThrParams->hwnd=hwnd;
  pThrParams->flSendNotify=TRUE;
#if 0
  DosCreateThread(&tid, (PFNTHREAD)SearchThread,
                        (ULONG)pThrParams, 0U, 32768);
#endif
//  DosWaitThread(G_tid,DCWW_WAIT);
  G_tid=_beginthread(SearchThread, NULL, THREADSTACKSIZE,
                        pThrParams);

}

void DestroyTree(HWND hwndCnr, PCNRENTRY pParent)
{ PCNRENTRY pRecord;
  PCNRENTRY pTempRecord;
  ULONG rc;

  pRecord=(PCNRENTRY)WinSendMsg(hwndCnr, CM_QUERYRECORD, MPFROMP(pParent), MPFROM2SHORT(CMA_FIRSTCHILD,CMA_ITEMORDER));

  if (!pRecord) 
  {
    free(pParent->szFullPathName);
    pParent->szFullPathName=NULL;
    free(pParent->record.pszName);
    pParent->record.pszName=NULL;
    pParent->record.pszIcon=NULL;
    pParent->record.pszTree=NULL;
    return;
  }

  while (pRecord)
  {
    DestroyTree(hwndCnr,pRecord);
    pRecord=(PCNRENTRY)WinSendMsg(hwndCnr, CM_QUERYRECORD, MPFROMP(pRecord), MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
    if (pRecord!=NULL) 
    {
      pTempRecord=pRecord;
    }
  }
  // OK co jest jeszcze?
  pRecord=(PCNRENTRY)WinSendMsg(hwndCnr, CM_QUERYRECORD, MPFROMP(NULL), MPFROM2SHORT(CMA_FIRST,CMA_ITEMORDER));
  pRecord=(PCNRENTRY)WinSendMsg(hwndCnr, CM_QUERYRECORD, MPFROMP(pRecord), MPFROM2SHORT(CMA_FIRSTCHILD,CMA_ITEMORDER));
  while (pRecord)
  {
    pRecord=(PCNRENTRY)WinSendMsg(hwndCnr, CM_QUERYRECORD, MPFROMP(pRecord), MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
    if (pRecord!=NULL)  
    {
      if (pRecord->szFullPathName!=NULL)
      {
        pTempRecord=pRecord;
        free(pRecord->szFullPathName);
        pRecord->szFullPathName=NULL;
        free(pRecord->record.pszName);
        pRecord->record.pszName=NULL;
        pRecord->record.pszIcon=NULL;
        pRecord->record.pszTree=NULL;
      }
    }   
  }
}

void AddRecord(HWND hwnd,PCNRENTRY pRecord, PCNRENTRY pParent)
{
  RECORDINSERT recsIn;

  recsIn.cb=sizeof(RECORDINSERT);
  recsIn.pRecordParent     = (PRECORDCORE) pParent;
  recsIn.pRecordOrder      = (PRECORDCORE) CMA_END;
  recsIn.zOrder            = CMA_TOP;
  recsIn.fInvalidateRecord = TRUE;
  recsIn.cRecordsInsert    = 1;
  WinSendMsg(hwnd,CM_INSERTRECORD,(PRECORDCORE)pRecord,&recsIn);
}


BOOL UtilWarpSansPresent(VOID)
{
  LONG cbFont = 0;
  HPS hps;
  BOOL rc;

  hps = WinGetPS(HWND_DESKTOP);
  rc = GpiQueryFonts(hps, QF_PUBLIC,"WarpSans", &cbFont, 0, NULL);
  WinReleasePS(hps);
  return rc;
}

VOID UtilCenterWindow(HWND hwnd)
{
  RECTL rclParent;
  RECTL rclWindow;

  WinQueryWindowRect(hwnd, &rclWindow);
  WinQueryWindowRect(WinQueryWindow(hwnd, QW_PARENT), &rclParent);

  rclWindow.xLeft = (rclParent.xRight - rclWindow.xRight) / 2;
  rclWindow.yBottom = (rclParent.yTop - rclWindow.yTop) / 2;

  WinSetWindowPos(hwnd, NULLHANDLE, rclWindow.xLeft, rclWindow.yBottom,
                  0, 0, SWP_MOVE);
}

void AddSelectedDir(HWND hwndCnr,PSZ pszSelectedDir, PCNRENTRY pParent)
{
  CHAR  szDir[CCHMAXPATH];
  ULONG cbDir;
  PCNRENTRY pRecord;
  PSZ szTemp;
  PTHRPARAM pThrParams;

  pRecord=(PCNRENTRY)WinSendMsg(hwndCnr, CM_QUERYRECORD, MPFROMP(pParent), MPFROM2SHORT(CMA_FIRSTCHILD,CMA_ITEMORDER));

  while (pRecord != NULL)
  {
//    dprintf("SZUKAMY W %s",pRecord->szFullPathName);
    cbDir=min(strlen(pszSelectedDir),strlen(pRecord->szFullPathName));
    if (strnicmp(pszSelectedDir,pRecord->szFullPathName,cbDir)==0)
    {
//        {
//          dprintf("EXPANDUJEMY %s",pRecord->szFullPathName);
          // Wchodzimy do nast©pnego katalogu
          WinSendMsg(hwndCnr,CM_SETRECORDEMPHASIS, MPFROMP(pRecord), MPFROM2SHORT(TRUE, CRA_SELECTED | CRA_CURSORED));

          cnrhScrollToRecord(hwndCnr,
                          (PRECORDCORE)pRecord,
                          CMA_ICON | CMA_TEXT,
                          FALSE);
          WinSendMsg(WinQueryWindow(hwndCnr,QW_PARENT), WM_QUERYSELECTED, MPFROMP(pRecord),  NULL);
//        }
//        else                
        if (strcmpi(pszSelectedDir,pRecord->szFullPathName) == 0)
        {
//          dprintf("jeste˜my tu!!!");
          WinSendMsg(hwndCnr, CM_COLLAPSETREE, MPFROMP(pRecord),  NULL);
          G_pszSelectedDir = NULL;
        }
        return;
    }
    pRecord=(PCNRENTRY)WinSendMsg(hwndCnr, CM_QUERYRECORD, MPFROMP(pRecord), MPFROM2SHORT(CMA_NEXT,CMA_ITEMORDER));
  }

  WinSendMsg(hwndCnr, CM_COLLAPSETREE, MPFROMP(pParent),  NULL);
          cnrhScrollToRecord(hwndCnr,
                          (PRECORDCORE)pParent,
                          CMA_ICON | CMA_TEXT,
                          FALSE);

  G_pszSelectedDir = NULL;
  return;
}


ULONG cnrhScrollToRecord(HWND hwndCnr,       // in: container window
                         PRECORDCORE pRec,   // in: record to scroll to
                         ULONG fsExtent,
                                 // in: this determines what parts of pRec
                                 // should be made visible. OR the following
                                 // flags:
                                 // -- CMA_ICON: the icon rectangle
                                 // -- CMA_TEXT: the record text
                                 // -- CMA_TREEICON: the "+" sign in tree view
                         BOOL KeepParent)
                                 // for tree views only: whether to keep
                                 // the parent record of pRec visible when scrolling.
                                 // If scrolling to pRec would make the parent
                                 // record invisible, we instead scroll so that
                                 // the parent record appears at the top of the
                                 // container workspace (Win95 style).

{
    QUERYRECORDRECT qRect, qRect2;
    RECTL           rclRecord, rclParentRecord, rclCnr, rclCnr2;
    POINTL          ptlRecord, ptlParentRecord;
    CNRINFO         CnrInfo;
    HAB             hab = WinQueryAnchorBlock(hwndCnr);
    BOOL            KeepParent2;
    LONG            lYOfs;

    qRect.cb = sizeof(qRect);
    qRect.pRecord = (PRECORDCORE)pRec;
    qRect.fsExtent = fsExtent;

    // query record location and size of container
    if (!WinSendMsg(hwndCnr, CM_QUERYRECORDRECT, &rclRecord, &qRect))
        return 1;
    if (!WinSendMsg(hwndCnr, CM_QUERYVIEWPORTRECT, &rclCnr, MPFROM2SHORT(CMA_WINDOW, FALSE)) )
        return 2;

    // check if left bottom point of pRec is currently visible in container
    ptlRecord.x = (rclRecord.xLeft);
    ptlRecord.y = (rclRecord.yBottom);
    // ptlRecord.x = (rclRecord.xLeft + rclRecord.xRight) / 2;
    // ptlRecord.y = (rclRecord.yBottom + rclRecord.yTop) / 2;
    if (WinPtInRect(hab, &rclCnr, &ptlRecord))
         return 3;

    if (KeepParent)
    {
        if (!WinSendMsg(hwndCnr, CM_QUERYCNRINFO, (MPARAM)&CnrInfo, (MPARAM)sizeof(CnrInfo)))
            return 4;
        else
            KeepParent2 = (CnrInfo.flWindowAttr & CV_TREE);
    }
    else
        KeepParent2 = FALSE;

    // calculate offset to scroll to make pRec visible
    lYOfs = (rclCnr.yBottom - rclRecord.yBottom)    // this would suffice
          + (rclRecord.yTop - rclRecord.yBottom);  // but we make the next rcl visible too

    if (KeepParent2)
    {
        qRect2.cb = sizeof(qRect2);
        qRect2.pRecord = (PRECORDCORE)WinSendMsg(hwndCnr,
                                                 CM_QUERYRECORD,
                                                 (MPARAM)pRec,
                                                 MPFROM2SHORT(CMA_PARENT,
                                                              CMA_ITEMORDER));
        qRect2.fsExtent = fsExtent;

        // now query PARENT record location and size of container
        if (!WinSendMsg(hwndCnr, CM_QUERYRECORDRECT, &rclParentRecord, &qRect2))
            return 5;

        ptlParentRecord.x = (rclParentRecord.xLeft);
        ptlParentRecord.y = (rclParentRecord.yTop);
        // ptlParentRecord.x = (rclParentRecord.xLeft + rclParentRecord.xRight) / 2;
        // ptlParentRecord.y = (rclParentRecord.yBottom + rclParentRecord.yTop) / 2;
        rclCnr2 = rclCnr;
        WinOffsetRect(hab, &rclCnr2, 0, -lYOfs);
        // if ( (rclParentRecord.yBottom - rclRecord.yBottom) > (rclCnr.yTop - rclCnr.yBottom) )
        if (!(WinPtInRect(hab, &rclCnr2, &ptlParentRecord)))
        {
            lYOfs = (rclCnr.yTop - rclParentRecord.yTop) // this would suffice
                  - (rclRecord.yTop - rclRecord.yBottom);  // but we make the previous rcl visible too
        }
    }

    if (!KeepParent2)
        // scroll horizontally
        WinPostMsg(hwndCnr,
                   CM_SCROLLWINDOW,
                   (MPARAM)CMA_HORIZONTAL,
                   (MPARAM)(rclRecord.xLeft - rclCnr.xLeft));

    // scroll vertically
    WinPostMsg(hwndCnr,
               CM_SCROLLWINDOW,
               (MPARAM)CMA_VERTICAL,
               (MPARAM)lYOfs);

    return 0;
}

APIRET UtilQueryDiskParams(ULONG ulLogicalDrive,        // in:  1 for A:, 2 for B:, 3 for C:, ...
                           PDRIVEPARAMS pdp)            // out: drive parameters
{
    APIRET arc = ERROR_INVALID_DRIVE;

//    dprintf("UtilQueryDiskParams(%d)",ulLogicalDrive);
    if (ulLogicalDrive)
    {
        #pragma pack(1)
        // parameter packet
        struct {
            UCHAR command, drive;
        } parms;
        #pragma pack()

        ULONG ulParmSize = sizeof(parms);
        ULONG ulDataSize = sizeof(DRIVEPARAMS);

        parms.command = 0; // read currently inserted media
        parms.drive=(UCHAR)(ulLogicalDrive-1);

        arc = DosDevIOCtl((HFILE)-1,
                          IOCTL_DISK,
                          DSK_GETDEVICEPARAMS,
                          // parameter packet:
                          &parms, ulParmSize, &ulParmSize,
                          // data packet: DRIVEPARAMS structure
                          pdp,    ulDataSize, &ulDataSize);
    }

//    dprintf("UtilQueryDiskParams=%d",arc);    
    return (arc);
}


VOID DirDlgOnPaint(HWND hwnd)
{
    RECTL   rclUpdate;
    SWP     swp; // prostok¥t kontenera
    POINTL  ptl[3];
    HPS     hps;
    HWND    hwndMain=WinQueryWindow(hwnd,QW_PARENT);

    hps=WinGetPS(hwnd);

    WinQueryWindowPos(WinWindowFromID(hwnd,CNR_TREE),&swp);

    // Rysowanie efektu 3D dla kontenera
    GpiSetColor(hps,CLR_BLACK);
    ptl[0].x=swp.x-1;       ptl[0].y=swp.y-1;
    ptl[1].x=swp.x-1;       ptl[1].y=swp.y+swp.cy;
    ptl[2].x=swp.x+swp.cx;  ptl[2].y=swp.y+swp.cy;
    GpiMove(hps,(POINTL*)&ptl);
    GpiPolyLine(hps,2,&ptl[1]);

    GpiSetColor(hps,CLR_PALEGRAY);
    ptl[0].x=swp.x+swp.cx;  ptl[0].y=swp.y+swp.cy;
    ptl[1].x=swp.x+swp.cx;  ptl[1].y=swp.y-1;
    ptl[2].x=swp.x-1;       ptl[2].y=swp.y-1;
    GpiMove(hps,(POINTL*)&ptl);
    GpiPolyLine(hps,2,&ptl[1]);

    GpiSetColor(hps,CLR_DARKGRAY);
    ptl[0].x=swp.x-2;       ptl[0].y=swp.y-2;
    ptl[1].x=swp.x-2;       ptl[1].y=swp.y+swp.cy+1;
    ptl[2].x=swp.x+swp.cx+1;ptl[2].y=swp.y+swp.cy+1;
    GpiMove(hps,(POINTL*)&ptl);
    GpiPolyLine(hps,2,&ptl[1]);

    GpiSetColor(hps,CLR_WHITE);
    ptl[0].x=swp.x+swp.cx+1;ptl[0].y=swp.y+swp.cy+1;
    ptl[1].x=swp.x+swp.cx+1;ptl[1].y=swp.y-2;
    ptl[2].x=swp.x-2;       ptl[2].y=swp.y-2;
    GpiMove(hps,(POINTL*)&ptl);
    GpiPolyLine(hps,2,&ptl[1]);

    WinReleasePS(hps);
}

void dprintf(const char *fmt,...)
{
#if 0
  va_list args;
  FILE *flog;

  va_start(args, fmt);

  flog=fopen("DEBUG.LOG","a+");
  if (flog!=NULL)
  {
    vfprintf(flog, fmt, args);  
    va_end(args);

    if (fmt[strlen(fmt)-1] != '\n')
      fprintf(flog, "\n");

    fclose(flog);
  }
#endif
}
