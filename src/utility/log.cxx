#include "log.hxx"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iterator>
#include <string>

namespace project::log {

namespace detail {
Level LogLevel = Level::Info;
FILE* LogTarget = stderr;
}  // namespace detail

Level level_from(std::string_view level)
{
	std::string level_lowercase;
	std::string_view it(level);
	std::transform(it.begin(), it.end(), std::back_inserter(level_lowercase), [](unsigned char c) {
		return static_cast<char>(std::tolower(static_cast<int>(c)));
	});

	Level result = Level::None;

	if (level_lowercase == "trace") {
		result = Level::Trace;
	}
	else if (level_lowercase == "debug") {
		result = Level::Debug;
	}
	else if (level_lowercase == "info" || level_lowercase == "status") {
		result = Level::Info;
	}
	else if (level_lowercase == "warn" || level_lowercase == "warning") {
		result = Level::Warning;
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

void set_target(FILE* target)
{
	detail::LogTarget = target;
}

FILE* get_target()
{
	return detail::LogTarget;
}

}  // namespace project::log
