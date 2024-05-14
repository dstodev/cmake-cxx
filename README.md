# Project

This project serves as an opinionated template for C/C++ developers.

Instead of small & concise, this project offers a comprehensive C/C++ development
environment to C++ developers, with the hope that it is faster to remove features
you do not need than it is to add features you may find useful yourself.
This is especially for those who are new to CMake or C/C++, who may wish to learn
by example.

It configures the build-system with CMake targets:

- Shared library `project` comprising main project code
- Unit tests `all-test`* for library testing
- Sample executable `app-console` which links to the library and offers basic command-line argument parsing
- Target `build-package`* creates a distributable package with the library, sample executable, and CMake exports
- Target `app-sample`* creates and runs the sample executable in the distribution package.  
  The sample is comprehensive: builds the package, includes the package CMake exports, compiles the sample source, &
  links to the shared library, all outside the context of the primary build system. See
  [sample.dio.svg](sample/sample.dio.svg) for more.

&ast; : This target runs by building it, and does not correlate to a file on disk.

It includes common libraries:

- [GoogleTest](https://github.com/google/googletest/) for testing
- [CMake Package Manager "CPM"](https://github.com/cpm-cmake/CPM.cmake) for dependency acquisition
- [cxxopts](https://github.com/jarro2783/cxxopts) for command-line argument parsing (used by sample app)

And plenty of CMake & C++ utilities; see [FEATURES.md](FEATURES.md) for details!

## Build dependencies

### All platforms

- [Git](https://git-scm.com/)
- [CMake](https://cmake.org/)
- [Docker](https://www.docker.com/) (to run Docker tests)

## Running tests

The following scripts have been tested on MacOS Monterey and Windows 11.

### Natively

- with **CMake**, run:  
  `cmake -S . -B build/manual; cmake --build build/manual --parallel --target all-test`

- on **Mac**, **Linux**, or Windows with [**WSL**](https://learn.microsoft.com/en-us/windows/wsl/), run:  
  `script/run-tests.sh`

### with Docker

- on **Mac** or **Linux**, run:  
  `script/run-docker-tests.bash`

- on **Windows**, run:  
  `script\run-docker-tests.bat`

- with [**Docker Compose**](https://docs.docker.com/compose/), run one of:  
  `docker compose -f docker/compose.yml run build`  
  `docker compose -f docker/compose.yml run build-clang`  
  `docker compose -f docker/compose.yml up`

## Build Package

- with **CMake**  
  `cmake -S . -B build/package; cmake --build build/package --parallel --target package`

> **NOTE:**  
> Building the package requires CPack, normally included with CMake.  
> If CMake is installed by an IDE, such as CLion, CPack may not be included.  
> Additionally, some package managers such as Chocolatey may include
> conflicting software also named `CPack`.

## Logging

The sample executable supports logging.

The main program additionally respects the `LOG_LEVEL` environment variable.
The value of `LOG_LEVEL` may be one of:

|   Level | Description               |
|--------:|:--------------------------|
|   ERROR | Errors only               |
| WARNING | Warnings and above        |
|    INFO | Status messages and above |
|   DEBUG | Debug messages and above  |
|   TRACE | Trace messages and above  |
|  (none) | No logging.               |

Release builds of the main application optimize out logging statements, so this
variable has no effect.

## Helper Commands

Open terminal in docker build environment  
`docker compose -f docker/compose.yml run shell`

Rebuild Docker image  
`docker compose -f docker/compose.yml build --no-cache build`

## Visual Studio Code integration

Open the project as a workspace using `project.code-workspace`.

Installing the extensions identified in `.vscode/extensions.json` will include,
among other helpful tools, the `ms-vscode.cmake-tools` extension. CMake Tools
integrates CMake into VS Code to provide code intellisense and other helpful
build utilities.

Notes:

- Make sure that the CMake Tools extension is configured to match the `CMAKE_BUILD_TYPE`.
- There are currently intellisense bugs on MacOS due to CMake Tools.  
  One such issue is being tracked [here](https://github.com/microsoft/vscode-cmake-tools/issues/1178).
- On Windows, make sure that you use the Ninja generator if you are able.  
  On other platforms, make sure that you use a Makefile generator. This is to
  [generate `compile_commands.json`](https://cmake.org/cmake/help/latest/variable/CMAKE_EXPORT_COMPILE_COMMANDS.html)
  for automatic intellisense.
- If CPM complains that a dependency's cache is dirty, you should delete the
  corresponding directory in `build/_dependency-cache/` and reconfigure.
  The Docker builds may complain anyway.
