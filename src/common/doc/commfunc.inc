.* Dokumentacja techniczna biblioteki okien kontrolnych asuactl.dll
.* opis funkcji og�lnego przeznaczenia
.*

.* Rozdzeia� 3 - Funkcje og�lnego przeznaczenia
.*
:h1 res=003.Funkcje
:p.Rozdzia� zawiera opis funkcji zawartych w module asuactl.dll.

.* Rozdzia� 3.1 - Funkcje eksportowane
.*
:h2 res=031.Funkcje eksportowane.
:p.Rozdzia� zawiera opis funkcji, z kt�rych mo�e korzysta� u�ytkownik
biblioteki asuactl.dll.


.* Funkcja AsuCtlInitialize
.*
:h3 group=1 width=30% scroll=none name=ASUCTLINITIALIZE.AsuCtlInitialize
:link dependent group=40 reftype=hd auto refid=ASUCTLINITIALIZE_S.
:link dependent group=1  reftype=hd auto refid=ASUCTLINITIALIZE.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=ASUCTLINITIALIZE_S.Sk�adnia:elink.
.br
:link dependent reftype=hd refid=ASUCTLINITIALIZE_P.Parametry:elink.
.br
:link dependent reftype=hd refid=ASUCTLINITIALIZE_E.Rezultaty:elink.
.br
:link dependent reftype=hd refid=ASUCTLINITIALIZE_N.Opis:elink.


.* SK�ADNIA
.*
:h3 group=2 x=30% width=70% hide name=ASUCTLINITIALIZE_S.AsuCtlInitialize - Sk�adnia
:p.Funkcja inicjuje bibliotek� asuactl.dll.
:p.
:cgraphic.
#define  INCL_WINWINDOWMGR
#include <os2.h>
#include "asuactl.h"

HAB    :link dependent reftype=hd refid=ASUCTLINITIALIZE_HAB.hab:elink.;    // Uchwyt anchor block PM
LONG   :link dependent reftype=hd refid=ASUCTLINITIALIZE_E.rc:elink.;     // Kod powrotu

rc =  AsuCtlInitialize(HAB hab)
:ecgraphic.



.* Parametr hab
.*
:h3 group=10 x=30% y=bottom width=70% height=225x hide name=ASUCTLINITIALIZE_HAB.AsuCtlInitialize Parametr - hab
:dl tsize=3 break=all.
:dt.:hp2.hab:ehp2. (HAB) - parametr.
:dd.Uchwyt anchor block presentation managera.
:edl.


.* PARAMETRY
.*
:h3 group=3 x=30% width=70% hide name=ASUCTLINITIALIZE_P.AsuCtlInitialize - Parametry
:dl tsize=3 break=all.
:dt.:hp2.hab:ehp2. (HAB) - parametr.
:dd.Uchwyt anchor block presentation managera.
:edl.


.* Rezultaty
:h3 group=4 x=30% y=bottom width=70% height=225x hide name=ASUCTLINITIALIZE_E.AsuCtlInitialize - rezultaty
:dl tsize=3 break=all.
:dt.:hp2.rc:ehp2. (LONG)
:dd.Kod b��du&colon.
:dl tsize=3 break=all.
:dt.0
:dd.Poprawne wykonanie funkcji.
:dt.1
:dd.B��d rejsetracji klas okien.
:edl.
:edl.


.* OPIS
:h3 group=5 x=30% width=70% hide name=ASUCTLINITIALIZE_N.AsuCtlInitialize - Opis
:p.Funkcja rejestruje wszystkie klasy okien zawarte w bibliotece asuactl.dll.
:p.W przypadku, gdy wyst�pi b�ad rejestracji jednej z klas okien, funkcja zwraca 1,
w przeciwnym wypadku 0. Wyst�pienie b��du rejestracji kt�rej� z klas nie powoduje
przerwania wykonywania funkcji i zostaje podj�ta pr�ba rejestracji klas,
znajduj�cych si� po b��dnej.







.* Funkcja AsuGetComponentVersion
.*
:h3 group=1 width=30% scroll=none name=ASUGETCOMPVER.AsuGetComponentVersion
:link dependent group=40 reftype=hd auto refid=ASUGETCOMPVER_S.
:link dependent group=1  reftype=hd auto refid=ASUGETCOMPVER.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=ASUGETCOMPVER_S.Sk�adnia:elink.
.br
:link dependent reftype=hd refid=ASUGETCOMPVER_P.Parametry:elink.
.br
:link dependent reftype=hd refid=ASUGETCOMPVER_E.Rezultaty:elink.
.br
:link dependent reftype=hd refid=ASUGETCOMPVER_N.Opis:elink.


.* SK�ADNIA
.*
:h3 group=2 x=30% width=70% hide name=ASUGETCOMPVER_S.AsuGetComponentVersion - Sk�adnia
:p.Funkcja umo�liwia odczyt wersji wybranego okna kontrolnego zawartego
w bibliotece asuactl.dll.
:p.
:cgraphic.
#define  INCL_WINWINDOWMGR
#include <os2.h>
#include "asuactl.h"

