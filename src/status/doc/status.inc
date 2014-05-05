.* Control windows library - Asu'a Custom Controls
.* Status window (WC_STATUS) - documentation.
.* (c) 1999 Wojciech Gazda
.* Translation to english by Bartosz Tomasik
.*
.* status/doc/status.inc
.*
.* $Author: gazda $
.* $Date: 2001/04/01 23:33:27 $
.* $Name:  $
.* $RCSfile: status.inc,v $
.* $Revision: 1.2 $
.*
.* Asu'a Programmers Group http://www.asua.org.pl
.*


:h1 name=WC_STATUS.WC_STATUS - Status window
:p.:hp2.Overview:ehp2.
:p.Status window looks similar to entry field, but it behaves like a
static window (WC_STATIC). It is used to show small text hints that can be
formatted in several ways. Example of status window usuage is a "bubble" help
for other windows.


:h2 name=WC_STATUS_STYLE.Window styles
:p.Following window styles are available for status window&colon.

:p.:hp2.Border drawing control&colon.:ehp2.
:dl tsize=4 break=all.
:dt.SS_BORDER
:dd.Standard double border is drawn around window, the same as entry field
control (WC_ENTRYFIELD) has. It is also possible to draw incomplete border,
consisting any part of frame. If you want to select which part of border
is to be drawn, exhange SS_BORDER style with desired combination of the
following values&colon.
:dl compact tsize=18 break=none.
:dt.SS_LEFTFRAME
:dd.draw left window border
:dt.SS_RIGHTFRAME
:dd.draw right window border
:dt.SS_TOPFRAME
:dd.draw top window border
:dt.SS_BOTOMFRAME
:dd.draw bottom window border
:edl.
:dt.SS_THINFRAME
:dd.If used along with SS_BORDER style, causes border to be drawn as a thin
black line (instead of standard 3D border). If used without SS_BORDER style,
gives no results (window is drawn without any border).
:edl.

:p.:p.:hp2.Text alignment options (horizontal direction)&colon.:ehp2.
:dl tsize=4 break=all.
:dt.SS_LEFT
:dd.Aligns text to the left edge of the window. Text is clipped from the right
if it doesn't fit into the window.
:dt.SS_CENTER
:dd.Centers text within the window. Text is clipped both from left and right
sides if it doesn't fit into the window.
:dt.SS_RIGHT
:dd.Aligns text to the right edge of the window. Text is clipped from the right if
doesn't fit into the window, thus initial part of text is always visible.
:dt.SS_AUTOSHIFT
:dd.If text does fit into the window it is left-aligned. If it is longer and
doesn't fit it is right-aligned and clipped from the left side, thus
final part of the text is always visible.
:edl.
:p.Vertically, text is always centered.

:p.:p.:hp2.Automatic window scaling&colon.:ehp2.
:dl tsize=4 break=all.
:dt.SS_XAUTOSCALE
:dd.Window automatically adjusts it's width to fit the text.
:dt.SS_YAUTOSCALE
:dd.Window atomatically adjusts it's height to fit the text.
:edl.
:p.Automatic scaling causes window to change size in selected direction if
one of the following cases occur:
:ul compact.
:li.after font change caused by WinSetPresParam function.
:li.after dropping a font from the system font palette.
:li.after window's text change caused by WinSetWindowText function.
:eul.
:p.When window is scaled in horizontal direction right window edge is moving.
When window is scaled in vertical direction top window edge is moving.
Position of the left bottom corner does not change.
:p.During window scaling caused by font dropped from system font palette, window
generates WM_CONTROL message with SMN_PRESPARAMCHANGED code. Window owner
is than able to adjust size and positions of suurounding controls
according to new size of the status window.

:p.:p.:hp2.Additional options&colon.:ehp2.
:dl tsize=4 break=all.
:dt.SS_OWNERDRAW
:dd.Window created with this style sends WM_DRAWITEM message to it's owner,
allowing him to redraw it's background. Text is always painted by status
window itself.
:dt.SS_MOUSETRANSPARENT
:dd.Window created with this style becames transparent for all mouse and
drag'n'drop related messages. All of those are being send to the owner.
:dt.SS_WINSTYLE
:dd.Window draws a 'window scale button' (set of highlighted 3D bars like
in Windows 9x) in its right-bottom corner.
:edl.


