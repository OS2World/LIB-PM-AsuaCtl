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
.* Opisy nowych kontrolek prosz� pisa� w osobnych plikach *.inc i dodawa� je za pomoc�
.* dyrektywy .im w zaznaczonym miejscu (prosz� zachowywa� alfabetyczn� kolejno�� tytu��w)

:userdoc.
:docprof toc=123456.
:title.asuactl.dll - Zestaw okien kontrolnych Asua


.* Rozdzia� 1 - Wst�p
.*
:h1 res=001.Wst�p
:p.Biblioteka asuactl.dll zawiera kod obs�uguj�cy szereg rodzaj�w okien
kontrolnych, pozbawionych jednocze�nie wad ich odpowiednik�w systemowych.
Biblioteka, ta zawiera tak�e zupe�nie nowe klasy okien, kt�rych nie ma w systemie.





.* Rozdzia� 1.1 - Jak korzysta� z okien
.*
:h2 res=011.Jak korzysta� z okien
:p.Aby mo�na by�o u�y� okien kontrolnych zawartych w pliku asuactl.dll, nale�y przed
u�yciem bibliotek� do��czy� do programu i zarejestrowa� nowe klasy okien.
:p.Plik ready.zip zawiera wszystko, co niezb�dne do wykorzystania nowych
okien kontrolnych. Powinny si� w nim znajdowa� pliki:
:dl tsize=5 break=all.
:dt.:hp2.asuactl.dll:ehp2.
:dd.Plik ten zawiera definicje wszystkich nowych klas okien.
:dt.:hp2.asuactl.lib:ehp2.
:dd.Jest to biblioteka symboli eksportowanych, kt�r� nale�y do��czy� do
programu korzystaj�cego z okien kontrolnych Asua.
:dt.:hp2.asuactl.h:ehp2.
:dd.Plik nag��wkowy, zawieraj�cy wszystkie deklaracje niezb�dne do prawid�owej
kompilacji program�w korzystaj�cych z okien kontrolnych. W kodzie �r�d�owym programu
nale�y umie�ci� odwo�anie tylko do tego pliku - pozosta�e pliki *.h zawarte w katalogu
"ready" s� do��czane automatycznie podczas kompilacji asuactl.h.
:dt.:hp2.pozosta�e pliki *.h:ehp2.
:dd.Zawieraj� one deklaracje komunikat�w, sta�ych i struktur danych poszczeg�lnych okien
kontrolnych. Pliki te musz� si� znale�� w tym samym katalogu co asuactl.h.
:edl.

:p.Inicjacji kodu mo�na dokona� na dwa sposoby&colon.
:ol.
:li.Do programu nale�y do��czy� bibliotek� asuactl.lib, a w kodzie progamu
umie�ci� wywo�anie funkcji
:link reftype=hd refid=ASUCTLINITIALIZE.AsuCtlInitialize:elink.,
kt�ra zarejstruje wszystkie klasy okien. Aby u�y� tego sposobu inicjacji,
biblioteka musi si� znajdowa� na systemowej �cie�ce LIBPATH.

:li.W programie nale�y umie�ci� nast�puj�cy fragment kodu, ktury za�aduje do pami�ci
bibliotek� asuactl.dll i j� zainicjuje:
:cgraphic.
// Zmienne globalne programu
HMODULE asuactl;    // Uchwyt modu�u
HAB     anchor;     // Wynik wywo�ania WinInitialize	


// Inicjacja biblioteki asuactl.dll - fragment np. main()
{ LONG _syscall (*Init)(HAB hab);

  asuctl = NULLHANDLE;
  rc = DosLoadModule(NULL, 0L, "ASUACTL", &amp.asuactl);
  if(asuctl != NULLHANDLE)
  { // Odczyt adresu funkcji inicjuj�cej
    rc = DosQueryProcAddr(asuctl, 0, "ASUCTLINITIALIZE", &amp.Init);
    if(Init != NULL) rc = Init(anchor);
  }
}
:ecgraphic.

:p.Po poprawnym wykonaniu, zmienna rc (typu LONG) powinna zawiera� zero.
Przed zako�czeniem programu modu� asuactl.dll trzeba usun�� z pami�ci za pomoc�
funkcji DosFreeModule.
:p.Przy tym sposobie inicjacji, biblioteka mo�e si� znajdowa�
w dowolnym miejscu na dysku, okre�lonym za pomoc� �cie�ki dost�pu przekazanej
funkcji DosLoadModule.
:eol.
:p.:hp8.Uwaga!:ehp8. W kodzie programu korzystaj�cego z asuactl.dll
nie nale�y definiowa� sta�ych&colon. __INTERNAL_USE__ i __BUILDING_LIBRARY__.




.* Rozdzia� 2 - "Struktury danych og�lnego przeznaczenia"
.*
.im common\doc\commdata.inc

.* Rozdzia� 3 - Funkcje og�lnego przeznaczenia
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
