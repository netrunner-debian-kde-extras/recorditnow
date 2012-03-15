set(SRCS
   abstractencoder.cpp)

kde4_add_library(recorditnow_encoder SHARED ${SRCS})
target_link_libraries(recorditnow_encoder ${KDE4_KDECORE_LIBS} recorditnow_plugin)
install(TARGETS recorditnow_encoder ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES recorditnow_encoder.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
