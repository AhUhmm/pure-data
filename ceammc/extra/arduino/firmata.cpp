// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include "firmata.h"

#include <iostream>
#include <fstream>

firmata_t::firmata_t(kaitai::kstream *p_io, kaitai::kstruct *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = this;
    m_message = new std::vector<message_t*>();
    while (!m__io->is_eof()) {
        m_message->push_back(new message_t(m__io, this, m__root));
    }
}

firmata_t::~firmata_t() {
    for (std::vector<message_t*>::iterator it = m_message->begin(); it != m_message->end(); ++it) {
        delete *it;
    }
    delete m_message;
}

firmata_t::extended_analog_t::extended_analog_t(kaitai::kstream *p_io, firmata_t::sysex_body_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_pin = m__io->read_u1();
    m_value = new string_bit7_t(m__io, this, m__root);
}

firmata_t::extended_analog_t::~extended_analog_t() {
    delete m_value;
}

firmata_t::sysex_t::sysex_t(kaitai::kstream *p_io, firmata_t::message_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_id = m__io->read_u1();
    m_body = new sysex_body_t(m__io, this, m__root);
}

firmata_t::sysex_t::~sysex_t() {
    delete m_body;
}

firmata_t::report_firmware_t::report_firmware_t(kaitai::kstream *p_io, firmata_t::sysex_body_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_major = m__io->read_u1();
    m_minor = m__io->read_u1();
    m_str = new string_bit7_t(m__io, this, m__root);
}

firmata_t::report_firmware_t::~report_firmware_t() {
    delete m_str;
}

firmata_t::capability_query_t::capability_query_t(kaitai::kstream *p_io, kaitai::kstruct *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
}

firmata_t::capability_query_t::~capability_query_t() {
}

firmata_t::sysex_body_t::sysex_body_t(kaitai::kstream *p_io, firmata_t::sysex_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    switch (_parent()->id()) {
    case 0:
        m_body = new sysex_ext_t(m__io, this, m__root);
        break;
    case 113:
        m_body = new string_bit7_t(m__io, this, m__root);
        break;
    case 121:
        m_body = new report_firmware_t(m__io, this, m__root);
        break;
    case 123:
        m_body = new capability_response_t(m__io, this, m__root);
        break;
    case 122:
        m_body = new sampling_interval_t(m__io, this, m__root);
        break;
    case 111:
        m_body = new extended_analog_t(m__io, this, m__root);
        break;
    }
}

firmata_t::sysex_body_t::~sysex_body_t() {
}

firmata_t::set_digital_pin_value_t::set_digital_pin_value_t(kaitai::kstream *p_io, firmata_t::message_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_pin = m__io->read_u1();
    m_value = m__io->read_u1();
}

firmata_t::set_digital_pin_value_t::~set_digital_pin_value_t() {
}

firmata_t::sysex_ext_t::sysex_ext_t(kaitai::kstream *p_io, firmata_t::sysex_body_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    f_id = false;
    m_lsb = m__io->read_u1();
    m_msb = m__io->read_u1();
    m_body = new string_bit7_t(m__io, this, m__root);
}

firmata_t::sysex_ext_t::~sysex_ext_t() {
    delete m_body;
}

int32_t firmata_t::sysex_ext_t::id() {
    if (f_id)
        return m_id;
    m_id = ((239 & lsb()) | ((239 & msb()) << 7));
    f_id = true;
    return m_id;
}

firmata_t::report_digital_port_t::report_digital_port_t(kaitai::kstream *p_io, firmata_t::message_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_state = m__io->read_u1();
}

firmata_t::report_digital_port_t::~report_digital_port_t() {
}

firmata_t::analog_io_t::analog_io_t(kaitai::kstream *p_io, firmata_t::message_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    f_value = false;
    m_lsb = m__io->read_u1();
    m_msb = m__io->read_u1();
}

firmata_t::analog_io_t::~analog_io_t() {
}

int32_t firmata_t::analog_io_t::value() {
    if (f_value)
        return m_value;
    m_value = ((224 & lsb()) | ((224 & msb()) << 7));
    f_value = true;
    return m_value;
}

firmata_t::pin_capability_list_t::pin_capability_list_t(kaitai::kstream *p_io, kaitai::kstruct *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_modes = new std::vector<pin_capability_t*>();
    {
        pin_capability_t* _;
        do {
            _ = new pin_capability_t(m__io, this, m__root);
            m_modes->push_back(_);
        } while (!(_->mode() != 127));
    }
}

firmata_t::pin_capability_list_t::~pin_capability_list_t() {
    for (std::vector<pin_capability_t*>::iterator it = m_modes->begin(); it != m_modes->end(); ++it) {
        delete *it;
    }
    delete m_modes;
}

