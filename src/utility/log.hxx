#ifndef LOG_HXX
#define LOG_HXX

#include <cstdio>
#include <string_view>
#include <utility>

#include <fmt/core.h>

#include <dll-export.h>

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
	None = 0,
	Error,
	Warning,
	Info,
	Debug,
	Trace,
};

namespace detail {
DLL extern Level LogLevel;
DLL extern FILE* LogTarget;
}  // namespace detail

/** @brief Convert a string to a logging severity level.
    @param level Options are: error warning info debug trace.
    @return Corresponding Level or Level::None if \p level is invalid or empty.

    std::string_view is constructible via e.g. cast from std::string const&
    (std::basic_string::operator basic_string_view())
    or via std::basic_string_view(char const* cstr) etc.

 */
DLL Level level_from(std::string_view level);

/// Ignore messages less-severe than \p level.
DLL void set_level(Level level);

DLL Level get_level();

DLL void set_target(FILE* target);

DLL FILE* get_target();

#ifndef ENABLE_LOGGING
#define ENABLE_LOGGING 0
#endif

/// Emit an error message.
template <typename... Args>
void constexpr error(fmt::format_string<Args...> const& format, Args&&... args)
{
#if ENABLE_LOGGING
	if (detail::LogLevel >= Level::Error) {
		fmt::print(detail::LogTarget, "Error: {}\n", fmt::format(format, std::forward<Args>(args)...));
	}
#else
	(void) format;
	((void) args, ...);
#endif
}

/// Emit a warning message.
template <typename... Args>
void constexpr warning(fmt::format_string<Args...> const& format, Args&&... args)
{
#if ENABLE_LOGGING
	if (detail::LogLevel >= Level::Warning) {
		fmt::print(detail::LogTarget, "Warning: {}\n", fmt::format(format, std::forward<Args>(args)...));
	}
#else
	(void) format;
	((void) args, ...);
#endif
}

template <typename... Args>
void constexpr warn(Args&&... args)
{
	warning(std::forward<Args>(args)...);
}

/// Emit an informational message.
template <typename... Args>
void constexpr info(fmt::format_string<Args...> const& format, Args&&... args)
{
#if ENABLE_LOGGING
	if (detail::LogLevel >= Level::Info) {
		fmt::print(detail::LogTarget, "Info: {}\n", fmt::format(format, std::forward<Args>(args)...));
	}
#else
	(void) format;
	((void) args, ...);
#endif
}

/// Emit a debugging message.
template <typename... Args>
void constexpr debug(fmt::format_string<Args...> const& format, Args&&... args)
{
#if ENABLE_LOGGING
	if (detail::LogLevel >= Level::Debug) {
		fmt::print(detail::LogTarget, "Debug: {}\n", fmt::format(format, std::forward<Args>(args)...));
	}
#else
	(void) format;
	((void) args, ...);
#endif
}

/// Emit a trace message.
template <typename... Args>
void constexpr trace(fmt::format_string<Args...> const& format, Args&&... args)
{
#if ENABLE_LOGGING
	if (detail::LogLevel >= Level::Trace) {
		fmt::print(detail::LogTarget, "Trace: {}\n", fmt::format(format, std::forward<Args>(args)...));
	}
#else
	(void) format;
	((void) args, ...);
#endif
}

}  // namespace project::log

#endif
