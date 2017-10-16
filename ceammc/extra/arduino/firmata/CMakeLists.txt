set(FIRMATA_SRC firmata.cpp firmata.h firmata.ksy kaitai/kaitaistream.cpp)

add_definitions(-DKS_STR_ENCODING_NONE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_library(arduino_firmata STATIC ${FIRMATA_SRC})

if(UNIX)
    set_target_properties(arduino_firmata PROPERTIES COMPILE_FLAGS "-fPIC")
endif()
