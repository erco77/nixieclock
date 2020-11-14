# NIXIECLOCK
# erco@seriss.com 1.10

SHELL=/bin/sh
FLTK_CONFIG=fltk-config

# FLTK BUILD 
CXX      = $(shell $(FLTK_CONFIG) --cxx)
CXXFLAGS = $(shell $(FLTK_CONFIG) --use-images --cxxflags )
LDLIBS   = $(shell $(FLTK_CONFIG) --use-images --ldstaticflags )
LINK     = $(CXX)
OS       = $(shell uname -s)
VERSION  = $(shell sed 's/^[^"]*"//;s/"//' version.H)

# WINDOWS?
ifneq ($(wildcard /c/windows/system32/*),)
	EXE=.exe
else
	EXE=
endif

# DEFAULT BUILD
all: nixieclock$(EXE) nixie-text-viewer$(EXE) docs

# HOW TO BUILD
.SUFFIXES: .o .C

%.o: %.C %.H
	$(CXX) $(CXXFLAGS) -c $<

# NIXIE MODULES
NixieDisplay.o: NixieDisplay.H NixieDisplay.C

NixieClock.o: NixieClock.H NixieClock.C NixieDisplay.o

# BUILD NIXIECLOCK
nixieclock$(EXE): nixieclock-app.o Digit.o NixieClock.o NixieDisplay.o
	$(LINK) nixieclock-app.o Digit.o NixieClock.o NixieDisplay.o $(LDLIBS) -o nixieclock$(EXE)
	$(FLTK_CONFIG) --post nixieclock$(EXE)

# BUILD STANDALONE TEST MODULES
nixie-text-viewer$(EXE): nixie-text-viewer.o Digit.o NixieClock.o NixieDisplay.o
	$(LINK) nixie-text-viewer.o Digit.o NixieClock.o NixieDisplay.o $(LDLIBS) -o nixie-text-viewer$(EXE)
	$(FLTK_CONFIG) --post nixie-text-viewer$(EXE)

# DOCUMENTATION
docs: FORCE
	pod2man --center "Erco's FLTK tools" \
	        --name=nixieclock \
		--section=1 \
		--release=$(VERSION) \
		< docs/nixieclock.pod > docs/nixieclock.1
	pod2html --title "nixieclock $(VERSION)-- Erco's FLTK tools" \
		< docs/nixieclock.pod > docs/nixieclock.html

# INSTALL
install: nixieclock$(EXE) docs
	if [ -d /Applications ]; then \
	    echo -- Installing binary in /Applications; \
	    cp -rp nixieclock.app /Applications ; \
	elif [ -d /usr/local/bin ]; then \
	    echo -- Installing binary in /usr/local/bin; \
	    cp nixieclock /usr/local/bin ; \
	else \
	    echo "Can't figure out where to install binary"; \
	    exit 1; \
	fi
	if [ -d /usr/share/man/man1 ]; then \
	    echo "-- Installing man page in /usr/share/man/man1"; \
	    cp docs/nixieclock.1 /usr/share/man/man1; \
	fi
	if [ -d /usr/share/doc ]; then \
	    echo -- Installing docs in /usr/share/doc/nixieclock-$(VERSION); \
	    if [ ! -d /usr/share/doc/nixieclock-$(VERSION) ]; then \
	        mkdir /usr/share/doc/nixieclock-$(VERSION); \
	    fi; \
	    cp docs/nixieclock.html /usr/share/doc/nixieclock-$(VERSION); \
	    cp LICENSE              /usr/share/doc/nixieclock-$(VERSION); \
	fi

# CLEAN UP
clean:
	-rm -f $(ALL) nixieclock$(EXE) nixie-text-viewer$(EXE) *.o core *.idb *.pdb *.obj *.exe *.rc *.res *.RES
	-rm -rf pod2htm*~~
	-rm -rf pod2htm*.tmp
	-rm -f docs/*.html
	-rm -f docs/*.1
	if [ -d nixieclock.app        ]; then rm -rf nixieclock.app;        fi
	if [ -d nixie-text-viewer.app ]; then rm -rf nixie-text-viewer.app; fi

# MAKE VERBOSITY
.SILENT:

# DO NOT REMOVE
FORCE:
