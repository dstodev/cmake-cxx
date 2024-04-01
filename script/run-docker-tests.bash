#!/usr/bin/env bash
set -euo pipefail

script_dir="$(cd "$(dirname "$0")" && env pwd -P)"
source_dir="$script_dir/.."
docker_dir="$source_dir/docker"

compose=(docker compose --file="$docker_dir/compose.yml")
services=(
  'build'
  'build-clang'
)

runners=()

echo

for service in "${services[@]}"; do
  echo "Starting service: $service"
  "${compose[@]}" build "$service"
  runners+=("$("${compose[@]}" run --detach "$service")")
  echo
done

codes=""

# Calculate longest service name length (width)
service_width=0
for service in "${services[@]}"; do
  width_candidate="${#service}"

  if [ "$width_candidate" -gt "$service_width" ]; then
    service_width="$width_candidate"
  fi
done

log_paths=()

for i in "${!runners[@]}"; do
  run_id="${runners[i]}"
  run_service="${services[i]}"
  id_tag="$(printf -- '%.12s' "$run_id")"
  printf -- "Waiting for: %-${service_width}s (%s...) " "$run_service" "$id_tag"
  code="$(docker wait "$run_id")"

  log_file="$run_service.log"
  log_path="$script_dir/$log_file"
  docker logs "$run_id" >"$log_path"
  printf -- 'done. Log recorded: %s\n' "$log_file"
  log_paths+=("$log_path")

  docker rm "$run_id" >/dev/null
  codes+="$code"
done

echo

for log_path in "${log_paths[@]}"; do
  echo "Contents of log: $log_path"
  cat -n "$log_path"
  echo
done

# Look for non-zero exit codes. If any are found, exit with a non-zero code.
grep --quiet --invert-match '[^0]' <<<"$codes"
