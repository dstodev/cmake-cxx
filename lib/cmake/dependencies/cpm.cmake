include(LineNumber)

# This code is modified from: (as of 5/17/2023)
# https://github.com/cpm-cmake/CPM.cmake/wiki/Downloading-CPM.cmake-in-CMake

function(_configure)
	set(output_root "${PROJECT_BINARY_DIR}/cmake")
	set(output_path ${output_root}/CPM.cmake)

	set(version "0.38.7")  # Versions: https://github.com/cpm-cmake/CPM.cmake/releases
	set(url "https://github.com/TheLartians/CPM.cmake/releases/download/v${version}/CPM.cmake")
	set(expected_hash "14ea07dfb484cad5db4ee1c75fd6a911")

	list(APPEND CMAKE_MODULE_PATH ${output_root})
	list(APPEND CMAKE_PREFIX_PATH ${output_root})

	if(NOT EXISTS ${output_path})
		message(STATUS "Downloading CPM.cmake to ${output_path}")
		file(DOWNLOAD ${url} ${output_path})
	endif()

	file(MD5 ${output_path} output_hash)
	if(NOT "${output_hash}" STREQUAL "${expected_hash}")
		line_number(hash_line ${CMAKE_CURRENT_FUNCTION_LIST_FILE} ${expected_hash})
		string(JOIN "\n " msg " "
			"CPM.cmake hash mismatch! This is suspicious and could warrant further investigation."
			"To continue, update expected_hash: ${CMAKE_CURRENT_LIST_FILE}:${hash_line}"
			""
			"Expected: ${expected_hash}"
			"Received: ${output_hash}"
			"")
		file(REMOVE ${output_path})
		message(FATAL_ERROR ${msg})
	endif()

	include(${output_path})
endfunction()

include_guard(GLOBAL)
_configure()
