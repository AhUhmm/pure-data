#ifndef FIRMATA_H_
#define FIRMATA_H_

// This is a generated file! Please edit source .ksy file and use kaitai-struct-compiler to rebuild

#include <kaitai/kaitaistruct.h>
#include <kaitai/kaitaistream.h>

#include <stdint.h>
#include <vector>
#include <sstream>

#if KAITAI_STRUCT_VERSION < 7000L
#error "Incompatible Kaitai Struct C++/STL API: version 0.7 or later is required"
#endif

class firmata_t : public kaitai::kstruct {

public:
    class extended_analog_t;
    class sysex_t;
    class report_firmware_t;
    class capability_query_t;
    class sysex_body_t;
    class set_digital_pin_value_t;
    class sysex_ext_t;
    class report_digital_port_t;
    class analog_io_t;
    class pin_capability_list_t;
    class string_bit7_t;
    class sampling_interval_t;
    class capability_response_t;
    class report_analog_pin_t;
    class set_pin_mode_t;
    class message_t;
    class digital_io_t;
    class protocol_version_t;

    enum pin_modes_t {
        PIN_MODES_INPUT = 0,
        PIN_MODES_OUTPUT = 1,
        PIN_MODES_ANALOG = 2,
        PIN_MODES_PWM = 3,
        PIN_MODES_SERVO = 4,
        PIN_MODES_I2C = 6,
        PIN_MODES_ONEWIRE = 7,
        PIN_MODES_STEPPER = 8,
        PIN_MODES_ENCODER = 9,
        PIN_MODES_SERIAL = 10,
        PIN_MODES_PULLUP = 11
    };

    firmata_t(kaitai::kstream* p_io, kaitai::kstruct* p_parent = 0, firmata_t* p_root = 0);
    ~firmata_t();

    class extended_analog_t : public kaitai::kstruct {

    public:

        extended_analog_t(kaitai::kstream* p_io, firmata_t::sysex_body_t* p_parent = 0, firmata_t* p_root = 0);
        ~extended_analog_t();

    private:
        uint8_t m_pin;
        string_bit7_t* m_value;
        firmata_t* m__root;
        firmata_t::sysex_body_t* m__parent;

    public:
        uint8_t pin() const { return m_pin; }
        string_bit7_t* value() const { return m_value; }
        firmata_t* _root() const { return m__root; }
        firmata_t::sysex_body_t* _parent() const { return m__parent; }
    };

    class sysex_t : public kaitai::kstruct {

    public:

        sysex_t(kaitai::kstream* p_io, firmata_t::message_t* p_parent = 0, firmata_t* p_root = 0);
        ~sysex_t();

    private:
        uint8_t m_id;
        sysex_body_t* m_body;
        firmata_t* m__root;
        firmata_t::message_t* m__parent;

    public:
        uint8_t id() const { return m_id; }
        sysex_body_t* body() const { return m_body; }
        firmata_t* _root() const { return m__root; }
        firmata_t::message_t* _parent() const { return m__parent; }
    };

    class report_firmware_t : public kaitai::kstruct {

    public:

        report_firmware_t(kaitai::kstream* p_io, firmata_t::sysex_body_t* p_parent = 0, firmata_t* p_root = 0);
        ~report_firmware_t();

    private:
        uint8_t m_major;
        uint8_t m_minor;
        string_bit7_t* m_str;
        firmata_t* m__root;
        firmata_t::sysex_body_t* m__parent;

    public:
        uint8_t major() const { return m_major; }
        uint8_t minor() const { return m_minor; }
        string_bit7_t* str() const { return m_str; }
        firmata_t* _root() const { return m__root; }
        firmata_t::sysex_body_t* _parent() const { return m__parent; }
    };

    /**
     * Request for capability
     */

    class capability_query_t : public kaitai::kstruct {

    public:

        capability_query_t(kaitai::kstream* p_io, kaitai::kstruct* p_parent = 0, firmata_t* p_root = 0);
        ~capability_query_t();

    private:
        firmata_t* m__root;
        kaitai::kstruct* m__parent;

    public:
        firmata_t* _root() const { return m__root; }
        kaitai::kstruct* _parent() const { return m__parent; }
    };

    class sysex_body_t : public kaitai::kstruct {

    public:

        sysex_body_t(kaitai::kstream* p_io, firmata_t::sysex_t* p_parent = 0, firmata_t* p_root = 0);
        ~sysex_body_t();

    private:
        kaitai::kstruct* m_body;
        firmata_t* m__root;
        firmata_t::sysex_t* m__parent;

    public:
        kaitai::kstruct* body() const { return m_body; }
        firmata_t* _root() const { return m__root; }
        firmata_t::sysex_t* _parent() const { return m__parent; }
    };

