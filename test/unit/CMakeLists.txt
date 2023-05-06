find_package(AddGoogleExecutable CONFIG REQUIRED)
find_package(AddLocalAllTarget CONFIG REQUIRED)

find_package(Eigen3 REQUIRED)

add_google_executable(test_project TEST
	SOURCES
		assertions.hxx
		grid_t.cxx
		placeholder_t.cxx
		point_t.cxx
		shear.cxx

	LIBRARIES
		project
		Eigen3::Eigen
)

add_local_all_target(_all_unit)
