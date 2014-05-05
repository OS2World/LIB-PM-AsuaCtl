.* Dokumentacja techniczna biblioteki okien kontrolnych asuactl.dll
.* opis funkcji og¢lnego przeznaczenia
.*

.* Rozdzeiaà 3 - Funkcje og¢lnego przeznaczenia
.*
:h1 res=003.Funkcje
:p.Rozdziaà zawiera opis funkcji zawartych w module asuactl.dll.

.* Rozdziaà 3.1 - Funkcje eksportowane
.*
:h2 res=031.Funkcje eksportowane.
:p.Rozdziaà zawiera opis funkcji, z kt¢rych moæe korzystaÜ uæytkownik
biblioteki asuactl.dll.


.* Funkcja AsuCtlInitialize
.*
:h3 group=1 width=30% scroll=none name=ASUCTLINITIALIZE.AsuCtlInitialize
:link dependent group=40 reftype=hd auto refid=ASUCTLINITIALIZE_S.
:link dependent group=1  reftype=hd auto refid=ASUCTLINITIALIZE.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=ASUCTLINITIALIZE_S.Skàadnia:elink.
.br
:link dependent reftype=hd refid=ASUCTLINITIALIZE_P.Parametry:elink.
.br
:link dependent reftype=hd refid=ASUCTLINITIALIZE_E.Rezultaty:elink.
.br
:link dependent reftype=hd refid=ASUCTLINITIALIZE_N.Opis:elink.


.* SKùADNIA
.*
:h3 group=2 x=30% width=70% hide name=ASUCTLINITIALIZE_S.AsuCtlInitialize - Skàadnia
:p.Funkcja inicjuje bibliotek© asuactl.dll.
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
:dd.Kod bà©du&colon.
:dl tsize=3 break=all.
:dt.0
:dd.Poprawne wykonanie funkcji.
:dt.1
:dd.Bà•d rejsetracji klas okien.
:edl.
:edl.


.* OPIS
:h3 group=5 x=30% width=70% hide name=ASUCTLINITIALIZE_N.AsuCtlInitialize - Opis
:p.Funkcja rejestruje wszystkie klasy okien zawarte w bibliotece asuactl.dll.
:p.W przypadku, gdy wyst•pi bàad rejestracji jednej z klas okien, funkcja zwraca 1,
w przeciwnym wypadku 0. Wyst•pienie bà©du rejestracji kt¢rejò z klas nie powoduje
przerwania wykonywania funkcji i zostaje podj©ta pr¢ba rejestracji klas,
znajduj•cych si© po bà©dnej.







.* Funkcja AsuGetComponentVersion
.*
:h3 group=1 width=30% scroll=none name=ASUGETCOMPVER.AsuGetComponentVersion
:link dependent group=40 reftype=hd auto refid=ASUGETCOMPVER_S.
:link dependent group=1  reftype=hd auto refid=ASUGETCOMPVER.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=ASUGETCOMPVER_S.Skàadnia:elink.
.br
:link dependent reftype=hd refid=ASUGETCOMPVER_P.Parametry:elink.
.br
:link dependent reftype=hd refid=ASUGETCOMPVER_E.Rezultaty:elink.
.br
:link dependent reftype=hd refid=ASUGETCOMPVER_N.Opis:elink.


.* SKùADNIA
.*
:h3 group=2 x=30% width=70% hide name=ASUGETCOMPVER_S.AsuGetComponentVersion - Skàadnia
:p.Funkcja umoæliwia odczyt wersji wybranego okna kontrolnego zawartego
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
:dd.Klasa okna, dla kt¢rej jest dokonywany odczyt wersji.
:edl.


.* PARAMETRY
.*
:h3 group=3 x=30% width=70% hide name=ASUGETCOMPVER_P.AsuGetComponentVersion - Parametry
:dl tsize=3 break=all.
:dt.:hp2.class:ehp2. (PSZ) - parametr.
:dd.Klasa okna, dla kt¢rej jest dokonywany odczyt wersji.
:edl.


