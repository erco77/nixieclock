/************************************************************************

    nixie-text-viewer.C - display a file using nixie font
    Copyright (C) 2000 Greg Ercolano, erco@seriss.com

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

*************************************************************************/


#include <string.h>			// memset()
#include <stdio.h>			// popen()
#include "NixieDisplay.H"

class MyWindow : public NixieDisplay {
    int tlines;
    char *cfile,		// loaded file
         *filepos;		// current position in file

    void ScrollToLine(int linepos) {
        filepos = cfile;
	if ( linepos > 1 ) {
	    while ( *filepos ) {
	        if ( *filepos == '\n' ) {
		    if ( --linepos == 0 ) break;
		}
		++filepos;
	    }
	}
	SetString(filepos);
    }
    void ScrollUp(int linecount=1) {
        // Search in reverse for previous line
        for ( char *s = filepos-2; s >= cfile; s-- ) {
	    if ( s < cfile ) {		// beyond begining of file?
	        filepos = cfile;	// stop at start of file
		SetString(filepos);
		return;
	    } else if ( *s == '\n' ) {
	        filepos = s+1;
		if ( --linecount <= 0 ) {
		    SetString(filepos);
		    return;
		}
	    }
	}
    }

    void ScrollDown(int linecount=1) {
        // Search for next line
        for ( char *s = filepos; 1; s++ ) {
	    if ( *s == '\0' ) {		// eof?
	        SetString(filepos);
		return;
	    } else if ( *s == '\n' ) {
	        ++s;
		if ( *s ) filepos = s;	// not eof? adjust
		if ( --linecount <= 0 ) {
		    SetString(filepos);
		    return;
		}
	    }
	}
    }

    int handle(int e) {
        switch ( e ) {
	    case FL_KEYBOARD:
	        switch ( Fl::event_key() ) {
		    case        FL_Up: ScrollUp(); return(1);
		    case      FL_Down: ScrollDown(); return(1);
		    case      FL_Home: ScrollToLine(0); return(1);
		    case       FL_End: ScrollToLine(tlines-5); return(1);
		    case   FL_Page_Up: ScrollUp(h()/FONTHEIGHT); return(1);
		    case FL_Page_Down: ScrollDown(h()/FONTHEIGHT); return(1);
		}
	}
	return(Fl_Double_Window::handle(e));
    }

public:
    MyWindow(int W, int H) : NixieDisplay(W,H) {
        tlines = 0;
        cfile = 0;
	filepos = 0;
    }

    void LoadCommand(const char *cmd) {
        // HACKY -- For demo purposes only.
	// Real code should be dynamic; use std::string instead.
	//
        if ( ! cfile ) cfile =  (char*)malloc(20000);
	memset(cfile, 0, 20000);
	filepos = cfile;
	FILE *fp = popen(cmd, "r");
	fread(cfile, 1, 20000-1, fp);
	pclose(fp);
	SetString(cfile);

	// COUNT NUMBER OF LINES IN FILE LOADED
	char *p = cfile;
        tlines = 0;
	while ( *p ) {
	    if ( *p++ == '\n' ) tlines++;
	}
    }
};

int main(int argc, char **argv) {
    MyWindow nix(1024, 700);
    //nix.SetString(cfile);
    nix.LoadCommand(argc == 2 ? argv[1] : "( echo abcdefghijklmnopqrstuvwxyz; pr -e8 -t NixieDisplay.H )");
    nix.resizable(nix);
    nix.SetUpdateSpeed(.10);
    nix.tooltip("You can use up/down arrows, PgUp/PgDn, Home and End to navigate the file");
    nix.show();
    //Fl::add_timeout(2.0, DateTick_CB, (void*)&nix);
    return(Fl::run());
}
