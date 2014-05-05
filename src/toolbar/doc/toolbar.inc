.* Control windows library - Asu'a Custom Controls
.* Toolbar (WC_TOOLBAR) - documentation.
.* (c) 1999-2001 Wojciech Gazda
.*
.* toolbar/doc/toolbar.inc
.*
.* $Author: gazda $
.* $Date: 2001/04/28 01:27:01 $
.* $Name:  $
.* $RCSfile: toolbar.inc,v $
.* $Revision: 1.1 $
.*
.* Asu'a Programmers Group http://www.asua.org.pl
.*


:h1 name=WC_TOOLBAR.WC_TOOLBAR - Toolbar
:p.:hp2.Overview:ehp2.
:p.The Toolbar window has been provided to fill a "hole" in the
OS/2 set of controls, which lacks of a toolbar control.


:h2 name=WC_TOOLBAR_ERRORS.Known errors
:p.:hp2.Known errors&colon.:ehp2.
:dl tsize=3 break=all.
:dt.:hp1.Documentation:ehp1.
:dd.
:ul compact.
:li.Lack of the TN_ROTATE style description.
:eul.
:dt.:hp1.Software:ehp1.
:dd.
:ul compact.
:li.Error in processing of TM_CALCNEWRECT, TM_QUERYWIDTH and TM_QUERYHEIGHT messages.
The "Rotate" button is not taken into account when dimensions are claculating.
:li.Objects are not centered&colon. in vertical direction for horizontal toolbars,
in horizontal direction for vertical toolbars.
:eul.
:edl.


:h2 name=WC_TOOLBAR_STYLE.Window styles
:p.Following window styles are available for the Toolbar window&colon.

