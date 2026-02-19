#ifndef INCLUDE_YAWL_YAWL_H_
#define INCLUDE_YAWL_YAWL_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
#include <xcb/xcb.h>
#include <xcb/xproto.h>
struct _X11Display;
typedef struct _X11Display X11Display;
typedef unsigned long X11ID;
typedef X11ID X11Window;

struct _YwX11Funcs {
	bool loaded;
	xcb_connection_t *(*connect)(const char *displayname, int *screenp);
	void (*disconnect)(xcb_connection_t *c);
	xcb_screen_iterator_t (*setup_roots_iterator)(xcb_setup_t *R);
	int (*flush)(xcb_connection_t *c);
	uint32_t (*generate_id)(xcb_connection_t *c);
	xcb_void_cookie_t (*create_window)(
		xcb_connection_t *c,
		uint8_t depth,
		xcb_window_t wid,
		xcb_window_t parent,
		int16_t x,
		int16_t y,
		uint16_t width,
		uint16_t height,
		uint16_t border_width,
		uint16_t _class,
		xcb_visualid_t visual,
		uint32_t value_mask,
		const void *value_list);
	xcb_void_cookie_t (*map_window)(xcb_connection_t *c, xcb_window_t window);
	xcb_void_cookie_t (*change_property)(
		xcb_connection_t *c,
		uint8_t mode,
		xcb_window_t window,
		xcb_atom_t property,
		xcb_atom_t type,
		uint8_t format,
		uint32_t data_len,
		const void *data);
	xcb_intern_atom_cookie_t (*intern_atom)(
		xcb_connection_t *c,
		uint8_t only_if_exists,
		uint16_t name_len,
		const char *name);
	xcb_intern_atom_reply_t *(*intern_atom_reply)(
		xcb_connection_t *c,
		xcb_intern_atom_cookie_t cookie,
		xcb_generic_error_t **e);
	xcb_generic_event_t *(*poll_for_event)(xcb_connection_t *c);
	void (*request_check)(xcb_connection_t *c, xcb_void_cookie_t cookie);
	struct xcb_setup_t *(*get_setup)(xcb_connection_t *c);
	void (*screen_next)(xcb_screen_iterator_t *i);
	int (*connection_has_error)(xcb_connection_t *c);
};

#endif //YAWL_X11

typedef struct {
	size_t width;
	size_t height;
#ifdef YAWL_X11
	xcb_connection_t *conn;
	xcb_window_t win;
	xcb_screen_t *screen;
#endif // YAWL_X11
#ifdef YAWL_WIN32
	HWND hwnd;
	HDC hdc;
#endif // YAWL_WIN32
} YwWindowData;

typedef struct {
	bool initialized;
#ifdef YAWL_X11
	int screen_num;
	xcb_atom_t wm_protocols;
	xcb_atom_t wm_delete_window;
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
	void *lib = dlopen("libxcb.so.1", RTLD_NOW | RTLD_LOCAL);
	if (!lib) {
		lib = dlopen("libxcb.so", RTLD_NOW | RTLD_LOCAL);
	}
	if (!lib) {
		fprintf(stderr, "no xcb: %s\n", dlerror());
		return false;
	}

	YW_LOAD_SYMBOL(s->x.connect, lib, "xcb_connect");
	YW_LOAD_SYMBOL(s->x.disconnect, lib, "xcb_disconnect");
	YW_LOAD_SYMBOL(s->x.setup_roots_iterator, lib, "xcb_setup_roots_iterator");
	YW_LOAD_SYMBOL(s->x.flush, lib, "xcb_flush");
	YW_LOAD_SYMBOL(s->x.generate_id, lib, "xcb_generate_id");
	YW_LOAD_SYMBOL(s->x.create_window, lib, "xcb_create_window");
	YW_LOAD_SYMBOL(s->x.map_window, lib, "xcb_map_window");
	YW_LOAD_SYMBOL(s->x.change_property, lib, "xcb_change_property");
	YW_LOAD_SYMBOL(s->x.intern_atom, lib, "xcb_intern_atom");
	YW_LOAD_SYMBOL(s->x.intern_atom_reply, lib, "xcb_intern_atom_reply");
	YW_LOAD_SYMBOL(s->x.poll_for_event, lib, "xcb_poll_for_event");
	YW_LOAD_SYMBOL(s->x.request_check, lib, "xcb_request_check");
	YW_LOAD_SYMBOL(s->x.get_setup, lib, "xcb_get_setup");
	YW_LOAD_SYMBOL(s->x.screen_next, lib, "xcb_screen_next");
	YW_LOAD_SYMBOL(s->x.connection_has_error, lib, "xcb_connection_has_error");
	s->x.loaded = true;

	return true;
}