:h2 name=WC_STATUS_STDCOLOR.Standard colors
:p.Following colors are used during status window drawing&colon.
:p.In case if presentation parameters has not been definied, they are inherited
from window's owner. If owner window also has not those definied, standard system
colors are used. If system colors are not available, defaults hard coded in the
status window are used.
:ul.
:li.Enabled window background color.
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_BACKGROUNDCOLOR
:row.
:c.Default system color
:c.SYSCLR_ENTRYFIELD
:row.
:c.Notes
:c.none
:etable.

:li.Disabled window background color.
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_DISABLEDBACKGROUNDCOLOR
:row.
:c.Default system color
:c.SYSCLR_ENTRYFIELD
:row.
:c.Notes
:c.none
:etable.

:li.Enabled window text color.
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_FOREGROUNDCOLOR
:row.
:c.Default system color
:c.SYSCLR_WINDOWTEXT
:row.
:c.Notes
:c.none
:etable.

:li.Disabled window text color.
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_DISABLEDFOREGROUNDCOLOR
:row.
:c.Default system color
:c.Dark gray (0x808080)
:row.
:c.Notes
:c.Default color is hard-coded within status window's source code.
:etable.

:li.Outer border highlight color (3D effect).
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_BORDERLIGHTCOLOR
:row.
:c.Default system color
:c.White (0xFFFFFF)
:row.
:c.Notes
:c.Default color is hard-coded within status window's source code.
:etable.

:li.Outer border shadow color (3D effect).
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_BORDERDARKCOLOR
:row.
:c.Default system color
:c.Dark gray (0x808080)
:row.
:c.Notes
:c.Default color is hard-coded within status window's source code.
:etable.

:li.Inner border highlight color (3D effect).
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_BORDER2LIGHTCOLOR
:row.
:c.Default system color
:c.Light gray (0xCCCCCC)
:row.
:c.Notes
:c.Default color is hard-coded within status window's source code.
:etable.

:li.Inner border shadow color (3D effect) and SS_THINFRAME style's border.
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_BORDER2DARKCOLOR
:row.
:c.Default system color
:c.Black (0x000000)
:row.
:c.Notes
:c.Default color is hard-coded within status window's source code.
:etable.
:eul.



.* Notification Messages
.*
:h2 name=WC_STATUS_NOTIFY.Notification Messages
:p.These messages are initiated by the button control window to notify its
owner of significant events.



.* WM_CONTROL
.*
:h2 group=1 scroll=none width=30% name=STATUS_WM_CONTROL.WM_CONTROL (in Status Windows)
:link auto reftype=hd dependent refid=STATUS_CONTROL_SYNTAX.
:link auto reftype=hd refid=STATUS_WM_CONTROL.
:p.Select topic&colon.
:p.
:link dependent reftype=hd refid=STATUS_CONTROL_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=STATUS_CONTROL_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=STATUS_CONTROL_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=STATUS_CONTROL_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=STATUS_CONTROL_DFPROC.Default processing:elink.


.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=STATUS_CONTROL_SYNTAX.WM_CONTROL (in Status Windows) - syntax
:p.For the cause of this message, see WM_COMMAND in the
"Presentation Manager Guide and Reference" book.
:p.:p.:font facename=Courier size=10x10.:cgraphic.
param1
     USHORT  :link dependent reftype=hd refid=STATUS_CONTROL_ID.id:elink.             // Status window identifier
     USHORT  :link dependent reftype=hd refid=STATUS_CONTROL_NOTIFY.usnotifycode:elink.   // Notification code

param2
     ULONG   :link dependent reftype=hd refid=STATUS_CONTROL_SPEC.flcontrolspec:elink.  // Control specific data
:ecgraphic.:font.

