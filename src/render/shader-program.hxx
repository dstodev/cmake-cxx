#ifndef SHADER_PROGRAM_HXX
#define SHADER_PROGRAM_HXX

#include <filesystem>
#include <string>

namespace project {

class ShaderProgram
{
public:
	using path_type = std::filesystem::path;

	ShaderProgram(path_type const& first_shader, std::string&& name);
	~ShaderProgram();

	ShaderProgram(ShaderProgram const& copy) = delete;
	ShaderProgram(ShaderProgram&& move) = default;
	ShaderProgram& operator=(ShaderProgram const& copy) = delete;
	ShaderProgram& operator=(ShaderProgram&& move) = default;

	std::string name() const;
	void add_shader(path_type const& path);
	void link() const;

	void use() const;
	int get_uniform_location(char const* name) const;

protected:
	unsigned _id;
	std::string _name;
	unsigned _vertex;
	unsigned _fragment;

	unsigned get_shader_type(path_type const& path) const;
};

}  // namespace project

#endif  // SHADER_PROGRAM_HXX
