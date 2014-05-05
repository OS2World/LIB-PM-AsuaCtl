.* Dokumentacja techniczna biblioteki okien kontrolnych asuactl.dll
.* opis struktur danych og¢lnego przeznaczenia
.*

.* Rozdziaˆ 2 - Struktury danych og¢lnego przeznaczenia
.*
:h1 res=002.Struktury danych
:p.Rozdziaˆ zawiera opis struktur danych u¾ywanych przez okna kontrolne.


.* Rozdziaˆ 2.1 - Struktury og¢lnego przenaczenia
.*
:h2 res=021.Struktury og¢lnego przenaczenia
:p.Rozdziaˆ zawiera opis struktur danych dost©pnych dla u¾ytkownika,
z kt¢rych korzystaj¥ okna kontrolne biblioteki asuactl.dll.


.* Struktura OWNERBACK
.*
:h3 name=OWNERBACK.OWNERBACK
:p.Struktura u¾ywana jest podczas przetwarzania komunikatu WM_DRAWITEM,
i daje u¾ytkownikowi mo¾liwo˜† przerysowania tˆa kontrolki.
:cgraphic.
typedef struct
{ HWND    :link dependent reftype=hd refid=OBK_HWND.hwnd:elink.;     // Uchwyt przerysowywanego okna
  HPS     :link dependent reftype=hd refid=OBK_HPS.hps:elink.;      // Uchwyt presentation space okna
  RECTL   :link dependent reftype=hd refid=OBK_BOUND.bound:elink.;    // Wsp¢ˆrz©dne obszaru, kt¢ry nale¾y przerysowa†
  POINTL  :link dependent reftype=hd refid=OBK_REFPOINT.refpoint:elink.; // Przesuni©cie tˆa wzgl©dem przerysowywanego obszaru
} OWNERBACK;
:ecgraphic.

.* Pole hwnd
.*
:h3 group=20 x=30% y=bottom width=70% height=225x hide name=OBK_HWND.OWNERBACK pole - hwnd
:dl tsize=3 break=all.
:dt.:hp2.hwnd:ehp2. (HWND)
:dd.Uchwyt przerysowywanego okna.
:edl.

.* Pole hps
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=OBK_HPS.OWNERBACK pole - hps
:dl tsize=3 break=all.
:dt.:hp2.hps:ehp2. (HPS)
:dd.Uchwyt presentation space, kt¢rej nale¾y u¾y† do rysowania wewn¥trz okna.
:edl.

.* Pole bound
.*
:h3 group=22 x=30% y=bottom width=70% height=225x hide name=OBK_BOUND.OWNERBACK pole - bound
:dl tsize=3 break=all.
:dt.:hp2.bound:ehp2. (RECTL)
:dd.Struktura :hp1.bound:ehp1. zawiera wsp¢ˆrz©dne przerysowywanego obszaru
liczone wzgl©dem lewego dolnego rogu okna.
:edl.

.* Pole refpoint
.*
:h3 group=23 x=30% y=bottom width=70% height=225x hide name=OBK_REFPOINT.OWNERBACK pole - refpoint
:dl tsize=3 break=all.
:dt.:hp2.refpoint:ehp2. (POINTL)
:dd.Struktura :hp1.refpoint:ehp1. zawiera przesuni©cie, kt¢re trzeba doda† do wsp¢ˆrz©dnych
przekazanych w polu :hp1.bound:ehp1. - przesuni©ty prostok¥t okre˜la
obszar w obr©bie wy˜wietlanej mapy bitowej. Poˆo¾enie tego obszaru jest liczone wzgl©dem
lewego dolnego rogu mapy bitowej.
:p.Przesuni©ty obszar w obr©bie mapy mapy bitowej nale¾y narysowa† w oknie, wewn¥trz obszaru
okre˜lonego wsp¢ˆrz©dnymi przekazanymi w strukturze :hp1.bound:ehp1.. Dzi©ki
tej manimpulacji, zachowana zostaje ci¥gˆo˜† rysowanego tˆa podczas przewijania
zawarto˜ci okna.
:p.Rysowanie mo¾na przeprowadzi† za pomoc¥ funkcji GpiWCBitBlt.
:p.Okna kt¢re nie obsˆuguj¥ pola :hp1.refpoint:ehp1. powinny ustawi† obydwie wsp¢ˆrz©dne
r¢wne 0.
:edl.





