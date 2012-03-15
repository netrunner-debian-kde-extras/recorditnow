set(recordmydesktop_SRCS
  recordmydesktoprecorder.cpp)

set(recordmydesktop_confg_SRCS
  recordmydesktopconfig.cpp
  sounddevice.cpp
  sounddevicedialog.cpp)


kde4_add_ui_files(recordmydesktop_SRCS ui/recordmydesktop.ui)
kde4_add_kcfg_files(recordmydesktop_SRCS recorditnow_recordmydesktop.kcfgc)
kde4_add_plugin(recorditnow_recordmydesktop ${recordmydesktop_SRCS})
target_link_libraries(recorditnow_recordmydesktop ${KDE4_KDECORE_LIBS} ${KDE4_KDEUI_LIBS} recorditnow_recorder recorditnow_plugin)
install(TARGETS recorditnow_recordmydesktop DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES recorditnow_recordmydesktop.desktop DESTINATION ${SERVICES_INSTALL_DIR})


kde4_add_ui_files(recordmydesktop_confg_SRCS ui/recordmydesktop.ui ui/sounddevicedialog.ui)
kde4_add_kcfg_files(recordmydesktop_confg_SRCS recorditnow_recordmydesktop.kcfgc)
kde4_add_plugin(recorditnow_recordmydesktop_config ${recordmydesktop_confg_SRCS})
target_link_libraries(recorditnow_recordmydesktop_config ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})
install(TARGETS recorditnow_recordmydesktop_config DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES recorditnow_recordmydesktop_config.desktop DESTINATION ${SERVICES_INSTALL_DIR})   
install(FILES recorditnow_recordmydesktop.kcfg DESTINATION ${KCFG_INSTALL_DIR})

