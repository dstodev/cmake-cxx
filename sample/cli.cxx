#include "cli.hxx"

#include <iostream>

#include <version.hxx>

Cli::Cli()
    : _options {"Project", "My example project using my library"}
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

Cli::Cli(int argc, char const* argv[])
    : Cli()
{
	parse(argc, argv);
}

void Cli::parse(int argc, char const* argv[])
{
	auto const result = _options.parse(argc, argv);

	if (result.count("help") || !result.unmatched().empty()) {
		std::cout << _options.help() << std::endl;
		std::exit(0);
	}
	if (result.count("version")) {
		std::cout << _options.program() << ' ' << PROJECT_VERSION << std::endl;
		std::exit(0);
	}

	_result = result;
}

auto Cli::result() const -> cxxopts::ParseResult const&
{
	return _result;
}

auto Cli::log_level() const -> std::optional<std::string>
{
	if (_result.count("log-level")) {
		return _result["log-level"].as<std::string>();
	}
	return std::nullopt;
}
