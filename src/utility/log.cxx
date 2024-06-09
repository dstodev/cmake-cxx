#include "log.hxx"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iterator>
#include <string>
#include <string_view>

namespace project::log {

namespace detail {
Level LogLevel {Level::Info};
std::FILE* LogTarget {stderr};  // https://en.cppreference.com/w/cpp/io/c/FILE
}  // namespace detail

auto level_from(std::string_view const level) -> Level
{
	std::string level_lowercase;
	std::transform(level.begin(), level.end(), std::back_inserter(level_lowercase), [](unsigned char const c) {
		return static_cast<char>(std::tolower(c));
	});

	auto result {Level::None};

	if (level_lowercase == "trace") {
		result = Level::Trace;
	}
	else if (level_lowercase == "debug") {
		result = Level::Debug;
	}
	else if (level_lowercase == "info" || level_lowercase == "status") {
		result = Level::Info;
	}
	else if (level_lowercase == "warning" || level_lowercase == "warn") {
		result = Level::Warning;
	}
	else if (level_lowercase == "error") {
		result = Level::Error;
	}

	return result;
}

void set_level(Level const level)
{
	detail::LogLevel = level;
}

auto get_level() -> Level
{
	return detail::LogLevel;
}

void set_target(std::FILE* target)
{
	detail::LogTarget = target;
}

auto get_target() -> std::FILE*
{
	return detail::LogTarget;
}

void print_enabled_levels()
{
	// clang-format off
	static auto constexpr error   {level_label(Level::Error)};
	static auto constexpr warning {level_label(Level::Warning)};
	static auto constexpr info    {level_label(Level::Info)};
	static auto constexpr debug   {level_label(Level::Debug)};
	static auto constexpr trace   {level_label(Level::Trace)};
	static auto constexpr none    {level_label(Level::None)};

	std::string levels {fmt::format("{}, {}, {}, {}, {}", error, warning, info, debug, trace)};

	static auto constexpr error_end    {              error.size()      };
	static auto constexpr warning_end  {error_end   + warning.size() + 2};  // +2 for ", "
	static auto constexpr info_end     {warning_end + info.size()    + 2};
	static auto constexpr debug_end    {info_end    + debug.size()   + 2};
	// auto constexpr trace_end {debug_end   + trace.size()   + 2};

	char const* msg_ptr {levels.data()};

	switch (detail::LogLevel) {
	case Level::Error:   levels[error_end]   = '\0'; break;
	case Level::Warning: levels[warning_end] = '\0'; break;
	case Level::Info:    levels[info_end]    = '\0'; break;
	case Level::Debug:   levels[debug_end]   = '\0'; break;
	case Level::Trace: /* levels[trace_end] = '\0'; */ break;
	case Level::None: msg_ptr = none.data(); break;
	}
	// clang-format on

	fmt::print(detail::LogTarget, "Logging: {}\n", msg_ptr);
}

}  // namespace project::log
