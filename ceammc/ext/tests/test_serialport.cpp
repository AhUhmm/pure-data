#include "../../extra/libserialport/libserialport.h"
#include "catch.hpp"

#include <iostream>
#include <string>

TEST_CASE("serialport", "[serialport]")
{
    SECTION("version")
    {
        REQUIRE(sp_get_package_version_string() == std::string("0.1.1"));
    }

    SECTION("list")
    {
        sp_port** ports = 0;
        REQUIRE(sp_list_ports(&ports) == SP_OK);
        REQUIRE(ports != 0);

        int n = 0;
        for (sp_port **pp = ports; pp && *pp; pp++, n++) {
            std::cerr << "Port:\n";
            std::cerr << "\tname:  " << sp_get_port_name(*pp) << "\n";
            std::cerr << "\tdescr: " << sp_get_port_description(*pp) << "\n";
            sp_transport t = sp_get_port_transport(*pp);

            char* ba = sp_get_port_bluetooth_address(*pp);
            if (ba)
                std::cerr << "\tbluetooth addr: " << ba << "\n";

            if (t == SP_TRANSPORT_BLUETOOTH)
                std::cerr << "\ttransport:    bluetooth\n";
            if (t == SP_TRANSPORT_NATIVE)
                std::cerr << "\ttransport:    native\n";
            if (t == SP_TRANSPORT_USB) {
                std::cerr << "\ttransport:    usb\n";
                std::cerr << "\tmanufacturer: " << sp_get_port_usb_manufacturer(*pp) << "\n";

                if (sp_get_port_usb_product(*pp))
                    std::cerr << "\tproduct:      " << sp_get_port_usb_product(*pp) << "\n";
                if (sp_get_port_usb_serial(*pp))
                    std::cerr << "\tserial:       " << sp_get_port_usb_serial(*pp) << "\n";

                int bus = 0, addr = 0;
                REQUIRE(sp_get_port_usb_bus_address(*pp, &bus, &addr) == SP_OK);
                std::cerr << "\tbus:          " << bus << "\n";
                std::cerr << "\taddr:         " << addr << "\n";

                int vid = 0, pid = 0;
                REQUIRE(sp_get_port_usb_vid_pid(*pp, &vid, &pid) == SP_OK);
                std::cerr << "\tvid:          " << vid << "\n";
                std::cerr << "\tpid:          " << pid << "\n";
            }
        }

#ifdef __APPLE__
        REQUIRE(n > 0);
#endif

        sp_free_port_list(ports);
        ports = 0;
    }
}
