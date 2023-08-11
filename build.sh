#!/bin/sh
workdir=$(cd $(dirname $0); pwd -P)
git submodule update --init --recursive

build_type="Release"
if [[ x"$1" == x"Debug" ]]; then
    build_type="Debug"
fi

target=""

cd ${workdir} && \
mkdir -p ./build && \
cmake -B ./build -DCMAKE_BUILD_TYPE=${build_type} && \
cmake --build ./build ${target} -- -j 12

if [[ $? -ne 0 ]]; then
    echo "ERROR: Failed to build"
    exit -1
fi