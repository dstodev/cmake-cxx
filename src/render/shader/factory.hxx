#ifndef FACTORY_HXX
#define FACTORY_HXX

#include <string>
#include <unordered_map>

#include <program.hxx>

namespace project::shader {

class Factory
{
public:
	using container_type = std::unordered_map<std::string, Program>;

	void compile();
	Program const& get(char const* name) const;

	void use(char const* name) const;

private:
	container_type _shaders;
};

}  // namespace project::shader

#endif
