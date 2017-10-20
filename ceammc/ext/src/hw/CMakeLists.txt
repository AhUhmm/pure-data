set(HW_SOURCES firmata_server.cpp firmata_parser.cpp
    firmata_lemon.c firmata_lemon.h firmata_lemon.y firmata_proto.h
    firmata_lexer.cpp firmata_lexer.h)
macro(ceammc_hw_extension name)
    list(APPEND HW_SOURCES "hw_${name}.cpp")
endmacro()

include_directories(${CMAKE_SOURCE_DIR}/ceammc/extra/arduino/firmata)

ceammc_hw_extension(firmata)

add_library(ceammc_hw STATIC mod_hw.cpp ${HW_SOURCES})
if(UNIX)
    set_target_properties(ceammc_hw PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
