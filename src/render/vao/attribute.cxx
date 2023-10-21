#include <attribute.hxx>

#include <GL/glew.h>

namespace project::vao {

Attribute::Attribute(unsigned index, unsigned num_values, Type type, unsigned offset_index)
    : _value_type(type)
    , _gl_type()
    , _index(index)
    , _num_values(num_values)
    , _offset_bytes()
    , _size_bytes()
    , _value_size_bytes()
{
	switch (_value_type) {
	case Type::Float:
		_gl_type = GL_FLOAT;
		_value_size_bytes = sizeof(float);
		break;
	case Type::Int:
		_gl_type = GL_INT;
		_value_size_bytes = sizeof(int);
		break;
	}
	_size_bytes = num_values * _value_size_bytes;
	_offset_bytes = offset_index * _value_size_bytes;
}

void Attribute::init(unsigned stride_bytes) const
{
	auto const normalized = GL_FALSE;
	auto const num_values = static_cast<GLint>(_num_values);
	auto const stride = static_cast<GLsizei>(stride_bytes);

	// For some reason, glVertexAttribPointer() takes a void* as its offset type
	auto const offset = reinterpret_cast<void*>(static_cast<GLsizeiptr>(_offset_bytes));

	// https://registry.khronos.org/OpenGL-Refpages/gl4/html/glVertexAttribPointer.xhtml
	glVertexAttribPointer(_index, num_values, _gl_type, normalized, stride, offset);
	glEnableVertexAttribArray(_index);
}

unsigned Attribute::size_bytes() const
{
	return _size_bytes;
}

}  // namespace project::vao
