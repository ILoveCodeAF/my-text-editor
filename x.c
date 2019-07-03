/* my libs */
#include "queue.h"
#include "te.h"
#include "x.h"
/* config.h for applying patches and the configuration. */
#include "config.h"

#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>
#include <time.h>

typedef struct _BUFFER_THREAD_MUTEX BTM;

typedef struct {
	Display* dpy;
	Colormap cmap;
	Window win;
	Drawable buf;

	XIM xim;
	XIC xic;
	XftDraw* draw;
	Visual* vis;
	int scr;
	int depth;
} XWindow;


/*globals */

XWindow xw;
XftFont* font;
XftColor* color_bg;
XftColor* color_fg;

unsigned int w; //width of window
unsigned int h; //height of window
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

void
xinit()
{
	// Open a connection to server
	if (!(xw.dpy = XOpenDisplay(NULL))){
		printf("can't open display\n");
		exit(1);
	}
	
	xw.scr = XDefaultScreen(xw.dpy);
	xw.depth = XDefaultDepth(xw.dpy, xw.scr);

	Window parent = XRootWindow(xw.dpy, xw.scr);
	int xpos = 0, ypos = 0;

	// Create window
	xw.win = XCreateSimpleWindow(xw.dpy, parent, xpos, ypos, WINDOW_WIDTH, WINDOW_HEIGHT, borderpx, bg, bg);

	xw.buf = XCreatePixmap(xw.dpy, xw.win, WINDOW_WIDTH, WINDOW_HEIGHT, xw.depth);

	// get MapNotify events
	XSelectInput(xw.dpy, xw.win, StructureNotifyMask);

	// "map" the window
	XMapWindow(xw.dpy, xw.win);
	
	XGCValues gcvalues;
	memset(&gcvalues, 0, sizeof(gcvalues));
	// Create a "Graphics context"
	GC gc = XCreateGC(xw.dpy, xw.win, 0, &gcvalues);

//	XFillRectangle(xw.dpy, xw.buf, gc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	// Wait for the MapNotify event
	for(;;){
		XEvent e;
		XNextEvent(xw.dpy, &e);
		if(e.type == MapNotify)
			break;
	}
	xloadfont();

	xw.vis = DefaultVisual(xw.dpy, xw.scr);
	xw.cmap = DefaultColormap(xw.dpy, xw.scr);

	xw.draw = XftDrawCreate(xw.dpy, xw.win, xw.vis, xw.cmap);
	
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
	
	xdrawrect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, color_bg);
	
	XFlush(xw.dpy);

	XGlyphInfo glyphinfo;
	XftChar8 c = 'h';
	XftTextExtents8(xw.dpy, font, &c, 1, &glyphinfo);

	// 
	xstep = glyphinfo.width+1;
	ystep = glyphinfo.height*2;
	xcursor = 2;
	ycursor = ystep;
	col = row = 1;
	max_col = (WINDOW_WIDTH-4)/xstep;
	max_row = (WINDOW_HEIGHT-4)/ystep;
}

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
xrun(void* param)
{
	BTM* btm = (BTM*) param;

	XSelectInput(xw.dpy, xw.win, KeyPressMask); // | KeyReleaseMask

	XEvent ev;
	KeySym ksym = 0;
	char buffer[32];
	int bytes_buffer = sizeof(buffer);

	XftChar8 xchar;
	int not_break = 1;
	int ret_lookup_string;

	xreset_blink_time();

	while(not_break){
		while(XPending(xw.dpy)){
			XNextEvent(xw.dpy, &ev);
			
			if(ev.type == KeyPress){
				XKeyEvent* e = &ev.xkey;	
				
				ret_lookup_string = XLookupString(e, buffer, 
						bytes_buffer, &ksym, NULL);

				pthread_mutex_lock(btm->mutex);
				queue_push(&btm->q, ksym);
				pthread_mutex_unlock(btm->mutex);

				// break if press esc
				if(ksym == XK_Escape)
					not_break = 0;
				if(ksym > 31 && ksym < 127){

					xdrawcursor(color_bg);
		
					xchar = ksym;		
					XftDrawString8(xw.draw, color_fg, font,
							xcursor, ycursor, &xchar, 1);
		
					xcursor += xstep;
					col += 1;
					if(col > max_col){
						col = 1;
						xcursor = 2;
						ycursor += ystep;
						row++;
					}
					xreset_blink_time();
					XFlush(xw.dpy);	
				}
			}
		
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
