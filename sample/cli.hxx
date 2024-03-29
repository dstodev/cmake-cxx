#ifndef CLI_HXX
#define CLI_HXX

#include <optional>
#include <string>
#include <vector>

#include <cxxopts.hpp>

class Cli
{
public:
	Cli(int argc, char const* argv[]);

	void parse(int argc, char const* argv[]);
	auto log_level() const -> std::optional<std::string>;

private:
	explicit Cli(char const* argv_0);

	auto basename(char const* argv_0) -> char const*;
	auto format_unrecognized_options() const -> std::string;

	std::string _program_name;  // Must initialize before `_options`
	std::vector<std::string> _unrecognized_options;

	cxxopts::Options _options;
	cxxopts::ParseResult _result;
};

#endif  // CLI_HXX
