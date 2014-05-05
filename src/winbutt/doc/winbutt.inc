.* Opis przycisk¢w WC_WINBUTTON
.*
.* Asua Programmers Group
.* (c) 1999 Przemysàaw Dobrowolski
.*
.* $Revision: 1.1 $

.* Main
:h1 name=WC_WINBUTTON.WC_WINBUTTON - Alternatywny przycisk
:p.:hp2.Przeznaczenie:ehp2.
:p.Przycisk WC_WINBUTTON jest podobny w wygl•dzie do systemowego przycisku, reaguje na wszystkie
systemowe wiadomoòci plus inne wàaòciwoòci np. konwersja zablokowanej bitmapy na odcienie
szaroòci.

.* Style przycisku
:h2 name=WC_WINBUTTON_STYLE.Style przycisku
:p.Dost©pne s• nast©puj•ce style przycisku&colon.

:dl tsize=4 break=all.
:dt.BS_AUTOSIZE
:dd.Przyciski z tym stylem ustawiaj• sw¢j rozmiar automatycznie dostosowywuj•c
go do rozmiar¢w aktualnego tekstu i/lub bitmapy.
:p.Styl r¢æni si© od stylu WC_BUTTON poniewaæ przy kaædych parametrach przycisk
przystosowywuje automatycznie rozmiar!!!
:p.Aby wiedzieÜ wi©cej uæyj pomocy&colon. :hp2.Presentation Manager Programming Guide and Ref:ehp2.

:dt.BS_BITMAP
:dd.Styl ten uæywany jest przez przecisk wtedy kiedy dodawane s• bitmapy do wyòwietlenia
wraz lub bez tekstu.
:p.Styl r¢æni si© od stylu przycisk¢ klasy WC_BUTTON, aby przycisk wyòwietlaà
bitmap© naleæy tworzyÜ w nast©puj•cy spos¢b&colon.
:cgraphic."#<ID1>#<ID2>\t<tekst>":ecgraphic.
gdzie: :cgraphic.ID1:ecgraphic.  bitmapa dla aktywnego przycisku
:cgraphic.ID2:ecgraphic.  bitmapa dla zablokowanego przycisku
:cgraphic.tekst:ecgraphic.  tekst wyswietlany razem z bitmapami
:p.Bitmapa moæe byÜ usytuowana na kilka sposob¢w z tekstem

:dt.BS_THINFRAME
:dd.Przycisk posiada w•skie obram¢wki.

:dt.BS_FLATBUTTON
:dd.Styl ten moæe byÜ uæywany wsp¢lnie z BS_BITMAP, BS_AUTOSIZE, BS_DEFAULT,
BS_SYSCOMMAND, BS_FLATBUTTON, BS_HELP. Okreòla specjalny wygl•d przycisku&colon.
Kiedy nie ma kursora myszki w obr©bie przycisku bitmapa kt¢ra moæe si© tam
znajdowaÜ jest w odcieniach szaroòci, kiedy kursor wchodzi w obr©b przycisku
rysuje si© obram¢wka i bitmapa w orginalnych kolorach.
:p.Jeòli BS_FLATBUTTON jest uæywany bez stylu BS_THINFRAME to jego obram¢wka
jest taka sama jak w Netscape Communicatorze 4.xx, jeòli nie to wygl•da jak
przycisku bez tego stylu lecz z w•skimi obram¢wkami.

:dt.BS_DEFAULT
:dd.Przycisk z tym stylem posiada grubsz• obram¢wk© i jest on wtedy przyciskiem
domyòlnym. Kiedy wciòni©ty zostanie Enter to reaguje przycisk kt¢ry ma w tej
chwili styl BS_DEFAULT.
:p.Styl jest identyczny jak w przyciskach klasy WC_BUTTON
:p.Aby wiedzieÜ wi©cej uæyj pomocy&colon. :hp2.Presentation Manager Programming Guide and Ref:ehp2.

:dt.BS_SYSCOMMAND
:dd.Przycisk wysyàa do wàaòciciela wiadmoòci WM_SYSCOMMAND zamiast WM_COMMAND
:p.Styl jest identyczny jak w przyciskach klasy WC_BUTTON
:p.Aby wiedzieÜ wi©cej uæyj pomocy&colon. :hp2.Presentation Manager Programming Guide and Ref:ehp2.