PSZ   :link dependent reftype=hd refid=ASUGETCOMPVER_CLASS.class:elink.;  // Klasa okna kontrolnego
LONG  :link dependent reftype=hd refid=ASUGETCOMPVER_E.ver:elink.;    // Wersja okna kontrolnego

ver = AsuGetComponentVersion(PSZ class);
:ecgraphic.



.* Parametr class
.*
:h3 group=10 x=30% y=bottom width=70% height=225x hide name=ASUGETCOMPVER_CLASS.AsuGetComponentVersion Parametr - class
:dl tsize=3 break=all.
:dt.:hp2.class:ehp2. (PSZ) - parametr.
:dd.Klasa okna, dla kt�rej jest dokonywany odczyt wersji.
:edl.


.* PARAMETRY
.*
:h3 group=3 x=30% width=70% hide name=ASUGETCOMPVER_P.AsuGetComponentVersion - Parametry
:dl tsize=3 break=all.
:dt.:hp2.class:ehp2. (PSZ) - parametr.
:dd.Klasa okna, dla kt�rej jest dokonywany odczyt wersji.
:edl.


.* Rezultaty
:h3 group=4 x=30% y=bottom width=70% height=225x hide name=ASUGETCOMPVER_E.AsuGetComponentVersion - rezultaty
:dl tsize=3 break=all.
:dt.:hp2.ver:ehp2. (LONG)
:dd.Odczytany numer wersji okna kontrolnego o podanej klasie.
.br
Bardziej znacz�ce s�owo rezultatu zawiera g��wny numer wersji, a mniej znacz�ce dodatkowy
numer wersji. Kod s�u��cy do odczytu obydwu cz�on�w numeru wersji mo�e wygl�da�
nast�puj�co&colon.
:cgraphic.
// Odczyt wersji paska narz�dzi
ver = AsuGetComponentVersion(WC_TOOLBAR);
// G��wny numer wersji
VerMajor = ver >> 16;
// Numer podwersji
VerMinor = ver &amp. 0xFFFF;
:ecgraphic.
:p.Zwracane warto�ci nale�y interpretowa� w nast�puj�cy spos�b&colon.
:table cols='10 10 20' rules=none frame=none.
:row.
:c.VerMajor
:c.VerMinor
:c.Pe�ny numer wersji
:row.
:c.1
:c.2
:c.1.02
:row.
:c.1
:c.20
:c.1.2
:row.
:c.2
:c.51
:c.2.51
:etable.
:edl.


.* OPIS
:h3 group=5 x=30% width=70% hide name=ASUGETCOMPVER_N.AsuGetComponentVersion - Opis
:p.Je�eli w procedurze okna kontrolnego zaimplementowano

metod� odczytu wersji, to funkcja AsuGetComponentVersion umo�liwia sprawdzenie
poprawno�ci rejestracji klasy okna. W przypadku gdy klasa nie zosta�a zarejestrowana
funkcja zwraca 0 zamiast numeru wersji.






.* Funkcja AsuGetVersion
.*
:h3 group=1 width=30% scroll=none name=ASUGETVERSION.AsuGetVersion
:link dependent group=40 reftype=hd auto refid=ASUGETVERSION_S.
:link dependent group=1  reftype=hd auto refid=ASUGETVERSION.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=ASUGETVERSION_S.Sk�adnia:elink.
.br
:link dependent reftype=hd refid=ASUGETVERSION_P.Parametry:elink.
.br
:link dependent reftype=hd refid=ASUGETVERSION_E.Rezultaty:elink.
.br
:link dependent reftype=hd refid=ASUGETVERSION_N.Opis:elink.


.* SK�ADNIA
.*
:h3 group=2 x=30% width=70% hide name=ASUGETVERSION_S.AsuGetVersion - Sk�adnia
:p.Funkcja umo�liwia odczyt wersji biblioteki dynamicznej asuactl.dll.
:cgraphic.
#define  INCL_WINWINDOWMGR
#include <os2.h>
#include "asuactl.h"

ver = AsuGetVersion(void);
:ecgraphic.



.* PARAMETRY
.*
:h3 group=3 x=30% width=70% hide name=ASUGETVERSION_P.AsuGetVersion - Parametry
:p.Brak.


.* Rezultaty
:h3 group=4 x=30% y=bottom width=70% height=225x hide name=ASUGETVERSION_E.AsuGetVersion - rezultaty
:dl tsize=3 break=all.
:dt.:hp2.ver:ehp2. (LONG)
:dd.Odczytany numer wersji biblioteki dynamicznej.
.br
Bardziej znacz�ce s�owo rezultatu zawiera g��wny numer wersji, a mniej znacz�ce dodatkowy
numer wersji. Kod s�u��cy do odczytu obydwu cz�on�w numeru wersji mo�e wygl�da�
nast�puj�co&colon.
:cgraphic.
// Odczyt wersji biblioteki asuactl.dll
ver = AsuGetVersion();
// G��wny numer wersji
VerMajor = ver >> 16;
// Numer podwersji
VerMinor = ver &amp. 0xFFFF;
:ecgraphic.
:p.Zwracane warto�ci nale�y interpretowa� w nast�puj�cy spos�b&colon.
:table cols='10 10 20' rules=none frame=none.
:row.
:c.VerMajor
:c.VerMinor
:c.Pe�ny numer wersji
:row.
:c.1
:c.2
:c.1.02
:row.
:c.1
:c.20
:c.1.2
:row.
:c.2
:c.51
:c.2.51
:etable.
:edl.


