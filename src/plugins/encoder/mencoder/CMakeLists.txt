set(SRCS
  mencoderencoder.cpp)

set(confg_SRCS
  mencoderencoderconfig.cpp
  formatdialog.cpp)


kde4_add_kcfg_files(SRCS recorditnow_mencoder.kcfgc)
kde4_add_plugin(recorditnow_mencoder ${SRCS})
target_link_libraries(recorditnow_mencoder ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} recorditnow_encoder recorditnow_plugin)
install(TARGETS recorditnow_mencoder DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES recorditnow_mencoder.desktop DESTINATION ${SERVICES_INSTALL_DIR})


kde4_add_ui_files(confg_SRCS ui/mencoder.ui ui/addformat.ui)
kde4_add_kcfg_files(confg_SRCS recorditnow_mencoder.kcfgc)
kde4_add_plugin(recorditnow_mencoder_config ${confg_SRCS})
target_link_libraries(recorditnow_mencoder_config ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})               
install(TARGETS recorditnow_mencoder_config DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES recorditnow_mencoder_config.desktop DESTINATION ${SERVICES_INSTALL_DIR})   
install(FILES recorditnow_mencoder.kcfg DESTINATION  ${KCFG_INSTALL_DIR})

