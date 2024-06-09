#include "cli.hxx"

#include <filesystem>
#include <iostream>

#include <project.hxx>

Cli::Cli(int const argc, char const* argv[])
    : Cli {argv[0]}
{
	parse(argc, argv);
}

Cli::Cli(char const* argv_0)
    : _program_name {}
    , _options {basename(argv_0), "My example application using my library\n"}
    , _result {}
{
	_options.set_width(80);

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

void Cli::parse(int const argc, char const* argv[])
{
	cxxopts::ParseResult result {};

	try {
		result = _options.parse(argc, argv);
	} catch (cxxopts::exceptions::parsing const& e) {
		// If user provides invalid options: print help text, then exit with error.
		// Since this is an error condition, print to stderr.
		std::cerr << "Error parsing options:\n\t" << e.what() << "\n\n";
		std::cerr << _options.help() << std::endl;
		std::exit(1);
	}

	auto const version_string {"Project " + std::string {project::version()}};

	// If user provides --help: print version number & help text, then exit without error.
	if (result.count("help")) {
		std::cout << version_string << '\n';
		std::cout << _options.help() << std::endl;
		std::exit(0);
	}

	// If user provides --version: print version number, then exit without error.
	if (result.count("version")) {
		std::cout << version_string << std::endl;
		std::exit(0);
	}

	_result = result;
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
