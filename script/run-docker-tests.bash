#!/bin/bash

script_dir="$(cd "$(dirname "$0")" && pwd -P)"
source_dir="$script_dir/.."
docker_dir="$source_dir/docker"
build_dir="$source_dir/build"

image_name='build:ubuntu2004'

docker build --file="$docker_dir/ubuntu2004.dockerfile" "$docker_dir" --tag="$image_name"

mounts=('--mount' "type=bind,readonly,src=$source_dir,dst=/project"
        '--mount' "type=bind,src=$build_dir,dst=/build")

mkdir -p "$build_dir"
docker run "${mounts[@]}" "$image_name" sh /project/script/run-tests.sh /build
