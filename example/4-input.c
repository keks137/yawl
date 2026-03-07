#define YAWL_IMPLEMENTATION
#include "../yawl.h"
#define LOADOPENGL_IMPLEMENTATION
#include "../loadopengl.h"

int main()
{
	YwState s = { 0 };
	YwWindow w = { 0 };
	YwKeyEvent key_current[YW_KEY_COUNT] = { 0 };
	YwKeyEvent key_prev[YW_KEY_COUNT] = { 0 };
	YwInitWindow(&s, &w, "Hi!");
	YwSetVSync(&w, true);
	load_gl_functions(&s);

	while (!w.should_close) {
		YwPollEvents(&w);
		memcpy(key_prev, key_current, sizeof(key_prev));
		YwKeyEvent keyev = { 0 };
		while (YwNextKeyEvent(&w, &keyev)) {
			key_current[keyev.key] = keyev;
		}
		YwBeginDrawing(&w);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		if (YwKeyDown(&w, YW_KEY_B)) {
			glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		}
		if (YwKeyDownMods(&w, YW_KEY_B, YW_KEYMOD_CTRL)) {
			glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		}
		glClear(GL_COLOR_BUFFER_BIT);
		YwEndDrawing(&w);
	}
}
