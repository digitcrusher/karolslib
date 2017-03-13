#pragma once
#include <SDL.h>
#define RGB888(r,g,b)((r & 0xFF) << 16) | ((g & 0xFF) << 8)  | ((b & 0xFF) << 0)
    static int drawImage(SDL_Surface* from, int x, int y, SDL_Surface* to) {
        int rtrn;
        SDL_Rect from_rect;
        from_rect.x = x-x-x;
        from_rect.y = y-y-y;
        from_rect.w = x+from->w;
        from_rect.h = y+from->h;
        if(!(rtrn=SDL_BlitSurface(from, &from_rect, to, NULL))) return rtrn;
        return 0;
    };
    static void setPixelSDL(SDL_Surface* surface, uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
        *((int*)(surface->pixels)+x+y*surface->w)=makeRGB888color(r,g,b);
    }
    void drawLine(int* pixels, int x1, int y1, int x2, int y2, int color, int width) {
        int d, dx, dy, ai, bi, xi, yi;
        int x = x1, y = y1;
        if (x1 < x2) {
            xi = 1;
            dx = x2 - x1;
        } else {
            xi = -1;
            dx = x1 - x2;
        }
        if (y1 < y2) {
            yi = 1;
            dy = y2 - y1;
        } else {
            yi = -1;
            dy = y1 - y2;
        }
        *(pixels+x+y*width) = color;
        if (dx > dy) {
            ai = (dy - dx) * 2;
            bi = dy * 2;
            d = bi - dx;
            while (x != x2) {
                if (d >= 0) {
                    x += xi;
                    y += yi;
                    d += ai;
                } else {
                    d += bi;
                    x += xi;
                }
                *(pixels+x+y*width) = color;
            }
        } else {
            ai = ( dx - dy ) * 2;
            bi = dx * 2;
            d = bi - dy;
            while (y != y2) {
                if (d >= 0) {
                    x += xi;
                    y += yi;
                    d += ai;
                } else {
                    d += bi;
                    y += yi;
                }
                *(pixels+x+y*width) = color;
            }
        }
    }
void drawCircle(int* pixels, int sx, int sy, int ex, int ey, int color, int width) {
    int x = ex/2+sx;
    int y = ey/2+sy;
    for(double i=0;i<360;i+=sqrt(sx+ex+sy+ey)/(PI*sqrt(sx+ex+sy+ey))) {
        *(pixels+(x+sin(i*M_PI/180)*(ex/2))+(y+cos(i*M_PI/180)*(ey/2))*width) = color;
    }
}
void drawCircle(int* pixels, int x, int y, int r, int color, int width) {
    int sx = x-r;
    int sy = y-r;
    int ex = x+r;
    int ey = y+r;
    for(double i=0;i<360;i+=sqrt(sx+ex+sy+ey)/((ex+ey)*sqrt(sx+ex+sy+ey))) {
        *(pixels+(x+sin(i*M_PI/180)*r)+(y+cos(i*M_PI/180)*r)*width) = color;
    }
}
