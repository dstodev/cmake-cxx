set(target unit-project)

find_package(Threads)

if(Threads_FOUND)
	message(DEBUG "Linking ${target} to Threads library")
	log_vars(CMAKE_THREAD_LIBS_INIT)
endif()

add_google_executable(${target}
	SOURCES
		log.cxx
		project.cxx

	LIBRARIES
		project
		${CMAKE_THREAD_LIBS_INIT}
)

add_local_all_target(all-unit)
