include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_BINARY_DIR}
                    ${QT_INCLUDE_DIR}
                    ${JOSCHYCORE_INCLUDE_DIR})



set(QT_USE_QTNETWORK true)
set(QT_DONT_USE_QTGUI true)
include(${QT_USE_FILE})

set(Qt_SRCS
    qnetworklayer.cpp
    transferjob.cpp)


QT4_AUTOMOC(${Qt_SRCS})
add_library(joschyqnetworklayer SHARED ${Qt_SRCS})
target_link_libraries(joschyqnetworklayer ${JOSCHYCORE_LIBRARIES} ${QT_LIBRARIES})
INSTALL(TARGETS joschyqnetworklayer DESTINATION ${JOSCHYCORE_PLUGIN_INSTALL_DIR})
INSTALL(FILES qnetworklayer.plugin DESTINATION ${JOSCHYCORE_PLUGIN_INSTALL_DIR})
