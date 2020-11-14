#include <time.h>	// time()..
#include <ctype.h>	// tolower()
#include <stdio.h>	// fopen()..
#include <string.h>	// sprintf()..
#include <errno.h>	// strerror()
#include <sys/stat.h>	// stat()
#ifndef _WIN32
#include <unistd.h>	// malloc()..
#include <stdlib.h>	// exit()..
#else /*_WIN32*/
#include <windows.h>
#include <FL/x.H>	// fl_xid
#define popen _popen
#define pclose _pclose
#endif /*_WIN32*/
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>

#include "NixieClock.H"

#ifdef _WIN32
#define snprintf _snprintf
#endif

////
//// MENU CALLBACKS
////
void NixieClock::AlwaysTopWindow_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetAbove(1);
    nc->SaveSettings();
}

void NixieClock::RegularWindow_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetAbove(0);
    nc->SaveSettings();
}

void NixieClock::Quit_CB(Fl_Widget *w, void *userdata) {
    _exit(0);
}

void NixieClock::TimeFormatDefault_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetTimeString("%H:%M %a %b %d");
    nc->SetTickSpeed(29.0);
    nc->SaveSettings();
}

void NixieClock::TimeFormat_HM_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetTimeString("%H:%M");
    nc->SetTickSpeed(29.0);
    nc->SaveSettings();
}

void NixieClock::TimeFormat_HMS_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetTimeString("%H:%M:%S");
    nc->SetTickSpeed(0.1);
    nc->SaveSettings();
}

void NixieClock::TimeFormat_HM_MDY_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetTimeString("%H:%M %m/%d/%y");
    nc->SetTickSpeed(29.0);
    nc->SaveSettings();
}

void NixieClock::TimeFormat_HM_DMY_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetTimeString("%H:%M %a %b %m %Y");
    nc->SetTickSpeed(29.0);
    nc->SaveSettings();
}

void NixieClock::TimeFormat_HM_A_MDY_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetTimeString("%H:%M %a %m/%d/%y");
    nc->SetTickSpeed(29.0);
    nc->SaveSettings();
}

void NixieClock::TimeFormat_HM_D_M_MDY(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetTimeString("%H:%M %a %b %m/%d/%y");
    nc->SetTickSpeed(29.0);
    nc->SaveSettings();
}

void NixieClock::TimeFormat_MDY_HM_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetTimeString("%m/%d/%y %H:%M");
    nc->SetTickSpeed(29.0);
    nc->SaveSettings();
}

void NixieClock::TimeFormat_A_MDY_HM_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetTimeString("%a %m-%d-%y %H:%M");
    nc->SetTickSpeed(29.0);
    nc->SaveSettings();
}

void NixieClock::TimeFormat_HM_ABMY_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetTimeString("%H:%M %a %b %d %Y");
    nc->SetTickSpeed(29.0);
    nc->SaveSettings();
}

void NixieClock::TimeFormat_HM_AB_MDY_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetTimeString("%H:%M %a %b %m/%d/%Y");
    nc->SetTickSpeed(29.0);
    nc->SaveSettings();
}

void NixieClock::TimeFormatUnix_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetTimeString("%a %d %b %Y %H:%M:%S %z");
    nc->SetTickSpeed(0.1);
    nc->SaveSettings();
}

void NixieClock::TimeFormatLocale_CB(Fl_Widget *w, void *userdata) {
    NixieClock *nc = (NixieClock*)userdata;
    nc->SetTimeString("%c");
    nc->SetTickSpeed(0.1);
    nc->SaveSettings();
}

// FIGURE OUT DIGIT WIDTH OF SPECIFIED TIME STRING
int NixieClock::TimeStringWidth() {
    char s[80];
    time_t timt = time(NULL);
    struct tm *tmbuf = localtime(&timt);
    strftime(s, sizeof(s)-1, _timestr, tmbuf);
    return(strlen(s));
}

void NixieClock::SetTimeString(const char *val) {
    strncpy(_timestr, val, sizeof(_timestr)-1);
    // Use time string's width to determine window size
    int numdigits = TimeStringWidth();
    int W = (FONTWIDTH + 2) * numdigits + (XMARGIN * 2);
    int H = FONTHEIGHT + 3;
    int X = x();
    int Y = y();

    // If nearer to right edge of screen,
    // maintain distance from right edge of clock
    // when user makes interactive changes to time format.
    if ( x()+w() > (Fl::w()/2) ) {
        int xdist = Fl::w() - (x() + w());
        X = Fl::w() - W - xdist;
    }
    resize(X,Y,W,H);
    UpdateTime();
}

// SET BORDER AROUND WINDOW
void NixieClock::SetBorder(int val) {
    _border = val;
    // (This doesn't work in FLTK 1.1.7 or older)
    border(val);
}

