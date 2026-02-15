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
struct _YaWin32Funcs {
	bool loaded;
	HMODULE gdi32;
	HMODULE user32;
	HMODULE kernel32;

	HWND (*CreateWindowExA)(
		DWORD, LPCSTR, LPCSTR, DWORD,
		int, int, int, int,
		HWND, HMENU, HINSTANCE, LPVOID);
	BOOL (*ShowWindow)(HWND, int);
	BOOL (*UpdateWindow)(HWND);
	BOOL (*SetWindowTextA)(HWND, LPCSTR);
	HDC (*GetDC)(HWND);
	int (*ReleaseDC)(HWND, HDC);
	BOOL (*SwapBuffers)(HDC);
	HINSTANCE (*GetModuleHandleA)(LPCSTR);
	HGDIOBJ (*GetStockObject)(int);
};

typedef struct {
	HWND hwnd;
	HDC hdc;
} YaWin32Window;
#endif // YAWL_WIN32

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
#ifdef YAWL_WIN32
	YaWin32Window w32;
#endif // YAWL_WIN32
} YaWindowData;

typedef struct {
	bool initialized;
#ifdef YAWL_X11
	int xscreen;
	struct _YaX11Funcs x;
#endif // YAWL_X11
#ifdef YAWL_WIN32
	struct _YaWin32Funcs w32;
	HINSTANCE hinstance;
#endif
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
#define YA_LOAD_SYMBOL(sym, lib, name)                                   \
	do {                                                             \
		FARPROC tmp = GetProcAddress(lib, name);                 \
		if (!tmp) {                                              \
			fprintf(stderr, "symbol not found: %s\n", name); \
			return false;                                    \
		}                                                        \
		sym = (void *)tmp;                                       \
	} while (0)

static LRESULT CALLBACK _YaWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	(void)wparam;
	(void)lparam;
	if (msg == WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcA(hwnd, msg, wparam, lparam);
}

static bool _YaWin32Load(YaState *s)
{
	s->w32.user32 = LoadLibraryA("user32.dll");
	s->w32.gdi32 = LoadLibraryA("gdi32.dll");
	s->w32.kernel32 = LoadLibraryA("kernel32.dll");
	if (!s->w32.user32 || !s->w32.gdi32 || !s->w32.kernel32)
		return false;

	YA_LOAD_SYMBOL(s->w32.CreateWindowExA, s->w32.user32, "CreateWindowExA");
	YA_LOAD_SYMBOL(s->w32.ShowWindow, s->w32.user32, "ShowWindow");
	YA_LOAD_SYMBOL(s->w32.UpdateWindow, s->w32.user32, "UpdateWindow");
	YA_LOAD_SYMBOL(s->w32.SetWindowTextA, s->w32.user32, "SetWindowTextA");
	YA_LOAD_SYMBOL(s->w32.GetDC, s->w32.user32, "GetDC");
	YA_LOAD_SYMBOL(s->w32.ReleaseDC, s->w32.user32, "ReleaseDC");
	YA_LOAD_SYMBOL(s->w32.GetModuleHandleA, s->w32.kernel32, "GetModuleHandleA");
	YA_LOAD_SYMBOL(s->w32.SwapBuffers, s->w32.gdi32, "SwapBuffers");
	YA_LOAD_SYMBOL(s->w32.GetStockObject, s->w32.gdi32, "GetStockObject");

	s->w32.loaded = true;
	return true;
}

static bool _YaInitWindowWin32(YaState *s, YaWindowData *w, const char *name)
{
	if (!s->w32.loaded && !_YaWin32Load(s))
		return false;

	s->hinstance = s->w32.GetModuleHandleA(NULL);

	WNDCLASSA wc = { 0 };
	wc.lpfnWndProc = _YaWndProc;
	wc.hInstance = s->hinstance;
	wc.lpszClassName = "YaWindowClass";
	wc.hbrBackground = (HBRUSH)s->w32.GetStockObject(BLACK_BRUSH);
	RegisterClassA(&wc);

	w->w32.hwnd = s->w32.CreateWindowExA(
		0, "YaWindowClass", name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		(int)w->width, (int)w->height,
		NULL, NULL, s->hinstance, NULL);

	if (!w->w32.hwnd)
		return false;

	s->w32.ShowWindow(w->w32.hwnd, SW_SHOW);
	s->w32.UpdateWindow(w->w32.hwnd);
	w->w32.hdc = s->w32.GetDC(w->w32.hwnd);
	return true;
}
#endif //YAWL_WIN32

bool YaInit(YaState *s)
{
	s->initialized = true;
	return true;
}

bool YaInitWindow(YaState *s, YaWindowData *w, const char *name)
{
	if (!s->initialized) {
		if (!YaInit(s))
			return false;
	}
#ifdef YAWL_X11
	return _YaInitWindowX11(s, w, name);
#elif defined(YAWL_WIN32)
	return _YaInitWindowWin32(s, w, name);
#else
	fprintf(stderr, "Yawl: Unsupported platform\n");
	return false;
#endif
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
#ifdef YAWL_WIN32
	s->w32.SwapBuffers(w->w32.hdc);
#endif // YAWL_WIN32
}

#endif // INCLUDE_YAWL_YAWL_H_
