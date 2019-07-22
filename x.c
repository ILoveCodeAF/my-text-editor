/* my libs */
#include "queue.h"
#include "te.h"
#include "x.h"
/* config.h for applying patches and the configuration. */
#include "config.h"

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/Xft/Xft.h>
#include <time.h>
//#include <locale.h>


typedef struct {
	Display* dpy;
	Colormap cmap;
	Window win;
	Drawable buf;

	XIM xim;
	XIC xic;
	XftDraw* draw;
	Visual* vis;

	int width;
	int height;
	int scr;
	int depth;
} XWindow;


/*globals */

XWindow xw;
XftFont* font;
XftColor* color_bg;
XftColor* color_fg;

//unsigned int w; //width of window
//unsigned int h; //height of window
int col; //
int row;
int max_col;
int max_row;
int xstep, ystep; //space between characters, lines
int xcursor; // position of input cursor
int ycursor;

clock_t t1, t2;
const int blink_times = 9;
int blink_count;
const float fps = 2; //1 blink / second
int flag = 0; //0 = draw by color_fg, 1= draw by color_bg
clock_t period_time, period_time_blink = 1000*fps/2;

typedef struct _SHARED Shared;
/* share buffer */
Shared* shared;

/* use in xrun to exit while loop */
Bool is_close = False;


void kpress(XEvent*);
void focus(XEvent*);
void resize(XEvent*);

// array handler
void (*handler[LASTEvent])(XEvent*) = {
	[KeyPress] = kpress,
	[FocusIn] = focus,
	[FocusOut] = focus,
	[ConfigureNotify] = resize
};

void
xdrawrect(int x, int y, int w, int h, XftColor* color)
{
	XftDrawRect(xw.draw, color, x, y, w, h);
}

int
xloadfont(){
	font = XftFontOpen( xw.dpy, xw.scr, XFT_FAMILY, XftTypeString, 
			"mono", XFT_SIZE, XftTypeDouble, font_size, 0);
}

void
xdrawcursor(XftColor* color)
{
	xdrawrect(xcursor, ycursor-ystep*2/3, xstep-1, ystep, color);
}

void
xredraw()
{
}

void xreset_blink_time(){
	xdrawcursor(color_fg);
	t1 = clock();
	blink_count = blink_times;
	flag = 1;
}
	


void
xfree()
{
	if(color_bg != NULL)
		XftColorFree(xw.dpy, xw.vis, xw.cmap, color_bg);
	if(color_fg != NULL)
		XftColorFree(xw.dpy, xw.vis, xw.cmap, color_fg);
}

void
xclose()
{
	XftFontClose(xw.dpy, font);
	
	XDestroyWindow(xw.dpy, xw.win);

	XCloseDisplay(xw.dpy);

}
	
void
xinit(void* param)
{
	shared = (Shared*)param;

	color_bg = (XftColor*) malloc(sizeof(XftColor));
	color_fg = (XftColor*) malloc(sizeof(XftColor));
	
	unsigned int alpha = (1<<16)-1;
	color_bg->color.red = bg;
	color_bg->color.green = bg;
	color_bg->color.blue = bg;
	color_bg->color.alpha = alpha;
	color_bg->pixel = 1;

	color_fg->color.red = fg;
	color_fg->color.green = fg;
	color_fg->color.blue = fg;
	color_fg->color.alpha = alpha;
	color_fg->pixel = 1;


//if(setlocale(LC_CTYPE, "") == NULL){
//	printf("cannot set locale\n");
//	exit(1);
//}
	// Open a connection to server
	if (!(xw.dpy = XOpenDisplay(NULL))){
		printf("can't open display\n");
		exit(1);
	}

	if(!XSupportsLocale()){
		printf("X does not support Display.\n");
		exit(1);
	}
	if(XSetLocaleModifiers("") == NULL){
		printf("warning: cannot set locale modifiers.\n");
	}
	
	xw.width = WINDOW_WIDTH;
	xw.height = WINDOW_HEIGHT;
	xw.scr = XDefaultScreen(xw.dpy);
	xw.depth = XDefaultDepth(xw.dpy, xw.scr);

	Window parent = XRootWindow(xw.dpy, xw.scr);
	int xpos = 0, ypos = 0;

	// Create window
	xw.win = XCreateSimpleWindow(xw.dpy, parent, xpos, ypos, xw.width, xw.height, borderpx, bg, bg);

	xw.buf = XCreatePixmap(xw.dpy, xw.win, xw.width, xw.height, xw.depth);

	XGCValues gcvalues;
	memset(&gcvalues, 0, sizeof(gcvalues));
	// Create a "Graphics context"
	GC gc = XCreateGC(xw.dpy, xw.win, 0, &gcvalues);
	XSetForeground(xw.dpy, gc, color_fg->pixel);
	XSetBackground(xw.dpy, gc, color_bg->pixel);
	XFillRectangle(xw.dpy, xw.buf, gc, 0, 0, xw.width, xw.height);


	if((xw.xim = XOpenIM(xw.dpy, NULL, NULL, NULL)) == NULL){
		printf("could not open input method\n");
		exit(1);
	}

	if((xw.xic = XCreateIC(xw.xim, XNInputStyle,
					XIMPreeditNothing | XIMStatusNothing,
					XNClientWindow, xw.win,
					XNFocusWindow, xw.win,
					NULL)) == NULL){
		printf("counld not create input context\n");
		exit(1);
	}
//	XFillRectangle(xw.dpy, xw.buf, gc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	// Wait for the MapNotify event
	
	XSelectInput(xw.dpy, xw.win, 
			ExposureMask | KeyPressMask | StructureNotifyMask);
	XSetICFocus(xw.xic);

	// "map" the window
	XMapWindow(xw.dpy, xw.win);
	
	XEvent e;
	for(;;){
		XNextEvent(xw.dpy, &e);
		if(XFilterEvent(&e, None))
			continue;
		if(e.type == MapNotify)
			break;
	}

	xloadfont();

	xw.vis = DefaultVisual(xw.dpy, xw.scr);
	xw.cmap = DefaultColormap(xw.dpy, xw.scr);

	xw.draw = XftDrawCreate(xw.dpy, xw.win, xw.vis, xw.cmap);
	
	
	xdrawrect(0, 0, xw.width, xw.height, color_bg);
	
	//XFlush(xw.dpy);

	XGlyphInfo glyphinfo;
	XftChar8 c = 'h';
	XftTextExtents8(xw.dpy, font, &c, 1, &glyphinfo);

	// 
	xstep = glyphinfo.width+1;
	ystep = glyphinfo.height*2;
	xcursor = 2;
	ycursor = ystep;
	col = row = 1;
	max_col = (xw.width-4)/xstep;
	max_row = (xw.height-4)/ystep;
}

