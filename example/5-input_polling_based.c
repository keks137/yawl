#define YAWL_IMPLEMENTATION
#include "../yawl.h"
#define LOADOPENGL_IMPLEMENTATION
#include "../loadopengl.h"

int main()
{
	YwState s = { 0 };
	YwWindowData w = { 0 };
	YwKeyEvent key_current[YW_KEY_COUNT] = { 0 };
	YwKeyEvent key_prev[YW_KEY_COUNT] = { 0 };
	YwInitWindow(&s, &w, "Hi!");
	YwSetVSync(&w, true);
	struct GLFuncs gl = { 0 };
	load_gl_functions(&s, &gl);
	bool b_down;

	while (!w.should_close) {
		YwPollEvents(&w);
		memcpy(key_prev, key_current, sizeof(key_prev));
		YwKeyEvent keyev = { 0 };
		while (YwNextKeyEvent(&w, &keyev)) {
			if (keyev.key == YW_KEY_B) {
				b_down = keyev.pressed;
			}
		}
		YwBeginDrawing(&w);
		gl.ClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		if (b_down) {
			gl.ClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		}
		gl.Clear(GL_COLOR_BUFFER_BIT);
		YwEndDrawing(&w);
	}
}
