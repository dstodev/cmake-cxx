set(target unit-project)

find_package(Threads)

if(Threads_FOUND)
	message(DEBUG "Linking ${target} to Threads library")
	set(Threads_lib Threads::Threads)
endif()

add_google_executable(${target}
	SOURCES
		log.cxx
		project.cxx

	LIBRARIES
		project
		${Threads_lib}
)

add_local_all_target(all-unit)
