#!/bin/sh

script_dir="$(cd "$(dirname "$0")" && pwd)"
source_dir="$script_dir/.."

$script_dir/fetch_dependencies.sh $1 || exit $?

if [ -z "$1" ]; then
	build_dir="$source_dir/build"
else
	build_dir="$1"
fi

cmake -B $build_dir -S $source_dir
cmake --build $build_dir --target all test
