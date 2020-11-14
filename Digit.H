#ifndef _DIGIT_H
#define _DIGIT_H

/************************************************************************

    Digit.H - a generic image of a digit class
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

#include <ctype.h>	// tolower()
#include <FL/Fl.H>
#include <FL/Fl_Pixmap.H>
#include <FL/fl_draw.H>

// IMAGE OF A DIGIT
//     Imports an XPM file.
//     Keeps track of the resulting RGB array suitable for fl_draw_image(),
//     and maintains the width and height.
//
class Digit {
    char c;				// character's ascii value
    int w, h;				// width/height of digit
    unsigned char *pixels;		// RGB array of pixels (w*h*4)

    // PRIVATE METHODS
    unsigned char *_Xpm2Rgb(const char **xpm, int& w, int& h);

public:
    Digit();
    Digit(char ascii, const char **xpm);	// ctor imports xpm
    ~Digit();

    int IsChar(char val) {
        return(val == c ? 1 : 0);		// tolower(val)
    }

    void Comp(Digit& a, Digit& b, float alpha_a, float alpha_b);
    void Draw(int x, int y) {
	fl_draw_image(pixels, x, y, w, h);
    }
};

#endif /* _DIGIT_H */
