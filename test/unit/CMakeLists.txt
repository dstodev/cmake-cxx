add_google_executable(unit-project TEST
	SOURCES
		assertions.hxx
		entity.cxx
		grid.cxx
		log.cxx
		notify-assignment.cxx
		placeholder.cxx
		player.cxx
		point.cxx
		priority-mutex.cxx
		shear.cxx
		simulation.cxx
		thread-deque.cxx
		thread-pool.cxx

	LIBRARIES
		project
)

if(TEST_GUI)
	add_google_executable(unit-sdl2 TEST
		SOURCES
			application.cxx
			texture.cxx

		LIBRARIES
			project
			SDL2::SDL2

		MAIN ../sdl-main.cxx
	)
endif()

add_local_all_target(all-unit)
