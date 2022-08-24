/************************************************************************

    nixieclock - a nixie desktop timepiece
    Copyright (C) 2000,2020 Greg Ercolano, erco@seriss.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    To report bugs, create an 'issue' on the project's github page:
    https://github.com/erco77/nixieclock

*************************************************************************/

#include <stdio.h>	// stderr
#include <unistd.h>	// fork()
#include <locale.h>	// setlocale()
#include <FL/Fl.H>	// Fl::error()
#include <FL/fl_ask.H>	// Fl::error()
#include "NixieClock.H"
#include "version.H"

// GLOBALS
NixieClock *G_nixieclock = 0;		// main window
int         G_nofork = 0;		// set if we shouldn't fork

void HelpAndExit() {
    Fl::error(
        "Nixieclock -- Version " VERSION "\n"
	"(C) Copyright Greg Ercolano 2000, 2020. (GPL V3 LICENSE)\n"
	"This program uses FLTK, a GUI library available from fltk.org\n"
	"\n"
	"USAGE\n"
	"    nixieclock [options]\n"
	"\n"
	"OPTIONS\n"
	"    -m              # month oriented date (default, eg. 07:57 Sat Feb 07)\n"
	"    -d              # digit date mode (eg. 07:57 07-30-01)\n"
	"    -D              # digit date mode (eg. 07:57 07-30-2001)\n"
	"    -s <timestr>    # use strftime(3) date format\n"
	"    -t              # test pattern for alphanumeric display\n"
	"    -n              # no background mode (don't fork)\n"
	"    -geometry <pos> # sets geometry (x windows style positions)\n"
	"    -c <epochtime>  # counts down to epochtime\n"
	"    -N              # no save: disables saving the current settings to ~/.nixieclock\n"

// NOT YET
//	"    -r cmd [args]   # runs command, each line of output updates in clock\n"
	"    -a              # above all windows (not supported on all platforms)\n"
	"    -b              # enable window borders\n"
	"    -nb             # disable window borders (default)\n"
	"    -v              # enables verbose mode/debugging\n"
	"    -h              # help\n"
	"\n"
	"FILES\n"
	"    Previous settings are saved in ~/.nixieclock\n"
	"\n"
        "BUGS\n"
	"    To report bugs, create an 'issue' on the project's github page:\n"
        "    https://github.com/erco77/nixieclock\n"
	"\n"
	"EXAMPLES\n"
	"    nixieclock                               -- (uses last set date format. Default: -m)\n"
	"    nixieclock -m                            -- 07:57 SAT FEB 07 (default)\n"
	"    nixieclock -d                            -- 07:57 07-30-01\n"
#ifdef _WIN32
	"    nixieclock -s %%c                         -- Locale specific: 01/01/00 23:59:59\n"
#else
	"    nixieclock -s %%c                         -- Locale specific: MON JAN 01 23:59:59 2000\n"
#endif
	"    nixieclock -s \"%%I:%%M %%p %%m/%%d/%%y\"        -- 12:59 PM 12/31/00\n"
	"    nixieclock -s \"%%H:%%M:%%S\"                 -- 23:59:59 (counts seconds)\n"
	"    nixieclock -s \"%%H:%%M:%%S %%a %%b %%d\"        -- 23:59:59 SAT DEC 31\n"
	"    nixieclock -s \"%%a %%b %%m %%H:%%M:%%S %%Y\"     -- Unix format; MON JAN 01 23:59:59 2000\n"
	"    nixieclock -s \"%%a %%b %%m %%H:%%M:%%S %%Y %%Z\"  -- Above + timezone; MON JAN 01 23:59:59 2000 PDT\n"
	"    nixieclock -geometry -0-0                 -- position lower right\n"
	"    nixieclock -geometry -5-5                 -- position 5 pixels lower right\n"
	"    nixieclock -geometry +0-0                 -- position lower left\n"
	"    nixieclock -N -c `perl -e 'print time()+3600;'` -s \"BEER: %%H:%%M:%%S\"\n"
	"                                              -- 'beer o'clock' countdown timer\n"
	"                                                  Sets 'countdown timer' to 1hr from now\n"
	"                                                  (3600 secs=1hr)\n"
// NOT YET
//	"    nixieclock -r 'vmstat 3' -geom 1280x40   -- vmstat appears in clock\n"
	"\n");

    exit(1);
}

#define ISIT(val)	if ( strcmp(argv[t], val) == 0 )