:dt.BS_HELP
:dd.Przycisk wysyàa do wàaòciciela wiadmoòci WM_HELP zamiast WM_COMMAND
:p.Styl jest identyczny jak w przyciskach klasy WC_BUTTON
:p.Aby wiedzieÜ wi©cej uæyj pomocy&colon. :hp2.Presentation Manager Programming Guide and Ref:ehp2.
:edl.

:h2 name=WC_WINBUTTON_CTLDATA.Data kontrolna - CTLDATA
:p.Przyciskowi moæna podawaÜ r¢æne parametry poprzez CTLDATA.
:p.Aby wiedzieÜ wi©cej o skàadni CTLDATA i uæycia uæyj ksi•æki&colon. :hp2.Presentation Manager Programming Guide and Ref:ehp2.
:p.Parametry jakie s• przesyàane r¢æni• si© od parametr¢w WC_BUTTON, a wygl•daj• tak&colon.
:cgraphic.
#pragma pack(push, 1)
typedef struct _WBTNCDATA    /* btncd */
{
  USHORT  :link dependent reftype=hd refid=WC_WINBUTTON_CTLDATA_CB.cb:elink.;             // dàugoòÜ struktury
  USHORT  :link dependent reftype=hd refid=WC_WINBUTTON_CTLDATA_IDSTDBITMAP.idStdBitmap:elink.;    // numer identyfikacyjny bitmapy zaszytej w resourcach DLL'a
  USHORT  :link dependent reftype=hd refid=WC_WINBUTTON_CTLDATA_HILITESTATE.fsHiliteState:elink.;  // Stan przycisku
  LHANDLE :link dependent reftype=hd refid=WC_WINBUTTON_CTLDATA_HCOLOR.hColorImage:elink.;    // Uchwyt do bitmapy
  LHANDLE :link dependent reftype=hd refid=WC_WINBUTTON_CTLDATA_HDISCOLOR.hColorDisImage:elink.; // Uchwyt do bitmapy stylu zablokowanego
  LHANDLE hImage;         // ZAREZERWOWANE
  LHANDLE hImageDis;      // ZAREZERWOWANE
} WBTNCDATA;
#pragma pack(pop)
:ecgraphic.

:h2 group=10 x=0% y=bottom width=100% height=225x hide name=WC_WINBUTTON_CTLDATA_CB.CTLDATA - cb
:dl tsize=3 break=all.
:dt.:hp2.cb:ehp2. (USHORT)
:dd.DàugoòÜ przesyàaniej struktury
:edl.

:h2 group=10 x=0% y=bottom width=100% height=225x hide name=WC_WINBUTTON_CTLDATA_IDSTDBITMAP.CTLDATA - idStdBitmap
:dl tsize=3 break=all.
:dt.:hp2.idStdBitmap:ehp2. (USHORT)
:dd.Numer bitmapy zaszytej w zasobach biblioteki
:cgraphic.#define BBMP_OK     1:ecgraphic.  Bitmapa OK
:cgraphic.#define BBMP_CANCEL 2:ecgraphic.  Bitmapa Cancel
:cgraphic.#define BBMP_HELP   3:ecgraphic.  Bitmapa Help

:edl.

:h2 group=10 x=0% y=bottom width=100% height=225x hide name=WC_WINBUTTON_CTLDATA_HILITESTATE.CTLDATA - fsHiliteState
:dl tsize=3 break=all.
:dt.:hp2.fsHiliteState:ehp2. (USHORT)
:dd.Stan przycisku w jakim ma si© znajdowaÜ&colon.
:p.:hp2.TRUE:ehp2.  Stan wciòni©ty
:p.:hp2.FALSE:ehp2.  Stan domyòlny, odciòni©ty
:edl.

:h2 group=10 x=0% y=bottom width=100% height=225x hide name=WC_WINBUTTON_CTLDATA_HCOLOR.CTLDATA - hColorImage
:dl tsize=3 break=all.
:dt.:hp2.hColorImage:ehp2. (LHANDLE)
:dd.Uchwyt do mapy bitowej dla stanu niezablokowanego
:edl.

:h2 group=10 x=0% y=bottom width=100% height=225x hide name=WC_WINBUTTON_CTLDATA_HDISCOLOR.CTLDATA - hColorDisImage
:dl tsize=3 break=all.
:dt.:hp2.hColorDisImage:ehp2. (LHANDLE)
:dd.Uchwyt do mapy bitowej dla stanu zablokowanego
:edl.


