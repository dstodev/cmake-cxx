#ifndef LOG_HXX
#define LOG_HXX

#include <cstdio>  // for std::FILE
#include <string_view>
#include <utility>  // for std::forward

#include <fmt/core.h>

#include <project_dll-export.h>

namespace project::log {

/** @brief Logging severity level.
@code
-- More severe --
 Level::Error
 Level::Warning
 Level::Info
 Level::Debug
 Level::Trace
-- Less severe --
@endcode
 */
enum class Level
{
	None,

	Error,
	Warning,
	Info,
	Debug,
	Trace,
};

namespace detail {
DLL extern Level LogLevel;
DLL extern std::FILE* LogTarget;
}  // namespace detail

/** @brief Convert a string to a logging severity level.
    @param level Options are: error warning info debug trace.
    @return Corresponding Level or Level::None if \p level is invalid or empty.

    std::string_view is constructible via e.g. cast from std::string const&
    (std::basic_string::operator basic_string_view())
    or via std::basic_string_view(char const* cstr) etc.

 */
DLL auto level_from(std::string_view level) -> Level;

/// Convert a logging severity level to a string.
auto constexpr level_label(Level level) -> std::string_view
{
	switch (level) {
	case Level::Error: return "Error";
	case Level::Warning: return "Warning";
	case Level::Info: return "Info";
	case Level::Debug: return "Debug";
	case Level::Trace: return "Trace";
	default: return "None";
	}
}

/// Set the global log level. Ignore messages less-severe than \p level.
DLL void set_level(Level level);
DLL auto get_level() -> Level;

/// Set the log output target.
DLL void set_target(std::FILE* target);
DLL auto get_target() -> std::FILE*;

/// Print all active log levels given the current global log level.
DLL void print_enabled_levels();

#ifndef ENABLE_LOGGING
#define ENABLE_LOGGING 0
#endif

/// Emit a log message.
template <typename... Args>
void print(Level level, Args&&... args)
{
#if ENABLE_LOGGING
	if (detail::LogLevel >= level) {
		fmt::print(detail::LogTarget, "{}: {}\n", level_label(level), fmt::format(std::forward<Args>(args)...));
	}
#else
	(void) level;
	((void) args, ...);
#endif
}

/// Emit an error message.
template <typename... Args>
void error(Args&&... args)
{
#if ENABLE_LOGGING
	print(Level::Error, std::forward<Args>(args)...);
#else
	((void) args, ...);
#endif
}

/// Emit a warning message.
template <typename... Args>
void warning(Args&&... args)
{
#if ENABLE_LOGGING
	print(Level::Warning, std::forward<Args>(args)...);
#else
	((void) args, ...);
#endif
}

/// Emit an informational message.
template <typename... Args>
void info(Args&&... args)
{
#if ENABLE_LOGGING
	print(Level::Info, std::forward<Args>(args)...);
#else
	((void) args, ...);
#endif
}

/// Emit a debugging message.
template <typename... Args>
void debug(Args&&... args)
{
#if ENABLE_LOGGING
	print(Level::Debug, std::forward<Args>(args)...);
#else
	((void) args, ...);
#endif
}

/// Emit a trace message.
template <typename... Args>
void trace(Args&&... args)
{
#if ENABLE_LOGGING
	print(Level::Trace, std::forward<Args>(args)...);
#else
	((void) args, ...);
#endif
}

}  // namespace project::log

#endif
