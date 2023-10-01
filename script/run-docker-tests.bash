#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && env pwd -P)"
source_dir="$script_dir/.."
docker_dir="$source_dir/docker"

compose=(docker compose --file="$docker_dir/compose.yml")

services=$("${compose[@]}" config --services --quiet)
runners=()

for service in $services; do
  echo "Starting service: $service..."
  "${compose[@]}" build "$service"
  runners+=("$("${compose[@]}" run --detach "$service")")
done

codes=""

for runner in "${runners[@]}"; do
  printf -- 'Waiting for %.8s... ' "$runner"
  code=$(docker wait "$runner")
  printf -- 'done.\n'

  printf -- "----------------------------------------\n"
  docker logs "$runner"
  printf -- "----------------------------------------\n"

  docker rm "$runner" >/dev/null
  codes+="$code"
done

# Look for non-zero exit codes. If any are found, exit with a non-zero code.
grep --quiet --invert-match '[^0]' <<<"$codes"
