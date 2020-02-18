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
LIBGLFW="glfw-3.3.2"
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
[ -f $LIBGLFW$TGZ ] || {
    curl -O https://codeload.github.com/glfw/glfw/tar.gz/3.3.2
    mv "3.3.2" $LIBGLFW$TGZ
    tar -xzf $LIBGLFW$TGZ
}

[ -d $LIBZLIB ] && {
    [ -f ../../libs/lib/libz.a ] || {
        [ -d ${LIBZLIB}_build ] || { 
            mkdir -v ${LIBZLIB}_build
        }
        cmake -S ${LIBZLIB} -B ${LIBZLIB}_build -DCMAKE_INSTALL_PREFIX="../../libs" -DSKIP_INSTALL_FILES=1
        cmake --build ${LIBZLIB}_build --config Release --target install
        rm ../../libs/lib/*so*
    }
}
[ -d $LIBPNG ] && {
    [ -f ../../libs/lib/libpng16.a ] || {
        [ -d ${LIBPNG}_build ] || { 
            mkdir -v ${LIBPNG}_build
        }
        cmake -S ${LIBPNG} -B ${LIBPNG}_build -DCMAKE_INSTALL_PREFIX="../../libs" -DSKIP_INSTALL_FILES=1 -DSKIP_INSTALL_PROGRAMS=1 -DPNG_SHARED=0 -DPNG_TESTS=0 -DPNG_BUILD_ZLIB=1 -DZLIB_INCLUDE_DIR="../../libs/include" -DZLIB_LIBRARY="../../libs/lib/libz.a"
        cmake --build ${LIBPNG}_build --config Release --target install
        rm -rf ../../libs/lib/libpng
        rm -rf ../../libs/bin
        rm -rf ../../libs/include/libpng16
        rm ../../libs/lib/libpng.a
    }
}

popd

PROJBUILD="$DEST/project"
[ -d ${PROJBUILD} ] || { 
    mkdir -pv ${PROJBUILD}
}

[ -f "${PROJBUILD}/firework" ] || {
    cmake -S ./ -B ${PROJBUILD}
    cmake --build ${PROJBUILD} --config Release
}

[ -f "${PROJBUILD}/firework" ] && {
    cp -v ${PROJBUILD}/firework ./firework
}

echo "Done!";
exit 0;
