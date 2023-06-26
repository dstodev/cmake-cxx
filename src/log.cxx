#include "log.hxx"

#include <algorithm>
#include <iterator>
#include <string>

#if ENABLE_LOGGING
SDL_LogOutputFunction static printer = nullptr;
void static* userdata = nullptr;

static void dummy(void*, int, SDL_LogPriority, char const*)
{}
#endif

namespace project::log {

Level level_from(char const* level)
{
	std::string level_lowercase;
	std::string_view it(level);
	std::transform(it.begin(), it.end(), std::back_inserter(level_lowercase), std::tolower);

	Level result = Level::None;

	if (level_lowercase == "trace") {
		result = Level::Trace;
	}
	else if (level_lowercase == "debug") {
		result = Level::Debug;
	}
	else if (level_lowercase == "info") {
		result = Level::Info;
	}
	else if (level_lowercase == "warn") {
		result = Level::Warn;
	}
	else if (level_lowercase == "error") {
		result = Level::Error;
	}

	return result;
}

void set_level(Level level)
{
#if ENABLE_LOGGING
	if (printer == nullptr) {
		SDL_LogGetOutputFunction(&printer, &userdata);
	}
	if (level == Level::None) {
		SDL_LogSetOutputFunction(dummy, nullptr);
	}
	else {
		SDL_LogSetOutputFunction(printer, userdata);
		SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, static_cast<SDL_LogPriority>(level));
	}
#endif
}

}  // namespace project::log