:h2 name=WC_WINBUTTON_STDCOLOR.Standardowe kolory
:p.Nast©puj•ce kolory s• uæywane podczas rysowania przycisku&colon.
:p.W przypadku gdy parametry prezentacji nie s• zdefiniowane,
odpowiadaj•ce im kolory s• dziedziczone od wàaòciciela. Gdy wàaòciciel przycisku
stanu takæe ma niezdefiniowane parametry, to przyjmowane s• domyòlne kolory
systemowe, lub kolory zapisane w kodzie okna WC_WINBUTTON.
:ul.
:li.Kolor tàa odblokowanego okna
:table cols='30 30'.
:row.
:c.Parametr prezentacji
:c.PP_BACKGROUNDCOLOR
:row.
:c.Domyòlny kolor systemowy
:c.SYSCLR_BUTTONMIDDLE
:row.
:c.Uwagi
:c.brak
:etable.

:li.Kolor tàa zablokowanego okna
:table cols='30 30'.
:row.
:c.Parametr prezentacji
:c.PP_DISABLEDBACKGROUNDCOLOR
:row.
:c.Domyòlny kolor systemowy
:c.SYSCLR_BUTTONMIDDLE
:row.
:c.Uwagi
:c.brak
:etable.

:li.Kolor tekstu w odblokowanym oknie
:table cols='30 30'.
:row.
:c.Parametr prezentacji
:c.PP_FOREGROUNDCOLOR
:row.
:c.Domyòlny kolor systemowy
:c.SYSCLR_BUTTONMIDDLE
:row.
:c.Uwagi
:c.brak
:etable.

:li.Kolor tekstu w zablokowanym oknie
:table cols='30 30'.
:row.
:c.Parametr prezentacji
:c.PP_DISABLEDFOREGROUNDCOLOR
:row.
:c.Domyòlny kolor systemowy
:c.SYSCLR_BUTTONDARK
:row.
:c.Uwagi
:c.Kolor zablokowanego tekstu jest tworzony z dw¢ch kolor¢w  PP_DISABLEDFOREGROUNDCOLOR i PP_BORDERLIGHTCOLOR
:etable.

:li.Kolory oòwietlenia obram¢wki (efekt 3D)
:p.G¢rna i lewa strona
:table cols='30 30'.
:row.
:c.Parametr prezentacji
:c.PP_BORDERLIGHTCOLOR
:row.
:c.Domyòlny kolor systemowy
:c.SYSCLR_BUTTONLIGHT
:row.
:c.Uwagi
:c.Brak
:etable.

:p.Dolna i prawa strona (wewn©trzna)
:table cols='30 30'.
:row.
:c.Parametr prezentacji
:c.PP_BORDERDARKCOLOR
:row.
:c.Domyòlny kolor systemowy
:c.SYSCLR_BUTTONDARK
:row.
:c.Uwagi
:c.Brak
:etable.

:p.Dolna i prawa strona (zewn©trzna) + obram¢wka zaznaczonego przycisku
:table cols='30 30'.
:row.
:c.Parametr prezentacji
:c.PP_BORDERDEFAULTCOLOR
:row.
:c.Domyòlny kolor systemowy
:c.SYSCLR_BUTTONDEFAULT
:row.
:c.Uwagi
:c.Brak
:etable.

:li.Kolory tekstu dla stylu BS_FLATBUTTON kiedy kursor myszy jest na przycisku
:table cols='30 30'.
:row.
:c.Parametr prezentacji
:c.PP_USER
:row.
:c.Domyòlny kolor systemowy
:c.Jasno niebieski (0x0000FF)
:row.
:c.Uwagi
:c.Brak
:etable.

:eul.


.* Komunikaty potwierdze‰
.*
:h2 name=WC_WIBUTTON_NOTIFY.Komunikaty potwierdze‰
:p.Komunikaty te s• generowane przez okno stanu wtedy, gdy wàaòcicelowi
naleæy przekazaÜ jakieò informacje.

