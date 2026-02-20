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
	YwSetVSync(&s,&w, true);
	YwInitWindow(&s, &w2, "Hi2!");
	YwSetVSync(&s,&w2, true);

	load_gl_functions(&s);

	while (!w.should_close && !w2.should_close) {
		YwPollEvents(&s, &w);
		YwGLMakeCurrent(&s, &w);
		YwBeginDrawing(&s, &w);
		gl.ClearColor(0.0f, 1.0f, 0.0f, 1.0f);
		gl.Clear(GL_COLOR_BUFFER_BIT);
		YwEndDrawing(&s, &w);
		YwPollEvents(&s, &w2);
		YwGLMakeCurrent(&s, &w2);
		YwBeginDrawing(&s, &w2);
		gl.ClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		gl.Clear(GL_COLOR_BUFFER_BIT);
		YwEndDrawing(&s, &w2);
	}
}