// Handle parsing our app's own arguments
int parse_args(int argc, char **argv, int &t) {
    int parsed_words = 0;
    ISIT("-nb") {
	++t;
	G_nixieclock->SetBorder(0);
	parsed_words++;
    } else ISIT("-b") {
	++t;
	G_nixieclock->SetBorder(1);
	parsed_words++;
    } else ISIT("-n") {
	++t;
	G_nofork = 1;
	parsed_words++;
    } else ISIT("-N") {
	++t;
	G_nixieclock->SetNoSave(1);
	parsed_words++;
    } else ISIT("-a") {
	++t;
	G_nixieclock->SetAbove(1);
	parsed_words++;
    } else ISIT("-s") {
	if ( ++t >= argc ) {
	    fprintf(stderr, "-s: missing argument\n");
	    exit(1);
	}
	G_nixieclock->SetTimeString(argv[t]);
	G_nixieclock->SetTickSpeed(0.25);	// fast update (assume user specified time that includes seconds)
	++t;
	parsed_words += 2;
    } else ISIT("-c") {
	if ( ++t >= argc ) {
	    fprintf(stderr, "-c: missing argument (expected a time(2) value)\n");
	    exit(1);
	}
	unsigned long ulval = 0;
	if ( sscanf(argv[t], "%lu", &ulval) != 1 ) {
	    fprintf(stderr, "-c: unexpected non-integer '%s'\n", argv[t]);
	    exit(1);
	}
	G_nixieclock->SetCountdown((time_t)ulval);
	++t;
	parsed_words += 2;
    } else ISIT("-h") {
	// 23:59
	++t;
	G_nixieclock->SetTimeString("%H:%M");
	G_nixieclock->SetTickSpeed(29.0);
	parsed_words++;
    } else ISIT("-d") {
	// 23:59 12/31/99
	++t;
	G_nixieclock->SetTimeString("%H:%M %m/%d/%y");
	G_nixieclock->SetTickSpeed(29.0);
	parsed_words++;
    } else ISIT("-D") {
	// 23:59 12/31/1999
	++t;
	G_nixieclock->SetTimeString("%H:%M %m/%d/%Y");
	G_nixieclock->SetTickSpeed(29.0);
	parsed_words++;
    } else ISIT("-m") {
	// 23:59 Sat Dec 31
	++t;
	G_nixieclock->SetTimeString("%H:%M %a %b %d");
	G_nixieclock->SetTickSpeed(29.0);
	parsed_words++;
    } else ISIT("-t") {
	++t;
	G_nixieclock->SetTimeString("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~!@#$%%^&*()_+`[]{}\\|;':\",./<>?");
	G_nixieclock->SetTickSpeed(30.0);
	parsed_words++;
    } else ISIT("-v") {
	++t;
	G_nixieclock->SetVerbose(1);
	parsed_words++;
    } else ISIT("-geometry") {
        // Check for, but don't parse this one.. let FLTK handle it
	G_nixieclock->SetGeometryForce(1);
	//parsed_words++;
    }
    return(parsed_words);
}

// MAIN
int main(int argc, char **argv) { 
    setlocale(LC_ALL, "");	// multilanguage support

    // BANNER
    fprintf(stderr, "\nnixieclock -- Version %s\n"
                    "(C) Copyright 2011,2020 Greg Ercolano (erco@seriss.com).\n"
		    "This code is GPL V3.\n\n", VERSION); 
    
    // PARSE HELP
    for ( int t=1; t<argc; t++ ) {
	// HELP
	if ( strcmp(argv[t], "-h") == 0 ||
	     strcmp(argv[t], "-help") == 0 ||
	     strcmp(argv[t], "--help") == 0 ) {
	     HelpAndExit();
	}
    }

    // MAKE MAIN WINDOW
    //    Arbitrary size/position for init;
    //    SetTimeString() will determine actual size/position.
    //
    G_nixieclock = new NixieClock(Fl::w()-30,Fl::h()-30,25,25,"nixieclock");
    G_nixieclock->ForceUpperCase(1);
    G_nixieclock->SetUpdateSpeed(0.50);
    G_nixieclock->SetBorder(0);

    // Load previously saved settings
    //    Then command line arguments can alter from there..
    //
    if ( G_nixieclock->LoadSettings() < 0 ) {
        fprintf(stderr, "%s: couldn't load previous settings: %s\n",
	    argv[0], G_nixieclock->GetErrmsg());
	fl_alert("%s: couldn't load previous settings: %s", argv[0], G_nixieclock->GetErrmsg());
    }

    // PARSE ARGUMENTS
    //    Handles both our own args and FLTK's.
    //    Our arguments are handled by the parse_args() function above.
    //
    int i = 1;
    if ( Fl::args(argc, argv, i, parse_args) < argc ) {
        fprintf(stderr, "%s: unknown option '%s'\n", argv[0], argv[i]);
	exit(1);
    }
#ifdef __APPLE__
    // Do not fork on mac; we loose mach ports bindings (or whatever)
    G_nofork = 1;
#else
    if ( G_nofork == -1 ) G_nofork = 0;
#endif

#ifndef _WIN32
#ifndef DARWIN
    // UNIX: RUN IN THE BACKGROUND
    if ( ! G_nofork ) {
	switch ( fork() ) {
	    case -1: perror("fork"); exit(1); 	// ERROR
	    case  0: break;			// CHILD
	    default: exit(0);			// PARENT DISCONNECTS
	}
    }
#endif /*DARWIN*/
#endif /*_WIN32*/

    // SET VISUAL MODE
    //
    //     FL_RGB prevents color map eating under X windows.
    //
    //     FL_DOUBLE|FL_INDEX enables hardware double buffering,
    //     but looks bad on Indy; digit backgrounds are gray.
    //     Fltk does a good job emulating double buffering
    //     in Fl_Double_Window w/out flickering, so leave off.
    //
    Fl::visual(FL_RGB);

    //// BAD FOR WINDOWS (causes clock to appear in upperleft until "touched")
    //// G_nixieclock->free_position();

    G_nixieclock->show(argc, argv);

    // HACK: Need to call this again /after/ show
    //       for it to work on win32..
    if ( G_nixieclock->GetAbove() ) {
        G_nixieclock->SetAbove(1);
    }

    if ( G_nixieclock->SaveSettings() ) {	// save as new settings
        fprintf(stderr, "%s: couldn't save previous settings: %s\n",
	    argv[0], G_nixieclock->GetErrmsg());
	fl_alert("%s: couldn't save previous settings: %s", 
	    argv[0], G_nixieclock->GetErrmsg());
    }
    return(Fl::run());
}
