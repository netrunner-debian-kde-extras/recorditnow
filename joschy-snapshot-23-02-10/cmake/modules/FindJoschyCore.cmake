# - Find JoschyCore
# Find the JoschyCore library
#
# This module defines
#  JOSCHYCORE_FOUND - whether the JoschyCore library was found
#  JOSCHYCORE_LIBRARIES - the library
#  JOSCHYCORE_INCLUDE_DIR - the include path of the library
#  JOSCHYCORE_PLUGIN_INSTALL_DIR - install path for plugins
#

if (JOSCHYCORE_INCLUDE_DIR AND JOSCHYCORE_LIBRARIES AND JOSCHYCORE_PLUGIN_INSTALL_DIR)
    # Already in cache
    set (JOSCHYCORE_FOUND TRUE)
else (JOSCHYCORE_INCLUDE_DIR AND JOSCHYCORE_LIBRARIES AND JOSCHYCORE_PLUGIN_INSTALL_DIR)
    if (NOT WIN32)
        # lib suffix
        if(CMAKE_SIZEOF_VOID_P EQUAL 4)
            SET(LIB_SUFFIX "")
        else(CMAKE_SIZEOF_VOID_P EQUAL 4)
            SET(LIB_SUFFIX 64)
        endif()
    endif(NOT WIN32)

    # libraries
    find_library (JOSCHYCORE_LIBRARIES NAMES joschycore PATHS ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})

    # include
    find_path (JOSCHYCORE_INCLUDE_DIR NAMES joschycore/manager.h PATHS ${INCLUDE_INSTALL_DIR})

    # plugin install dir
    set (JOSCHYCORE_PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/joschy)

    # status
    message(STATUS)
    message(STATUS "--------------- JoschyCore ---------------")
    message(STATUS "Libraries:    " ${JOSCHYCORE_LIBRARIES})
    message(STATUS "Include:      " ${JOSCHYCORE_INCLUDE_DIR})
    message(STATUS "Plugins:      " ${JOSCHYCORE_PLUGIN_INSTALL_DIR})
    message(STATUS "------------------------------------------")
    message(STATUS)


    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(JoschyCore DEFAULT_MSG JOSCHYCORE_LIBRARIES JOSCHYCORE_INCLUDE_DIR JOSCHYCORE_PLUGIN_INSTALL_DIR)

endif (JOSCHYCORE_INCLUDE_DIR AND JOSCHYCORE_LIBRARIES AND JOSCHYCORE_PLUGIN_INSTALL_DIR)


mark_as_advanced(JOSCHYCORE_INCLUDE_DIR JOSCHYCORE_LIBRARIES JOSCHYCORE_PLUGIN_INSTALL_DIR)