void
xrun()
{	
	XEvent ev;
	xreset_blink_time();

	while(!is_close){
		while(XPending(xw.dpy)){
			XNextEvent(xw.dpy, &ev);
			if(XFilterEvent(&ev, None))
				continue;
			if(handler[ev.type])
				(handler[ev.type])(&ev);
		}	
		if(blink_count > 0){
			t2 = clock();
			period_time = (t2-t1)/(CLOCKS_PER_SEC/1000); //milisecond

			if(period_time > period_time_blink){
				if(flag){//if flag = 1 draw by fg
					xdrawcursor(color_fg);
					flag = 0;
					blink_count -= 1;
				}
				else{
					flag = 1;
					xdrawcursor(color_bg);
				}
				XFlush(xw.dpy);
				t1 = clock();
			}
		}	
	}
}

void
kpress(XEvent* ev){
	KeySym ksym = 0;
	char buffer[32] = {0};
	int bytes_buffer = sizeof(buffer);
	Status status;
	XftChar8 xchar;
	int i = 0;
	
	XKeyEvent* e = &ev->xkey;	
	
	int len = XmbLookupString(xw.xic, e, buffer, 
			bytes_buffer, &ksym, &status);

	if(len == 0)
		return;

	switch(status) {
	case XBufferOverflow:
		printf("buffer overflow\n");
		exit(1);
	case XLookupNone:
		break;
	case XLookupKeySym:
	case XLookupBoth:
		if(ksym == XK_Escape){
			shared_lock(shared);
			queue_push(&shared->buffer, 27);// esc = 27
			shared_unlock(shared);
//		exit(0);
			is_close = True;
			return;
		}
		if(status == XLookupKeySym)
			break;
	case XLookupChars:
		shared_lock(shared);
		while(i<len){
			queue_push(&shared->buffer, buffer[i]);
			++i;
		}
		shared_unlock(shared);
		printf("%s\n", buffer);
		break;
	}	
//shared_lock(shared);
//queue_push(&shared->buffer, ksym);
//shared_unlock(shared);
//
//if(ksym > 31 && ksym < 127){
//
//	xdrawcursor(color_bg);
//
//	xchar = ksym;		
//	XftDrawString8(xw.draw, color_fg, font,
//			xcursor, ycursor, &xchar, 1);
//
//	xcursor += xstep;
//	col += 1;
//	if(col > max_col){
//		col = 1;
//		xcursor = 2;
//		ycursor += ystep;
//		row++;
//	}
//	xreset_blink_time();
//	XFlush(xw.dpy);	
//}
}

void
focus(XEvent* ev)
{
	if(ev->type == FocusIn){
		XSetICFocus(xw.xic);
	}
	else if(ev->type == FocusOut){
		XUnsetICFocus(xw.xic);
	}
}

void
resize(XEvent* ev)
{
}
