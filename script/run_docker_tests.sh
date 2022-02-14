#!/bin/sh

script_dir="$(cd "$(dirname "$0")" && pwd)"
source_dir="$script_dir/.."
docker_dir="$source_dir/docker"

image_name='build:ubuntu2004'

docker build --file "$docker_dir/ubuntu2004.dockerfile" "$docker_dir" --tag "$image_name"
docker run --rm --volume "$source_dir:/temp:ro" "$image_name" '/temp/script/run_tests.sh' '/build'
