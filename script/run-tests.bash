#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && env pwd --physical)"
source_dir="$script_dir/.."

if [ -z "${1-}" ]; then
	build_dir="$source_dir/build/run-tests-bash"
else
	build_dir="$1"
fi

cmake -B "$build_dir" -S "$source_dir" -DCMAKE_BUILD_TYPE=Release
cmake --build "$build_dir" --target sample-app --parallel
