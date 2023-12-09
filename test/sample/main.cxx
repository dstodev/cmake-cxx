#include <cstdlib>
#include <iostream>

#include <cxxopts.hpp>

#include <log.hxx>
#include <version.hxx>

using namespace project;

cxxopts::ParseResult parse_cli_options(int argc, char* argv[]);
void set_log_level(cxxopts::ParseResult const& cli);

int main(int argc, char* argv[])
{
	auto cli = parse_cli_options(argc, argv);

	set_log_level(cli);

	log::error("Error messages enabled\n");
	log::warn("Warning messages enabled\n");
	log::info("Info messages enabled\n");
	log::debug("Debug messages enabled\n");
	log::trace("Trace messages enabled\n");

	return 0;
}

cxxopts::ParseResult parse_cli_options(int argc, char* argv[])
{
	cxxopts::Options options("Project", "My example project using my library");

	options.allow_unrecognised_options();

	// clang-format off
	options.add_options()
		("h,help", "Print usage")
		("v,version", "Print version")
		("l,log-level", "Set log level. Options are: error warn info debug trace.",
			cxxopts::value<std::string>()->default_value("none"))
	;  // clang-format on

	auto result = options.parse(argc, argv);

	if (result.count("help") || !result.unmatched().empty()) {
		std::cout << options.help() << std::endl;
		std::exit(0);
	}
	else if (result.count("version")) {
		std::cout << options.program() << ' ' << PROJECT_VERSION << std::endl;
		std::exit(0);
	}

	return result;
}

void set_log_level(cxxopts::ParseResult const& cli)
{
	log::Level level = log::Level::None;

	if (cli.count("log-level")) {
		auto const level_str = cli["log-level"].as<std::string>();
		level = log::level_from(level_str);
	}
	else if (char const* level_str = std::getenv("LOG_LEVEL")) {
		level = log::level_from(level_str);
	}

	log::set_level(level);
}
