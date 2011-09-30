# Copyright(c) 2011 Red Hat, Inc.
#
# This is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library in the file COPYING.LIB;
# if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
#
# ====================================================================
#
# nmake.inc Master makefile definitions.
#           This file defines CPU architecture and basic compiler
#           and linker parameters.
# Common params:
#                 CPU       Compile for specified CPU. Supported CPU's are:
#                           X86 (Common x86 architecture)
#                           X64 (AMD64/EMT64 architecture)
#                           I64 (Intel IA64 architecture)
#                           If not specified it will default to the
#                           PROCESSOR_ARCHITECTURE environment variable
#                           or to the X86 if not specified.
#                 WINVER    Compile for specified Windows version
#                           WINNT   for Windows 2000 and up(default)
#                           WINXP   for Windows XP and up
#                           WIN2003 for Windows 2003 and up
#                           WVISTA  for Windows Vista and up
#                           WIN7    for Windows 7 and up
#                 BUILD     Build version
#                           RETAIL or RELEASE (default)
#                           DEBUG
#                 TARGET    Build application target
#                           EXE Console executable (default)
#                           GUI Windows GUI executable
#                           DLL Dynamic Link Library
#                           LIB Static library
# Environment variables  used:
#                 CFLAGS    Added to the common CFLAGS
#                 CPPFLAGS  Added to the common CPPFLAGS
#                 LIBS      Added to the common LIBS
#                 INCLUDES  Added to the common INCLUDES
#                 LFLAGS    Added to the common LFLAGS
#                 RCFLAGS   Added to the common RCFLAGS
#
# Compiler tools environment variables:
#                 CC        C compiler  (defaults to cl.exe)
#                 LINK      Linker (defaults to link.exe)
#                 RC        Resource compiler (defaults to rc.exe)
#                 MT        Manifest toolkit (defaults to mt.exe)
#                 ML        Assembler (defaults to ml.exe or ml64.exe)
#
# ====================================================================
#

!IF !DEFINED(CC) || "$(CC)" == ""
CC = cl.exe
!ENDIF

!IF !DEFINED(LINK) || "$(LINK)" == ""
LINK = link.exe
!ENDIF

!IF !DEFINED(RC) || "$(RC)" == ""
RC = rc.exe
!ENDIF

!IF !DEFINED(MT) || "$(MT)" == ""
MT = mt.exe
!ENDIF

# Read the RPM_ARCH environment value for a CPU type

!IF "$(RPM_ARCH)" == "i386" || "$(RPM_ARCH)" == "x86" || "$(RPM_ARCH)" == "i686"
CPU=X86
!ENDIF
!IF "$(RPM_ARCH)" == "amd64" || "$(RPM_ARCH)" == "x86_64" || "$(RPM_ARCH)" == "x64"
CPU=X64
!ENDIF
!IF "$(RPM_ARCH)" == "ia64" || "$(RPM_ARCH)" == "i64"
CPU=I64
!ENDIF

!IF !DEFINED(CPU) || "$(CPU)" == ""
!IF "$(PROCESSOR_ARCHITECTURE)" == ""
!IF "$(PROCESSOR_ARCHITEW6432)" == ""
CPU=X86
!ELSE
CPU=$(PROCESSOR_ARCHITEW6432)
!ENDIF
!ELSE
CPU=$(PROCESSOR_ARCHITECTURE)
!ENDIF
!ENDIF


!IF "$(CPU)" == "I386"
CPU=X86
!ENDIF
!IF "$(CPU)" == "AMD64"
CPU=X64
!ENDIF
!IF "$(CPU)" == "IA64"
CPU=I64
!ENDIF

!IF "$(CPU)" != "X86"
!IF "$(CPU)" != "X64"
!IF "$(CPU)" != "I64"
!ERROR Must specify CPU environment variable (X86, X64, I64) $(CPU)
!ENDIF
!ENDIF
!ENDIF

!IF !DEFINED(TARGET) || "$(TARGET)" == ""
TARGET=EXE
!ENDIF

!IF "$(TARGET)" != "EXE"
!IF "$(TARGET)" != "GUI"
!IF "$(TARGET)" != "DLL"
!IF "$(TARGET)" != "LIB"
!ERROR Must specify TARGET environment variable (EXE, GUI, DLL, LIB)
!ENDIF
!ENDIF
!ENDIF
!ENDIF

