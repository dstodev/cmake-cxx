# This code is modified from: https://github.com/cpm-cmake/CPM.cmake/wiki/Downloading-CPM.cmake-in-CMake (as of 5/17/2023)

set(output_root "${PROJECT_BINARY_DIR}/cmake")
set(output_path ${output_root}/CPM.cmake)

set(version "0.38.2")  # Versions: https://github.com/cpm-cmake/CPM.cmake/releases
set(url "https://github.com/TheLartians/CPM.cmake/releases/download/v${version}/CPM.cmake")
set(expected_hash "b00d27ba1825ffd9cca81bb994b8614f")

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
expect(output_hash STREQUAL expected_hash REQUIRED MESSAGE ${msg})
include(${output_path})
