#define YAWL_IMPLEMENTATION
#include "yawl.h"
#define LOADOPENGL_IMPLEMENTATION
#include "loadopengl.h"

int main()
{
	YwState s = { 0 };
	YwWindowData w = { 0 };
	YwInitWindow(&s, &w, "Hi!");

	load_gl_functions(&s);
	gl.ClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	while (!w.should_close) {
		YwPollEvents(&s, &w);
		YwBeginDrawing(&s, &w);
		gl.Clear(GL_COLOR_BUFFER_BIT);
		YwEndDrawing(&s, &w);
	}
}
