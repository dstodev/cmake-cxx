#ifndef SHADER_PROGRAM_HXX
#define SHADER_PROGRAM_HXX

namespace project {

class ShaderProgram
{
public:
	ShaderProgram();

	~ShaderProgram();
	ShaderProgram(ShaderProgram const& copy) = delete;
	ShaderProgram(ShaderProgram&& move) = default;
	ShaderProgram& operator=(ShaderProgram const& copy) = delete;
	ShaderProgram& operator=(ShaderProgram&& move) = default;

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

}  // namespace project

#endif  // SHADER_PROGRAM_HXX