!IF !DEFINED(WINVER) || "$(WINVER)" == ""
WINVER=WINXP
!ENDIF


!IF "$(WINVER)" != "WINNT"
!IF "$(WINVER)" != "WINXP"
!IF "$(WINVER)" != "WIN2003"
!IF "$(WINVER)" != "WIN7"
!IF "$(WINVER)" != "WVISTA"
!ERROR Must specify WINVER environment variable (WINNT, WINXP, WIN2003, WVISTA, WIN7)
!ENDIF
!ENDIF
!ENDIF
!ENDIF
!ENDIF
!ENDIF

!IF "$(WINVER)" == "WINNT"
NMAKE_WINVER = 0x0500
_WIN32_IE = 0x0500
!ELSEIF "$(WINVER)" == "WINXP"
NMAKE_WINVER = 0x0501
_WIN32_IE = 0x0600
!ELSEIF "$(WINVER)" == "WIN2003"
NMAKE_WINVER = 0x0502
_WIN32_IE = 0x0600
!ELSEIF "$(WINVER)" == "WVISTA"
NMAKE_WINVER = 0x0600
_WIN32_IE = 0x0700
!ELSEIF "$(WINVER)" == "WIN7"
NMAKE_WINVER = 0x0601
_WIN32_IE = 0x0800
!ELSE
!ERROR Must specify WINVER environment variable (WINNT, WINXP, WIN2003, WVISTA)
!ENDIF

NMAKE_WINNT = -D_WINNT -DWINNT -D_WIN32_WINNT=$(NMAKE_WINVER) -DWINVER=$(NMAKE_WINVER) -D_WIN32_IE=$(_WIN32_IE)

!IF !DEFINED(BUILD) || "$(BUILD)" == ""
BUILD=RELEASE
!ENDIF

!IFDEF RELEASE
BUILD=RELEASE
!ENDIF

!IFDEF DEBUG
BUILD=DEBUG
!ENDIF

!IFDEF NODEBUG
BUILD=RELEASE
!ENDIF

!IF "$(BUILD)" != "RELEASE"
!IF "$(BUILD)" != "DEBUG"
!ERROR Must specify BUILD environment variable (RELEASE, DEBUG)
!ENDIF
!ENDIF

# Common flags for all platforms
CMN_CFLAGS =  -c -nologo -DWIN32 -D_WIN32 -D_WINDOWS $(NMAKE_WINNT) -W3
!IF "$(TARGET)" == "EXE"
CMN_CFLAGS = $(CMN_CFLAGS) -D_CONSOLE
!ENDIF

!IF "$(CPU)" == "X86"
CPU_CFLAGS = -D_X86_=1
MACHINE=X86
!ELSEIF "$(CPU)" == "X64"
CPU_CFLAGS = -D_AMD64_=1 -D_X64_=1 -DWIN64 -D_WIN64
MACHINE=AMD64
!ELSEIF "$(CPU)" == "I64"
CPU_CFLAGS = -D_IA64_=1 -DWIN64 -D_WIN64
MACHINE=IA64
!ENDIF

!IF "$(BUILD)" == "RELEASE"
!IF "$(CPU)" == "X86"
OPT_CFLAGS = -O2 -Ob2 -Oy- -Zi -DNDEBUG
!ELSE
OPT_CFLAGS = -O2 -Ob2 -Zi -DNDEBUG
!ENDIF
!ELSE
OPT_CFLAGS = -Od -Zi -DDEBUG -D_DEBUG
!ENDIF

!IF DEFINED(STATIC_CRT)
CRT_CFLAGS = -D_MT -MT
!ELSE
CRT_CFLAGS = -D_MT -MD
!ENDIF

!IF "$(BUILD)" == "DEBUG"
CRT_CFLAGS = $(CRT_CFLAGS)d
!ENDIF

CFLAGS = $(CMN_CFLAGS) $(CPU_CFLAGS) $(OPT_CFLAGS) $(CRT_CFLAGS) $(CFLAGS)