// MAKE THIS WINDOW ABOVE ALL OTHERS
void NixieClock::SetAbove(int val) {
    _above = val;

#ifdef _WIN32
     SetWindowPos(fl_xid(this), val ? HWND_TOPMOST : HWND_NOTOPMOST, 0,0,0,0, SWP_NOSIZE|SWP_NOMOVE);
#endif /*_WIN32*/
}

void NixieClock::UpdateTime() {
    // NEW TIME STRING
    char s[256];
    static char last_s[256] = "";

    time_t timt = time(NULL);
    struct tm *tmbuf = localtime(&timt);
    if (_countdown) {
        // epoch countdown
        timt = _countdown - timt;
	timt = ( timt < 0 ) ? 0 : timt; 	// stop counter at zero
	tmbuf = gmtime(&timt);
    } else {
	tmbuf = localtime(&timt);
    }
    strftime(s, sizeof(s)-1, GetTimeStr(), tmbuf);

    // CHANGE?
    if ( strcmp(last_s, s) != 0 ) {
	SetString(s);
	strcpy(last_s, s);
	redraw();
	_justupdate = 1;
    }
}

// CALLBACK TO UPDATE TIME
void NixieClock::Tick_CB(void *data) {
    NixieClock *nc = (NixieClock*)data;
    nc->UpdateTime();
    Fl::repeat_timeout(nc->_tickspeed, nc->Tick_CB, data);	// repeat timer
    if ( nc->_verbose ) fprintf(stderr, "NixieClock: Tick (repeat %.02f)\n", nc->_tickspeed);
}

int NixieClock::handle(int e) {				// event handler method
    static int xoff = 0, yoff = 0;
    int ret = NixieDisplay::handle(e);
    int isctrl = Fl::event_state() & FL_CTRL;
    switch ( e ) {
	// DOWNCLICK IN WINDOW CREATES CURSOR OFFSETS
	case FL_PUSH:
	    if ( Fl::event_button() == FL_LEFT_MOUSE && !isctrl ) {
		xoff = x() - Fl::event_x_root();
		yoff = y() - Fl::event_y_root();
		ret = 1;
	    } else if ( Fl::event_button() == FL_RIGHT_MOUSE ||
	                ( Fl::event_button() == FL_LEFT_MOUSE && isctrl ) ) {
		 time_t timt = time(NULL);
		 struct tm *tmbuf = localtime(&timt);
	         char HM[80];           strftime(HM,           80, "%H:%M",                    tmbuf);	// 22:00
	         char HMS[80];          strftime(HMS,          80, "%H:%M:%S",                 tmbuf);	// 22:00:59
	         char HM_ABD[80];       strftime(HM_ABD,       80, "%H:%M %a %b %d",           tmbuf);	// 22:00 Fri Oct 9 (default time)
	         char HM_MDY[80];       strftime(HM_MDY,       80, "%H:%M %m/%d/%y",           tmbuf);	// 22:00 M/D/Y
	         char HM_A_MDY[80];     strftime(HM_A_MDY,     80, "%H:%M %a %m/%d/%y",        tmbuf);	// 22:00 A M/D/Y
		 char HM_DMY[80];       strftime(HM_DMY,       80, "%H:%M %a %b %m %Y",        tmbuf);  // 22:00 Fri Oct 9 2015
	         char HM_D_M_MDY[80];   strftime(HM_D_M_MDY,   80, "%H:%M %a %b %m/%d/%y",     tmbuf);	// 22:00 Fri Oct 10/09/15
	         char MDY_HM[80];       strftime(MDY_HM,       80, "%m/%d/%y %H:%M",           tmbuf);	// 10/09/15 22:00
	         char A_MDY_HM[80];     strftime(A_MDY_HM,     80, "%a %m-%d-%y %H:%M",        tmbuf);	// Fri 10/09/15 22:00
	         char HM_ABMY[80];      strftime(HM_ABMY,      80, "%H:%M %a %b %d %Y",        tmbuf);	// 22:00 Fri Oct 9 2015
	         char HM_AB_MDY[80];    strftime(HM_AB_MDY,    80, "%H:%M %a %b %m/%d/%Y",     tmbuf);	// 22:00 Fri Oct 10/09/2015 <- greg's favorite
	         char UNIX[80];         strftime(UNIX,         80, "%a %d %b %Y %H:%M:%S %z",  tmbuf);	// <unix>
	         char LOCALE[80];       strftime(LOCALE,       80, "%c",                       tmbuf);	// <Locale specific>
		 Fl_Menu_Item rclick_menu[] = {
#ifdef _WIN32
		     { "Always top window",               0, AlwaysTopWindow_CB,       (void*)this },
		     { "Regular window",                  0, RegularWindow_CB,         (void*)this },
#else
		     { "Always top window",               0, AlwaysTopWindow_CB,       (void*)this, FL_MENU_INACTIVE },
		     { "Regular window",                  0, RegularWindow_CB,         (void*)this, FL_MENU_INACTIVE },
#endif
		     { "Time Format",                     0, 0,                        (void*)0,    FL_SUBMENU },
			 { HM,                            0, TimeFormat_HM_CB,         (void*)this },
			 { HMS,                           0, TimeFormat_HMS_CB,        (void*)this },
			 { HM_ABD,                        0, TimeFormatDefault_CB,     (void*)this },
			 { HM_MDY, 	                  0, TimeFormat_HM_MDY_CB,     (void*)this },
		         { HM_DMY,                        0, TimeFormat_HM_DMY_CB,     (void*)this },
			 { HM_A_MDY,                      0, TimeFormat_HM_A_MDY_CB,   (void*)this },
	                 { HM_D_M_MDY,                    0, TimeFormat_HM_D_M_MDY,    (void*)this },
			 { MDY_HM,                        0, TimeFormat_MDY_HM_CB,     (void*)this },
			 { A_MDY_HM,                      0, TimeFormat_A_MDY_HM_CB,   (void*)this },
			 { HM_ABMY,                       0, TimeFormat_HM_ABMY_CB,    (void*)this },
			 { HM_AB_MDY,                     0, TimeFormat_HM_AB_MDY_CB,  (void*)this },
			 { UNIX,                          0, TimeFormatUnix_CB,        (void*)this },
			 { LOCALE,                        0, TimeFormatLocale_CB,      (void*)this, FL_MENU_DIVIDER },
		     { 0 },
		     { "Quit",                            0, Quit_CB,                  (void*)this },
		     { 0 }
		 };
		 const Fl_Menu_Item *m = rclick_menu->popup(Fl::event_x(), Fl::event_y(), 0, 0, 0);
		 if ( m ) m->do_callback(0, m->user_data());
		 return(1);
	    }
	    break;

	case FL_DRAG:
	    if ( Fl::event_button() == FL_LEFT_MOUSE && !isctrl ) {
		// DRAG THE WINDOW AROUND THE SCREEN
		position(xoff + Fl::event_x_root(), yoff + Fl::event_y_root());
		redraw();
		ret = 1;
	    }
	    break;

	case FL_RELEASE:
	    if ( Fl::event_button() == FL_LEFT_MOUSE && !isctrl ) {
		if ( SaveSettings() < 0 ) {		// save new position
		    fprintf(stderr, "nixieclock: couldn't save settings: %s\n", GetErrmsg());
		}
		show();		// raise
		ret = 1;
	    }
	    break;
    }
    return(ret);
}

