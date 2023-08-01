add_google_executable(test-sdl-integration TEST
	SOURCES
		application.cxx

	LIBRARIES
		project

		SDL2::SDL2

	MAIN ../sdl-main.cxx
)

add_local_all_target(all-integration)
