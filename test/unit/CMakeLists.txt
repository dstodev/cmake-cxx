find_package(AddGoogleExecutable CONFIG REQUIRED)
find_package(AddLocalAllTarget CONFIG REQUIRED)

add_google_executable(test_project TEST
	SOURCES
		assertions.hxx
		test-image-utilities.cxx
		test-image.cxx
		test-placeholder.cxx
		test-shear.cxx

	LIBRARIES
		project
)
target_compile_features(test_project
	PRIVATE
		cxx_std_17
)

add_local_all_target(all_unit)