void NixieClock::SetTickSpeed(float val) {
    Fl::remove_timeout(Tick_CB, (void*)this);
    _tickspeed = val;
    Fl::add_timeout(_tickspeed, Tick_CB, (void*)this);
    if ( _verbose ) fprintf(stderr, "NixieClock::SetTickSpeed: add_timeout in %.02f\n", _tickspeed);
}

// LOAD NIXIECLOCK SETTINGS FROM FILENAME
//    Returns -1 on error, _errmsg has reason.
//
int NixieClock::Load(const char *filename) {
    int err = 0;
    FILE *fp = fopen(filename, "r");
    if ( fp == 0 ) {
	// sizeof(_errmsg):512
        sprintf(_errmsg, "%.250s: %.250s", filename, strerror(errno));
        err = -1;
    } else {
        float f;
	int a,b,c,d;
        char s[512], s1[512];
        while ( fgets(s, sizeof(s)-1, fp) ) {
	    if ( s[0] == '\n' || s[0] == '#' ) continue;
	    if ( sscanf(s, "xywh %d %d %d %d", &a,&b,&c,&d) == 4 ) {
	        if ( ! _geometryforce ) {
		   resize(a,b,c,d);
		}
	    }
	    else if ( sscanf(s, "above %d", &a) == 1 ) {
	        SetAbove(a);
	    }
	    else if ( sscanf(s, "border %d", &a) == 1 ) {
	        SetBorder(a);
	    }
	    else if ( sscanf(s, "timestring %511[^\n]", s1) == 1 ) {
	        SetTimeString(s1);
	    }
	    else if ( sscanf(s, "tickspeed %f", &f) == 1 ) {
	        SetTickSpeed(f);
	    }
	    // Ignore unknown commands
	}
	fclose(fp);
    }
    return(err);
}

