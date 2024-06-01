#define _CRT_SECURE_NO_WARNINGS 1

#include <cstdlib>  // for std::getenv
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <cli.hxx>
#include <project.hxx>

using namespace project;

namespace {
void set_log_level(std::optional<std::string> const& cli_level = std::nullopt);
auto get_env_var(char const* name) -> std::optional<std::string>;
void print_enabled_log_levels();
}  // namespace

int main(int argc, char const* argv[])
{
	Cli const cli {argc, argv};  // May call std::exit(); see Cli constructor documentation.

#if ENABLE_LOGGING
	set_log_level(cli.log_level());
	print_enabled_log_levels();
#endif

	std::cout << "Welcome!" << std::endl;

	return 0;
}

namespace {

void set_log_level(std::optional<std::string> const& cli_level)
{
	log::Level level {log::Level::None};

	// Give CLI precedence over environment variable.
	if (cli_level) {
		level = log::level_from(cli_level.value());
	}
	else if (auto const env_level {get_env_var("LOG_LEVEL")}) {
		level = log::level_from(env_level.value());
	}

	log::set_level(level);
}

auto get_env_var(char const* name) -> std::optional<std::string>
{
	char const* value {std::getenv(name)};
	return value ? std::make_optional(value) : std::nullopt;
}

void print_enabled_log_levels()
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

	// For consistency with the default log target, print to stderr.
	std::cerr << "Logging: ";

	auto const last_item {std::prev(log_levels.rend())};
	for (auto it {log_levels.rbegin()}; it != last_item; ++it) {
		std::cerr << *it << ", ";
	}
	std::cerr << *last_item << std::endl;
}

}  // namespace

#undef _CRT_SECURE_NO_WARNINGS
