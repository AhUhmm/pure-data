#ifndef FIRMATA_PROTO_H
#define FIRMATA_PROTO_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

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

enum FirmataPinMode {
    PROTO_PIN_MODE_INPUT = 0,
    PROTO_PIN_MODE_OUTPUT = 1,
    PROTO_PIN_MODE_ANALOG = 2,
    PROTO_PIN_MODE_PWM = 3,
    PROTO_PIN_MODE_SERVO = 4,
    PROTO_PIN_MODE_I2C = 6,
    PROTO_PIN_MODE_ONEWIRE = 7,
    PROTO_PIN_MODE_STEPPER = 8,
    PROTO_PIN_MODE_ENCODER = 9,
    PROTO_PIN_MODE_SERIAL = 10,
    PROTO_PIN_MODE_PULLUP = 11
};

enum FirmataDigitalPinState {
    PROTO_PIN_LOW = 0,
    PROTO_PIN_HIGH = 1
};

enum FirmataParseState {
    STATE_IN_PROCESS = 0,
    STATE_DONE,
    STATE_ERROR
};

typedef struct FirmataProtoVersion {
    unsigned char major;
    unsigned char minor;
} FirmataProtoVersion;

typedef struct FirmataMessage {
    enum FirmataProtoCommand command;
    int value;
    int type;
    enum FirmataPinMode pin_mode;
    unsigned char pin;
    unsigned char byte;
    unsigned char state;
    FirmataProtoVersion proto_version;

} FirmataMessage;

typedef int FirmataParseTOKENTYPE;
typedef void (*FirmataParseFn)(void*, int, FirmataParseTOKENTYPE);

FirmataMessage* message();
void* FirmataParseAlloc(void* (*mallocProc)(size_t));
void FirmataParseFree(void* p, void (*freeProc)(void*));
void FirmataParse(void* yyp, int yymajor, /* The major token code number */
    FirmataParseTOKENTYPE yyminor /* The value for the token */);

#ifdef __cplusplus
}
#endif

#endif // FIRMATA_PROTO_H
