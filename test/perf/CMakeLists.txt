find_package(AddGoogleExecutable CONFIG REQUIRED)

add_google_executable(performance BENCHMARK
	SOURCES
		shear.cxx

	LIBRARIES
		project
)
target_compile_features(performance
	PRIVATE
		cxx_std_17
)
