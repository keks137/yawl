#define YAWL_IMPLEMENTATION
#include "../yawl.h"
#define LOADOPENGL_IMPLEMENTATION
#include "../loadopengl.h"

int main()
{
	YwState s = { 0 };
	YwWindowData w = { 0 };
	YwWindowData w2 = { 0 };
	YwInitWindow(&s, &w, "Hi!");
	YwSetVSync(&w, true);
	YwInitWindow(&s, &w2, "Hi2!");
	YwSetVSync(&w2, true);

	struct GLFuncs gl = { 0 };
	load_gl_functions(&s, &gl);

	while (!w.should_close && !w2.should_close) {
		YwPollEvents(&w);
		YwGLMakeCurrent(&w);
		YwBeginDrawing(&w);
		gl.ClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		gl.Clear(GL_COLOR_BUFFER_BIT);
		YwEndDrawing(&w);
		YwPollEvents(&w2);
		YwGLMakeCurrent(&w2);
		YwBeginDrawing(&w2);
		gl.ClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		gl.Clear(GL_COLOR_BUFFER_BIT);
		YwEndDrawing(&w2);
	}
}
