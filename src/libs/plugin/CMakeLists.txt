set(SRCS
   recorditnowplugin.cpp)

kde4_add_library(recorditnow_plugin SHARED ${SRCS})
target_link_libraries(recorditnow_plugin ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS})
install(TARGETS recorditnow_plugin ${INSTALL_TARGETS_DEFAULT_ARGS})
