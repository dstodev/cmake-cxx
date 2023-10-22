#include "vao.hxx"

#include <GL/glew.h>

namespace project::vao {

Vao::Vao()
    : _vao()
    , _vbo()
    , _ebo()
    , _attribute_stride_bytes()
    , _num_indices()
    , _attributes()
{}

Vao::~Vao() = default;
Vao::Vao(Vao const& copy) = default;
Vao::Vao(Vao&& move) = default;
Vao& Vao::operator=(Vao const& copy) = default;
Vao& Vao::operator=(Vao&& move) = default;

void Vao::add_attribute(Attribute&& attribute)
{
	_attributes.emplace_back(attribute);
	_attribute_stride_bytes += attribute.size_bytes();
}

void Vao::init()
{
	bool const initialized = _vao != 0;

	if (initialized) {
		return;
	}

	gen_buffers();

	bind();  // VAO first
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

	init_attributes();
	init_vertex_indices();  // subclass implementation
}

void Vao::gen_buffers()
{
	glGenVertexArrays(1, &_vao);
	glGenBuffers(1, &_vbo);
	glGenBuffers(1, &_ebo);
}

void Vao::bind() const
{
	glBindVertexArray(_vao);
}

void Vao::draw_vertex_data(const float* vertex_data, unsigned int num_elements, int gl_mode) const
{
	set_vertex_data(vertex_data, num_elements, gl_mode);
	draw();
}

void Vao::set_vertex_data(const float vertex_data[], unsigned num_elements, int gl_mode) const
{
	auto const size = static_cast<GLsizeiptr>(num_elements * sizeof(vertex_data[0]));
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, vertex_data, gl_mode);
}

void Vao::draw() const
{
	// This will only draw one shape. EBO indices only index for one set of vertices
	auto const num_indices = static_cast<GLsizei>(_num_indices);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, nullptr);
}

void Vao::init_attributes() const
{
	for (auto const& attribute : _attributes) {
		attribute.init(_attribute_stride_bytes);
	}
}

void Vao::deinit()
{
	delete_buffers();
}

void Vao::delete_buffers()
{
	glDeleteVertexArrays(1, &_vao);
	glDeleteBuffers(1, &_vbo);
	glDeleteBuffers(1, &_ebo);
	_vbo = 0;
	_ebo = 0;
	_vao = 0;
}

}  // namespace project::vao
