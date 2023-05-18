@echo off

set script_dir=%~dp0
set source_dir=%script_dir%..
set docker_dir=%source_dir%\docker
set build_dir=%source_dir%\build\run-docker-tests-bat
set cache_dir=%source_dir%\build\dependency-cache

set image_name=build:ubuntu

docker build --file="%docker_dir%\ubuntu.dockerfile" "%docker_dir%" --tag="%image_name%"

if not exist "%build_dir%\NUL" mkdir "%build_dir%"

docker run --mount "type=bind,readonly,src=%source_dir%,dst=/project"^
           --mount "type=bind,src=%build_dir%,dst=/home/build"^
           --mount "type=bind,src=%cache_dir%,dst=/home/dependency-cache"^
           --rm "%image_name%" bash -c "/project/script/run-tests.bash /home/build"

:: TODO: Abstract with e.g. Python instead of these scripts?
