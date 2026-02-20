#define YAWL_IMPLEMENTATION
#include "../yawl.h"
#define LOADOPENGL_IMPLEMENTATION
#include "../loadopengl.h"

int main()
{
	YwState s = { 0 };
	YwWindowData w = { 0 };
	YwInitWindow(&s, &w, "Hi!");
	YwSetVSync(&w, true);

	struct GLFuncs gl = { 0 };
	load_gl_functions(&s, &gl);
	gl.ClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	while (!w.should_close) {
		YwPollEvents(&w);
		YwBeginDrawing(&w);
		gl.Clear(GL_COLOR_BUFFER_BIT);
		YwEndDrawing(&w);
	}
}
