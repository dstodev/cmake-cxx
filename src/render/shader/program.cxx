#include "program.hxx"

#include <GL/glew.h>

#include <log.hxx>

namespace project::shader {

constexpr auto LOG_SIZE = 512u;

Program::Program()
    : _id(glCreateProgram())
{}

Program::~Program()
{
	glDeleteProgram(_id);
}

Program::Program(Program&& move) noexcept = default;
Program& Program::operator=(Program&& move) noexcept = default;

void Program::add_shader(char const shader_source[], VertexType) const
{
	add_shader(shader_source, GL_VERTEX_SHADER);
}

void Program::add_shader(char const shader_source[], FragmentType) const
{
	add_shader(shader_source, GL_FRAGMENT_SHADER);
}

void Program::add_shader(char const shader_source[], int shader_type) const
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

void Program::link() const
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

void Program::use() const
{
	glUseProgram(_id);
}

int Program::get_uniform_location(const char* name) const
{
	return glGetUniformLocation(_id, name);
}

}  // namespace project::shader
