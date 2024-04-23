#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS 1  // NOLINT(*-reserved-identifier)
#endif

#include <cstdlib>
#include <iostream>
#include <optional>
#include <string>

#include <cli.hxx>
#include <project.hxx>

using namespace project;

void set_log_level(std::optional<std::string> const& cli_level = std::nullopt);
auto get_env_var(char const* name) -> std::optional<std::string>;
void print_enabled_log_levels();

int main(int argc, char const* argv[])
{
	Cli const cli(argc, argv);

	auto test = cli.log_level();

#if ENABLE_LOGGING
	set_log_level(cli.log_level());
	print_enabled_log_levels();
#endif

	std::cout << "Welcome!" << std::endl;

	return 0;
}

void set_log_level(std::optional<std::string> const& cli_level)
{
	log::Level level = log::Level::None;

	// Give CLI precedence over environment variable.
	if (cli_level) {
		level = log::level_from(cli_level.value());
	}
	else if (auto const env_level = get_env_var("LOG_LEVEL")) {
		level = log::level_from(env_level.value());
	}

	log::set_level(level);
}

auto get_env_var(char const* name) -> std::optional<std::string>
{
	char const* value = std::getenv(name);
	return value ? std::make_optional(value) : std::nullopt;
}

void print_enabled_log_levels()
{
	std::cout << "Logging: ";
	std::vector<char const*> log_levels;
	switch (log::get_level()) {
	case log::Level::Trace: log_levels.emplace_back("Trace"); [[fallthrough]];
	case log::Level::Debug: log_levels.emplace_back("Debug"); [[fallthrough]];
	case log::Level::Info: log_levels.emplace_back("Info"); [[fallthrough]];
	case log::Level::Warn: log_levels.emplace_back("Warning"); [[fallthrough]];
	case log::Level::Error: log_levels.emplace_back("Error"); [[fallthrough]];
	default:;
	}
	if (log_levels.empty()) {
		std::cout << "None";
	}
	else {
		auto const last_item = std::prev(log_levels.rend());
		for (auto it = log_levels.rbegin(); it != last_item; ++it) {
			std::cout << *it;
			std::cout << ", ";
		}
		std::cout << *last_item;
	}
	std::cout << std::endl;
}
