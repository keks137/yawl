#include <assert.h>
#define YAWL_IMPLEMENTATION
#include "yawl.h"
int main()
{
	YaState s = { 0 };
	YaWindowData w = { 0 };
	w.width = 800;
	w.height = 600;
	assert(YaInitWindow(&s, &w, "Hi!"));
	while (true) {
		YaBeginDrawing(&s, &w);
		YaEndDrawing(&s, &w);
	}
}