// SAVE THE STATE OF THE CLOCK
//    This way it remembers its last settings
//    even if the machine reboots.
//    Returns -1 on error, _errmsg has reason.
//
int NixieClock::Save(const char *filename) {
    int err = 0;
    char *tmpfilename = (char*)malloc(strlen(filename) + 20);
    sprintf(tmpfilename, "%s.tmp", filename);
    FILE *fp = fopen(tmpfilename, "w");
    if ( !fp ) {
        err = -1;
	// sizeof(_errmsg):512
        sprintf(_errmsg, "%.250s: %.250s", tmpfilename, strerror(errno));
    } else {
	fprintf(fp, "# Nixieclock settings file\n");
	fprintf(fp, "#    For docs on 'timestring' format, see strftime(3)\n");
	fprintf(fp, "#\n");
        fprintf(fp, "xywh %d %d %d %d\n", x(), y(), w(), h());
        fprintf(fp, "above %d\n", _above);
        fprintf(fp, "border %d\n", _border);
        fprintf(fp, "timestring %s\n", _timestr);
        fprintf(fp, "tickspeed %.02f\n", _tickspeed);
	fclose(fp);

#ifdef _WIN32
	// WINDOWS: CANT RENAME IF DESTINATION EXISTS >:(
	// Must remove old first, if it exists
	//
	if ( Exists(filename) ) {
	    if ( ::remove(filename) < 0 ) {
		// sizeof(_errmsg):512
		sprintf(_errmsg, "remove(%.200s): %.80s", filename, strerror(errno));
		err = -1;
	    }
	}
	if ( ! err ) {
	    if ( ::rename(tmpfilename, filename) < 0 ) {
		// sizeof(_errmsg):512
		sprintf(_errmsg, "rename(%.200s,%.200s): %.80s", tmpfilename, filename, strerror(errno));
		err = -1;
	    }
	}
#else
	if ( ::rename(tmpfilename, filename) < 0 ) {
	    // sizeof(_errmsg):512
	    sprintf(_errmsg, "rename(%.200s,%.200s): %.80s", tmpfilename, filename, strerror(errno));
	    err = -1;
	}
#endif
    }
    free((void*)tmpfilename);
    return(err);
}

// RETURN THE FILENAME OF THE NIXIECLOCK SETTINGS FILE
//     Returns -1 if could not be determined.
//
int NixieClock::GetSettingsFilename(char *filename, int maxsize) {
    if ( getenv("HOME") ) {
        snprintf(filename, maxsize-1, "%s/.nixieclock", getenv("HOME"));
	filename[maxsize-1] = 0;
    } else if ( getenv("HOMEPATH") ) {
        char *homedrive = getenv("HOMEDRIVE");
        char *homepath  = getenv("HOMEPATH");
	if ( homepath && homedrive ) {
	    snprintf(filename, maxsize-1, "%s%s/.nixieclock", homedrive, homepath);
	    filename[maxsize-1] = 0;
	} else {
	    return(-1);
	}
    } else {
        return(-1);
    }
    return(0);
}

// SEE IF FILENAME EXISTS
int NixieClock::Exists(const char *filename) {
    struct stat buf;
    if ( stat(filename, &buf) < 0 ) return(0);
    return(1);
}

// LOAD NIXIECLOCK SETTINGS FROM ~/.nixieclock FILE
int NixieClock::LoadSettings() {
    // Get settings filename
    char filename[512];
    if ( GetSettingsFilename(filename, sizeof(filename)-1) < 0 ) {
        return(0);
    }
    // If the file exists, load it
    if ( Exists(filename) ) {
	return(Load(filename));
    }
    return(0);
}

// SAVE NIXIECLOCK SETTINGS TO ~/.nixieclock FILE
int NixieClock::SaveSettings() {
    if ( _nosave ) return(0);		// ignore save requests
    // Get settings filename
    char filename[512];
    if ( GetSettingsFilename(filename, sizeof(filename)-1) < 0 ) {
        return(0);
    }
    // Save settings
    return(Save(filename));
}

void NixieClock::SetVerbose(int val) {
    _verbose = val;
    NixieDisplay::SetVerbose(val);
}

void NixieClock::SetGeometryForce(int val) {
    _geometryforce = val;
}

void NixieClock::Init() {
    _timestr[0]    = 0;
    _verbose       = 0;
    _geometryforce = 0;
    _tickspeed    = 29.0;
    _border       = 0;
    _above        = 0;
    _justupdate   = 0;
    _errmsg[0]    = 0;
    _countdown    = 0;
    _nosave       = 0;
    clear_border();
    ForceUpperCase(1);
    SetTimeString("%H:%M %a %b %d");		// default time format if unspecified

    // SETUP TIMER TO UPDATE CLOCK'S TIME STRING
    Fl::add_timeout(_tickspeed, Tick_CB, (void*)this);
    if ( _verbose ) fprintf(stderr, "NixieClock::Init: add_timeout in %.02f\n", _tickspeed);
}
