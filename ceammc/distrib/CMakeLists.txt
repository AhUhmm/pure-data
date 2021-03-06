add_subdirectory(win)
add_subdirectory(mac)

set(MAKELIB_SCRIPT "")
set(MAKELIB_EXTENDED_SCRIPT "")

if(APPLE)
    set(DYLIBBUNDLER "${CMAKE_CURRENT_BINARY_DIR}/mac/dylibbundler")
    set(MAKELIB_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/mac/make_lib_mac.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mac/make_lib_mac.sh
                   ${CMAKE_CURRENT_BINARY_DIR}/mac/make_lib_mac.sh @ONLY)

    set(MAKELIB_EXTENDED_SCRIPT "${CMAKE_CURRENT_BINARY_DIR}/mac/make_lib_extended_mac.sh")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/mac/make_lib_extended_mac.sh
                   ${CMAKE_CURRENT_BINARY_DIR}/mac/make_lib_extended_mac.sh @ONLY)

endif()

if(MAKELIB_SCRIPT)
    add_custom_target(ceammc_lib
        COMMAND "${MAKELIB_SCRIPT}"
            "${CMAKE_CURRENT_SOURCE_DIR}/.."
            "${CMAKE_CURRENT_BINARY_DIR}/../ext"
            "${CMAKE_BINARY_DIR}/dist"
            "${CEAMMC_LIB_VERSION}"
        SOURCES mac/make_lib_mac.sh)
endif()

if(MAKELIB_EXTENDED_SCRIPT)
    add_custom_target(ceammc_lib_extended
        COMMAND "${MAKELIB_EXTENDED_SCRIPT}"
            "${CMAKE_CURRENT_SOURCE_DIR}/.."
            "${CMAKE_CURRENT_BINARY_DIR}/.."
            "${CMAKE_BINARY_DIR}/dist"
            "${CEAMMC_LIB_VERSION}"
        SOURCES mac/make_lib_extended_mac.sh)
endif()

add_custom_target(distrib_sources SOURCES ceammc_pddoc_link_fix.py)
