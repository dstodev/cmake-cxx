add_google_executable(test-project TEST
	SOURCES
		assertions.hxx
		entity.cxx
		grid_t.cxx
		log.cxx
		notify-assignment.cxx
		placeholder_t.cxx
		player.cxx
		point_t.cxx
		shear.cxx
		simulation.cxx
		texture.cxx

	LIBRARIES
		project

		SDL2::SDL2

	MAIN ../sdl-main.cxx
)

add_local_all_target(all-unit)
