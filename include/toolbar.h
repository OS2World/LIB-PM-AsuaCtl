/* Asuactl.dll
 * Pasek narz©dzi gˆ¢wny plik nagˆ¢wkowy
 *
 * (c) 1999 Wojciech Gazda
 *
 * toolbar.h
 *
 * $Author: Wojciech_Gazda $
 * $Date: 1999/06/27 12:36:07 $
 * $Name:  $
 * $RCSfile: toolbar.h $
 * $Revision: 1.1 $
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


// Definicja klasy okna
#define WC_TOOLBAR       "WC_TOOLBAR"   // Pasek stanu

// Identyfikatory okien potomnych
#define TCID_TITLEBAR    0xFFFF         // Pasek tytuˆu
#define TCID_TOOLTIP     0xFFFE         // Okno podpowiedzi
#define TCID_ROTATE      0xFFFC         // Przycisk obracania okna
#define TCID_OBJECTPAD   0xFFF9         // Okno przechowuj¥ce obiekty


// Definicje komunikat¢w
// Przed plikiem toolbar.h mo¾na zdefiniowa† wˆasny indeks pierwszego komuniaktu
// tak, aby unikn¥† konfliktu z innymi komunikatami. Nowy indeks musi by† liczony
// wzgl©dem WM_USER (na przykˆad WM_USER + 5)
#ifndef TOOL_FIRST_MSG
#define TOOL_FIRST_MSG  (WM_USER + 60)
#endif

// Numer ostatniego komunikatu
#define TOOL_LAST_MSG       (TOOL_FIRST_MSG + 19)

// Komunikaty steruj¥ce oknem WC_TOOLBAR
// Pozycjonowanie paska narz©dzi
#define TM_AUTOSIZE         (TOOL_FIRST_MSG + 0)  // ½¥danie automatycznego przeskalowania
#define TM_CALCNEWRECT      (TOOL_FIRST_MSG + 1)  // Obliczenie proponowanej pozycji i wymiar¢w paska narz©dzi
#define TM_ENABLEROTATE     (TOOL_FIRST_MSG + 2)  // Sterowanie obecno˜ci¥ przycisku obracania
#define TM_QUERYHEIGHT      (TOOL_FIRST_MSG + 3)  // Odczyt wysoko˜ci paska narz©dzi
#define TM_QUERYWIDTH       (TOOL_FIRST_MSG + 4)  // Odczyt szeroko˜ci paska narz©dzi
#define TM_QUERYSTATE       (TOOL_FIRST_MSG + 5)  // Odczyt stanu paska - staˆe TST_*
#define TM_ROTATE           (TOOL_FIRST_MSG + 6)  // ½¥danie obr¢cenia paska narz©dzi

// Kontrola nad obiektami
#define TM_DELETEOBJECT     (TOOL_FIRST_MSG + 7)  // Usuni©cie obiektu z paska narz©dzi
#define TM_IDFROMPOSITION   (TOOL_FIRST_MSG + 8)  // Odczyt identyfikatora na podstawie pozycji
#define TM_INSERTOBJECT     (TOOL_FIRST_MSG + 9)  // Dodanie obiektu do paska narz©dzi
#define TM_INSERTTOOLTIP    (TOOL_FIRST_MSG + 10) // Dodanie/zmiana podpowiedzi do obiektu
#define TM_MOVEOBJECT       (TOOL_FIRST_MSG + 11) // Przesuni©cie obiektu na pasku narz©dzi
#define TM_OBJECTCONTROL    (TOOL_FIRST_MSG + 12) // Pomocniczy komunikat steruj¥cy
#define TM_POSITIONFROMID   (TOOL_FIRST_MSG + 13) // Odczyt pozycji na podstawie identyfikatora
#define TM_QUERYHANDLE      (TOOL_FIRST_MSG + 14) // Odczyt uchwytu obiektu
#define TM_QUERYOBJECT      (TOOL_FIRST_MSG + 15) // Odczyt struktury informacyjnej obiektu
#define TM_QUERYOBJECTCOUNT (TOOL_FIRST_MSG + 16) // Odczyt liczby obiekt¢w zawartych na pasku
#define TM_QUERYTIP         (TOOL_FIRST_MSG + 17) // Odczyt tekstu podpowiedzi lub jego dˆugo˜ci
#define TM_SETOBJECT        (TOOL_FIRST_MSG + 18) // Zmiana zawarto˜ci struktury informacyjnej obiektu
#define TM_SHOWOBJECT       (TOOL_FIRST_MSG + 19) // Ukrycie lub sy˜wietlenie obiektu



// Opcje dla komunikat¢w TM_QUERYHEIGHT i TM_QUERYWIDTH
#define TDQ_RESTORED          0x0000    // Wymiary paska w stanie floating
#define TDQ_MINIMIZED         0x0001    // Wymiary zminimalizowanego paska narz©dzi
#define TDQ_FIXED             0x0002    // Wymiary zmaksymalizowanego paska narz©dzi
#define TDQ_CURRENT           0x0003    // Aktualne wymiary paska narz©dzi

// Opcje steruj¥ce obracaniem, komunikat: TM_ROTATE
#define TBR_ROTATE            0         // Zmiana stanu na przeciwny
#define TBR_HORIZONTAL        1         // Przej˜cie do pozycji pionowej
#define TBR_VERTICAL          2         // Przej˜cie do pozycji pionowej

// Opcje steruj¥ce aktualizacj¥ parametr¢w obiektu: TM_SETOBJECT
#define TSO_GROUP             0x0001    // Zmiana grupy do kt¢rej nale¾y obiekt
#define TSO_SIZE              0x0002    // Zmiana wymiar¢w obiektu
#define TSO_USER              0x0004    // Zmiana pola u¾ytkownika





// Kody potwierdzeä (usnotifycode) przesyˆne komunikatem WM_CONTROL
#define TN_AUTOSIZE           1    // Korekta pozycji podczas automatycznego skalowania
#define TN_BEGINTRACK         2    // Rozpocz©to przesuwanie okna
#define TN_TRACKING           3    // Trwa przesuwanie okna
#define TN_ENDTRACK           4    // Zakoäczono przesuwanie okna
#define TN_FIXTOOL            5    // Korekta pozycji podczas dokowania
#define TN_MINIMIZE           6    // Korekta pozycji podczas minimalizacji
#define TN_RESTORE            7    // Korekta pozycji podczas odtworzania stanu
#define TN_ROTATE             8    // Korekta pozycji podczas obracania okna



// Definicje parametr¢w prezentacji
#ifndef TOOL_FIRST_PP
#define TOOL_FIRST_PP    (PP_USER + 0)
#endif

// Kolor tˆa podpowiedzi
#define PP_TOOLTIPBACKGROUND  (TOOL_FIRST_PP + 0)
// Kolor tekstu podpowiedzi
#define PP_TOOLTIPTEXT        (TOOL_FIRST_PP + 1)
// Kolor obramowania podpowiedzi
#define PP_TOOLTIPFRAME       (TOOL_FIRST_PP + 2)
// Czcionka u¾ywana do wy˜wietlania podpowiedzi
#define PP_TOOLTIPFONT        (TOOL_FIRST_PP + 3)
// Czas op¢«nienia przed wy˜wietleniem podpowiedzi (w milisekundach)
#define PP_TOOLTIPDELAY       (TOOL_FIRST_PP + 4)




// Style okna WC_TOOLBAR
// Pozycjonowanie obiekt¢w
#define TBS_STANDARD          0x0000    // Obiekty s¥ obcinane gdy nie mieszcz¥ si©
#define TBS_SCROLLABLE        0x0001    // Pojawiaj¥ si© przyciski do przewijania zawarto˜ci
#define TBS_MULTILINE         0x0002    // Wystaj¥ce obiekty s¥ rozmieszczane w kolejnych liniach
#define TBS_CUTTYPES          0x0007    // Maska umo¾liwiaj¥ca wyci©cie r¢¾nych typ¢w pozycjonowania

// Dodatkowe przyciski steruj¥ce paskiem narz©dzi
#define TBS_MOVEWITHOWNER     0x0800    // Pasek w stanie floating przesuwa si© wraz z wˆa˜cicelem
#define TBS_ENABLEMINIMIZE    0x1000    // Odblokowanie minimalizacji za pomoc¥ myszy
#define TBS_MOVEBUTTON        0x2000    // Przycisk umo¾liwiaj¥cy przesuwanie paska mysz¥
#define TBS_FIXBUTTON         0x4000    // Przycisk umo¾liwiaj¥cy "przyklejanie" paska do okna
#define TBS_ROTATEBUTTON      0x8000    // Przycisk umo¾liwiaj¥cy obracanie paska

// Stany paska narz©dzi dost©pne dla u¾ytkownika
// Pozycjonowanie:
#define TST_FIXED             0x0001    // Pasek jest "przyklejony" do okna rodzicielskiego
#define TST_MINIMIZED         0x0002    // Pasek jest zminimalizowany
#define TST_ROTATED           0x0004    // Pasek jest w pozycji pionowej


// Struktury danych i zwi¥zane z nimi staˆe
#ifdef __WATCOMC__
#pragma pack(push)
#else
#pragma pack()
#endif

// Atrybuty steruj¥ce wygl¥dem paska, przekazywane za po˜rednictwem
// danych Class-Specific (pole flToolAttrs).
// Rysowanie obramowania:
#define TBA_FRAME             0x000F    // Kompletne, cieniowane obramowanie
#define TBA_LEFTFRAME         0x0001    // O˜wietlenie z lewej strony
#define TBA_BOTTOMFRAME       0x0002    // Cieniowanie od doˆu
#define TBA_RIGHTFRAME        0x0004    // Cieniowanie z prawej
#define TBA_TOPFRAME          0x0008    // O˜wietlenie od g¢ry

// Miejsce dokowania paska narz©dzi w oknie nadrz©dnym
#define TBA_FIXEDTOP          0x0000    // Poziomy pasek dokuje przy g¢rnej kraw©dzi okna
#define TBA_FIXEDBOTTOM       0x0010    // Poziomy pasek dokuje przy dolnej kraw©dzi okna
#define TBA_FIXEDLEFT         0x0000    // Pionowy pasek dokuje przy lewej kraw©dzi okna
#define TBA_FIXEDRIGHT        0x0020    // Pionowy pasek dokuje przy prawej kraw©dzi okna

// Wst©pna orientacja paska
#define TBA_HORIZONTAL        0x0000    // Poziomy pasek narz©dzi (warto˜† domy˜lna)
#define TBA_VERTICAL          0x0040    // Pionowy pasek narz©dzi



// Dane "Class Specific" paska narz©dzi
#pragma pack(2)
typedef struct _TOOLINFO
{ USHORT  cb;                 // Rozmiar struktury w bajtach
  USHORT  flToolAttrs;        // Atrybuty okna
  USHORT  usMaxLength;        // Maksymalna dˆugo˜† paska narz©dzi
} TOOLINFO;





// Struktura przekazywana przez komunikat WM_CONTROL
// podczas skalowania paska za pomoc¥ TM_AUTOSIZE, oraz zmiany stanu
// (dokowania, minimalizacji, przej˜cia do stanu "floating").
#pragma pack(4)
typedef struct _TOOLADJ
{ HWND   hwndToolBar;         // Uchwyt paska narz©dzi
  HWND   hwndParent;          // Uchwyt proponowanego okna rodzicielskiego
  RECTL  rclSizePos;          // Proponowana pozycja i rozmiary paska
  ULONG  flAttrs;             // Proponowane atrybuty paska
  ULONG  flState;             // Poprzedni stan paska narz©dzi (w przypadku TN_AUTOSIZE - aktualny)
} TOOLADJ;



// Struktura przekazywana przez komunikat WM_CONTROL
// w momencie rozpocz©cia przesuwania paska narz©dzi (TN_BEGINTRACK)
typedef struct _TOOLTRACK
{ HWND   hwndToolBar;         // Uchwyt paska narz©dzi
  HWND   hwndParent;          // Uchwyt okna rodzicielskiego
  RECTL  rclSizePos;          // Rozmiar i pozycja paska narz©dzi wzgl©dem okna rodzicielskiego
  RECTL  rclBoundary;         // Obszar ruchu (wewn¥trz okna rodzicielskiego)
  ULONG  flAttrs;             // Atrybuty paska
  ULONG  flState;             // Aktualny stan paska narz©dzi
} TOOLTRACK;





// Atrybuty steruj¥ce zachowaniem obiektu umieszczonego na pasku narz©dzi
// struktura TOOLOBJ, pole flAttrs.
// Podstawowe typy obiekt¢w:
#define TBO_STDWINDOW         0x0000    // Obiekt jest standardowym oknem kontrolnym
#define TBO_BUTTON            0x0001    // Okno ma by† traktowane jak przycisk WC_WINBUTTON
#define TBO_STDBUTTON         0x0002    // Pasek wspomaga sterowanie systemowymi przyciskami klasy WC_BUTTON
#define TBO_SEPARATOR         0x0003    // Obiekt jest separatorem (to nie jest okno!)
#define TBO_BASICTYPES        0x000F    // Maska bitowa umo¾liwiaj¥ca wyci©cie typu z atrybut¢w

// Pozycjonowanie obiektu:
#define TBO_BEGINALIGN        0x0000    // Obiekt ma by† pozycjonowany przy pocz¥tku paska lewo/g¢ra
#define TBO_ENDALIGN          0x0010    // Obiekt ma by† pozycjonowany przy koäcu paska prawo/d¢ˆ
#define TBO_CENTERALIGN       0x0020    // Obiekt ma by† umieszczony po ˜rodku
#define TBO_JUSTTYPES         0x0030    // Maska bitowa umo¾ˆiwiaj¥ca wyci©cie typu justowania

// Wˆa˜ciwo˜ci grupy, do kt¢rej nale¾y przycisk:
#define TBO_PUSHBUTTON        0x0040    // Przyciski w grupie dziaˆaj¥ jak BS_PUSHBUTTON
#define TBO_CHECKBOX          0x0080    // Przyciski w grupie dziaˆaj¥ jak BS_CHECKBOX
#define TBO_RADIOBUTTON       0x00C0    // Przyciski w grupie dziaˆaj¥ jak BS_RADIOBUTTON

// Og¢lny stan obiektu
#define TBO_HIDE              0x0100    // Obiekt jest ukryty
#define TBO_DISABLE           0x0200    // Obiekt jest zablokowany

// Style separator¢w (fragment pola flAttrs),
// wszystkie typy separator¢w maj¥ szeroko˜† 5 pikseli:
#define SPS_FLAT              0x0000    // Gˆadki obszar w kolorze tˆa
#define SPS_LINE              0x2000    // Linia w kolorze PP_BORDERDARKCOLOR
#define SPS_CONCAVE           0x4000    // Wkl©sˆa linia 3D
#define SPS_CONVEX            0x6000    // Wypukˆa linia 3D
#define SPS_SEPARATORS        0xE000    // Maska umo¾liwiaj¥ca wydzielenie stylu separat¢r¢w

// Pozycja umo¾liwiaj¥ca dodanie obiektu na koäcu paska
#define TB_END            0xFFFFFFFF



// Struktura informacyjna obiektu, stosowana podczas dodawania/usuwania
// i sterowania obiektem umiszczeonym na pasku narz©dzi
typedef struct _TOOLOBJ
{ ULONG   iPosition;          // Poˆo¾enie obiektu na pasku narz©dzi (numer kolejny)
  ULONG   ulGroup;            // Grupa do kt¢rej nale¾y obiekt (wa¾ne tylko dla przycisk¢w)
  ULONG   flAttribute;        // Dodatkowe atrybuty obiektu
  ULONG   cx, cy;             // Pocz¥tkowe wymiary obiektu (nie dotyczy przycisk¢w TBO_BUTTON)
  ULONG   id;                 // Identyfikator obiektu
  ULONG   ulUser;             // Pole do wykorzystania przez u¾ytkownika
} TOOLOBJ;



// Struktura zawieraj¥ca dodatkowe informacje u¾ywane przez WinCreateWindow podczas
// tworzenia obiektu:
typedef struct _TOOLWIN
{ PSZ     pszClass;           // Klasa tworzonego okna
  PSZ     pszName;            // Tekst zapisany w oknie (window text)
  ULONG   flStyle;            // Styl tworzonego okna
  PVOID   pCtlData;           // Wska«nik do danych class-specific
  PVOID   pPresParams;        // Parametry prezentacji okna
} TOOLWIN;

// Odtworzenie pierwotnego wyr¢wnania struktur
#ifdef __WATCOMC__
#pragma pack(pop)
#else
#pragma pack()
#endif


// Prototypy funkcji
LONG ASUAAPI CtlToolbarInitialize(HAB hab);

/*
 * $Log: toolbar.h $
 * Revision 1.1  1999/06/27 12:36:07  Wojciech_Gazda
 * Initial revision
 *
 */
