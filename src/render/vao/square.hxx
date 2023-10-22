#ifndef VAO_SQUARE_HXX
#define VAO_SQUARE_HXX

#include <vao.hxx>

namespace project::vao {

class Square : public Vao
{
protected:
	void init_vertex_indices() override;
};

}  // namespace project::vao

#endif  // VAO_SQUARE_HXX
