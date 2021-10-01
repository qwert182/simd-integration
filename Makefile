EXENAME=simd-integration
OBJS=$(OBJDIR)/integration$(OBJEXT) \
  $(OBJDIR)/methods$(OBJEXT) \
  $(OBJDIR)/utils$(OBJEXT)

ENABLE_DEBUG_INFO=1
ENABLE_WARNINGS=1

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

!if $(ENABLE_WARNINGS)-0 == 1
CFLAGS=$(CFLAGS) -W4 -wd4204
!endif

$(EXENAME)$(EXEEXT): $(OBJDIR)/$(EXENAME)$(EXEEXT)
	copy /y $(OBJDIR)\$@ $(MAKEDIR)

$(OBJDIR)/$(EXENAME)$(EXEEXT): $(OBJDIR) $(OBJDIR)/.hdr $(OBJS)
	@cd $(OBJDIR)
	@set OBJS=$(OBJS)
	@call echo Linking $@: %%OBJS:$(OBJDIR)/=%%
	$(LINK) -Fe"$(@F)" %%OBJS:$(OBJDIR)/=%% -link $(LDFLAGS)
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
UNAME_S:=$(shell uname -s)
EXEEXT=
OBJEXT=.o
COMPILE=$(CC) -c $(CFLAGS)
LDFLAGS+= -lm
LINK=$(CC) $(CFLAGS)

ifeq ($(ENABLE_DEBUG_INFO),1)
CFLAGS+= -g
endif

ifeq ($(ENABLE_WARNINGS),1)
CFLAGS+= -Wall -Wextra -Wconversion
ifeq ($(UNAME_S),Darwin)
CFLAGS+= -Wno-unused-local-typedef
else
CFLAGS+= -Wno-unused-local-typedefs
endif
endif

$(OBJDIR)/$(EXENAME)$(EXEEXT): $(OBJS)
	cd $(OBJDIR) && $(LINK) -o $(@F) $(+F) $(LDFLAGS)

$(EXENAME)$(EXEEXT): $(OBJDIR)/$(EXENAME)$(EXEEXT)
	cp -f $< $@

$(OBJDIR)/%$(OBJEXT): $(SRCDIR)/%.c $(SRCDIR)/*.h | $(OBJDIR)
	$(COMPILE) -o $@ $<

$(OBJDIR):
	@mkdir $(OBJDIR)

clean:
	$(RM) -r $(OBJDIR)
	$(RM) $(EXENAME)$(EXEEXT)

endif
!endif :

all: $(EXENAME)$(EXEEXT)
