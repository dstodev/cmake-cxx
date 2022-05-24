#!/bin/sh

script_dir="$(cd "$(dirname "$0")" && pwd -P)"
source_dir="$script_dir/.."
docker_dir="$source_dir/docker"

image_name='build:ubuntu2004'

docker build --file="$docker_dir/ubuntu2004.dockerfile" "$docker_dir" --tag="$image_name"
docker run --mount type=bind,readonly,src="$source_dir",dst=/project "$image_name" sh /project/script/run_tests.sh
