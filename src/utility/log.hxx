#ifndef LOG_HXX
#define LOG_HXX

#include <utility>

#include <fmt/core.h>

#include <dll-export.h>

namespace project::log {

/** @brief Logging severity level.
@code
-- More severe --
 Level::Error
 Level::Warn
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
	Warn,
	Info,
	Debug,
	Trace,
};

namespace detail {
DLL extern Level LogLevel;
}  // namespace detail

/** @brief Convert a string to a logging severity level.
    @param level Options are: error warn info debug trace.
    @return Corresponding Level or Level::None if \p level is invalid or empty.
 */
DLL Level level_from(char const* level);

/// Ignore messages less-severe than \p level.
DLL void set_level(Level level);

DLL Level get_level();

/// Emit an error message.
template <typename... Args>
void constexpr error(fmt::format_string<Args...> const& format, Args&&... args)
{
#if ENABLE_LOGGING
	if (detail::LogLevel >= Level::Error) {
		fmt::print("Error: {}", fmt::format(format, std::forward<Args>(args)...));
	}
#endif
}

/// Emit a warning message.
template <typename... Args>
void constexpr warn(fmt::format_string<Args...> const& format, Args&&... args)
{
#if ENABLE_LOGGING
	if (detail::LogLevel >= Level::Warn) {
		fmt::print("Warn:  {}", fmt::format(format, std::forward<Args>(args)...));
	}
#endif
}

/// Emit an informational message.
template <typename... Args>
void constexpr info(fmt::format_string<Args...> const& format, Args&&... args)
{
#if ENABLE_LOGGING
	if (detail::LogLevel >= Level::Info) {
		fmt::print("Info:  {}", fmt::format(format, std::forward<Args>(args)...));
	}
#endif
}

/// Emit a debugging message.
template <typename... Args>
void constexpr debug(fmt::format_string<Args...> const& format, Args&&... args)
{
#if ENABLE_LOGGING
	if (detail::LogLevel >= Level::Debug) {
		fmt::print("Debug: {}", fmt::format(format, std::forward<Args>(args)...));
	}
#endif
}

/// Emit a trace message.
template <typename... Args>
void constexpr trace(fmt::format_string<Args...> const& format, Args&&... args)
{
#if ENABLE_LOGGING
	if (detail::LogLevel >= Level::Trace) {
		fmt::print("Trace: {}", fmt::format(format, std::forward<Args>(args)...));
	}
#endif
}

}  // namespace project::log

#endif
