set(target unit-project)

add_google_executable(${target}
	SOURCES
		log.cxx
		project.cxx

	LIBRARIES
		project
)

add_local_all_target(all-unit)
