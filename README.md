# Project

This project serves as an opinionated template for C++ developers.
It includes:

- [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) \([repo](https://gitlab.com/libeigen/eigen)\)
- [SDL2](https://www.libsdl.org/) \([repo](https://github.com/libsdl-org/SDL)\)
- [GoogleTest](https://github.com/google/googletest/)
- [Google Benchmark](https://github.com/google/benchmark/)
- [CMake Package Manager "CPM"](https://github.com/cpm-cmake/CPM.cmake)

## Build dependencies

- [CMake](https://cmake.org/)
- [Docker](https://www.docker.com/) (to run Docker tests)

## Running tests

The following scripts have been tested on MacOS Monterey and Windows 11.

- Natively
    - on Mac, Linux, or Windows with WSL  
      Run `script/run-tests.sh`

- through Docker
    - on Mac/Linux  
      Run `script/run-docker-tests.bash`

    - on Windows  
      Run `script\run-docker-tests.bat`

    - with [Compose](https://docs.docker.com/compose/)  
      Run `docker compose -f docker/compose.yaml run build`, or  
      Run `docker compose -f docker/compose.yaml run build-clang`, or  
      Run `docker compose -f docker/compose.yaml up`

## Build Package

- with CMake  
  `cmake -B build/package && cmake --build build/package --target package --parallel`

## Helper Commands

Open terminal in docker build environment  
`docker compose -f docker/compose.yaml run shell`

Rebuild Docker image  
`docker compose -f docker/compose.yaml build --no-cache build`

## Visual Studio Code integration

Open the project as a workspace using `project.code-workspace`.

Installing the extensions identified in `.vscode/extensions.json` will include,
among other helpful tools, the `ms-vscode.cmake-tools` extension. CMake Tools
integrates CMake into VS Code to provide code intellisense and other helpful
build utilities.

Notes:

- Make sure that the CMake Tools extension is configured to match the
  `CMAKE_BUILD_TYPE`.
- There are currently intellisense bugs on MacOS due to CMake Tools.  
  One such issue is being tracked [here](https://github.com/microsoft/vscode-cmake-tools/issues/1178).
- On Windows, make sure that you use the Ninja generator if you are able.  
  On other platforms, make sure that you use a Makefile generator.  
  This is to [generate `compile_commands.json`](https://cmake.org/cmake/help/latest/variable/CMAKE_EXPORT_COMPILE_COMMANDS.html)
  for automatic intellisense.
- If CPM complains that a dependency's cache is dirty, you should delete the
  corresponding directory in `build/_dependency-cache/` and reconfigure.
  The Docker builds may complain anyway.
