#ifndef LOG_HXX
#define LOG_HXX

#include <SDL_log.h>

namespace project::log {

template <typename... Args>
inline void error(char const* format, Args... args)
{
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, format, args...);
}

inline void error(char const* message)
{
	error("%s", message);
}

template <typename... Args>
inline void warn(char const* format, Args... args)
{
	SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, format, args...);
}

inline void warn(char const* message)
{
	warn("%s", message);
}

template <typename... Args>
inline void info(char const* format, Args... args)
{
	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, format, args...);
}

inline void info(char const* message)
{
	info("%s", message);
}

template <typename... Args>
inline void debug(char const* format, Args... args)
{
	SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, format, args...);
}

inline void debug(char const* message)
{
	debug("%s", message);
}

template <typename... Args>
inline void trace(char const* format, Args... args)
{
	SDL_LogVerbose(SDL_LOG_CATEGORY_APPLICATION, format, args...);
}

inline void trace(char const* message)
{
	trace("%s", message);
}

}  // namespace project::log

#endif
