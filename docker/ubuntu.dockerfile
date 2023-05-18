FROM ubuntu:latest

RUN apt update && apt install -y \
	build-essential \
	clang \
	git \
	python3-pip

RUN pip3 install \
	cmake
