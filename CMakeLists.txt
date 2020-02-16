cmake_minimum_required(VERSION 3.10)

project(firework LANGUAGES CXX)

set(SOURCE_FILES
        src/firework.cpp
        src/fpoint.cpp
        src/main.cpp
        src/mTexture.cpp
        src/mWidget.cpp
)
set(HEADER_FILES
        src/firework.h
        src/fpoint.h
        src/mTexture.h
        src/mWidget.h
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall")
endif()

#g++ -std=c++0x -I $LIBS/libpng -I $LIBS/zlib -pthread -Wall -g $SRC -lpthread -lGL -lGLU -lX11 $LIBS/libpng/libpng16.a $LIBS/zlib/libz.a -o 1;

set(LIBS "./libs/lib")
set(LIBRARIES -lpthread
              -lGL
              -lGLU
              -lX11
              -lm
              libpng16.a
              libz.a
)
set(LIBS_HEADERS "./libs/include")

include_directories(${LIBS_HEADERS})
link_directories(${LIBS})
add_executable(firework ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(firework ${LIBRARIES})

