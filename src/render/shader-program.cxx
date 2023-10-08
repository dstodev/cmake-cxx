#include "shader-program.hxx"

#include <GL/glew.h>

#include <file-to-string.hxx>
#include <log.hxx>

namespace project {

constexpr auto LOG_SIZE = 512u;

ShaderProgram::ShaderProgram(path_type const& first_shader, std::string&& name)
    : _id(glCreateProgram())
    , _name(std::move(name))
    , _vertex(0)
    , _fragment(0)
{
	add_shader(first_shader);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(_id);
}

std::string ShaderProgram::name() const
{
	return _name;
}

void ShaderProgram::add_shader(path_type const& path)
{
	auto const shader_type = get_shader_type(path);
	int success;
	static char info_log[LOG_SIZE];

	// The shader source is read as one single string (with multiple newlines),
	// which is compilable by OpenGL.
	auto shader_source = file_to_string(path);
	auto const* shader_source_ptr = shader_source.c_str();

	auto shader_id = glCreateShader(shader_type);

	glShaderSource(shader_id, 1, &shader_source_ptr, nullptr);
	glCompileShader(shader_id);
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

	auto const shader_filename = path.filename().string();

	if (success) {
		log::info("Compiling {} succeeded!\n", shader_filename);

		switch (shader_type) {
		case GL_VERTEX_SHADER: _vertex = shader_id; break;
		case GL_FRAGMENT_SHADER: _fragment = shader_id; break;
		default: break;
		}

		glAttachShader(_id, shader_id);
	}
	else {
		glGetShaderInfoLog(shader_id, sizeof(info_log), nullptr, info_log);
		log::error("Compiling {} failed:\n{}\n", shader_filename, info_log);
	}

	glDeleteShader(shader_id);
}

void ShaderProgram::link() const
{
	int success;
	static char info_log[LOG_SIZE];

	glLinkProgram(_id);
	glGetProgramiv(_id, GL_LINK_STATUS, &success);

	if (success) {
		log::info("Linking {} succeeded!\n", _name);
	}
	else {
		glGetProgramInfoLog(_id, sizeof(info_log), nullptr, info_log);
		log::error("Linking {} failed:\n{}\n", _name);
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

unsigned ShaderProgram::get_shader_type(path_type const& path) const
{
	auto const extension = path.extension();
	auto const filename = path.filename().string();

	unsigned shader_type = 0;

	if (extension == ".vs") {
		log::info("Compiling vertex shader: {}\n", filename);
		shader_type = GL_VERTEX_SHADER;
	}
	else if (extension == ".fs") {
		log::info("Compiling fragment shader: {}\n", filename);
		shader_type = GL_FRAGMENT_SHADER;
	}
	else {
		log::warn("Unknown shader file: {}\n", path.string());
	}

	return shader_type;
}

}  // namespace project