.* Rozdziaˆ 2.2 - Wewn©trzne struktury danych
.*
:h2 res=022.Wewn©trzne struktury danych
:p.Rozdziaˆ zawiera opis struktur danych wykorzystywanych przez
moduˆ asuactl.dll, niedost©pnych dla u¾ytkownika. Programista dodaj¥cy
now¥ kontrolk© mo¾e jednak z nich korzysta†.



.* Struktura CTLREG
.*
:h3 name=CTLREG.CTLREG
:p.Struktura ta u¾ywana jest przez funkcj©
:link reftype=hd refid=ASUQUERYCTLINFO.AsuQueryCtlInfo:elink.,
przekazuj¥c¥ dane dotycz¥ce okna kontrolnego do moduˆu asuactl.c.
Obecnie zawiera ona pola daj¥ce mo¾liwo˜† odczytu wersji okna kontrolnego.

:cgraphic.
typedef struct
{ ULONG   :link dependent reftype=hd refid=CTL_CBSIZE.cbSize;:elink.        // Rozmiar
  PSZ     :link dependent reftype=hd refid=CTL_RCSVER.szRCSVersion;:elink.  // Numer wersji w ormacie RCS
  USHORT  :link dependent reftype=hd refid=CTL_VMAJOR.usVerMajor;:elink.    // Gˆ¢wny numer wersji
  USHORT  :link dependent reftype=hd refid=CTL_VMINOR.usVerMinor;:elink.    // Dodatkowy numer wersji
} CTLINFO;
:ecgraphic.

.* Pole cbSize
.*
:h3 group=20 x=30% y=bottom width=70% height=225x hide name=CTL_CBSIZE.CTLINFO pole - cbSize
:dl tsize=3 break=all.
:dt.:hp2.cbSize:ehp2. (ULONG)
:dd.Pole to powinno zawiera† liczb© bajt¢w obejmuj¥c¥ pola, kt¢rych warto˜†
ma zosta† odczytana.
.br
Zaleca si© u¾ycie nast©puj¥cego kodu:
:cgraphic.
ctl.cbSize = sizeof(CTLINFO);
:ecgraphic.
:edl.


.* Pole szRCSVersion
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=CTL_RCSVER.CTLINFO pole - szRCSVersion
:dl tsize=3 break=all.
:dt.:hp2.szRCSVersion:ehp2. (PSZ)
:dd.Pole to powinno zawiera† wska«nik do ˆaäcucha tekstowego, nast©puj¥cej postaci:
.br
:hp1."dowolny tekst x.y":ehp1., gdzie&colon. :hp2.x:ehp2. to gˆ¢wny numer wersji, a :hp2.y:ehp2.
to numer podwersji. Gdy korzystamy z RCS-a, mo¾e to wygl¥da† nast©puj¥co:
:cgraphic.
ctl.szRCSVersion = "$Revision 1.4 $";
:ecgraphic.
:p.Aby odczyt przebiegˆ poprawnie, pola :hp1.usVerMajor:ehp1. i :hp1.usVerMinor:ehp1.
struktury CTLINFO powinny by† r¢wne 0. W przeciwnym wypadku ich zawarto˜† zast¥pi
informacje zawarte w polu :hp1.szRCSVersion:ehp1..
:p.W podanym przykˆadzie, warto˜ci zwracane przez funkcj©
:link reftype=hd refid=ASUGETCOMPVER.AsuGetComponentVersion:elink.
b©d¥ nast©puj¥ce:
:dl tsize=25 break=none compact.
:dt.gˆ¢wny numer wersji:
:dd.1,
:dt.numer podwersji:
:dd.40,
:edl.
:p.co nale¾y czyta† jako wersj© 1.4.
:edl.



