#ifndef __X_H_INCLUDED__
#define __X_H_INCLUDED__

#include <X11/Xft/Xft.h>

void xinit();
//int xloadfont();
//void xdrawrect(int, int, int, int, XftColor*);
//void xdrawcursor(XftColor*);
//void xredraw();
void xfree();
void xclose();
void xrun();
//void xreset_blink_time();

extern unsigned int WINDOW_WIDTH;
extern unsigned int WINDOW_HEIGHT;
extern unsigned int bg;
extern unsigned int fg;
extern int borderpx;
extern float font_size;
extern int tab;
#endif
