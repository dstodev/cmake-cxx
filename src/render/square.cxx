#include "square.hxx"

#include <GL/glew.h>

namespace project {

void Square::set_vertices(const float vertices[], unsigned num_squares, int gl_mode)
{
	_num_squares = num_squares;
	GLsizeiptr const num_floats = _num_squares * shader_parameters_per_square;
	bind();
	glBufferData(GL_ARRAY_BUFFER, num_floats * static_cast<GLsizeiptr>(sizeof(float)), vertices, gl_mode);
}

void Square::draw() const
{
	auto const num_indices = static_cast<GLsizei>(_num_squares * indices_per_square);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
}

void Square::gen_buffers()
{
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
}

void Square::delete_buffers()
{
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
	_vbo = 0;
	_ebo = 0;
}

void Square::bind_buffers() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
}

void Square::post_init()
{
	init_indices();

	// First shader parameter is 3-float position vector
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	// TODO: Add these to shader class as "register_attributes" which returns the number of shader parameters (always
	//       float?) per vertex

	//	// Second shader parameter is 3-float color vector
	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	//	glEnableVertexAttribArray(1);
}

void Square::init_indices() const
{
	static unsigned constexpr indices[] = {
	    //  First triangle
	    0,  // top-left      |----/
	    1,  // top-right     |  /
	    3,  // bottom-left   |/

	    //  Second triangle
	    2,  // bottom-right      /|
	    1,  // top-right       /  |
	    3,  // bottom-left   /----|
	};

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

}  // namespace project
