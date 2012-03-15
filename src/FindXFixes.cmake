
find_package(PkgConfig)
if (NOT PKG_CONFIG_FOUND)
    message(STATUS "Could not find pkg-config (pkg name: pkg-config)")
else()
    pkg_search_module(XFIXES xfixes)
    if (XFIXES_FOUND)
        include_directories(${XFIXES_INCLUDE_DIRS})
        add_definitions(${XFIXES_DEFINITIONS})
    endif (XFIXES_FOUND)
endif (NOT PKG_CONFIG_FOUND)


