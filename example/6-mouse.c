#include <stdio.h>
#define YAWL_IMPLEMENTATION
#include "../yawl.h"
#define LOADOPENGL_IMPLEMENTATION
#include "../loadopengl.h"

int main()
{
	YwState s = { 0 };
	YwWindow w = { 0 };
	YwInitWindow(&s, &w, "Hi!");
	YwSetVSync(&w, true);
	YwSetPollBlocking(&w, true);
	load_gl_functions(&s);

	while (!w.should_close) {
		YwPollEvents(&w);

		if (YwMouseButtonPressed(&w, YW_MOUSE_BUTTON_LEFT)) {
			printf("x: %f, y: %f, wheel x: %f, y: %f\n", w.mouse_x, w.mouse_y, w.mouse_wheel_x, w.mouse_wheel_y);
		}
		YwBeginDrawing(&w);
		glClearColor(1.0, 0.0, 0.0, 1.0);

		if (YwMouseButtonDown(&w, YW_MOUSE_BUTTON_RIGHT)) {
			glClearColor(0.0f, 0.0f, 1.00f, 1.0f);
		}
		if (YwMouseButtonDown(&w, YW_MOUSE_BUTTON_MIDDLE)) {
			glClearColor(1.0f, 0.0f, 0.50f, 1.0f);
		}
		glClear(GL_COLOR_BUFFER_BIT);
		YwEndDrawing(&w);
	}
}
