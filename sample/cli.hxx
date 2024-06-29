#ifndef CLI_HXX
#define CLI_HXX

#include <optional>
#include <string>

#include <cxxopts.hpp>

class Cli
{
public:
	/** @brief Parse command line options.

	    Calls @c std::exit() if user provides:
	    @li invalid options: print error message & help text, then exit with error.
	    @li @c --help : print version number & help text, then exit without error.
	    @li @c --version : print version number, then exit without error.
	 */
	Cli(int argc, char const* argv[]);

	auto log_level() const -> std::optional<std::string>;

private:
	explicit Cli(char const* argv_0);

	void parse(int argc, char const* argv[]);

	auto basename(char const* argv_0) -> char const*;

	std::string _program_name;  // Must initialize before `_options`

	cxxopts::Options _options;
	cxxopts::ParseResult _result;
};

#endif  // CLI_HXX
