#!/bin/sh
workdir=$(cd $(dirname $0); pwd -P)
git submodule update --init --recursive

cd third-party && \
sh build.sh


if [[ $? -ne 0 ]]; then
    echo "ERROR: Failed to build"
    exit -1
fi