#include "triangle.hxx"

namespace project::vao {

void Triangle::init_vertex_indices()
{
	static unsigned constexpr indices[] = {0, 1, 2};
	_num_indices = sizeof(indices) / sizeof(indices[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

}  // namespace project::vao