static bool _YwEGLLoad(YwState *s)
{
	void *egl = dlopen("libEGL.so.1", RTLD_NOW | RTLD_LOCAL);
	if (!egl)
		egl = dlopen("libEGL.so", RTLD_NOW | RTLD_LOCAL);
	if (!egl) {
		fprintf(stderr, "no EGL: %s\n", dlerror());
		return false;
	}

	return true;
}

static xcb_screen_t *_YwGetScreen(YwState *s, xcb_connection_t *c)
{
	xcb_screen_iterator_t iter = s->x.setup_roots_iterator(s->x.get_setup(c));
	for (; iter.rem; --s->screen_num, s->x.screen_next(&iter)) {
		if (s->screen_num == 0)
			return iter.data;
	}
	return NULL;
}
static bool _YwInitWindowX11(YwState *s, YwWindowData *w, const char *name)
{
	if (!s->x.loaded)
		if (!_YwX11Load(s))
			return false;

	w->conn = s->x.connect(NULL, &s->screen_num);
	if (!w->conn || s->x.connection_has_error(w->conn))
		return false;

	w->screen = _YwGetScreen(s, w->conn);
	if (!w->screen)
		return false;

	w->win = s->x.generate_id(w->conn);
	uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	uint32_t values[2] = {
		w->screen->black_pixel,
		XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS
	};

	s->x.create_window(
		w->conn,
		XCB_COPY_FROM_PARENT,
		w->win,
		w->screen->root,
		0, 0,
		(uint16_t)w->width,
		(uint16_t)w->height,
		1,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		w->screen->root_visual,
		mask,
		values);

	xcb_intern_atom_cookie_t proto_cookie = s->x.intern_atom(w->conn, 0, 12, "WM_PROTOCOLS");
	xcb_intern_atom_cookie_t del_cookie = s->x.intern_atom(w->conn, 0, 16, "WM_DELETE_WINDOW");
	xcb_intern_atom_reply_t *proto_reply = s->x.intern_atom_reply(w->conn, proto_cookie, NULL);
	xcb_intern_atom_reply_t *del_reply = s->x.intern_atom_reply(w->conn, del_cookie, NULL);

	if (proto_reply) {
		s->wm_protocols = proto_reply->atom;
		free(proto_reply);
	}
	if (del_reply) {
		s->wm_delete_window = del_reply->atom;
		free(del_reply);
	}

	if (s->wm_protocols && s->wm_delete_window) {
		s->x.change_property(w->conn, XCB_PROP_MODE_REPLACE, w->win,
				     s->wm_protocols, XCB_ATOM_ATOM, 32, 1, &s->wm_delete_window);
	}

	s->x.change_property(w->conn, XCB_PROP_MODE_REPLACE, w->win,
			     XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen(name), name);

	s->x.map_window(w->conn, w->win);
	s->x.flush(w->conn);

	return true;
}

static void _YwPollEventsX11(YwState *s, YwWindowData *w)
{
	xcb_generic_event_t *ev;
	while ((ev = s->x.poll_for_event(w->conn))) {
		switch (ev->response_type & ~0x80) {
		case XCB_EXPOSE:
			break;
		case XCB_CLIENT_MESSAGE: {
			xcb_client_message_event_t *cm = (xcb_client_message_event_t *)ev;
			if (cm->data.data32[0] == s->wm_delete_window) {
				/* window closed */
			}
			break;
		}
		case XCB_KEY_PRESS:
			break;
		}
		free(ev);
	}
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

static bool _YwInit(YwState *s)
{
	s->initialized = true;
	return true;
}

void YwPollEvents(YwState *s, YwWindowData *w)
{
#ifdef YAWL_X11
	_YwPollEventsX11(s, w);
#elif defined(YAWL_WIN32)
#else
	fprintf(stderr, "Ywwl: Unsupported platform\n");
#endif
}

bool YwInitWindow(YwState *s, YwWindowData *w, const char *name)
{
	if (!s->initialized) {
		if (!_YwInit(s))
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
}
#endif // YAWL_IMPLEMENTATION

void YwBeginDrawing(YwState *s, YwWindowData *w)
{
}
void YwEndDrawing(YwState *s, YwWindowData *w)
{
#ifdef YAWL_X11
	s->x.flush(w->conn);
#endif // YAWL_X11
#ifdef YAWL_WIN32
	SwapBuffers(w->hdc);
#endif // YAWL_WIN32
}

#endif // INCLUDE_YAWL_YAWL_H_
