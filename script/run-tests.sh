#!/usr/bin/env sh
set -eu

script_dir="$(cd "$(dirname "$0")" && pwd -P)"
source_dir="$script_dir/.."

if [ -z "${1-}" ]; then
  build_dir="$source_dir/build/run-tests-sh"
else
  build_dir="$1"
fi

cmake -S "$source_dir" -B "$build_dir" -DCMAKE_BUILD_TYPE=Release
cmake --build "$build_dir" --target all-test app-sample --parallel
