include(GoogleTest)

find_package(HelpParseArguments REQUIRED)

function(add_google_executable target)
	help_parse_arguments(args
		"TEST;BENCHMARK"
		""
		"SOURCES;LIBRARIES"
	)

	if (args_TEST)
		find_package(GTest REQUIRED)
		list(APPEND google_links "GTest::gtest_main")
	endif()

	if (args_BENCHMARK)
		find_package(benchmark REQUIRED)
		list(APPEND google_links "benchmark::benchmark")
	endif()

	if (NOT google_links)
		message(FATAL_ERROR "At least one option must be provided: TEST or BENCHMARK")
	endif()

	add_executable(${target}
		${args_SOURCES}
	)
	target_link_libraries(${target}
		PRIVATE
			${args_LIBRARIES}
			${google_links}
	)
	target_compile_features(${target}
		PRIVATE
			cxx_std_11  # GoogleTest requires at least C++11 after version 1.8.1
			            # https://github.com/google/googletest/releases/tag/release-1.8.1
	)
	target_compile_definitions(${target}
		PRIVATE
			_GLIBCXX_USE_CXX11_ABI=0  # For compatibility with default Conan settings; see:
			                          # https://docs.conan.io/en/latest/howtos/manage_gcc_abi.html
			                          # https://gcc.gnu.org/onlinedocs/libstdc++/manual/using_dual_abi.html
	)

	if (args_TEST)
		gtest_discover_tests(${target})
	endif()
endfunction()
