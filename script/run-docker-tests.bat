@echo off

set script_dir=%~dp0
set source_dir=%script_dir%..
set docker_dir=%source_dir%\docker
set build_dir=%source_dir%\build\run-docker-tests-bat
set cache_dir=%source_dir%\build\_dependency-cache

set image_name=build

docker build --file="%docker_dir%\build.dockerfile" "%docker_dir%" --tag="%image_name%"

if not exist "%build_dir%\NUL" mkdir "%build_dir%"

docker run --mount "type=bind,readonly,src=%source_dir%,dst=/project"^
           --mount "type=bind,src=%build_dir%,dst=/home/build"^
           --mount "type=bind,src=%cache_dir%,dst=/home/_dependency-cache"^
           --rm "%image_name%" sh -c "/project/script/run-tests.sh /home/build"

:: TODO: Abstract with e.g. Python instead of these scripts?
