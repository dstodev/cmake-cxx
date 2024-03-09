include_guard()

include(GoogleTest)

include(HelpParseArguments)

#[[
	add_google_executable(<name>
		[SOURCES <sources>...]
		[LIBRARIES <libraries>...]
		[STANDARD <standard>]
		[MAIN main.cxx]
	)

	Adds a new target <name> to the project with the given sources and which
	links to the given libraries.

	The standard is configurable by passing e.g. STANDARD 17
	or by setting CMAKE_CXX_STANDARD before calling this function.

	GoogleTest 1.14.x and later require C++14.

	An example main.cxx which runs GoogleTest tests is provided by Google
	in their documentation primer, copied here for convenience:

	// (2/16/2024)

		int main(int argc, char **argv) {
			testing::InitGoogleTest(&argc, argv);
			return RUN_ALL_TESTS();
		}

	// from: https://google.github.io/googletest/primer.html#writing-the-main-function
]]
function(add_google_executable target)
	help_parse_arguments(args "" "MAIN;STANDARD" "SOURCES;LIBRARIES")

	if(args_MAIN)
		list(APPEND google_links "gtest")
	else()
		list(APPEND google_links "gtest_main")
	endif()

	add_executable(${target} ${args_SOURCES} ${args_MAIN})
	target_link_libraries(${target} PRIVATE ${args_LIBRARIES} ${google_links})

	set(min_standard 14) # GoogleTest 1.14.x and later require C++14

	if(args_STANDARD)
		set(std ${args_STANDARD})
	elseif(CMAKE_CXX_STANDARD)
		set(std ${CMAKE_CXX_STANDARD})
	else()
		set(std ${min_standard})
	endif()

	set(known_versions "${CMAKE_CXX_COMPILE_FEATURES}")
	list(FILTER known_versions INCLUDE REGEX "cxx_std_")
	list(FILTER known_versions EXCLUDE REGEX "(_98|_11)$") # Remove versions less than min_standard
	list(JOIN known_versions " " known_version_str)

	if(std LESS min_standard)
		string(JOIN "\n " msg " "
			"C++ standard (${std}) is not supported by GoogleTest!"
			"GoogleTest requires C++${min_standard} or greater!"
			"Supported features: ${known_version_str}"
			"")
		message(FATAL_ERROR "${msg}")
	endif()

	target_compile_features(${target} PRIVATE cxx_std_${std})

	# TODO: Better properties support https://cmake.org/cmake/help/latest/command/set_tests_properties.html
	gtest_discover_tests(${target})
endfunction()