.* window identifier
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=STATUS_CONTROL_ID.WM_CONTROL (in Status Windows) Field - id
:dl tsize=3 break=all.
:dt.:hp2.id:ehp2. (USHORT)
:dd.Status window identifier.
:edl.

.* notification code
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=STATUS_CONTROL_NOTIFY.WM_CONTROL (in Status Windows) Field - usnotifycode
:dl tsize=3 break=all.
:dt.:hp2.usnotifycode:ehp2. (USHORT)
:dd.Notification code
:p.The status window uses following notification codes&colon.
:dl tsize=7 break=all.
:dt.SMN_PRESPARAMCHANGED
:dd.One of window's presentation parameters has changed.
:p.WM_CONTROL message with this notification code is sent to the owner in case
of&colon.
:ul compact.
:li.after dropping a color from a color palette,
:li.after dropping a font from a font palette,
:li.after another thread or process, changes one of the following parameters:
PP_BACKGROUNDCOLOR, PP_FOREGROUNDCOLOR, PP_FONTNAMESIZE using WinSetPresParams function.

:eul.
:p.:hp2.flcontrolspec:ehp2. contains code of changed presentation parameter.
:edl.
:edl.

.* control specific data
.*
:h2 group=12 x=30% y=bottom width=70% height=225x hide name=STATUS_CONTROL_SPEC.WM_CONTROL (in Status Windows) Field - flcontrolspec
:dl tsize=3 break=all.
:dt.:hp2.flcontrolspec:ehp2. (LONG)
:dd.Control specific data.
:p.Contains code of presentation parameter changed as a result of dropping an
object from system palletes (font, color) or WinSetPresParams called
in an another thread or process.
:edl.


.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=STATUS_CONTROL_PARAMS.WM_CONTROL (in Status Windows) - parameters
:dl tsize=3 break=all.
:dt.:hp2.id:ehp2. (USHORT)
:dd.Status window identifier.
:dt.:hp2.usnotifycode:ehp2. (USHORT)
:dd.Notification code.
:p.The status window uses following notification codes&colon.
:dl tsize=7 break=all.
:dt.SMN_PRESPARAMCHANGED
:dd.One of window's presentation parameters has changed.
:p.WM_CONTROL message with this notification code is sent to the owner in case
of&colon.
:ul compact.
:li.after dropping a color from a color palette,
:li.after dropping a font from a font palette,
:li.after another thread or process changes one of following parameters:
PP_BACKGROUNDCOLOR, PP_FOREGROUNDCOLOR, PP_FONTNAMESIZE using WinSetPresParams function.

:eul.
:p.:hp2.flcontrolspec:ehp2. contains code of changed presentation parameter.
:edl.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=STATUS_CONTROL_RESULT.WM_CONTROL (in Status Windows) - results
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (LONG)
:dd.Reserved value, equal 0.
:edl.


.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=STATUS_CONTROL_REMARK.WM_CONTROL (in Status Windows) - remarks
:p.The status window posts this message to the owner's message queue. Message
informs owner about color or font change caused by drag and drop operation involwing
font or color palette. It gives possibility to save new colors and to
immediate adjust colors and size of window being owner of the control.
:p.Message is posted to owner's message queue in the following cases:
:ul compact.
:li.after dropping a color from a color palette,
:li.after dropping a font from a font palette,
:li.after another thread or process changes one of following parameters:
PP_BACKGROUNDCOLOR, PP_FOREGROUNDCOLOR, PP_FONTNAMESIZE using WinSetPresParams or
WinRemovePresParam function.
:eul.
:p.Message is not send if WinSetPresParam or WinRemovePresParam functions are
called by a status window's owner thread.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=STATUS_CONTROL_DFPROC.WM_CONTROL (in Status Windows) - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.ulReserved:ehp1. to 0.



