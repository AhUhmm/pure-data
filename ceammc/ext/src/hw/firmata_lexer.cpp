#include "firmata_lexer.h"
#include "firmata_lemon.h"

FirmataLexer::FirmataLexer(void* parser, FirmataParseFn fn)
    : parser_(parser)
    , fn_(fn)
{
}

void FirmataLexer::push(unsigned char ch)
{
    LexerToken t = token(ch);

    fn_(parser_, t.type0, t.v0);
    if (t.packed) {
        fn_(parser_, t.type1, t.v1);
    }
}

void FirmataLexer::parse(const std::string& str)
{
    for (size_t i = 0; i < str.size(); i++)
        push(str[i]);
}

LexerToken FirmataLexer::token(unsigned char ch) const
{
    LexerToken t;
    t.packed = 0;
    t.v0 = ch;
    t.type1 = FIRMATA_BYTE7;

    switch (ch) {
    case PROTO_PROTOCOL_VERSION:
        t.type0 = FIRMATA_PROTOCOL_VERSION;
        break;
    default: {
        switch (0xF0 & ch) {
        case PROTO_ANALOG_IO_MESSAGE:
            t.packed = 1;
            t.type0 = FIRMATA_ANALOG_IO;
            t.v0 = 0xF0 & ch;
            t.type1 = FIRMATA_BYTE7;
            t.v1 = 0x0F & ch;
            break;
        case PROTO_DIGITAL_IO_MESSAGE:
            t.packed = 1;
            t.type0 = FIRMATA_DIGITAL_IO;
            t.v0 = 0xF0 & ch;
            t.type1 = FIRMATA_BYTE7;
            t.v1 = 0x0F & ch;
            break;
        default:
            t.type0 = FIRMATA_BYTE7;
            break;
        }
    }
    }

    return t;
}