.* Rezultaty
:h3 group=4 x=30% y=bottom width=70% height=225x hide name=ASUGETCOMPVER_E.AsuGetComponentVersion - rezultaty
:dl tsize=3 break=all.
:dt.:hp2.ver:ehp2. (LONG)
:dd.Odczytany numer wersji okna kontrolnego o podanej klasie.
.br
Bardziej znacz•ce sàowo rezultatu zawiera gà¢wny numer wersji, a mniej znacz•ce dodatkowy
numer wersji. Kod sàuæ•cy do odczytu obydwu czàon¢w numeru wersji moæe wygl•daÜ
nast©puj•co&colon.
:cgraphic.
// Odczyt wersji paska narz©dzi
ver = AsuGetComponentVersion(WC_TOOLBAR);
// Gà¢wny numer wersji
VerMajor = ver >> 16;
// Numer podwersji
VerMinor = ver &amp. 0xFFFF;
:ecgraphic.
:p.Zwracane wartoòci naleæy interpretowaÜ w nast©puj•cy spos¢b&colon.
:table cols='10 10 20' rules=none frame=none.
:row.
:c.VerMajor
:c.VerMinor
:c.Peàny numer wersji
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
:p.Jeæeli w procedurze okna kontrolnego zaimplementowano

metod© odczytu wersji, to funkcja AsuGetComponentVersion umoæliwia sprawdzenie
poprawnoòci rejestracji klasy okna. W przypadku gdy klasa nie zostaàa zarejestrowana
funkcja zwraca 0 zamiast numeru wersji.






.* Funkcja AsuGetVersion
.*
:h3 group=1 width=30% scroll=none name=ASUGETVERSION.AsuGetVersion
:link dependent group=40 reftype=hd auto refid=ASUGETVERSION_S.
:link dependent group=1  reftype=hd auto refid=ASUGETVERSION.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=ASUGETVERSION_S.Skàadnia:elink.
.br
:link dependent reftype=hd refid=ASUGETVERSION_P.Parametry:elink.
.br
:link dependent reftype=hd refid=ASUGETVERSION_E.Rezultaty:elink.
.br
:link dependent reftype=hd refid=ASUGETVERSION_N.Opis:elink.


.* SKùADNIA
.*
:h3 group=2 x=30% width=70% hide name=ASUGETVERSION_S.AsuGetVersion - Skàadnia
:p.Funkcja umoæliwia odczyt wersji biblioteki dynamicznej asuactl.dll.
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
Bardziej znacz•ce sàowo rezultatu zawiera gà¢wny numer wersji, a mniej znacz•ce dodatkowy
numer wersji. Kod sàuæ•cy do odczytu obydwu czàon¢w numeru wersji moæe wygl•daÜ
nast©puj•co&colon.
:cgraphic.
// Odczyt wersji biblioteki asuactl.dll
ver = AsuGetVersion();
// Gà¢wny numer wersji
VerMajor = ver >> 16;
// Numer podwersji
VerMinor = ver &amp. 0xFFFF;
:ecgraphic.
:p.Zwracane wartoòci naleæy interpretowaÜ w nast©puj•cy spos¢b&colon.
:table cols='10 10 20' rules=none frame=none.
:row.
:c.VerMajor
:c.VerMinor
:c.Peàny numer wersji
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






.* Rozdziaà 3.2 - Funkcje wewn©trzne
:h2 res=032.Funkcje wewn©trzne.
:p.Rozdziaà zawiera opis funkcji niedost©pnych dla uæytkownika,
z kt¢rych moæe korzystaÜ programista dodaj•cy nowe okna kontrolne
do biblioteki asuactl.dll.


.* Funkcja AsuQueryCtlInfo
.*
:h3 group=1 width=30% scroll=none name=ASUQUERYCTLINFO.AsuQueryCtlInfo
:link dependent group=40 reftype=hd auto refid=ASUQUERYCTLINFO_S.
:link dependent group=1  reftype=hd auto refid=ASUQUERYCTLINFO.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=ASUQUERYCTLINFO_S.Skàadnia:elink.
.br
:link dependent reftype=hd refid=ASUQUERYCTLINFO_P.Parametry:elink.
.br
:link dependent reftype=hd refid=ASUQUERYCTLINFO_E.Rezultaty:elink.
.br
:link dependent reftype=hd refid=ASUQUERYCTLINFO_N.Opis:elink.
.br
:link dependent reftype=hd refid=ASUQUERYCTLINFO_C.Przykàadowy kod:elink.


.* SKùADNIA
.*
:h3 group=2 x=30% width=70% hide name=ASUQUERYCTLINFO_S.AsuQueryCtlInfo - Skàadnia
:p.Funkcja przekazuej numer wersji do moduàu asuactl.c.
:p.
:cgraphic.
#include <os2.h>
#include "asuintl.h"

