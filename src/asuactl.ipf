.* Control windows library - Asua Controls
.* Main documentation file
.* (c) 1999-2001 Wojciech Gazda
.*
.* asuactl.ipf
.*
.* $Author: gazda $
.* $Date: 2001/04/28 01:30:50 $
.* $Name:  $
.* $RCSfile: asuactl.ipf,v $
.* $Revision: 1.5 $
.*
.*
.* Opisy nowych kontrolek prosz© pisa† w osobnych plikach *.inc i dodawa† je za pomoc¥
.* dyrektywy .im w zaznaczonym miejscu (prosz© zachowywa† alfabetyczn¥ kolejno˜† tytuˆ¢w)

:userdoc.
:docprof toc=123456.
:title.asuactl.dll - Zestaw okien kontrolnych Asua


.* Rozdziaˆ 1 - Wst©p
.*
:h1 res=001.Wst©p
:p.Biblioteka asuactl.dll zawiera kod obsˆuguj¥cy szereg rodzaj¢w okien
kontrolnych, pozbawionych jednocze˜nie wad ich odpowiednik¢w systemowych.
Biblioteka, ta zawiera tak¾e zupeˆnie nowe klasy okien, kt¢rych nie ma w systemie.





.* Rozdziaˆ 1.1 - Jak korzysta† z okien
.*
:h2 res=011.Jak korzysta† z okien
:p.Aby mo¾na byˆo u¾y† okien kontrolnych zawartych w pliku asuactl.dll, nale¾y przed
u¾yciem bibliotek© doˆ¥czy† do programu i zarejestrowa† nowe klasy okien.
:p.Plik ready.zip zawiera wszystko, co niezb©dne do wykorzystania nowych
okien kontrolnych. Powinny si© w nim znajdowa† pliki:
:dl tsize=5 break=all.
:dt.:hp2.asuactl.dll:ehp2.
:dd.Plik ten zawiera definicje wszystkich nowych klas okien.
:dt.:hp2.asuactl.lib:ehp2.
:dd.Jest to biblioteka symboli eksportowanych, kt¢r¥ nale¾y doˆ¥czy† do
programu korzystaj¥cego z okien kontrolnych Asua.
:dt.:hp2.asuactl.h:ehp2.
:dd.Plik nagˆ¢wkowy, zawieraj¥cy wszystkie deklaracje niezb©dne do prawidˆowej
kompilacji program¢w korzystaj¥cych z okien kontrolnych. W kodzie «r¢dˆowym programu
nale¾y umie˜ci† odwoˆanie tylko do tego pliku - pozostaˆe pliki *.h zawarte w katalogu
"ready" s¥ doˆ¥czane automatycznie podczas kompilacji asuactl.h.
:dt.:hp2.pozostaˆe pliki *.h:ehp2.
:dd.Zawieraj¥ one deklaracje komunikat¢w, staˆych i struktur danych poszczeg¢lnych okien
kontrolnych. Pliki te musz¥ si© znale«† w tym samym katalogu co asuactl.h.
:edl.

:p.Inicjacji kodu mo¾na dokona† na dwa sposoby&colon.
:ol.
:li.Do programu nale¾y doˆ¥czy† bibliotek© asuactl.lib, a w kodzie progamu
umie˜ci† wywoˆanie funkcji
:link reftype=hd refid=ASUCTLINITIALIZE.AsuCtlInitialize:elink.,
kt¢ra zarejstruje wszystkie klasy okien. Aby u¾y† tego sposobu inicjacji,
biblioteka musi si© znajdowa† na systemowej ˜cie¾ce LIBPATH.

:li.W programie nale¾y umie˜ci† nast©puj¥cy fragment kodu, ktury zaˆaduje do pami©ci
bibliotek© asuactl.dll i j¥ zainicjuje:
:cgraphic.
// Zmienne globalne programu
HMODULE asuactl;    // Uchwyt moduˆu
HAB     anchor;     // Wynik wywoˆania WinInitialize	


// Inicjacja biblioteki asuactl.dll - fragment np. main()
{ LONG _syscall (*Init)(HAB hab);

  asuctl = NULLHANDLE;
  rc = DosLoadModule(NULL, 0L, "ASUACTL", &amp.asuactl);
  if(asuctl != NULLHANDLE)
  { // Odczyt adresu funkcji inicjuj¥cej
    rc = DosQueryProcAddr(asuctl, 0, "ASUCTLINITIALIZE", &amp.Init);
    if(Init != NULL) rc = Init(anchor);
  }
}
:ecgraphic.

:p.Po poprawnym wykonaniu, zmienna rc (typu LONG) powinna zawiera† zero.
Przed zakoäczeniem programu moduˆ asuactl.dll trzeba usun¥† z pami©ci za pomoc¥
funkcji DosFreeModule.
:p.Przy tym sposobie inicjacji, biblioteka mo¾e si© znajdowa†
w dowolnym miejscu na dysku, okre˜lonym za pomoc¥ ˜cie¾ki dost©pu przekazanej
funkcji DosLoadModule.
:eol.
:p.:hp8.Uwaga!:ehp8. W kodzie programu korzystaj¥cego z asuactl.dll
nie nale¾y definiowa† staˆych&colon. __INTERNAL_USE__ i __BUILDING_LIBRARY__.




.* Rozdziaˆ 2 - "Struktury danych og¢lnego przeznaczenia"
.*
.im common\doc\commdata.inc

.* Rozdziaˆ 3 - Funkcje og¢lnego przeznaczenia
.*
.im common\doc\commfunc.inc



.* ***********************************************************
.* *** Add source documentation files for new windows here ***
.* ***********************************************************

.* WC_STATUS - Status window
.im status\doc\status.inc

.* WC_TOOLBAR - Toolbar
.im toolbar\doc\toolbar.inc

.* WC_WINBUTTON - Alternative button
.im winbutt\doc\winbutt.inc



.* ******************
.* *** Appendixes ***
.* ******************

.* Build and development procedures
.im common\doc\build.inc

:euserdoc.

.*
.* $Log: asuactl.ipf,v $
.* Revision 1.5  2001/04/28 01:30:50  gazda
.* Toolbar documentation added
.*
.* Revision 1.3  2001/04/25 22:05:45  gazda
.* Toolbar documentation added - 90%
.*
.* Revision 1.2  2001/03/19 20:38:05  gazda
.* Added english version of build procedures
.*
.* Revision 1.1  2001/03/11 17:41:50  Wojciech
.* Initial version
.*
.*