.* Pole usVerMajor
.*
:h3 group=22 x=30% y=bottom width=70% height=225x hide name=CTL_VMAJOR.CTLINFO pole - usVerMajor
:dl tsize=3 break=all.
:dt.:hp2.usVerMajor:ehp2. (USHORT)
:dd.Pole to powinno zawiera† gˆ¢wny numer wersji, np. 1, 2, itp.
:edl.


.* Pole ulVerMinor
.*
:h3 group=23 x=30% y=bottom width=70% height=225x hide name=CTL_VMINOR.CTLINFO pole - usVerMinor
:dl tsize=3 break=all.
:dt.:hp2.usVerMinor:ehp2. (USHORT)
:dd.Pole to powinno zawiera† numer podwersji. Je¾eli przykˆadowo, pole :hp1.usVerMajor:ehp1.
zawiera 2, to w zale¾no˜ci od zawart˜ci pola :hp1.usVerMinor:ehp1. peˆny numer wersji
zwracany przez
:link reftype=hd refid=ASUGETCOMPVER.AsuGetComponentVersion:elink.
powinien by† interpretowny w nast©puj¥cy spos¢b&colon.
:dl tsize=25 break=none compact.
:dthd.:hp2.usVerMinor:ehp2.
:ddhd.:hp2.Numer wersji:ehp2.
:dt.1
:dd.2.01
:dt.10
:dd.2.1
:dt.15
:dd.2.15
:edl.
:edl.






:h3 name=PPTEMPLATE.PPTEMPLATE
:p.Struktura u¾ywana jest przez funkcje
:link reftype=hd refid=PPMQUERYPRESCOLORS.PpmQueryPresColors:elink. i
:link reftype=hd refid=PPMSTOREPRESPARAMS.PpmStorePresParams:elink..
Zawiera ona list© parametr¢w prezentacji okna, na kt¢rych dokonywane
s¥ manipulacje.

:cgraphic.
typedef struct
{ ULONG :link dependent reftype=hd refid=PPT_ULPRESPARAM.ulPresParam:elink.;     // Identyfikator parametru prezentacji
  ULONG :link dependent reftype=hd refid=PPT_ULPRESFLAGS.ulPresFlags:elink.;     // Opcje QPF_* u¾ywane przez funkcj© WinQueryPresParam
  ULONG :link dependent reftype=hd refid=PPT_ULSYSCOLOR.ulSysColor:elink.;      // Indeks SYSCLR_* koloru systemowego
  ULONG :link dependent reftype=hd refid=PPT_ULRGBCOLOR.ulRgbColor:elink.;      // Domy˜lny Kolor przypisany parametrowi prezentacji
} PPTEMPLATE;
:ecgraphic.

.* Pole ulPresParam
.*
:h3 group=20 x=30% y=bottom width=70% height=225x hide name=PPT_ULPRESPARAM.PPTEMPLATE pole - ulPresParam
:dl tsize=3 break=all.
:dt.:hp2.ulPresParam:ehp2. (ULONG)
:dd.Pole to, zawiera identyfikator parametru przezentacji, kt¢ry ma by† uwzgl©dniony
przez funkcje - zapami©tany w pami©ci okna, lub z niej odczytany.
:edl.


.* Pole ulPresFlags
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=PPT_ULPRESFLAGS.PPTEMPLATE pole - ulPresFlags
:dl tsize=3 break=all.
:dt.:hp2.ulPresFlags:ehp2. (ULONG)
:dd.Pole to zawiera opcje przekazywane funkcji WinQueryPresParam, kt¢re pozwalaj¥
okre˜li†, czy na przykˆad parametr prezentacji ma by† dziedziczony. Dopuszczalne s¥
nast©puj¥ce opcje&colon.

