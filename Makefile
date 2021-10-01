EXENAME=simd-integration
OBJS=$(OBJDIR)/integration$(OBJEXT)

ENABLE_DEBUG_INFO=1

SRCDIR=src
OBJDIR=out

all:

ifdef WINDOWS_NMAKE:
!ifndef WINDOWS_NMAKE:

EXEEXT=.exe
OBJEXT=.obj
COMPILE=$(CC) -nologo -c $(CFLAGS)
LINK=$(CC) -nologo $(CFLAGS)
CFLAGS=$(CFLAGS) -MD

!if $(ENABLE_DEBUG_INFO)-0 == 1
CFLAGS=$(CFLAGS) -Zi
COMPILE=$(COMPILE) -Fd"$(OBJDIR)\vc.pdb"
!endif

$(EXENAME)$(EXEEXT): $(OBJDIR)/$(EXENAME)$(EXEEXT)
	copy /y $(OBJDIR)\$@ $(MAKEDIR)

$(OBJDIR)/$(EXENAME)$(EXEEXT): $(OBJDIR) $(OBJDIR)/.hdr $(OBJS)
	@cd $(OBJDIR)
	@set OBJS=$(OBJS)
	@call echo Linking $@: %%OBJS:$(OBJDIR)/=%%
	$(LINK) -Fe"$(@F)" %%OBJS:$(OBJDIR)/=%% -link $(LFLAGS)
	@cd ..

$(OBJDIR):
	@md $(OBJDIR)

{$(SRCDIR)}.c{$(OBJDIR)}$(OBJEXT)::
	-@for %%I in ($<) do @del $(OBJDIR)\%%~nI$(OBJEXT) >nul 2>&1
	-$(COMPILE) $<
	-@for %%I in ($<) do @move /y %%~nI$(OBJEXT) $(OBJDIR) >nul 2>&1
	@for %%I in ($<) do @if not exist $(OBJDIR)\%%~nI$(OBJEXT) exit /b 1

$(OBJDIR)/.hdr: $(SRCDIR)/*.h
	@echo %%DATE%% %%TIME%%> $@
	-@del $(OBJS:/=\\)>nul 2>&1

clean:
	-rd /s /q $(OBJDIR)
	-del $(EXENAME)$(EXEEXT)


!else
else # Linux/Mac


.PHONY: all
EXEEXT=
OBJEXT=.o

$(EXENAME)$(EXEEXT): #$(OBJS)
	echo test make

endif
!endif :

all: $(EXENAME)$(EXEEXT)
