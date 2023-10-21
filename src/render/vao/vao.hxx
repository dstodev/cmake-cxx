#ifndef VAO_HXX
#define VAO_HXX

#include <vector>

#include <GL/glew.h>

#include <attribute.hxx>

namespace project::vao {

class Vao  // TODO: rename to Shape?
{
public:
	Vao();

	virtual ~Vao();
	Vao(Vao const& copy);
	Vao(Vao&& move);
	Vao& operator=(Vao const& copy);
	Vao& operator=(Vao&& move);

	/// Add all attributes before calling init()
	void add_attribute(Attribute&& attribute);

	void init();  // calls bind()
	void deinit();

	void bind() const;

	/// Calls bind() and sets vertex data
	void set_vertex_data(const float vertex_data[], unsigned num_elements, int gl_mode = GL_STATIC_DRAW) const;

	/// Draws elements in vertex buffer.
	/// Assumes bind() has already been called.
	void draw() const;

protected:
	void gen_buffers();
	void delete_buffers();
	void init_attributes() const;

	virtual void init_vertex_indices() = 0;

	unsigned _vao;
	unsigned _vbo;
	unsigned _ebo;
	unsigned _attribute_stride_bytes;
	unsigned _num_indices;  // set in init_vertex_indices()

	std::vector<Attribute> _attributes;
};

}  // namespace project::vao

#endif
