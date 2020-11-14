/************************************************************************

    NixieDisplay.C - a widget for showing multi-line nixie alphanumerics
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
#include <stdio.h>			// fprintf()
#include <stdlib.h>			// exit()
#include "NixieDisplay.H"

// THE DIGIT XPM FILES
#include "xpms/d0.xpm"
#include "xpms/d1.xpm"
#include "xpms/d2.xpm"
#include "xpms/d3.xpm"
#include "xpms/d4.xpm"
#include "xpms/d5.xpm"
#include "xpms/d6.xpm"
#include "xpms/d7.xpm"
#include "xpms/d8.xpm"
#include "xpms/d9.xpm"
#include "xpms/a_a.xpm"
#include "xpms/a_b.xpm"
#include "xpms/a_c.xpm"
#include "xpms/a_d.xpm"
#include "xpms/a_e.xpm"
#include "xpms/a_f.xpm"
#include "xpms/a_g.xpm"
#include "xpms/a_h.xpm"
#include "xpms/a_i.xpm"
#include "xpms/a_j.xpm"
#include "xpms/a_k.xpm"
#include "xpms/a_l.xpm"
#include "xpms/a_m.xpm"
#include "xpms/a_n.xpm"
#include "xpms/a_o.xpm"
#include "xpms/a_p.xpm"
#include "xpms/a_q.xpm"
#include "xpms/a_r.xpm"
#include "xpms/a_s.xpm"
#include "xpms/a_t.xpm"
#include "xpms/a_u.xpm"
#include "xpms/a_v.xpm"
#include "xpms/a_w.xpm"
#include "xpms/a_x.xpm"
#include "xpms/a_y.xpm"
#include "xpms/a_z.xpm"
#include "xpms/lower_a.xpm"
#include "xpms/lower_b.xpm"
#include "xpms/lower_c.xpm"
#include "xpms/lower_d.xpm"
#include "xpms/lower_e.xpm"
#include "xpms/lower_f.xpm"
#include "xpms/lower_g.xpm"
#include "xpms/lower_h.xpm"
#include "xpms/lower_i.xpm"
#include "xpms/lower_j.xpm"
#include "xpms/lower_k.xpm"
#include "xpms/lower_l.xpm"
#include "xpms/lower_m.xpm"
#include "xpms/lower_n.xpm"
#include "xpms/lower_o.xpm"
#include "xpms/lower_p.xpm"
#include "xpms/lower_q.xpm"
#include "xpms/lower_r.xpm"
#include "xpms/lower_s.xpm"
#include "xpms/lower_t.xpm"
#include "xpms/lower_u.xpm"
#include "xpms/lower_v.xpm"
#include "xpms/lower_w.xpm"
#include "xpms/lower_x.xpm"
#include "xpms/lower_y.xpm"
#include "xpms/lower_z.xpm"
#include "xpms/invalid.xpm"
#include "xpms/space.xpm"
#include "xpms/underbar.xpm"
#include "xpms/percent.xpm"
#include "xpms/ampersand.xpm"
#include "xpms/slash.xpm"
#include "xpms/backslash.xpm"
#include "xpms/pipe.xpm"
#include "xpms/colon.xpm"
#include "xpms/semi.xpm"
#include "xpms/plus.xpm"
#include "xpms/dash.xpm"
#include "xpms/equal.xpm"
#include "xpms/star.xpm"
#include "xpms/question.xpm"
#include "xpms/exclamation.xpm"
#include "xpms/period.xpm"
#include "xpms/comma.xpm"
#include "xpms/singlequote.xpm"
#include "xpms/backquote.xpm"
#include "xpms/lparen.xpm"
#include "xpms/rparen.xpm"
#include "xpms/lsquare.xpm"
#include "xpms/rsquare.xpm"
#include "xpms/lcurly.xpm"
#include "xpms/rcurly.xpm"
#include "xpms/less.xpm"
#include "xpms/greater.xpm"
#include "xpms/hash.xpm"
#include "xpms/dollar.xpm"
#include "xpms/dquote.xpm"
#include "xpms/at.xpm"
#include "xpms/carat.xpm"
#include "xpms/approx.xpm"

void NixieDisplay::_Init() {
    _verbose = 0;
    box(FL_FLAT_BOX); 
    color(FL_BLACK); 
    callback(_WinQuit_CB, (void*)this);

    // INITIALIZE
    _tdigits = 0;
    _digits[_tdigits++] = new Digit('\x7f', invalid_xpm);
    _digits[_tdigits++] = new Digit('A', a_a_xpm);
    _digits[_tdigits++] = new Digit('B', a_b_xpm);
    _digits[_tdigits++] = new Digit('C', a_c_xpm);
    _digits[_tdigits++] = new Digit('D', a_d_xpm);
    _digits[_tdigits++] = new Digit('E', a_e_xpm);
    _digits[_tdigits++] = new Digit('F', a_f_xpm);
    _digits[_tdigits++] = new Digit('G', a_g_xpm);
    _digits[_tdigits++] = new Digit('H', a_h_xpm);
    _digits[_tdigits++] = new Digit('I', a_i_xpm);
    _digits[_tdigits++] = new Digit('J', a_j_xpm);
    _digits[_tdigits++] = new Digit('K', a_k_xpm);
    _digits[_tdigits++] = new Digit('L', a_l_xpm);
    _digits[_tdigits++] = new Digit('M', a_m_xpm);
    _digits[_tdigits++] = new Digit('N', a_n_xpm);
    _digits[_tdigits++] = new Digit('O', a_o_xpm);
    _digits[_tdigits++] = new Digit('P', a_p_xpm);
    _digits[_tdigits++] = new Digit('Q', a_q_xpm);
    _digits[_tdigits++] = new Digit('R', a_r_xpm);
    _digits[_tdigits++] = new Digit('S', a_s_xpm);
    _digits[_tdigits++] = new Digit('T', a_t_xpm);
    _digits[_tdigits++] = new Digit('U', a_u_xpm);
    _digits[_tdigits++] = new Digit('V', a_v_xpm);
    _digits[_tdigits++] = new Digit('W', a_w_xpm);
    _digits[_tdigits++] = new Digit('X', a_x_xpm);
    _digits[_tdigits++] = new Digit('Y', a_y_xpm);
    _digits[_tdigits++] = new Digit('Z', a_z_xpm);
    _digits[_tdigits++] = new Digit('a', lower_a_xpm);
    _digits[_tdigits++] = new Digit('b', lower_b_xpm);
    _digits[_tdigits++] = new Digit('c', lower_c_xpm);
    _digits[_tdigits++] = new Digit('d', lower_d_xpm);
    _digits[_tdigits++] = new Digit('e', lower_e_xpm);
    _digits[_tdigits++] = new Digit('f', lower_f_xpm);
    _digits[_tdigits++] = new Digit('g', lower_g_xpm);
    _digits[_tdigits++] = new Digit('h', lower_h_xpm);
    _digits[_tdigits++] = new Digit('i', lower_i_xpm);
    _digits[_tdigits++] = new Digit('j', lower_j_xpm);
    _digits[_tdigits++] = new Digit('k', lower_k_xpm);
    _digits[_tdigits++] = new Digit('l', lower_l_xpm);
    _digits[_tdigits++] = new Digit('m', lower_m_xpm);
    _digits[_tdigits++] = new Digit('n', lower_n_xpm);
    _digits[_tdigits++] = new Digit('o', lower_o_xpm);
    _digits[_tdigits++] = new Digit('p', lower_p_xpm);
    _digits[_tdigits++] = new Digit('q', lower_q_xpm);
    _digits[_tdigits++] = new Digit('r', lower_r_xpm);
    _digits[_tdigits++] = new Digit('s', lower_s_xpm);
    _digits[_tdigits++] = new Digit('t', lower_t_xpm);
    _digits[_tdigits++] = new Digit('u', lower_u_xpm);
    _digits[_tdigits++] = new Digit('v', lower_v_xpm);
    _digits[_tdigits++] = new Digit('w', lower_w_xpm);
    _digits[_tdigits++] = new Digit('x', lower_x_xpm);
    _digits[_tdigits++] = new Digit('y', lower_y_xpm);
    _digits[_tdigits++] = new Digit('z', lower_z_xpm);
    _digits[_tdigits++] = new Digit('0', d0);
    _digits[_tdigits++] = new Digit('1', d1);
    _digits[_tdigits++] = new Digit('2', d2);
    _digits[_tdigits++] = new Digit('3', d3);
    _digits[_tdigits++] = new Digit('4', d4);
    _digits[_tdigits++] = new Digit('5', d5);
    _digits[_tdigits++] = new Digit('6', d6);
    _digits[_tdigits++] = new Digit('7', d7);
    _digits[_tdigits++] = new Digit('8', d8);
    _digits[_tdigits++] = new Digit('9', d9);
    _digits[_tdigits++] = new Digit('+', plus_xpm);
    _digits[_tdigits++] = new Digit('-', dash_xpm);
    _digits[_tdigits++] = new Digit('=', equal_xpm);
    _digits[_tdigits++] = new Digit('_', underbar_xpm);
    _digits[_tdigits++] = new Digit('%', percent_xpm);
    _digits[_tdigits++] = new Digit('&', ampersand_xpm);
    _digits[_tdigits++] = new Digit('/', slash_xpm);
    _digits[_tdigits++] = new Digit('\\', backslash_xpm);
    _digits[_tdigits++] = new Digit('|', pipe_xpm);
    _digits[_tdigits++] = new Digit(':', colon_xpm);
    _digits[_tdigits++] = new Digit(';', semi_xpm);
    _digits[_tdigits++] = new Digit('.', period_xpm);
    _digits[_tdigits++] = new Digit(',', comma_xpm);
    _digits[_tdigits++] = new Digit('\'', singlequote_xpm);
    _digits[_tdigits++] = new Digit('`', backquote_xpm);
    _digits[_tdigits++] = new Digit('!', exclamation_xpm);
    _digits[_tdigits++] = new Digit('?', question_xpm);
    _digits[_tdigits++] = new Digit('(', lparen_xpm);
    _digits[_tdigits++] = new Digit(')', rparen_xpm);
    _digits[_tdigits++] = new Digit('[', lsquare_xpm);
    _digits[_tdigits++] = new Digit(']', rsquare_xpm);
    _digits[_tdigits++] = new Digit('{', lcurly_xpm);
    _digits[_tdigits++] = new Digit('}', rcurly_xpm);
    _digits[_tdigits++] = new Digit('<', less_xpm);
    _digits[_tdigits++] = new Digit('>', greater_xpm);
    _digits[_tdigits++] = new Digit('*', star_xpm);
    _digits[_tdigits++] = new Digit(' ', space_xpm);
    _digits[_tdigits++] = new Digit('#', hash_xpm);
    _digits[_tdigits++] = new Digit('$', dollar_xpm);
    _digits[_tdigits++] = new Digit('"', dquote_xpm);
    _digits[_tdigits++] = new Digit('@', at_xpm);
    _digits[_tdigits++] = new Digit('^', carat_xpm);
    _digits[_tdigits++] = new Digit('~', approx_xpm);
    _space = new Digit(' ', space_xpm);

    _speed = 0.0;
    _forceupper = 0;
    memset(_currdig, 0, sizeof(_currdig));
    memset(_lastdig, 0, sizeof(_lastdig));
    
    SetString("-- -- -- --");
    SetUpdateSpeed(10.0);
}

// NUMERIC VALUE (0-9) RETURNS APPROPRIATE RGB DIGIT
Digit *NixieDisplay::Value2Digit(char val) {
    val = _forceupper ? toupper(val) : val;
    for ( int t=0; t<_tdigits; t++ ) {
        if ( _digits[t]->IsChar(val) ) {
	    return(_digits[t]);
	}
    }
    return(_digits[0]);				// unknown character
}

void NixieDisplay::_Tick_CB(void *data) {
    NixieDisplay *o = (NixieDisplay*)data;
    o->_Tick_CB2();
}

void NixieDisplay::_Tick_CB2() {
    /// I YAM HERE: IMPLEMENT FADE ALGORITHM HERE.
    ///             Make an IMAGE BUFFER, such that the old pixels are
    ///             slowly 'washed away' by dividing them away.
    ///             New pixels (or 'on' pixels) should get applied
    ///             similarly
    ///     pixel = pixel * .5;			// fade away old pixel value
    ///     pixel = pixel + newpixel;		// add in new value
    ///     if ( pixel > 1.0 ) pixel = 1;	// don't go > 1
    ///

    //    if ( memcmp(_currdig, _lastdig, sizeof(_currdig)) != 0 ) {
    //        Fl::repeat_timeout(0.5, _Tick_CB, (void*)this);		// changed? 
    //        if ( _verbose ) fprintf(stderr, "NixieDisplay: Tick (0.5.. diff: %p<->%p)\n", &_currdig, &_lastdig);
    //    } else {
    //        Fl::repeat_timeout(_speed, _Tick_CB, (void*)this);	// same
    //        if ( _verbose ) fprintf(stderr, "NixieDisplay: Tick (%0.2f)\n", _speed);
    //    }
    redraw();
}

// DRAW TWO DIGITS COMPED OVER ONE ANOTHER AT X/Y POSITION
//    Returns 1 if comp occurred.
//
int NixieDisplay::_DrawDigitComp(Digit *a, Digit *b, int x, int y) {
    if ( !a ) a = _space;	// NULL? use a space
    if ( !b ) b = _space;	// NULL? use a space

    // DONT COMP UNLESS DIGITS ARE DIFFERENT
    if ( a == b ) {
        // SAME? JUST DRAW IT
	a->Draw(x, y);
	return(0);
    } else {
        // DIFFERENT? COMP
	static Digit tmp;
	tmp.Comp(*a, *b, 1.0, 1.0);
	tmp.Draw(x, y);
	return(1);
    }
}

// WINDOW DRAW HANDLER
void NixieDisplay::draw(void) {
    // DRAW WINDOW
    Fl_Double_Window::draw();

    // REDRAW ENTIRE SCREEN
    //    Comp any digits that changed.
    //
    int ypos = YMARGIN;
    for ( int dy=0; dy<YMAX; dy++ ) {
	int xpos = XMARGIN;
	for ( int dx=0; dx<XMAX; dx++ ) {
	    _DrawDigitComp(_currdig[dy][dx], _lastdig[dy][dx], xpos, ypos);
	    _lastdig[dy][dx] = _currdig[dy][dx];
	    xpos += FONTWIDTH + 2;
	}
	ypos += FONTHEIGHT;
    }
    if ( _verbose ) fprintf(stderr, "NixieDisplay: draw()\n"); 
}