.* OPIS
:h3 group=5 x=30% width=70% hide name=ASUGETVERSION_N.AsuGetVersion - Opis
:p.Funkcja odczytuje numer wersji biblioteki asuactl.dll.






.* Rozdzia� 3.2 - Funkcje wewn�trzne
:h2 res=032.Funkcje wewn�trzne.
:p.Rozdzia� zawiera opis funkcji niedost�pnych dla u�ytkownika,
z kt�rych mo�e korzysta� programista dodaj�cy nowe okna kontrolne
do biblioteki asuactl.dll.


.* Funkcja AsuQueryCtlInfo
.*
:h3 group=1 width=30% scroll=none name=ASUQUERYCTLINFO.AsuQueryCtlInfo
:link dependent group=40 reftype=hd auto refid=ASUQUERYCTLINFO_S.
:link dependent group=1  reftype=hd auto refid=ASUQUERYCTLINFO.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=ASUQUERYCTLINFO_S.Sk�adnia:elink.
.br
:link dependent reftype=hd refid=ASUQUERYCTLINFO_P.Parametry:elink.
.br
:link dependent reftype=hd refid=ASUQUERYCTLINFO_E.Rezultaty:elink.
.br
:link dependent reftype=hd refid=ASUQUERYCTLINFO_N.Opis:elink.
.br
:link dependent reftype=hd refid=ASUQUERYCTLINFO_C.Przyk�adowy kod:elink.


.* SK�ADNIA
.*
:h3 group=2 x=30% width=70% hide name=ASUQUERYCTLINFO_S.AsuQueryCtlInfo - Sk�adnia
:p.Funkcja przekazuej numer wersji do modu�u asuactl.c.
:p.
:cgraphic.
#include <os2.h>
#include "asuintl.h"

HWND     :link dependent reftype=hd refid=ASUQUERYCTLINFO_HWND.hwnd:elink.;       // Uchwyt okna
ULONG    :link dependent reftype=hd refid=ASUQUERYCTLINFO_MSG.msg:elink.;        // Kod komunikatu
MPARAM   :link dependent reftype=hd refid=ASUQUERYCTLINFO_MP1.mp1:elink.;        // Wska�nik do struktur modu�u asuactl.c
:link dependent reftype=hd refid=CTLREG.CTLREG:elink.  :link dependent reftype=hd refid=ASUQUERYCTLINFO_CTL.*CtlInfo:elink.;    // Struktura zawieraj�ca informacje o klasie okna
LONG     :link dependent reftype=hd refid=ASUQUERYCTLINFO_E.rc:elink.;         // Rezultat

rc =  AsuQueryCtlInfo(HWND hwnd, LONG msg, MPARAM mp1,
                      CTLINFO *CtlInfo);
:ecgraphic.



.* Parametr hwnd
.*
:h3 group=10 x=30% y=bottom width=70% height=225x hide name=ASUQUERYCTLINFO_HWND.AsuQueryCtlInfo Parametr - hwnd
:dl tsize=3 break=all.
:dt.:hp2.hwnd:ehp2. (HWND) - parametr.
:dd.Uchwyt okna. Funkcja AsuQueryCtlInfo odczytuje numer wersji tylko wtedy gdy
parametr ten jest r�wny NULLHANDLE.
:edl.

.* Parametr msg
.*
:h3 group=11 x=30% y=bottom width=70% height=225x hide name=ASUQUERYCTLINFO_MSG.AsuQueryCtlInfo Parametr - msg
:dl tsize=3 break=all.
:dt.:hp2.msg:ehp2. (ULONG) - parametr
:dd.Kod komunikatu.
Funkcja AsuQueryCtlInfo odczytuje numer wersji tylko wtedy gdy
parametr ten jest r�wny WM_NULL.
:edl.

.* Parametr mp1
.*
:h3 group=12 x=30% y=bottom width=70% height=225x hide name=ASUQUERYCTLINFO_MP1.AsuQueryCtlInfo Parametr - mp1
:dl tsize=3 break=all.
:dt.:hp2.mp1:ehp2. MPARAM - rezultat
:dd.Funkcja :link reftype=hd refid=ASUGETCOMPVER.AsuGetComponentVersion:elink.
przekazuje za po�rednictwem tego parametru wska�nik do swoich wewn�trznych struktur
danych.
:edl.

.* Parametr CtlInfo
.*
:h3 group=13 x=30% y=bottom width=70% height=225x hide name=ASUQUERYCTLINFO_CTL.AsuQueryCtlInfo Parametr - *CtlInfo
:dl tsize=3 break=all.
:dt.:hp2.*CtlInfo:ehp2. :link dependent reftype=hd refid=CTLREG.CTLREG:elink. - parametr
:dd.Wska�nik do struktury, kt�ra powinna by� zainicjowana danymi dotycz�cymi
okna kontrolnego.
:edl.

