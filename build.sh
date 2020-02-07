#!/bin/bash
LIBS="../libs"
SRC="mTexture.cpp main.cpp firework.cpp fpoint.cpp mWidget.cpp"
g++ -std=c++0x -I $LIBS/libpng -I $LIBS/zlib -pthread -Wall -g $SRC -lpthread -lGL -lGLU -lX11 $LIBS/libpng/libpng16.a $LIBS/zlib/libz.a -o 1;
echo "Done!";
exit 0;
