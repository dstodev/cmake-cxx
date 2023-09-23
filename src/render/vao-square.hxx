#ifndef VAO_SQUARES_HXX
#define VAO_SQUARES_HXX

#include <GL/glew.h>

namespace project::vao {

class Square
{
public:
	~Square();

	void init();

	void bind() const;
	void unbind() const;

	void draw(float const vertices[], int gl_mode = GL_STATIC_DRAW) const;

protected:
	unsigned int _vao;
	unsigned int _vbo;
	unsigned int _ebo;

	void init_indices();
};

}  // namespace project::vao

#endif  // VAO_SQUARES_HXX
