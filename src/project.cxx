#include "project.hxx"

namespace project {

auto version() -> std::string_view
{
	static std::string_view const version {PROJECT_VERSION};
	return version;
}

}  // namespace project
