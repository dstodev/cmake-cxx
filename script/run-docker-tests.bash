#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && env pwd -P)"
source_dir="$script_dir/.."
docker_dir="$source_dir/docker"
build_dir="$source_dir/build/run-docker-tests-bash"
cache_dir="$source_dir/build/_dependency-cache"

image_name='build'

docker build --file="$docker_dir/build.dockerfile" "$docker_dir" --tag="$image_name"

mounts=(
	'--mount' "type=bind,readonly,src=$source_dir,dst=/project"
	'--mount' "type=bind,src=$build_dir,dst=/home/build"
	'--mount' "type=bind,src=$cache_dir,dst=/home/_dependency-cache"
)

mkdir -p "$build_dir"
docker run --rm "${mounts[@]}" "$image_name" sh -c "/project/script/run-tests.sh /home/build"