:p.:hp2.Clipping control (if objects don't fit within a toolbar)&colon.:ehp2.
:dl tsize=4 break=all.
:dt.TBS_STANDARD
:dd.Objects that don't fit within a toolbar are clipped out at the right
border (horizontal toolbars) or bottom border (vertical toolbars).
:dt.TBS_SCROLLABLE
:dd.Scroll buttons appear if toolbar contains objects that do not fit within
(not implemented yet).
:dt.TBS_MULTILINE	
:dd.Object that do not fit are moved to the next line (not implemented yet).
:edl.

:p.:hp2.Direct manipulation control&colon.:ehp2.
:dl tsize=4 break=all.
:dt.TBS_MOVEWITHOWNER
:dd.Toolbar moves in the "floating" state together with owner.
:dt.TBS_ENABLEMINIMIZE
:dd.Enable Toolbar minimization. Clicking a title bar
(small rectangle on the left) switches toolbar between minimized
and maximized state.
:dt.TBS_MOVEBUTTON
:dd.Toolbar shows the title bar (small rectangle on the left),
which allows user to drag the toolbar using mouse.
:dt.TBS_FIXBUTTON      	
:dd.The docking button appears on the title bar. It is a small triangle,
which allows docking/undocking of the Toolbar. Undocking means
switching the Toolbar to the floating state, where it can be moved idependently
on the owner (see also TBS_MOVEWITHOWNER).
:dt.TBS_ROTATEBUTTON
:dd.Toolbar shows special button that allows user to rotate the Toolbar
using mouse.
:edl.



:h2 name=WC_TOOLBAR_STDCOLOR.Standard colors, fonts and timeouts
:p.Presentation parameters listed here consist of colors, fonts and defaults
for timers controlling display of tool tips. All parameters are not inherited
from the owner.

:p.:hp2.Default colors&colon.:ehp2.
:ul compact.
:li.Toolbar background.
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_BACKGROUNDCOLOR
:row.
:c.Default system color
:c.SYSCLR_BUTTONMIDDLE
:row.
:c.Notes
:c.none
:etable.

:li.Titlebar highlighted background
.br
This color is used to highlight the title bar and rotate button
of the Toolbar, when mouse pointer passes over these objects.
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_ACTIVECOLOR
:row.
:c.Default system color
:c.Light blue (0xC7D5FF)
:row.
:c.Notes
:c.Default color is hard-coded within Toolbar's source code.
:etable.

:li.Highlighted outer border color (3D effect).
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_BORDERLIGHTCOLOR
:row.
:c.Default system color
:c.White (0xFFFFFF)
:row.
:c.Notes
:c.Default color is hard-coded within Toolbar's source code.
:etable.

:li.Shadowed outer border color (3D effect).
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_BORDERDARKCOLOR
:row.
:c.Default system color
:c.Gray (0x808080)
:row.
:c.Notes
:c.Default color is hard-coded within Toolbar's source code.
:etable.

:li.Tool-tip background color.
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_TOOLTIPBACKGROUND
:row.
:c.Default system color
:c.Pale yellow (0xFFFFC8)
:row.
:c.Notes
:c.Default color is hard-coded within Toolbar's source code.
:etable.

:li.Tool-tip text color.
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_TOOLTIPTEXT
:row.
:c.Default system color
:c.Black (0x000000)
:row.
:c.Notes
:c.Default color is hard-coded within Toolbar's source code.
:etable.

:li.Color of frame drawn around tool-tip.
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_TOOLTIPFRAME
:row.
:c.Default system color
:c.Black (0x000000)
:row.
:c.Notes
:c.Default color is hard-coded within Toolbar's source code.
:etable.

:li.Highlighted inner border (3D effect)
.br
Drawn when Toolbar is in the foloating state or is being dragged.
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_BORDER2LIGHTCOLOR
:row.
:c.Default system color
:c.Black (0x808080)
:row.
:c.Notes
:c.Default color is hard-coded within Toolbar's source code.
:etable.

:li.Shadowed inner border (3D effect)
.br
Drawn when Toolbar is in the foloating state or is being dragged.
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_BORDER2DARKCOLOR
:row.
:c.Default system color
:c.Black (0x000000)
:row.
:c.Notes
:c.Default color is hard-coded within Toolbar's source code.
:etable.
:eul.


:p.:hp2.Default fonts&colon.:ehp2.
:ul compact.
:li.Tool-tip font
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_TOOLTIPFONT
:row.
:c.Default font
:c.8.Helv
:row.
:c.Notes
:c.none
:etable.
:eul.

:p.:hp2.Default timings&colon.:ehp2.
:ul compact.
:li.Delay time before tool-tip is being shown (in miliseconds).
:table cols='30 30'.
:row.
:c.Presenation parameter
:c.PP_TOOLTIPDELAY
:row.
:c.Default time
:c.1000
:row.
:c.Notes
:c.none
:etable.
:eul.



.* Internal windows identifiers
.*
:h2 name=WC_TOOLBAR_SUBWINDOWS.Internal windows
:p.Following object's identifiers (child windows) have been reserved
for internal use&colon.
:dl tsize=4 break=all.
:dt.TCID_TITLEBAR (0xFFFF)
:dd.Title bar.
:dt.TCID_TOOLTIP (0xFFFE)
:dd.Tool-tip window.
:dt.TCID_ROTATE (0xFFFC)
:dd.Rotate button.
:dt.TCID_OBJECTPAD (0xFFF9)
:dd.Window holding objects.
:edl.
:p.:hp2.Note&colon.:ehp2. These identifiers may change in next release.
Do not use, and do not rely on them during software development.



.* Data structures
.*
:h2 name=WC_TOOLBAR_DATA.Control data structures
:p.See the following for information on the Toolbar control data structures&colon.
:ul compact.
:li.:link reftype=hd refid=TOOLADJ.TOOLADJ:elink.
:li.:link reftype=hd refid=TOOLINFO.TOOLINFO:elink.
:li.:link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink.
:li.:link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink.
:li.:link reftype=hd refid=TOOLWIN.TOOLWIN:elink.
:eul.


.* TOOLADJ structure
.* *****************
.*
:h3 group=20 name=TOOLADJ.TOOLADJ
:p.
:cgraphic.
typedef struct _TOOLADJ
{ HWND   :link dependent reftype=hd refid=TOOLADJ_TOOL.hwndToolBar:elink.;   // Toolbar window handle
  HWND   :link dependent reftype=hd refid=TOOLADJ_PARENT.hwndParent:elink.;    // Handle of suggested parent window
  RECTL  :link dependent reftype=hd refid=TOOLADJ_SIZE.rclSizePos:elink.;    // Suggested size and position
  ULONG  :link dependent reftype=hd refid=TOOLADJ_ATTRS.flAttrs:elink.;       // Suggested attributes
  ULONG  :link dependent reftype=hd refid=TOOLADJ_STATE.flState:elink.;       // Former state of the Toolabr
                        // (current for the TN_AUTOSIZE notification)
} TOOLADJ;
:ecgraphic.
:p.This structure is used by the following messages (see its description for
details about fields usage)&colon.
:link reftype=hd refid=TM_CALCNEWRECT.TM_CALCNEWRECT:elink.,
:link reftype=hd refid=TOOLBAR_WM_CONTROL.WM_CONTROL:elink. in the
TN_AUTOSIZE, TN_FIXTOOL, TN_MINIMIZE and TN_RESTORE notifications.


.* TOOLADJ - hwndToolbar
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLADJ_TOOL.TOOLADJ - hwndToolbar field
:dl tsize=3 break=all.
:dt.:hp2.hwndToolbar:ehp2. (HWND)
:dd.Toolbar window handle.
:edl.

.* TOOLADJ - hwndParent
.*
:h3 group=22 x=30% y=bottom width=70% height=225x hide name=TOOLADJ_PARENT.TOOLADJ - hwndParent field
:dl tsize=3 break=all.
:dt.:hp2.hwndParent:ehp2. (HWND)
:dd.Handle of the Toolbar parent window, valid during processing of the
:link reftype=hd refid=TOOLBAR_WM_CONTROL.WM_CONTROL:elink. message for the
TN_AUTOSIZE, TN_FIXTOOL, TN_MINIMIZE and TN_RESTORE notifications.
:edl.

.* TOOLADJ - rclSizePos
.*
:h3 group=23 x=30% y=bottom width=70% height=225x hide name=TOOLADJ_SIZE.TOOLADJ - rclSizePos field
:dl tsize=3 break=all.
:dt.:hp2.rclSizePos:ehp2. (RECTL)
:dd.During processing of the
:link reftype=hd refid=TOOLBAR_WM_CONTROL.WM_CONTROL:elink. message it contains
suggested target position calculated by the Toolbar, which can be changed by
modification of this field.
:p.Result of :link reftype=hd refid=TM_CALCNEWRECT.TM_CALCNEWRECT:elink. message
processing - size of the Toolbar for specified set of parameters.
:edl.

.* TOOLADJ - flAttrs
.*
:h3 group=24 x=30% y=bottom width=70% height=225x hide name=TOOLADJ_ATTRS.TOOLADJ - flAttrs field
:dl tsize=3 break=all.
:dt.:hp2.flAttrs:ehp2. (ULONG)
:dd.Combination of the following atributes can be passed&colon.
:p.:hp2.Border drawing&colon.:ehp2.
:dl tsize=3 break=all compact.
:dt.TBA_FRAME
:dd.Complete 3D-style border around the Toolbar.
:dt.TBA_LEFTFRAME
:dd.Border only at the left edge.
:dt.TBA_BOTTOMFRAME
:dd.Border only at the bottom edge.
:dt.TBA_RIGHTFRAME
:dd.Border only at the right edge.
:dt.TBA_TOPFRAME
:dd.Border only at the top edge.
:edl.
:p.:hp2.Docking control&colon.:ehp2.
:dl tsize=3 break=all compact.
:dt.TBA_FIXEDTOP
:dd.Horizontal Toolbar docks at the top edge of parent window.
:dt.TBA_FIXEDBOTTOM
:dd.Horizontal Toolbar docks at the bottom edge of parent window.
:dt.TBA_FIXEDLEFT
:dd.Vertical Toolbar docks at the left edge of parent window.
:dt.TBA_FIXEDRIGHT
:dd.Vertical toolbar docks at the right edge of parent window.
:edl.
:p.:hp2.Initial toolbar orientation&colon.:ehp2.
:dl tsize=3 break=all compact.
:dt.TBA_HORIZONTAL
:dd.Horizontal Toolbar (default).
:dt.TBA_VERTICAL
:dd.Vertical Toolbar.
:edl.
:p.It may be changed on the fly, during
:link reftype=hd refid=TOOLBAR_WM_CONTROL.WM_CONTROL:elink. message processing.
:edl.

.* TOOLADJ - flState
.*
:h3 group=25 x=30% y=bottom width=70% height=225x hide name=TOOLADJ_STATE.TOOLADJ - flState field
:dl tsize=3 break=all.
:dt.:hp2.flState:ehp2. (ULONG)
:dd.Initial state of the Toolbar (target state for the TN_AUTOSIZE notification
of the :link reftype=hd refid=TOOLBAR_WM_CONTROL.WM_CONTROL:elink. message)&colon.
:dl tsize=3 break=all compact.
:dt.TST_FIXED
:dd.Toolbar in docked state.
:dt.TST_MINIMIZED
:dd.Toolbar is minimized.
:dt.TST_ROTATED
:dd.Toolbar is rotated (in vertical position).
:edl.
:p.If both TST_FIXED and TST_MINIMIZED flags are not set, Toolbar is in
a floating state.
:edl.



.* TOOLINFO structure
.* ******************
.*
:h3 group=20 name=TOOLINFO.TOOLINFO
:p.
:cgraphic.
typedef struct _TOOLINFO
{ USHORT  :link dependent reftype=hd refid=TOOLINFO_CB.cb:elink.;                 // Total size of the structure in bytes
  USHORT  :link dependent reftype=hd refid=TOOLINFO_ATTRS.flToolAttrs:elink.;        // Toolbar attributes
  USHORT  :link dependent reftype=hd refid=TOOLINFO_LENGTH.usMaxLength:elink.;        // Maximum length of the Toolbar
} TOOLINFO;
:ecgraphic.
:p.This structure contains control-data used during creation of
the Toolbar (WinCreateWindow - the :hp1.pCtlData:ehp1. parameter), and
by WM_SETWINDOWPARAMS message. These data controls behaviour of the Toolbar.

.* TOOLINFO - cb
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLINFO_CB.TOOLINFO - cb field
:dl tsize=3 break=all.
:dt.:hp2.cb:ehp2. (USHORT)
:dd.This is total size of the structure in bytes.
:edl.

.* TOOLINFO - flToolAttrs
.*
:h3 group=22 x=30% y=bottom width=70% height=225x hide name=TOOLINFO_ATTRS.TOOLINFO - flToolAttrs field
:dl tsize=3 break=all.
:dt.:hp2.flToolAttrs:ehp2. (USHORT)
:dd.Toolbar attributes.
:p.:hp2.Border drawing&colon.:ehp2.
:dl tsize=3 break=all compact.
:dt.TBA_FRAME
:dd.Complete 3D-style border around the Toolbar.
:dt.TBA_LEFTFRAME
:dd.Border only at the left edge.
:dt.TBA_BOTTOMFRAME
:dd.Border only at the bottom edge.
:dt.TBA_RIGHTFRAME
:dd.Border only at the right edge.
:dt.TBA_TOPFRAME
:dd.Border only at the top edge.
:edl.
:p.:hp2.Docking control&colon.:ehp2.
:dl tsize=3 break=all compact.
:dt.TBA_FIXEDTOP
:dd.Horizontal Toolbar docks at the top edge of parent window.
:dt.TBA_FIXEDBOTTOM
:dd.Horizontal Toolbar docks at the bottom edge of parent window.
:dt.TBA_FIXEDLEFT
:dd.Vertical Toolbar docks at the left edge of parent window.
:dt.TBA_FIXEDRIGHT
:dd.Vertical toolbar docks at the right edge of parent window.
:edl.
:p.:hp2.Initial toolbar orientation&colon.:ehp2.
:dl tsize=3 break=all compact.
:dt.TBA_HORIZONTAL
:dd.Horizontal Toolbar (default).
:dt.TBA_VERTICAL
:dd.Vertical Toolbar.
:edl.
:edl.

.* TOOLINFO - usMaxLength
.*
:h3 group=23 x=30% y=bottom width=70% height=225x hide name=TOOLINFO_LENGTH.TOOLINFO - usMaxLength field
:dl tsize=3 break=all.
:dt.:hp2.usMaxLength:ehp2. (USHORT)
:dd.This parameter determines maximum length of the Toolbar in a floating state.
For horizontal toolbars it is width, for the vertical ones it is height. Parameter
is ignored when set to 0.
:edl.



.* TOOLOBJ structure
.* *****************
.*
:h3 group=20 name=TOOLOBJ.TOOLOBJ
:p.
:cgraphic.
typedef struct _TOOLOBJ
{ ULONG   :link reftype=hd dependent refid=TOOLOBJ_POS.iPosition:elink.;     // Object position on the Toolbar (ordinal number)
  ULONG   :link reftype=hd dependent refid=TOOLOBJ_GRP.ulGroup:elink.;       // Button group object belongs to (valid for buttons only)
  ULONG   :link reftype=hd dependent refid=TOOLOBJ_ATTR.flAttribute:elink.;   // Additional attributes
  ULONG   :link reftype=hd dependent refid=TOOLOBJ_CX.cx:elink., :link reftype=hd refid=TOOLOBJ_CY.cy:elink.;        // Initial size of the object (except TBO_BUTTON)
  ULONG   :link reftype=hd dependent refid=TOOLOBJ_ID.id:elink.;            // Object identifier
  ULONG   :link reftype=hd dependent refid=TOOLOBJ_USER.ulUser:elink.;        // Field available for an user
} TOOLOBJ;
:ecgraphic.
:p.This structure is used by the
:link reftype=hd refid=TM_INSERTOBJECT.TM_INSERTOBJECT:elink.,
:link reftype=hd refid=TM_QUERYOBJECT.TM_QUERYOBJECT:elink. and
:link reftype=hd refid=TM_SETOBJECT.TM_SETOBJECT:elink. messages.

.* TOOLOBJ - iPosition
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLOBJ_POS.TOOLOBJ - iPosition field
:dl tsize=3 break=all.
:dt.:hp2.iPosition:ehp2. (ULONG)
:dd.Position of an object on the Toolbar, calculated from the beginning of the Toolabr.
Specifying TB_END flag allows to add object at the and of the Toolbar.
:edl.

.* TOOLOBJ - ulGroup
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLOBJ_GRP.TOOLOBJ - ulGroup field
:dl tsize=3 break=all.
:dt.:hp2.ulGroup:ehp2. (ULONG)
:dd.Group of objects. This field has meaning only for objects being created with
TBO_BUTTON and TBO_STDBUTTON attributes, and determines behaviour of
button group.
:p.During object adding by :link reftype=hd refid=TM_INSERTOBJECT.TM_INSERTOBJECT:elink.
it is possible to change style of a whole group, which will make effect on
behaviour of all buttons in this group.
:p.Using following flags in the :hp1.flAttributes:ehp1. field, it is possible to
get the appropriate behaviour of buttons in the group&colon.
:dl tsize=3 break=all.
:dt.TBO_PUSHBUTTON
:dd.Buttons belonging to a group will be independent among themselves.
:dt.TBO_CHECKBOX 	
:dd.Buttons will be bistable - each button remain pressed when clicked, and then
depressed when clicked again. Several buttons at a time can remain in the pressed
state.
:dt.TBO_RADIOBUTTON
:dd.Only single button in a group can remain in pressed state at a time, the all
remaining ones automatically change its state to depressed.
:edl.
:p.If none of the above style is used, current group style remains unchanged.
If we didn't use any style creating first button in a group, the
default TBO_PUSHBUTTON style will be used.
:p.Buttons in a group may not be adjacent.
:edl.

.* TOOLOBJ - flAttribute
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLOBJ_ATTR.TOOLOBJ - flAttribute field
:dl tsize=3 break=all.
:dt.:hp2.flAttribute:ehp2. (ULONG)
:dd.Styles for newly added objects.
:p.:hp2.Basic object styles&colon.:ehp2.
:dl tsize=3 break=all compact.
:dt.TBO_STDWINDOW
:dd.The control window being added (an object) will not be supported by
the Toolbar in any special way.
:dt.TBO_BUTTON
:dd.Button (the WC_WINBUTTON class) fully supported by the Toolbar.
:dt.TBO_STDBUTON
:dd.Like the above, except automatic resizing (for the WC_BUTTON class).
:dt.TBO_SEPARTOR
:dd.Objects separator.
:edl.
:p.The TBO_STDBUTON can be used also fir WC_WINBUTTON class, but in this
case buttons will not be scaled automatically. See description of
the :hp1.ulGroup:ehp1. for more about button support.
:p.:hp2.Objects positioning&colon.:ehp2.
:dl tsize=3 break=all compact.
:dt.TBO_BEGINALIGN
:dd.Object aligned to the beginning of the Toolbar.
:dt.TBO_ENDALIGN
:dd.Object aligned to the end of the Toolbar.
:dt.TBO_CENTERALIGN
:dd.Centered object.
:edl.
:p.For details see description of the :link reftype=hd refid=TM_INSERTOBJECT.TM_INSERTOBJECT:elink.
message.
:p.:hp2.Generic state of objects&colon.:ehp2.
:dl tsize=3 break=all compact.
:dt.TBO_HIDE
:dd.Hidden object will be created.
:dt.TBO_DISABLE
:dd.Disabled object will be created.
:edl.
:p.:hp2.Separator styles&colon.:ehp2.
:dl tsize=3 break=all compact.
:dt.SPS_FLAT
:dd.Flat separator - space in background color.
:dt.SPS_LINE		
:dd.Line in the PP_BORDERDARKCOLOR color.
:dt.SPS_CONCAVE
:dd.Concave 3D-style line.
:dt.SPS_CONVEX
:dd.Convex 3D-style line.
:edl.
:edl.

.* TOOLOBJ - cx
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLOBJ_CX.TOOLOBJ - cx field
:dl tsize=3 break=all.
:dt.:hp2.cx:ehp2. (ULONG)
:dd.Initial object width. Ignored for TB_BUTTON on vertical Toolbars. Ignored
for separators on horizontal Toolbars.
:edl.

.* TOOLOBJ - cy
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLOBJ_CY.TOOLOBJ - cy field
:dl tsize=3 break=all.
:dt.:hp2.cy:ehp2. (ULONG)
:dd.Initial object height. Ignored for TB_BUTTON on horizontal Toolbars. Ignored
for separators on vertical Toolbars.
:edl.

.* TOOLOBJ - id
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLOBJ_ID.TOOLOBJ - id field
:dl tsize=3 break=all.
:dt.:hp2.id:ehp2. (ULONG)
:dd.Object identifier. Should be unique for all objects placed on the Toolabr, and
for all objects posting the WM_COMMAND, WM_HELP and WM_SYSCOMMAND messages
to theirs common owner.
:p.For example, if a :hp1.hwnd:ehp1. window owns a Toolbar holding objects with
identifiers&colon. 100, 101 and 102 than the :hp1.hwnd:ehp1. window cannot
own another objects (eg. buttons) with the same identifiers. Similarily,
other Toolbars owned by the :hp1.hwnd:ehp1. window cannot contain objects
with the same identifiers.
:edl.

.* TOOLOBJ - ulUser
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLOBJ_USER.TOOLOBJ - ulUser field
:dl tsize=3 break=all.
:dt.:hp2.ulUser:ehp2. (ULONG)
:dd.Any data supplied by an user (i.e. pointer to custom data structures associated
with an object).
:edl.



.* TOOLTRACK structure
.* *******************
.*
:h3 group=20 name=TOOLTRACK.TOOLTRACK
:p.
:cgraphic.
typedef struct _TOOLTRACK
{ HWND   :link reftype=hd dependent refid=TOOLTRACK_TOOL.hwndToolBar:elink.;     // Toolbar window handle
  HWND   :link reftype=hd dependent refid=TOOLTRACK_PARENT.hwndParent:elink.;      // Parent window handle
  RECTL  :link reftype=hd dependent refid=TOOLTRACK_SPOS.rclSizePos:elink.;      // Size and position relative to parent window
  RECTL  :link reftype=hd dependent refid=TOOLTRACK_BOUND.rclBoundary:elink.;     // Boundary of a movement area (within parent window)
  ULONG  :link reftype=hd dependent refid=TOOLTRACK_ATTR.flAttrs:elink.;         // Toolbar attributes
  ULONG  :link reftype=hd dependent refid=TOOLTRACK_STAT.flState:elink.;         // Current state of the Toolabr
} TOOLTRACK;
:ecgraphic.
:p.Structure is used during processing of the
:link reftype=hd refid=TOOLBAR_WM_CONTROL.WM_CONTROL:elink. message
for TN_BEGINTRACK and TN_ENDTRACK notifications (see
:link reftype=hd refid=TOOLBAR_WM_CONTROL.WM_CONTROL:elink. parameters
description for details).

.* TOOLTRACK - hwndToolBar
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLTRACK_TOOL.TOOLTRACK - hwndToolBar field
:dl tsize=3 break=all.
:dt.:hp2.hwndToolBar:ehp2. (HWND)
:dd.Toolbar window handle.
:edl.

.* TOOLTRACK - hwndParent
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLTRACK_PARENT.TOOLTRACK - hwndParent field
:dl tsize=3 break=all.
:dt.:hp2.hwndParent:ehp2. (HWND)
:dd.Handle of Toolbar's parent window. Can be used to determine Toolbar coordinates
in relation to the parent window's.
:edl.

.* TOOLTRACK - rclSizePos
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLTRACK_SPOS.TOOLTRACK - rclSizePos field
:dl tsize=3 break=all.
:dt.:hp2.rclSizePos:ehp2. (RECTL)
:dd.Size and position of the Toolbar when dragging begins or ends.
Modification affects target position and size of the Toolbar when drag operation ends.
:edl.

.* TOOLTRACK - rclBoundary
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLTRACK_BOUND.TOOLTRACK - rclBoundary field
:dl tsize=3 break=all.
:dt.:hp2.rclBoundary:ehp2. (RECTL)
:dd.Size and position of area within Toolbar can be dragged.
Modification affects boundaries of dragging area of the Toolbar when drag
operation starts.
:edl.

.* TOOLTRACK - flAttrs
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLTRACK_ATTR.TOOLTRACK - flAttrs field
:dl tsize=3 break=all.
:dt.:hp2.flAttrs:ehp2. (ULONG)
:dd.Informational field, may contain combinations of the following attributes&colon.
:p.:hp2.Border drawing&colon.:ehp2.
:dl tsize=3 break=all compact.
:dt.TBA_FRAME
:dd.Complete 3D-style border around the Toolbar.
:dt.TBA_LEFTFRAME
:dd.Border only at the left edge.
:dt.TBA_BOTTOMFRAME
:dd.Border only at the bottom edge.
:dt.TBA_RIGHTFRAME
:dd.Border only at the right edge.
:dt.TBA_TOPFRAME
:dd.Border only at the top edge.
:edl.
:p.:hp2.Docking control&colon.:ehp2.
:dl tsize=3 break=all compact.
:dt.TBA_FIXEDTOP
:dd.Horizontal Toolbar docks at the top edge of parent window.
:dt.TBA_FIXEDBOTTOM
:dd.Horizontal Toolbar docks at the bottom edge of parent window.
:dt.TBA_FIXEDLEFT
:dd.Vertical Toolbar docks at the left edge of parent window.
:dt.TBA_FIXEDRIGHT
:dd.Vertical toolbar docks at the right edge of parent window.
:edl.
:p.:hp2.Initial toolbar orientation&colon.:ehp2.
:dl tsize=3 break=all compact.
:dt.TBA_HORIZONTAL
:dd.Horizontal Toolbar (default).
:dt.TBA_VERTICAL
:dd.Vertical Toolbar.
:edl.
:edl.

.* TOOLTRACK - flState
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLTRACK_STAT.TOOLTRACK - flState field
:dl tsize=3 break=all.
:dt.:hp2.flState:ehp2. (ULONG)
:dd.Informational field, may contain combination of the following attributes&colon.
:dl tsize=3 break=all compact.
:dt.TST_FIXED
:dd.Toolbar in docked state.
:dt.TST_MINIMIZED
:dd.Toolbar is minimized.
:dt.TST_ROTATED
:dd.Toolbar is rotated (in vertical position).
:edl.
:p.If both TST_FIXED and TST_MINIMIZED flags are not set, Toolbar is in
a floating state.
:edl.


.* TOOLWIN structure
.* *******************
.*
:h3 group=20 name=TOOLWIN.TOOLWIN
:p.
:cgraphic.
typedef struct _TOOLWIN
{ PSZ     :link reftype=hd dependent refid=TOOLWIN_CLASS.pszClass:elink.;     // Class of newly created control window
  PSZ     :link reftype=hd dependent refid=TOOLWIN_NAME.pszName:elink.;      // Window text for created window
  ULONG   :link reftype=hd dependent refid=TOOLWIN_STYL.flStyle:elink.;      // Window style flage
  PVOID   :link reftype=hd dependent refid=TOOLWIN_PCTL.pCtlData:elink.;     // Pointer to class-specific data
  PVOID   :link reftype=hd dependent refid=TOOLWIN_PPARAMS.pPresParams:elink.;  // Windows presentation parameters
} TOOLWIN;
:ecgraphic.
:p.This structure supplies information passed by
:link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink. structure with parameters
needed by WinCreateWindow system call actually creating control window on the Toolbar.
:p.This structure is used only by the
:link reftype=hd refid=TM_INSERTOBJECT.TM_INSERTOBJECT:elink. message. It is
not necessary for separators.

.* TOOLWIN - pszClass
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLWIN_CLASS.TOOLWIN - pszClass field
:dl tsize=3 break=all.
:dt.:hp2.pszClass:ehp2. (PSZ)
:dd.Window class of created window.
:edl.

.* TOOLWIN - pszName
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLWIN_NAME.TOOLWIN - pszName field
:dl tsize=3 break=all.
:dt.:hp2.pszName:ehp2. (PSZ)
:dd.Window text for created window.
:edl.

.* TOOLWIN - flStyle
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLWIN_STYL.TOOLWIN - flStyle field
:dl tsize=3 break=all.
:dt.:hp2.flStyle:ehp2. (ULONG)
:dd.Window style flags.
:edl.

.* TOOLWIN - pCtlData
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLWIN_PCTL.TOOLWIN - pCtlData field
:dl tsize=3 break=all.
:dt.:hp2.pCtlData:ehp2. (PVOID)
:dd.Pointer to windows class-specific data structure.
:edl.

.* TOOLWIN - pPresParams
.*
:h3 group=21 x=30% y=bottom width=70% height=225x hide name=TOOLWIN_PPARAMS.TOOLWIN - pPresParams field
:dl tsize=3 break=all.
:dt.:hp2.pPresParams:ehp2. (PVOID)
:dd.Pointer to windows presentation parametes structure.
:edl.




.* Notification Messages
.*
:h2 name=WC_TOOLBAR_NOTIFY.Notification Messages
:p.These messages are initiated by the Tolbar control window to notify its
owner of significant events.


.* WM_CONTROL
.*
:h2 group=1 scroll=none width=30% name=TOOLBAR_WM_CONTROL.WM_CONTROL (in Toolbar Windows)
:link auto reftype=hd dependent refid=TOOLBAR_CONTROL_SYNTAX.
:link auto reftype=hd refid=TOOLBAR_WM_CONTROL.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TOOLBAR_CONTROL_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TOOLBAR_CONTROL_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TOOLBAR_CONTROL_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TOOLBAR_CONTROL_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TOOLBAR_CONTROL_DFPROC.Default processing:elink.


.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TOOLBAR_CONTROL_SYNTAX.WM_CONTROL (in Toolbar Windows) - syntax
:p.For the cause of this message, see WM_CONTROL in the
"Presentation Manager Guide and Reference" book.
:cgraphic.
param1
     USHORT  :link dependent reftype=hd refid=TOOLBAR_CONTROL_ID.id:elink.             // Toolbar window identifier
     USHORT  :link dependent reftype=hd refid=TOOLBAR_CONTROL_NOTIFY.usnotifycode:elink.   // Notification code

param2
     ULONG   :link dependent reftype=hd refid=TOOLBAR_CONTROL_SPEC.flcontrolspec:elink.  // Control specific data
:ecgraphic.

.* window identifier
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TOOLBAR_CONTROL_ID.WM_CONTROL (in Toolbar Windows) Field - id
:dl tsize=3 break=all.
:dt.:hp2.id:ehp2. (USHORT)
:dd.Toolbar window identifier.
:edl.

.* notification code
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TOOLBAR_CONTROL_NOTIFY.WM_CONTROL (in Toolbar Windows) Field - usnotifycode
:dl tsize=3 break=all.
:dt.:hp2.usnotifycode:ehp2. (USHORT)
:dd.Notification code
:p.The Toolbar window uses following notification codes&colon.
:dl tsize=7 break=all.
:dt.TN_AUTOSIZE
:dd.This notification is generated by TM_AUTOSIZE message and allows
to take control over automatic window positioning process.
:p.The :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. structure passed together with notification contains&colon.
Toolbar handle, parent window handle, suggested position, suggested size of
the Toolbar, attributes and current window status. During processing of the
TN_AUTOSIZE notification, it is possible to change target position
and size of the Toolbar by modification of the suggested size and position.
:p.:hp2.flcontrolspec:ehp2. contains pointer to the :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. structure.

:dt.TN_BEGINTRACK
:dd.This notification is send when dragging of the Toolbar begins.
:p.The :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink. structure passed together with notification allows change of
the area limiting possible range of dragging. The rclBoundary field is to be used
for this purpose.
:p.By use of the WinSetWindowPos call it is possible to change current window state
(SWP_RESTORE, SWP_MAXIMIZE, SWP_MINIMIZE). The rclSizePos, rclBoundary and hwndParent
fields of the :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink. structure shall be updated with values set by the
WinSetWindowPos call, otherwise results are unpredictable.
:p.The flState and flAttrs fields are informational. Change made to them makes
no effect on message processing.
:p.:hp2.flcontrolspec:ehp2. contains pointer to the :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink. structure.

:dt.TN_BUTTONPAINT
:dd.This notification is generated by the Toolbar in response for the WM_COMMAND
message generated by object of style TBO_STDBUTTON (the WC_BUTTON window
having a BS_OWNERDRAW style enabled).
:p.The BN_PAINT notification sent by buton is being translated into
TN_BUTTONPAINT code.
:p.:hp2.flcontrolspec:ehp2. contains pointer to the standard, system defined
USERBUTTON structure.

:dt.TN_ENDTRACK
:dd.This notification is generated just before finishing of a drag operation.
It allows to change a target position and size of the Toolbar. The
rclSizePos field of :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink. structure shall be updated. All remaining
fields of the :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink. structure are informational and it's modification
doesn't make any effect on message processing.
:p.:hp2.flcontrolspec:ehp2. contains pointer to the :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink. structure.

:dt.TN_TRACKING
:dd.This notification is generated during dragging the Toolbar.
:p.The flcontrolspec pointer points to the system defined POINTL structure
containing current position of the bottom left Toolbar corner.
Calculated position is relative to bottom left corner of parent window.
:p.When "Full Window Drag" option has been enabled, notification is generated
in response of every WM_MOUSEMOVE message. When this option is disabled,
and also in tha OS/2 Warp 3 system, notification is send directly
before finishing of window dragging, but before the TN_ENDTRACK notification.
:p.:hp2.flcontrolspec:ehp2. contains pointer to the POINTL structure.

:dt.TN_FIXTOOL, TN_MINIMIZE, TN_RESTORE
:dd.These notifications are send during execution of the WinSetWindowPos
system call, depending on control bytes passed by :hp1.fl:ehp1. parameter&colon.
:dl tsize=3 break=all.
:dt.TN_FIXTOOL
:dd.Generated during Toolbar docking, when the SWP_MAXIMIZE bit is set.
:dt.TN_MINIMIZE
:dd.Generated during Toolbar minimization, when the SWP_MINIMIZE bit is set.
:dt.TN_RESTORE
:dd.Generated when Toolbar is switching to "floating" state, when the
SWP_RESTORE bit is set.
:edl.
:p.If any of the SWP_MINIMIZE, SWP_MAXIMIZE or SWP_RESTORE bits is set, WinSetWindowPos
call ignores coordinates passed in it's parameters generating this notifications.
Invocations of WinSetWindowPos with other control bytes set don't cause
generation of these notification - user has full control over position
and size of the Toolbar.
:p.During processing or the notifications, Toolbar owner can change following
parameters using passed :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. structure:
:ul.
:li.handle of a parent window (hwndParent) using the WinSetParent call, which
if parent window has been changed to HWND_DESKTOP, allows to move Toolbar otside
of the application window borders.
:li.Target size and position by modification of rclSizePos structure.
Position shall be calculated relatively to parent window.
:li.Attributes, by modification of flAttrs bits.
:eul.
:p.The flState field of the :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. structure contains previous window state
(TST_FIXED, TST_MINIMIZED or TST_ROTATED). Thanks to that parameters of
a target state can depend on the previous one.
:edl.
:edl.

.* Control specific data
.*
:h2 group=12 x=30% y=bottom width=70% height=225x hide name=TOOLBAR_CONTROL_SPEC.WM_CONTROL (in Toolbar Windows) Field - flcontrolspec
:dl tsize=3 break=all.
:dt.:hp2.flcontrolspec:ehp2. (LONG)
:dd.Control specific data.
:p.This field contains pointer to data structure, which depends on the
notification code
:link dependent reftype=hd refid=TOOLBAR_CONTROL_NOTIFY.usnotifycode:elink.&colon.
:dl tsize=3 break=all.
:dt.TN_AUTOSIZE, TN_FIXTOOL, TN_MINIMIZE, TN_RESTORE
:dd.Pointer to the :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. structure.
:dt.TN_BEGINTRACK, TN_ENDTRACK
:dd.Pointer to the :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink. structure.
:dt.TN_BUTTONPAINT
:dd.Pointer to the system defined USERBUTTON structure.
:dt.TN_TRACKING
:dd.Pointer to the system defined POINTL structure.
:edl.
:edl.


.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TOOLBAR_CONTROL_PARAMS.WM_CONTROL (in Toolbar Windows) - parameters
:dl tsize=3 break=all.
:dt.:hp2.id:ehp2. (USHORT)
:dd.Toolbar window identifier.

:dt.:hp2.usnotifycode:ehp2. (USHORT)
:dd.Notification code.
:p.The Toolbar window uses following notification codes&colon.
:dl tsize=7 break=all.
:dt.TN_AUTOSIZE
:dd.This notification is generated by TM_AUTOSIZE message and allows
to take control over automatic window positioning process.
:p.The :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. structure passed together with notification contains&colon.
Toolbar handle, parent window handle, suggested position, suggested size of
the Toolbar, attributes and current window status. During processing of the
TN_AUTOSIZE notification, it is possible to change target position
and size of the Toolbar by modification of the suggested size and position.
:p.:hp2.flcontrolspec:ehp2. contains pointer to the :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. structure.

:dt.TN_BEGINTRACK
:dd.This notification is send when dragging of the Toolbar begins.
:p.The :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink. structure passed together with notification allows change of
the area limiting possible range of dragging. The rclBoundary field is to be used
for this purpose.
:p.By use of the WinSetWindowPos call it is possible to change current window state
(SWP_RESTORE, SWP_MAXIMIZE, SWP_MINIMIZE). The rclSizePos, rclBoundary and hwndParent
fields of the :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink. structure shall be updated with values set by the
WinSetWindowPos call, otherwise results are unpredictable.
:p.The flState and flAttrs fields are informational. Change made to them makes
no effect on message processing.
:p.:hp2.flcontrolspec:ehp2. contains pointer to the :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink.
structure.

:dt.TN_BUTTONPAINT
:dd.This notification is generated by the Toolbar in response for the WM_COMMAND
message generated by objject of style TBO_STDBUTTON (the WC_BUTTON window
having a BS_OWNERDRAW style enabled).
:p.The BN_PAINT notification sent by buton is being translated into
TN_BUTTONPAINT code.
:p.:hp2.flcontrolspec:ehp2. contains pointer to the standard, system defined
USERBUTTON structure.

:dt.TN_ENDTRACK
:dd.This notification is generated just before finishing of a drag operation.
It allows to change a target position and size of the Toolbar. The
rclSizePos field of :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink. structure shall be updated. All remaining
fields of the :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink. structure are informational and it's modification
doesn't make any effect on message processing.
:p.:hp2.flcontrolspec:ehp2. contains pointer to the :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink. structure.

:dt.TN_TRACKING
:dd.This notification is generated during dragging the Toolbar.
:p.The flcontrolspec pointer points to the system defined POINTL structure
containing current position of the bottom left Toolbar corner.
Calculated position is relative to bottom left corner of parent window.
:p.When "Full Window Drag" option has been enabled, notification is generated
in response of every WM_MOUSEMOVE message. When this option is disabled,
and also in tha OS/2 Warp 3 system, notification is send directly
before finishing of window dragging, but before the TN_ENDTRACK notification.
:p.:hp2.flcontrolspec:ehp2. contains pointer to the POINTL structure.

:dt.TN_FIXTOOL, TN_MINIMIZE, TN_RESTORE
:dd.These notifications are send during execution of the WinSetWindowPos
system call, depending on control bytes passed by :hp1.fl:ehp1. parameter&colon.
:dl tsize=3 break=all.
:dt.TN_FIXTOOL
:dd.Generated during Toolbar docking, when the SWP_MAXIMIZE bit is set.
:dt.TN_MINIMIZE
:dd.Generated during Toolbar minimization, when the SWP_MINIMIZE bit is set.
:dt.TN_RESTORE
:dd.Generated when Toolbar is switching to "floating" state, when the
SWP_RESTORE bit is set.
:edl.
:p.If any of the SWP_MINIMIZE, SWP_MAXIMIZE or SWP_RESTORE bits is set, WinSetWindowPos
call ignores coordinates passed in it's parameters generating this notifications.
Invocations of WinSetWindowPos with other control bytes set don't cause
generation of these notification - user has full control over position
and size of the Toolbar.
:p.During processing or the notifications, Toolbar owner can change following
parameters using passed :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. structure:
:ul.
:li.handle of a parent window (hwndParent) using the WinSetParent call, which
if parent window has been changed to HWND_DESKTOP, allows to move Toolbar otside
of the application window borders.
:li.Target size and position by modification of rclSizePos structure.
Position shall be calculated relatively to parent window.
:li.Attributes, by modification of flAttrs bits.
:eul.
:p.The flState field of the :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. structure contains previous window state
(TST_FIXED, TST_MINIMIZED or TST_ROTATED). Thanks to that parameters of
a target state can depend on the previous one.
:edl.

:dt.:hp2.flcontrolspec:ehp2. (ULONG)
:dd.Control specific data.
:p.This field contains pointer to data structure, which depends on the
notification code
:link dependent reftype=hd refid=TOOLBAR_CONTROL_NOTIFY.usnotifycode:elink.&colon.
:dl tsize=3 break=all.
:dt.TN_AUTOSIZE, TN_FIXTOOL, TN_MINIMIZE, TN_RESTORE
:dd.Pointer to the :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. structure.
:dt.TN_BEGINTRACK, TN_ENDTRACK
:dd.Pointer to the :link reftype=hd refid=TOOLTRACK.TOOLTRACK:elink. structure.
:dt.TN_BUTTONPAINT
:dd.Pointer to the system defined USERBUTTON structure.
:dt.TN_TRACKING
:dd.Pointer to the system defined POINTL structure.
:edl.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TOOLBAR_CONTROL_RESULT.WM_CONTROL (in Toolbar Windows) - results
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (LONG)
:dd.Reserved value, equal 0.
:edl.


.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TOOLBAR_CONTROL_REMARK.WM_CONTROL (in Toolbar Windows) - remarks
:p.The Toolabr window always sends this message to the owner. Message
informs owner about changes of size, position, ownership being already about
to start, finish or in progress. Owner may change default Toolbar window behaviour
changing content of passed data structures, or issuing system calls allowed
during processing of particular notification.


.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TOOLBAR_CONTROL_DFPROC.WM_CONTROL (in Toolbar Windows) - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.ulReserved:ehp1. to 0.




:h2 name=WC_TOOLBAR_CTRL.Toolbar window's control messages
:p.This section describes the window procedure actions upon receiving the
following messages.

.* Automatic positioning
.*
:h2 group=1 scroll=none width=30% name=TM_AUTOSIZE.TM_AUTOSIZE
:link auto reftype=hd dependent refid=TM_AUTOSIZE_SYNTAX.
:link auto reftype=hd refid=TM_AUTOSIZE.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_AUTOSIZE_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_AUTOSIZE_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_AUTOSIZE_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_AUTOSIZE_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_AUTOSIZE_DFPROC.Default processing:elink.


.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_AUTOSIZE_SYNTAX.TM_AUTOSIZE - syntax
:p.This message returns maximal width of characters used to paint the text.
:p.:cgraphic.
param1
     ULONG  :link dependent reftype=hd refid=TM_AUTOSIZE_MAXLEN.ulMaxLength:elink.  // Maximum toolbar length

param2
     ULONG  :link dependent reftype=hd refid=TM_AUTOSIZE_RES.ulReserved:elink.   // Reserved value, equal to 0.
:ecgraphic.

.* Maximum length of the toolbar
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_AUTOSIZE_MAXLEN.TM_AUTOSIZE - ulMaxLength
:dl tsize=3 break=all.
:dt.:hp2.ulMaxLength:ehp2. (ULONG)
:dd.This parameter, specified in pixels, limits maximum length of toolbar during
automatic scaling. If set to 0, toolbar sizes itself to fit all objects placed
on it. If set to value other than 0, length of toolbar cannot exceed specified value -
object that doesn't fit within are clipped.
:p.The length is a width for horizontal toolbars and height for the vertical ones.
:edl.

.* Reserved value, equal to 0.
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_AUTOSIZE_RES.TM_AUTOSIZE - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_AUTOSIZE_PARAMS.TM_AUTOSIZE - parameters
:dl tsize=3 break=all.
:dt.:hp2.ulMaxLength:ehp2. (ULONG)
:dd.This parameter, specified in pixels, limits maximum length of toolbar during
automatic scaling. If set to 0, toolbar sizes itself to fit all objects placed
on it. If set to value other than 0, length of toolbar cannot exceed specified value -
object that doesn't fit within are clipped.
:p.The length is a width for horizontal toolbars and height for the vertical ones.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_AUTOSIZE_RESULT.TM_AUTOSIZE - results
:dl tsize=3 break=all.
:dt.TRUE
:dd.Size of toolbar changed.
:dt.FALSE
:dd.Size of toolbar didn't change.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_AUTOSIZE_REMARK.TM_AUTOSIZE - remarks
:p.Respone upon this message depends on current Toolbar state. Positioning
takes place according to the following rules&colon.
:ul.
:li.If toolbar is minimized or docked (maximized) it is automatically set
relatively to parent window depending on the following attributes&colon.
:dl tsize=3 break=all.
:dt.TBA_FIXEDTOP
:dd.Horizontal toolbar is aligned to the left along with parent window top edge.
:dt.TBA_FIXEDLEFT
:dd.Vertical toolbar is aligned to the top along with parent window left edge.
:dt.TBA_FIXEDBOTTOM
:dd.Horizontal toolbar is aligned to the left along with parent window bottom edge.
:dt.TBA_FIXEDRIGHT
:dd.Vertical toolbar is aligned to the top along with right parent window edge.
:edl.
:li.In the floating state, position of the Toolbar doesn't change.
:eul.
:p.Resizing takes place according to the following rules&colon.
:ul.
:li.When Toolbar is docked (maximized) it is stretched to fit entire length
of the parent window.
:li.When Toolbar is minimized, it's size is decreased to fit only the title bar.
:li.When Toolbar is in the floating state, its size is adjusted to fit
all object placed on it. The :hp1.ulMaxLength:ehp1. parameters allows to
limit the toolbar size in case it contains as many objects causing toolbar
exceeding a total available size.
:p.The :hp1.ulMaxLength:ehp1. parameter is ignored when set to 0.
:eul.
:p.The toolbar sends the TM_AUTOSIZE notification to the owner, which allows
him to control positioning and resizing process.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_AUTOSIZE_DFPROC.TM_AUTOSIZE - default processing
:p.The default window procedure takes no action on this message, other than
to set result to FALSE.




.* Calc proposed size of toolbar
.*
:h2 group=1 scroll=none width=30% name=TM_CALCNEWRECT.TM_CALCNEWRECT
:link auto reftype=hd dependent refid=TM_CALCNEWRECT_SYNTAX.
:link auto reftype=hd refid=TM_CALCNEWRECT.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_CALCNEWRECT_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_CALCNEWRECT_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_CALCNEWRECT_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_CALCNEWRECT_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_CALCNEWRECT_DFPROC.Default processing:elink.


.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_CALCNEWRECT_SYNTAX.TM_CALCNEWRECT - syntax
:p.This message calculates suggested size and position of the toolbar.
:p.:cgraphic.
param1
     :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. :link dependent reftype=hd refid=TM_CALCNEWRECT_CALCPOS.adjNewRect:elink.  // Calculated size and position

param2
     ULONG   :link dependent reftype=hd refid=TM_CALCNEWRECT_RES.ulReserved:elink.  // Reserved value, equal to 0.
:ecgraphic.

.* Calculated size and position
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_CALCNEWRECT_CALCPOS.TM_CALCNEWRECT - adjNewRect
:dl tsize=3 break=all.
:dt.:hp2.adjNewRect:ehp2. (:link reftype=hd refid=TOOLADJ.TOOLADJ:elink.)
:dd.On input the :hp1.flState:ehp1. and :hp1.flAttrs:ehp1. shall be set.
On output it contains calculated size and position.
:edl.

.* Reserved value, equal to 0.
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_CALCNEWRECT_RES.TM_CALCNEWRECT - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_CALCNEWRECT_PARAMS.TM_CALCNEWRECT - parameters
:dl tsize=3 break=all.
:dt.:hp2.adjNewRect:ehp2. (:link reftype=hd refid=TOOLADJ.TOOLADJ:elink.)
:dd.On input the :hp1.flState:ehp1. and :hp1.flAttrs:ehp1. shall be set.
On output it contains calculated size and position.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_CALCNEWRECT_RESULT.TM_CALCNEWRECT - results
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_CALCNEWRECT_REMARK.TM_CALCNEWRECT - remarks
:p.Message allows to calculate suggested size and position of the Toolbar based
upon initial content of the :hp1.adjNewRect:ehp1. structure. Following fields
or the :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. structure are to be initialized before sending of this message&colon.
:dl tsize=3 break=all.
:dt.:hp2.flStat:ehp2.
:dd.The selected TST_* flags describing queried state of the Toolbar are to be set.
:dt.:hp2.flAttrs:ehp2.
:dd.The selected TBA_* flags describing queried state of the Toolbar are to be set.
:edl.
:p.:p.As response, the :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. structure has following fields filled&colon.
:dl tsize=3 break=all.
:dt.:hp2.hwndToolBar:ehp2.
:dd.Window handle of the toolbar.
:dt.:hp2.hwndParent:ehp2.
:dd.Current handle of parent window (it could be changed by WinSetParent),
:dt.:hp2.rclSizePos:ehp2.
:dd.New posiion and size of the Toolbar, calculated relatively to parent window.
:edl.
:p.Message makes positioning of the Toolbar easier during processing of the
TN_FIXTOOL, TN_MINIMIZE and TN_RESTORE notifications. Expected position and size
after modification of the :hp1.flAttrs:ehp1. and :hp1.flState:ehp1. attributes
can be obtained, optionally modified and loaded into :link reftype=hd refid=TOOLADJ.TOOLADJ:elink. structure being available
during processing of these notifications.
:p.Toolbar sends this message itself during minimization, docking (maximization) and
switching to the floating state.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_CALCNEWRECT_DFPROC.TM_CALCNEWRECT - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.ulReserved:ehp1. to default value of 0.




.* Delete object
.*
:h2 group=1 scroll=none width=30% name=TM_DELETEOBJECT.TM_DELETEOBJECT
:link auto reftype=hd dependent refid=TM_DELETEOBJECT_SYNTAX.
:link auto reftype=hd refid=TM_DELETEOBJECT.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_DELETEOBJECT_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_DELETEOBJECT_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_DELETEOBJECT_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_DELETEOBJECT_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_DELETEOBJECT_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_DELETEOBJECT_SYNTAX.TM_DELETEOBJECT - syntax
:p.This message removes object (window) with specified identifier.
:p.:cgraphic.
param1
     ULONG  :link dependent reftype=hd refid=TM_DELETEOBJECT_OBJ.objId:elink.       // Object identifier

param2
     ULONG  :link dependent reftype=hd refid=TM_DELETEOBJECT_RES.ulReserved:elink.  // Reserved value, equal to 0.
:ecgraphic.

.* Object identifier
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_DELETEOBJECT_OBJ.TM_DELETEOBJECT - objId
:dl tsize=3 break=all.
:dt.:hp2.objId:ehp2. (ULONG)
:dd.Identifier of an object placed on the Toolbar (child control window).
:edl.

.* Reserved value, equal to 0.
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_DELETEOBJECT_RES.TM_DELETEOBJECT - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_DELETEOBJECT_PARAMS.TM_DELETEOBJECT - parameters
:dl tsize=3 break=all.
:dt.:hp2.objId:ehp2. (ULONG)
:dd.Identifier of an object placed on the Toolbar (child control window).
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_DELETEOBJECT_RESULT.TM_DELETEOBJECT - results
:dl tsize=3 break=all.
:dt.:hp2.ulStatus:ehp2. (BOOL)
:dd.Exit status:
:dl tsize=3 break=all compact.
:dt.FALSE
:dd.Object removal failed.
:dt.TRUE
:dd.Object has been successfully removed.
:edl.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_DELETEOBJECT_REMARK.TM_DELETEOBJECT - remarks
:p.Toolbar responds to this message removing specified object (child control window).

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_DELETEOBJECT_DFPROC.TM_DELETEOBJECT - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.ulStatus:ehp1. to default value of 0.




.* Enable/disable rotate button
.*
:h2 group=1 scroll=none width=30% name=TM_ENABLEROTATE.TM_ENABLEROTATE
:link auto reftype=hd dependent refid=TM_ENABLEROTATE_SYNTAX.
:link auto reftype=hd refid=TM_ENABLEROTATE.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_ENABLEROTATE_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_ENABLEROTATE_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_ENABLEROTATE_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_ENABLEROTATE_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_ENABLEROTATE_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_ENABLEROTATE_SYNTAX.TM_ENABLEROTATE - syntax
:p.This message returns total window height in pixels.
:p.:cgraphic.
param1
     BOOL   :link dependent reftype=hd refid=TM_ENABLEROTATE_ENABLE.bEnable:elink.     // Enable/disable rotate button flag

param2
     ULONG  :link dependent reftype=hd refid=TM_ENABLEROTATE_RES.ulReserved:elink.  // Reserved value, equal to 0.
:ecgraphic.

.* Reserved value, equal to 0.
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_ENABLEROTATE_ENABLE.TM_ENABLEROTATE - bEnable
:dl tsize=3 break=all.
:dt.:hp2.bEnable:ehp2. (BOOL)
:dd.Enable/disable rotate button flag. If set to TRUE, rotate button becomes visible
and ready to work. If set to FALSE, rotate button is removed.
:edl.

.* Reserved value, equal to 0.
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_ENABLEROTATE_RES.TM_ENABLEROTATE - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_ENABLEROTATE_PARAMS.TM_ENABLEROTATE - parameters
:dl tsize=3 break=all.
:dt.:hp2.bEnable:ehp2. (BOOL)
:dd.Enable/disable rotate button flag. If set to TRUE, rotate button becomes visible
and ready to work. If set to FALSE, rotate button is removed.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_ENABLEROTATE_RESULT.TM_ENABLEROTATE - results
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (LONG)
:dd.Reserved value, always 0.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_ENABLEROTATE_REMARK.TM_ENABLEROTATE - remarks
:p.Message switches Toolbar rotate button between visible (active) and invisible
(inactive) state.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_ENABLEROTATE_DFPROC.TM_ENABLEROTATE - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.ulReserved:ehp1. to default value of 0.



.* Query object identifier by position
.*
:h2 group=1 scroll=none width=30% name=TM_IDFROMPOSITION.TM_IDFROMPOSITION
:link auto reftype=hd dependent refid=TM_IDFROMPOSITION_SYNTAX.
:link auto reftype=hd refid=TM_IDFROMPOSITION.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_IDFROMPOSITION_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_IDFROMPOSITION_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_IDFROMPOSITION_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_IDFROMPOSITION_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_IDFROMPOSITION_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_IDFROMPOSITION_SYNTAX.TM_IDFROMPOSITION - syntax
:p.This message returns current window's enable/disable state.
:p.:cgraphic.
param1
     LONG   :link dependent reftype=hd refid=TM_IDFROMPOSITION_OBJPOS.lObjPos:elink.     // Queried object position

param2
     ULONG  :link dependent reftype=hd refid=TM_IDFROMPOSITION_RES.ulReserved:elink.  // Reserved value, equal to 0.
:ecgraphic.

.* Object's position
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_IDFROMPOSITION_OBJPOS.TM_IDFROMPOSITION - lObjPos
:dl tsize=3 break=all.
:dt.:hp2.lObjPos:ehp2. (ULONG)
:dd.Queried object position, taken from the beginning of the Toolbar (0 - leftmost
position for horizontal Toolbars or topmost position for the vartical ones).
:edl.

.* Reserved value, equal to 0.
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_IDFROMPOSITION_RES.TM_IDFROMPOSITION - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_IDFROMPOSITION_PARAMS.TM_IDFROMPOSITION - parameters
:dl tsize=3 break=all.
:dt.:hp2.lObjPos:ehp2. (ULONG)
:dd.Queried object position, taken from the beginning of the Toolbar (0 - leftmost
position for horizontal Toolbars or topmost position for the vartical ones).
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved value, should be 0.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_IDFROMPOSITION_RESULT.TM_IDFROMPOSITION - results
:dl tsize=3 break=all.
:dt.:hp2.lObjId:ehp2. (BOOL)
:dd.Returned object identifier.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_IDFROMPOSITION_REMARK.TM_IDFROMPOSITION - remarks
:p.Message reads identifier of object located at the specified position.
TB_END constant can be specified as :hp1.param1:ehp1., which will read
identifier of a last object.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_IDFROMPOSITION_DFPROC.TM_IDFROMPOSITION - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.lObjId:ehp1. to default value of 0.



.* Insert object
.*
:h2 group=1 scroll=none width=30% name=TM_INSERTOBJECT.TM_INSERTOBJECT
:link auto reftype=hd dependent refid=TM_INSERTOBJECT_SYNTAX.
:link auto reftype=hd refid=TM_INSERTOBJECT.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_INSERTOBJECT_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_INSERTOBJECT_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_INSERTOBJECT_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_INSERTOBJECT_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_INSERTOBJECT_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_INSERTOBJECT_SYNTAX.TM_INSERTOBJECT - syntax
:p.This message returns total window width in pixels.
:p.:cgraphic.
param1
     :link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink. :link dependent reftype=hd refid=TM_INSERTOBJECT_OBJ.toolObj:elink.  // Definition of new object

param2
     :link reftype=hd refid=TOOLWIN.TOOLWIN:elink. :link dependent reftype=hd refid=TM_INSERTOBJECT_WIN.toolWin:elink.  // Attributes of new window
:ecgraphic.

.* Definition of new object
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_INSERTOBJECT_OBJ.TM_INSERTOBJECT - toolObj
:dl tsize=3 break=all.
:dt.:hp2.toolObj:ehp2. (:link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink.)
:dd.Pointer to structure containing definition of created object.
:edl.

.* Attributes of new window
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_INSERTOBJECT_WIN.TM_INSERTOBJECT - toolWin
:dl tsize=3 break=all.
:dt.:hp2.toolWin:ehp2. (:link reftype=hd refid=TOOLWIN.TOOLWIN:elink.)
:dd.Pointer to supplementary structure describing child window created on
the Toolbar.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_INSERTOBJECT_PARAMS.TM_INSERTOBJECT - parameters
:dl tsize=3 break=all.
:dt.:hp2.toolObj:ehp2. (:link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink.)
:dd.Pointer to structure containing definition of created object.
:dt.:hp2.toolWin:ehp2. (:link reftype=hd refid=TOOLWIN.TOOLWIN:elink.)
:dd.Pointer to supplementary structure describing child window created on
the Toolbar.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_INSERTOBJECT_RESULT.TM_INSERTOBJECT - results
:dl tsize=3 break=all.
:dt.:hp2.bStatus:ehp2. (BOOL)
:dd.Status of object insertion&colon.
:dl tsize=3 break=all compact.
:dt.FALSE
:dd.Object has not been added to the Toolbar.
:dt.TRUE
:dd.Successful completion.
:edl.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_INSERTOBJECT_REMARK.TM_INSERTOBJECT - remarks
:p.Message adds an object to the Toolbar. Main description of the object
shall be placed in the :link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink. structure. Pointer to this structure is to be
passed using message :hp1.param1:ehp1.
:p.Objects other than separators (a child control windows) require additional
:link reftype=hd refid=TOOLWIN.TOOLWIN:elink. structure containing description of window that is going to be created.
:p.Object creation process depends on options passed in the :hp1.flAttribute:ehp1.
field of the :link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink. structure&colon.

:dl tsize=3 break=all.
:dt.TBO_STDWINDOW
:dd.Causes control window to be created with parameters described in the
:link reftype=hd refid=TOOLWIN.TOOLWIN:elink. structure, which contains&colon.
:ul compact.
:li.Window class.
:li.Window text.
:li.Window style.
:li.Pointer to it's class specific data.
:li.Pointer to it's presentation parameters.
:eul.

:dt.TBO_BUTTON
:dd.Creates a button fully supported by the Toolbar. Created child window should
behave as window of the WC_WINBUTTON class, it must respond to the following system
messages&colon. BM_SETHILITE, BM_QUERYHILITE, and additional nonstandard&colon.
BM_QUERYWIDTH and BM_QUERYHEIGHT, which are used for autmatic buttons resizing.

:dt.TBO_STDBUTTON
:dd.Creates a button partially supported by the Toolbar. Support dffer from TBO_BUTTON
only that tolbar doesn't send BM_QUERYWIDTH and BM_QUERYHEIGHT messages, thus
button scalling must be performed by a programmer. This style has been provided
to operate with standard buttons (WC_BUTTON) created with BS_PUSHBUTTON style.

:dt.TBO_SEPARATOR
:dd.Creates a separator. Separator is not a window but an object drawn by Toolbar itself.
Therefore it doesn't require additional :link reftype=hd refid=TOOLWIN.TOOLWIN:elink. structure and thus doesn't
have a window handle.
:edl.
:p.The Toolbar's TCID_OBJECTS child window becomes parent of newly created window.
The TCID_OBJECTS window holds, controls and redraws all objects placed on
the Toolbar. Owner of the Toolbar always becomes owner of newly created control
window.

:p.:p.Object being added can be positioned in the following ways&colon.
:dl tsize=3 break=all.
:dt.TBO_BEGINALIGN
:dd.Object is aligned to the beginning  of the Toolbar (on the left or top).
:dt.TBO_ENDALIGN
:dd.Object is aligned ti the end (on the right or bottom).
:dt.TBO_CENTERALIGN
:dd.Object is centered between object adde with TBO_BEGINALIGN and TBO_ENDALIGN flags.
:edl.
:p.Adding new object, in order to keep consistency, may cause alignment change
of previously added object:
:ul.
:li.Object with TBO_BEGINALIGN applies it's style to all object located on it's
left side making them left (top) aligned.
:li.Object with TBO_ENDALIGN applies it's style to all object located on it's
right side making them right (bottom) aligned.
:li.Object with TBO_CENTERALIGN uses more tricky rules. All objects on it's right side
with TBO_BEGINALIGN style are centered. Similarily, all objects on it's left side
with TBO_ENDALIGN style are also centered.
:eul.
:p.These rules allows to keep consistency in objects alignment&colon. left aligned
objects are first, then centered, and right aligned at the end.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_INSERTOBJECT_DFPROC.TM_INSERTOBJECT - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.bStatus:ehp1. to default value of 0.



.* Insert tool tip
.*
:h2 group=1 scroll=none width=30% name=TM_INSERTTOOLTIP.TM_INSERTTOOLTIP
:link auto reftype=hd dependent refid=TM_INSERTTOOLTIP_SYNTAX.
:link auto reftype=hd refid=TM_INSERTTOOLTIP.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_INSERTTOOLTIP_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_INSERTTOOLTIP_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_INSERTTOOLTIP_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_INSERTTOOLTIP_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_INSERTTOOLTIP_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_INSERTTOOLTIP_SYNTAX.TM_INSERTTOOLTIP - syntax
:p.This message sets/removes a tooltip for an object.
:p.:cgraphic.
param1
     ULONG  :link dependent reftype=hd refid=TM_INSERTTOOLTIP_OBJ.ulObjId:elink.     // Object identifier

param2
     PSZ    :link dependent reftype=hd refid=TM_INSERTTOOLTIP_TEXT.szTipText:elink.   // Tooltip text
:ecgraphic.

.* Object identifier
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_INSERTTOOLTIP_OBJ.TM_INSERTTOOLTIP - ulObjId
:dl tsize=3 break=all.
:dt.:hp2.ulObjId:ehp2. (ULONG)
:dd.Identifier of objects for whcich a tooltip is to be added.
:edl.

.* Tooltip text
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_INSERTTOOLTIP_TEXT.TM_INSERTTOOLTIP - szTipText
:dl tsize=3 break=all.
:dt.:hp2.szTipText:ehp2. (PSZ)
:dd.Pointer to tooltip text which is to be displayed over object.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_INSERTTOOLTIP_PARAMS.TM_INSERTTOOLTIP - parameters
:dl tsize=3 break=all.
:dt.:hp2.ulObjId:ehp2. (ULONG)
:dd.Identifier of objects for whcich a tooltip is to be added.
:dt.:hp2.szTipText:ehp2. (PSZ)
:dd.Pointer to tooltip text which is to be displayed over object.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_INSERTTOOLTIP_RESULT.TM_INSERTTOOLTIP - results
:dl tsize=3 break=all.
:dt.:hp2.bStatus:ehp2. (BOOL)
:dd.Status of tooltip insertion&colon.
:dl tsize=3 break=all compact.
:dt.FALSE
:dd.Tool tip has not been added to the Toolbar.
:dt.TRUE
:dd.Successful completion.
:edl.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_INSERTTOOLTIP_REMARK.TM_INSERTTOOLTIP - remarks
:p.Message adds or removes tooltip for object with specified identifier located
on Toolbar. The :hp1.param2:ehp1. shall contain pointer to the ASCIIZ string
containig a tip. Specifying the NULL pointer causes tooltip to be removed.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_INSERTTOOLTIP_DFPROC.TM_INSERTTOOLTIP - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.ulStatus:ehp1. to default value of 0.




.* TM_MOVEOBJECT - Move object
.* ***************************
.*
:h2 group=1 scroll=none width=30% name=TM_MOVEOBJECT.TM_MOVEOBJECT
:link auto reftype=hd dependent refid=TM_MOVEOBJECT_SYNTAX.
:link auto reftype=hd refid=TM_MOVEOBJECT.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_MOVEOBJECT_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_MOVEOBJECT_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_MOVEOBJECT_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_MOVEOBJECT_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_MOVEOBJECT_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_MOVEOBJECT_SYNTAX.TM_MOVEOBJECT - syntax
:p.Message moves object located on the Toolbar.
:p.:cgraphic.
param1
     SHORT1 :link dependent reftype=hd refid=TM_MOVEOBJECT_OBJ.usObjId:elink.   // Identifier of moved object
     SHORT2 :link dependent reftype=hd refid=TM_MOVEOBJECT_POS.usNewPos:elink.  // Target position of the object

param2
     ULONG  :link dependent reftype=hd refid=TM_MOVEOBJECT_ATTR.ulAttrs:elink.   // New positioning attributes
:ecgraphic.

.* Object identifier
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_MOVEOBJECT_OBJ.TM_MOVEOBJECT - usObjId
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Object identifier.
:edl.

.* Target position
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_MOVEOBJECT_POS.TM_MOVEOBJECT - usNewPos
:dl tsize=3 break=all.
:dt.:hp2.usNewPos:ehp2. (USHORT)
:dd.Target position of object being moved, calculated from the beginning of the Toolbar.
0 means first position.
:edl.

.* Attributes
.*
:h2 group=12 x=30% y=bottom width=70% height=225x hide name=TM_MOVEOBJECT_ATTR.TM_MOVEOBJECT - ulAttrs
:dl tsize=3 break=all.
:dt.:hp2.ulAttrs:ehp2. (ULONG)
:dd.Attributes describing object alignment at target position.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_MOVEOBJECT_PARAMS.TM_MOVEOBJECT - parameters
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Object identifier.
:dt.:hp2.usNewPos:ehp2. (USHORT)
:dd.Target position of object being moved, calculated from the beginning of the
Toolbar. 0 means first position.
:dt.:hp2.ulAttrs:ehp2. (ULONG)
:dd.Attributes describing object alignment at target position.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_MOVEOBJECT_RESULT.TM_MOVEOBJECT - results
:dl tsize=3 break=all.
:dt.:hp2.bStatus:ehp2. (BOOL)
:dd.Status of object insertion&colon.
:dl tsize=3 break=all compact.
:dt.FALSE
:dd.Failure.
:dt.TRUE
:dd.Successful completion.
:edl.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_MOVEOBJECT_REMARK.TM_MOVEOBJECT - remarks
:p.Message allows to move object from current position to new, calculated
from the beginning of the Toolabr. Following alignment attributes are accepted:
TBO_BEGINALIGN, TBO_ENDALIGN and TBO_CENTERALIGN. Current alignment is retained
if :hp1.flAttrs:ehp1. is set to 0. For more information about alignment
see remarks for :link reftype=hd refid=TM_INSERTOBJECT.TM_INSERTOBJECT:elink..

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_MOVEOBJECT_DFPROC.TM_MOVEOBJECT - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.bStatus:ehp1. to default value of 0.




.* TM_POSITIONFROMID - Obtain object position from identifier
.* **********************************************************
.*
:h2 group=1 scroll=none width=30% name=TM_POSITIONFROMID.TM_POSITIONFROMID
:link auto reftype=hd dependent refid=TM_POSITIONFROMID_SYNTAX.
:link auto reftype=hd refid=TM_POSITIONFROMID.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_POSITIONFROMID_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_POSITIONFROMID_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_POSITIONFROMID_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_POSITIONFROMID_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_POSITIONFROMID_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_POSITIONFROMID_SYNTAX.TM_POSITIONFROMID - syntax
:p.Message obtains object position form it's identifier.
:p.:cgraphic.
param1
     SHORT  :link dependent reftype=hd refid=TM_POSITIONFROMID_OBJ.usObjId:elink.     // Object identifier.

param2
     ULONG  :link dependent reftype=hd refid=TM_POSITIONFROMID_RES.ulReserved:elink.  // Reserved, always 0.
:ecgraphic.

.* Object identifier
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_POSITIONFROMID_OBJ.TM_POSITIONFROMID - usObjId
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Identifier of queried object.
:edl.

.* Reserved value, equal to 0.
.*
:h2 group=12 x=30% y=bottom width=70% height=225x hide name=TM_POSITIONFROMID_RES.TM_POSITIONFROMID - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_POSITIONFROMID_PARAMS.TM_POSITIONFROMID - parameters
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Identifier of queried object.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_POSITIONFROMID_RESULT.TM_POSITIONFROMID - results
:dl tsize=3 break=all.
:dt.:hp2.lPosition:ehp2. (LONG)
:dd.Current object's position&colon.
:dl tsize=3 break=all compact.
:dt.-1
:dd.Error - object doesn't exist.
:dt.>=0
:dd.Returned object's position.
:edl.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_POSITIONFROMID_REMARK.TM_POSITIONFROMID - remarks
:p.Message reads current position of a specified object, calculated from
the beginning of the Tollbar (it is actually an ordinal number). If object
with specified identifier doesn't exist -1 is returned.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_POSITIONFROMID_DFPROC.TM_POSITIONFROMID - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.lPosition:ehp1. to default value of 0.




.* TM_QUERYHANDLEQ - uery object's window handle
.* *********************************************
.*
:h2 group=1 scroll=none width=30% name=TM_QUERYHANDLE.TM_QUERYHANDLE
:link auto reftype=hd dependent refid=TM_QUERYHANDLE_SYNTAX.
:link auto reftype=hd refid=TM_QUERYHANDLE.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_QUERYHANDLE_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_QUERYHANDLE_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_QUERYHANDLE_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_QUERYHANDLE_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_QUERYHANDLE_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_QUERYHANDLE_SYNTAX.TM_QUERYHANDLE - syntax
:p.Message obtains object's window handle.
:p.:cgraphic.
param1
     SHORT  :link dependent reftype=hd refid=TM_QUERYHANDLE_OBJ.usObjId:elink.     // Object identifier.

param2
     ULONG  :link dependent reftype=hd refid=TM_QUERYHANDLE_RES.ulReserved:elink.  // Reserved, always 0.
:ecgraphic.

.* Object identifier
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_QUERYHANDLE_OBJ.TM_QUERYHANDLE - usObjId
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Identifier of queried object.
:edl.

.* Reserved value, equal to 0.
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_QUERYHANDLE_RES.TM_QUERYHANDLE - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_QUERYHANDLE_PARAMS.TM_QUERYHANDLE - parameters
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Identifier of queried object.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_QUERYHANDLE_RESULT.TM_QUERYHANDLE - results
:dl tsize=3 break=all.
:dt.:hp2.lPosition:ehp2. (LONG)
:dd.Current object's position&colon.
:dl tsize=3 break=all compact.
:dt.NULLHANDLE
:dd.Readout error, object doesn't exist or is a separator.
:dt.<>NULLHANDLE
:dd.Returned object's window handle.
:edl.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_QUERYHANDLE_REMARK.TM_QUERYHANDLE - remarks
:p.Message reads object's window handle.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_QUERYHANDLE_DFPROC.TM_QUERYHANDLE - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.lPosition:ehp1. to default value of 0.




.* TM_QUERYHEIGHT - Query window height
.* ************************************
.*
:h2 group=1 scroll=none width=30% name=TM_QUERYHEIGHT.TM_QUERYHEIGHT
:link auto reftype=hd dependent refid=TM_QUERYHEIGHT_SYNTAX.
:link auto reftype=hd refid=TM_QUERYHEIGHT.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_QUERYHEIGHT_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_QUERYHEIGHT_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_QUERYHEIGHT_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_QUERYHEIGHT_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_QUERYHEIGHT_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_QUERYHEIGHT_SYNTAX.TM_QUERYHEIGHT - syntax
:p.Message queries Toolbar height required to hold all objects without clipping.
:p.:cgraphic.
param1
     ULONG  :link dependent reftype=hd refid=TM_QUERYHEIGHT_OPT.flOptions:elink.    // Options

param2
     ULONG  :link dependent reftype=hd refid=TM_QUERYHEIGHT_MAX.ulMaxLength:elink.  // Maximum possible length of toolbar
:ecgraphic.

.* Options
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_QUERYHEIGHT_OPT.TM_QUERYHEIGHT - flOptions
:dl tsize=3 break=all.
:dt.:hp2.flOptions:ehp2. (ULONG)
:dd.These options determine type of returned information:
:dl tsize=3 break=all.
:dt.TDQ_RESTORED
:dd.Full Toolbar height in the floating state.
:dt.TDQ_FIXED	
:dd.Height in the maximized (docked) state.
:dt.TDQ_MINIMIZED
:dd.Height in the minimized state.
:edl.
:edl.

.* Maximum length of the toolbar
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_QUERYHEIGHT_MAX.TM_QUERYHEIGHT - ulMaxLength
:dl tsize=3 break=all.
:dt.:hp2.ulMaxLength:ehp2. (ULONG)
:dd.Expected, maximum possible length of the Toolbar. With TBS_MULTILINE style set,
width of the Toolbar may cause effects on it's height. Ignored if set to 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_QUERYHEIGHT_PARAMS.TM_QUERYHEIGHT - parameters
:dl tsize=3 break=all.
:dt.:hp2.flOptions:ehp2. (ULONG)
:dd.These options determine type of returned information:
:dl tsize=3 break=all.
:dt.TDQ_RESTORED
:dd.Full Toolbar height in the floating state.
:dt.TDQ_FIXED	
:dd.Height in the maximized (docked) state.
:dt.TDQ_MINIMIZED
:dd.Height in the minimized state.
:edl.
:dt.:hp2.ulMaxLength:ehp2. (ULONG)
:dd.Expected, maximum possible length of the Toolbar. With TBS_MULTILINE style set,
width of the Toolbar may cause effects on it's height. Ignored if set to 0.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_QUERYHEIGHT_RESULT.TM_QUERYHEIGHT - results
:dl tsize=3 break=all.
:dt.:hp2.lHeight:ehp2. (LONG)
:dd.Expected height of the Toolbar, calculated for specified parameters.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_QUERYHEIGHT_REMARK.TM_QUERYHEIGHT - remarks
:p.Message reads height of the Toolbar required to hold all objects without clipping.
Toolbar window procedure reads current heights of all objects placed on it
(object sizes are set automatically during adding and removing them). Further
calculations depends on specified toolbar state&colon.
:ul.
:li.In case of the horizontal Toolbar, dimension of a highest object is taken,
increased by height of a border and then returned. With :hp1.ulMaxLength:ehp1.
parameter specified, toolbar with TBS_MULTILINE style calculates
height taking into account possible wrapping of objects that doesn't fit
within specified :hp1.ulMaxLength:ehp1..

:li.In csae of the vertical Toolbar, object's heights are added together (single
pixel space between objects is also included) next, heights of control objects
are added (title bar, rotate button and separators). If calculated height exceeds
:hp1.ulMaxLength:ehp1. it is truncated to :hp1.ulMaxLength:ehp1.. Final result
increased by border height is returned.

