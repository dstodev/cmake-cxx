[(back)](README.md)

# Project Features

This project implements CMake and C++ utilities for developers starting new
modern C++ projects.

## CMake Features

- Custom CMake testing framework [expect()](lib/cmake/config/ExpectConfig.cmake)
- Argument parsing helper [help_parse_arguments()](lib/cmake/config/HelpParseArgumentsConfig.cmake)
- Logging utilities e.g. [log_vars](lib/cmake/config/LogVarsConfig.cmake)
- Target utilities e.g. [directory_targets()](lib/cmake/config/TargetUtilitiesConfig.cmake)

## C++ Features

- [Basic logging](src/utility/log.hxx) using [fmt](https://github.com/fmtlib/fmt)
- [Testing](test/unit/shear.cxx) with [GoogleTest](https://github.com/google/googletest)
