# Project

This project serves as an opinionated template project which includes:
- [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page) \([repo](https://gitlab.com/libeigen/eigen)\)
- [GoogleTest](https://github.com/google/googletest)
- [Google Benchmark](https://github.com/google/benchmark)
- [CMake Package Manager "CPM"](https://github.com/cpm-cmake/CPM.cmake)

## Build dependencies

- [CMake 3.14+](https://cmake.org/)
- [Docker Desktop](https://www.docker.com/products/docker-desktop) (to run Docker tests)

## Running tests

The following scripts have been tested on MacOS Monterey and Windows 10.

- Natively
  - on Mac, Linux, or Windows with WSL  
    Run `script/run-tests.sh`

- through Docker
  - on Mac/Linux  
    Run `script/run-docker-tests.bash`

  - on Windows  
    Run `script\run-docker-tests.bat`
  
  - with [Compose](https://docs.docker.com/compose/)  
    Run `docker compose -f docker/compose.yaml run ubuntu`, or  
    Run `docker compose -f docker/compose.yaml run ubuntu-clang`

### Helpers

Open terminal in docker build environment  
`docker compose -f docker/compose.yaml run --entrypoint=/bin/bash ubuntu`

Rebuild Docker image  
`docker compose -f docker/compose.yaml build ubuntu`

# Visual Studio Code integration

Open the project as a workspace using `project.code-workspace`.

Installing the extensions identified in `.vscode/extensions.json` will include, among other helpful
tools, the `ms-vscode.cmake-tools` extension. CMake Tools integrates CMake into VS Code to provide
code intellisense and other helpful build utilities.

Notes:  
- Make sure that the CMake Tools extension is configured to match the CMAKE_BUILD_TYPE.
- There are currently intellisense bugs on MacOS due to CMake Tools. One such issue is being tracked
  [here](https://github.com/microsoft/vscode-cmake-tools/issues/1178).
- On Windows make sure that you use the Ninja generator if you are able.  
  On other platforms make sure that you use a Makefile generator.  
  This is to [generate `build/vscode/compile_commands.json`](https://cmake.org/cmake/help/latest/variable/CMAKE_EXPORT_COMPILE_COMMANDS.html)
  for automatic intellisense.
