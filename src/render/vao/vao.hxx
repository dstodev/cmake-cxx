#ifndef VAO_HXX
#define VAO_HXX

#include <vector>

#include <attribute.hxx>

namespace project::vao {

class Vao
{
public:
	Vao();

	virtual ~Vao();
	Vao(Vao const& copy);
	Vao(Vao&& move);
	Vao& operator=(Vao const& copy);
	Vao& operator=(Vao&& move);

	void init();  // also calls bind()
	void deinit();

	void bind() const;

	void add_attribute(Attribute&& attribute);
	void init_attributes() const;

protected:
	void init_attribute(Attribute const& attribute) const;

	virtual void gen_buffers();
	virtual void delete_buffers();
	virtual void bind_buffers() const;
	virtual void post_init();

	unsigned _vao;
	unsigned _stride_bytes;
	unsigned _offset;

	std::vector<Attribute> _attributes;
};

}  // namespace project::vao

#endif