.* WM_CONTROL
.*
:h3 group=1 scroll=none width=30% name=BUTTON_WM_CONTROL.WM_CONTROL (w przycisku)
:link auto reftype=hd dependent refid=BUTTON_CONTROL_SYNTAX.
:link auto reftype=hd refid=BUTTON_WM_CONTROL.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=BUTTON_CONTROL_SYNTAX.Skàadnia:elink.
.br
:link dependent reftype=hd refid=BUTTON_CONTROL_PARAMS.Parametry:elink.
.br
:link dependent reftype=hd refid=BUTTON_CONTROL_RESULT.Rezultaty:elink.
.br
:link dependent reftype=hd refid=BUTTON_CONTROL_REMARK.Opis:elink.
.br
:link dependent reftype=hd refid=BUTTON_CONTROL_DFPROC.Standardowe wykonanie:elink.


.* Skàadnia
.*
:h3 group=2 x=30% width=70% hide name=BUTTON_CONTROL_SYNTAX.WM_CONTROL (w przycisku) - skàadnia
:p.Opis przyczyn zgàoszenia komunikatu moæna znale´Ü w
:hp2.Presentation Manager Guide and Reference":ehp2.
:p.:p.:cgraphic.
param1
     USHORT  :link dependent reftype=hd refid=BUTTON_CONTROL_ID.id:elink.             // Identyfikator okna stanu
     USHORT  :link dependent reftype=hd refid=BUTTON_CONTROL_NOTIFY.usnotifycode:elink.   // Kod potwierdzenia

param2
     ULONG   :link dependent reftype=hd refid=BUTTON_CONTROL_SPEC.flcontrolspec:elink.  // Informacje specyficzne dla kontrolki
:ecgraphic.

.* Identyfikator okna
.*
:h3 group=10 x=30% y=bottom width=70% height=225x hide name=BUTTON_CONTROL_ID.WM_CONTROL (w przycisku) Pole - id
:dl tsize=3 break=all.
:dt.:hp2.id:ehp2. (USHORT)
:dd.Identyfikator okna stanu.
:edl.

.* Kod potwierdzenia
.*
:h3 group=11 x=30% y=bottom width=70% height=225x hide name=BUTTON_CONTROL_NOTIFY.WM_CONTROL (w przycisku) Pole - usnotifycode
:dl tsize=3 break=all.
:dt.:hp2.usnotifycode:ehp2. (USHORT)
:dd.Kod potwierdzenia.
:p.Przycisk korzysta z nast©puj•cych kod¢w potwierdzenia&colon.
:dl tsize=7 break=all.
:dt.BN_CLICKED
:dd.Komunikat WM_CONTROL z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela kiedy zostaà przyciòni©ty lewy klawisz myszy na przycisku.
:dt.BN_BUTTON2CLICKED
:dd.Komunikat WM_CONTROL z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela kiedy zostaà przyciòni©ty prawy klawisz myszy na przycisku.
:dt.BN_DBLCLICKED
:dd.Komunikat WM_CONTROL z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela kiedy odbyà si© "dwumlask" myszy na przycisku.
:dt.BN_MOUSEENTER
:dd.Komunikat WM_CONTROL z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela kiedy kursor myszy wchodzi w obr©b przycisku
:dt.BN_MOUSELEAVE
:dd.Komunikat WM_CONTROL z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela gdy kursor myszy wychodzi poza obr©b przycisku
:dt.BN_PRESPARAMCHANGED
:dd.Nast•piàa zmiana jednego z parametr¢w prezentacji okna.
:p.Komunikat WM_CONTROL z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela w nast©puj•cych sytuacjach&colon.
:ul compact.
:li.po wrzuceniu koloru z palety systemowej,
:li.po wrzuceniu czcionki z palety systemowej,
:li.gdy inny w•tek, lub proces zmienia za pomoc• funkcji WinSetPresParams jeden
z nast©puj•cych parametr¢w: PP_BACKGROUNDCOLOR, PP_FOREGROUNDCOLOR
lub PP_FONTNAMESIZE.
:eul.
:p.:hp2.flcontrolspec:ehp2. zawiera numer parametru prezentacji,
kt¢ry ulegà zmianie.
:edl.
:edl.

.* Informacje specyficzne dla kontrolki
.*
:h3 group=12 x=30% y=bottom width=70% height=225x hide name=BUTTON_CONTROL_SPEC.WM_CONTROL (w przycisku) Pole - flcontrolspec
:dl tsize=3 break=all.
:dt.:hp2.flcontrolspec:ehp2. (LONG)
:dd.Informacja specyficzna dla kontrolki.
:p.Zawiera kod parametru prezentacji, zmienionego w wyniku drag&amp.drop z jednej
z palet systemowych, lub uæycia WinSetPresParams w innym w•tku.
:edl.


