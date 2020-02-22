#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <exception>

namespace window {

	struct bounds {
		double x;
		double y;
	};

	extern int width;
	extern int height;
    extern bounds lower_bounds;
    extern bounds upper_bounds;
	extern int max_iter;
	extern GLFWwindow* ref;
	void init(int, int, const char*);
	void display(void);
}