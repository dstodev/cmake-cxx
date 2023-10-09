#include "shader-factory.hxx"

#include <xr-yg-zb-fs.hxx>
#include <xr-yg-zb-vs.hxx>

namespace project {

void ShaderFactory::compile()
{
	{
		auto const& program = _shaders.try_emplace("xr-yg-zb").first->second;
		program.add_shader(xr_yg_zb_vs, ShaderProgram::vertex_type);
		program.add_shader(xr_yg_zb_fs, ShaderProgram::fragment_type);
	}

	for (auto const& [key, program] : _shaders) {
		program.link();
	}
}

ShaderProgram const& ShaderFactory::get(char const* name) const
{
	return _shaders.at(name);
}

void ShaderFactory::use(char const* name) const
{
	get(name).use();
}

}  // namespace project
