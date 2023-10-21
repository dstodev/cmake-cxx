#ifndef VAO_TRIANGLE_HXX
#define VAO_TRIANGLE_HXX

#include <vao.hxx>

namespace project::vao {

class Triangle : public Vao
{
protected:
	void init_vertex_indices() override;
};

}  // namespace project::vao

#endif  // VAO_TRIANGLE_HXX
