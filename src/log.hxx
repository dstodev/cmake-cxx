#ifndef LOG_HXX
#define LOG_HXX

#include <SDL_log.h>

namespace project::log {

template <typename... Args>
void constexpr error(char const* format, Args... args)
{
#if ENABLE_LOGGING
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, format, args...);
#endif
}

void constexpr error(char const* message)
{
	error("%s", message);
}

template <typename... Args>
void constexpr warn(char const* format, Args... args)
{
#if ENABLE_LOGGING
	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, format, args...);
#endif
}

void constexpr warn(char const* message)
{
	warn("%s", message);
}

template <typename... Args>
void constexpr info(char const* format, Args... args)
{
#if ENABLE_LOGGING
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, format, args...);
#endif
}

void constexpr info(char const* message)
{
	info("%s", message);
}

template <typename... Args>
void constexpr debug(char const* format, Args... args)
{
#if ENABLE_LOGGING
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, format, args...);
#endif
}

void constexpr debug(char const* message)
{
	debug("%s", message);
}

template <typename... Args>
void constexpr trace(char const* format, Args... args)
{
#if ENABLE_LOGGING
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, format, args...);
#endif
}

void constexpr trace(char const* message)
{
	trace("%s", message);
}

}  // namespace project::log

#endif
