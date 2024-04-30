#!/bin/sh
workdir=$(cd $(dirname $0); pwd)/../libpag

package=libpag
install_path=$workdir/../deploy/$package

############ build ${package} ############
echo "--- build ${package} ---"

cd "$workdir/" && \
sh sync_deps.sh && \
./build_pag -DPAG_BUILD_FRAMEWORK=OFF -DPAG_BUILD_SHARED=OFF -DPAG_USE_SWIFTSHADER=ON -o ./deploy

if [[ $? -ne 0 ]]; then
    echo "ERROR: Failed to build ${package}"
    exit -1
fi

echo "--- copy ${package} ---"

cp -r ./deploy/ $install_path && \
cp -r ./include $install_path && \
cp -r ./third_party/tgfx/vendor/swiftshader $install_path

if [[ $? -ne 0 ]]; then
    echo "ERROR: Failed to copy ${package}"
    exit -1
fi


echo "success!"