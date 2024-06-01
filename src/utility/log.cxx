#include "log.hxx"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iterator>
#include <string>
#include <string_view>
#include <vector>

namespace project::log {

namespace detail {
Level LogLevel {Level::Info};
std::FILE* LogTarget {stderr};  // https://en.cppreference.com/w/cpp/io/c/FILE
}  // namespace detail

auto level_from(std::string_view level) -> Level
{
	std::string level_lowercase;
	std::string_view it(level);
	std::transform(it.begin(), it.end(), std::back_inserter(level_lowercase), [](unsigned char c) {
		return static_cast<char>(std::tolower(static_cast<int>(c)));
	});

	Level result {Level::None};

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

auto level_label(Level level) -> std::string_view
{
	switch (level) {
	case Level::Error: return "Error";
	case Level::Warning: return "Warning";
	case Level::Info: return "Info";
	case Level::Debug: return "Debug";
	case Level::Trace: return "Trace";
	default: return "None";
	}
}

void set_level(Level level)
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
	std::vector<std::string_view> log_levels;

	auto const append = [&log_levels](log::Level level) { log_levels.emplace_back(level_label(level)); };

	// clang-format off
	switch (log::get_level()) {
	case log::Level::Trace:   append(log::Level::Trace);   [[fallthrough]];
	case log::Level::Debug:   append(log::Level::Debug);   [[fallthrough]];
	case log::Level::Info:    append(log::Level::Info);    [[fallthrough]];
	case log::Level::Warning: append(log::Level::Warning); [[fallthrough]];
	case log::Level::Error:   append(log::Level::Error);   break;

	case log::Level::None: [[fallthrough]];
	default: append(log::Level::None); break;
	}
	// clang-format on

	fmt::print(detail::LogTarget, "Logging: ");

	auto const last_item {std::prev(log_levels.rend())};

	for (auto it {log_levels.rbegin()}; it != last_item; ++it) {
		fmt::print(detail::LogTarget, "{}, ", *it);
	}

	fmt::print(detail::LogTarget, "{}\n", *last_item);
}

}  // namespace project::log
