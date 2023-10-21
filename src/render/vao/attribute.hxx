#ifndef ATTRIBUTE_HXX
#define ATTRIBUTE_HXX

namespace project::vao {

class Attribute
{
public:
	enum class Type
	{
		Float,
		Int,
	};

	Attribute(unsigned index, unsigned num_values, Type type, unsigned offset_index);

	void init(unsigned stride_bytes) const;

	unsigned size_bytes() const;

private:
	Type _value_type;

	int _gl_type;
	unsigned _index;
	unsigned _num_values;
	unsigned _offset_bytes;
	unsigned _size_bytes;
	unsigned _value_size_bytes;
};

}  // namespace project::vao

#endif  // ATTRIBUTE_HXX