.* Komunikat WM_DRAWITEM
.*
:h2 group=1 scroll=none width=30% name=STATUS_WM_DRAWITEM.WM_DRAWITEM (in Status Windows)
:link auto reftype=hd dependent refid=STATUS_DRAWITEM_SYNTAX.
:link auto reftype=hd refid=STATUS_WM_DRAWITEM.
:p.Select topic&colon.
:p.
:link dependent reftype=hd refid=STATUS_DRAWITEM_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=STATUS_DRAWITEM_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=STATUS_DRAWITEM_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=STATUS_DRAWITEM_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=STATUS_DRAWITEM_DFPROC.Default processing:elink.


.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=STATUS_DRAWITEM_SYNTAX.WM_DRAWITEM (in Status Windows) - syntax
:p.This notification is sent to the owner of the status window each time background
should be drawn.

:p.:p.:font facename=Courier size=10x10.:cgraphic.
param1
     USHORT     :link dependent reftype=hd refid=STATUS_DRAWITEM_ID.idStatus:elink.       // Status window identifier

param2
     POWNERBACK :link dependent reftype=hd refid=STATUS_DRAWITEM_BACK.pownerback:elink.     // Background owner-draw structure
:ecgraphic.:font.

.* status window identifier
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=STATUS_DRAWITEM_ID.WM_DRAWITEM (in Status Windows) Field - idStatus
:dl tsize=3 break=all.
:dt.:hp2.idStatus:ehp2. (USHORT)
:dd.Status window identifier.
:edl.

.* user background structure
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=STATUS_DRAWITEM_BACK.WM_DRAWITEM (in Status Windows) Field - pownerback
:dl tsize=3 break=all.
:dt.:hp2.pownerback:ehp2. (POWNERBACK)
:dd.Structure allowing user to redraw window's background.
:p.Parameter points to a structure that allows owner redrawing status window background
- see&colon.
:link dependent reftype=hd refid=OWNERBACK.OWNERBACK:elink..
:edl.


.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=STATUS_DRAWITEM_PARAMS.WM_DRAWITEM (in Status Windows) - parameters
:dl tsize=3 break=all.
:dt.:hp2.idStatus:ehp2. (USHORT)
:dd.Status window identifier.
:dt.:hp2.pownerback:ehp2. (POWNERBACK)
:dd.Structure allowing user to redraw window's background.
:p.Parameter points to a structure that allows owner redrawing status window background
- see&colon.
:link dependent reftype=hd refid=OWNERBACK.OWNERBACK:elink..
:edl.


.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=STATUS_DRAWITEM_RESULT.WM_DRAWITEM (in Status Windows) - results
:dl tsize=3 break=all.
:dt.:hp2.rc:ehp2. (BOOL)
:dd.Background redraw indicator.
:dl tsize=6 break=all.
:dt.TRUE
:dd.The owner has drawn the background, thus status window does not draw it.
:dt.FALSE
:dd.The owner hasn't drawn the background, thus status the window does draw it.
:edl.
:edl.



.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=STATUS_DRAWITEM_REMARK.WM_DRAWITEM (in Status Windows) - remarks
:p.
:p.The status window created with SS_OWNERDRAW style set, generates this message and sends it to
its owner, giving him a chance to redraw status window's background. Message is send after
border painting is completed, but before text painting begins.
:p.Owner should return TRUE if he had handled message, or FALSE if not - status window will draw it's
background by itself.
:p.
:link dependent reftype=hd refid=OWNERBACK.OWNERBACK:elink. structure contains
all required data needed to redrawing the window&colon.
:ul compact.
:li.status window handle
:li.window's presentation space handle,
:li.coordinates of rectangle to be redrawn.
:eul.


