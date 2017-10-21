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
        p << PROTO_PROTOCOL_VERSION << 2 << 5;
        REQUIRE(p.msg().command == PROTO_PROTOCOL_VERSION);
        REQUIRE(p.msg().proto_version.major == 2);
        REQUIRE(p.msg().proto_version.minor == 5);
        REQUIRE(p.isDone());

        // we should reset parser
        p << PROTO_ANALOG_IO_MESSAGE << 2 << 6;
        REQUIRE(p.msg().command == PROTO_PROTOCOL_VERSION);
        p.reset();

        // analog value
        p << (PROTO_ANALOG_IO_MESSAGE | 4) << '\x02' << '\x06';
        REQUIRE(p.msg().command == PROTO_ANALOG_IO_MESSAGE);
        REQUIRE(p.msg().pin == 4);
        REQUIRE(p.msg().value == 770);
        REQUIRE(p.isDone());
        p.reset();

        // STACK overflow test
        // should not overflow because of left recursion used
        // defualt stack size == 500
        int n = 1000;
        while (n-- > 0) {
            // digital pin value
            p << (PROTO_DIGITAL_IO_MESSAGE | 2) << '\x03' << '\x06';
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

        p << PROTO_PROTOCOL_VERSION << '\x02' << '\x05';
        REQUIRE(p.msg().command == PROTO_PROTOCOL_VERSION);
        REQUIRE(p.msg().proto_version.major == 2);
        REQUIRE(p.msg().proto_version.minor == 5);
        REQUIRE_FALSE(p.isError());

        p.reset();

        // set pin mode
        p << PROTO_SET_PIN_MODE << 8 << PROTO_PIN_MODE_ONEWIRE;
        REQUIRE(p.isDone());
        REQUIRE(p.msg().pin == 8);
        REQUIRE(p.msg().pin_mode == PROTO_PIN_MODE_ONEWIRE);

        p.reset();

        // set pin value
        p << PROTO_SET_DIGITAL_PIN_VALUE << 10 << PROTO_PIN_HIGH;
        REQUIRE(p.isDone());
        REQUIRE(p.msg().pin == 10);
        REQUIRE(p.msg().value == PROTO_PIN_HIGH);
    }
}
