include(GoogleTest)

find_package(HelpParseArguments CONFIG REQUIRED)

function(add_google_executable target)
	help_parse_arguments(args
		"TEST;BENCHMARK"
		"MAIN"
		"SOURCES;LIBRARIES"
	)

	if (args_TEST)
		if (args_MAIN)
			list(APPEND google_links "gtest")
		else()
			list(APPEND google_links "gtest_main")
		endif()
	endif()

	if (args_BENCHMARK)
		if (args_MAIN)
			list(APPEND google_links "benchmark")
		else()
			list(APPEND google_links "benchmark_main")
		endif()
	endif()

	if (NOT google_links)
		message(FATAL_ERROR "At least one option must be provided: TEST or BENCHMARK")
	endif()

	add_executable(${target} EXCLUDE_FROM_ALL
		${args_SOURCES}
		${args_MAIN}
	)
	target_link_libraries(${target}
		PRIVATE
			${args_LIBRARIES}
			${google_links}
	)
	target_compile_features(${target}
		PRIVATE
			cxx_std_14  # GoogleTest requires at least C++14 as of version 1.13
	)

	if (args_TEST)
		# TODO: Better properties support https://cmake.org/cmake/help/latest/command/set_tests_properties.html
		gtest_discover_tests(${target})
	endif()
endfunction()
