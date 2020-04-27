#include "buffers.h"

GLuint buffers::mandelbrot_quad_uid;
GLuint buffers::VBO = 0;
GLuint buffers::VAO = 0;

void buffers::init(void) {
	glGenBuffers(1, &buffers::VBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(buffers::VAO);
	glBindBuffer(GL_ARRAY_BUFFER, buffers::VBO);

	const float mandelbrot_quad[6][2] = {
		{ -1.f, -1.f }, { -1.f, 1.f }, { 1.f, 1.f },
		{ -1.f, -1.f }, { 1.f, -1.f }, { 1.f, 1.f }
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(mandelbrot_quad), mandelbrot_quad, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);
}

void buffers::render(void) {
	static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	glClearBufferfv(GL_COLOR, 0, black);

	glBindVertexArray(buffers::VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}