:dl tsize=6 break=all.
:dt.QPF_NOINHERIT
:dd.Odczytywany parametr prezentacji nie jest dziedziczony po wˆa˜cicielu
okna korzystaj¥cego z funkcji
:link reftype=hd refid=PPMQUERYPRESCOLORS.PpmQueryPresColors:elink..
Gdy opcja nie jest u¾yta - parametr jest dziedziczony.
:dt.QPF_ID1COLORINDEX
:dd.Opcja powinna by† u¾yta wtedy, gdy w polu ulPresParam struktury PPTEMPLATE
zdefiniowano parametr prezentacji b©d¥cy indeksem odczytywanego koloru
(np. PP_BACKGROUNDCOLORINDEX zamiast PP_BACKGROUNDCOLOR).
:dt.QPF_ID2COLORINDEX
:dd.Opcja nie powinna by† u¾ywana.
:dt.QPF_PURERGBCOLOR
:dd.Opcja wymusza odczyt czystego koloru RGB, nie podlegaj¥cego ditheringowi.
:edl.
:edl.



.* Pole ulSysColor
.*
:h3 group=22 x=30% y=bottom width=70% height=225x hide name=PPT_ULSYSCOLOR.PPTEMPLATE pole - ulSysColor
:dl tsize=3 break=all.
:dt.:hp2.ulSysColor:ehp2. (ULONG)
:dd.Pole to jest wykorzystywane przez funkcj©
:link reftype=hd refid=PPMQUERYPRESCOLORS.PpmQueryPresColors:elink.
wtedy gdy odczyt parametru prezentacji nie powiedzie si©.
:p.Odczytywany jest w¢wczas kolor systemowy SYSCLR_* o indeksie zdefiniowanym
w polu :hp1.ulSysColor:ehp1..
:edl.


.* Pole ulRgbColor
.*
:h3 group=23 x=30% y=bottom width=70% height=225x hide name=PPT_ULRGBCOLOR.PPTEMPLATE pole - ulRgbColor
:dl tsize=3 break=all.
:dt.:hp2.ulRgbColor:ehp2. (ULONG)
:dd.Pole to jest wykorzystywane przez funkcj©
:link reftype=hd refid=PPMQUERYPRESCOLORS.PpmQueryPresColors:elink.
wtedy gdy pole :hp1.ulSysColor:ehp1. zawiera 0. W sytuacji takiej, funkcja
:link reftype=hd refid=PPMQUERYPRESCOLORS.PpmQueryPresColors:elink.
zwraca kolor RGB odczytany z :hp1.ulRgbColor:ehp1..
:edl.






.* Rozdziaˆ 2.3 - Zmienne globalne
.*
:h2 res=023.Zmienne globalne.
:p.Rozdziaˆ zawiera opis zmiennych globalnych udost©pnianych programi˜cie
pisz¥cemu okna kontrolne przez funkcje inicjuj¥ce bibliotek© dynamiczn¥.
:p.Wykorzystanie



.* Zmienna hResource
.*
:h3 name=HRESOURCE.hResource
:p.Jest to uchwyt bilioteki asuactl.dll, z kt¢rego mog¥ korzysta† kontrolki w celu
uzyskania dost©pu np. do zasob¢w doˆ¥czonych do DLL-a. Zmienna jest inicjowana
podczas ˆadowania biblioteki do pami©ci.
:p.Zmienna jest zdefiniowana w pliku *.h nast©puj¥co&colon.
:cgraphic.
extern HMODULE hResource;
:ecgraphic.



.* Zmienna hResource
.*
:h3 name=ULWARPVER.ulWarpVer
:p.Zmienna ta zawiera wersj© systemu operacyjnego. Jst ona inicjowana
podczas ˆadowania biblioteki do pami©ci.
:p.Zmienna jest zdefiniowana w pliku *.h nast©puj¥co&colon.
:cgraphic.
extern ULONG ulWarpVer;
:ecgraphic.
:p.W zale¾no˜ci od wersji systemu zawiera ona nast©puj¥ce warto˜ci&colon.
:table cols='10 20'.
:row.:c.ulWarpVer:c.Wersja systemu&colon.
:row.:c.00:c.OS/2 2.0
:row.:c.10:c.OS/2 2.1
:row.:c.11:c.OS/2 2.11
:row.:c.30:c.OS/2 3.0
:row.:c.40:c.OS/2 4.0
:etable.

