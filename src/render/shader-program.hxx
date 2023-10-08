#ifndef SHADER_PROGRAM_HXX
#define SHADER_PROGRAM_HXX

namespace project {

class ShaderProgram
{
public:
	// clang-format off
	static struct VertexType {} vertex_type;
	static struct FragmentType {} fragment_type;
	// clang-format on

	ShaderProgram();

	~ShaderProgram();
	ShaderProgram(ShaderProgram const& copy) = delete;
	ShaderProgram(ShaderProgram&& move) = default;
	ShaderProgram& operator=(ShaderProgram const& copy) = delete;
	ShaderProgram& operator=(ShaderProgram&& move) = default;

	void add_shader(char const shader_source[], VertexType);
	void add_shader(char const shader_source[], FragmentType);
	void link() const;

	void use() const;
	int get_uniform_location(char const* name) const;

protected:
	void add_shader(char const shader_source[], int shader_type);

	unsigned _id;
};

}  // namespace project

#endif  // SHADER_PROGRAM_HXX
