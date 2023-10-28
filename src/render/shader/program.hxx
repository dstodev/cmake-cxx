#ifndef PROGRAM_HXX
#define PROGRAM_HXX

namespace project::shader {

class Program
{
public:
	Program();

	~Program();
	Program(Program&& move) noexcept;
	Program(Program const& copy) = delete;
	Program& operator=(Program&& move) noexcept;
	Program& operator=(Program const& copy) = delete;

	// clang-format off
	static constexpr struct VertexType {} vertex_type;
	static constexpr struct FragmentType {} fragment_type;
	// clang-format on

	void add_shader(char const shader_source[], VertexType) const;
	void add_shader(char const shader_source[], FragmentType) const;
	void link() const;

	void use() const;
	int get_uniform_location(char const* name) const;

protected:
	void add_shader(char const shader_source[], int shader_type) const;

	unsigned _id;
};

}  // namespace project::shader

#endif  // PROGRAM_HXX
