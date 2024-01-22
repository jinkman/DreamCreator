#!/bin/sh
workdir=$(cd $(dirname $0); pwd)/../libpag

package=libpag
install_path=$workdir/../deploy/$package

############ build ${package} ############
echo "--- build ${package} ---"

cd "$workdir/" && \
sh sync_deps.sh && \
cd linux && \
sh build_pag.sh

if [[ $? -ne 0 ]]; then
    echo "ERROR: Failed to build ${package}"
    exit -1
fi

echo "--- copy ${package} ---"

cd .. && \
cp -r ./linux/vendor/ $install_path

if [[ $? -ne 0 ]]; then
    echo "ERROR: Failed to copy ${package}"
    exit -1
fi


echo "success!"