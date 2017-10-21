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
    if (status_ != YYPUSH_MORE && msg_.state != STATE_IN_PROCESS) {
        return *this;
    }

    ParserToken t = token(byte);
    FIRMATA_STYPE type;
    type = msg_;
    type.byte = t.v0;
    status_ = firmata_push_parse(static_cast<firmata_pstate*>(parser_state_), t.type0, &type, &msg_);

    if (status_ == YYPUSH_MORE && t.packed) {
        type = msg_;
        type.byte = t.v1;
        status_ = firmata_push_parse(static_cast<firmata_pstate*>(parser_state_), t.type1, &type, &msg_);
    }

    return *this;
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
    msg_.state = STATE_IN_PROCESS;
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
