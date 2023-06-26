#ifndef LOG_HXX
#define LOG_HXX

#include <SDL_log.h>

#include <project-api.h>

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
	Error = SDL_LOG_PRIORITY_CRITICAL,
	Warn = SDL_LOG_PRIORITY_WARN,
	Info = SDL_LOG_PRIORITY_INFO,
	Debug = SDL_LOG_PRIORITY_DEBUG,
	Trace = SDL_LOG_PRIORITY_VERBOSE,
};

/** @brief Convert a string to a logging severity level.
    @param level Options are: error warn info debug trace.
    @return Corresponding Level or Level::None if \p level is invalid or empty.
 */
PROJECT_API Level level_from(char const* level);

/// Ignore messages less-severe than \p level.
PROJECT_API void set_level(Level level);

/// Emit an error message.
template <typename... Args>
void constexpr error(char const* format, Args... args)
{
#if ENABLE_LOGGING
	SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, format, args...);
#endif
}

/// Emit an error message.
void constexpr error(char const* message)
{
	error("%s", message);
}

/// Emit a warning message.
template <typename... Args>
void constexpr warn(char const* format, Args... args)
{
#if ENABLE_LOGGING
	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, format, args...);
#endif
}

/// Emit a warning message.
void constexpr warn(char const* message)
{
	warn("%s", message);
}

/// Emit an informational message.
template <typename... Args>
void constexpr info(char const* format, Args... args)
{
#if ENABLE_LOGGING
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, format, args...);
#endif
}

/// Emit an informational message.
void constexpr info(char const* message)
{
	info("%s", message);
}

/// Emit a debugging message.
template <typename... Args>
void constexpr debug(char const* format, Args... args)
{
#if ENABLE_LOGGING
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, format, args...);
#endif
}

/// Emit a debugging message.
void constexpr debug(char const* message)
{
	debug("%s", message);
}

/// Emit a trace message.
template <typename... Args>
void constexpr trace(char const* format, Args... args)
{
#if ENABLE_LOGGING
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, format, args...);
#endif
}

/// Emit a trace message.
void constexpr trace(char const* message)
{
	trace("%s", message);
}

}  // namespace project::log

#endif
