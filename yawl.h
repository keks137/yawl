#ifndef INCLUDE_YAWL_YAWL_H_
#define INCLUDE_YAWL_YAWL_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#if defined(_WIN32)
#define YAWL_WIN32
#include <windows.h>
#elif defined(__linux__)
#include <dlfcn.h>
#ifndef YAWL_WAYLAND
#define YAWL_X11
#endif // YAWL_WAYLAND
#endif // __linux__
#endif // _WIN32

#ifdef YAWL_X11
struct _X11Display;
typedef struct _X11Display X11Display;
typedef unsigned long X11ID;
typedef X11ID X11Window;

struct _YaX11Funcs {
	bool loaded;
	X11Display *(*OpenDisplay)(const char *);
	int (*DefaultScreen)(X11Display *);
	X11Window (*CreateSimpleWindow)(
		X11Display * /* display */,
		X11Window /* parent */,
		int /* x */,
		int /* y */,
		unsigned int /* width */,
		unsigned int /* height */,
		unsigned int /* border_width */,
		unsigned long /* border */,
		unsigned long /* background */
	);
	X11Window (*RootWindow)(
		X11Display * /* display */,
		int /* screen_number */
	);
	unsigned long (*BlackPixel)(
		X11Display * /* display */,
		int /* screen_number */
	);
	unsigned long (*WhitePixel)(
		X11Display * /* display */,
		int /* screen_number */
	);
	int (*StoreName)(
		X11Display * /* display */,
		X11Window /* w */,
		const char * /* window_name */
	);
	int (*MapWindow)(
		X11Display * /* display */,
		X11Window /* w */
	);
	int (*Flush)(
		X11Display * /* display */
	);
};

#endif //YAWL_X11

typedef struct {
	size_t width;
	size_t height;
#ifdef YAWL_X11
	X11Display *dpy;
	X11Window win;
#endif // YAWL_X11
} YaWindowData;

typedef struct {
	bool initialized;
#ifdef YAWL_X11
	int xscreen;
	struct _YaX11Funcs x;
#endif // YAWL_X11
} YaState;

#ifndef YaMemset
#include <string.h>
#define YaMemset(ptr, val, n_bytes) memset(ptr, val, n_bytes)
#endif //YaMemset

#if defined(_MSC_VER)
#include <malloc.h>
#define YaAlloca(size) _alloca(size)
#else //  __GNUC__
#define YaAlloca(size) __builtin_alloca(size)
#endif

bool YaInitWindow(YaState *s, YaWindowData *w, const char *name);
void YaBeginDrawing(YaState *s, YaWindowData *w);
void YaEndDrawing(YaState *s, YaWindowData *w);

#ifdef YAWL_IMPLEMENTATION

#ifdef YAWL_X11
#define YA_LOAD_SYMBOL(sym, lib, name)                                 \
	do {                                                           \
		void *tmp_symbol = dlsym(lib, name);                   \
		if (tmp_symbol == NULL) {                              \
			fprintf(stderr, "symbol not found: %s", name); \
			return false;                                  \
		}                                                      \
		sym = tmp_symbol;                                      \
	} while (0)

#endif // YAWL_X11

static bool _YaX11Load(YaState *s)
{
	void *lib = dlopen("libX11.so", RTLD_NOW);
	if (!lib) {
		fprintf(stderr, "no x11: %s\n", dlerror());
		return false;
	}
	YA_LOAD_SYMBOL(s->x.OpenDisplay, lib, "XOpenDisplay");
	YA_LOAD_SYMBOL(s->x.DefaultScreen, lib, "XDefaultScreen");
	YA_LOAD_SYMBOL(s->x.CreateSimpleWindow, lib, "XCreateSimpleWindow");
	YA_LOAD_SYMBOL(s->x.RootWindow, lib, "XRootWindow");
	YA_LOAD_SYMBOL(s->x.BlackPixel, lib, "XBlackPixel");
	YA_LOAD_SYMBOL(s->x.WhitePixel, lib, "XWhitePixel");
	YA_LOAD_SYMBOL(s->x.StoreName, lib, "XStoreName");
	YA_LOAD_SYMBOL(s->x.MapWindow, lib, "XMapWindow");
	YA_LOAD_SYMBOL(s->x.Flush, lib, "XFlush");

	s->x.loaded = true;
	return true;
}
bool YaInit(YaState *s)
{
	s->initialized = true;
	return true;
}
static bool _YaInitWindowX11(YaState *s, YaWindowData *w, const char *name)
{
	if (!s->x.loaded)
		if (!_YaX11Load(s))
			return false;

	w->dpy = s->x.OpenDisplay(NULL);
	if (!w->dpy)
		return false;
	s->xscreen = s->x.DefaultScreen(w->dpy);
	w->win = s->x.CreateSimpleWindow(w->dpy, s->x.RootWindow(w->dpy, s->xscreen),
					 0, 0, (unsigned int)w->width, (unsigned int)w->height, 1,
					 s->x.BlackPixel(w->dpy, s->xscreen), s->x.WhitePixel(w->dpy, s->xscreen));

	s->x.StoreName(w->dpy, w->win, name);
	// XSelectInput(w->dpy, w->win, ExposureMask | KeyPressMask);
	s->x.MapWindow(w->dpy, w->win);
	//
	// GC gc = XCreateGC(w->dpy, win, 0, NULL);
	// XSetForeground(w->dpy, gc, BlackPixel(w->dpy, s->xscreen));
	//
	// XEvent ev;
	// while (XNextEvent(w->dpy, &ev), 1) {
	// 	if (ev.type == Expose)
	// 		XDrawString(w->dpy, win, gc, 150, 100, "Hello, X11!", 11);
	// 	if (ev.type == KeyPress)
	// 		break;
	// }
	return true;
}

bool YaInitWindow(YaState *s, YaWindowData *w, const char *name)
{
	if (!s->initialized) {
		if (!YaInit(s))
			return false;
	}
#ifdef YAWL_X11
	if (!_YaInitWindowX11(s, w, name))
		return false;
#endif // YAWL_X11
	return true;
}
#endif // YAWL_IMPLEMENTATION

void YaBeginDrawing(YaState *s, YaWindowData *w)
{
}
void YaEndDrawing(YaState *s, YaWindowData *w)
{
#ifdef YAWL_X11
	s->x.Flush(w->dpy);
#endif // YAWL_X11
}

#endif // INCLUDE_YAWL_YAWL_H_
