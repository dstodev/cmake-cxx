include(GoogleTest)

find_package(HelpParseArguments REQUIRED)
find_package(GTest REQUIRED)

function(help_gtest_library target)
	help_parse_arguments(args
		""
		""
		"SOURCES;LIBRARIES"
	)

	add_executable(${target}
		${args_SOURCES}
	)
	target_link_libraries(${target}
		PRIVATE
			${args_LIBRARIES}
			GTest::gtest_main
	)
	target_compile_features(${target}
		PRIVATE
			cxx_std_11  # GoogleTest requires at least C++11 after version 1.8.1
			            # https://github.com/google/googletest/releases/tag/release-1.8.1
	)

	gtest_discover_tests(${target})
endfunction()
