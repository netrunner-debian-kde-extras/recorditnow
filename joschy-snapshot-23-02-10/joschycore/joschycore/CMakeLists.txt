
include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_BINARY_DIR} ${QT_INCLUDE_DIR})

set(SRCS
    manager.cpp
    plugin.cpp
    abstractnetworklayer.cpp
    abstractprovider.cpp
    video.cpp
    plugininfo.cpp
    pluginmanager.cpp
    postfile.cpp
    abstractresponseparser.cpp
    scheduler.cpp
    schedulerthread.cpp
    abstractjob.cpp
    config.cpp
    actionreply.cpp)

set(HEADERS 
    manager.h
    plugin.h
    abstractnetworklayer.h
    abstractprovider.h
    video.h
    joschy_global.h
    plugininfo.h
    postfile.h
    abstractresponseparser.h
    scheduler.h
    abstractjob.h
    joschy_version.h
    config.h
    actionreply.h)



QT4_AUTOMOC(${SRCS})
add_library(joschycore SHARED ${SRCS})
target_link_libraries(joschycore ${QT_LIBRARIES})
INSTALL(TARGETS joschycore DESTINATION ${JOSCHY_LIBRARY_INSTALL_DIR})
install(FILES ${HEADERS} DESTINATION ${JOSCHY_INCLUDE_INSTALL_DIR} COMPONENT Devel)

#add_subdirectory(qt)
#add_subdirectory(provider)
