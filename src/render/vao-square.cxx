#include "vao-square.hxx"

#include <GL/glew.h>

namespace project::vao {

Square::~Square()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
}

void Square::init()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);

	bind();  // Bind the VAO first
	init_indices();

	// First shader parameter is 3-float position vector
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

//	// Second shader parameter is 3-float color vector
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
//	glEnableVertexAttribArray(1);

	unbind();
}

void Square::init_indices()
{
	static unsigned constexpr indices[] = {
	    // First triangle
	    0,  // top-left
	    1,  // top-right
	    3,  // bottom-left
	    // Second triangle
	    2,  // bottom-right
	    1,  // top-right
	    3,  // bottom-left
	};
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void Square::bind() const
{
	glBindVertexArray(_vao);
}

void Square::unbind() const
{
	glBindVertexArray(0);
}

void Square::draw(float const vertices[], int gl_mode) const
{
	bind();
	// glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertices, gl_mode);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

}  // namespace project::vao
