#include "factory.hxx"

#include <xr-yg-zb-fs.hxx>
#include <xr-yg-zb-vs.hxx>

namespace project::shader {

void Factory::compile()
{
	{
		auto const& program = _shaders.try_emplace("xr-yg-zb").first->second;
		program.add_shader(xr_yg_zb_vs, Program::vertex_type);
		program.add_shader(xr_yg_zb_fs, Program::fragment_type);
	}

	for (auto const& [key, program] : _shaders) {
		program.link();
	}
}

Program const& Factory::get(char const* name) const
{
	return _shaders.at(name);
}

void Factory::use(char const* name) const
{
	get(name).use();
}

}  // namespace project::shader