.* PARAMETRY
.*
:h3 group=3 x=30% width=70% hide name=ASUQUERYCTLINFO_P.AsuQueryCtlInfo - Parametry
:dl tsize=3 break=all.
:dt.:hp2.hwnd:ehp2. (HWND) - parametr.
:dd.Uchwyt okna. Funkcja AsuQueryCtlInfo odczytuje numer wersji tylko wtedy gdy
parametr ten jest r�wny NULLHANDLE.
:dt.:hp2.msg:ehp2. (ULONG) - parametr
:dd.Kod komunikatu.
Funkcja AsuQueryCtlInfo odczytuje numer wersji tylko wtedy gdy
parametr ten jest r�wny WM_NULL.
:dt.:hp2.mp1:ehp2. MPARAM - rezultat
:dd.Funkcja :link reftype=hd refid=ASUGETCOMPVER.AsuGetComponentVersion:elink.
przekazuje za po�rednictwem tego parametru wska�nik do swoich wewn�trznych struktur
danych.
:dt.:hp2.*CtlInfo:ehp2. :link dependent reftype=hd refid=CTLREG.CTLREG:elink. - parametr
:dd.Wska�nik do struktury, kt�ra powinna by� zainicjowana danymi dotycz�cymi
okna kontrolnego.
:edl.


.* Rezultaty
.*
:h3 group=4 x=30% y=bottom width=70% height=225x hide name=ASUQUERYCTLINFO_E.AsuQueryCtlInfo - rezultaty
:dl tsize=3 break=all.
:dt.:hp2.rc:ehp2. (LONG)
:dd.Warto�� zarezerwowana, r�wna 0.
:edl.


.* OPIS
.*
:h3 group=5 x=30% width=70% hide name=ASUQUERYCTLINFO_N.AsuQueryCtlInfo - Opis
:p.Funkcja powinna zosta� wywo�ywana w g��wnej procedurze okna kontrolnego,
podczas przetwarzania komunikatu WM_NULL.
:p.Funkcja przepisuje parametry dostarczone za po�rednictwem struktury
:link dependent reftype=hd refid=CTLREG.CTLREG:elink.
do obszaru wskazywanego przez parametr :hp1.mp1:ehp1., znajduj�cego si�
w obszarze zmiennych funkcji
:link reftype=hd refid=ASUGETCOMPVER.AsuGetComponentVersion:elink..
:p.Funkcja ta wywo�uje g��wn� procedur� okna kontrolnego z nast�puj�cymi parametrami&colon.
:dl tsize=10 break=none compact.
:dt.hwnd
:dd.NULLHANDLE
:dt.mp1
:dd.WM_NULL
:dt.mp2
:dd.!= NULL - wska�nik do docelowego obszaru danych.
:dt.mp2
:dd.0
:edl.
:p.Natomiast procedura okna powinna wywo�a� AsuQueryCtlInfo.
:p.Poniewa� system wysy�a komunikat WM_NULL z nast�puj�cymi parametrami&colon.
:dl tsize=10 break=none compact.
:dt.hwnd
:dd.!= NULLHANDLE
:dt.mp1
:dd.WM_NULL
:dt.mp2
:dd.0
:dt.mp2
:dd.0
:edl.
:p.tak wi�c jest mo�liwe rozr��nienie wywo�a� i prawid�owa reakcja na nie.
:p.Czwartym parametrem funkcji AsuQueryCtlInfo powinien by� wska�nik
do zainicjowanej struktury
:link dependent reftype=hd refid=CTLREG.CTLREG:elink.,
zawieraj�cej dane dotycz�ce obiektu.


.* Przyk�adowy kod
.*
:h3 group=6 x=30% width=70% hide name=ASUQUERYCTLINFO_C.AsuQueryCtlInfo - Przyk�adowy kod
:p.W przyk�adzie mamy dwie definicje struktury
:link dependent reftype=hd refid=CTLREG.CTLREG:elink.,
jedna u�ywana przy wsp��pracy z RCS-em, druga wykorzystywana
podczas "r�cznego" nadawania numeru wersji.
:p.
:cgraphic.
#include <os2.h>
#include "asuintl.h"

// Deklaracja danych u�ywana przy korzystaniu z RCS-a
static CTLREG ctlButtonVersion =
{ sizeof(CTLREG),
  "$Revision: 1.2 $",
  0, 0
};

// Deklaracja danych u�ywana przy r�cznym podawaniu numeru wersji
static CTLREG ctlButtonVersion =
{ sizeof(CTLREG),
  NULL,
  1, 40
};




// Fragment procedury okna obs�uguj�cej odczyt wersji
switch(msg)
{ case WM_NULL:
    AsuQueryCtlInfo(hwnd, msg, mp1, &amp.ctlButtonVersion);
    break;
}
:ecgraphic.





