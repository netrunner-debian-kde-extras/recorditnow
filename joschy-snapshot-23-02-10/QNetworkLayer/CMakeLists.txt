project(QNetworkLayer)                                              

cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/../cmake/modules)


if (WIN32 AND NOT BUID_WITH_JOSCHYCORE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-enable-auto-image-base -Wl,-enable-auto-import")
endif (WIN32 AND NOT BUID_WITH_JOSCHYCORE)


# find Qt4
if (NOT BUILD_WITH_JOSCHYCORE)
    find_package(Qt4 REQUIRED)
endif (NOT BUILD_WITH_JOSCHYCORE)
set(QT_DONT_USE_QTGUI true)

# find JoschyCore
if (NOT BUILD_WITH_JOSCHYCORE)
    find_package(JoschyCore REQUIRED)
endif(NOT BUILD_WITH_JOSCHYCORE)

# cflags
if (NOT WIN32)
    set(CMAKE_CXX_FLAGS                  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE          "-O3 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELWITHDEBUGINFO "-g -O3 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG            "-g -ggdb ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUGFULL        "-g3 -ffast-math ${CMAKE_CXX_FLAGS_DEBUG}")
endif(NOT WIN32)


# definitions
add_definitions(${QT_DEFINITIONS} -DJOSCHY_LIBRARY)


add_subdirectory(src)