.* Parametry
.*
:h3 group=3 x=30% width=70% hide name=BUTTON_CONTROL_PARAMS.WM_CONTROL (w przycisku) - parametry
:dl tsize=3 break=all.
:dt.:hp2.id:ehp2. (USHORT)
:dd.Identyfikator okna stanu.
:dt.:hp2.usnotifycode:ehp2. (USHORT)
:dd.Kod potwierdzenia.
:p.Okno stanu korzysta z nast©puj•cych kod¢w potwierdzenia&colon.
:dl tsize=7 break=all.
:dt.BN_CLICKED
:dd.Komunikat WM_CONTROL z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela kiedy zostaà przyciòni©ty lewy klawisz myszy na przycisku.
:dt.BN_DBLCLICKED
:dd.Komunikat WM_CONTROL z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela kiedy odbyà si© "dwumlask" myszy na przycisku.
:dt.BN_MOUSEENTER
:dd.Komunikat WM_CONTROL z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela kiedy kursor myszy wchodzi w obr©b przycisku
:dt.BN_MOUSELEAVE
:dd.Komunikat WM_CONTROL z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela gdy kursor myszy wychodzi poza obr©b przycisku
:dt.BN_PRESPARAMCHANGED
:dd.Nast•piàa zmiana jednego z parametr¢w prezentacji okna.
:p.Komunikat WM_CONTROL z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela w nast©puj•cych sytuacjach&colon.
:ul compact.
:li.po wrzuceniu koloru z palety systemowej,
:li.po wrzuceniu czcionki z palety systemowej,
:li.gdy inny w•tek, lub proces zmienia za pomoc• funkcji WinSetPresParams jeden
z nast©puj•cych parametr¢w: PP_BACKGROUNDCOLOR, PP_FOREGROUNDCOLOR
lub PP_FONTNAMESIZE.
:eul.
:p.:hp2.flcontrolspec:ehp2. zawiera numer parametru prezentacji,
kt¢ry ulegà zmianie.
:edl.
:dt.:hp2.flcontrolspec:ehp2. (LONG)
:dd.Informacja specyficzna dla kontrolki.
:p.Zawiera kod parametru prezentacji, zmienionego w wyniku drag&amp.drop z jednej
z palet systemowych, lub uæycia WinSetPresParams w innym w•tku.
:edl.


.* Rezultaty
.*
:h3 group=4 x=30% y=bottom width=70% height=225x hide name=BUTTON_CONTROL_RESULT.WM_CONTROL (w oknie stanu) - rezultaty
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (LONG)
:dd.WartoòÜ zarezerwowana, r¢wna 0.
:edl.


.* Opis
.*
:h3 group=5 x=30% width=70% hide name=BUTTON_CONTROL_REMARK.WM_CONTROL (w przycisku) - opis
:p.Przycisk wstawia ten komunikat do kolejki wàaòciciela w nast©puj•cych sytuacjach
:ul.
:li.Przycisk zostaà przyciòni©ty.
:li.Przycisk zostaà dwa razy przyciòni©ty ("dwumlask").
:li.Kursor myszy wszedà na obszar przycisku.
:li.Kursor myszy wyszedà z obszaru przycisku.
:li.Nast•piàa zmiana koloru lub czcionki w wyniku drag&amp.drop z systemowej palety czcionek i kolor¢w.
:eul.

.* Standardowe wykonanie
.*
:h3 group=6 x=30% width=70% hide name=BUTTON_CONTROL_DFPROC.WM_CONTROL (w przycisku) - standardowe wykonanie
:p.Standardowa procedura okna ignoruje komunikat, ustawiaj•c :hp1.ulReserved:ehp1. na 0.

.*
.* Komunikat WM_COMMAND
.*
:h3 group=1 scroll=none width=30% name=BUTTON_WM_COMMAND.WM_COMMAND (w przycisku)
:link auto reftype=hd dependent refid=BUTTON_COMMAND_SYNTAX.
:link auto reftype=hd refid=BUTTON_WM_COMMAND.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=BUTTON_COMMAND_SYNTAX.Skàadnia:elink.
.br
:link dependent reftype=hd refid=BUTTON_COMMAND_PARAMS.Parametry:elink.
.br
:link dependent reftype=hd refid=BUTTON_COMMAND_RESULT.Rezultaty:elink.
.br
:link dependent reftype=hd refid=BUTTON_COMMAND_REMARK.Opis:elink.
.br
:link dependent reftype=hd refid=BUTTON_COMMAND_DFPROC.Standardowe wykonanie:elink.


