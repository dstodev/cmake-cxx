#include "cli.hxx"

#include <filesystem>
#include <iostream>
#include <sstream>

#include <project.hxx>

Cli::Cli(int argc, char const* argv[])
    : Cli(argv[0])
{
	parse(argc, argv);
}

Cli::Cli(char const* argv_0)
    : _options {basename(argv_0), "My example project using my library"}
{
	_options.allow_unrecognised_options();

	// clang-format off
	_options.add_options()
		("h,help", "Print usage")
		("v,version", "Print version")
#if ENABLE_LOGGING
		("l,log-level", "Set log level. LEVEL=error|warning|info|debug|trace",
			cxxopts::value<std::string>()->default_value("none"), "LEVEL")
#endif
		;  // terminate add_options()
	// clang-format on
}

auto Cli::basename(char const* argv_0) -> char const*
{
	// Store in a data member to take ownership of the string lifetime.
	_program_name = std::filesystem::path(argv_0).filename().generic_string();
	return _program_name.c_str();
}

void Cli::parse(int argc, char const* argv[])
{
	auto const result = _options.parse(argc, argv);
	auto const version_string = "Project " + std::string {project::version()};

	int exit_code = -1;  // <0 means no exit, 0 means exit without error, >0 means exit with error.

	// If user provides --help, print version number & help text, then exit without error.
	if (result.count("help")) {
		std::cout << version_string << '\n';
		std::cout << _options.help() << std::endl;
		exit_code = 0;
	}

	// If user provides unrecognized options, print help text then exit with error.
	// Since this is an error condition, print to stderr.
	if (!std::empty(result.unmatched())) {
		_unrecognized_options = project::stable_deduplicate(result.unmatched());
		std::cerr << format_unrecognized_options();
		std::cerr << _options.help() << std::endl;
		exit_code = 1;
	}

	// If user provides --version, print version number then exit without error.
	if (result.count("version")) {
		std::cout << version_string << std::endl;
		exit_code = 0;
	}

	if (exit_code >= 0) {
		std::exit(exit_code);
	}

	_result = result;
}

auto Cli::format_unrecognized_options() const -> std::string
{
	std::stringstream result;

	if (!std::empty(_unrecognized_options)) {
		auto const& last = _unrecognized_options.back();

		result << "Unrecognized options: ";

		for (auto const& option : _unrecognized_options) {
			result << option;

			if (&option != &last) {
				result << ", ";
			}
		}

		result << '\n';
	}

	return result.str();
}

auto Cli::log_level() const -> std::optional<std::string>
{
	if (_result.count("log-level")) {
		return _result["log-level"].as<std::string>();
	}
	else {
		return std::nullopt;
	}
}
