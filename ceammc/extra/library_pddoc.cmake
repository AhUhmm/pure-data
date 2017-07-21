# makes target for generating PureData library documentation
#
# arguments are:
#   NAME    - library name
#   VERSION - library version
#   HOME    - library home page
#   FILES   - list of library PDDOC file names (without pddoc extension)
#
# creates:
#   target <b>NAME_pddoc</b> to start generation
#
# installes:
#    generated help files into ${PD_INTERNAL_EXT_INSTALL_PATH}/NAME
#    generated library help file into ${PD_INTERNAL_EXT_INSTALL_PATH}/NAME/NAME-help.pd
#    generated library xlet database file (NAME-into ${PD_INTERNAL_EXT_INSTALL_PATH}/NAME/NAME.db
# 
# uses:
#   PD_DOC2PD  - path to pd_doc2pd
#   PD_MAKELIB - path to pd_makelibrary
#   PD_LIB2PD  - path to pd_lib2pd
#   cat, sort  - unix command line utilities
#   ${PD_INTERNAL_EXT_INSTALL_PATH}
# 

include(CMakeParseArguments)
# allow creating targets with reserved names or which do not match the validity pattern
if(${CMAKE_VERSION} VERSION_GREATER "3.0")
    cmake_policy(SET CMP0037 OLD)
endif()

function(make_pddoc_lib)
    set(_OPTIONS_ARGS)
    set(_ONE_VALUE_ARGS NAME VERSION HOME)
    set(_MULTI_VALUE_ARGS FILES)

    cmake_parse_arguments(_LIB "${_OPTIONS_ARGS}" "${_ONE_VALUE_ARGS}" "${_MULTI_VALUE_ARGS}" ${ARGN})
    
    set(_LIB_PD_FILES)
    set(_LIB_XLET_DB_FILES)
    set(_LIB_PDDOC_FILES)
    
    foreach(f ${_LIB_FILES})
        set(fname "${CMAKE_CURRENT_BINARY_DIR}/${f}-help.pd")
        set(fname_pddoc "${CMAKE_CURRENT_SOURCE_DIR}/${f}.pddoc")
        list(APPEND _LIB_PD_FILES ${fname})
        list(APPEND _LIB_PDDOC_FILES ${fname_pddoc})
        add_custom_command(
            OUTPUT "${fname}"
            DEPENDS "${fname_pddoc}"
            COMMAND ${PD_DOC2PD}
                 --force
                 --version "${_LIB_VERSION}"
                 --website "${_LIB_HOME}"
                 --xlet-db "${CMAKE_SOURCE_DIR}/ceammc/ext/doc/ceammc.db"
                "${fname_pddoc}" ${fname}
            COMMAND ${CMAKE_COMMAND} -E copy_if_different ${fname} ${CMAKE_CURRENT_SOURCE_DIR}
            VERBATIM)
        
        # install generated help files
        install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${f}-help.pd"
            DESTINATION "${PD_INTERNAL_EXT_INSTALL_PATH}/${_LIB_NAME}")
    endforeach()

    add_custom_command(
        OUTPUT "${_LIB_NAME}_lib.xml"
        DEPENDS ${_LIB_PDDOC_FILES}
        COMMAND ${PD_MAKELIB}
            --library "${_LIB_NAME}"
            --version "${_LIB_VERSION}"
            --output "${_LIB_NAME}_lib.xml" ${_LIB_PDDOC_FILES}
            --search-path ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_LIB_NAME}_lib.xml" ${CMAKE_CURRENT_SOURCE_DIR})

    add_custom_command(
        OUTPUT "${_LIB_NAME}-help.pd"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${_LIB_NAME}_lib.xml"
        COMMAND ${PD_LIB2PD} "${CMAKE_CURRENT_SOURCE_DIR}/${_LIB_NAME}_lib.xml"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_LIB_NAME}-help.pd" ${CMAKE_CURRENT_SOURCE_DIR})

    add_custom_target(${_LIB_NAME}_pddoc
        DEPENDS ${_LIB_PD_FILES} "${_LIB_NAME}_lib.xml" "${_LIB_NAME}-help.pd"
        COMMAND cat "*-xlet_db.txt" | sort > "${_LIB_NAME}.db"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_LIB_NAME}.db" ${CMAKE_CURRENT_SOURCE_DIR})
    
    # install library help file
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${_LIB_NAME}-help.pd"
        DESTINATION "${PD_INTERNAL_EXT_INSTALL_PATH}/${_LIB_NAME}")
    # install library xlet db file
    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/${_LIB_NAME}.db"
        DESTINATION "${PD_INTERNAL_EXT_INSTALL_PATH}/${_LIB_NAME}")
endfunction()
