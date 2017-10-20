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

typedef struct FirmataProtoVersion {
    unsigned char major;
    unsigned char minor;
} FirmataProtoVersion;

typedef struct FirmataMessage {
    enum FirmataProtoCommand command;
    int value;
    unsigned char pin;
    unsigned char byte;
    unsigned char done;
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
