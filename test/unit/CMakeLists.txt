find_package(AddGoogleExecutable CONFIG REQUIRED)
find_package(AddLocalAllTarget CONFIG REQUIRED)

add_google_executable(test-project TEST
	SOURCES
		assertions.hxx
		grid_t.cxx
		placeholder_t.cxx
		point_t.cxx
		shear.cxx

	LIBRARIES
		project
)

add_local_all_target(_all-unit)
