include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_BINARY_DIR}
                    ${QT_INCLUDE_DIR}
                    ${JOSCHYCORE_INCLUDE_DIR})


set(QT_USE_QTXML true)
set(QT_DONT_USE_QTGUI true)
include(${QT_USE_FILE})


set(YT_SRCS
    youtubeprovider.cpp
    responseparser.cpp)


QT4_AUTOMOC(${YT_SRCS})
add_library(joschyyoutube SHARED ${YT_SRCS})
target_link_libraries(joschyyoutube ${QT_LIBRARIES} ${JOSCHYCORE_LIBRARIES})
INSTALL(TARGETS joschyyoutube DESTINATION ${JOSCHYCORE_PLUGIN_INSTALL_DIR})
INSTALL(FILES youtubeprovider.plugin joschycore-youtube.png DESTINATION ${JOSCHYCORE_PLUGIN_INSTALL_DIR})
