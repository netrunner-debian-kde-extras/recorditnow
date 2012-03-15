set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR})

set(SRCS
    event.cpp
    manager.cpp
    deviceinfo.cpp
    device.cpp)

# KeyMon
kde4_add_library(recorditnow_keymon SHARED ${SRCS})
target_link_libraries(recorditnow_keymon ${KDE4_KDECORE_LIBS} ${KDE4_SOLID_LIBS})
install(TARGETS recorditnow_keymon ${INSTALL_TARGETS_DEFAULT_ARGS})


set(KAUTH_HELPER_BACKEND_NAME "DBUS")
# helper
set(HELPER_SRCS
    devicehelper.cpp)

kde4_add_executable(recorditnow_helper ${HELPER_SRCS})
target_link_libraries(recorditnow_helper ${KDE4_KDECORE_LIBS} recorditnow_keymon)
install(TARGETS recorditnow_helper DESTINATION ${LIBEXEC_INSTALL_DIR})

kde4_install_auth_helper_files(recorditnow_helper org.kde.recorditnow.helper root)
kde4_install_auth_actions(org.kde.recorditnow.helper helper.actions)


