# nixieclock

A nixietube desktop clock. Runs on Mac, Linux, Windows.

![Image of nixieclock](https://raw.githubusercontent.com/erco77/nixieclock/main/docs/images/nixiedate-anim.gif)

The nixieclock program is a simple little desktop clock that shows the
current time using a 'nixie tube' style display, and can display the
time in various time formats as set by the `-s` flag.

It can be run from the command line with `./nixieclock` (or on windows, `nixieclock.exe`), and moved around on the screen by click and dragging it to a new position. When closed and re-opened, it remembers the last position and time format you configured last.

You can choose different time formats by right clicking, and choose from different presets:
![Popup Menu](https://raw.githubusercontent.com/erco77/nixieclock/main/docs/images/nixieclock-popup-menu.png)

..or use the command line `-s` flag to create custom time formats using strftime(3) format strings, e.g.

Command | Result
------- | -------
`nixieclock -s "%H:%M %m/%d/%y"` | ![Time format example](https://raw.githubusercontent.com/erco77/nixieclock/main/docs/images/nixiedate-digitformat.jpg)
`nixieclock -s "%H:%M %a %m/%d/%y"` | ![Time format example](https://raw.githubusercontent.com/erco77/nixieclock/main/docs/images/nixiedate-hm_dow_mm_dd_yy.jpg)
`nixieclock -s "%a %b %m %H:%M:%S %Y"` | ![Time format example](https://raw.githubusercontent.com/erco77/nixieclock/main/docs/images/nixiedate-unix-format.jpg)

For more info, see the `strftime(3)` and `nixieclock(1)` man pages, or the `docs/nixieclock.html` html documentation, or invoke nixieclock with the `-help` flag.

Also, there is also a text file viewer using nixie digits; try running:
`./nixie-text-viewer` The Up/Down arrow keys move through the source code
a line at a time, PgUp/PgDn to move a page at a time, Home/End to move
quickly to top/bottom of file.

The '`NixieDisplay`' class that can be used in your own applications to
do general display of multiline ASCII text.

For instance, the '`nixie-text-viewer`' demo shows the program's own source
code in a text viewer using the same nixie font used by the clock..
![Nixie text viewer screenshot](https://raw.githubusercontent.com/erco77/nixieclock/main/docs/images/nixie-text-viewer.jpg)

## LICENSE

This code is licensed as GPL3; see `LICENSE` file for details.

## BUGS

    To report bugs, create an 'issue' on the project's github page:
    https://github.com/erco77/nixieclock

## DOCUMENTATION

See `./docs/nixieclock.html`

## WEBSITE

    github:  https://github.com/erco77/nixieclock
    webpage: http://seriss.com/people/erco/unixtools/nixieclock

## CONTRIBUTORS

* Greg Ercolano - nixieclock application (first written for the SGI + Linux, later OSX and Windows)
* Vance Kochenderfer - man page
* Lemon - freebsd and countdown patch
* Terry mrtembryyahoocom - mods for mingw linux cross compile, fixes
