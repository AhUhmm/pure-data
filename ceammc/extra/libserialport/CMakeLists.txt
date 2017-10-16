# system checks
include(CheckIncludeFile)
include(CheckTypeSize)
include(CheckFunctionExists)
include(CheckStructHasMember)
include(CheckSymbolExists)

check_function_exists(realpath HAVE_REALPATH)
check_include_file(dlfcn.h HAVE_DLFCN_H)
check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)

set(CMAKE_REQUIRED_INCLUDES linux/termios.h)
check_type_size(termios2 HAVE_STRUCT_TERMIOS2)
check_type_size(termiox HAVE_STRUCT_TERMIOX)

set(CMAKE_REQUIRED_INCLUDES linux/serial.h)
check_type_size(serial_struct HAVE_STRUCT_SERIAL_STRUCT)

check_struct_has_member(termios2 c_ispeed linux/termios.h HAVE_STRUCT_TERMIOS2_C_ISPEED)
check_struct_has_member(termios2 c_ospeed linux/termios.h HAVE_STRUCT_TERMIOS2_C_OSPEED)
check_struct_has_member(termios c_ispeed linux/termios.h HAVE_STRUCT_TERMIOS_C_ISPEED)
check_struct_has_member(termios c_ospeed linux/termios.h HAVE_STRUCT_TERMIOS_C_OSPEED)
check_symbol_exists(BOTHER linux/termios.h HAVE_DECL_BOTHER)
set(SP_API "")
set(SP_PRIV "")

# generating config
configure_file(config.h.in "${CMAKE_CURRENT_BINARY_DIR}/config-sp.h")
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# adding sources
set(LIBSERIALPORT_SOURCES libserialport_internal.h libserialport.h serialport.c)
if(APPLE)
    list(APPEND LIBSERIALPORT_SOURCES macosx.c)
    check_include_file(IOKit/IOKitLib.h _IOKIT_INCLUDE)
elseif(WIN32 OR MSYS OR MINGW)
    list(APPEND LIBSERIALPORT_SOURCES windows.c)
else()
    list(APPEND LIBSERIALPORT_SOURCES linux.c linux_termios.c linux_termios.h)
endif()

# create library
add_library(libserialport STATIC ${LIBSERIALPORT_SOURCES})
add_custom_target(libserialport_config SOURCES config.h.in libserialport.h.in)

# linking
if(APPLE)
    target_link_libraries(libserialport "-framework IOKit -framework CoreFoundation")
elseif(WIN32 OR MINGW OR MSYS)
    target_link_libraries(libserialport setupapi)
else()
    set_target_properties(libserialport PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

