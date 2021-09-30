all:

ifdef WINDOWS_NMAKE:
!ifndef WINDOWS_NMAKE:
EXEEXT=.exe

simd-integration.exe:
	echo test nmake
!else
else
EXEEXT=

simd-integration:
	echo test make
endif
!endif :

all: simd-integration$(EXEEXT)
