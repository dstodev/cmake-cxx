#!/bin/sh

script_dir="$(cd "$(dirname "$0")" && pwd -P)"
source_dir="$script_dir/.."

if [ -z "$1" ]; then
	build_dir="$source_dir/build"
else
	build_dir="$1"
fi

"$script_dir/fetch_dependencies.sh" "$build_dir" || exit $?

cmake -B "$build_dir" -S "$source_dir"
cmake --build "$build_dir" --target all test
