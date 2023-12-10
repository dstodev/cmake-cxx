#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS 1  // NOLINT(*-reserved-identifier)
#endif

#include <cstdlib>
#include <optional>
#include <string>

#include <cli.hxx>
#include <log.hxx>

using namespace project;

void set_log_level(std::optional<std::string> const& cli_level = std::nullopt);
auto get_env_var(char const* name) -> std::optional<std::string>;

int main(int argc, char const* argv[])
{
	Cli const cli(argc, argv);

	set_log_level(cli.log_level());

	log::error("Error messages enabled\n");
	log::warn("Warning messages enabled\n");
	log::info("Info messages enabled\n");
	log::debug("Debug messages enabled\n");
	log::trace("Trace messages enabled\n");

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