.* Skàadnia
.*
:h3 group=2 x=30% width=70% hide name=BUTTON_COMMAND_SYNTAX.WM_COMMAND (w przycisku) - skàadnia
:p.Zgàoszenia komunikatu opisane jest w :hp2.Presentation Manager Guide and Reference:ehp2.
Patrz na WM_COMMAND(in Button Controls)
:cgraphic.
param1
     USHORT uscmd;      // Numer komendy

param2
     USHORT ussource;   // çr¢dào wysàania komunikatu do kolejki
                        // Dla WC_WINBUTTON - CMDSRC_OTHER
     USHORT uspointer;  // Czy zostaào to zasygnalizowane mysz• czy klawiatur•
:ecgraphic.

.* Identyfikator okna
.*
:h3 group=10 x=30% y=bottom width=70% height=225x hide name=BUTTON_COMMAND_ID.WM_COMMAND (w przycisku) Pole - id
:dl tsize=3 break=all.
:dt.:hp2.id:ehp2. (USHORT)
:dd.Identyfikator okna stanu.
:edl.

.* Kod potwierdzenia
.*
:h3 group=11 x=30% y=bottom width=70% height=225x hide name=BUTTON_COMMAND_NOTIFY.WM_COMMAND (w przycisku) Pole - usnotifycode
:dl tsize=3 break=all.
:dt.:hp2.usnotifycode:ehp2. (USHORT)
:dd.Kod potwierdzenia.
:p.Przycisk korzysta z nast©puj•cych kod¢w potwierdzenia&colon.
:dl tsize=7 break=all.
:dt.BN_CLICKED
:dd.Komunikat WM_COMMAND z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela kiedy zostaà przyciòni©ty lewy klawisz myszy na przycisku.
:dt.BN_DBLCLICKED
:dd.Komunikat WM_COMMAND z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela kiedy odbyà si© "dwumlask" myszy na przycisku.
:dt.BN_MOUSEENTER
:dd.Komunikat WM_COMMAND z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela kiedy kursor myszy wchodzi w obr©b przycisku
:dt.BN_MOUSELEAVE
:dd.Komunikat WM_COMMAND z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela gdy kursor myszy wychodzi poza obr©b przycisku
:dt.BN_PRESPARAMCHANGED
:dd.Nast•piàa zmiana jednego z parametr¢w prezentacji okna.
:p.Komunikat WM_COMMAND z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela w nast©puj•cych sytuacjach&colon.
:ul compact.
:li.po wrzuceniu koloru z palety systemowej,
:li.po wrzuceniu czcionki z palety systemowej,
:li.gdy inny w•tek, lub proces zmienia za pomoc• funkcji WinSetPresParams jeden
z nast©puj•cych parametr¢w: PP_BACKGROUNDCOLOR, PP_FOREGROUNDCOLOR
lub PP_FONTNAMESIZE.
:eul.
:p.:hp2.flcontrolspec:ehp2. zawiera numer parametru prezentacji,
kt¢ry ulegà zmianie.
:edl.
:edl.

.* Informacje specyficzne dla kontrolki
.*
:h3 group=12 x=30% y=bottom width=70% height=225x hide name=BUTTON_COMMAND_SPEC.WM_COMMAND (w przycisku) Pole - flcontrolspec
:dl tsize=3 break=all.
:dt.:hp2.flcontrolspec:ehp2. (LONG)
:dd.Informacja specyficzna dla kontrolki.
:p.Zawiera kod parametru prezentacji, zmienionego w wyniku drag&amp.drop z jednej
z palet systemowych, lub uæycia WinSetPresParams w innym w•tku.
:edl.


