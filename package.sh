#!/bin/sh
cd ./build/bin
echo "install ..."

/opt/homebrew/Cellar/qt@5/5.15.13_1/bin/macdeployqt DreamCreator.app
RETURN_CODE=$?
if [ "$RETURN_CODE" -eq "0" ]
then
    echo "install success"
else
    echo "install faild"
    exit 1
fi

install_name_tool -add_rpath "@executable_path/../Frameworks" DreamCreator.app/Contents/MacOS/DreamCreator
# 签名
codesign --deep --force --verify --verbose --sign "beyond-today" DreamCreator.app