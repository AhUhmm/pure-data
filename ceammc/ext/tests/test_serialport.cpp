#include "serial/serial.h"

#include "catch.hpp"

#include <iostream>
#include <string>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

using namespace serial;

static std::string findArduino(const std::vector<PortInfo>& lst)
{
    for (size_t i = 0; i < lst.size(); i++) {
        if (lst[i].description.size() >= 7 && lst[i].description.substr(0, 7) == "Arduino")
            return lst[i].port;
    }

    return "";
}

TEST_CASE("serial", "[serial]")
{
    SECTION("list_ports")
    {
        std::vector<PortInfo> lst = list_ports();

        for (size_t i = 0; i < lst.size(); i++) {
            PortInfo pi = lst[i];
            REQUIRE(pi.hardware_id != "");
            REQUIRE(pi.description != "");
            REQUIRE(pi.port != "");

            std::cerr << "\tport:        " << pi.port << "\n";
            std::cerr << "\tdescription: " << pi.description << "\n";
            std::cerr << "\tid:          " << pi.hardware_id << "\n";
        }
    }

    SECTION("write")
    {
        std::string port = findArduino(list_ports());
        if (port.size() > 0) {
            std::cerr << "Arduino found: " << port << "\n";

            Serial arduino(port, 57600);
            REQUIRE(arduino.isOpen());
            arduino.setDTR(false);
            arduino.setStopbits(stopbits_one);
            arduino.setTimeout(Timeout::max(), 10, 0, 10, 0);

            // waiting for firmata handshake
            std::string res;
            uint8_t chr = 0;

            sleep(4);

            while (arduino.read(&chr, 1) == 1) {
                if (chr == 0xf9) {
                    uint8_t buf[2];
                    while (arduino.read(buf, 2) != 2)
                        ;

                    std::cerr << "Firmata version:  " << (int)buf[0] << "." << (int)buf[1] << "\n";
                }

                if (chr == 0xf0) {
                    std::vector<uint8_t> sysex;
                    uint8_t ch;
                    while (arduino.read(&ch, 1) == 1) {
                        if (ch == 0xf7)
                            break;

                        sysex.push_back(ch);
                    }

                    if (sysex.at(0) == 0x79) {
                        std::cerr << "Firmware: " << (int)sysex[1] << "." << (int)sysex[2] << "\n";

                        std::string f;
                        for (size_t i = 3; i < sysex.size(); i += 2) {
                            f.push_back(sysex[i]);
                        }

                        std::cerr << "str: " << f << "\n";
                    }

                    //                    std::cerr << "Sysex: " <<  (char*)sysex.data() << "\n";
                }
            }

            std::cerr << "\n";

            uint8_t set_mode[3] = { 0xF4, 13, 1 };
            REQUIRE(arduino.write(set_mode, 3) == 3);

            uint8_t cmd[3] = { 0xF5, 13, 0 };
            for (size_t i = 0; i < 10; i++) {
                cmd[2] = i % 2;
                arduino.write(cmd, 3);
                sleep(1);
            }
        }
    }

    SECTION("wine")
    {
#ifdef _WIN32
        Serial p("COM1");
        REQUIRE(p.isOpen());
#endif
    }
}