:li.If TDQ_MINIMIZED option has been specified, horizontal toolbar returns constant
height of the title bar increased by a border height.
.br
For minimized vertical Toolbar, message returns it's width before minimization,
increased by 50%.
:eul.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_QUERYHEIGHT_DFPROC.TM_QUERYHEIGHT - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.lHeight:ehp1. to default value of 0.




.* TM_QUERYOBJECT - Query information about an object
.* **************************************************
.*
:h2 group=1 scroll=none width=30% name=TM_QUERYOBJECT.TM_QUERYOBJECT
:link auto reftype=hd dependent refid=TM_QUERYOBJECT_SYNTAX.
:link auto reftype=hd refid=TM_QUERYOBJECT.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_QUERYOBJECT_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_QUERYOBJECT_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_QUERYOBJECT_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_QUERYOBJECT_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_QUERYOBJECT_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_QUERYOBJECT_SYNTAX.TM_QUERYOBJECT - syntax
:p.Message queries Toolbar for object's control structures.
:p.:cgraphic.
param1
     USHORT  :link dependent reftype=hd refid=TM_QUERYOBJECT_OBJ.usObjId:elink.  // Object identifier

param2
     :link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink. :link dependent reftype=hd refid=TM_QUERYOBJECT_INFO.objInfo:elink.  // Object information structure
