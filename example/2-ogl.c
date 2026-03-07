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

	load_gl_functions(&s);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

	while (!w.should_close) {
		YwPollEvents(&w);
		YwBeginDrawing(&w);
		glClear(GL_COLOR_BUFFER_BIT);
		YwEndDrawing(&w);
	}
}
