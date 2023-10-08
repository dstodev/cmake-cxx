#ifndef SHADER_FACTORY_HXX
#define SHADER_FACTORY_HXX

#include <string>
#include <unordered_map>

#include <shader-program.hxx>

namespace project {

class ShaderFactory
{
public:
	using container_type = std::unordered_map<std::string, ShaderProgram>;

	void compile();
	ShaderProgram const& get(char const* name) const;

	void use(char const* name) const;

private:
	container_type _shaders;
};

}  // namespace project

#endif
