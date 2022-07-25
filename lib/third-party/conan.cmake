# This code is lifted (and slightly modified) from: https://github.com/conan-io/cmake-conan (as of 5/23/2022)

set(conan_output_root ${PROJECT_BINARY_DIR})

list(APPEND CMAKE_MODULE_PATH ${conan_output_root})
list(APPEND CMAKE_PREFIX_PATH ${conan_output_root})

if (NOT EXISTS "${conan_output_root}/conan.cmake")
	message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
	file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.18.1/conan.cmake"
		"${conan_output_root}/conan.cmake"
		TLS_VERIFY ON)
endif()

include(${conan_output_root}/conan.cmake)
