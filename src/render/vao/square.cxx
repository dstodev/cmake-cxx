#include "square.hxx"

#include <GL/glew.h>

namespace project::vao {

void Square::init_vertex_indices()
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
	_num_indices = sizeof(indices) / sizeof(indices[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

}  // namespace project::vao