    class set_digital_pin_value_t : public kaitai::kstruct {

    public:

        set_digital_pin_value_t(kaitai::kstream* p_io, firmata_t::message_t* p_parent = 0, firmata_t* p_root = 0);
        ~set_digital_pin_value_t();

    private:
        uint8_t m_pin;
        uint8_t m_value;
        firmata_t* m__root;
        firmata_t::message_t* m__parent;

    public:
        uint8_t pin() const { return m_pin; }
        uint8_t value() const { return m_value; }
        firmata_t* _root() const { return m__root; }
        firmata_t::message_t* _parent() const { return m__parent; }
    };

    class sysex_ext_t : public kaitai::kstruct {

    public:

        sysex_ext_t(kaitai::kstream* p_io, firmata_t::sysex_body_t* p_parent = 0, firmata_t* p_root = 0);
        ~sysex_ext_t();

    private:
        bool f_id;
        int32_t m_id;

    public:
        int32_t id();

    private:
        uint8_t m_lsb;
        uint8_t m_msb;
        string_bit7_t* m_body;
        firmata_t* m__root;
        firmata_t::sysex_body_t* m__parent;

    public:
        uint8_t lsb() const { return m_lsb; }
        uint8_t msb() const { return m_msb; }
        string_bit7_t* body() const { return m_body; }
        firmata_t* _root() const { return m__root; }
        firmata_t::sysex_body_t* _parent() const { return m__parent; }
    };

    class report_digital_port_t : public kaitai::kstruct {

    public:

        report_digital_port_t(kaitai::kstream* p_io, firmata_t::message_t* p_parent = 0, firmata_t* p_root = 0);
        ~report_digital_port_t();

    private:
        uint8_t m_state;
        firmata_t* m__root;
        firmata_t::message_t* m__parent;

    public:
        uint8_t state() const { return m_state; }
        firmata_t* _root() const { return m__root; }
        firmata_t::message_t* _parent() const { return m__parent; }
    };

    class analog_io_t : public kaitai::kstruct {

    public:

        analog_io_t(kaitai::kstream* p_io, firmata_t::message_t* p_parent = 0, firmata_t* p_root = 0);
        ~analog_io_t();

    private:
        bool f_value;
        int32_t m_value;

    public:
        int32_t value();

    private:
        uint8_t m_lsb;
        uint8_t m_msb;
        firmata_t* m__root;
        firmata_t::message_t* m__parent;

    public:
        uint8_t lsb() const { return m_lsb; }
        uint8_t msb() const { return m_msb; }
        firmata_t* _root() const { return m__root; }
        firmata_t::message_t* _parent() const { return m__parent; }
    };

    class pin_capability_list_t : public kaitai::kstruct {

    public:
        class pin_capability_t;

        pin_capability_list_t(kaitai::kstream* p_io, kaitai::kstruct* p_parent = 0, firmata_t* p_root = 0);
        ~pin_capability_list_t();

        class pin_capability_t : public kaitai::kstruct {

        public:

            pin_capability_t(kaitai::kstream* p_io, kaitai::kstruct* p_parent = 0, firmata_t* p_root = 0);
            ~pin_capability_t();

        private:
            uint8_t m_mode;
            uint8_t m_resolution;
            firmata_t* m__root;
            kaitai::kstruct* m__parent;

        public:
            uint8_t mode() const { return m_mode; }
            uint8_t resolution() const { return m_resolution; }
            firmata_t* _root() const { return m__root; }
            kaitai::kstruct* _parent() const { return m__parent; }
        };

    private:
        std::vector<pin_capability_t*>* m_modes;
        firmata_t* m__root;
        kaitai::kstruct* m__parent;

    public:
        std::vector<pin_capability_t*>* modes() const { return m_modes; }
        firmata_t* _root() const { return m__root; }
        kaitai::kstruct* _parent() const { return m__parent; }
    };

    class string_bit7_t : public kaitai::kstruct {

    public:

        string_bit7_t(kaitai::kstream* p_io, kaitai::kstruct* p_parent = 0, firmata_t* p_root = 0);
        ~string_bit7_t();

    private:
        std::vector<uint8_t>* m_b;
        firmata_t* m__root;
        kaitai::kstruct* m__parent;

    public:
        std::vector<uint8_t>* b() const { return m_b; }
        firmata_t* _root() const { return m__root; }
        kaitai::kstruct* _parent() const { return m__parent; }
    };

    class sampling_interval_t : public kaitai::kstruct {

    public:

        sampling_interval_t(kaitai::kstream* p_io, firmata_t::sysex_body_t* p_parent = 0, firmata_t* p_root = 0);
        ~sampling_interval_t();

    private:
        bool f_interval;
        int32_t m_interval;

    public:
        int32_t interval();

