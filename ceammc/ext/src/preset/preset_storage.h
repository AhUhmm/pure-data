#ifndef PARAM_STORAGE_H
#define PARAM_STORAGE_H

#include "ceammc_atomlist.h"
#include "ceammc_message.h"
#include "ceammc_object.h"

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <map>

using namespace ceammc;

class Preset {
    mutable t_symbol* name_;
    std::vector<Message> data_;
    int ref_count_;

public:
    static t_symbol* SYM_NONE;
    static t_symbol* SYM_LOAD;
    static t_symbol* SYM_STORE;
    static t_symbol* SYM_UPDATE;
    static t_symbol* SYM_CLEAR;
    static t_symbol* SYM_PRESET_ALL;

public:
    Preset(t_symbol* name);

    t_symbol* name() const;

    std::vector<Message>& data();
    const std::vector<Message>& data() const;

    bool hasDataAt(size_t idx) const;
    bool hasDataTypeAt(size_t idx, Message::Type t) const;
    bool hasFloatAt(size_t idx) { return hasDataTypeAt(idx, Message::FLOAT); }
    bool hasSymbolAt(size_t idx) { return hasDataTypeAt(idx, Message::SYMBOL); }
    bool hasListAt(size_t idx) { return hasDataTypeAt(idx, Message::LIST); }
    bool hasAnyAt(size_t idx) { return hasDataTypeAt(idx, Message::ANY); }

    float floatAt(size_t idx, float def = 0) const;
    t_symbol* symbolAt(size_t idx, t_symbol* def = SYM_NONE) const;
    AtomList listAt(size_t idx, const AtomList& def = AtomList()) const;
    AtomList anyAt(size_t idx, const AtomList& def = AtomList()) const;

    bool setFloatAt(size_t idx, float v);
    bool setSymbolAt(size_t idx, t_symbol* v);
    bool setListAt(size_t idx, const AtomList& l);
    bool setAnyAt(size_t idx, t_symbol* sel, const AtomList& args);

    bool clearAt(size_t idx);

    void refCountUp() { ref_count_++; }
    void refCountDown() { ref_count_--; }
    int refCount() const { return ref_count_; }
};

typedef boost::shared_ptr<Preset> PresetPtr;

class PresetStorage {
    PresetStorage();
    PresetStorage(const PresetStorage&);
    void operator=(const PresetStorage&);

private:
    typedef boost::unordered_map<t_symbol*, PresetPtr> PresetMap;
    PresetMap params_;

public:
    static PresetStorage& instance();
    size_t maxPresetCount() const;

    bool setFloatValueAt(t_symbol* name, size_t presetIdx, float v);
    float floatValueAt(t_symbol* name, size_t presetIdx, float def = 0) const;
    bool clearValueAt(t_symbol* name, size_t presetIdx);

    bool setSymbolValueAt(t_symbol* name, size_t presetIdx, t_symbol* v);
    t_symbol* symbolValueAt(t_symbol* name, size_t presetIdx, t_symbol* def) const;

    bool setListValueAt(t_symbol* name, size_t presetIdx, const AtomList& l);
    AtomList listValueAt(t_symbol* name, size_t presetIdx, const AtomList& def = AtomList()) const;

    bool setAnyValueAt(t_symbol* name, size_t presetIdx, t_symbol* sel, const AtomList& l);
    AtomList anyValueAt(t_symbol* name, size_t presetIdx, const AtomList& def = AtomList()) const;

    bool hasValueAt(t_symbol* name, size_t presetIdx) const;
    bool hasValueTypeAt(t_symbol* name, Message::Type t, size_t presetIdx) const;
    bool hasFloatValueAt(t_symbol* name, size_t presetIdx);

    bool write(const char* path) const;
    bool read(const char* path);

    AtomList keys() const;

    bool hasPreset(t_symbol* name);
    void bindPreset(t_symbol* name);
    void unbindPreset(t_symbol* name);

    void clearAll();
    void loadAll(size_t idx);
    void storeAll(size_t idx);
    void updateAll();

private:
    PresetPtr getOrCreate(t_symbol* name);
};

class PresetExternal : public BaseObject {
    t_canvas* root_cnv_;
    std::string patch_dir_;

public:
    PresetExternal(const PdArgs& args);

    AtomList p_keys() const;

    void m_load(t_symbol*, const AtomList& l);
    void m_store(t_symbol*, const AtomList& l);
    void m_clear(t_symbol*, const AtomList& l);
    void m_write(t_symbol*, const AtomList& fname);
    void m_read(t_symbol*, const AtomList& fname);
    void m_update(t_symbol*, const AtomList&);

    std::string makeDefaultPresetPath() const;
};

void setup_preset_storage();

#endif // PARAM_STORAGE_H
