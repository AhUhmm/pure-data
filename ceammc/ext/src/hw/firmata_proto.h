enum FirmataProtoCommand {
    PROTO_ANALOG_IO_MESSAGE = 0xE0,
    PROTO_DIGITAL_IO_MESSAGE = 0x90,
    PROTO_REPORT_ANALOG_PIN = 0xC0,
    PROTO_REPORT_DIGITAL_PORT = 0xD0,
    PROTO_START_SYSEX = 0xF0,
    PROTO_SET_PIN_MODE = 0xF4,
    PROTO_SET_DIGITAL_PIN_VALUE = 0xF5,
    PROTO_SYSEX_END = 0xF7,
    PROTO_PROTOCOL_VERSION = 0xF9,
    PROTO_SYSTEM_RESET = 0xFF
};

typedef struct FirmataMessage {
    enum FirmataProtoCommand command;
    int value;
    unsigned char pin;
    unsigned char byte;
    unsigned char proto_maj;
    unsigned char proto_min;
} FirmataMessage;
