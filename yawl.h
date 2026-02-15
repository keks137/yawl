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
#endif // _WIN32

#ifdef YAWL_WIN32


#endif // YAWL_WIN32

#ifdef YAWL_X11
struct _X11Display;
typedef struct _X11Display X11Display;
typedef unsigned long X11ID;
typedef X11ID X11Window;

struct _YwX11Funcs {
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
#ifdef YAWL_WIN32
	HWND hwnd;
	HDC hdc;
#endif // YAWL_WIN32
} YwWindowData;

typedef struct {
	bool initialized;
#ifdef YAWL_X11
	int xscreen;
	struct _YwX11Funcs x;
#endif // YAWL_X11
#ifdef YAWL_WIN32
	HINSTANCE hinstance;
	HMODULE gdi32;
	HMODULE user32;
	HMODULE kernel32;
#endif
} YwState;

#ifndef YwMemset
#include <string.h>
#define YwMemset(ptr, val, n_bytes) memset(ptr, val, n_bytes)
#endif //YwMemset

#if defined(_MSC_VER)
#include <malloc.h>
#define YwAlloca(size) _alloca(size)
#else //  __GNUC__
#define YwAlloca(size) __builtin_alloca(size)
#endif

bool YwInitWindow(YwState *s, YwWindowData *w, const char *name);
void YwBeginDrawing(YwState *s, YwWindowData *w);
void YwEndDrawing(YwState *s, YwWindowData *w);

#ifdef YAWL_IMPLEMENTATION

#ifdef YAWL_X11
#define YW_LOAD_SYMBOL(sym, lib, name)                                 \
	do {                                                           \
		void *tmp_symbol = dlsym(lib, name);                   \
		if (tmp_symbol == NULL) {                              \
			fprintf(stderr, "symbol not found: %s", name); \
			return false;                                  \
		}                                                      \
		sym = tmp_symbol;                                      \
	} while (0)

static bool _YwX11Load(YwState *s)
{
	void *lib = dlopen("libX11.so", RTLD_NOW);
	if (!lib) {
		fprintf(stderr, "no x11: %s\n", dlerror());
		return false;
	}
	YW_LOAD_SYMBOL(s->x.OpenDisplay, lib, "XOpenDisplay");
	YW_LOAD_SYMBOL(s->x.DefaultScreen, lib, "XDefaultScreen");
	YW_LOAD_SYMBOL(s->x.CreateSimpleWindow, lib, "XCreateSimpleWindow");
	YW_LOAD_SYMBOL(s->x.RootWindow, lib, "XRootWindow");
	YW_LOAD_SYMBOL(s->x.BlackPixel, lib, "XBlackPixel");
	YW_LOAD_SYMBOL(s->x.WhitePixel, lib, "XWhitePixel");
	YW_LOAD_SYMBOL(s->x.StoreName, lib, "XStoreName");
	YW_LOAD_SYMBOL(s->x.MapWindow, lib, "XMapWindow");
	YW_LOAD_SYMBOL(s->x.Flush, lib, "XFlush");

	s->x.loaded = true;
	return true;
}
static bool _YwInitWindowX11(YwState *s, YwWindowData *w, const char *name)
{
	if (!s->x.loaded)
		if (!_YwX11Load(s))
			return false;

	w->dpy = s->x.OpenDisplay(NULL);
	if (!w->dpy)
		return false;
	s->xscreen = s->x.DefaultScreen(w->dpy);
	w->win = s->x.CreateSimpleWindow(w->dpy, s->x.RootWindow(w->dpy, s->xscreen),
					 0, 0, (unsigned int)w->width, (unsigned int)w->height, 1,
					 s->x.BlackPixel(w->dpy, s->xscreen), s->x.BlackPixel(w->dpy, s->xscreen));

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
#endif // YAWL_X11
#if defined(YAWL_WIN32)
#define YW_LOAD_SYMBOL(sym, lib, name)                                   \
	do {                                                             \
		FARPROC tmp = GetProcAddress(lib, name);                 \
		if (!tmp) {                                              \
			fprintf(stderr, "symbol not found: %s\n", name); \
			return false;                                    \
		}                                                        \
		sym = (void *)tmp;                                       \
	} while (0)

static LRESULT CALLBACK _YwWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	(void)wparam;
	(void)lparam;
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcA(hwnd, msg, wparam, lparam);
}

static bool _YwInitWindowWin32(YwState *s, YwWindowData *w, const char *name)
{
	s->hinstance = GetModuleHandleA(NULL);

	WNDCLASSA wc = { 0 };
	wc.lpfnWndProc = _YwWndProc;
	wc.hInstance = s->hinstance;
	wc.lpszClassName = "YwWindowClass";
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	RegisterClassA(&wc);

	w->hwnd = CreateWindowExA(
		0, "YwWindowClass", name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(int)w->width, (int)w->height,
		NULL, NULL, s->hinstance, NULL);

	if (!w->hwnd)
		return false;

	ShowWindow(w->hwnd, SW_SHOW);
	UpdateWindow(w->hwnd);
	w->hdc = GetDC(w->hwnd);
	return true;
}
#endif //YAWL_WIN32

bool YwInit(YwState *s)
{
	s->initialized = true;
	return true;
}

bool YwInitWindow(YwState *s, YwWindowData *w, const char *name)
{
	if (!s->initialized) {
		if (!YwInit(s))
			return false;
	}
#ifdef YAWL_X11
	return _YwInitWindowX11(s, w, name);
#elif defined(YAWL_WIN32)
	return _YwInitWindowWin32(s, w, name);
#else
	fprintf(stderr, "Ywwl: Unsupported platform\n");
	return false;
#endif
	return true;
}
#endif // YAWL_IMPLEMENTATION

void YwBeginDrawing(YwState *s, YwWindowData *w)
{
}
void YwEndDrawing(YwState *s, YwWindowData *w)
{
#ifdef YAWL_X11
	s->x.Flush(w->dpy);
#endif // YAWL_X11
#ifdef YAWL_WIN32
	SwapBuffers(w->hdc);
#endif // YAWL_WIN32
}

#endif // INCLUDE_YAWL_YAWL_H_
