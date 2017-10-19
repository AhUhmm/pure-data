%include {
#include <assert.h>
#include "firmata_lemon.h"
#include "firmata_proto.h"

struct FirmataMessage message_;

void output_message(FirmataMessage* msg) {

}
}

%token_type {int}
%token_prefix FIRMATA_

%syntax_error {
/*    std::cout << "Syntax error!" << std::endl;*/
}

program ::= message.   { output_message(&message_); }


message    ::= ANALOG_IO pin(B) value14(C).        {
    message_.command = PROTO_ANALOG_IO_MESSAGE;
    message_.pin = B;
    message_.value = C;
}

message    ::= DIGITAL_IO pin(B) value14(C).      {
    message_.command = PROTO_DIGITAL_IO_MESSAGE;
    message_.pin = B;
    message_.value = C;
}

message    ::= REPORT_ANALOG pin switch.   { ; }
message    ::= REPORT_DIGITAL pin switch. { ; }
message    ::= SET_PIN_MODE pin pin_mode.  { ; }

message    ::= SET_DPIN_VALUE pin(B) switch(C). {
    message_.command = PROTO_SET_DIGITAL_PIN_VALUE;
    message_.pin = B;
    message_.value = C;
}

message    ::= PROTOCOL_VERSION protocol_version. { ; }
message    ::= sysex.                      { ; }
message    ::= SYSEM_RESET.                { ; }

value14(A) ::= byte7(LSB) byte7(MSB). { A = (LSB | (MSB << 7)); }
pin(A)     ::= BYTE7(B).   { A = (0xF & B); }
switch(A)     ::= ON.  { A = 1; }
switch(A)     ::= OFF. { A = 0; }
byte7(A)   ::= BYTE7(B). { A = (0x7F & B); }
pin_mode   ::= INPUT. { ; }
pin_mode   ::= OUTPUT. { ; }
pin_mode   ::= ANALOG. { ; }
pin_mode   ::= PWM. { ; }
pin_mode   ::= SERVO. { ; }
pin_mode   ::= I2C. { ; }
pin_mode   ::= ONEWIRE. { ; }
pin_mode   ::= STEPPER. { ; }
pin_mode   ::= ENCODER. { ; }
pin_mode   ::= SERIAL. { ; }
pin_mode   ::= PULLUP. { ; }

protocol_version ::= byte7(MAJ) byte7(MINOR). {
    message_.command = PROTO_PROTOCOL_VERSION;
    message_.proto_maj = MAJ;
    message_.proto_min = MINOR;
}

sysex      ::= START_SYSEX sysex_body STOP_SYSEX. {}
sysex_body ::= SYSEX_ID . {}
