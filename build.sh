#!/bin/bash
LIBS="./libs"
DEST="./build"

[ -d $LIBS ] || {
    mkdir -pv $LIBS
    mkdir -pv $LIBS/include
    mkdir -pv $LIBS/lib
}
[ -d $DEST ] || {
    mkdir -pv $DEST
}
[ -z $(which curl) ] && {
    echo "curl not found, please install it to continue..."
    exit 1
}

LIBPNG="libpng-1.6.37"
LIBZLIB="zlib-1.2.11"
LIBSBUILD="$DEST/libs"
[ -d $LIBSBUILD ] || { 
    mkdir -pv $LIBSBUILD
}

TGZ=".tar.gz"
pushd $LIBSBUILD
[ -f $LIBPNG$TGZ ] || {
    curl -O https://netcologne.dl.sourceforge.net/project/libpng/libpng16/1.6.37/$LIBPNG$TGZ
    tar -xzf $LIBPNG$TGZ
}
[ -f $LIBZLIB$TGZ ] || {
    curl -O http://www.zlib.net/$LIBZLIB$TGZ
    tar -xzf $LIBZLIB$TGZ
}

[ -d $LIBZLIB ] && {
    mkdir -v ${LIBZLIB}_build
    cmake -S ${LIBZLIB} -B ${LIBZLIB}_build -DCMAKE_INSTALL_PREFIX="../../libs" -DSKIP_INSTALL_FILES=1
    cmake --build ${LIBZLIB}_build --config Release --target install
    rm ../../libs/lib/*so*
}
[ -d $LIBPNG ] && {
    echo "TODO: compile $LIBPNG"
}

popd

SRC="mTexture.cpp main.cpp firework.cpp fpoint.cpp mWidget.cpp"
#g++ -std=c++0x -I $LIBS/libpng -I $LIBS/zlib -pthread -Wall -g $SRC -lpthread -lGL -lGLU -lX11 $LIBS/libpng/libpng16.a $LIBS/zlib/libz.a -o 1;
echo "Done!";
exit 0;
