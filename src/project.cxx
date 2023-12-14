#include "project.hxx"

#include <version.h>

namespace project {

auto version() -> std::string const&
{
	static std::string const version = PROJECT_VERSION;
	return version;
}

}  // namespace project
