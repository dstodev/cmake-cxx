#include "shader-program.hxx"

#include <GL/glew.h>

#include <log.hxx>

namespace project {

constexpr auto LOG_SIZE = 512u;

ShaderProgram::ShaderProgram()
    : _id(glCreateProgram())
{}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_id);
}

void ShaderProgram::add_shader(char const shader_source[], VertexType) const
{
	add_shader(shader_source, GL_VERTEX_SHADER);
}

void ShaderProgram::add_shader(char const shader_source[], FragmentType) const
{
	add_shader(shader_source, GL_FRAGMENT_SHADER);
}

void ShaderProgram::add_shader(char const shader_source[], int shader_type) const
{
	int success;
	static char info_log[LOG_SIZE];

	auto shader_id = glCreateShader(shader_type);

	glShaderSource(shader_id, 1, &shader_source, nullptr);
	glCompileShader(shader_id);
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

	if (success) {
		glAttachShader(_id, shader_id);
	}
	else {
		glGetShaderInfoLog(shader_id, sizeof(info_log), nullptr, info_log);
		log::error("ShaderProgram::add_shader(): {}\n", info_log);
	}

	glDeleteShader(shader_id);
}

void ShaderProgram::link() const
{
	int success;
	static char info_log[LOG_SIZE];

	glLinkProgram(_id);
	glGetProgramiv(_id, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(_id, sizeof(info_log), nullptr, info_log);
		log::error("ShaderProgram::link(): {}\n", info_log);
	}
}

void ShaderProgram::use() const
{
	glUseProgram(_id);
}

int ShaderProgram::get_uniform_location(const char* name) const
{
	return glGetUniformLocation(_id, name);
}

}  // namespace project
