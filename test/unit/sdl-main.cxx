#include <gtest/gtest.h>

// A "main" function is required for SDL to link properly; will be redefined
// to "SDL_main()" by SDL_main.h

#include <SDL_log.h>

int main(int argc, char* argv[])
{
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_VERBOSE);
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
