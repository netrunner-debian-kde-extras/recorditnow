set(lib_SRCS
   abstractrecorder.cpp)

kde4_add_library(recorditnow_recorder SHARED ${lib_SRCS})
target_link_libraries(recorditnow_recorder ${KDE4_KDECORE_LIBS} recorditnow_plugin)
install(TARGETS recorditnow_recorder ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES recorditnow_recorder.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
