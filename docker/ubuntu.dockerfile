FROM ubuntu:22.04

RUN apt update && apt install -y \
	build-essential \
    clang \
	python3-pip

RUN pip3 install \
		cmake
