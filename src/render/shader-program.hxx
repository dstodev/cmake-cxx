#ifndef SHADER_PROGRAM_HXX
#define SHADER_PROGRAM_HXX

#include <filesystem>
#include <string>

namespace project {

class ShaderProgram
{
public:
	ShaderProgram(std::filesystem::path const& path, std::string name);
	~ShaderProgram();

	ShaderProgram(ShaderProgram const& copy) = delete;
	ShaderProgram(ShaderProgram&& move) = default;
	ShaderProgram& operator=(ShaderProgram const& copy) = delete;
	ShaderProgram& operator=(ShaderProgram&& move) = default;

	void add_shader(std::filesystem::path const& path);
	void link() const;
	void use() const;

	int get_uniform_location(char const* name) const;

	std::string name() const;

protected:
	unsigned int _id;
	std::string _name;
	unsigned int _vertex;
	unsigned int _fragment;

	unsigned int get_shader_type(std::filesystem::path const& path) const;
};

}  // namespace project

#endif  // SHADER_PROGRAM_HXX