    private:
        uint8_t m_lsb;
        uint8_t m_msb;
        firmata_t* m__root;
        firmata_t::sysex_body_t* m__parent;

    public:
        uint8_t lsb() const { return m_lsb; }
        uint8_t msb() const { return m_msb; }
        firmata_t* _root() const { return m__root; }
        firmata_t::sysex_body_t* _parent() const { return m__parent; }
    };

    class capability_response_t : public kaitai::kstruct {

    public:

        capability_response_t(kaitai::kstream* p_io, firmata_t::sysex_body_t* p_parent = 0, firmata_t* p_root = 0);
        ~capability_response_t();

    private:
        uint8_t m_b;
        firmata_t* m__root;
        firmata_t::sysex_body_t* m__parent;

    public:
        uint8_t b() const { return m_b; }
        firmata_t* _root() const { return m__root; }
        firmata_t::sysex_body_t* _parent() const { return m__parent; }
    };

    class report_analog_pin_t : public kaitai::kstruct {

    public:

        report_analog_pin_t(kaitai::kstream* p_io, firmata_t::message_t* p_parent = 0, firmata_t* p_root = 0);
        ~report_analog_pin_t();

    private:
        uint8_t m_state;
        firmata_t* m__root;
        firmata_t::message_t* m__parent;

    public:
        uint8_t state() const { return m_state; }
        firmata_t* _root() const { return m__root; }
        firmata_t::message_t* _parent() const { return m__parent; }
    };

    class set_pin_mode_t : public kaitai::kstruct {

    public:

        set_pin_mode_t(kaitai::kstream* p_io, firmata_t::message_t* p_parent = 0, firmata_t* p_root = 0);
        ~set_pin_mode_t();

    private:
        uint8_t m_pin;
        pin_modes_t m_mode;
        firmata_t* m__root;
        firmata_t::message_t* m__parent;

    public:
        uint8_t pin() const { return m_pin; }
        pin_modes_t mode() const { return m_mode; }
        firmata_t* _root() const { return m__root; }
        firmata_t::message_t* _parent() const { return m__parent; }
    };

    class message_t : public kaitai::kstruct {

    public:

        message_t(kaitai::kstream* p_io, firmata_t* p_parent = 0, firmata_t* p_root = 0);
        ~message_t();

    private:
        bool f_event_type;
        int32_t m_event_type;

    public:
        int32_t event_type();

    private:
        bool f_channel;
        int32_t m_channel;

    public:
        int32_t channel();

    private:
        uint8_t m_message_header;
        sysex_t* m_sysex;
        bool n_sysex;

    public:
        bool _is_null_sysex() { sysex(); return n_sysex; };

    private:
        kaitai::kstruct* m_message_body;
        bool n_message_body;

    public:
        bool _is_null_message_body() { message_body(); return n_message_body; };

    private:
        firmata_t* m__root;
        firmata_t* m__parent;

    public:
        uint8_t message_header() const { return m_message_header; }

        /**
         * et
         */
        sysex_t* sysex() const { return m_sysex; }
        kaitai::kstruct* message_body() const { return m_message_body; }
        firmata_t* _root() const { return m__root; }
        firmata_t* _parent() const { return m__parent; }
    };

    class digital_io_t : public kaitai::kstruct {

    public:

        digital_io_t(kaitai::kstream* p_io, firmata_t::message_t* p_parent = 0, firmata_t* p_root = 0);
        ~digital_io_t();

    private:
        bool f_value;
        int32_t m_value;

    public:
        int32_t value();

    private:
        uint8_t m_lsb;
        uint8_t m_msb;
        firmata_t* m__root;
        firmata_t::message_t* m__parent;

    public:
        uint8_t lsb() const { return m_lsb; }
        uint8_t msb() const { return m_msb; }
        firmata_t* _root() const { return m__root; }
        firmata_t::message_t* _parent() const { return m__parent; }
    };

    class protocol_version_t : public kaitai::kstruct {

    public:

        protocol_version_t(kaitai::kstream* p_io, firmata_t::message_t* p_parent = 0, firmata_t* p_root = 0);
        ~protocol_version_t();

    private:
        uint8_t m_major;
        uint8_t m_minor;
        firmata_t* m__root;
        firmata_t::message_t* m__parent;

    public:
        uint8_t major() const { return m_major; }
        uint8_t minor() const { return m_minor; }
        firmata_t* _root() const { return m__root; }
        firmata_t::message_t* _parent() const { return m__parent; }
    };

private:
    std::vector<message_t*>* m_message;
    firmata_t* m__root;
    kaitai::kstruct* m__parent;

public:
    std::vector<message_t*>* message() const { return m_message; }
    firmata_t* _root() const { return m__root; }
    kaitai::kstruct* _parent() const { return m__parent; }
};

#endif  // FIRMATA_H_
