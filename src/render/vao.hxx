#ifndef VAO_HXX
#define VAO_HXX

namespace project {

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

protected:
	virtual void gen_buffers();
	virtual void delete_buffers();
	virtual void bind_buffers() const;
	virtual void post_init();

	unsigned _vao;
};

}  // namespace project

#endif
