#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && env pwd -P)"
source_dir="$script_dir/.."
docker_dir="$source_dir/docker"

docker compose --file="$docker_dir/compose.yml" up
