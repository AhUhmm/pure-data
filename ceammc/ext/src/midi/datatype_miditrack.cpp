#include "datatype_miditrack.h"
#include "ceammc_datatypes.h"

const DataType DataTypeMidiTrack::dataType = data::DATA_MIDI_TRACK;

DataTypeMidiTrack::DataTypeMidiTrack()
{
}

DataTypeMidiTrack::DataTypeMidiTrack(const MidiEventList& lst)
    : events_(lst)
{
    events_.linkNotePairs();
}

DataTypeMidiTrack* DataTypeMidiTrack::clone() const
{
    return new DataTypeMidiTrack(*this);
}

DataType DataTypeMidiTrack::type() const
{
    return data::DATA_MIDI_TRACK;
}

size_t DataTypeMidiTrack::eventCount() const
{
    return events_.getSize();
}

MidiEventList& DataTypeMidiTrack::events()
{
    return events_;
}

MidiEvent* DataTypeMidiTrack::eventAt(size_t n)
{
    return &events_[n];
}

const MidiEvent* DataTypeMidiTrack::eventAt(size_t n) const
{
    return &events_[n];
}