.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=STATUS_DRAWITEM_DFPROC.WM_DRAWITEM (in Status Windows) - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.rc:ehp1. to FALSE (status window draw it's background by itself).



:h2 name=WC_STATUS_CTRL.Status window's window messages
:p.This section describes the window procedure actions upon receiving the
following messages.


.* query character maximal width
.*
:h2 group=1 scroll=none width=30% name=SM_QUERYMAXCHARINC.SM_QUERYMAXCHARINC
:link auto reftype=hd dependent refid=SM_QUERYMAXCHARINC_SYNTAX.
:link auto reftype=hd refid=SM_QUERYMAXCHARINC.
:p.Select topic&colon.
:p.
:link dependent reftype=hd refid=SM_QUERYMAXCHARINC_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=SM_QUERYMAXCHARINC_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=SM_QUERYMAXCHARINC_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=SM_QUERYMAXCHARINC_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=SM_QUERYMAXCHARINC_DFPROC.Default processing:elink.


.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=SM_QUERYMAXCHARINC_SYNTAX.SM_QUERYMAXCHARINC - syntax
:p.This message returns maximal width of characters used to paint the text.
:p.:p.:font facename=Courier size=10x10.:cgraphic.
param1
     ULONG  :link dependent reftype=hd refid=SM_QUERYMAXCHARINC_RES.ulReserved:elink.  // Reserved value, equal to 0.

param2
     ULONG  :link dependent reftype=hd refid=SM_QUERYMAXCHARINC_RES.ulReserved:elink.  // Reserved value, equal to 0.
:ecgraphic.:font.

.* Reserved value, equal to 0.
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=SM_QUERYMAXCHARINC_RES.SM_QUERYMAXCHARINC - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=SM_QUERYMAXCHARINC_PARAMS.SM_QUERYMAXCHARINC - parameters
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.


.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=SM_QUERYMAXCHARINC_RESULT.SM_QUERYMAXCHARINC - results
:dl tsize=3 break=all.
:dt.:hp2.lMaxCharInc:ehp2. (LONG)
:dd.Maximal width of characters used to paint the text within the window.
:p.This value is equal to lMaxCharInc field of FONTMETRICS structure.
:edl.


.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=SM_QUERYMAXCHARINC_REMARK.SM_QUERYMAXCHARINC - remarks
:p.Status window responds to this message returning maximal width of
character of a font used to paint the text. Value is provided in pixels.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=SM_QUERYMAXCHARINC_DFPROC.SM_QUERYMAXCHARINC - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.lMaxCharInc:ehp1. to default value of 0.




.* Get character's maximal height
.*
:h2 group=1 scroll=none width=30% name=SM_QUERYTEXTHEIGHT.SM_QUERYTEXTHEIGHT
:link auto reftype=hd dependent refid=SM_QUERYTEXTHEIGHT_SYNTAX.
:link auto reftype=hd refid=SM_QUERYTEXTHEIGHT.
:p.Select topic&colon.
:p.
:link dependent reftype=hd refid=SM_QUERYTEXTHEIGHT_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=SM_QUERYTEXTHEIGHT_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=SM_QUERYTEXTHEIGHT_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=SM_QUERYTEXTHEIGHT_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=SM_QUERYTEXTHEIGHT_DFPROC.Default processing:elink.


.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=SM_QUERYTEXTHEIGHT_SYNTAX.SM_QUERYTEXTHEIGHT - syntax
:p.This message returns maximal height of characters used to paint the text.
:p.:p.:font facename=Courier size=10x10.:cgraphic.
param1
     ULONG  :link dependent reftype=hd refid=SM_QUERYTEXTHEIGHT_RES.ulReserved:elink.  // Reserved value, equal to 0.

param2
     ULONG  :link dependent reftype=hd refid=SM_QUERYTEXTHEIGHT_RES.ulReserved:elink.  // Reserved value, equal to 0.
:ecgraphic.:font.

.* Reserved value, equal to 0.
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=SM_QUERYTEXTHEIGHT_RES.SM_QUERYTEXTHEIGHT - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=SM_QUERYTEXTHEIGHT_PARAMS.SM_QUERYTEXTHEIGHT - parameters
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.


.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=SM_QUERYTEXTHEIGHT_RESULT.SM_QUERYTEXTHEIGHT - results
:dl tsize=3 break=all.
:dt.:hp2.lMaxBaselineExt:ehp2. (LONG)
:dd.Maximal height of characters used to paint the text within the window.
:p.This value is equal to lMaxBaselineExt field of FONTMETRICS structure.
:edl.


.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=SM_QUERYTEXTHEIGHT_REMARK.SM_QUERYTEXTHEIGHT - remarks
:p.Status window responds to this message by returning maximal height of
character of font used to paint the text. Value is provided in pixels.
:p.Returned value includes top and bottom margins (so the text doesn't mix with a window's border).


.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=SM_QUERYTEXTHEIGHT_DFPROC.SM_QUERYTEXTHEIGHT - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.lMaxCharInc:ehp1. to default value of 0.




.* Query text width
.*
:h2 group=1 scroll=none width=30% name=SM_QUERYTEXTWIDTH.SM_QUERYTEXTWIDTH
:link auto reftype=hd dependent refid=SM_QUERYTEXTWIDTH_SYNTAX.
:link auto reftype=hd refid=SM_QUERYTEXTWIDTH.
:p.Select topic&colon.
:p.
:link dependent reftype=hd refid=SM_QUERYTEXTWIDTH_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=SM_QUERYTEXTWIDTH_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=SM_QUERYTEXTWIDTH_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=SM_QUERYTEXTWIDTH_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=SM_QUERYTEXTWIDTH_DFPROC.Default processing:elink.


.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=SM_QUERYTEXTWIDTH_SYNTAX.SM_QUERYTEXTWIDTH - syntax
:p.This message returns text width in pixels.
:p.:p.:font facename=Courier size=10x10.:cgraphic.
param1
     ULONG  :link dependent reftype=hd refid=SM_QUERYTEXTWIDTH_RES.ulReserved:elink.  // Reserved value, equal to 0.

param2
     ULONG  :link dependent reftype=hd refid=SM_QUERYTEXTWIDTH_RES.ulReserved:elink.  // Reserved value, equal to 0.
:ecgraphic.:font.

.* Reserved value, equal to 0.
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=SM_QUERYTEXTWIDTH_RES.SM_QUERYTEXTWIDTH - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=SM_QUERYTEXTWIDTH_PARAMS.SM_QUERYTEXTWIDTH - parameters
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.


.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=SM_QUERYTEXTWIDTH_RESULT.SM_QUERYTEXTWIDTH - results
:dl tsize=3 break=all.
:dt.:hp2.lTextWidth:ehp2. (LONG)
:dd.Width of the text to be painted within a window.
:edl.


.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=SM_QUERYTEXTWIDTH_REMARK.SM_QUERYTEXTWIDTH - remarks
:p.Status window responds to this message by returning text width (in pixels).
Based on this value, application is able to for instance set window width that will
allow to show whole text.


.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=SM_QUERYTEXTWIDTH_DFPROC.SM_QUERYTEXTWIDTH - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.lTextWidth:ehp1. to default value of 0.




.* query total window height
.*
:h2 group=1 scroll=none width=30% name=SM_QUERYHEIGHT.SM_QUERYHEIGHT
:link auto reftype=hd dependent refid=SM_QUERYHEIGHT_SYNTAX.
:link auto reftype=hd refid=SM_QUERYHEIGHT.
:p.Select topic&colon.
:p.
:link dependent reftype=hd refid=SM_QUERYHEIGHT_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=SM_QUERYHEIGHT_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=SM_QUERYHEIGHT_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=SM_QUERYHEIGHT_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=SM_QUERYHEIGHT_DFPROC.Default processing:elink.


.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=SM_QUERYHEIGHT_SYNTAX.SM_QUERYHEIGHT - syntax
:p.This message returns total window height in pixels.
:p.:p.:font facename=Courier size=10x10.:cgraphic.
param1
     ULONG  :link dependent reftype=hd refid=SM_QUERYHEIGHT_RES.ulReserved:elink.  // Reserved value, equal to 0.

param2
     ULONG  :link dependent reftype=hd refid=SM_QUERYHEIGHT_RES.ulReserved:elink.  // Reserved value, equal to 0.
:ecgraphic.:font.

.* Reserved value, equal to 0.
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=SM_QUERYHEIGHT_RES.SM_QUERYHEIGHT - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=SM_QUERYHEIGHT_PARAMS.SM_QUERYHEIGHT - parameters
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.


.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=SM_QUERYHEIGHT_RESULT.SM_QUERYHEIGHT - results
:dl tsize=3 break=all.
:dt.:hp2.lHeight:ehp2. (LONG)
:dd.Total window's height.
:edl.


.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=SM_QUERYHEIGHT_REMARK.SM_QUERYHEIGHT - remarks
:p.Status window responds to this message by returning window's height
necessary to fit the text and the border. It is equal to value returned by
SM_QUERYTEXTHEIGHT with addition of top and bottom border's width.
:p.Returned value does not depend on current window size set by WinSetWindowPos.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=SM_QUERYHEIGHT_DFPROC.SM_QUERYHEIGHT - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.lHeight:ehp1. to default value of 0.


.* Query window's state
.*
:h2 group=1 scroll=none width=30% name=SM_QUERYHILITE.SM_QUERYHILITE
:link auto reftype=hd dependent refid=SM_QUERYHILITE_SYNTAX.
:link auto reftype=hd refid=SM_QUERYHILITE.
:p.Select topic&colon.
:p.
:link dependent reftype=hd refid=SM_QUERYHILITE_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=SM_QUERYHILITE_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=SM_QUERYHILITE_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=SM_QUERYHILITE_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=SM_QUERYHILITE_DFPROC.Default processing:elink.


.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=SM_QUERYHILITE_SYNTAX.SM_QUERYHILITE - syntax
:p.This message returns current window's enable/disable state.
:p.:p.:font facename=Courier size=10x10.:cgraphic.
param1
     ULONG  :link dependent reftype=hd refid=SM_QUERYHILITE_RES.ulReserved:elink.  // Reserved value, equal to 0.

param2
     ULONG  :link dependent reftype=hd refid=SM_QUERYHILITE_RES.ulReserved:elink.  // Reserved value, equal to 0.
:ecgraphic.:font.

.* Reserved value, equal to 0.
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=SM_QUERYHILITE_RES.SM_QUERYHILITE - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=SM_QUERYHILITE_PARAMS.SM_QUERYHILITE - parameters
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.


.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=SM_QUERYHILITE_RESULT.SM_QUERYHILITE - results
:dl tsize=3 break=all.
:dt.:hp2.bActive:ehp2. (BOOL)
:dd.Current enable/disable state of the window.
:edl.


.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=SM_QUERYHILITE_REMARK.SM_QUERYHILITE - remarks
:p.Status window responds to this message by returning TRUE if it's enabled or
FALSE if it's disabled.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=SM_QUERYHILITE_DFPROC.SM_QUERYHILITE - default processing
The default window procedure takes no action on this message, other than
to set :hp1.bActive:ehp1. to default value of 0.



.* query total window width
.*
:h2 group=1 scroll=none width=30% name=SM_QUERYWIDTH.SM_QUERYWIDTH
:link auto reftype=hd dependent refid=SM_QUERYWIDTH_SYNTAX.
:link auto reftype=hd refid=SM_QUERYWIDTH.
:p.Select topic&colon.
:p.
:link dependent reftype=hd refid=SM_QUERYWIDTH_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=SM_QUERYWIDTH_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=SM_QUERYWIDTH_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=SM_QUERYWIDTH_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=SM_QUERYWIDTH_DFPROC.Default processing:elink.


.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=SM_QUERYWIDTH_SYNTAX.SM_QUERYWIDTH - syntax
:p.This message returns total window width in pixels.
:p.:p.:font facename=Courier size=10x10.:cgraphic.
param1
     ULONG  :link dependent reftype=hd refid=SM_QUERYWIDTH_RES.ulReserved:elink.  // Reserved value, equal to 0.

param2
     ULONG  :link dependent reftype=hd refid=SM_QUERYWIDTH_RES.ulReserved:elink.  // Reserved value, equal to 0.
:ecgraphic.:font.

.* Reserved value, equal to 0.
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=SM_QUERYWIDTH_RES.SM_QUERYWIDTH - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=SM_QUERYWIDTH_PARAMS.SM_QUERYWIDTH - parameters
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.


.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=SM_QUERYWIDTH_RESULT.SM_QUERYWIDTH - results
:dl tsize=3 break=all.
:dt.:hp2.lWidth:ehp2. (LONG)
:dd.Total window's width.
:edl.


.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=SM_QUERYWIDTH_REMARK.SM_QUERYWIDTH - remarks
:p.Status window responds to this message by returning window's width
necessary to fit the text and the border. It is equal to value returned by
SM_QUERYTEXTWIDTH with addition of left and right border's width and value
from SM_QUERYMAXCHARINC divided by two (average character's width).
:p.Returned value does not depend on current window size set by WinSetWindowPos.


.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=SM_QUERYWIDTH_DFPROC.SM_QUERYWIDTH - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.lWidth:ehp1. to default value of 0.



.* enable/disable window
.*
:h2 group=1 scroll=none width=30% name=SM_SETHILITE.SM_SETHILITE
:link auto reftype=hd dependent refid=SM_SETHILITE_SYNTAX.
:link auto reftype=hd refid=SM_SETHILITE.
:p.Select topic&colon.
:p.
:link dependent reftype=hd refid=SM_SETHILITE_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=SM_SETHILITE_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=SM_SETHILITE_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=SM_SETHILITE_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=SM_SETHILITE_DFPROC.Default processing:elink.


.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=SM_SETHILITE_SYNTAX.SM_SETHILITE - syntax
:p.This function sets the window enabled state.
:p.:p.:font facename=Courier size=10x10.:cgraphic.
param1
     BOOL   :link dependent reftype=hd refid=SM_SETHILITE_NEW.bNewstate:elink.   // New window state

param2
     ULONG  :link dependent reftype=hd refid=SM_SETHILITE_RES.ulReserved:elink.  // Reserved value, equal to 0
:ecgraphic.:font.

.* New window state
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=SM_SETHILITE_NEW.SM_SETHILITE - bNewstate
:dl tsize=3 break=all.
:dt.:hp2.bNewstate:ehp2. (ULONG)
:dd.New window state &colon.
:dl tsize=6 break=all.
:dt.TRUE
:dd.Window will be enabled.
:dt.FALSE
:dd.Window will be disabled.
:edl.
:edl.

.* Reserved value, equal to 0
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=SM_SETHILITE_RES.SM_SETHILITE - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=SM_SETHILITE_PARAMS.SM_SETHILITE - parameters
:dl tsize=3 break=all.
:dt.:hp2.bNewstate:ehp2. (ULONG)
:dd.New window state &colon.
:dl tsize=6 break=all.
:dt.TRUE
:dd.Window will be enabled.
:dt.FALSE
:dd.Window will be disabled.
:edl.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.


.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=SM_SETHILITE_RESULT.SM_SETHILITE - results
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, equal to 0.
:edl.


.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=SM_SETHILITE_REMARK.SM_SETHILITE - remarks
:p.Status window responds to this message by changing it's apperance. Besides
this window behavior does not change - it still processes and sens messages.
:p.Enabling window causes drawing it using following colors&colon.
:dl compact tsize=10 break=none.
:dt.Background&colon.
:dd.PP_BACKGROUNDCOLOR
:dt.Text&colon.
:dd.PP_FOREGROUNDCOLOR
:edl.
:p.Disabling window causes drawing it using following colors&colon.
:dl compact tsize=10 break=none.
:dt.Background&colon.
:dd.PP_DISABLEDBACKGROUNDCOLOR
:dt.Text&colon.
:dd.PP_DISABLEDFOREGROUNDCOLOR
:edl.
:p.Border colors does not change.
:p.WinCreateWindow function always creates enabled status window.


.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=SM_SETHILITE_DFPROC.SM_SETHILITE - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.ulReserved:ehp1. to default value of 0.






