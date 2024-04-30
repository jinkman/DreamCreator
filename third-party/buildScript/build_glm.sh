#!/bin/sh
workdir=$(cd $(dirname $0); pwd)/../glm

package=glm

############ build ${package} ############
echo "--- build ${package} ---"

cd "$workdir/" && \
mkdir -p "$workdir/build" && cd "$workdir/build/" && \
cmake -DCMAKE_CXX_STANDARD=17 -DGLM_TEST_ENABLE=ON -DCMAKE_INSTALL_PREFIX="$workdir/../deploy/$package" ../ && \
cmake --build ./ && \
cmake --install ./

if [[ $? -ne 0 ]]; then
    echo "ERROR: Failed to build ${package}"
    exit -1
fi

echo "success!"