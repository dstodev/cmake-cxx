# This code is modified from: (as of 5/17/2023)
# https://github.com/cpm-cmake/CPM.cmake/wiki/Downloading-CPM.cmake-in-CMake

function(_configure)
	set(output_root "${PROJECT_BINARY_DIR}/cmake")
	set(output_path ${output_root}/CPM.cmake)

	set(version "0.38.5")  # Versions: https://github.com/cpm-cmake/CPM.cmake/releases
	set(url "https://github.com/TheLartians/CPM.cmake/releases/download/v${version}/CPM.cmake")
	set(expected_hash "c98d14a13dfd1952e115979c095f6794")

	list(APPEND CMAKE_MODULE_PATH ${output_root})
	list(APPEND CMAKE_PREFIX_PATH ${output_root})

	if(NOT EXISTS ${output_path})
		message(STATUS "Downloading CPM.cmake to ${output_path}")
		file(DOWNLOAD ${url} ${output_path})
	endif()

	file(MD5 ${output_path} output_hash)
	string(JOIN "\n " msg " "
		"CPM.cmake hash mismatch! This is suspicious and could warrant further investigation."
		"To continue, update expected_hash in ${CMAKE_CURRENT_LIST_FILE}:0."
		""
		"Expected: ${expected_hash}"
		"Received: ${output_hash}"
		""
	)
	if(NOT "${output_hash}" STREQUAL "${expected_hash}")
		file(REMOVE ${output_path})
		message(FATAL_ERROR "${msg}")
	endif()

	include(${output_path})
endfunction()

include_guard(GLOBAL)
_configure()
