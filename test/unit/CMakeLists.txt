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

	LIBRARIES
		project
)

add_local_all_target(all-unit)
