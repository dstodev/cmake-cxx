find_package(AddGoogleExecutable CONFIG REQUIRED)

add_google_executable(test_shear TEST
	SOURCES
		test-shear.cxx

	LIBRARIES
		shear
)
