#include "log.hxx"

#include <algorithm>
#include <iterator>
#include <string>

namespace project::log {

namespace detail {
Level LogLevel = Level::Info;
}  // namespace detail

Level level_from(char const* level)
{
	std::string level_lowercase;
	std::string_view it(level);
	std::transform(it.begin(), it.end(), std::back_inserter(level_lowercase), std::tolower);

	Level result = Level::None;

	if (level_lowercase == "trace") {
		result = Level::Trace;
	}
	else if (level_lowercase == "debug") {
		result = Level::Debug;
	}
	else if (level_lowercase == "info") {
		result = Level::Info;
	}
	else if (level_lowercase == "warn") {
		result = Level::Warn;
	}
	else if (level_lowercase == "error") {
		result = Level::Error;
	}

	return result;
}

void set_level(Level level)
{
	detail::LogLevel = level;
}

Level get_level()
{
	return detail::LogLevel;
}

}  // namespace project::log
