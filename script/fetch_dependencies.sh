#!/bin/sh

script_dir="$(cd "$(dirname "$0")" && pwd -P)"
source_dir="$script_dir/.."

if [ -z "$1" ]; then
	build_dir="$source_dir/build"
else
	build_dir="$1"
fi

conan install "$source_dir" --install-folder="$build_dir" --build=missing
r=$?

if [ $r -ne 0 ]; then
	echo "Conan failed to install dependencies with error code $r"
	exit $r
fi
