#ifndef __GLIBC_USE
#define __GLIBC_USE(x) 0
#endif
#define YAWL_IMPLEMENTATION
#include "../yawl.h"
void ANativeActivity_onCreate(ANativeActivity *activity, void *savedState, size_t savedStateSize)
{
	(void)savedState;
	(void)savedStateSize;
	YwState s = { 0 };
	YwWindowData w = { 0 };

	YwAndroidSetActivity(&w, activity);
	YwInitWindow(&s, &w, "app");

	while (!w.should_close) {
		YwPollEvents(&s, &w);
		// draw...
		YwEndDrawing(&s, &w);
	}
}
