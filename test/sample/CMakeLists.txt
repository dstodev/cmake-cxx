cmake_minimum_required(VERSION 3.18)
project(SampleProject)

file(GLOB_RECURSE project_exports "${PROJECT_BINARY_DIR}/../project.cmake")
list(FILTER project_exports INCLUDE REGEX "ZIP")
list(LENGTH project_exports project_exports_length)
if(NOT project_exports_length EQUAL 1)
	message(FATAL_ERROR "Cannot locate project.cmake")
endif()

add_executable(main main.cxx)

get_filename_component(exports_dir "${project_exports}" DIRECTORY)
set(CMAKE_MSVCIDE_RUN_PATH "${exports_dir}/../bin")
add_custom_target(run main)

include(${project_exports})
target_link_libraries(main PRIVATE project::project)
