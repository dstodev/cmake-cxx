[(back)](README.md)

# Project Features

This project implements many CMake and C++ utilities as a starting point for
developers starting new modern C++ projects.

## CMake Features

- Custom CMake testing framework [expect()](lib/cmake/ExpectConfig.cmake)
- Argument parsing helper [help_parse_arguments()](lib/cmake/HelpParseArgumentsConfig.cmake)
- Logging utilities e.g. [log_vars](lib/cmake/LogVarsConfig.cmake)
- Target utilities e.g. [directory_targets()](lib/cmake/TargetUtilitiesConfig.cmake)

## C++ Features

- [Basic logging](src/utility/log.hxx) using [fmt](https://github.com/fmtlib/fmt)
- [Testing](test/unit/shear.cxx) with [GoogleTest](https://github.com/google/googletest)
- [Benchmarking](test/performance/shear.cxx) with [Google Benchmark](https://github.com/google/benchmark)
- Template data structures e.g. [Grid](src/utility/grid.hxx)
- Thread tools e.g. [ThreadPool](src/utility/thread-pool.hxx)
