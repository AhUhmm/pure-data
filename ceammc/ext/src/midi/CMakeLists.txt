set(MIDI_SOURCES 
    datatype_midistream.cpp datatype_midistream.h
    datatype_miditrack.cpp datatype_miditrack.h
    midi_common.cpp midi_common.h
)

macro(ceammc_midi_extension name)
    list(APPEND MIDI_SOURCES "midi_${name}.cpp")
endmacro()

ceammc_midi_extension(event)
ceammc_midi_extension(event_to_ctl)
ceammc_midi_extension(file)
ceammc_midi_extension(track)
ceammc_midi_extension(ctl2str)
ceammc_midi_extension(key2str)
ceammc_midi_extension(prg2str)
ceammc_midi_extension(str2key)

include_directories(${PROJECT_SOURCE_DIR}/ceammc/extra/midifile)
add_library(ceammc_midi STATIC mod_midi.cpp ${MIDI_SOURCES})
if(UNIX)
    set_target_properties(ceammc_midi PROPERTIES COMPILE_FLAGS "-fPIC")
endif()

target_link_libraries(ceammc_midi midifile)