:ecgraphic.

.* Object identifier
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_QUERYOBJECT_OBJ.TM_QUERYOBJECT - usObjId
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Object identifier.
:edl.

.* Object information structure
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_QUERYOBJECT_INFO.TM_QUERYOBJECT - objInfo
:dl tsize=3 break=all.
:dt.:hp2.objInfo:ehp2. (:link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink.)
:dd.Object information structure.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_QUERYOBJECT_PARAMS.TM_QUERYOBJECT - parameters
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Object identifier.
:dt.:hp2.objInfo:ehp2. (:link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink.)
:dd.Object information structure.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_QUERYOBJECT_RESULT.TM_QUERYOBJECT - results
:dl tsize=3 break=all.
:dt.:hp2.bStatus:ehp2. (BOOL)
:dd.Status of object information readout&colon.
:dl tsize=3 break=all compact.
:dt.FALSE
:dd.Readout error, object doesn't exists or it is a separator.
:dt.TRUE
:dd.Object informaton successfully read.
:edl.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_QUERYOBJECT_REMARK.TM_QUERYOBJECT - remarks
:p.Message reads control structures for a specified object, containing followind
date&colon.
:ul compact.
:li.object's position,
:li.group membership,
:li.attributes,
:li.size,
:li.identifier,
:li.and user's private data.
:eul.
:p.Size read may differ from real object size on the Toolbar. In case of
horizontal toolbar, object width is always correct, but it's height can be
greater than real because Toolbar adjust correct object's heights itself.
:p.Similar situation occurs in case of vertical Toolbar, where height of object
is always correct, but width may differ from real.
:p.Data structure read, contains initial object size set during it's creation.
To read real object size, use the following code&colon.
:cgraphic.
hwnd = (HWND)WinSendMsg(hwndToolbar, TM_QUERYHANDLE, (MPARAM)objId, 0);
WinQueryWindowRect(hwnd, &amp.rect);
:ecgraphic.

