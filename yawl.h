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

struct _YwWGLFuncs {
	bool loaded;
	bool ext_loaded;
	PROC(WINAPI *get_proc_address)(LPCSTR);
	HGLRC(WINAPI *create_context)(HDC);
	BOOL(WINAPI *delete_context)(HGLRC);
	BOOL(WINAPI *make_current)(HDC, HGLRC);
	BOOL(WINAPI *share_lists)(HGLRC, HGLRC);
	BOOL(WINAPI *swap_interval)(int);
};

#endif // YAWL_WIN32

#ifdef YAWL_X11
#define YAWL_EGL_EXTENDED
#include <xcb/xcb.h>
#include <xcb/xproto.h>
#include <EGL/egl.h>
#ifdef YAWL_EGL_EXTENDED
#include <EGL/eglext.h>
#endif // YAWL_EGL_EXTENDED
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

struct _YwEGLFuncs {
	bool loaded;

	void *(EGLAPIENTRY *get_proc_address)(const char *);
	EGLDisplay (*get_display)(EGLNativeDisplayType);
	EGLBoolean (*initialize)(EGLDisplay, EGLint *, EGLint *);
	EGLBoolean (*terminate)(EGLDisplay);
	EGLContext (*create_context)(EGLDisplay, EGLConfig, EGLContext, const EGLint *);
	EGLBoolean (*make_current)(EGLDisplay, EGLSurface, EGLSurface, EGLContext);
	EGLBoolean (*swap_buffers)(EGLDisplay, EGLSurface);
	EGLSurface (*create_window_surface)(EGLDisplay, EGLConfig, EGLNativeWindowType, const EGLint *);
	EGLBoolean (*bind_api)(EGLenum);
	EGLBoolean (*choose_config)(EGLDisplay, const EGLint *, EGLConfig *, EGLint, EGLint *);
	EGLint (*get_error)(void);
	EGLBoolean (*swap_interval)(EGLDisplay, EGLint);
#ifdef YAWL_EGL_EXTENDED
	PFNEGLGETPLATFORMDISPLAYEXTPROC get_platform_display;
#endif // YAWL_EGL_EXTENDED
};

#endif //YAWL_X11

typedef struct {
	size_t width;
	size_t height;
	bool should_close;
#ifdef YAWL_X11
	xcb_connection_t *conn;
	xcb_window_t win;
	xcb_screen_t *screen;
	EGLSurface egl_surface;
	EGLContext egl_context;
	EGLDisplay egl_display;
#endif // YAWL_X11
#ifdef YAWL_WIN32
	HWND hwnd;
	HDC hdc;
	HGLRC gl_context;
#endif // YAWL_WIN32
} YwWindowData;

