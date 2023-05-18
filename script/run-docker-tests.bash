#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && env pwd --physical)"
source_dir="$script_dir/.."
docker_dir="$source_dir/docker"
build_dir="$source_dir/build/run-docker-tests-bash"
cache_dir="$source_dir/build/dependency-cache"

image_name='build:ubuntu'

docker build --file="$docker_dir/ubuntu.dockerfile" "$docker_dir" --tag="$image_name"

mounts=(
	'--mount' "type=bind,readonly,src=$source_dir,dst=/project"
	'--mount' "type=bind,src=$build_dir,dst=/home/build"
	'--mount' "type=bind,src=$cache_dir,dst=/home/dependency-cache"
)

mkdir --parents "$build_dir"
docker run --rm "${mounts[@]}" "$image_name" bash -c "/project/script/run-tests.bash /home/build"