.* Funkcja PpmQueryPresColors
.*
:h3 group=1 width=30% scroll=none name=PPMQUERYPRESCOLORS.PpmQueryPresColors
:link dependent group=40 reftype=hd auto refid=PPMQUERYPRESCOLORS_S.
:link dependent group=1  reftype=hd auto refid=PPMQUERYPRESCOLORS.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=PPMQUERYPRESCOLORS_S.Sk�adnia:elink.
.br
:link dependent reftype=hd refid=PPMQUERYPRESCOLORS_P.Parametry:elink.
.br
:link dependent reftype=hd refid=PPMQUERYPRESCOLORS_E.Rezultaty:elink.
.br
:link dependent reftype=hd refid=PPMQUERYPRESCOLORS_N.Opis:elink.
.br
:link dependent reftype=hd refid=PPMQUERYPRESCOLORS_C.Przyk�adowy kod:elink.


.* SK�ADNIA
.*
:h3 group=2 x=30% width=70% hide name=PPMQUERYPRESCOLORS_S.PpmQueryPresColors - Sk�adnia
:p.Funkcja tworzy tablic� kolor�w na podstawie presentation parameters okna.
:p.
:cgraphic.
#define  INCL_WINSYS
#include <os2.h>
#include "pparams.h"

HWND        :link dependent reftype=hd refid=PPMQUERYPRESCOLORS_HWND.hwnd:elink.;        // Uchwyt okna
LONG        :link dependent reftype=hd refid=PPMQUERYPRESCOLORS_NCOLORS.nColors:elink.;     // Liczba odczytywanych kolor�w
:link dependent reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. :link dependent reftype=hd refid=PPMQUERYPRESCOLORS_PPM.*ppm:elink.;         // Struktura steruj�ca
LONG       :link dependent reftype=hd refid=PPMQUERYPRESCOLORS_LCOLTABLE.*lColTable:elink.;   // Tworzona tablica kolor�w
LONG        :link dependent reftype=hd refid=PPMQUERYPRESCOLORS_E.rc:elink.;          // Kod b��du

rc =  PpmQueryPresColors(HWND hwnd, LONG nColors,
                         PPTEMPLATE *ppm,
                         LONG *lColTable);
:ecgraphic.



.* Parametr hwnd
.*
:h3 group=10 x=30% y=bottom width=70% height=225x hide name=PPMQUERYPRESCOLORS_HWND.PpmQueryPresColors Parametr - hwnd
:dl tsize=3 break=all.
:dt.:hp2.hwnd:ehp2. (HWND) - parametr.
:dd.Uchwyt okna, kt�rego dotyczy odczyt.
:edl.

.* Parametr nColors
.*
:h3 group=11 x=30% y=bottom width=70% height=225x hide name=PPMQUERYPRESCOLORS_NCOLORS.PpmQueryPresColors Parametr - nColors
:dl tsize=3 break=all.
:dt.:hp2.nColors:ehp2. (LONG) - parametr
:dd.Liczba odczytywanych kolor�w, i jednocze�nie liczba p�l w tablicy struktur steruj�cych
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. oraz tworzonej tablicy kolor�w
lColTable.
:edl.

.* Parametr ppm
.*
:h3 group=12 x=30% y=bottom width=70% height=225x hide name=PPMQUERYPRESCOLORS_PPM.PpmQueryPresColors Parametr - *ppm
:dl tsize=3 break=all.
:dt.:hp2.*ppm:ehp2. (:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.) - parametr
:dd.Wska�nik do tablicy zawieraj�cej list� odczytywanych parametr�w prezentacji, oraz
odpowiadaj�cych im domy�lnych kolor�w systemowych.
:p.Tablica struktur
:link dependent reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. mo�e te� zawiera�
mi�dzy innymi informacje dotycz�ce dziedziczenia parametr�w prezentacji.
:edl.

.* Parametr lColTable
.*
:h3 group=13 x=30% y=bottom width=70% height=225x hide name=PPMQUERYPRESCOLORS_LCOLTABLE.PpmQueryPresColors Parametr - *lColTable
:dl tsize=3 break=all.
:dt.:hp2.*lColTable:ehp2. (PLONG) - rezultat
:dd.Wska�nik do tablicy zawieraj�cej odczytane kolory, odpowiadaj�ce parametrom
prezentacji zdefiniowanym w tablicy
:link reftype=hd dependent refid=PPTEMPLATE.PPTEMPLATE:elink..
:edl.

.* PARAMETRY
.*
:h3 group=3 x=30% width=70% hide name=PPMQUERYPRESCOLORS_P.PpmQueryPresColors - Parametry
:dl tsize=3 break=all.
:dt.:hp2.hwnd:ehp2. (HWND) - parametr.
:dd.Uchwyt okna, kt�rego dotyczy odczyt.
:dt.:hp2.nColors:ehp2. (LONG) - parametr
:dd.Liczba odczytywanych kolor�w, i jednocze�nie liczba p�l w tablicy struktur steruj�cych
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. oraz tworzonej tablicy kolor�w
lColTable.
:dt.:hp2.*ppm:ehp2. (:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.) - parametr
:dd.Wska�nik do tablicy zawieraj�cej list� odczytywanych parametr�w prezentacji, oraz
odpowiadaj�cych im domy�lnych kolor�w systemowych.
:p.Tablica struktur
:link dependent reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. mo�e te� zawiera�
mi�dzy innymi informacje dotycz�ce dziedziczenia parametr�w prezentacji.
:dt.:hp2.*lColTable:ehp2. (PLONG) - rezultat
:dd.Wska�nik do tablicy zawieraj�cej odczytane kolory, odpowiadaj�ce parametrom
prezentacji zdefiniowanym w tablicy
:link reftype=hd dependent refid=PPTEMPLATE.PPTEMPLATE:elink..
:edl.


