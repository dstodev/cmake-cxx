#include <attribute.hxx>

#include <GL/glew.h>

namespace project::vao {

Attribute::Attribute(unsigned index, unsigned num_values, Type type, unsigned offset)
    : type(type)
    , index(index)
    , num_values(num_values)
    , value_size()
    , size()
    , offset(offset)
{
	switch (type) {
	case Type::Float:
		gl_type = GL_FLOAT;
		value_size = sizeof(float);
		break;
	case Type::Int:
		gl_type = GL_INT;
		value_size = sizeof(int);
		break;
	}
	size = num_values * value_size;
}

}  // namespace project::vao