:p.The rect structure will contain real size of an object. It doesn't work
for separators, because they are not a windows. In this case&colon.
:ul.
:li.Separator thickness is always equal to 5 pixels.
:li.Separator length is equal to width of Toolbar child window with
TCID_OBJECTPAD identifier. Actual size of separator can be read in the
following way&colon.
:cgraphic.
// Read separator's parameters
WinSendMsg(hwndToolbar, TM_QUERYOBJECT, separatorId, (MPARAM)&amp.objInfo);
// Read window handle of window holding object
hwndObjPad = WinWindowFromID(hwndToolbar, TCID_OBJECTPAD);
// Read size of that window
WinQueryWindowRect(hwndObjPad, &amp.wsize);

// Determine size of separator
state = (LONG)WinSendMsg(hwndToolbar, TM_QUERYSTATE, 0, 0);
if(state &amp. TST_ROTATED)
{ cx = wsize.xRight - wsize.xLeft;
  cy = objInfo.cy;
}
else
{ cx = objInfo.cx;
  cy = wsize.yTop - wsize.yBottom;
}
:ecgraphic.
:p.The cx and cy variables contain separator size.
:eul.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_QUERYOBJECT_DFPROC.TM_QUERYOBJECT - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.lPosition:ehp1. to default value of 0.




