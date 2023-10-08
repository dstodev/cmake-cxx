#include "vao.hxx"

#include <GL/glew.h>

namespace project {

Vao::Vao()
    : _vao(0)
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

}  // namespace project
