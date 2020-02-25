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

if (WIN32)
    set(ICON rc/firework.rc)
elseif (APPLE)
    set(ICON rc/firework.icns)
endif()

set(LIBSUFFIX "a")

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++11")
    set(LIBSUFFIX "lib")
else()
	set(CMAKE_CXX_STANDARD 11)
endif()

#g++ -std=c++0x -I $LIBS/libpng -I $LIBS/zlib -pthread -Wall -g $SRC -lpthread -lGL -lGLU -lX11 $LIBS/libpng/libpng16.a $LIBS/zlib/libz.a -o 1;
find_package(OpenGL REQUIRED)

set(LIBS "./libs/lib")
set(LIBRARIES -lpthread
              -lm
	      libpng16.${LIBSUFFIX}
              libz.${LIBSUFFIX}
	      OpenGL::GL
	      libglfw3.${LIBSUFFIX}
)
set(LIBS_HEADERS "./libs/include")

if(APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "-framework Cocoa -framework OpenGL -framework IOKit")
endif(APPLE)

include_directories(${LIBS_HEADERS})
link_directories(${LIBS})
add_executable(firework WIN32 MACOSX_BUNDLE ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(firework ${LIBRARIES})

if (APPLE)
	set_target_properties(firework PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "Firework")

    set_source_files_properties(rc/firework.icns PROPERTIES
                                MACOSX_PACKAGE_LOCATION "Resources")
    set_target_properties(firework PROPERTIES
                          MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0"
                          MACOSX_BUNDLE_LONG_VERSION_STRING "1.0.0"
                          MACOSX_BUNDLE_ICON_FILE rc/firework.icns
                          MACOSX_BUNDLE_INFO_PLIST "src/MacOSXBundleInfo.plist.in")
endif()
