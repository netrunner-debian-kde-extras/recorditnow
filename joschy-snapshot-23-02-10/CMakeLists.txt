project(Joschy)

cmake_minimum_required(VERSION 2.6)


# vars
set(BUILD_BLIP true)
set(BUILD_YOUTUBE true)
set(BUILD_QNETWORKLAYER true)
set(BUILD_KNETWORKLAYER false)

if (WIN32)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-enable-auto-image-base -Wl,-enable-auto-import")
endif (WIN32)


# find Qt4
find_package(Qt4 REQUIRED)

# definitions
add_definitions(${QT_DEFINITIONS} -DJOSCHY_LIBRARY)

# build with core
set(BUILD_WITH_JOSCHYCORE true)

# core
add_subdirectory(joschycore)

set(JOSCHYCORE_PLUGIN_INSTALL_DIR ${JOSCHY_PLUGIN_INSTALL_DIR})
set(JOSCHYCORE_LIBRARIES joschycore)
set(JOSCHYCORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/joschycore/)

if (BUILD_KNETWORKLAYER)
    find_package(KDE4)
endif (BUILD_KNETWORKLAYER)


if (BUILD_QNETWORKLAYER)
    add_subdirectory(QNetworkLayer)
endif (BUILD_QNETWORKLAYER)

if (KDE4_FOUND)
    message(STATUS "KNetworkLayer needs an update...")
    #add_subdirectory(KNetworkLayer)
endif(KDE4_FOUND)

add_subdirectory(Provider)