.* Rezultaty
.*
:h3 group=4 x=30% y=bottom width=70% height=225x hide name=PPMQUERYPRESCOLORS_E.PpmQueryPresColors - rezultaty
:dl tsize=3 break=all.
:dt.:hp2.rc:ehp2. (LONG)
:dd.Warto�� zarezerwowana, r�wna 0.
:edl.


.* OPIS
.*
:h3 group=5 x=30% width=70% hide name=PPMQUERYPRESCOLORS_N.PpmQueryPresColors - Opis
:p.Funkcja odczytuje kolory (warto�ci RGB) odpowiadaj�ce parametrom prezentacji
zdefiniowanym w tablicy :hp1.*ppm:ehp1. i umieszcza je w tablicy :hp1.lColTable:ehp1..
Programista korzystaj�c z danych zawartych w tablicy :hp1.lColTable:ehp1. mo�e teraz
utworzy� za pomoc� funkcji GpiCreateLogColorTable logiczn� tablic� kolor�w,
i skorzysta� z niej podczas rysowania wn�trza okna kontrolnego.
:p.Funkcja PpmQueryPresColors, dla ka�dego elementu tablicy steruj�cej
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.
na podstawie zapisanych w niej danych odczytuje kolor wed�ug
nast�puj�cego algorytmu&colon.
:ol.
:li.Funkcja PpmQueryPresColors pr�buje odczyta� parametr prezentacji zapisany
w polau :link reftype=hd refid=PPT_ULPRESPARAM.ulPresParam:elink.
struktury :link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink..
:p.Podczas odczytu wykorzystywane s� opcje zapisane w polu
:link reftype=hd refid=PPT_ULPRESFLAGS.ulPresFlags:elink.. Odpowiadaj� one parametrowi
:hp1.flOptions:ehp1. funkcji WinQueryPresParam, i mog� by� kombinacj� nast�puj�cych
warto�ci&colon.

:dl tsize=6 break=all.
:dt.QPF_NOINHERIT
:dd.Odczytywany parametr prezentacji nie jest dziedziczony po w�a�cicielu
okna :hp1.hwnd:ehp1. korzystaj�cego z funkcji PpmQueryPresColors.
Gdy opcja nie jest u�yta - parametr jest dziedziczony.
:dt.QPF_ID1COLORINDEX
:dd.Opcja powinna by� u�yta wtedy, gdy w polu
:link reftype=hd refid=PPT_ULPRESPARAM.ulPresParam:elink.
struktury
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.
zdefiniowano parametr prezentacji b�d�cy indeksem odczytywanego koloru
(np. PP_BACKGROUNDCOLORINDEX zamiast PP_BACKGROUNDCOLOR).
:dt.QPF_ID2COLORINDEX
:dd.Opcja nie powinna by� u�ywana.
:dt.QPF_PURERGBCOLOR
:dd.Opcja wymusza odczyt czystego koloru RGB, nie podlegaj�cego ditheringowi.
Opcja ta ma zanczenie tylko dla kart graficznych wy�wietlaj�cych 16 lub 256 kolor�w.             Specifies that either or both of idAttrType1 and idAttrType2
:edl.
:p.Gdy odczyt parametru prezentacji zako�czy si� niepowodzeniem, funkcja PpmQueryPresColors
przechodzi do punktu 2, w przeciwnym wypadku nast�puje odczyt kolejnego koloru.

:li.Gdy w polu
:link reftype=hd refid=PPT_ULSYSCOLOR.ulSysColor:elink. struktury
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. zdefiniowano
jeden z kolor�w systemowych (warto�� r��na od zera) funkcja PpmQueryPresColors
odczytuje ten kolor i przechodzi do odczytu kolejnego koloru. W przeciwnym wypadku
nast�puje przej�cie do punktu 3.

:li.Gdy odczyt parametru prezentacji nie powiedzie si� i nie zdefiniowano koloru systemowego,
funkcja PpmQueryPresColors odczytuje warto�� RGB zapisan� na sta�e w polu
:link reftype=hd refid=PPT_ULRGBCOLOR.ulRgbColor:elink. struktury
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. i przechodzi
do odczytu kolejnego koloru.
:eol.


.* Przyk�adowy kod
.*
:h3 group=6 x=30% width=70% hide name=PPMQUERYPRESCOLORS_C.PpmQueryPresColors - Przyk�adowy kod
:p.W przyk�adzie, odczytywane s� kolory t�a oraz tekstu dla odblokowanego i zablokowanego
okna. Na tej podstawie jest tworzona logiczna tablica kolor�w. Kolory tekstu
nie s� dziedziczone.
:p.
:cgraphic.
#define  INCL_WINSYS
#define  INCL_GPILOGCOLORTABLE
#include <os2.h>
#include "pparams.h"

