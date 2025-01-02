include(LineNumber)

# This code is modified from: (as of 5/17/2023)
# https://github.com/cpm-cmake/CPM.cmake/wiki/Downloading-CPM.cmake-in-CMake

function(_configure)
	set(output_root "${PROJECT_BINARY_DIR}/cmake")
	set(output_path "${output_root}/CPM.cmake")

	set(version "0.40.5")  # Versions: https://github.com/cpm-cmake/CPM.cmake/releases
	set(url "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${version}/CPM.cmake")
	set(expected_hash "19cbb284c7b175d239670d47dd9d0e9e")

	# If the file already exists, check its hash:
	# - If the hash is not expected, remove the file before re-downloading.
	# - This is useful when updating to a new version where the expected
	#   hash changes, but a previous version already exists on-disk.
	# If running on a clean cache, force re-download.
	if(EXISTS "${output_path}")
		file(MD5 "${output_path}" output_hash)

		if(NOT "${output_hash}" STREQUAL "${expected_hash}" OR NOT "${__cpm_configured}")
			message(STATUS "Removing ${output_path}")
			file(REMOVE "${output_path}")
		endif()
	endif()

	# Download CPM.cmake
	if(NOT EXISTS "${output_path}")
		message(STATUS "Downloading CPM.cmake to ${output_path}")
		file(DOWNLOAD "${url}" "${output_path}")
	endif()

	# Validate hash
	file(MD5 "${output_path}" output_hash)

	if(NOT "${output_hash}" STREQUAL "${expected_hash}")
		line_number(hash_line "${CMAKE_CURRENT_FUNCTION_LIST_FILE}" ${expected_hash})
		string(JOIN "\n " msg " "
			"CPM.cmake hash mismatch! This is suspicious and could warrant further investigation."
			"To continue, update expected_hash: ${CMAKE_CURRENT_LIST_FILE}:${hash_line}"
			""
			"Expected: ${expected_hash}"
			"Received: ${output_hash}"
			"")
		file(REMOVE "${output_path}")
		message(FATAL_ERROR "${msg}")
	endif()

	include("${output_path}")
endfunction()

include_guard(GLOBAL)

_configure()

set(__cpm_configured TRUE CACHE BOOL "Completed first-time CPM configuration?")
