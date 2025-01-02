# Build files

This directory contains builds and files from various tools. They build in
different directories without interfering with each other:

```text
build
 ├ clion-debug
 ├ clion-default
 ├ clion-release
 ├ compose-build  
 ├ compose-build-clang  
 ├ manual
 ├ run-docker-tests-bash
 ├ run-docker-tests-bat
 ├ run-tests-sh
 └ vscode
```

The `_dependency-cache` directory contains dependency repositories managed by
[CPM](https://github.com/cpm-cmake/CPM.cmake)
