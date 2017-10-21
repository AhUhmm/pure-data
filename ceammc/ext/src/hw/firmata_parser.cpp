#include "firmata_parser.h"
#include "firmata_proto.h"

extern "C" {
#include "firmata_bison.h"
}

FirmataParser::FirmataParser()
    : parser_state_(0)
    , status_(YYPUSH_MORE)
{
    parser_state_ = firmata_pstate_new();
    msg_.state = STATE_IN_PROCESS;
    msg_.proto_version.major = 0;
    msg_.proto_version.minor = 0;
}

FirmataParser::~FirmataParser()
{
    firmata_pstate_delete(static_cast<firmata_pstate*>(parser_state_));
}

FirmataParser& FirmataParser::operator<<(uint8_t byte)
{
    if (status_ != YYPUSH_MORE || msg_.state != STATE_IN_PROCESS) {
        return *this;
    }

    ParserToken t = token(byte);
    pushToParser(t.type0, t.v0);

    if (status_ == YYPUSH_MORE && t.packed) {
        pushToParser(t.type1, t.v1);
    }

    return *this;
}

void FirmataParser::pushToParser(int type, int ch)
{
    FIRMATA_STYPE t;
    t = msg_;
    t.byte = ch;

    status_ = firmata_push_parse(static_cast<firmata_pstate*>(parser_state_), type, &t, &msg_);
}

bool FirmataParser::isDone() const
{
    return msg_.state == STATE_DONE;
}

bool FirmataParser::isError() const
{
    return msg_.state == STATE_ERROR;
}

void FirmataParser::reset()
{
    if (msg_.state == STATE_ERROR) {
        pushToParser(FIRMATA_TOKEN_ERROR_RESET, 0);
    }

    msg_.state = STATE_IN_PROCESS;
    status_ = YYPUSH_MORE;
}

ParserToken FirmataParser::token(uint8_t ch) const
{
    ParserToken t;
    t.packed = 0;

    switch (ch) {
    case PROTO_PROTOCOL_VERSION:
        t.type0 = FIRMATA_TOKEN_PROTOCOL_VERSION;
        t.v0 = ch;
        return t;
    case PROTO_SET_PIN_MODE:
        t.type0 = FIRMATA_TOKEN_SET_PIN_MODE;
        t.v0 = ch;
        return t;
    // mask types
    default: {
        switch (0xF0 & ch) {
        case PROTO_ANALOG_IO_MESSAGE:
            t.packed = 1;
            t.type0 = FIRMATA_TOKEN_ANALOG_IO;
            t.v0 = (0xF0 & ch);
            t.type1 = FIRMATA_TOKEN_BYTE7;
            t.v1 = (0x0F & ch);
            return t;
        case PROTO_DIGITAL_IO_MESSAGE:
            t.packed = 1;
            t.type0 = FIRMATA_TOKEN_DIGITAL_IO;
            t.v0 = (0xF0 & ch);
            t.type1 = FIRMATA_TOKEN_BYTE7;
            t.v1 = 0x0F & ch;
            return t;
        default:
            t.type0 = FIRMATA_TOKEN_BYTE7;
            t.v0 = ch;
            return t;
        }
    }
    }
}
