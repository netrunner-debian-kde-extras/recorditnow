set(SRCS
  ffmpegencoder.cpp)

set(confg_SRCS
  ffmpegencoderconfig.cpp
  formatdialog.cpp)


kde4_add_kcfg_files(SRCS recorditnow_ffmpeg.kcfgc)
kde4_add_plugin(recorditnow_ffmpeg ${SRCS})
target_link_libraries(recorditnow_ffmpeg ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} recorditnow_encoder recorditnow_plugin)
install(TARGETS recorditnow_ffmpeg DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES recorditnow_ffmpeg.desktop DESTINATION ${SERVICES_INSTALL_DIR})


kde4_add_ui_files(confg_SRCS ui/ffmpeg.ui ui/addformat.ui)
kde4_add_kcfg_files(confg_SRCS recorditnow_ffmpeg.kcfgc)
kde4_add_plugin(recorditnow_ffmpeg_config ${confg_SRCS})
target_link_libraries(recorditnow_ffmpeg_config ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
install(TARGETS recorditnow_ffmpeg_config DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES recorditnow_ffmpeg_config.desktop DESTINATION ${SERVICES_INSTALL_DIR})   
install(FILES recorditnow_ffmpeg.kcfg DESTINATION ${KCFG_INSTALL_DIR})

