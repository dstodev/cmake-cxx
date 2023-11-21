#include <cstdlib>

#include <log.hxx>

using namespace project;

int main(int argc, char* args[])
{
	if (char const* level = std::getenv("LOG_LEVEL")) {
		log::set_level(log::level_from(level));
	}

	log::error("Error messages enabled\n");
	log::warn("Warning messages enabled\n");
	log::info("Info messages enabled\n");
	log::debug("Debug messages enabled\n");
	log::trace("Trace messages enabled\n");

	return 0;
}
