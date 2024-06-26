#!/bin/sh
workdir=$(cd $(dirname $0); pwd)/../opencv

package=opencv

############ build ${package} ############
echo "--- build ${package} ---"

cd "$workdir/" && \
mkdir -p "$workdir/build" && cd "$workdir/build/" && \
cmake -DCMAKE_CXX_STANDARD=17 -DCMAKE_BUILD_TYPE=Release -DBUILD_ZLIB=OFF -DENABLE_PRECOMPILED_HEADERS=OFF -DWITH_FFMPEG=OFF -DBUILD_PERF_TESTS=OFF -DBUILD_TESTS=OFF -DBUILD_opencv_python3=OFF -DBUILD_opencv_apps=OFF -DCMAKE_INSTALL_PREFIX="$workdir/../deploy/$package" ../ && \
cmake --build ./ && \
cmake --install ./

if [[ $? -ne 0 ]]; then
    echo "ERROR: Failed to build ${package}"
    exit -1
fi

echo "success!"
