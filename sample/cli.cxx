#include "cli.hxx"

#include <filesystem>
#include <iostream>

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
		("l,log-level", "Set log level. Options are: error warn info debug trace.",
			cxxopts::value<std::string>()->default_value("none"))
	;  // clang-format on
}

auto Cli::basename(char const* argv_0) -> char const*
{
	_program_path = std::filesystem::path(argv_0).filename().generic_string();
	return _program_path.c_str();
}

void Cli::parse(int argc, char const* argv[])
{
	auto const result = _options.parse(argc, argv);
	auto const version_string = "Project " + project::version();

	if (result.count("help")) {
		std::cout << version_string << '\n';
		std::cout << _options.help() << std::endl;
		std::exit(0);
	}
	if (!result.unmatched().empty() || argc == 1) {
		std::cout << _options.help() << std::endl;
		std::exit(0);
	}
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
	return std::nullopt;
}
