#ifndef CLI_HXX
#define CLI_HXX

#include <optional>
#include <string>

#include <cxxopts.hpp>

class Cli
{
public:
	Cli();
	Cli(int argc, char* argv[]);

	void parse(int argc, char* argv[]);
	auto result() const -> cxxopts::ParseResult const&;

	auto log_level() const -> std::optional<std::string>;

private:
	cxxopts::Options _options;
	cxxopts::ParseResult _result;
};

#endif  // CLI_HXX