.* TM_QUERYSTATE - Query current state of the Toolbar
.* **************************************************
.*
:h2 group=1 scroll=none width=30% name=TM_QUERYSTATE.TM_QUERYSTATE
:link auto reftype=hd dependent refid=TM_QUERYSTATE_SYNTAX.
:link auto reftype=hd refid=TM_QUERYSTATE.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_QUERYSTATE_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_QUERYSTATE_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_QUERYSTATE_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_QUERYSTATE_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_QUERYSTATE_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_QUERYSTATE_SYNTAX.TM_QUERYSTATE - syntax
:p.Message queries current Toolbar state.
:p.:cgraphic.
param1
     ULONG :link dependent reftype=hd refid=TM_QUERYSTATE_RES.ulReserved:elink.  // Reserved, should be 0.

param2
     ULONG :link dependent reftype=hd refid=TM_QUERYSTATE_RES.ulReserved:elink.  // Reserved, should be 0.
:ecgraphic.

.* Reserved, should be 0.
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_QUERYSTATE_RES.TM_QUERYSTATE - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_QUERYSTATE_PARAMS.TM_QUERYSTATE - parameters
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_QUERYSTATE_RESULT.TM_QUERYSTATE - results
:dl tsize=3 break=all.
:dt.:hp2.ulState:ehp2. (ULONG)
:dd.Current state of the toolbar&colon.
:dl tsize=3 break=all compact.
:dt.TST_MINIMIZED
:dd.Toolbar is in the minimized state.
:dt.TST_FIXED
:dd.Toolbar is in the maximized (docked) state.
:dt.TST_ROTATED
:dd.Toolbar is rotated (in vertical position).
:edl.
:p.If neither TST_MINIMIZED nor TST_FIXED is set, Toolbar is in the floating state.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_QUERYSTATE_REMARK.TM_QUERYSTATE - remarks
:p.None

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_QUERYSTATE_DFPROC.TM_QUERYSTATE - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.ulState:ehp1. to default value of 0.