HWND     :link dependent reftype=hd refid=ASUQUERYCTLINFO_HWND.hwnd:elink.;       // Uchwyt okna
ULONG    :link dependent reftype=hd refid=ASUQUERYCTLINFO_MSG.msg:elink.;        // Kod komunikatu
MPARAM   :link dependent reftype=hd refid=ASUQUERYCTLINFO_MP1.mp1:elink.;        // Wska´nik do struktur moduàu asuactl.c
:link dependent reftype=hd refid=CTLREG.CTLREG:elink.  :link dependent reftype=hd refid=ASUQUERYCTLINFO_CTL.*CtlInfo:elink.;    // Struktura zawieraj•ca informacje o klasie okna
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
parametr ten jest r¢wny NULLHANDLE.
:edl.

.* Parametr msg
.*
:h3 group=11 x=30% y=bottom width=70% height=225x hide name=ASUQUERYCTLINFO_MSG.AsuQueryCtlInfo Parametr - msg
:dl tsize=3 break=all.
:dt.:hp2.msg:ehp2. (ULONG) - parametr
:dd.Kod komunikatu.
Funkcja AsuQueryCtlInfo odczytuje numer wersji tylko wtedy gdy
parametr ten jest r¢wny WM_NULL.
:edl.

.* Parametr mp1
.*
:h3 group=12 x=30% y=bottom width=70% height=225x hide name=ASUQUERYCTLINFO_MP1.AsuQueryCtlInfo Parametr - mp1
:dl tsize=3 break=all.
:dt.:hp2.mp1:ehp2. MPARAM - rezultat
:dd.Funkcja :link reftype=hd refid=ASUGETCOMPVER.AsuGetComponentVersion:elink.
przekazuje za poòrednictwem tego parametru wska´nik do swoich wewn©trznych struktur
danych.
:edl.

.* Parametr CtlInfo
.*
:h3 group=13 x=30% y=bottom width=70% height=225x hide name=ASUQUERYCTLINFO_CTL.AsuQueryCtlInfo Parametr - *CtlInfo
:dl tsize=3 break=all.
:dt.:hp2.*CtlInfo:ehp2. :link dependent reftype=hd refid=CTLREG.CTLREG:elink. - parametr
:dd.Wska´nik do struktury, kt¢ra powinna byÜ zainicjowana danymi dotycz•cymi
okna kontrolnego.
:edl.

.* PARAMETRY
.*
:h3 group=3 x=30% width=70% hide name=ASUQUERYCTLINFO_P.AsuQueryCtlInfo - Parametry
:dl tsize=3 break=all.
:dt.:hp2.hwnd:ehp2. (HWND) - parametr.
:dd.Uchwyt okna. Funkcja AsuQueryCtlInfo odczytuje numer wersji tylko wtedy gdy
parametr ten jest r¢wny NULLHANDLE.
:dt.:hp2.msg:ehp2. (ULONG) - parametr
:dd.Kod komunikatu.
Funkcja AsuQueryCtlInfo odczytuje numer wersji tylko wtedy gdy
parametr ten jest r¢wny WM_NULL.
:dt.:hp2.mp1:ehp2. MPARAM - rezultat
:dd.Funkcja :link reftype=hd refid=ASUGETCOMPVER.AsuGetComponentVersion:elink.
przekazuje za poòrednictwem tego parametru wska´nik do swoich wewn©trznych struktur
danych.
:dt.:hp2.*CtlInfo:ehp2. :link dependent reftype=hd refid=CTLREG.CTLREG:elink. - parametr
:dd.Wska´nik do struktury, kt¢ra powinna byÜ zainicjowana danymi dotycz•cymi
okna kontrolnego.
:edl.


.* Rezultaty
.*
:h3 group=4 x=30% y=bottom width=70% height=225x hide name=ASUQUERYCTLINFO_E.AsuQueryCtlInfo - rezultaty
:dl tsize=3 break=all.
:dt.:hp2.rc:ehp2. (LONG)
:dd.WartoòÜ zarezerwowana, r¢wna 0.
:edl.


