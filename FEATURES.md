[(back)](README.md)

# Project Features

This project implements CMake and C++ utilities for developers starting new,
modern C++ projects.

## CMake Features

- Custom CMake testing framework [expect()](lib/cmake/module/Expect.cmake)
- Argument parsing helper [help_parse_arguments()](lib/cmake/module/HelpParseArguments.cmake)
- Logging utilities e.g. [log_vars()](lib/cmake/module/LogVars.cmake)
- Target utilities e.g. [directory_targets()](lib/cmake/module/DirectoryTargets.cmake)
- See `lib/cmake/module` for more!  
  All modules are included by the top-level `CMakeLists.txt` file.

## C++ Features

- [Basic logging](src/utility/log.hxx) using [fmt](https://github.com/fmtlib/fmt)
- [Testing](test/unit/project.cxx) with [GoogleTest](https://github.com/google/googletest)
