#ifndef _NIXIEDISPLAY_H
#define _NIXIEDISPLAY_H

/************************************************************************

    NixieDisplay.H - a widget for showing multi-line nixie alphanumerics
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

#include <stdlib.h>		// exit()
#include <string.h>		// memset(), memcpy..
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Menu_Item.H>
#include "Digit.H"

#define XMARGIN		5		// margin around clock on X
#define YMARGIN		5		// margin around clock on Y
#define FONTWIDTH	23
#define FONTHEIGHT	39
#define XMAX 		80
#define YMAX		40

class NixieDisplay : public Fl_Double_Window {
    int _tdigits;			// total digits loaded
    Digit *_digits[128],		// rgb digit array (ascii)
          *_currdig[YMAX][XMAX],	// digit values for each position
          *_lastdig[YMAX][XMAX],	// last digits (to detect deltas)
	  *_space;			// space
    float _speed;			// update speed
    int   _forceupper;			// force all text to uppercase
    int   _verbose;			// verbose messages

    static void _WinQuit_CB(Fl_Widget*, void *data) {
        exit(0);
    }

    void _Init();

    Digit *Value2Digit(char val);

    int _DrawDigitComp(Digit* a, Digit* b, int x, int y);

    static void _Tick_CB(void *data);
    void _Tick_CB2();

    // MENU CALLBACKS
    static void AlwaysTopWindow_CB(Fl_Widget *w, void *userdata);
    static void RegularWindow_CB(Fl_Widget *w, void *userdata);
    static void Quit_CB(Fl_Widget *w, void *userdata);

protected:
    void draw(void);			// screen draw method

public:
    NixieDisplay(int X,int Y,int W,int H,const char *L=0):Fl_Double_Window(X,Y,W,H,L) {
        _Init();
    }
    NixieDisplay(int W,int H,const char *L=0):Fl_Double_Window(W,H,L) {
        _Init();
    }

    void ForceUpperCase(int val) {
        _forceupper = val;
    }

    void SetUpdateSpeed(float f) {
	Fl::remove_timeout(_Tick_CB, (void*)this);
	_speed = f;
	Fl::add_timeout(_speed, _Tick_CB, (void*)this);
    }

    void SetString(const char *s) {
        // SAVE OLD _dig ARRAY
	memcpy(_lastdig, _currdig, sizeof(_currdig));

	// CONVERT STRING TO NEW DIG ARRAY
	memset(_currdig, 0, sizeof(_currdig));
	for ( int done=0, y=0, x=0, t=0; !done && s[t]; t++ ) {
	    if ( y > YMAX ) break;
	    switch ( s[t] ) {
	         case '\r':
		     x = 0;
		     break;
		 case '\n':
		     x = 0;
		     if ( ++y >= YMAX ) {
		         done = 1;
			 break;
		     }
		     break;
		 default: 
		     _currdig[y][x] = Value2Digit(s[t]);
		     if ( ++x >= XMAX ) {
		         x = 0;
			 if ( ++y >= YMAX ) {
			     done = 1;
			     break;
			 }
		     }
		     break;
	     }
	}
	redraw();
	Fl::remove_timeout(_Tick_CB, (void*)this);
	Fl::add_timeout(0.5, _Tick_CB, (void*)this);
    }

    void SetVerbose(int val) { _verbose = val; }
};
#endif /* _NIXIEDISPLAY_H */
