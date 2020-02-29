#!/bin/bash
################## utilities
function on_windows(){
    [[ "$(uname -s)" == *"MINGW"* ]] && echo "yes" || echo "no"
}

function on_linux(){
    [[ "$(uname -s)" == *"Linux"* ]] && echo "yes" || echo "no"
}

function on_macosx(){
    [[ "$(uname -s)" == *"Darwin"* ]] && echo "yes" || echo "no"
}

function get_cpu_cores_count(){
    cores=1
    if [[ "$(on_windows)" == "yes" ]]; then
        cores=4
    fi
    if [[ "$(on_linux)" == "yes" ]]; then
        cores=`nproc --all`
    fi
    if [[ "$(on_macosx)" == "yes" ]]; then
        cores=`sysctl -n hw.ncpu`
    fi
    echo ${cores}
}
ROOT_DIR=$(pwd)
############################
LIBS="${ROOT_DIR}/libs"
DEST="${ROOT_DIR}/build"

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

SOSUFFIX="so"
ASUFFIX="a"
if [[ "$(on_windows)" == "yes" ]]; then
    SOSUFFIX="dll"
    ASUFFIX="lib"
fi
if [[ "$(on_macosx)" == "yes" ]]; then
    SOSUFFIX="dylib"
fi

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
    [ -f ${LIBS}/lib/libz.${ASUFFIX} ] || {
        [ -d ${LIBZLIB}_build ] || { 
            mkdir -v ${LIBZLIB}_build
        }
        cmake -G "Unix Makefiles" -S ${LIBZLIB} -B ${LIBZLIB}_build -DCMAKE_INSTALL_PREFIX="${LIBS}" -DSKIP_INSTALL_FILES=1
        cmake --build ${LIBZLIB}_build --config Release --target install
        rm ${LIBS}/lib/*${SOSUFFIX}*
    }
}
[ -d $LIBPNG ] && {
    [ -f ${LIBS}/lib/libpng16.${ASUFFIX} ] || {
        [ -d ${LIBPNG}_build ] || { 
            mkdir -v ${LIBPNG}_build
        }
        cmake -G "Unix Makefiles" -S ${LIBPNG} -B ${LIBPNG}_build -DCMAKE_INSTALL_PREFIX="${LIBS}" -DSKIP_INSTALL_FILES=1 -DSKIP_INSTALL_PROGRAMS=1 -DPNG_SHARED=0 -DPNG_TESTS=0 -DPNG_BUILD_ZLIB=1 -DZLIB_INCLUDE_DIR="${LIBS}/include" -DZLIB_LIBRARY="${LIBS}/lib/libz.${ASUFFIX}"
        cmake --build ${LIBPNG}_build --config Release --target install
        rm -rf ${LIBS}/lib/libpng
        rm -rf ${LIBS}/bin
        rm -rf ${LIBS}/include/libpng16
        rm ${LIBS}/lib/libpng.${ASUFFIX}
    }
}
[ -d $LIBGLFW ] && {
    [ -f ${LIBS}/lib/libglfw3.${ASUFFIX} ] || {
        [ -d ${LIBGLFW}_build ] || { 
            mkdir -v ${LIBGLFW}_build
        }
        cmake -G "Unix Makefiles" -S ${LIBGLFW} -B ${LIBGLFW}_build -DCMAKE_INSTALL_PREFIX="${LIBS}" -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_DOCS=OFF
        cmake --build ${LIBGLFW}_build --config Release --target install 
    }
}
popd

PROJBUILD="$DEST/project"
[ -d ${PROJBUILD} ] || { 
    mkdir -pv ${PROJBUILD}
}

[ -f "${PROJBUILD}/firework" ] || {
    cmake -G "Unix Makefiles" -S ./ -B ${PROJBUILD}
    cmake --build ${PROJBUILD} --config Release
}

[ -f "${PROJBUILD}/firework" ] && {
    cp -v ${PROJBUILD}/firework ./firework
}

echo "Done!";
exit 0;
