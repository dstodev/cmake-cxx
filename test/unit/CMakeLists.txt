find_package(AddGoogleExecutable CONFIG REQUIRED)
find_package(AddLocalAllTarget CONFIG REQUIRED)

add_google_executable(test_project TEST
	SOURCES
		test-image.cxx
		test-image-utilities.cxx
		test-shear.cxx

	LIBRARIES
		project
)

add_local_all_target(all_unit)
