EXENAME=simd-integration
OBJS=$(OBJDIR)/integration$(OBJEXT) \
  $(OBJDIR)/methods$(OBJEXT) \
  $(OBJDIR)/functions_opt$(OBJEXT) \
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
OPTS_FLAGS_DEFAULT=-Od
OPTS_FLAGS=-O2

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
	-$(COMPILE) $(OPTS_FLAGS_DEFAULT) $<
	-@for %%I in ($<) do @move /y %%~nI$(OBJEXT) $(OBJDIR) >nul 2>&1
	@for %%I in ($<) do @if not exist $(OBJDIR)\%%~nI$(OBJEXT) exit /b 1

$(OBJDIR)/.hdr: $(SRCDIR)/*.h
	@echo %%DATE%% %%TIME%%> $@
	-@del $(OBJS:/=\\)>nul 2>&1

### Optimized functions.c ###
$(OBJDIR)/functions_opt$(OBJEXT): $(SRCDIR)\functions.c
	$(COMPILE) $(OPTS_FLAGS) -Fo"$@" $**

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
OPTS_FLAGS_DEFAULT=-O0
OPTS_FLAGS=-O2

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
	$(COMPILE) $(OPTS_FLAGS_DEFAULT) -o $@ $<

$(OBJDIR):
	@mkdir $(OBJDIR)

### Optimized functions.c ###
$(OBJDIR)/functions_opt$(OBJEXT): $(SRCDIR)/functions.c $(SRCDIR)/*.h | $(OBJDIR)
	$(COMPILE) $(OPTS_FLAGS) -o $@ $<

clean:
	$(RM) -r $(OBJDIR)
	$(RM) $(EXENAME)$(EXEEXT)

endif
!endif :

all: $(EXENAME)$(EXEEXT)
