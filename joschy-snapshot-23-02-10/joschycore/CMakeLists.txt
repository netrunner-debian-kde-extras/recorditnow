project(joschycore)

cmake_minimum_required(VERSION 2.6)


if (WIN32 AND NOT BUID_WITH_JOSCHYCORE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-enable-auto-image-base -Wl,-enable-auto-import")
endif (WIN32 AND NOT BUID_WITH_JOSCHYCORE)


# find Qt4
if (NOT BUILD_WITH_JOSCHYCORE)
    find_package(Qt4 REQUIRED)
endif (NOT BUILD_WITH_JOSCHYCORE)

set(QT_DONT_USE_QTGUI true)
include(${QT_USE_FILE})

# 64 bit?
if (NOT WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 4)
        SET(LIB_SUFFIX "")
    else(CMAKE_SIZEOF_VOID_P EQUAL 4)
        SET(LIB_SUFFIX 64)
    endif(CMAKE_SIZEOF_VOID_P EQUAL 4)
endif(NOT WIN32)

# cflags
if (NOT WIN32)
    set(CMAKE_CXX_FLAGS                  "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE          "-O3 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELWITHDEBUGINFO "-g -O3 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG            "-g -ggdb ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUGFULL        "-g3 -ffast-math ${CMAKE_CXX_FLAGS_DEBUG}")
endif(NOT WIN32)

# paths
set(JOSCHY_LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX} CACHE FILEPATH "Install path for libs")
set(JOSCHY_PLUGIN_INSTALL_DIR ${JOSCHY_LIBRARY_INSTALL_DIR}/joschy CACHE FILEPATH "Install path for plugins")
set(JOSCHY_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/joschycore)

# definitions
add_definitions(${QT_DEFINITIONS} -DJOSCHY_LIBRARY -DJOSCHY_PLUGINDIR="${JOSCHY_PLUGIN_INSTALL_DIR}")


# src
add_subdirectory(joschycore)


message(STATUS "----------------------")
message(STATUS "LIBRARY_INSTALL_DIR=" ${JOSCHY_LIBRARY_INSTALL_DIR})
message(STATUS "PLUGIN_INSTALL_DIR=" ${JOSCHY_PLUGIN_INSTALL_DIR})
message(STATUS "INCLUDE_INSTALL_DIR=" ${JOSCHY_INCLUDE_INSTALL_DIR})
message(STATUS "----------------------")