.* TM_QUERYWIDTH - Query required toolbar width
.* ********************************************
.*
:h2 group=1 scroll=none width=30% name=TM_QUERYWIDTH.TM_QUERYWIDTH
:link auto reftype=hd dependent refid=TM_QUERYWIDTH_SYNTAX.
:link auto reftype=hd refid=TM_QUERYWIDTH.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_QUERYWIDTH_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_QUERYWIDTH_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_QUERYWIDTH_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_QUERYWIDTH_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_QUERYWIDTH_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_QUERYWIDTH_SYNTAX.TM_QUERYWIDTH - syntax
:p.Message queries Toolbar width required to hold all objects without clipping.
:p.:cgraphic.
param1
     ULONG  :link dependent reftype=hd refid=TM_QUERYWIDTH_OPT.flOptions:elink.    // Options

param2
     ULONG  :link dependent reftype=hd refid=TM_QUERYWIDTH_MAX.ulMaxLength:elink.  // Maximum possible length of toolbar
:ecgraphic.

.* Options
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_QUERYWIDTH_OPT.TM_QUERYWIDTH - flOptions
:dl tsize=3 break=all.
:dt.:hp2.flOptions:ehp2. (ULONG)
:dd.These options determine type of returned information:
:dl tsize=3 break=all.
:dt.TDQ_RESTORED
:dd.Full Toolbar width in the floating state.
:dt.TDQ_FIXED	
:dd.Width in the maximized (docked) state.
:dt.TDQ_MINIMIZED
:dd.Width in the minimized state.
:edl.
:edl.

.* Maximum width of the toolbar
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_QUERYWIDTH_MAX.TM_QUERYWIDTH - ulMaxLength
:dl tsize=3 break=all.
:dt.:hp2.ulMaxLength:ehp2. (ULONG)
:dd.Expected, maximum possible length of the Toolbar. With TBS_MULTILINE style set,
height of the Toolbar may cause effects on it's width (for vertical Toolbars).
Ignored if set to 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_QUERYWIDTH_PARAMS.TM_QUERYWIDTH - parameters
:dl tsize=3 break=all.
:dt.:hp2.flOptions:ehp2. (ULONG)
:dd.These options determine type of returned information:
:dl tsize=3 break=all.
:dt.TDQ_RESTORED
:dd.Full Toolbar width in the floating state.
:dt.TDQ_FIXED	
:dd.Width in the maximized (docked) state.
:dt.TDQ_MINIMIZED
:dd.Width in the minimized state.
:edl.
:dt.:hp2.ulMaxLength:ehp2. (ULONG)
:dd.Expected, maximum possible length of the Toolbar. With TBS_MULTILINE style set,
height of the Toolbar may cause effects on it's width (for vertical Toolbars).
Ignored if set to 0.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_QUERYWIDTH_RESULT.TM_QUERYWIDTH - results
:dl tsize=3 break=all.
:dt.:hp2.lWidth:ehp2. (LONG)
:dd.Expected width of the Toolbar, calculated for specified parameters.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_QUERYWIDTH_REMARK.TM_QUERYWIDTH - remarks
:p.Message reads width of the Toolbar required to hold all objects without clipping.

Toolbar window procedure reads current widths of all objects placed on it
(object sizes are set automatically during adding and removing them). Further
calculations depends on specified toolbar state&colon.
:ul.
:li.In csae of the horizontal Toolbar, object's widths are added together (single
pixel space between objects is also included) next, widths of control objects
are added (title bar, rotate button and separators). If calculated width exceeds
:hp1.ulMaxLength:ehp1. it is truncated to :hp1.ulMaxLength:ehp1.. Final result
increased by border width is returned.

:li.In case of the vertical Toolbar, dimension of a widest object is taken,
increased by width of a border and then returned. With :hp1.ulMaxLength:ehp1.
parameter specified, toolbar with TBS_MULTILINE style calculates
width taking into account possible wrapping of objects that doesn't fit
within specified :hp1.ulMaxLength:ehp1..

:li.If TDQ_MINIMIZED option has been specified, vertical toolbar returns constant
height of the title bar increased by a border height.
.br
For minimized horizontal Toolbar, message returns it's height before minimization,
increased by 50%.
:eul.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_QUERYWIDTH_DFPROC.TM_QUERYWIDTH - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.lWidth:ehp1. to default value of 0.




.* TM_QUERYOBJECTCOUNT - Query number of objects located on the toolbar
.* ********************************************************************
.*
:h2 group=1 scroll=none width=30% name=TM_QUERYOBJECTCOUNT.TM_QUERYOBJECTCOUNT
:link auto reftype=hd dependent refid=TM_QUERYOBJECTCOUNT_SYNTAX.
:link auto reftype=hd refid=TM_QUERYOBJECTCOUNT.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_QUERYOBJECTCOUNT_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_QUERYOBJECTCOUNT_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_QUERYOBJECTCOUNT_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_QUERYOBJECTCOUNT_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_QUERYOBJECTCOUNT_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_QUERYOBJECTCOUNT_SYNTAX.TM_QUERYOBJECTCOUNT - syntax
:p.Message queries number of objects located on the Toolbar.
:p.:cgraphic.
param1
     ULONG :link dependent reftype=hd refid=TM_QUERYOBJECTCOUNT_RES.ulReserved:elink.  // Reserved, should be 0.

param2
     ULONG :link dependent reftype=hd refid=TM_QUERYOBJECTCOUNT_RES.ulReserved:elink.  // Reserved, should be 0.
:ecgraphic.

.* Reserved, should be 0.
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_QUERYOBJECTCOUNT_RES.TM_QUERYOBJECTCOUNT - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_QUERYOBJECTCOUNT_PARAMS.TM_QUERYOBJECTCOUNT - parameters
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_QUERYOBJECTCOUNT_RESULT.TM_QUERYOBJECTCOUNT - results
:dl tsize=3 break=all.
:dt.:hp2.ulObjCount:ehp2. (ULONG)
:dd.Number of objects placed on the Toolbar.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_QUERYOBJECTCOUNT_REMARK.TM_QUERYOBJECTCOUNT - remarks
:p.Function returns number of objects placed on the Toolbar. Control objects like&colon.
titlebar and rotate button are excluded.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_QUERYOBJECTCOUNT_DFPROC.TM_QUERYOBJECTCOUNT - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.ulObjCount:ehp1. to default value of 0.





.* TM_QUERYTIP - Query tool tip text
.* *********************************
.*
:h2 group=1 scroll=none width=30% name=TM_QUERYTIP.TM_QUERYTIP
:link auto reftype=hd dependent refid=TM_QUERYTIP_SYNTAX.
:link auto reftype=hd refid=TM_QUERYTIP.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_QUERYTIP_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_QUERYTIP_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_QUERYTIP_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_QUERYTIP_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_QUERYTIP_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_QUERYTIP_SYNTAX.TM_QUERYTIP - syntax
:p.Message returns tooltip text associated with specified object.
:p.:cgraphic.
param1
     USHORT1 :link dependent reftype=hd refid=TM_QUERYTIP_OBJ.usObjId:elink.   // Object identifier
     USHORT2 :link dependent reftype=hd refid=TM_QUERYTIP_BUFSIZE.usBufSize:elink. // Size of the output buffer

param2
     PSZ     :link dependent reftype=hd refid=TM_QUERYTIP_BUF.szTipText:elink. // Pointer to output buffer
:ecgraphic.

.* Object identifier
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_QUERYTIP_OBJ.TM_QUERYTIP - usObjId
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Object identifier.
:edl.

.* Size of the output buffer
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_QUERYTIP_BUFSIZE.TM_QUERYTIP - usBufSize
:dl tsize=3 break=all.
:dt.:hp2.usBufSize:ehp2. (USHORT)
:dd.Size of the output buffer, including NULL terminator.
:edl.