.* Parametry
.*
:h3 group=3 x=30% width=70% hide name=BUTTON_COMMAND_PARAMS.WM_COMMAND (w przycisku) - parametry
:dl tsize=3 break=all.
:dt.:hp2.id:ehp2. (USHORT)
:dd.Identyfikator okna stanu.
:dt.:hp2.usnotifycode:ehp2. (USHORT)
:dd.Kod potwierdzenia.
:p.Okno stanu korzysta z nast©puj•cych kod¢w potwierdzenia&colon.
:dl tsize=7 break=all.
:dt.BN_CLICKED
:dd.Komunikat WM_COMMAND z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela kiedy zostaà przyciòni©ty lewy klawisz myszy na przycisku.
:dt.BN_DBLCLICKED
:dd.Komunikat WM_COMMAND z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela kiedy odbyà si© "dwumlask" myszy na przycisku.
:dt.BN_MOUSEENTER
:dd.Komunikat WM_COMMAND z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela kiedy kursor myszy wchodzi w obr©b przycisku
:dt.BN_MOUSELEAVE
:dd.Komunikat WM_COMMAND z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela gdy kursor myszy wychodzi poza obr©b przycisku
:dt.BN_PRESPARAMCHANGED
:dd.Nast•piàa zmiana jednego z parametr¢w prezentacji okna.
:p.Komunikat WM_COMMAND z tym kodem potwierdzenia, jest wstawiany do kolejki
komunikat¢w wàaòciciela w nast©puj•cych sytuacjach&colon.
:ul compact.
:li.po wrzuceniu koloru z palety systemowej,
:li.po wrzuceniu czcionki z palety systemowej,
:li.gdy inny w•tek, lub proces zmienia za pomoc• funkcji WinSetPresParams jeden
z nast©puj•cych parametr¢w: PP_BACKGROUNDCOLOR, PP_FOREGROUNDCOLOR
lub PP_FONTNAMESIZE.
:eul.
:p.:hp2.flcontrolspec:ehp2. zawiera numer parametru prezentacji,
kt¢ry ulegà zmianie.
:edl.
:dt.:hp2.flcontrolspec:ehp2. (LONG)
:dd.Informacja specyficzna dla kontrolki.
:p.Zawiera kod parametru prezentacji, zmienionego w wyniku drag&amp.drop z jednej
z palet systemowych, lub uæycia WinSetPresParams w innym w•tku.
:edl.


.* Rezultaty
.*
:h3 group=4 x=30% y=bottom width=70% height=225x hide name=BUTTON_COMMAND_RESULT.WM_COMMAND (w oknie stanu) - rezultaty
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (LONG)
:dd.WartoòÜ zarezerwowana, r¢wna 0.
:edl.


.* Opis
.*
:h3 group=5 x=30% width=70% hide name=BUTTON_COMMAND_REMARK.WM_COMMAND (w przycisku) - opis
:p.Przycisk wstawia ten komunikat do kolejki wàaòciciela w nast©puj•cych sytuacjach
:ul.
:li.Przycisk zostaà przyciòni©ty.
:li.Przycisk zostaà dwa razy przyciòni©ty ("dwumlask").
:li.Kursor myszy wszedà na obszar przycisku.
:li.Kursor myszy wyszedà z obszaru przycisku.
:li.Nast•piàa zmiana koloru lub czcionki w wyniku drag&amp.drop z systemowej palety czcionek i kolor¢w.
:eul.

.* Standardowe wykonanie
.*
:h3 group=6 x=30% width=70% hide name=BUTTON_COMMAND_DFPROC.WM_COMMAND (w przycisku) - standardowe wykonanie
:p.Standardowa procedura okna ignoruje komunikat, ustawiaj•c :hp1.ulReserved:ehp1. na 0.



.* Komunikat WM_DRAWITEM
.*
:h3 group=1 scroll=none width=30% name=WINBUTT_WM_DRAWITEM.WM_DRAWITEM (w oknie stanu)
:link auto reftype=hd dependent refid=WINBUTT_DRAWITEM_SYNTAX.
:link auto reftype=hd refid=WINBUTT_WM_DRAWITEM.
:p.Wybierz temat&colon.
:p.
:link dependent reftype=hd refid=WINBUTT_DRAWITEM_SYNTAX.Skàadnia:elink.
.br
:link dependent reftype=hd refid=WINBUTT_DRAWITEM_PARAMS.Parametry:elink.
.br
:link dependent reftype=hd refid=WINBUTT_DRAWITEM_RESULT.Rezultaty:elink.
.br
:link dependent reftype=hd refid=WINBUTT_DRAWITEM_REMARK.Opis:elink.
.br
:link dependent reftype=hd refid=WINBUTT_DRAWITEM_DFPROC.Standardowe wykonanie:elink.