typedef struct {
	bool initialized;
#ifdef YAWL_X11
	void *lgl;
	int screen_num;
	xcb_atom_t wm_protocols;
	xcb_atom_t wm_delete_window;
	struct _YwX11Funcs x;
	struct _YwEGLFuncs e;
#endif // YAWL_X11
#ifdef YAWL_WIN32
	HINSTANCE hinstance;
	HMODULE opengl32;
	struct _YwWGLFuncs wgl;
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
bool YwLoadGLProc(YwState *s, void **proc, const char *name);
void YwSetVSync(YwState *s, YwWindowData *w, bool enabled);

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

	YW_LOAD_SYMBOL(s->e.get_proc_address, egl, "eglGetProcAddress");
	YW_LOAD_SYMBOL(s->e.get_display, egl, "eglGetDisplay");
	YW_LOAD_SYMBOL(s->e.initialize, egl, "eglInitialize");
	YW_LOAD_SYMBOL(s->e.terminate, egl, "eglTerminate");
	YW_LOAD_SYMBOL(s->e.create_context, egl, "eglCreateContext");
	YW_LOAD_SYMBOL(s->e.make_current, egl, "eglMakeCurrent");
	YW_LOAD_SYMBOL(s->e.swap_buffers, egl, "eglSwapBuffers");
	YW_LOAD_SYMBOL(s->e.create_window_surface, egl, "eglCreateWindowSurface");
	YW_LOAD_SYMBOL(s->e.bind_api, egl, "eglBindAPI");
	YW_LOAD_SYMBOL(s->e.choose_config, egl, "eglChooseConfig");
	YW_LOAD_SYMBOL(s->e.get_error, egl, "eglGetError");
	YW_LOAD_SYMBOL(s->e.swap_interval, egl, "eglSwapInterval");
	// #ifdef YAWL_EGL_EXTENDED
	YW_LOAD_SYMBOL(s->e.get_platform_display, egl, "eglGetPlatformDisplay");
	// #endif // YAWL_EGL_EXTENDED
	s->e.loaded = true;

	return true;
}
static bool _YwEGLCreateContext(YwState *s, YwWindowData *w)
{
#ifdef YAWL_X11
	w->egl_display = s->e.get_platform_display(EGL_PLATFORM_XCB_EXT, w->conn, NULL);
#else
	w->egl_display = s->e.get_display(w->conn);
#endif // YAWL_X11

	if (w->egl_display == EGL_NO_DISPLAY)
		return false;

	EGLint major, minor;
	if (!s->e.initialize(w->egl_display, &major, &minor))
		return false;

	s->e.bind_api(EGL_OPENGL_API);

	EGLint attribs[] = {
		EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_NONE
	};

	EGLConfig config;
	EGLint num_configs;
	if (!s->e.choose_config(w->egl_display, attribs, &config, 1, &num_configs) || num_configs < 1) {
		fprintf(stderr, "No EGL config found\n");
		return false;
	}

	w->egl_surface = s->e.create_window_surface(w->egl_display, config, w->win, NULL);
	if (w->egl_surface == EGL_NO_SURFACE) {
		EGLint err = s->e.get_error();
		fprintf(stderr, "eglCreateWindowSurface failed: 0x%x (%d)\n", err, err);
		return false;
	}

	EGLint ctx_attribs[] = { EGL_NONE };
	w->egl_context = s->e.create_context(w->egl_display, config, EGL_NO_CONTEXT, ctx_attribs);
	if (w->egl_context == EGL_NO_CONTEXT)
		return false;

	s->e.make_current(w->egl_display, w->egl_surface, w->egl_surface, w->egl_context);
	s->e.swap_interval(w->egl_display, 0); // disable VSYNC
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
	if (w->width == 0 || w->height == 0) {
		w->width = w->screen->width_in_pixels;
		w->height = w->screen->height_in_pixels;
	}

	w->win = s->x.generate_id(w->conn);
	uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	uint32_t values[2] = {
		w->screen->black_pixel,
		XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_STRUCTURE_NOTIFY
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

	if (!s->e.loaded)
		if (!_YwEGLLoad(s))
			return false;

	if (!_YwEGLCreateContext(s, w))
		return false;

	s->e.swap_buffers(w->egl_display, w->egl_surface);

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
				w->should_close = true;
			}
			break;
		}
		case XCB_CONFIGURE_NOTIFY: {
			xcb_configure_notify_event_t *cn = (xcb_configure_notify_event_t *)ev;
			w->width = cn->width;
			w->height = cn->height;
			break;
		}
		case XCB_KEY_PRESS:
			break;
		}
		free(ev);
	}
}
static bool _YwLoadGLProcX11(YwState *s, void **proc, const char *name)
{
	void *p = NULL;
	p = s->e.get_proc_address(name);
	*proc = p;
	return p != NULL;
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

static bool _YwWGLLoadExtensions(YwState *s)
{
	if (!s->wgl.loaded)
		return false;

	void *p = (void *)s->wgl.get_proc_address("wglSwapIntervalEXT");
	if (!p) {
		fprintf(stderr, "wglSwapIntervalEXT not available\n");
		return false;
	}
	s->wgl.swap_interval = (BOOL(WINAPI *)(int))p;
	s->wgl.ext_loaded = true;

	return true;
}
static bool _YwWGLLoad(YwState *s)
{
	if (!s->opengl32) {
		s->opengl32 = LoadLibraryA("opengl32.dll");
		if (!s->opengl32)
			return false;
	}

	YW_LOAD_SYMBOL(s->wgl.get_proc_address, s->opengl32, "wglGetProcAddress");
	YW_LOAD_SYMBOL(s->wgl.create_context, s->opengl32, "wglCreateContext");
	YW_LOAD_SYMBOL(s->wgl.delete_context, s->opengl32, "wglDeleteContext");
	YW_LOAD_SYMBOL(s->wgl.make_current, s->opengl32, "wglMakeCurrent");
	YW_LOAD_SYMBOL(s->wgl.share_lists, s->opengl32, "wglShareLists");

	s->wgl.loaded = true;
	return true;
}

static LRESULT CALLBACK _YwWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	YwWindowData *w = (YwWindowData *)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

	switch (msg) {
	case WM_CREATE: {
		CREATESTRUCTA *cs = (CREATESTRUCTA *)lparam;
		SetWindowLongPtrA(hwnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
		return 0;
	}
	case WM_DESTROY:
		if (w)
			w->should_close = true;
		PostQuitMessage(0);
		return 0;
	case WM_SIZE:
		if (w) {
			w->width = LOWORD(lparam);
			w->height = HIWORD(lparam);
		}
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
		NULL, NULL, s->hinstance, w);

	if (!w->hwnd)
		return false;

	ShowWindow(w->hwnd, SW_SHOW);
	UpdateWindow(w->hwnd);
	w->hdc = GetDC(w->hwnd);
	if (!_YwWGLLoad(s))
		return false;
	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pf = ChoosePixelFormat(w->hdc, &pfd);
	SetPixelFormat(w->hdc, pf, &pfd);

	w->gl_context = s->wgl.create_context(w->hdc);
	s->wgl.make_current(w->hdc, w->gl_context);

	if (!s->wgl.ext_loaded)
		if (!_YwWGLLoadExtensions(s))
			return false;

	s->wgl.swap_interval(0);
	return true;
}
static void _YwPollEventsWin32(YwState *s, YwWindowData *w)
{
	MSG msg;
	while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			w->should_close = true;
		}
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
	}
}