.* OPIS
.*
:h3 group=5 x=30% width=70% hide name=ASUQUERYCTLINFO_N.AsuQueryCtlInfo - Opis
:p.Funkcja powinna zostaÜ wywoàywana w gà¢wnej procedurze okna kontrolnego,
podczas przetwarzania komunikatu WM_NULL.
:p.Funkcja przepisuje parametry dostarczone za poòrednictwem struktury
:link dependent reftype=hd refid=CTLREG.CTLREG:elink.
do obszaru wskazywanego przez parametr :hp1.mp1:ehp1., znajduj•cego si©
w obszarze zmiennych funkcji
:link reftype=hd refid=ASUGETCOMPVER.AsuGetComponentVersion:elink..
:p.Funkcja ta wywoàuje gà¢wn• procedur© okna kontrolnego z nast©puj•cymi parametrami&colon.
:dl tsize=10 break=none compact.
:dt.hwnd
:dd.NULLHANDLE
:dt.mp1
:dd.WM_NULL
:dt.mp2
:dd.!= NULL - wska´nik do docelowego obszaru danych.
:dt.mp2
:dd.0
:edl.
:p.Natomiast procedura okna powinna wywoàaÜ AsuQueryCtlInfo.
:p.Poniewaæ system wysyàa komunikat WM_NULL z nast©puj•cymi parametrami&colon.
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
:p.tak wi©c jest moæliwe rozr¢ænienie wywoàa‰ i prawidàowa reakcja na nie.
:p.Czwartym parametrem funkcji AsuQueryCtlInfo powinien byÜ wska´nik
do zainicjowanej struktury
:link dependent reftype=hd refid=CTLREG.CTLREG:elink.,
zawieraj•cej dane dotycz•ce obiektu.


.* Przykàadowy kod
.*
:h3 group=6 x=30% width=70% hide name=ASUQUERYCTLINFO_C.AsuQueryCtlInfo - Przykàadowy kod
:p.W przykàadzie mamy dwie definicje struktury
:link dependent reftype=hd refid=CTLREG.CTLREG:elink.,
jedna uæywana przy wsp¢àpracy z RCS-em, druga wykorzystywana
podczas "r©cznego" nadawania numeru wersji.
:p.
:cgraphic.
#include <os2.h>
#include "asuintl.h"

// Deklaracja danych uæywana przy korzystaniu z RCS-a
static CTLREG ctlButtonVersion =
{ sizeof(CTLREG),
  "$Revision: 1.2 $",
  0, 0
};

// Deklaracja danych uæywana przy r©cznym podawaniu numeru wersji
static CTLREG ctlButtonVersion =
{ sizeof(CTLREG),
  NULL,
  1, 40
};




// Fragment procedury okna obsàuguj•cej odczyt wersji
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
:link dependent reftype=hd refid=PPMQUERYPRESCOLORS_S.Skàadnia:elink.
.br
:link dependent reftype=hd refid=PPMQUERYPRESCOLORS_P.Parametry:elink.
.br
:link dependent reftype=hd refid=PPMQUERYPRESCOLORS_E.Rezultaty:elink.
.br
:link dependent reftype=hd refid=PPMQUERYPRESCOLORS_N.Opis:elink.
.br
:link dependent reftype=hd refid=PPMQUERYPRESCOLORS_C.Przykàadowy kod:elink.


.* SKùADNIA
.*
:h3 group=2 x=30% width=70% hide name=PPMQUERYPRESCOLORS_S.PpmQueryPresColors - Skàadnia
:p.Funkcja tworzy tablic© kolor¢w na podstawie presentation parameters okna.
:p.
:cgraphic.
#define  INCL_WINSYS
#include <os2.h>
#include "pparams.h"

HWND        :link dependent reftype=hd refid=PPMQUERYPRESCOLORS_HWND.hwnd:elink.;        // Uchwyt okna
LONG        :link dependent reftype=hd refid=PPMQUERYPRESCOLORS_NCOLORS.nColors:elink.;     // Liczba odczytywanych kolor¢w
:link dependent reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. :link dependent reftype=hd refid=PPMQUERYPRESCOLORS_PPM.*ppm:elink.;         // Struktura steruj•ca
LONG       :link dependent reftype=hd refid=PPMQUERYPRESCOLORS_LCOLTABLE.*lColTable:elink.;   // Tworzona tablica kolor¢w
LONG        :link dependent reftype=hd refid=PPMQUERYPRESCOLORS_E.rc:elink.;          // Kod bà©du

rc =  PpmQueryPresColors(HWND hwnd, LONG nColors,
                         PPTEMPLATE *ppm,
                         LONG *lColTable);
:ecgraphic.



.* Parametr hwnd
.*
:h3 group=10 x=30% y=bottom width=70% height=225x hide name=PPMQUERYPRESCOLORS_HWND.PpmQueryPresColors Parametr - hwnd
:dl tsize=3 break=all.
:dt.:hp2.hwnd:ehp2. (HWND) - parametr.
:dd.Uchwyt okna, kt¢rego dotyczy odczyt.
:edl.

.* Parametr nColors
.*
:h3 group=11 x=30% y=bottom width=70% height=225x hide name=PPMQUERYPRESCOLORS_NCOLORS.PpmQueryPresColors Parametr - nColors
:dl tsize=3 break=all.
:dt.:hp2.nColors:ehp2. (LONG) - parametr
:dd.Liczba odczytywanych kolor¢w, i jednoczeònie liczba p¢l w tablicy struktur steruj•cych
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. oraz tworzonej tablicy kolor¢w
lColTable.
:edl.