#define  CTL_MAXCOLOR         4;   // Liczba kolor�w u�ywanych w oknie
#define  CTL_BACKGRND         0;   // Kolor t�a
#define  CTL_DISBACKGRND      1;   // Kolor t�a zablokowanego okna
#define  CTL_TEXT             2;   // Kolor tekstu
#define  CTL_DISTEXT          3;   // Kolor tekstu w zablokowanym oknie

HWND  hwndControl;            // Uchwyt okna kontrolnego
HPS   hpsControl;             // Uchwyt presentation space okna kontrolnego
LONG  coltab[CTL_MAXCOLOR];   // Tablica kolor�w

// Lista parametr�w prezentacji u�ytych w oknie
PPTEMPLATE PresParams[CTL_MAXCOLOR] =
  {{ PP_BACKGROUNDCOLOR,         0L,            // Kolor t�a
     SYSCLR_ENTRYFIELD,          0xFFFFFF,
   },
   { PP_DISABLEDBACKGROUNDCOLOR, 0L,            // Kolor t�a zablokowanego okna
     SYSCLR_ENTRYFIELD,          0xFFFFFF,
   },
   { PP_FOREGROUNDCOLOR,         QPF_NOINHERIT, // Kolor tekstu
     SYSCLR_WINDOWTEXT,          0x000000,
   },
   { PP_DISABLEDFOREGROUNDCOLOR, QPF_NOINHERIT, // Kolor tekstu w zablokowanym oknie
     0,                          0x808080,
   }
  };

// Odczyt kolor�w okna
PppQueryPresColors(hwndControl, CTL_MAXCOLOR, PresParams, coltab);
// Utworzenie logicznej tablicy kolor�w
GpiCreateLogColorTable(hpsControl, 0L, LCOLF_CONSECRGB, 0L, CTL_MAXCOLOR, coltab);
:ecgraphic.





.* Funkcja PpmStorePresParams
.*
:h3 group=1 width=30% scroll=none name=PPMSTOREPRESPARAMS.PpmStorePresParams
:link dependent group=40 reftype=hd auto refid=PPMSTOREPRESPARAMS_S.
:link dependent group=1  reftype=hd auto refid=PPMSTOREPRESPARAMS.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=PPMSTOREPRESPARAMS_S.Sk�adnia:elink.
.br
:link dependent reftype=hd refid=PPMSTOREPRESPARAMS_P.Parametry:elink.
.br
:link dependent reftype=hd refid=PPMSTOREPRESPARAMS_E.Rezultaty:elink.
.br
:link dependent reftype=hd refid=PPMSTOREPRESPARAMS_N.Opis:elink.
.br
:link dependent reftype=hd refid=PPMSTOREPRESPARAMS_C.Przyk�adowy kod:elink.


.* SK�ADNIA
.*
:h3 group=2 x=30% width=70% hide name=PPMSTOREPRESPARAMS_S.PpmStorePresParams - Sk�adnia
:p.Funkcja zapisuje parametry prezentacji odczytane ze struktury PRESPARAMS w pami�ci okna.
:p.
:cgraphic.
#define  INCL_WINSYS
#include <os2.h>
#include "pparams.h"

HWND        :link dependent reftype=hd refid=PPMSTOREPRESPARAMS_HWND.hwnd:elink.;        // Uchwyt okna
PRESPARAMS :link dependent reftype=hd refid=PPMSTOREPRESPARAMS_PPM.*ppm:elink.;         // Lista parametr�w prezentacji
:link dependent reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. :link dependent reftype=hd refid=PPMSTOREPRESPARAMS_PPT.*ppt:elink.;         // Struktura steruj�ca
LONG        :link dependent reftype=hd refid=PPMSTOREPRESPARAMS_NCOLORS.nColors:elink.;     // Liczba zapisywanych parametr�w
LONG        :link dependent reftype=hd refid=PPMSTOREPRESPARAMS_E.rc:elink.;          // Kod b��du

rc PpmStorePresParams(HWND hwnd, PRESPARAMS *ppm,
                      PPTEMPLATE *ppt, LONG nColors);
:ecgraphic.



.* Parametr hwnd
.*
:h3 group=10 x=30% y=bottom width=70% height=225x hide name=PPMSTOREPRESPARAMS_HWND.PpmStorePresParams Parametr - hwnd
:dl tsize=3 break=all.
:dt.:hp2.hwnd:ehp2. (HWND) - parametr.
:dd.Uchwyt okna do kt�rego s� zapisywane parametry.
:edl.

.* Parametr *ppm
.*
:h3 group=11 x=30% y=bottom width=70% height=225x hide name=PPMSTOREPRESPARAMS_PPM.PpmStorePresParams Parametr - *ppm
:dl tsize=3 break=all.
:dt.:hp2.*ppm:ehp2. (LONG) - parametr
:dd.Wska�nik do struktury PRESPARAMS, zawieraj�cej zapisywane parametry prezentacji
i ich warto�ci.
:edl.

