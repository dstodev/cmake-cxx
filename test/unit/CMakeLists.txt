set(target unit-project)

find_package(Threads)

if(Threads_FOUND)
	message(DEBUG "Linking ${target} to Threads library")
	set(threads Threads::Threads)
endif()

add_google_executable(${target}
	SOURCES
		log.cxx
		project.cxx

	LIBRARIES
		project
		${threads}
)

add_local_all_target(all-unit)
