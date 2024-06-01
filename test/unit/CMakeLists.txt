add_google_executable(unit-project
	SOURCES
		log.cxx
		project.cxx

	LIBRARIES
		project
)

add_local_all_target(all-unit)
