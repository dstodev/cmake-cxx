#ifndef ATTRIBUTE_HXX
#define ATTRIBUTE_HXX

namespace project::vao {  // TODO: move to project::shader?

struct Attribute
{
	enum class Type
	{
		Float,
		Int,
	} type;

	Attribute(unsigned index, unsigned num_values, Type type, unsigned offset);

	int gl_type;
	unsigned index;
	unsigned num_values;
	unsigned size;
	unsigned value_size;
	unsigned offset;
};

}  // namespace project::vao

#endif  // ATTRIBUTE_HXX
