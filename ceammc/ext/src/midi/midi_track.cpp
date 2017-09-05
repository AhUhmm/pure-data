#include "midi_track.h"
#include "MidiFile.h"
#include "ceammc_factory.h"
#include "datatype_midistream.h"

static t_symbol* SYM_MIDI_EVENT = gensym("MidiEvent");

MidiTrack::MidiTrack(const PdArgs& args)
    : BaseObject(args)
    , midi_track_()
    , join_(0)
    , track_idx_(0)
    , tempo_(0)
    , current_event_idx_(0)
{
    join_ = new FlagProperty("@join");
    track_idx_ = new SizeTProperty("@index", positionalFloatArgument(0, 0));
    tempo_ = new IntProperty("@tempo", 0);

    createProperty(join_);
    createProperty(track_idx_);

    createOutlet();
    createOutlet();

    createCbProperty("@events", &MidiTrack::p_events);

    // reserve space to avoid runtime reallocations
    current_event_.reserve(10);
}

void MidiTrack::onBang()
{
    dataTo(0, midi_track_.clone());
}

void MidiTrack::onDataT(const DataTypeMidiStream& s)
{
    if (join_->value()) {
        // copy
        MidiFile mf = *s.midifile();
        mf.joinTracks();

        midi_track_ = DataTypeMidiTrack(mf[0]);
        tempo_->setValue(mf.getTicksPerQuarterNote());

    } else {
        const size_t trackN = track_idx_->value();
        const MidiFile mf = *s.midifile();
        if (mf.getTrackCount() <= trackN) {
            OBJ_ERR << "invalid track index: " << trackN;
            return;
        }

        midi_track_ = DataTypeMidiTrack(mf[trackN]);
        tempo_->setValue(mf.getTicksPerQuarterNote());
    }

    current_event_idx_ = 0;
}

AtomList MidiTrack::p_events() const
{
    return Atom(size());
}

void MidiTrack::m_next(t_symbol*, const AtomList&)
{
    if (current_event_idx_ >= size()) {
        OBJ_DBG << "end of track reached";
        return;
    }

    size_t next_idx = findNextEventIndex(current_event_idx_);
    if (!next_idx) {
        OBJ_DBG << "end of track reached";
        return;
    }

    current_event_idx_ = next_idx;
    outputCurrent();
}

void MidiTrack::m_reset(t_symbol*, const AtomList&)
{
    current_event_idx_ = 0;
}

void MidiTrack::m_output(t_symbol*, const AtomList&)
{
    for (size_t i = 0; i < size(); i++)
        outputEvent(midi_track_.eventAt(i));
}

void MidiTrack::outputEvent(MidiEvent* ev)
{
    current_event_.clear();

    current_event_.append(ev->tick);
    current_event_.append(ev->track);
    current_event_.append(ev->getDurationInSeconds() / 1000.f);

    const size_t size = ev->size();
    for (size_t i = 0; i < size; i++)
        current_event_.append(Atom(ev->operator[](i)));

    anyTo(0, SYM_MIDI_EVENT, current_event_);
}

struct NewTickFinder {
    const int tick;
    NewTickFinder(int t)
        : tick(t)
    {
    }

    bool operator()(MidiEvent* e) { return e->tick != tick; }
};

MidiTrack::MidiEventIterator MidiTrack::findNextEvent(MidiEventIterator ev)
{
    return std::find_if(ev, end(), NewTickFinder((*ev)->tick));
}

MidiTrack::MidiEventConstIterator MidiTrack::findNextEvent(MidiTrack::MidiEventConstIterator ev) const
{
    return std::find_if(ev, end(), NewTickFinder((*ev)->tick));
}

size_t MidiTrack::findNextEventIndex(size_t idx) const
{
    if (idx >= size())
        return 0;

    MidiEventConstIterator cur_ev = begin() + idx;
    MidiEventConstIterator next_ev = findNextEvent(cur_ev);

    return next_ev - begin();
}

int MidiTrack::currentTick() const
{
    if (current_event_idx_ >= size())
        return 0;

    return midi_track_.eventAt(current_event_idx_)->tick;
}

struct EventOutput {
    MidiTrack* track;
    EventOutput(MidiTrack* t)
        : track(t)
    {
    }

    void operator()(MidiEvent* e) { track->outputEvent(e); }
};

void MidiTrack::outputCurrent()
{
    if (current_event_idx_ >= size())
        return;

    MidiEventIterator cur_ev = begin() + current_event_idx_;
    MidiEventIterator next_ev = findNextEvent(cur_ev);

    int tick_duration = 0;
    if (next_ev != end())
        tick_duration = (*next_ev)->tick - (*cur_ev)->tick;

    floatTo(1, tick_duration);

    // output all events before next tick
    EventOutput event_out(this);
    std::for_each(cur_ev, next_ev, event_out);
}

void setup_midi_track()
{
    ObjectFactory<MidiTrack> obj("midi.track");
    obj.processData<DataTypeMidiStream>();
    obj.addMethod("next", &MidiTrack::m_next);
    obj.addMethod("reset", &MidiTrack::m_reset);
    obj.addMethod("output", &MidiTrack::m_output);
}