static bool _YwLoadGLProcWin32(YwState *s, void **proc, const char *name)
{
	void *p = (void *)s->wgl.get_proc_address(name);
	if (!p)
		p = (void *)GetProcAddress(s->opengl32, name);
	*proc = p;
	return p != NULL;
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
	_YwPollEventsWin32(s, w);
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
void YwBeginDrawing(YwState *s, YwWindowData *w)
{
#ifdef YAWL_X11
	s->e.make_current(w->egl_display, w->egl_surface, w->egl_surface, w->egl_context);
#endif
#ifdef YAWL_WIN32
	s->wgl.make_current(w->hdc, w->gl_context);
#endif // YAWL_WIN32
}
void YwEndDrawing(YwState *s, YwWindowData *w)
{
#ifdef YAWL_X11
	// s->x.flush(w->conn);
	s->e.swap_buffers(w->egl_display, w->egl_surface);
#endif // YAWL_X11
#ifdef YAWL_WIN32
	SwapBuffers(w->hdc);
#endif // YAWL_WIN32
}

bool YwLoadGLProc(YwState *s, void **proc, const char *name)
{
#ifdef YAWL_WIN32
	return _YwLoadGLProcWin32(s, proc, name);
#endif // YAWL_WIN32
#ifdef YAWL_X11
	return _YwLoadGLProcX11(s, proc, name);
#endif // YAWL_X11
}

void YwSetVSync(YwState *s, YwWindowData *w, bool enabled)
{
	int interval = enabled ? 1 : 0;
#ifdef YAWL_X11
	if (s->e.loaded)
		s->e.swap_interval(w->egl_display, interval);
#endif
#ifdef YAWL_WIN32
	if (s->wgl.loaded)
		s->wgl.swap_interval(interval);
#endif
}

#endif // YAWL_IMPLEMENTATION

#endif // INCLUDE_YAWL_YAWL_H_
