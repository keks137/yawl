#define YAWL_IMPLEMENTATION
#include "../yawl.h"
#define LOADOPENGL_IMPLEMENTATION
#include "../loadopengl.h"

#define key_pressed(k) (!key_prev[k].pressed && key_current[k].pressed)
#define key_released(k) (key_prev[k].pressed && !key_current[k].pressed)
#define key_held(k) (key_current[k].pressed)
#define key_mod_ctl(k) ((key_current[k].mods & 2) >> 1)
#define key_mod_alt(k) ((key_current[k].mods & 4) >> 2)
#define key_mod_super(k) ((key_current[k].mods & 8) >> 3)
#define key_mod_shift(k) ((key_current[k].mods & 1))

int main()
{
	YwState s = { 0 };
	YwWindowData w = { 0 };
	YwKeyEvent key_current[YW_KEY_COUNT] = { 0 };
	YwKeyEvent key_prev[YW_KEY_COUNT] = { 0 };
	YwInitWindow(&s, &w, "Hi!");
	YwSetVSync(&s, &w, true);
	load_gl_functions(&s);

	while (!w.should_close) {
		YwPollEvents(&s, &w);
		YwKeyEvent keyev = { 0 };
		memcpy(key_prev, key_current, sizeof(key_prev));
		while (YwNextKeyEvent(&s, &w, &keyev)) {
			key_current[keyev.key] = keyev;
		}
		YwBeginDrawing(&s, &w);
		gl.ClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		if (key_held(YW_KEY_B)) {
			gl.ClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		}
		if (key_held(YW_KEY_B) && key_mod_ctl(YW_KEY_B)) {
			gl.ClearColor(1.0f, 0.0f, 1.0f, 1.0f);
		}
		gl.Clear(GL_COLOR_BUFFER_BIT);
		YwEndDrawing(&s, &w);
	}
}
