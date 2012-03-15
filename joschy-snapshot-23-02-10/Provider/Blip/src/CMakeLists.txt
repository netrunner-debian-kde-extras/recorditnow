include_directories(${CMAKE_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_BINARY_DIR}
                    ${QT_INCLUDE_DIR}
                    ${JOSCHYCORE_INCLUDE_DIR})


set(QT_USE_QTXML true)
set(QT_DONT_USE_QTGUI true)
include(${QT_USE_FILE})


set(SRCS
    blipprovider.cpp
    responseparser.cpp)


QT4_AUTOMOC(${SRCS})
add_library(joschyblip SHARED ${SRCS})
target_link_libraries(joschyblip ${QT_LIBRARIES} ${JOSCHYCORE_LIBRARIES})
INSTALL(TARGETS joschyblip DESTINATION ${JOSCHYCORE_PLUGIN_INSTALL_DIR})
INSTALL(FILES blipprovider.plugin joschycore-blip.png DESTINATION ${JOSCHYCORE_PLUGIN_INSTALL_DIR})