.* Parametr *ppt
.*
:h3 group=12 x=30% y=bottom width=70% height=225x hide name=PPMSTOREPRESPARAMS_PPT.PpmStorePresParams Parametr - *ppt
:dl tsize=3 break=all.
:dt.:hp2.*ppt:ehp2. (:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.) - parametr
:dd.Wska�nik do tablicy zawieraj�cej list� parametr�w prezentacji, kt�re
nale�y skopiowa� z przekazanej struktury :hp1.*ppm:ehp1.
:p.Gdy wska�nik :hp1.*ppt:ehp1. jest r�wny NULL, kopiowane s� wszystkie parametry
zapisane na li�cie :hp1.*ppm:ehp1..
:edl.

.* Parametr nColors
.*
:h3 group=13 x=30% y=bottom width=70% height=225x hide name=PPMSTOREPRESPARAMS_NCOLORS.PpmStorePresParams Parametr - *nColors
:dl tsize=3 break=all.
:dt.:hp2.nColors:ehp2. (LONG) - parametr
:dd.Rozmiar tablicy
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.
zawieraj�cej list� kolor�w kopiowanych z tablicy :hp1.*ppm:ehp1..
:edl.

.* PARAMETRY
.*
:h3 group=3 x=30% width=70% hide name=PPMSTOREPRESPARAMS_P.PpmStorePresParams - Parametry
:dl tsize=3 break=all.
:dt.:hp2.hwnd:ehp2. (HWND) - parametr.
:dd.Uchwyt okna do kt�rego s� zapisywane parametry.
:dt.:hp2.*ppm:ehp2. (LONG) - parametr
:dd.Wska�nik do struktury PRESPARAMS, zawieraj�cej zapisywane parametry prezentacji
i ich warto�ci.
:dt.:hp2.*ppt:ehp2. (:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.) - parametr
:dd.Wska�nik do tablicy zawieraj�cej list� parametr�w prezentacji, kt�re
nale�y skopiowa� z przekazanej struktury :hp1.*ppm:ehp1.
:p.Gdy wska�nik :hp1.*ppt:ehp1. jest r�wny NULL, kopiowane s� wszystkie parametry
zapisane na li�cie :hp1.*ppm:ehp1..
:dt.:hp2.nColors:ehp2. (LONG) - parametr
:dd.Rozmiar tablicy
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.
zawieraj�cej list� kolor�w kopiowanych z tablicy :hp1.*ppm:ehp1..
:edl.


.* Rezultaty
.*
:h3 group=4 x=30% y=bottom width=70% height=225x hide name=PPMSTOREPRESPARAMS_E.PpmStorePresParams - rezultaty
:dl tsize=3 break=all.
:dt.:hp2.rc:ehp2. (LONG)
:dd.Warto�� zarezerwowana, r�wna 0.
:edl.


.* OPIS
.*
:h3 group=5 x=30% width=70% hide name=PPMSTOREPRESPARAMS_N.PpmStorePresParams - Opis
:p.Funkcja mo�e by� u�yta podczas tworzenia okna (komunikat WM_CREATE)
w celu przepisania parametr�w prezentacji przekazanych za po�rednictwem struktury
CREATESTRUCT (pole pPresParams) do pami�ci okna.
:p.Funkcja analizuje struktur� PRESPARAMS, odczytuje zawarte w niej parametry i za
pomoc� WinSetPresParam zapisuje je w pami�ci okna wed�ug nast�puj�cego algorytmu&colon.

:ol.
:li.Je�li odczytany parametr to PP_FONTNAMESIZE funkcja PpmStorePresParams
umieszcza go w pami�ci okna.

:li.Je�li odczytany parametr to kolor, funkcja PpmStorePresParams sprawdza czy
wska�nik :hp1.*ppt:ehp1. jest r�wny NULL. Je�li tak to zapisuje parametr w pami�ci
okna. Je�li nie to nast�puje przej�cie do punktu 3.

:li.Funkcja poszukuje parametru na li�cie przekazanej za po�rednictwem
wska�nika :hp1.*ppt:ehp1. (tablica struktur :link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.).
Parametr znaleziony na li�cie zostaje zapisany w pami�ci okna.
:eol.
:p.Pola&colon.
ulPresFlags, ulSysColor i ulRgbColor struktury
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.
s� przez funkcj� PpmStorePresParams ignorowane.


.* Przyk�adowy kod
.*
:h3 group=6 x=30% width=70% hide name=PPMSTOREPRESPARAMS_C.PpmStorePresParams - Przyk�adowy kod
:p.W przyk�adzie, podczas obs�ugi WM_CREATE, parametry prezentacji przekazane za
po�rednictwem struktury CREATESTRUCT zostaj� przepisane do pami�ci okna.
:p.
:cgraphic.
#define  INCL_WINWINDOWMGR
#define  INCL_WINSYS
#include <os2.h>
#include "pparams.h"

MPARAM  mp2;        // Drugi parametr przekazany procedurze okna


case WM_CREATE:
  { CREATESTRUCT *pcr;

    // Odczyt adresu struktury inicjuj�cej
    pcr = (CREATESTRUCT *)PVOIDFROMMP(mp2);
    // Za�adowanie presentation parameters ze struktury CREATESTRUCT
    PpmStorePresParams(hwnd, (PRESPARAMS *)pcr->pPresParams, NULL, 0L);
  }
  break;
:ecgraphic.

