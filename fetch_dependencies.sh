#!/bin/sh
conan install . --install-folder build -s compiler.libcxx=libstdc++11 --build missing
