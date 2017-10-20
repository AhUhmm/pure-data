#include "../hw/firmata_lemon.h"
#include "../hw/firmata_lexer.h"
#include "../hw/firmata_proto.h"

#include "catch.hpp"

#include <iostream>

TEST_CASE("firmata_parser", "[firmata]")
{
    SECTION("simple")
    {
        void* pParser = FirmataParseAlloc(malloc);

        FirmataParse(pParser, FIRMATA_PROTOCOL_VERSION, 15);
        FirmataParse(pParser, FIRMATA_BYTE7, 2);
        FirmataParse(pParser, FIRMATA_BYTE7, 5);
        FirmataParse(pParser, 0, 0);

        FirmataParseFree(pParser, free);
    }

    SECTION("manual lexer")
    {
        void* pParser = FirmataParseAlloc(malloc);

        std::string input("\xF9\x02\x05");
        for (size_t i = 0; i < input.size(); i++) {
            unsigned char ch = input[i];
            switch (ch) {
            case PROTO_PROTOCOL_VERSION:
                FirmataParse(pParser, FIRMATA_PROTOCOL_VERSION, ch);
                break;
            default:
                FirmataParse(pParser, FIRMATA_BYTE7, ch);
                break;
            }
        }

        FirmataParse(pParser, 0, 0);

        FirmataParseFree(pParser, free);
    }

    SECTION("lexer")
    {
        void* pParser = FirmataParseAlloc(malloc);

        message()->done = 0;
        FirmataLexer lex(pParser, FirmataParse);
        lex.parse("\xF9\x02\x05");
//        lex.push(0);
//        lex.parse("\xE4\x02\x06");
//        lex.push(0);
//        lex.parse("\x92\x02\x06");
        lex.push(0);
        //

        REQUIRE(message()->done);

        FirmataParseFree(pParser, free);
    }
}
