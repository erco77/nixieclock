/************************************************************************

    Digit.C - a generic image of a digit class
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

#include <stdio.h>
#include <stdlib.h>
#include "Digit.H"

// CTOR
Digit::Digit() {
    pixels = NULL;
    w = 0;
    h = 0;
    c = 0;
}

// CONVERT ASCII/XPM -> RGB IMAGE SUITABLE FOR WRITEPIXELS
Digit::Digit(char ascii, const char **xpm) {
    c = ascii;
    pixels = _Xpm2Rgb(xpm, w, h);
}

// FREE THE DIGIT IMAGE
Digit::~Digit() {
    if ( pixels ) free(pixels);
}

// CONVERT XPM TO RGB
unsigned char *Digit::_Xpm2Rgb(const char **xpm, int& w, int& h) {
    static long vals[256], val;
    int t, x, y, colors, cpp;
    unsigned char c;
    unsigned char *savergb, *rgb;

    // PARSE HEADER
    if ( sscanf(xpm[0], "%d%d%d%d", &w, &h, &colors, &cpp) != 4 ) {
        fprintf(stderr, "nixieclock: bad xpm header: '%s'\n", xpm[0]);
	exit(1);
    }

    savergb = rgb = (unsigned char*)malloc(h * w * 3);

    // LOAD XPM COLORMAP LONG ENOUGH TO DO CONVERSION
    for ( t=0; t<colors; t++ ) {
	sscanf(xpm[t+1], "%c c #%lx", &c, &val);
	vals[c] = val;
    }

    // COLORMAP -> RGB CONVERSION
    //    Get low 3 bytes from vals[]
    //
    const char *p;
    for ( y=h-1; y>=0; y-- ) {
	for ( p = xpm[1+colors+(h-y-1)], x=0; x<w; x++, rgb+=3 ) {
	    val = vals[(int)(*p++)];
	    *(rgb+2) = val & 0xff; val >>= 8;  // 2:B
	    *(rgb+1) = val & 0xff; val >>= 8;  // 1:G
	    *(rgb+0) = val & 0xff;             // 0:R
	}
    }
    return(savergb);
}

// COMP ONE DIGIT OVER ANOTHER
//    Both input digits must be the same size.
//    Neither digit's pixels are affected; a copy is made.
//
void Digit::Comp(Digit& a, Digit& b, float alpha_a = 1.0, float alpha_b = 1.0) {
    if ( pixels ) free(pixels);

    int size = a.w * a.h * 3;
    unsigned char 
         *ap = a.pixels,
         *bp = b.pixels,
	 *rgb;

    // GET WIDTH, HEIGHT, PIXELS
    w = a.w;
    h = a.h;
    pixels = rgb = (unsigned char*)malloc(size);

    // COMP
    for ( int t=0; t<size; t++ ) {
        *rgb++ = (unsigned char)(*ap++ * alpha_a) | 
		 (unsigned char)(*bp++ * alpha_b);
    }
}
