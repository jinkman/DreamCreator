#!/bin/sh
workdir=$(cd $(dirname $0); pwd -P)

build_type="Release"
if [[ x"$1" == x"Debug" ]]; then
    build_type="Debug"
fi

if [[ x"$1" == x"r" ]]; then
    rm -rf ${workdir}/build
    build_type="Debug"
fi

target=""

cd ${workdir} && \
mkdir -p ${workdir}/build && \
cmake -B ${workdir}/build -DCMAKE_BUILD_TYPE=${build_type} && \
cmake --build ${workdir}/build ${target} -- -j 12

if [[ $? -ne 0 ]]; then
    echo "ERROR: Failed to build"
    exit -1
fi