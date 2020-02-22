#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <string>

#include "buffers.h"

namespace shader {
	extern GLuint window_dim_uniform;
	extern GLuint upper_bounds_uniform;
	extern GLuint lower_bounds_uniform;
	extern GLuint max_iter_uniform;

	GLuint create_from_file(const char*, GLenum);
	void init(void);
};
