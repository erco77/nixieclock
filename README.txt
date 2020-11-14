NIXIE CLOCK

    The nixieclock program is a simple little desktop clock that shows the
    current time using a 'nixie tube' style display, and can display the time
    in various time formats as set by the -s flag.

    It can be run from the command line with ./nixieclock
    (Or on windows, nixieclock.exe)

    For more info, see the nixieclock(1) man page, or in docs/nixieclock.html,
    or invoke nixieclock with the -help flag.


NIXIE TEXT FILE VIEWER

    Try it -- it's fun (trust me):

        ./nixie-text-viewer

    The Up/Down arrow keys move through the source code a line at a
    time, PgUp/PgDn to move a page at a time, Home/End to move quickly
    to top/bottom of file.


NIXIE DISPLAY WIDGET

    The 'NixieDisplay' class that can be used in your own applications
    to do general display of multiline ASCII text.

    For instance, compile and run the class's test program 
    'nixie-text-viewer'. The demo shows the program's own source code
    in a text viewer using the same nixie font used by the clock..

FILES

    DOCS
	README.md    -- main page on github
	README.txt   -- console docs on app code (this document)
	CHANGES.txt  -- modifications/releases history, most recent at top
	INSTALL.txt  -- how to build/install the app
	LICENSE      -- GPL license file for this project (1.66 - GPL3)
	notes        -- todo items
	docs         -- man pages, html, screenshots, etc

    CODE             _
	Digit.C       |__ manage individual digits of Nixie font
	Digit.H      _|  _
	NixieClock.C      |__ clock management
	NixieClock.H     _|  _
	NixieDisplay.C        |__ manage digits as strings
	NixieDisplay.H       _|   and potentially multi-line
	Makefile             -- cross platform Makefile
	nixie-text-viewer.C  -- test application for multiline display
	nixieclock-app.C     -- clock application code
	version.H            -- manually maintained version# file
	xpms                 -- glyphs for each ASCII digit


DOCUMENTATION
    See ./docs/nixieclock.html


LICENSE
    This code is GPL'ed; see LICENSE file for details.
    Send bugs or RFEs to me.                             -erco@seriss.com


CONTRIBUTORS
    Vance Kochenderfer - man page
    Lemon - freebsd and countdown patch
    Terry mrtembry<AT>yahoo<D0T>com - mods for mingw linux cross compile, fixes
    Greg Ercolano - initial implementation/project author
