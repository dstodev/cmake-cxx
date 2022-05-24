cmake_minimum_required(VERSION 3.16)
project(EigenSample)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

list(APPEND CMAKE_PREFIX_PATH  "${CMAKE_CURRENT_LIST_DIR}/lib/cmake")

include(CPack)
include(CTest)

# Using conan integration approach from: https://github.com/conan-io/cmake-conan
include(lib/third_party/conan.cmake)

conan_cmake_autodetect(conan_settings)  # https://github.com/conan-io/cmake-conan#conan_cmake_autodetect
conan_cmake_install(PATH_OR_REFERENCE "${CMAKE_CURRENT_LIST_DIR}/conanfile.txt"  # https://github.com/conan-io/cmake-conan#conan_cmake_install
    BUILD missing
    REMOTE conancenter
    SETTINGS ${conan_settings}
)

add_subdirectory(src)
add_subdirectory(test)