!IF DEFINED(EXTRA_CFLAGS)
CFLAGS = $(CFLAGS) $(EXTRA_CFLAGS)
!ENDIF

# Linker section
LIBS = kernel32.lib advapi32.lib ws2_32.lib mswsock.lib ole32.lib shell32.lib rpcrt4.lib $(LIBS)
!IF "$(INCLUDE_BUFFEROVERFLOWU)" == "1"
!IF "$(CPU)" != "X86"
LIBS = $(LIBS) bufferoverflowu.lib
!ENDIF
!ENDIF

!IF DEFINED(EXTRA_LIBS)
LIBS = $(LIBS) $(EXTRA_LIBS)
!ENDIF

COMMON_LFLAGS = /NOLOGO

# Always add debugging to the linker
OPT_LFLAGS = /INCREMENTAL:NO /DEBUG
!IF "$(BUILD)" == "RELEASE"
OPT_LFLAGS = /OPT:REF
!ENDIF

!IF "$(TARGET)" == "EXE"
LFLAGS = $(COMMON_LFLAGS) /SUBSYSTEM:CONSOLE /MACHINE:$(MACHINE)
!ELSEIF "$(TARGET)" == "GUI"
LFLAGS = $(COMMON_LFLAGS) /SUBSYSTEM:WINDOWS /MACHINE:$(MACHINE)
!ELSEIF "$(TARGET)" == "DLL"
LFLAGS = $(COMMON_LFLAGS) /DLL /SUBSYSTEM:WINDOWS /MACHINE:$(MACHINE)
!ELSEIF "$(TARGET)" == "LIB"
LFLAGS = -lib $(COMMON_LFLAGS)
!ENDIF

!IF DEFINED(EXTRA_LFLAGS)
LFLAGS = $(LFLAGS) $(EXTRA_LFLAGS)
!ENDIF

!IF "$(TARGET)" != "LIB"
LFLAGS = $(LFLAGS) $(OPT_LFLAGS)
!ENDIF

# Resource compiler flags

BASE_RCFLAGS=/l 0x409
!IF "$(BUILD)" == "RELEASE"
BASE_RCFLAGS = $(BASE_RCFLAGS) /d "NDEBUG"
!ELSE
BASE_RCFLAGS = $(BASE_RCFLAGS) /d "_DEBUG" /d "DEBUG"
!ENDIF
RCFLAGS = $(BASE_RCFLAGS) $(RCFLAGS)

# Build Target dir e.g. WINNT_I386_RELEASE_DLL
!IF !DEFINED(WORKDIR) || "$(WORKDIR)" == ""
!IF !DEFINED(WORKDIR_EXT) || "$(WORKDIR_EXT)" == ""
WORKDIR = $(WINVER)_$(CPU)_$(TARGET)_$(BUILD)
WORKDIR_DLL = $(WINVER)_$(CPU)_DLL_$(BUILD)
WORKDIR_LIB = $(WINVER)_$(CPU)_LIB_$(BUILD)
WORKDIR_EXE = $(WINVER)_$(CPU)_EXE_$(BUILD)
!ELSE
WORKDIR = $(WINVER)_$(CPU)_$(BUILDIR_EXT)_$(BUILD)
!ENDIF
!ENDIF

CLEANTARGET=if exist "$(WORKDIR)\$(NULL)" rd /s /q $(WORKDIR)
MAKEWORKDIR=if not exist "$(WORKDIR)\$(NULL)" mkdir $(WORKDIR)
MAKEINSTALL=if not exist "$(INSTALLLOC)" mkdir $(INSTALLLOC)

!IF DEFINED(JAVA_HOME) && "$(JAVA_HOME)" != ""
JAVA_INCLUDES=-I "$(JAVA_HOME)\include" -I "$(JAVA_HOME)\include\win32"
!ENDIF

!IF !DEFINED(ML) || "$(ML)" == ""
!IF "$(CPU)" == "X86"
ML = ml.exe
AFLAGS = /coff /Zi /c
!ELSEIF "$(CPU)" == "X64"
ML = ml64.exe
AFLAGS = /Zi /c
!ELSEIF "$(CPU)" == "I64"
ML = ml64.exe
AFLAGS = /coff /Zi /c
!ENDIF
!ENDIF