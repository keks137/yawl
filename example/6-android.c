#ifndef __GLIBC_USE
#define __GLIBC_USE(x) 0
#endif
#define YAWL_IMPLEMENTATION
#include "../yawl.h"
#define LOADOPENGL_IMPLEMENTATION
#include "../loadopengl.h"

#define LOG_TAG "MyNativeCode"
#include <android/log.h>
#include <pthread.h>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// Structure to hold our app data (passed to the rendering thread)
typedef struct {
	YwState state;
	YwWindowData window;
	ANativeActivity *activity;
} App;

void game_main(YwState *s, YwWindowData *w);
// The rendering thread function
static void *rendering_thread(void *arg)
{
	LOGI("Rendering thread started");
	App *app = (App *)arg;

	// Prepare a looper for this thread and attach the pipe's read end
	ALooper *looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
	ALooper_addFd(looper, app->window.msgread, ALOOPER_POLL_CALLBACK,
		      ALOOPER_EVENT_INPUT, _YwAndroidPipeCallback, &app->window);
	app->window.looper = looper; // store if needed elsewhere

	game_main(&app->state, &app->window);
	// Now initialize the window – this will wait until the main thread sends CMD_WINDOW_CREATED

	return NULL;
}

void ANativeActivity_onCreate(ANativeActivity *activity,
			      void *savedState, size_t savedStateSize)
{
	(void)savedState;
	(void)savedStateSize;

	// Allocate app data (will be freed later, but for simplicity we ignore cleanup)
	App *app = (App *)calloc(1, sizeof(App));
	app->activity = activity;

	// Set up yawl callbacks and create the pipe (does NOT attach to main looper now)
	YwAndroidSetActivity(&app->state, &app->window, activity);

	// Create the rendering thread (detached so it cleans up automatically)
	pthread_t thread;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&thread, &attr, rendering_thread, app);
	pthread_attr_destroy(&attr);

	// Return immediately – main thread now handles Android events
}

void game_main(YwState *s, YwWindowData *w)
{
	if (!YwInitWindow(s, w, "app")) {
		LOGI("YwInitWindow failed");
		return;
	}

	YwSetVSync(w, true);

	struct GLFuncs gl = { 0 };
	load_gl_functions(s, &gl);
	gl.ClearColor(1.0f, 0.0f, 1.0f, 1.0f);

	while (!w->should_close) {
		YwPollEvents(w);
		YwBeginDrawing(w);
		gl.Clear(GL_COLOR_BUFFER_BIT);
		YwEndDrawing(w);
	}
}