firmata_t::pin_capability_list_t::pin_capability_t::pin_capability_t(kaitai::kstream *p_io, kaitai::kstruct *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_mode = m__io->read_u1();
    m_resolution = m__io->read_u1();
}

firmata_t::pin_capability_list_t::pin_capability_t::~pin_capability_t() {
}

firmata_t::string_bit7_t::string_bit7_t(kaitai::kstream *p_io, kaitai::kstruct *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_b = new std::vector<uint8_t>();
    {
        uint8_t _;
        do {
            _ = m__io->read_u1();
            m_b->push_back(_);
        } while (!(_ != 255));
    }
}

firmata_t::string_bit7_t::~string_bit7_t() {
    delete m_b;
}

firmata_t::sampling_interval_t::sampling_interval_t(kaitai::kstream *p_io, firmata_t::sysex_body_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    f_interval = false;
    m_lsb = m__io->read_u1();
    m_msb = m__io->read_u1();
}

firmata_t::sampling_interval_t::~sampling_interval_t() {
}

int32_t firmata_t::sampling_interval_t::interval() {
    if (f_interval)
        return m_interval;
    m_interval = ((224 & lsb()) | ((224 & msb()) << 7));
    f_interval = true;
    return m_interval;
}

firmata_t::capability_response_t::capability_response_t(kaitai::kstream *p_io, firmata_t::sysex_body_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_b = m__io->read_u1();
}

firmata_t::capability_response_t::~capability_response_t() {
}

firmata_t::report_analog_pin_t::report_analog_pin_t(kaitai::kstream *p_io, firmata_t::message_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_state = m__io->read_u1();
}

firmata_t::report_analog_pin_t::~report_analog_pin_t() {
}

firmata_t::set_pin_mode_t::set_pin_mode_t(kaitai::kstream *p_io, firmata_t::message_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_pin = m__io->read_u1();
    m_mode = static_cast<firmata_t::pin_modes_t>(m__io->read_u1());
}

firmata_t::set_pin_mode_t::~set_pin_mode_t() {
}

firmata_t::message_t::message_t(kaitai::kstream *p_io, firmata_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    f_event_type = false;
    f_channel = false;
    m_message_header = m__io->read_u1();
    n_sysex = true;
    if (message_header() == 240) {
        n_sysex = false;
        m_sysex = new sysex_t(m__io, this, m__root);
    }
    n_message_body = true;
    if (message_header() != 240) {
        n_message_body = false;
        switch (event_type()) {
        case 224:
            m_message_body = new analog_io_t(m__io, this, m__root);
            break;
        case 244:
            m_message_body = new set_pin_mode_t(m__io, this, m__root);
            break;
        case 144:
            m_message_body = new digital_io_t(m__io, this, m__root);
            break;
        case 245:
            m_message_body = new set_digital_pin_value_t(m__io, this, m__root);
            break;
        case 208:
            m_message_body = new report_digital_port_t(m__io, this, m__root);
            break;
        case 192:
            m_message_body = new report_analog_pin_t(m__io, this, m__root);
            break;
        case 254:
            m_message_body = new protocol_version_t(m__io, this, m__root);
            break;
        }
    }
}

firmata_t::message_t::~message_t() {
    if (!n_sysex) {
        delete m_sysex;
    }
}

int32_t firmata_t::message_t::event_type() {
    if (f_event_type)
        return m_event_type;
    m_event_type = (message_header() & 240);
    f_event_type = true;
    return m_event_type;
}

int32_t firmata_t::message_t::channel() {
    if (f_channel)
        return m_channel;
    m_channel = (message_header() & 15);
    f_channel = true;
    return m_channel;
}

firmata_t::digital_io_t::digital_io_t(kaitai::kstream *p_io, firmata_t::message_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    f_value = false;
    m_lsb = m__io->read_u1();
    m_msb = m__io->read_u1();
}

firmata_t::digital_io_t::~digital_io_t() {
}

int32_t firmata_t::digital_io_t::value() {
    if (f_value)
        return m_value;
    m_value = ((224 & lsb()) | ((224 & msb()) << 7));
    f_value = true;
    return m_value;
}

firmata_t::protocol_version_t::protocol_version_t(kaitai::kstream *p_io, firmata_t::message_t *p_parent, firmata_t *p_root) : kaitai::kstruct(p_io) {
    m__parent = p_parent;
    m__root = p_root;
    m_major = m__io->read_u1();
    m_minor = m__io->read_u1();
}

firmata_t::protocol_version_t::~protocol_version_t() {
}