.* Pointer to output buffer
.*
:h2 group=12 x=30% y=bottom width=70% height=225x hide name=TM_QUERYTIP_BUF.TM_QUERYTIP - szTipText
:dl tsize=3 break=all.
:dt.:hp2.usTipText:ehp2. (PSZ)
:dd.Pointer to output buffer where tooltip text is stored.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_QUERYTIP_PARAMS.TM_QUERYTIP - parameters
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Object identifier.
:dt.:hp2.usBufSize:ehp2. (USHORT)
:dd.Size of the output buffer, including NULL terminator.
:dt.:hp2.usTipText:ehp2. (PSZ)
:dd.Pointer to output buffer where tooltip text is stored.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_QUERYTIP_RESULT.TM_QUERYTIP - results
:dl tsize=3 break=all.
:dt.:hp2.ulStrLen:ehp2. (ULONG)
:dd.Actual length of tooltip text.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_QUERYTIP_REMARK.TM_QUERYTIP - remarks
:p.Message reads tooltip text associated with given object, or length of this text.
Specifying :hp1.szTipText:ehp1. parameter equal to NULL or :hp1.usBufSize:ehp1. equal to 0
allows to read length of tooltip text without actual read of the text.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_QUERYTIP_DFPROC.TM_QUERYTIP - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.ulStrLen:ehp1. to default value of 0.




.* TM_ROTATE - Rotate toolbar
.* **************************
.*
:h2 group=1 scroll=none width=30% name=TM_ROTATE.TM_ROTATE
:link auto reftype=hd dependent refid=TM_ROTATE_SYNTAX.
:link auto reftype=hd refid=TM_ROTATE.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_ROTATE_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_ROTATE_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_ROTATE_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_ROTATE_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_ROTATE_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_ROTATE_SYNTAX.TM_ROTATE - syntax
:p.Message rotates the Toolbar.
:p.:cgraphic.
param1
     ULONG :link dependent reftype=hd refid=TM_ROTATE_OPT.flOptions:elink.   // Rotation options

param2
     ULONG :link dependent reftype=hd refid=TM_ROTATE_RES.ulReserved:elink.  // Reserved, equal to 0
:ecgraphic.

.* Options
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_ROTATE_OPT.TM_ROTATE - flOptions
:dl tsize=3 break=all.
:dt.:hp2.flOptions:ehp2. (ULONG)
:dd.Options controlling rotation&colon.
:dl tsize=4 break=all compact.
:dt.TBR_ROTATE
:dd.Change state to opposite.
:dt.TBR_HORIZONTAL
:dd.Set horizontal direction.
:dt.TBR_VERTICAL
:dd.Set vertical direction.
:edl.
:edl.

.* Reserved, shold be 0.
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_ROTATE_RES.TM_ROTATE - ulReserved
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_ROTATE_PARAMS.TM_ROTATE - parameters
:dl tsize=3 break=all.
:dt.:hp2.flOptions:ehp2. (ULONG)
:dd.Options controlling rotation&colon.
:dl tsize=4 break=all compact.
:dt.TBR_ROTATE
:dd.Change state to opposite.
:dt.TBR_HORIZONTAL
:dd.Set horizontal direction.
:dt.TBR_VERTICAL
:dd.Set vertical direction.
:edl.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, should be 0.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_ROTATE_RESULT.TM_ROTATE - results
:dl tsize=3 break=all.
:dt.:hp2.ulReserved:ehp2. (ULONG)
:dd.Reserved, equal to 0.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_ROTATE_REMARK.TM_ROTATE - remarks
:p.Message causes rotate of the Toolbar, according to options specified
by :hp1.flOptions:ehp1. parameter.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_ROTATE_DFPROC.TM_ROTATE - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.ulReserved:ehp1. to default value of 0.



.* TM_SETOBJECT - Set object parameters.
.* *************************************
.*
:h2 group=1 scroll=none width=30% name=TM_SETOBJECT.TM_SETOBJECT
:link auto reftype=hd dependent refid=TM_SETOBJECT_SYNTAX.
:link auto reftype=hd refid=TM_SETOBJECT.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_SETOBJECT_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_SETOBJECT_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_SETOBJECT_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_SETOBJECT_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_SETOBJECT_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_SETOBJECT_SYNTAX.TM_SETOBJECT - syntax
:p.Message changes parameters in object's control structures.
:p.:cgraphic.
param1
     USHORT1 :link dependent reftype=hd refid=TM_SETOBJECT_OBJ.usObjId:elink.     // Object identifier
     USHORT2 :link dependent reftype=hd refid=TM_SETOBJECT_OPT.usOptions:elink.   // Object identifier

param2
     :link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink. :link dependent reftype=hd refid=TM_SETOBJECT_INFO.objInfo:elink.     // Object information structure
:ecgraphic.

.* Object identifier
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_SETOBJECT_OBJ.TM_SETOBJECT - usObjId
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Object identifer.
:edl.

.* Options
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_SETOBJECT_OPT.TM_SETOBJECT - usOptions
:dl tsize=3 break=all.
:dt.:hp2.usOptions:ehp2. (USHORT)
:dd.Options controlling parameters change.
:edl.

.* Object information structure
.*
:h2 group=12 x=30% y=bottom width=70% height=225x hide name=TM_SETOBJECT_INFO.TM_SETOBJECT - objInfo
:dl tsize=3 break=all.
:dt.:hp2.objInfo:ehp2. (:link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink.)
:dd.Object information structure containing parameters being changed.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_SETOBJECT_PARAMS.TM_SETOBJECT - parameters
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Object identifer.
:dt.:hp2.usOptions:ehp2. (USHORT)
:dd.Options controlling parameters change.
:dt.:hp2.objInfo:ehp2. (:link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink.)
:dd.Object information structure containing parameters being changed.
:edl.

.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_SETOBJECT_RESULT.TM_SETOBJECT - results
:dl tsize=3 break=all.
:dt.:hp2.bStatus:ehp2. (BOOL)
:dd.Operation stateu&colon.
:dl tsize=3 break=all compact.
:dt.FALSE
:dd.Parameter change failed.
:dt.TRUE
:dd.Successful completion.
:edl.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_SETOBJECT_REMARK.TM_SETOBJECT - remarks
:p.Message changes parameters stored in object's control structures.
According to options specified by :hp1.usOptions:ehp1. following
parameters can be changed&colon.
:dl tsize=3 break=all.
:dt.TSO_GROUP
:dd.Changes group membership. The :hp1.ulGroup:ehp1. field of the :link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink.
structure will be considered. Object takes the TBO_PUSHBUTTON, TBO_CHECKBOX or
TBO_RADIOBUTTON attributes from group wehere it is moved to. Remaining TBO_*
and SPS_* atributes remain unchanged.
:dt.TSO_SIZE
:dd.Change of object size. The :hp1.cx:ehp1. and :hp1.cy:ehp1. fields of the :link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink.
structure will be considered. Display of size change depends on current
Toolbar position&colon.
:ul compact.
:li.For horizontal Toolbar, width of object is changed, but height only if it
exceeds current height of the Toolbar.
:li.For vertical Toolabr, height of object is changed, but width only if it
exceeds current width of the Toolbar.
:li.Objects created with TBO_BUTTON and TBO_SEPARATOR attribute set it's size
automatically, thus any size change done by TM_SETOBJECT is ignored.
:eul.
:dt.TSO_USER
:dd.Changes content of the :hp1.ulUser:ehp1. field associated with object.
:edl.
:p.In the current implementation it is impossible to modify the :hp1.flAttribute:ehp1 field
(maybe in next version). The :hp1.id:ehp1. will not be modifable. The :hp1.iPosition:ehp1.
field can be modified indirectly using
:link reftype=hd refid=TM_MOVEOBJECT.TM_MOVEOBJECT:elink. message.
:p.Modification of several fields at a time is possible by joining options
using the OR operator.

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_SETOBJECT_DFPROC.TM_SETOBJECT - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.bSatus:ehp1. to default value of 0.



.* TM_SHOWOBJECT - show/hide an object
.* ***********************************
.*
:h2 group=1 scroll=none width=30% name=TM_SHOWOBJECT.TM_SHOWOBJECT
:link auto reftype=hd dependent refid=TM_SHOWOBJECT_SYNTAX.
:link auto reftype=hd refid=TM_SHOWOBJECT.
:p.Select an item&colon.
:p.
:link dependent reftype=hd refid=TM_SHOWOBJECT_SYNTAX.Syntax:elink.
.br
:link dependent reftype=hd refid=TM_SHOWOBJECT_PARAMS.Parameters:elink.
.br
:link dependent reftype=hd refid=TM_SHOWOBJECT_RESULT.Results:elink.
.br
:link dependent reftype=hd refid=TM_SHOWOBJECT_REMARK.Remarks:elink.
.br
:link dependent reftype=hd refid=TM_SHOWOBJECT_DFPROC.Default processing:elink.

.* Syntax
.*
:h2 group=2 x=30% width=70% hide name=TM_SHOWOBJECT_SYNTAX.TM_SHOWOBJECT - syntax
:p.Message changes parameters in object's control structures.
:p.:cgraphic.
param1
     ULONG :link dependent reftype=hd refid=TM_SHOWOBJECT_OBJ.usObjId:elink.   // Object identifier

param2
     BOOL  :link dependent reftype=hd refid=TM_SHOWOBJECT_STAT.bVisible:elink.  // New visibility state
:ecgraphic.

.* Object identifier
.*
:h2 group=10 x=30% y=bottom width=70% height=225x hide name=TM_SHOWOBJECT_OBJ.TM_SHOWOBJECT - usObjId
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Object identifer.
:edl.

.* New object state
.*
:h2 group=11 x=30% y=bottom width=70% height=225x hide name=TM_SHOWOBJECT_STAT.TM_SHOWOBJECT - bVisible
:dl tsize=3 break=all.
:dt.:hp2.bVisible:ehp2. (BOOL)
:dd.New object's visibility state&colon.
:dl tsize=3 break=all compact.
:dt.TRUE
:dd.Object becomes visible.
:dt.FALSE
:dd.Object becomes invisible.
:edl.
:edl.

.* Parameters
.*
:h2 group=3 x=30% width=70% hide name=TM_SHOWOBJECT_PARAMS.TM_SHOWOBJECT - parameters
:dl tsize=3 break=all.
:dt.:hp2.usObjId:ehp2. (USHORT)
:dd.Object identifer.
:dt.:hp2.bVisible:ehp2. (BOOL)
:dd.New object's visibility state&colon.
:dl tsize=3 break=all compact.
:dt.TRUE
:dd.Object becomes visible.
:dt.FALSE
:dd.Object becomes invisible.
:edl.
:edl.


.* Results
.*
:h2 group=4 x=30% y=bottom width=70% height=225x hide name=TM_SHOWOBJECT_RESULT.TM_SHOWOBJECT - results
:dl tsize=3 break=all.
:dt.:hp2.bStatus:ehp2. (BOOL)
:dd.Operation stateu&colon.
:dl tsize=3 break=all compact.
:dt.FALSE
:dd.Error&colon. invalid object identifier, or identifier of separator.
:dt.TRUE
:dd.Successful completion.
:edl.
:edl.

.* Remarks
.*
:h2 group=5 x=30% width=70% hide name=TM_SHOWOBJECT_REMARK.TM_SHOWOBJECT - remarks
:p.Commands controls visibility of object placed on the Toolbar.
Hidden object doesn't change it's position and identifier. It also doesn't
free any resources consumed like memory. Current state of the object can be read
using the :link reftype=hd refid=TM_QUERYOBJECT.TM_QUERYOBJECT:elink. message -
the :hp1.flAttribute:ehp1. field of returned :link reftype=hd refid=TOOLOBJ.TOOLOBJ:elink. structure contains
the TBO_HIDE bit set according to the current state.
:p.By testing the TBO_DISABLE bit it is possible to check whether object is disabled
(it doesn't concern separators).

.* Default processing
.*
:h2 group=6 x=30% width=70% hide name=TM_SHOWOBJECT_DFPROC.TM_SHOWOBJECT - default processing
:p.The default window procedure takes no action on this message, other than
to set :hp1.bSatus:ehp1. to default value of 0.


.* WM_MINMAXFRAME
.* **************
:h2 name=WM_MINMAXFRAME.WM_MINMAXFRAME (In Toolbar windows)
:p.To be described.

.* WM_PRESPARAMCHANGED
.* *******************
:h2 name=WM_PRESPARAMCHANGED.WM_PRESPARAMCHANGED (In Toolbar windows)
:p.To be described.

.* WM_QUERYWINDOWPARAMS
.* ********************
:h2 name=WM_QUERYWINDOWPARAMS.WM_QUERYWINDOWPARAMS (In Toolbar windows)
:p.To be described.

.* WM_SETWINDOWPARAMS
.* ******************
:h2 name=WM_SETWINDOWPARAMS.WM_SETWINDOWPARAMS (In Toolbar windows)
:p.To be described.

.* WM_TRACKFRAME
.* *************
:h2 name=WM_TRACKFRAME.WM_TRACKFRAME (In Toolbar windows)
:p.To be described.


.* *******************************
.*Instrukcja u¾ytkowania (tematy do napisania).
.*
.*Struktura paska narz©dzi.
.*  Predefiniowane okna potomne.
.*Sterowanie pozycjonowaniem paska narz©dzi.
.*  Sterowanie automatycznym skalowaniem.
.*  Sterowanie procesem maksymalizacji (dokowania).
.*  Sterowanie procesem minimalizacji.
.*  Sterowanie procesem przej˜cia do stanu "floating".
.*  Sterowanie obracaniem.
.*  Komunikat WM_MINMAXFRAME
.*Kontrola przesuwania paska narz©dzi.
.*  Rozpocz©cie przesuwania.
.*  Przesuwanie.
.*  Zakoäczenie przesuwania.
.*  Komunikat WM_TRACKFRAME.
.*Dodawanie i usuwanie obiekt¢w.
.*Sterowanie zachowaniem obiekt¢w.
