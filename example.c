#define YAWL_IMPLEMENTATION
#include "yawl.h"
int main()
{
	YwState s = { 0 };
	YwWindowData w = { 0 };
	w.width = 800;
	w.height = 600;
	YwInitWindow(&s, &w, "Hi!");
	while (true) {
		YwBeginDrawing(&s, &w);
		YwEndDrawing(&s, &w);
	}
}
