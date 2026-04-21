#ifndef __ANSI_CODES__
#define __ANSI_CODES__

/*
    Codes here are for a VT220-style terminal.
*/

#define ESC         "\x1B"
#define CSI         "\x1B["

#define C_CLEAR     CSI"2J"CSI"0;0H"
#define C_NORMAL    CSI"0m"
#define C_BOLD      CSI"1m"
#define C_FAINT     CSI"2m"
#define C_REVERSE   CSI"7m"

// DEC VT100/VT220
#define C_SAVE      ESC"7"
#define C_RESTORE   ESC"8"

#define C_CLEARLINE CSI"2K"
#define C_POSITION(l,c) CSI l ";" c "H"

#endif