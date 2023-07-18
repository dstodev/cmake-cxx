#include <cstdlib>

#include <SDL_main.h>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <application.hxx>
#include <log.hxx>

using namespace project;

int main(int argc, char* args[])
{
	Application app;

#ifdef _WIN32
	// If there is no console allocated, writing to stdout will crash the application.
	if (GetStdHandle(STD_OUTPUT_HANDLE) == nullptr) {
		log::set_level(log::Level::None);
	}
	else {
#endif
		if (char const* level = std::getenv("LOG_LEVEL")) {
			log::set_level(log::level_from(level));
		}
#ifdef _WIN32
	}
#endif

	log::error("Error messages enabled\n");
	log::warn("Warning messages enabled\n");
	log::info("Info messages enabled\n");
	log::debug("Debug messages enabled\n");
	log::trace("Trace messages enabled\n");

	int result = 0;

	try {
		app.init();
		result = app.app_main(argc, args);
	} catch (std::exception& e) {
		log::error("Application failed: {}\n", e.what());
	}

	log::info("Application closed with result: {}\n", result);

	return result;
}
