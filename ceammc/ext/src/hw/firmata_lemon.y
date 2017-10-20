%include {
#include <assert.h>
#include "firmata_lemon.h"
#include "firmata_proto.h"

struct FirmataMessage message_;

void output_message(FirmataMessage* msg) {
    printf("message parsed: %d.%d\n", msg->proto_version.major, msg->proto_version.minor);
}

void output_analog_value(FirmataMessage* msg) {
    printf("analog value: %d at pin #%d\n", msg->value, msg->pin);
}

void output_digital_value(FirmataMessage* msg) {
    printf("digital value: %d at pin #%d\n", msg->value, msg->pin);
}

FirmataMessage* message() {
    return &message_;
}
}

%name FirmataParse
%token_prefix FIRMATA_
%token_type            {int}
%default_type          {int}
%type byte7            {int}
%type protocol_version {FirmataProtoVersion}
%parse_accept {
    printf("parse_accept: OK\n\n");
}


%syntax_error {
/*    std::cout << "Syntax error!" << std::endl;*/
}

program ::= message.   { printf("Hi, there!!!\n"); }


message    ::= ANALOG_IO pin(B) value14(C).        {
    message_.command = PROTO_ANALOG_IO_MESSAGE;
    message_.pin = B;
    message_.value = C;
    message_.done = 1;
    output_analog_value(&message_);
}

message    ::= DIGITAL_IO pin(B) value14(C).      {
    message_.command = PROTO_DIGITAL_IO_MESSAGE;
    message_.pin = B;
    message_.value = C;
    message_.done = 1;
    output_digital_value(&message_);
}

message    ::= REPORT_ANALOG pin switch.   { ; }
message    ::= REPORT_DIGITAL pin switch. { ; }
message    ::= SET_PIN_MODE pin pin_mode.  { ; }

message    ::= SET_DPIN_VALUE pin(B) switch(C). {
    message_.command = PROTO_SET_DIGITAL_PIN_VALUE;
    message_.pin = B;
    message_.value = C;
    message_.done = 1;
}

message    ::= PROTOCOL_VERSION protocol_version(A). {
    message_.command = PROTO_PROTOCOL_VERSION;
    message_.proto_version = A;
    output_message(&message_);
    message_.done = 1;
}

message    ::= sysex.                 { ; }
message    ::= SYSTEM_RESET.          { ; }

value14(A) ::= byte7(LSB) byte7(MSB). { A = (LSB | (MSB << 7)); }
pin(A)     ::= BYTE7(B).              { A = (0xF & B); }
switch(A)     ::= ON.                 { A = 1; }
switch(A)     ::= OFF.                { A = 0; }
byte7(A)      ::= BYTE7(B).           { A = (0x7F & B); }
pin_mode(A)   ::= INPUT.              { A = 0; }
pin_mode(A)   ::= OUTPUT.             { A = 1; }
pin_mode(A)   ::= ANALOG.             { A = 2; }
pin_mode(A)   ::= PWM.                { A = 3; }
pin_mode(A)   ::= SERVO.              { A = 4; }
pin_mode(A)   ::= I2C.                { A = 6; }
pin_mode(A)   ::= ONEWIRE.            { A = 7; }
pin_mode(A)   ::= STEPPER.            { A = 8; }
pin_mode(A)   ::= ENCODER.            { A = 9; }
pin_mode(A)   ::= SERIAL.             { A = 10; }
pin_mode(A)   ::= PULLUP.             { A = 11; }

protocol_version(A) ::= byte7(MAJ) byte7(MINOR). {
    printf("protocol_version...\n");
    A.major = MAJ;
    A.minor = MINOR;
}

sysex      ::= START_SYSEX sysex_body STOP_SYSEX. {}
sysex_body ::= SYSEX_ID . {}
