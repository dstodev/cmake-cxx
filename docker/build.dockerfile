FROM alpine:latest

RUN apk add --no-cache \
    build-base \
    clang \
    cmake \
    git
