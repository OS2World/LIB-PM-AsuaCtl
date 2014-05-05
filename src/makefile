#*  
#*  This program is free software; you can redistribute it and/or modify
#*  it under the terms of the GNU General Public License Version 2 as
#*  published by the Free Software Foundation.
#*
#*  This program is distributed in the hope that it will be useful,
#*  but WITHOUT ANY WARRANTY; without even the implied warranty of
#*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#*  GNU General Public License for more details.
#*
#*  You should have received a copy of the GNU General Public License
#*  along with this program; if not, write to the Free Software
#*  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#*
# $Id$
#
#
# (C) 2000 Przemysˆaw Dobrowolski
#
CC      = icc
AS      = alp
CXX     = icc
LD      = ilink
RC      = rc

ASUACTL2_INCLUDE = ..\include
ASUACTL2_BIN     = ..\bin

!ifdef DEBUG
CFLAGS           = -Q  -Si -Ti -Ge- -Ss+ -W3 -Gm+ /Gn+  -I$(ASUACTL2_INCLUDE)
#CFLAGS            = -Se -Re -Ss -Ms -Gm+ -Ge- /Gn+ -I$(ASUACTL2_INCLUDE)
LDFLAGS          =  /nologo /DE /noe /map /NOD /packcode /packdata /exepack:2
!else
CFLAGS           = -Q -Si -G5 -O+ -Oi+ -Ge- -Ss+ -W3 -Gm+ /Gn+ -I$(ASUACTL2_INCLUDE)
LDFLAGS          = /noe /map /packcode /packdata /exepack:2 /nod
!endif

RCFLAGS = -I $(CPPMAIN)\include -I $(ASUACTL2_INCLUDE) -r

OBJS = \
$(ASUACTL2_BIN)\asuactl.obj \
$(ASUACTL2_BIN)\asudebug.obj \
$(ASUACTL2_BIN)\carrier.obj \
$(ASUACTL2_BIN)\fonts.obj \
$(ASUACTL2_BIN)\graphics.obj \
$(ASUACTL2_BIN)\objmgr.obj \
$(ASUACTL2_BIN)\pparams.obj \
$(ASUACTL2_BIN)\progbar.obj \
$(ASUACTL2_BIN)\status.obj \
$(ASUACTL2_BIN)\tcontrol.obj \
$(ASUACTL2_BIN)\toolbar.obj \
$(ASUACTL2_BIN)\WINBUTT.obj \
$(ASUACTL2_BIN)\dirdlg.obj

TARGET = asuactl

all:    $(OBJS) \
        $(ASUACTL2_BIN)\$(TARGET).res \
        $(ASUACTL2_BIN)\$(TARGET).dll

.c{$(ASUACTL2_BIN)}.obj:
    $(CC) $(CFLAGS) -c -Fo$(ASUACTL2_BIN)\$(@B).obj $<

.rc{$(ASUACTL2_BIN)}.res:
    $(RC) $(RCFLAGS) asuactl.rc $@ 


$(ASUACTL2_BIN)\$(TARGET).dll: $(OBJS) $(TARGET).def $(ASUACTL2_BIN)\$(TARGET).lrf
    $(LD) $(LDFLAGS) @$(ASUACTL2_BIN)\$(TARGET).lrf
    $(RC) $(ASUACTL2_BIN)\asuactl.res $@

$(ASUACTL2_BIN)\$(TARGET).lrf: makefile
    @echo Creating file <<$@
/OUT:$(ASUACTL2_BIN)\$(TARGET).dll
/MAP:$(ASUACTL2_BIN)\$(TARGET).map
$(OBJS)
$(TARGET).def
OS2386.LIB
cppom30.lib
<<keep

#
#