.* Parametr ppm
.*
:h3 group=12 x=30% y=bottom width=70% height=225x hide name=PPMQUERYPRESCOLORS_PPM.PpmQueryPresColors Parametr - *ppm
:dl tsize=3 break=all.
:dt.:hp2.*ppm:ehp2. (:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.) - parametr
:dd.Wska´nik do tablicy zawieraj•cej list© odczytywanych parametr¢w prezentacji, oraz
odpowiadaj•cych im domyòlnych kolor¢w systemowych.
:p.Tablica struktur
:link dependent reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. moæe teæ zawieraÜ
mi©dzy innymi informacje dotycz•ce dziedziczenia parametr¢w prezentacji.
:edl.

.* Parametr lColTable
.*
:h3 group=13 x=30% y=bottom width=70% height=225x hide name=PPMQUERYPRESCOLORS_LCOLTABLE.PpmQueryPresColors Parametr - *lColTable
:dl tsize=3 break=all.
:dt.:hp2.*lColTable:ehp2. (PLONG) - rezultat
:dd.Wska´nik do tablicy zawieraj•cej odczytane kolory, odpowiadaj•ce parametrom
prezentacji zdefiniowanym w tablicy
:link reftype=hd dependent refid=PPTEMPLATE.PPTEMPLATE:elink..
:edl.

.* PARAMETRY
.*
:h3 group=3 x=30% width=70% hide name=PPMQUERYPRESCOLORS_P.PpmQueryPresColors - Parametry
:dl tsize=3 break=all.
:dt.:hp2.hwnd:ehp2. (HWND) - parametr.
:dd.Uchwyt okna, kt¢rego dotyczy odczyt.
:dt.:hp2.nColors:ehp2. (LONG) - parametr
:dd.Liczba odczytywanych kolor¢w, i jednoczeònie liczba p¢l w tablicy struktur steruj•cych
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. oraz tworzonej tablicy kolor¢w
lColTable.
:dt.:hp2.*ppm:ehp2. (:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.) - parametr
:dd.Wska´nik do tablicy zawieraj•cej list© odczytywanych parametr¢w prezentacji, oraz
odpowiadaj•cych im domyòlnych kolor¢w systemowych.
:p.Tablica struktur
:link dependent reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. moæe teæ zawieraÜ
mi©dzy innymi informacje dotycz•ce dziedziczenia parametr¢w prezentacji.
:dt.:hp2.*lColTable:ehp2. (PLONG) - rezultat
:dd.Wska´nik do tablicy zawieraj•cej odczytane kolory, odpowiadaj•ce parametrom
prezentacji zdefiniowanym w tablicy
:link reftype=hd dependent refid=PPTEMPLATE.PPTEMPLATE:elink..
:edl.


.* Rezultaty
.*
:h3 group=4 x=30% y=bottom width=70% height=225x hide name=PPMQUERYPRESCOLORS_E.PpmQueryPresColors - rezultaty
:dl tsize=3 break=all.
:dt.:hp2.rc:ehp2. (LONG)
:dd.WartoòÜ zarezerwowana, r¢wna 0.
:edl.


.* OPIS
.*
:h3 group=5 x=30% width=70% hide name=PPMQUERYPRESCOLORS_N.PpmQueryPresColors - Opis
:p.Funkcja odczytuje kolory (wartoòci RGB) odpowiadaj•ce parametrom prezentacji
zdefiniowanym w tablicy :hp1.*ppm:ehp1. i umieszcza je w tablicy :hp1.lColTable:ehp1..
Programista korzystaj•c z danych zawartych w tablicy :hp1.lColTable:ehp1. moæe teraz
utworzyÜ za pomoc• funkcji GpiCreateLogColorTable logiczn• tablic© kolor¢w,
i skorzystaÜ z niej podczas rysowania wn©trza okna kontrolnego.
:p.Funkcja PpmQueryPresColors, dla kaædego elementu tablicy steruj•cej
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.
na podstawie zapisanych w niej danych odczytuje kolor wedàug
nast©puj•cego algorytmu&colon.
:ol.
:li.Funkcja PpmQueryPresColors pr¢buje odczytaÜ parametr prezentacji zapisany
w polau :link reftype=hd refid=PPT_ULPRESPARAM.ulPresParam:elink.
struktury :link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink..
:p.Podczas odczytu wykorzystywane s• opcje zapisane w polu
:link reftype=hd refid=PPT_ULPRESFLAGS.ulPresFlags:elink.. Odpowiadaj• one parametrowi
:hp1.flOptions:ehp1. funkcji WinQueryPresParam, i mog• byÜ kombinacj• nast©puj•cych
wartoòci&colon.

