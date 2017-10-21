#include "../hw/firmata_parser.h"
#include "../hw/firmata_proto.h"

#include "catch.hpp"

#include <iostream>

TEST_CASE("firmata_parser", "[firmata]")
{
    SECTION("bison")
    {
        FirmataParser p;
        REQUIRE_FALSE(p.isDone());

        // protocol version
        p << '\xF9' << '\x02' << '\x05';
        REQUIRE(p.msg().command == PROTO_PROTOCOL_VERSION);
        REQUIRE(p.msg().proto_version.major == 2);
        REQUIRE(p.msg().proto_version.minor == 5);
        REQUIRE(p.isDone());

        // we should reset
        p << '\xE4' << '\x02' << '\x06';
        REQUIRE(p.msg().command == PROTO_PROTOCOL_VERSION);
        p.reset();

        // analog value
        p << '\xE4' << '\x02' << '\x06';
        REQUIRE(p.msg().command == PROTO_ANALOG_IO_MESSAGE);
        REQUIRE(p.msg().pin == 4);
        REQUIRE(p.msg().value == 770);
        REQUIRE(p.isDone());
        p.reset();

        int n = 1000;
        while (n-- > 0) {
            // digital pin value
            p << '\x92' << '\x03' << '\x06';
            REQUIRE(p.msg().command == PROTO_DIGITAL_IO_MESSAGE);
            REQUIRE(p.msg().pin == 2);
            REQUIRE(p.msg().value == 771);
            REQUIRE(p.isDone());
            p.reset();
            REQUIRE_FALSE(p.isDone());
        }

        p << 'e' << 'r' << 'r' << 'o' << 'r';
        REQUIRE(p.isError());
        REQUIRE_FALSE(p.isDone());

        p.reset();
        REQUIRE_FALSE(p.isError());
        p << '\xF9' << '\x02' << '\x05';
        REQUIRE(p.msg().command == PROTO_PROTOCOL_VERSION);
        REQUIRE(p.msg().proto_version.major == 2);
        REQUIRE(p.msg().proto_version.minor == 5);
        REQUIRE_FALSE(p.isError());
    }
}
