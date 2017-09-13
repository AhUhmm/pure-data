#include "mod_midi.h"
#include "midi_event.h"
#include "midi_file.h"
#include "midi_progchange2str.h"
#include "midi_track.h"

void ceammc_midi_setup()
{
    setup_midi_event();
    setup_midi_file();
    setup_midi_track();
    setup_midi_progchange2str();
}