:dl tsize=6 break=all.
:dt.QPF_NOINHERIT
:dd.Odczytywany parametr prezentacji nie jest dziedziczony po wàaòcicielu
okna :hp1.hwnd:ehp1. korzystaj•cego z funkcji PpmQueryPresColors.
Gdy opcja nie jest uæyta - parametr jest dziedziczony.
:dt.QPF_ID1COLORINDEX
:dd.Opcja powinna byÜ uæyta wtedy, gdy w polu
:link reftype=hd refid=PPT_ULPRESPARAM.ulPresParam:elink.
struktury
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.
zdefiniowano parametr prezentacji b©d•cy indeksem odczytywanego koloru
(np. PP_BACKGROUNDCOLORINDEX zamiast PP_BACKGROUNDCOLOR).
:dt.QPF_ID2COLORINDEX
:dd.Opcja nie powinna byÜ uæywana.
:dt.QPF_PURERGBCOLOR
:dd.Opcja wymusza odczyt czystego koloru RGB, nie podlegaj•cego ditheringowi.
Opcja ta ma zanczenie tylko dla kart graficznych wyòwietlaj•cych 16 lub 256 kolor¢w.             Specifies that either or both of idAttrType1 and idAttrType2
:edl.
:p.Gdy odczyt parametru prezentacji zako‰czy si© niepowodzeniem, funkcja PpmQueryPresColors
przechodzi do punktu 2, w przeciwnym wypadku nast©puje odczyt kolejnego koloru.

:li.Gdy w polu
:link reftype=hd refid=PPT_ULSYSCOLOR.ulSysColor:elink. struktury
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. zdefiniowano
jeden z kolor¢w systemowych (wartoòÜ r¢æna od zera) funkcja PpmQueryPresColors
odczytuje ten kolor i przechodzi do odczytu kolejnego koloru. W przeciwnym wypadku
nast©puje przejòcie do punktu 3.

:li.Gdy odczyt parametru prezentacji nie powiedzie si© i nie zdefiniowano koloru systemowego,
funkcja PpmQueryPresColors odczytuje wartoòÜ RGB zapisan• na staàe w polu
:link reftype=hd refid=PPT_ULRGBCOLOR.ulRgbColor:elink. struktury
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. i przechodzi
do odczytu kolejnego koloru.
:eol.


.* Przykàadowy kod
.*
:h3 group=6 x=30% width=70% hide name=PPMQUERYPRESCOLORS_C.PpmQueryPresColors - Przykàadowy kod
:p.W przykàadzie, odczytywane s• kolory tàa oraz tekstu dla odblokowanego i zablokowanego
okna. Na tej podstawie jest tworzona logiczna tablica kolor¢w. Kolory tekstu
nie s• dziedziczone.
:p.
:cgraphic.
#define  INCL_WINSYS
#define  INCL_GPILOGCOLORTABLE
#include <os2.h>
#include "pparams.h"

#define  CTL_MAXCOLOR         4;   // Liczba kolor¢w uæywanych w oknie
#define  CTL_BACKGRND         0;   // Kolor tàa
#define  CTL_DISBACKGRND      1;   // Kolor tàa zablokowanego okna
#define  CTL_TEXT             2;   // Kolor tekstu
#define  CTL_DISTEXT          3;   // Kolor tekstu w zablokowanym oknie

HWND  hwndControl;            // Uchwyt okna kontrolnego
HPS   hpsControl;             // Uchwyt presentation space okna kontrolnego
LONG  coltab[CTL_MAXCOLOR];   // Tablica kolor¢w

// Lista parametr¢w prezentacji uæytych w oknie
PPTEMPLATE PresParams[CTL_MAXCOLOR] =
  {{ PP_BACKGROUNDCOLOR,         0L,            // Kolor tàa
     SYSCLR_ENTRYFIELD,          0xFFFFFF,
   },
   { PP_DISABLEDBACKGROUNDCOLOR, 0L,            // Kolor tàa zablokowanego okna
     SYSCLR_ENTRYFIELD,          0xFFFFFF,
   },
   { PP_FOREGROUNDCOLOR,         QPF_NOINHERIT, // Kolor tekstu
     SYSCLR_WINDOWTEXT,          0x000000,
   },
   { PP_DISABLEDFOREGROUNDCOLOR, QPF_NOINHERIT, // Kolor tekstu w zablokowanym oknie
     0,                          0x808080,
   }
  };

