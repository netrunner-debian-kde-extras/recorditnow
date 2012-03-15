include_directories(${CMAKE_SOURCE_DIR}/src/libs/plugin
    ${CMAKE_SOURCE_DIR}/src/libs/recorder
    ${CMAKE_SOURCE_DIR}/src/libs/encoder
    ${CMAKE_SOURCE_DIR}/src/libs/upload
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_BINARY_DIR}
    ${KDE4_INCLUDES})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})


if (BUILD_WITH_JOSCHY)
    include_directories(${JOSCHY_SNAPSHOT_INCLUDE_DIR})
endif (BUILD_WITH_JOSCHY)

macro_optional_find_package(XFixes)
macro_log_feature(XFIXES_FOUND "XFixes"
    "X.Org Xfixes library"
    "http://xorg.freedesktop.org/")

if (XFIXES_FOUND)
    add_definitions(-DXFIXES_FOUND=true)
endif()


add_subdirectory(libs)
add_subdirectory(plugins)
add_subdirectory(icons)
add_subdirectory(keymon)

macro_display_feature_log()


set(gui_SRCS
   main.cpp
   mainwindow.cpp
   recorditnowpluginmanager.cpp
   recordermanager.cpp
   encodermanager.cpp
   cursorwidget.cpp
   application.cpp
   keymonmanager.cpp
   devicesearchdialog.cpp
   checkicon.cpp

   zoom/zoomview.cpp
   zoom/zoomdock.cpp

   frame/frame.cpp
   frame/frameinfowidget.cpp
   frame/movewidget.cpp

   timeline/timelinedock.cpp
   timeline/timeline.cpp
   timeline/timelinetopicsdialog.cpp
   timeline/topicwidget.cpp
   timeline/topic.cpp
   timeline/topictreeitem.cpp

   upload/pluginpage.cpp
   upload/uploadwizard.cpp
   upload/videopage.cpp
   upload/accountpage.cpp
   upload/termspage.cpp
   upload/uploadpage.cpp

   keyboard/keyboarddock.cpp
   keyboard/keywidget.cpp
   keyboard/flowlayout.cpp
   keyboard/keyboardkey.cpp
   keyboard/proxylayout.cpp

   config/configdialog.cpp
   config/frameconfig.cpp
   config/keyboardconfig.cpp
   config/keyboardwizard.cpp
   config/keyboardkeypage.cpp
   config/keyboardkeyiconpage.cpp
   config/mouseconfig.cpp
   config/mousebutton.cpp
   config/configpage.cpp
   config/shortcutsconfig.cpp
   config/zoomconfig.cpp
   config/timelineconfig.cpp
   config/pluginconfig.cpp)


# GUI
kde4_add_ui_files(gui_SRCS 
    ui/toolbarwidget.ui
    ui/settings.ui
    ui/mouseconfig.ui 
    ui/zoom.ui 
    ui/timeline.ui 
    ui/timelinetopics.ui 
    ui/timelineconfig.ui 
    ui/frameconfig.ui
    upload/ui/pluginpage.ui
    upload/ui/videopage.ui
    upload/ui/uploadpage.ui
    upload/ui/termspage.ui
    upload/ui/accountpage.ui
    ui/keyboarddock.ui
    ui/keyboardconfig.ui
    ui/keyboardkeypage.ui
    ui/keyboardkeyiconpage.ui
    ui/keyboarddocktitlewidget.ui
    ui/devicesearchdialog.ui
    ui/shortcutsconfig.ui
    ui/pluginconfig.ui
    ui/timelinedocktitlewidget.ui
    ui/zoomdock.ui
    ui/zoomdocktitlewidget.ui)


kde4_add_kcfg_files(gui_SRCS recorditnow.kcfgc)
kde4_add_executable(recorditnow ${gui_SRCS})
target_link_libraries(recorditnow ${KDE4_KDEUI_LIBS}
                                  ${KDE4_KUTILS_LIBS}
                                  ${KDE4_KIO_LIBS}
                                  recorditnow_recorder
                                  recorditnow_encoder
                                  recorditnow_keymon
                                  ${X11_Xcursor_LIB})

if (BUILD_WITH_JOSCHY)
    target_link_libraries(recorditnow ${JOSCHY_SNAPSHOT_LIBRARIES})
else (BUILD_WITH_JOSCHY)
    target_link_libraries(recorditnow ${JOSCHYCORE_LIBRARIES})
endif (BUILD_WITH_JOSCHY)

install(TARGETS recorditnow ${INSTALL_TARGETS_DEFAULT_ARGS})

add_subdirectory(keyboard)

########### install files ###############
install(FILES recorditnow.kcfg  DESTINATION  ${KCFG_INSTALL_DIR})
install(FILES recorditnowui.rc  DESTINATION  ${DATA_INSTALL_DIR}/recorditnow)
