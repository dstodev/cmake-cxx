# Project

This project serves as an opinionated template for C++ developers.

It includes common libraries:

- [GoogleTest](https://github.com/google/googletest/)
- [Google Benchmark](https://github.com/google/benchmark/)
- [CMake Package Manager "CPM"](https://github.com/cpm-cmake/CPM.cmake)
- See [dependencies.cmake](dependencies.cmake) for more!

And plenty of CMake & C++ utilities; see [FEATURES.md](FEATURES.md) for details!

## Build dependencies

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
  On other platforms, make sure that you use a Makefile generator.  
  This is to
  [generate `compile_commands.json`](https://cmake.org/cmake/help/latest/variable/CMAKE_EXPORT_COMPILE_COMMANDS.html)
  for automatic intellisense.
- If CPM complains that a dependency's cache is dirty, you should delete the
  corresponding directory in `build/_dependency-cache/` and reconfigure.
  The Docker builds may complain anyway.
