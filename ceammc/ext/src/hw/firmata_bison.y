%{
#include <stdio.h>

#include "firmata_proto.h"

void yyerror(FirmataMessage* msg, const char* s) {
    fprintf(stderr, "%s\n", s);
    msg->state = STATE_ERROR;
}

#define YYMAXDEPTH 500

%}

%define api.value.type {FirmataMessage}

%define api.pure full
%define api.push-pull push
%define api.token.prefix {FIRMATA_TOKEN_}
%define api.prefix {firmata_}
%parse-param {FirmataMessage *res_msg}
%token
    ANALOG_IO
    DIGITAL_IO
    PROTOCOL_VERSION
    SET_PIN_MODE
    START_SYSEX
    STOP_SYSEX
    ERROR_RESET
    BYTE7;

%%

program     :  message         {
                    *res_msg = $1;
                    res_msg->state = STATE_DONE; }
            |  program message {
                    *res_msg = $2;
                    res_msg->state = STATE_DONE; }
;

message     : ANALOG_IO pin value14 {
                    printf("Analog pin #%d value: %d\n", $2.pin, $3.value);
                    $$.command = PROTO_ANALOG_IO_MESSAGE;
                    $$.pin = $2.pin;
                    $$.value = $3.value;
                }
            | DIGITAL_IO pin value14 {
                    // printf("Digital pin #%d value: %d\n", $2.pin, $3.value);
                    $$.command = PROTO_DIGITAL_IO_MESSAGE;
                    $$.pin = $2.pin;
                    $$.value = $3.value;
                }
            | PROTOCOL_VERSION byte7 byte7 {
                    printf("Protocol version: %d.%d\n", $2.byte, $3.byte);
                    $$.command = PROTO_PROTOCOL_VERSION;
                    $$.proto_version.major = $2.byte;
                    $$.proto_version.minor = $3.byte;
                }
            | SET_PIN_MODE pin byte7 {
                    printf("Set pin mode: %d at pin #%d\n", $2.pin, $3.byte);
                    $$.command = PROTO_SET_PIN_MODE;
                    $$.pin = $2.pin;
                    switch($3.byte) {
                    case PROTO_PIN_MODE_INPUT:
                    case PROTO_PIN_MODE_OUTPUT:
                    case PROTO_PIN_MODE_ANALOG:
                    case PROTO_PIN_MODE_PWM:
                    case PROTO_PIN_MODE_SERVO:
                    case PROTO_PIN_MODE_I2C:
                    case PROTO_PIN_MODE_ONEWIRE:
                    case PROTO_PIN_MODE_STEPPER:
                    case PROTO_PIN_MODE_ENCODER:
                    case PROTO_PIN_MODE_SERIAL:
                    case PROTO_PIN_MODE_PULLUP:
                        $$.pin_mode = $3.byte;
                        break;
                    default:
                        printf("invalid pin mode value: %d\n", $3.byte);
                        $$.pin_mode = PROTO_PIN_MODE_OUTPUT;
                        break;
                    }
                }
            | sysex {}
            | ERROR_RESET   { $$.state = STATE_IN_PROCESS; }
;

pin         : byte7         { $$.pin = (0xF & $1.byte); };
value14     : byte7 byte7   { $$.value = ($1.byte) | ($2.byte << 7); };
byte7       : BYTE7         { $$.byte = (0x7F & $1.byte); };

sysex       : START_SYSEX sysex_body STOP_SYSEX { printf("sysex received...\n"); };
sysex_body  : byte7 {};