.* Skàadnia
.*
:h2 group=2 x=30% width=70% hide name=WINBUTT_DRAWITEM_SYNTAX.WM_DRAWITEM (w oknie stanu) - skàadnia
:p.Potwierdzenie to, jest wysyàane do wàaòciciela za kaædym razem, gdy trzeba przerysowaÜ
tào okna stanu.
:p.:p.:cgraphic.
param1
     USHORT     :link dependent reftype=hd refid=WINBUTT_DRAWITEM_ID.idStatus:elink.       // Identyfikator okna stanu

param2
     POWNERBACK :link dependent reftype=hd refid=WINBUTT_DRAWITEM_BACK.pownerback:elink.     // Struktura tàa uæytkownika
:ecgraphic.

.* Identyfikator okna
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=WINBUTT_DRAWITEM_ID.WM_DRAWITEM (w oknie stanu) Pole - idStatus
:dl tsize=3 break=all.
:dt.:hp2.idStatus:ehp2. (USHORT)
:dd.Identyfikator okna stanu.
:edl.

.* Struktura tàa uæytkownika
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=WINBUTT_DRAWITEM_BACK.WM_DRAWITEM (w oknie stanu) Pole - pownerback
:dl tsize=3 break=all.
:dt.:hp2.pownerback:ehp2. (POWNERBACK)
:dd.Struktura umoæliwiaj•cea rysowanie tàa.
:p.Parametr wskazuje na struktur© zawieraj•c• dane umoæliwiaj•ce wàaòcicielowi
okna narysowanie tàa - zobacz&colon.
:link dependent reftype=hd refid=OWNERBACK.OWNERBACK:elink..
:edl.


.* Parametry
.*
:h2 group=3 x=30% width=70% hide name=WINBUTT_DRAWITEM_PARAMS.WM_DRAWITEM (w oknie stanu) - parametry
:dl tsize=3 break=all.
:dt.:hp2.idStatus:ehp2. (USHORT)
:dd.Identyfikator okna stanu.
:dt.:hp2.pownerback:ehp2. (POWNERBACK)
:dd.Struktura umoæliwiaj•cea rysowanie tàa.
:p.Parametr wskazuje na struktur© zawieraj•c• dane umoæliwiaj•ce wàaòcicielowi
okna narysowanie tàa - zobacz&colon.
:link dependent reftype=hd refid=OWNERBACK.OWNERBACK:elink..
:edl.


.* Rezultaty
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=WINBUTT_DRAWITEM_RESULT.WM_DRAWITEM (w oknie stanu) - rezultaty
:dl tsize=3 break=all.
:dt.:hp2.rc:ehp2. (BOOL)
:dd.Znacznik przerysowania tàa.
:dl tsize=6 break=all.
:dt.TRUE
:dd.Wàaòciciel narysowaà tào, w zwi•zku z czym okno stanu nie rysuje go.
:dt.FALSE
:dd.Wàaòciciel nie przerysowaà tàa, w zwi•zku z czym okno stanu rysuje go.
:edl.
:edl.


.* Opis
.*
:h2 group=5 x=30% width=70% hide name=WINBUTT_DRAWITEM_REMARK.WM_DRAWITEM (w oknie stanu) - opis
:p.Okno stanu utworzone z uæyciem stylu SS_OWNERDRAW wysyàa ten komunikat do wàaòciciela
okna, daj•c mu okazj© do narysowania tàa. Komunikat jest wysyàany po zako‰czeniu rysowania
ramek, ale przed wyòwietleniem tekstu.
:p.Wàaòciciel powinien zwruciÜ TRUE gdy narysowaà tào, oraz FALSE gdy go nie narysowaà -
w tej sytuacji okno stanu samodzielnie narysuje tào.
:p.Struktura
:link dependent reftype=hd refid=OWNERBACK.OWNERBACK:elink. zawiera wszystkie informacje
potrzebne do przerysowania tàa, takie jak&colon.
:ul compact.
:li.uchwyt okna stanu,
:li.uchwyt presentation space okna,
:li.wsp¢àrz©dne prostok•ta, kt¢ry naleæy odòwieæyÜ.
:eul.


.* Standardowe wykonanie
.*
:h2 group=6 x=30% width=70% hide name=WINBUTT_DRAWITEM_DFPROC.WM_DRAWITEM (w oknie stanu) - standardowe wykonanie
:p.Standardowa procedura okna ignoruje komunikat, ustawiaj•c :hp1.rc:ehp1. r¢wny FALSE,
co powoduje, æe okno stanu samodzielnie rysuje tào.






