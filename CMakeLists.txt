cmake_minimum_required(VERSION 3.14)
project(EigenSample)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Using approach from:
# https://docs.conan.io/en/latest/integrations/build_system/cmake/cmake_find_package_generator.html#in-a-conanfile-txt
include(${PROJECT_BINARY_DIR}/conan_paths.cmake)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/cmake")

enable_testing()

add_subdirectory(src)
add_subdirectory(test)
