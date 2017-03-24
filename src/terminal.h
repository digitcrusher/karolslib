/*
 * terminal.h
 * karolslib Source Code
 * Available on Github
 *
 * Copyright (C) 2017 Karol "digitcrusher" Łacina
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _KAROLSLIB_TERMINAL_H_
#define _KAROLSLIB_TERMINAL_H_
#if defined(__linux__)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#elif defined(_WIN32)
#include <windows.h>
#include <pthread.h>
#endif

#define TERMINAL_GET_IBUFF_CHAR(t, x, y) ( *((t)->ibuff+(x)+(y)*(t)->buffw) )
#define TERMINAL_GET_OBUFF_CHAR(t, x, y) ( *((t)->obuff+(x)+(y)*(t)->buffw) )
#define TERMINAL_GET_CURR_IBUFF_CHAR(t) ( TERMINAL_GET_IBUFF_CHAR((t), (t)->icurx, (t)->icury) )
#define TERMINAL_GET_CURR_OBUFF_CHAR(t) ( TERMINAL_GET_OBUFF_CHAR((t), (t)->ocurx, (t)->ocury) )
#define TERMINAL_GET_BUFF_BYTES(t) ( (t)->buffw*(t)->buffh )
#define TERMINAL_GET_BUFF_BITS(t) ( sizeof(char)*TERMINAL_GET_BUFF_BYTES((t)) )
#define TERMINAL_GET_CHAR_X_COORD(t, x) ( (t)->offsetx+((x)+1)*(t)->marginleft+(x)*(t)->marginright+(x)*(t)->fontw )
#define TERMINAL_GET_CHAR_Y_COORD(t, y) ( (t)->offsety+((y)+1)*(t)->margintop+(y)*(t)->marginbottom+(y)*(t)->fonth )
#define TERMINAL_GET_TEXTAREA_WIDTH(t) ( ((t)->buffw)*((t)->marginright+(t)->marginleft)+(t)->buffw*(t)->fontw )
#define TERMINAL_GET_TEXTAREA_HEIGHT(t) ( ((t)->buffh)*((t)->margintop+(t)->marginbottom)+(t)->buffh*(t)->fonth )

//Buffers
#define TERMINAL_OUTPUT 0
#define TERMINAL_INPUT 1
//Flags
#define TERMINAL_IECHO 1
#define TERMINAL_N_UPDATE 2
#define TERMINAL_MOVE_OCUR 4
#define TERMINAL_CURSOR 8
//Default values
#if defined(__linux__)
#define TERMINAL_DEFAULT_FONT_WIDTH 5
#define TERMINAL_DEFAULT_FONT_HEIGHT 11
#define TERMINAL_DEFAULT_OFFSETX 0
#define TERMINAL_DEFAULT_OFFSETY 9
#define TERMINAL_DEFAULT_MARGINTOP 1
#define TERMINAL_DEFAULT_MARGINRIGHT 0
#define TERMINAL_DEFAULT_MARGINBOTTOM 0
#define TERMINAL_DEFAULT_MARGINLEFT 1
#elif defined(_WIN32)
#define TERMINAL_DEFAULT_FONT_WIDTH 8
#define TERMINAL_DEFAULT_FONT_HEIGHT 15
#define TERMINAL_DEFAULT_OFFSETX 0
#define TERMINAL_DEFAULT_OFFSETY 0
#define TERMINAL_DEFAULT_MARGINTOP 0
#define TERMINAL_DEFAULT_MARGINRIGHT 0
#define TERMINAL_DEFAULT_MARGINBOTTOM 0
#define TERMINAL_DEFAULT_MARGINLEFT 0
#endif
#define TERMINAL_DEFAULT_BUFF_WIDTH 80
#define TERMINAL_DEFAULT_BUFF_HEIGHT 25
#define TERMINAL_DEFAULT_FLAGS (TERMINAL_IECHO | TERMINAL_N_UPDATE | TERMINAL_MOVE_OCUR | TERMINAL_CURSOR)

struct terminal {
#if defined(__linux__)
    Display* d;
    Window w;
    GC gc;
    Pixmap p;
    int s;
#elif defined(_WIN32)
    HWND hwnd;
    HDC hdc;
    pthread_t thread;
#endif
    int fontw, fonth;
    int offsetx, offsety;
    int margintop, marginright, marginbottom, marginleft;
    void (*close)(terminal*);
    void (*redraw)(terminal*);

    int buffw, buffh;
    char* ibuff;
    char* obuff;
    int icurx, icury;
    int ocurx, ocury;
    int flags;
};

terminal* createTerminal(int w, int h, int flags, void (*close)(terminal*), void (*redraw)(terminal*));
void deleteTerminal(terminal* term);
void redrawTerminal(terminal* term);
void updateTerminal(terminal* term); //Calls close when received a destroy message
void checkTerminal(terminal* term);
void swritef(terminal* term, char* fmt, ...); //Simillar to printf
void swrite(terminal* term, const char* str);
char* sread(terminal* term); //Blocks until enter AKA '\n' is pressed.
void cwrite(terminal* term, char chr);
char cread(terminal* term); //Blocks until key is pressed.
void swritef(terminal* term, char* str, ...);
void flush(terminal* term, bool buff);
bool kbhit(terminal* term);

extern terminal* stdterm; //Global terminal

#endif
