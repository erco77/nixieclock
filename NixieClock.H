#ifndef _NIXIECLOCK_H
#define _NIXIECLOCK_H

/************************************************************************

    NixieClock.H - a class to manage clock oriented time keeping
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

*************************************************************************/
#include "NixieDisplay.H"
#include <FL/Fl_Menu_Button.H>

class NixieClock : public NixieDisplay {
    char   _timestr[256];
    int    _verbose;
    int    _above;
    int    _geometryforce;
    int    _border;
    int    _justupdate;
    float  _tickspeed;
    time_t _countdown;
    int    _nosave;
    char   _errmsg[512];

    // MENU CALLBACKS
    static void AlwaysTopWindow_CB(Fl_Widget *w, void *userdata);
    static void RegularWindow_CB(Fl_Widget *w, void *userdata);
    static void Quit_CB(Fl_Widget *w, void *userdata);
    static void TimeFormatDefault_CB(Fl_Widget *w, void *userdata);
    static void TimeFormat_HM_CB(Fl_Widget *w, void *userdata);
    static void TimeFormat_HMS_CB(Fl_Widget *w, void *userdata);
    static void TimeFormat_HM_DMY_CB(Fl_Widget *w, void *userdata);
    static void TimeFormat_HM_MDY_CB(Fl_Widget *w, void *userdata);
    static void TimeFormat_HM_A_MDY_CB(Fl_Widget *w, void *userdata);
    static void TimeFormat_HM_D_M_MDY(Fl_Widget *w, void *userdata);
    static void TimeFormat_MDY_HM_CB(Fl_Widget *w, void *userdata);
    static void TimeFormat_A_MDY_HM_CB(Fl_Widget *w, void *userdata);
    static void TimeFormat_HM_ABMY_CB(Fl_Widget *w, void *userdata);
    static void TimeFormat_HM_AB_MDY_CB(Fl_Widget *w, void *userdata);
    static void TimeFormatUnix_CB(Fl_Widget *w, void *userdata);
    static void TimeFormatLocale_CB(Fl_Widget *w, void *userdata);

    // OTHER CALLBACKS
    static void Tick_CB(void *data);

    void Init();
    int TimeStringWidth();
    int Load(const char *filename);
    int Save(const char *filename);
    int Exists(const char *filename);
    int GetSettingsFilename(char *filename, int filenamesz);
    const char *GetTimeStr() const { return(_timestr); }

public:
    // CTOR
    NixieClock(int X,int Y,int W,int H,const char*L):NixieDisplay(X,Y,W,H,L) {
        Init();
    }
    // CTOR
    NixieClock(int W,int H,const char*L):NixieDisplay(W,H,L) {
        Init();
    }

    void SetVerbose(int val);
    void SetGeometryForce(int val);
    void SetTickSpeed(float val);
    void SetBorder(int val);
    void SetAbove(int val);
    void SetTimeString(const char *val);
    void SetCountdown(time_t val) { _countdown = val; }
    void SetNoSave(int val) { _nosave = val; }
    void UpdateTime();

    int GetAbove() const { return(_above); }

    int LoadSettings();
    int SaveSettings();

    int handle(int e);

    const char *GetErrmsg() const {
        return(_errmsg);
    }
    // intercept redraws
    //    Most of the time we're redrawn by the timer tick.
    //    But sometimes we're redrawn when a machine wakes up from a long sleep
    //    (notebook lid opened). If so, _justupdate won't be set, so force call
    //    to UpdateTime() to get rid of the stale time from the display.
    //
    void draw() {
        if ( !_justupdate ) {
	    UpdateTime();		// makes sure clock quickly updates time if machine just woken up
	}
        NixieDisplay::draw();
	_justupdate = 0;
    }
};

#endif
