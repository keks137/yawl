#define YAWL_IMPLEMENTATION
#include "../yawl.h"
#define LOADOPENGL_IMPLEMENTATION
#include "../loadopengl.h"

#define key_pressed(k) (!key_prev[k].pressed && key_current[k].pressed)
#define key_released(k) (key_prev[k].pressed && !key_current[k].pressed)
#define key_held(k) (key_current[k].pressed)
#define key_mod_ctl(k) ((key_current[k].pressed & YW_KEYMOD_CTRL))
#define key_mod_alt(k) ((key_current[k].pressed & YW_KEYMOD_ALT))
#define key_mod_super(k) ((key_current[k].pressed & YW_KEYMOD_SUPER))
#define key_mod_shift(k) ((key_current[k].pressed & YW_KEYMOD_SHIFT))

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

	while (!w.should_close) {
		YwPollEvents(&w);
		memcpy(key_prev, key_current, sizeof(key_prev));
		YwKeyEvent keyev = { 0 };
		while (YwNextKeyEvent(&w, &keyev)) {
			key_current[keyev.key] = keyev;
		}
		YwBeginDrawing(&w);
		gl.ClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		if (key_held(YW_KEY_B)) {
			gl.ClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		}
		if (key_held(YW_KEY_B) && key_mod_ctl(YW_KEY_B)) {
			gl.ClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		}
		gl.Clear(GL_COLOR_BUFFER_BIT);
		YwEndDrawing(&w);
	}
}
