find_package(AddGoogleExecutable REQUIRED)

add_google_executable(test_shear TEST
	SOURCES
	test_shear.cxx

	LIBRARIES
	shear
	)
