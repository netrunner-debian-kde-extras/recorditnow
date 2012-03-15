set(SRCS
  ffmpegrecorder.cpp)

set(confg_SRCS
  ffmpegrecorderconfig.cpp)


kde4_add_ui_files(SRCS ui/ffmpegrecorderconfig.ui)
kde4_add_kcfg_files(SRCS recorditnow_ffmpegrecorder.kcfgc)
kde4_add_plugin(recorditnow_ffmpegrecorder ${SRCS})
target_link_libraries(recorditnow_ffmpegrecorder ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} recorditnow_recorder recorditnow_plugin ${X11_LIBRARIES})
install(TARGETS recorditnow_ffmpegrecorder DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES recorditnow_ffmpegrecorder.desktop DESTINATION ${SERVICES_INSTALL_DIR})


kde4_add_ui_files(confg_SRCS ui/ffmpegrecorderconfig.ui)
kde4_add_kcfg_files(confg_SRCS recorditnow_ffmpegrecorder.kcfgc)
kde4_add_plugin(recorditnow_ffmpegrecorder_config ${confg_SRCS})
target_link_libraries(recorditnow_ffmpegrecorder_config ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
install(TARGETS recorditnow_ffmpegrecorder_config DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES recorditnow_ffmpegrecorder_config.desktop DESTINATION ${SERVICES_INSTALL_DIR})   
install(FILES recorditnow_ffmpegrecorder.kcfg DESTINATION ${KCFG_INSTALL_DIR})

