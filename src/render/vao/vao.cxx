#include "vao.hxx"

#include <GL/glew.h>

namespace project::vao {

Vao::Vao()
    : _vao(0)
    , _stride_bytes(0)
    , _offset(0)
{}

Vao::~Vao() = default;
Vao::Vao(Vao const& copy) = default;
Vao::Vao(Vao&& move) = default;
Vao& Vao::operator=(Vao const& copy) = default;
Vao& Vao::operator=(Vao&& move) = default;

void Vao::init()
{
	if (_vao != 0) {
		return;
	}

	glGenVertexArrays(1, &_vao);
	gen_buffers();

	bind();  // VAO first
	bind_buffers();

	init_attributes();
	post_init();
}

void Vao::gen_buffers()
{
	// implement in subclasses
}

void Vao::bind() const
{
	glBindVertexArray(_vao);
}

void Vao::add_attribute(Attribute&& attribute)
{
	_attributes.emplace_back(attribute);
	_stride_bytes += attribute.num_values * attribute.value_size;
	_offset += attribute.offset;
}

void Vao::init_attributes() const
{
	for (auto const& attribute : _attributes) {
		init_attribute(attribute);
	}
}

void Vao::init_attribute(Attribute const& attribute) const
{
	auto const index = attribute.index;
	auto const size = static_cast<GLint>(attribute.num_values);
	auto const type = attribute.gl_type;
	auto const normalized = GL_FALSE;
	auto const stride = static_cast<GLsizei>(_stride_bytes);
	auto const pointer = (void*) static_cast<GLsizeiptr>(attribute.offset);

	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
	glEnableVertexAttribArray(index);
}

void Vao::bind_buffers() const
{
	// implement in subclasses
}

void Vao::post_init()
{
	// implement in subclasses
}

void Vao::deinit()
{
	glDeleteVertexArrays(1, &_vao);
	_vao = 0;
	delete_buffers();  // for subclasses
}
void Vao::delete_buffers()
{
	// implement in subclasses
}

}  // namespace project::vao
