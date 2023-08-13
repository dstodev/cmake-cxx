add_google_executable(unit-project TEST
	SOURCES
		assertions.hxx
		entity.cxx
		grid_t.cxx
		log.cxx
		notify-assignment.cxx
		placeholder-t.cxx
		player.cxx
		point-t.cxx
		priority-mutex-t.cxx
		shear.cxx
		simulation.cxx
		thread-pool-t.cxx

	LIBRARIES
		project
)

add_google_executable(unit-sdl2 TEST
	SOURCES
		application.cxx
		texture.cxx

	LIBRARIES
		project

		SDL2::SDL2

	MAIN ../sdl-main.cxx
)

add_local_all_target(all-unit)