// Odczyt kolor¢w okna
PppQueryPresColors(hwndControl, CTL_MAXCOLOR, PresParams, coltab);
// Utworzenie logicznej tablicy kolor¢w
GpiCreateLogColorTable(hpsControl, 0L, LCOLF_CONSECRGB, 0L, CTL_MAXCOLOR, coltab);
:ecgraphic.





.* Funkcja PpmStorePresParams
.*
:h3 group=1 width=30% scroll=none name=PPMSTOREPRESPARAMS.PpmStorePresParams
:link dependent group=40 reftype=hd auto refid=PPMSTOREPRESPARAMS_S.
:link dependent group=1  reftype=hd auto refid=PPMSTOREPRESPARAMS.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=PPMSTOREPRESPARAMS_S.Skàadnia:elink.
.br
:link dependent reftype=hd refid=PPMSTOREPRESPARAMS_P.Parametry:elink.
.br
:link dependent reftype=hd refid=PPMSTOREPRESPARAMS_E.Rezultaty:elink.
.br
:link dependent reftype=hd refid=PPMSTOREPRESPARAMS_N.Opis:elink.
.br
:link dependent reftype=hd refid=PPMSTOREPRESPARAMS_C.Przykàadowy kod:elink.


.* SKùADNIA
.*
:h3 group=2 x=30% width=70% hide name=PPMSTOREPRESPARAMS_S.PpmStorePresParams - Skàadnia
:p.Funkcja zapisuje parametry prezentacji odczytane ze struktury PRESPARAMS w pami©ci okna.
:p.
:cgraphic.
#define  INCL_WINSYS
#include <os2.h>
#include "pparams.h"

HWND        :link dependent reftype=hd refid=PPMSTOREPRESPARAMS_HWND.hwnd:elink.;        // Uchwyt okna
PRESPARAMS :link dependent reftype=hd refid=PPMSTOREPRESPARAMS_PPM.*ppm:elink.;         // Lista parametr¢w prezentacji
:link dependent reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink. :link dependent reftype=hd refid=PPMSTOREPRESPARAMS_PPT.*ppt:elink.;         // Struktura steruj•ca
LONG        :link dependent reftype=hd refid=PPMSTOREPRESPARAMS_NCOLORS.nColors:elink.;     // Liczba zapisywanych parametr¢w
LONG        :link dependent reftype=hd refid=PPMSTOREPRESPARAMS_E.rc:elink.;          // Kod bà©du

rc PpmStorePresParams(HWND hwnd, PRESPARAMS *ppm,
                      PPTEMPLATE *ppt, LONG nColors);
:ecgraphic.



.* Parametr hwnd
.*
:h3 group=10 x=30% y=bottom width=70% height=225x hide name=PPMSTOREPRESPARAMS_HWND.PpmStorePresParams Parametr - hwnd
:dl tsize=3 break=all.
:dt.:hp2.hwnd:ehp2. (HWND) - parametr.
:dd.Uchwyt okna do kt¢rego s• zapisywane parametry.
:edl.

.* Parametr *ppm
.*
:h3 group=11 x=30% y=bottom width=70% height=225x hide name=PPMSTOREPRESPARAMS_PPM.PpmStorePresParams Parametr - *ppm
:dl tsize=3 break=all.
:dt.:hp2.*ppm:ehp2. (LONG) - parametr
:dd.Wska´nik do struktury PRESPARAMS, zawieraj•cej zapisywane parametry prezentacji
i ich wartoòci.
:edl.

.* Parametr *ppt
.*
:h3 group=12 x=30% y=bottom width=70% height=225x hide name=PPMSTOREPRESPARAMS_PPT.PpmStorePresParams Parametr - *ppt
:dl tsize=3 break=all.
:dt.:hp2.*ppt:ehp2. (:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.) - parametr
:dd.Wska´nik do tablicy zawieraj•cej list© parametr¢w prezentacji, kt¢re
naleæy skopiowaÜ z przekazanej struktury :hp1.*ppm:ehp1.
:p.Gdy wska´nik :hp1.*ppt:ehp1. jest r¢wny NULL, kopiowane s• wszystkie parametry
zapisane na liòcie :hp1.*ppm:ehp1..
:edl.

