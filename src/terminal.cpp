/*
 * terminal.cpp
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
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <src/utils.h>
#include <src/terminal.h>
#include <src/karolslib.h>

terminal* stdterm = createTerminal(TERMINAL_DEFAULT_BUFF_WIDTH, TERMINAL_DEFAULT_BUFF_HEIGHT, TERMINAL_DEFAULT_FLAGS, NULL);

#if defined(_WIN32)
static unsigned int windowid=0;
struct winmsg {
    HWND hwnd;
    UINT iMsg;
    WPARAM wParam;
    LPARAM lParam;
}
static LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hwnd, iMsg, wParam, lParam); //Dump remainning message that wasn't calculated
}
#endif
terminal* createTerminal(int w, int h, int flags, void (*close)(terminal*)) {
    terminal* term = (terminal*)malloc(sizeof(terminal));
    term->fontw = TERMINAL_DEFAULT_FONT_WIDTH;
    term->fonth = TERMINAL_DEFAULT_FONT_HEIGHT;
    term->offsetx = TERMINAL_DEFAULT_OFFSETX;
    term->offsety = TERMINAL_DEFAULT_OFFSETY;
    term->margintop = TERMINAL_DEFAULT_MARGINTOP;
    term->marginright = TERMINAL_DEFAULT_MARGINRIGHT;
    term->marginleft = TERMINAL_DEFAULT_MARGINLEFT;
    term->marginbottom = TERMINAL_DEFAULT_MARGINBOTTOM;
    term->close = close;
    term->buffw = w;
    term->buffh = h;
    term->ibuff = (char*)malloc(TERMINAL_GET_BUFF_BITS(term));
    term->obuff = (char*)malloc(TERMINAL_GET_BUFF_BITS(term));
    term->icurx = 0;
    term->icury = 0;
    term->ocurx = 0;
    term->ocury = 0;
    term->flags = flags;
#if defined(__linux__)
    term->d = XOpenDisplay(NULL);
    if(term->d == NULL) {
        return NULL;
    }
    term->s = DefaultScreen(term->d);
    term->w = XCreateSimpleWindow(term->d, RootWindow(term->d, term->s), 0, 0
                                ,TERMINAL_GET_TEXTAREA_WIDTH(term)
                                ,TERMINAL_GET_TEXTAREA_HEIGHT(term)
                                ,1, WhitePixel(term->d, term->s), BlackPixel(term->d, term->s));
    XSetStandardProperties(term->d, term->w, "Terminal", "Terminal", None, NULL, 0, NULL);
    XSelectInput(term->d, term->w, StructureNotifyMask | ExposureMask | KeyPressMask);
    term->gc = XCreateGC(term->d, term->w, 0, 0);
    XSetBackground(term->d, term->gc, BlackPixel(term->d, term->s));
    XSetForeground(term->d, term->gc, WhitePixel(term->d, term->s));
    XClearWindow(term->d, term->w);
    XMapWindow(term->d, term->w);
#elif defined(_WIN32)
    term->szAppName = uitos(windowid);
    WNDCLASSEX wndclass; //Temporary structure with window settings
    wndclass.cbSize        = sizeof(wndclass); //Size of WNDCLASSEX
    wndclass.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Style parameters
    wndclass.lpfnWndProc   = WndProc; //Pointer to WndProc which handles messages
    wndclass.cbClsExtra    = 0;
    wndclass.cbWndExtra    = 0;
    wndclass.hInstance     = winargs.hInstance;
    wndclass.hIcon         = LoadIcon(NULL,IDI_APPLICATION); //Load icon for taskbar
    wndclass.hCursor       = LoadCursor(NULL,IDC_ARROW); //Load cursor for window
    wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH); //Window background color
    wndclass.lpszMenuName  = NULL;
    wndclass.lpszClassName = term->szAppName; //Window ID
    wndclass.hIconSm       = LoadIcon(NULL,IDI_APPLICATION); //Load icon for title bar
    if(RegisterClassEx(&wndclass) == NULL) { //Register wndclass structure
        return NULL;
    }
    term->hwnd = CreateWindow(term->szAppName, //Window ID
                        "Terminal", //Window title
                        WS_OVERLAPPEDWINDOW, //Window style
                        CW_USEDEFAULT, //Starting x position
                        CW_USEDEFAULT, //Starting y position
                        TERMINAL_GET_TEXTAREA_WIDTH(term), //Starting width
                        TERMINAL_GET_TEXTAREA_HEIGHT(term), //Starting height
                        NULL, //Handle to parent window
                        NULL, //Handle to parent window menu
                        winargs.hInstance, //Handle to window initiation argument
                        NULL
                        );
    if(term->hwnd == NULL) {
        return NULL;
    }
    ShowWindow(term->hwnd, winargs.iCmdShow); //Show window
    UpdateWindow(term->hwnd); //Redraw window
#endif
    return term;
}
void deleteTerminal(terminal* term) {
    free(term->ibuff);
    free(term->obuff);
#if defined(__linux__)
    XFreeGC(term->d, term->gc);
    XDestroyWindow(term->d, term->w);
    XCloseDisplay(term->d);
#elif defined(_WIN32)
#endif
    free(term);
}
void redrawTerminal(terminal* term) {
#if defined(__linux__)
    XWindowAttributes wa;
    XGetWindowAttributes(term->d, term->w, &wa);
    term->p = XCreatePixmap(term->d, term->w
                            ,TERMINAL_GET_TEXTAREA_WIDTH(term)
                            ,TERMINAL_GET_TEXTAREA_HEIGHT(term)
                            ,wa.depth);
    XSetForeground(term->d, term->gc, BlackPixel(term->d, term->s));
    XFillRectangle(term->d, term->p, term->gc, 0, 0
                  ,TERMINAL_GET_TEXTAREA_WIDTH(term)
                  ,TERMINAL_GET_TEXTAREA_HEIGHT(term));
    XSetForeground(term->d, term->gc, WhitePixel(term->d, term->s));
    for(int x=0; x<term->buffw; x++) {
        for(int y=0; y<term->buffh; y++) {
            char c[2];
            *c = TERMINAL_GET_OBUFF_CHAR(term, x, y);
            *(c+1) = '\0';
            if(term->flags & TERMINAL_CURSOR && x == term->ocurx && y == term->ocury) {
                XFillRectangle(term->d, term->p, term->gc
                              ,TERMINAL_GET_CHAR_X_COORD(term, x)-term->offsetx-term->marginleft
                              ,TERMINAL_GET_CHAR_Y_COORD(term, y)-term->offsety+term->marginbottom
                              ,term->fontw+term->marginleft+term->marginright
                              ,term->fonth-2+term->margintop+term->marginbottom);
                XSetForeground(term->d, term->gc, BlackPixel(term->d, term->s));
                XDrawString(term->d, term->p, term->gc
                           ,TERMINAL_GET_CHAR_X_COORD(term, x)
                           ,TERMINAL_GET_CHAR_Y_COORD(term, y), c, strlen(c));
                XSetForeground(term->d, term->gc, WhitePixel(term->d, term->s));
            }else {
                XDrawString(term->d, term->p, term->gc
                           ,TERMINAL_GET_CHAR_X_COORD(term, x)
                           ,TERMINAL_GET_CHAR_Y_COORD(term, y), c, strlen(c));
            }
        }
    }
    XCopyArea(term->d, term->p, term->w, term->gc, 0, 0
             ,TERMINAL_GET_TEXTAREA_WIDTH(term)
             ,TERMINAL_GET_TEXTAREA_HEIGHT(term), 0, 0);
    XFreePixmap(term->d, term->p);
#elif defined(_WIN32)
#endif
}
void updateTerminal(terminal* term) {
#if defined(__linux__)
    XEvent e;
    while(XPending(term->d)) {
        XNextEvent(term->d, &e);
        switch(e.type) {
            case Expose:
                redrawTerminal(term);
                break;
            case KeyPress:
                char buff[16];
                KeySym ks;
                XLookupString(&e.xkey, buff, 16, &ks, NULL);
                if(*buff == '\0') {
                    switch(ks) {
                        case XK_Up:
                            --term->icury;
                            if(term->flags & TERMINAL_MOVE_OCUR) {
                                --term->ocury;
                            }
                            break;
                        case XK_Left:
                            --term->icurx;
                            if(term->flags & TERMINAL_MOVE_OCUR) {
                                --term->ocurx;
                            }
                            break;
                        case XK_Down:
                            if(TERMINAL_GET_CURR_IBUFF_CHAR(term) != '\0') {
                                ++term->icury;
                            }
                            if(term->flags & TERMINAL_MOVE_OCUR) {
                                ++term->ocury;
                            }
                            break;
                        case XK_Right:
                            if(TERMINAL_GET_CURR_IBUFF_CHAR(term) != '\0') {
                                ++term->icurx;
                            }
                            if(term->flags & TERMINAL_MOVE_OCUR) {
                                ++term->ocurx;
                            }
                            break;
                    }
                    checkTerminal(term);
                    break;
                }
                switch(*buff) {
                    case '\b':
                        --term->icurx;
                        TERMINAL_GET_CURR_IBUFF_CHAR(term) = '\0';
                        break;
                    default:
                        TERMINAL_GET_CURR_IBUFF_CHAR(term) = *buff;
                        ++term->icurx;
                        break;
                }
                checkTerminal(term);
                if(term->flags & TERMINAL_IECHO) {
                    cwrite(term, *buff);
                }
                break;
            case DestroyNotify:
                if(term->close != NULL) {
                    term->close(term);
                }
                break;
        }
    }
    redrawTerminal(term);
#elif defined(_WIN32)
    switch(iMsg) {
        case WM_PAINT:
            hdc = BeginPaint(hwnd,&ps);
            render(hwnd);
            EndPaint(hwnd,&ps);
            return 0;
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            break;
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
            break;
        case WM_KEYDOWN:
            switch(wParam) {
                case VK_UP:
                    input.v=0.25;
                    break;
                case VK_LEFT:
                    input.vr=-0.25;
                    break;
                case VK_DOWN:
                    input.v=-0.25;
                    break;
                case VK_RIGHT:
                    input.vr=0.25;
                    break;
                case VK_HOME:
                    reset=1;
                    break;
            }
            return 0;
            break;
        case WM_KEYUP:
            switch(wParam) {
                case VK_UP:
                    input.v=0;
                    break;
                case VK_LEFT:
                    input.vr=0;
                    break;
                case VK_DOWN:
                    input.v=0;
                    break;
                case VK_RIGHT:
                    input.vr=0;
                    break;
            }
            return 0;
            break;
    }
#endif
}
void checkTerminal(terminal* term) {
#if defined(__linux__)
    XResizeWindow(term->d, term->w, TERMINAL_GET_TEXTAREA_WIDTH(term), TERMINAL_GET_TEXTAREA_HEIGHT(term));
#elif defined(_WIN32)
MoveWindow(
  _In_ HWND hWnd,
  _In_ int  X,
  _In_ int  Y,
  _In_ int  nWidth,
  _In_ int  nHeight, 1);
#endif
    if(term->ocurx < 0) {
        term->ocurx = term->buffw-1;
        --term->ocury;
    }
    if(term->ocury < 0) {
        term->ocurx = 0;
        term->ocury = 0;
    }
    if(term->ocurx >= term->buffw) {
        term->ocurx = 0;
        ++term->ocury;
    }
    if(term->ocury >= term->buffh) {
        --term->ocury;
        memmove(term->obuff, term->obuff+term->buffw, TERMINAL_GET_BUFF_BYTES(term)-term->buffw);
        memset(&TERMINAL_GET_CURR_OBUFF_CHAR(term), '\0', term->buffw);
    }
    if(term->icurx < 0) {
        term->icurx = term->buffw-1;
        --term->icury;
    }
    if(term->icury < 0) {
        term->icurx = 0;
        term->icury = 0;
    }
    if(term->icurx >= term->buffw) {
        term->icurx = 0;
        ++term->icury;
    }
    if(term->icury >= term->buffh) {
        --term->icury;
        memmove(term->ibuff, term->ibuff+term->buffw, TERMINAL_GET_BUFF_BYTES(term)-term->buffw);
        memset(&TERMINAL_GET_CURR_IBUFF_CHAR(term), '\0', term->buffw);
    }
}
void swritef(terminal* term, char* fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    int written=0;
    for(unsigned int i=0; i<strlen(fmt); i++) {
        switch(fmt[i]) {
            case '%':
                ++i;
                if(!(i<strlen(fmt))) {
                    return;
                }
                switch(fmt[i]) {
                    case 'd':
                    case 'i': {
                        int val=va_arg(vl, int);
                        char* str=itos(val);
                        swrite(term, str);
                        written += strlen(str);
                        } break;
                    case 'u': {
                        unsigned int val=va_arg(vl, unsigned int);
                        char* str=uitos(val);
                        swrite(term, str);
                        written += strlen(str);
                        } break;
                    case 'f':
                    case 'F': {
                        double val=va_arg(vl, double);
                        char* str=ftos(val);
                        swrite(term, str);
                        written += strlen(str);
                        } break;
                    case 'c': {
                        char val=va_arg(vl, int);
                        cwrite(term, val);
                        ++written;
                        } break;
                    case 's': {
                        char* val=va_arg(vl, char*);
                        swrite(term, val);
                        written += strlen(val);
                        } break;
                    case 'n': {
                        int* val=va_arg(vl, int*);
                        *val = written;
                        } break;
                    case '%': {
                        cwrite(term, fmt[i]);
                        ++written;
                        } break;
                }
                break;
            default:
                cwrite(term, fmt[i]);
                ++written;
                break;
        }
    }
    va_end(vl);
}
void swrite(terminal* term, const char* str) {
    for(unsigned int i=0; i<strlen(str); i++) {
        cwrite(term, str[i]);
    }
}
char* sread(terminal* term) {
    int len;
    char chr;
    updateTerminal(term);
    for(len = 0; !(*(term->ibuff+len) == '\n' || *(term->ibuff+len) == '\r' || *(term->ibuff+len) == '\0'); len++);
    if(*(term->ibuff+len) == '\n' && *(term->ibuff+len) == '\r') {
        goto getter;
    }
    chr = TERMINAL_GET_CURR_IBUFF_CHAR(term);
    while(chr != '\n' && chr != '\r') {
        updateTerminal(term);
        chr = *(&TERMINAL_GET_CURR_IBUFF_CHAR(term)-1);
    }
    len = term->icurx+term->icury*term->buffw;
    getter:
    char* str = (char*)malloc(sizeof(char)*len);
    for(int i=0; i<len; i++) {
        *(str+i) = cread(term);
    }
    *(str+len-1) = '\0';
    return str;
}
void cwrite(terminal* term, char chr) {
    switch(chr) {
        case '\r':
        case '\n':
            term->ocurx = 0;
            ++term->ocury;
            if(term->flags & TERMINAL_N_UPDATE) {
                updateTerminal(term);
            }
            break;
        case '\b':
            --term->ocurx;
            TERMINAL_GET_CURR_OBUFF_CHAR(term) = '\0';
            break;
        default:
            TERMINAL_GET_CURR_OBUFF_CHAR(term) = chr;
            ++term->ocurx;
            break;
    }
    checkTerminal(term);
}
char cread(terminal* term) {
    while(*term->ibuff == '\0') {
        updateTerminal(term);
    }
    char chr = *term->ibuff;
    --term->icurx;
    checkTerminal(term);
    memmove(term->ibuff, term->ibuff+1, TERMINAL_GET_BUFF_BYTES(term)-1);
    TERMINAL_GET_IBUFF_CHAR(term, term->buffw-1, term->buffh-1) = '\0';
    return chr;
}
void flush(terminal* term, bool buff) {
    if(buff) {
        memset(term->ibuff, '\0', TERMINAL_GET_BUFF_BYTES(term));
        term->icurx = 0;
        term->icury = 0;
    }else {
        memset(term->obuff, '\0', TERMINAL_GET_BUFF_BYTES(term));
        term->ocurx = 0;
        term->ocury = 0;
    }
}
bool kbhit(terminal* term) {
    return *term->ibuff;
}