.* Parametr nColors
.*
:h3 group=13 x=30% y=bottom width=70% height=225x hide name=PPMSTOREPRESPARAMS_NCOLORS.PpmStorePresParams Parametr - *nColors
:dl tsize=3 break=all.
:dt.:hp2.nColors:ehp2. (LONG) - parametr
:dd.Rozmiar tablicy
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.
zawieraj•cej list© kolor¢w kopiowanych z tablicy :hp1.*ppm:ehp1..
:edl.

.* PARAMETRY
.*
:h3 group=3 x=30% width=70% hide name=PPMSTOREPRESPARAMS_P.PpmStorePresParams - Parametry
:dl tsize=3 break=all.
:dt.:hp2.hwnd:ehp2. (HWND) - parametr.
:dd.Uchwyt okna do kt¢rego s• zapisywane parametry.
:dt.:hp2.*ppm:ehp2. (LONG) - parametr
:dd.Wska´nik do struktury PRESPARAMS, zawieraj•cej zapisywane parametry prezentacji
i ich wartoòci.
:dt.:hp2.*ppt:ehp2. (:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.) - parametr
:dd.Wska´nik do tablicy zawieraj•cej list© parametr¢w prezentacji, kt¢re
naleæy skopiowaÜ z przekazanej struktury :hp1.*ppm:ehp1.
:p.Gdy wska´nik :hp1.*ppt:ehp1. jest r¢wny NULL, kopiowane s• wszystkie parametry
zapisane na liòcie :hp1.*ppm:ehp1..
:dt.:hp2.nColors:ehp2. (LONG) - parametr
:dd.Rozmiar tablicy
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.
zawieraj•cej list© kolor¢w kopiowanych z tablicy :hp1.*ppm:ehp1..
:edl.


.* Rezultaty
.*
:h3 group=4 x=30% y=bottom width=70% height=225x hide name=PPMSTOREPRESPARAMS_E.PpmStorePresParams - rezultaty
:dl tsize=3 break=all.
:dt.:hp2.rc:ehp2. (LONG)
:dd.WartoòÜ zarezerwowana, r¢wna 0.
:edl.


.* OPIS
.*
:h3 group=5 x=30% width=70% hide name=PPMSTOREPRESPARAMS_N.PpmStorePresParams - Opis
:p.Funkcja moæe byÜ uæyta podczas tworzenia okna (komunikat WM_CREATE)
w celu przepisania parametr¢w prezentacji przekazanych za poòrednictwem struktury
CREATESTRUCT (pole pPresParams) do pami©ci okna.
:p.Funkcja analizuje struktur© PRESPARAMS, odczytuje zawarte w niej parametry i za
pomoc• WinSetPresParam zapisuje je w pami©ci okna wedàug nast©puj•cego algorytmu&colon.

:ol.
:li.Jeòli odczytany parametr to PP_FONTNAMESIZE funkcja PpmStorePresParams
umieszcza go w pami©ci okna.

:li.Jeòli odczytany parametr to kolor, funkcja PpmStorePresParams sprawdza czy
wska´nik :hp1.*ppt:ehp1. jest r¢wny NULL. Jeòli tak to zapisuje parametr w pami©ci
okna. Jeòli nie to nast©puje przejòcie do punktu 3.

:li.Funkcja poszukuje parametru na liòcie przekazanej za poòrednictwem
wska´nika :hp1.*ppt:ehp1. (tablica struktur :link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.).
Parametr znaleziony na liòcie zostaje zapisany w pami©ci okna.
:eol.
:p.Pola&colon.
ulPresFlags, ulSysColor i ulRgbColor struktury
:link reftype=hd refid=PPTEMPLATE.PPTEMPLATE:elink.
s• przez funkcj© PpmStorePresParams ignorowane.


.* Przykàadowy kod
.*
:h3 group=6 x=30% width=70% hide name=PPMSTOREPRESPARAMS_C.PpmStorePresParams - Przykàadowy kod
:p.W przykàadzie, podczas obsàugi WM_CREATE, parametry prezentacji przekazane za
poòrednictwem struktury CREATESTRUCT zostaj• przepisane do pami©ci okna.
:p.
:cgraphic.
#define  INCL_WINWINDOWMGR
#define  INCL_WINSYS
#include <os2.h>
#include "pparams.h"

MPARAM  mp2;        // Drugi parametr przekazany procedurze okna


case WM_CREATE:
  { CREATESTRUCT *pcr;

    // Odczyt adresu struktury inicjuj•cej
    pcr = (CREATESTRUCT *)PVOIDFROMMP(mp2);
    // Zaàadowanie presentation parameters ze struktury CREATESTRUCT
    PpmStorePresParams(hwnd, (PRESPARAMS *)pcr->pPresParams, NULL, 0L);
  }
  break;
:ecgraphic.

