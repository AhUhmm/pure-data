#include "../../extra/libserialport/libserialport.h"
#include "../../extra/libserialport/serialport.h"
#include "catch.hpp"

#include <iostream>
#include <string>

#define REQUIRE_EMPTY_SERIAL(s)                \
    {                                          \
        REQUIRE_FALSE(s.isOpened());           \
        REQUIRE(s.mode() == SerialPort::NONE); \
        REQUIRE(s.name() == "");               \
        SerialPortInfo si = s.info();          \
        REQUIRE(si.isNative());                \
        REQUIRE_FALSE(si.isBluetooth());       \
        REQUIRE_FALSE(si.isUSB());             \
        REQUIRE(si.name() == "");              \
        REQUIRE(si.description() == "");       \
        REQUIRE(si.usbBus() == -1);            \
        REQUIRE(si.usbAddr() == -1);           \
        REQUIRE(si.usbManufacturer() == "");   \
        REQUIRE(si.usbVendorId() == -1);       \
        REQUIRE(si.usbProductId() == -1);      \
        REQUIRE(si.usbSerial() == "");         \
    }

bool findArduino(const SerialPortInfo& i)
{
    if (i.usbManufacturer().size() < 7)
        return false;

    if (i.usbManufacturer().substr(0, 7) == "Arduino")
        return true;

    return false;
}

bool findUSB(const SerialPortInfo& i)
{
    return i.isUSB();
}

bool findAll(const SerialPortInfo& i)
{
    return true;
}

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

    SECTION("set write")
    {
        sp_port** ports = 0;
        REQUIRE(sp_list_ports(&ports) == SP_OK);
        REQUIRE(ports != 0);

        if (ports[0] == 0) {
            std::cerr << "no serial devices found...\n";
            return;
        }

        char* name = sp_get_port_name(ports[2]);
        std::cerr << "\n~~~~~~~\ntrying to open: " << name << "\n";

        sp_port* dev = 0;
        if (sp_get_port_by_name(name, &dev) == SP_OK) {
            std::cerr << "\tok\n";
            unsigned char buf[100];

            REQUIRE(sp_open(dev, SP_MODE_READ_WRITE) == SP_OK);

            REQUIRE(sp_set_baudrate(dev, 57600) == SP_OK);
            REQUIRE(sp_set_stopbits(dev, 1) == SP_OK);
            REQUIRE(sp_set_bits(dev, 8) == SP_OK);
            REQUIRE(sp_set_parity(dev, SP_PARITY_NONE) == SP_OK);

            sleep(2);

            const char* str = "\xF9";
            REQUIRE(sp_blocking_write(dev, str, strlen(str), 1) == strlen(str));
            REQUIRE(sp_blocking_read(dev, buf, 3, 0) == 3);
            REQUIRE(buf[0] == 0xF9);
            REQUIRE(buf[1] == 2);
            REQUIRE(buf[2] == 5);

            REQUIRE(sp_close(dev) == SP_OK);
            sp_free_port(dev);
        } else {
            std::cerr << "can't open: " << name << "\n";
        }

        sp_free_port_list(ports);
    }

    SECTION("serial")
    {
        return;
        SerialPort s;
        REQUIRE_FALSE(s.isOpened());
        REQUIRE_EMPTY_SERIAL(s);

        // double close
        REQUIRE_FALSE(s.close());
        REQUIRE_FALSE(s.isOpened());
        REQUIRE_FALSE(s.close());
        REQUIRE_FALSE(s.isOpened());

        REQUIRE_FALSE(s.open("not-exists", SerialPort::READ));
        REQUIRE(SerialPort::lastErrorCode() != 0);
        REQUIRE(SerialPort::lastErrorMessage() != "");
        REQUIRE_EMPTY_SERIAL(s);
        REQUIRE_FALSE(s.close());
        REQUIRE_EMPTY_SERIAL(s);

        // macosx
        const char* MAC_MODEM = "/dev/cu.Bluetooth-Incoming-Port";
        if (s.open(MAC_MODEM, SerialPort::READ)) {
            REQUIRE(s.isOpened());
            REQUIRE(s.mode() == SerialPort::READ);
            REQUIRE(s.info().description() != "1");
            REQUIRE(s.info().name() != "");
            REQUIRE(s.info().isNative());
            REQUIRE(!s.info().isUSB());
            REQUIRE(!s.info().isBluetooth());
            REQUIRE(s.info().usbAddr() == -1);
            REQUIRE(s.info().usbBus() == -1);
            REQUIRE(s.info().usbManufacturer() == "");
            REQUIRE(s.info().usbSerial() == "");
            REQUIRE(s.info().usbVendorId() == -1);
            REQUIRE(s.info().usbProductId() == -1);

            REQUIRE(s.isOpened());

            SerialPortConfig cfg = s.config();
            REQUIRE(cfg.baudRate() != -1);
            REQUIRE(cfg.bits() != -1);

            cfg.setBaudRate(8912);
            REQUIRE(cfg.baudRate() == 8912);

            if (s.open(MAC_MODEM, SerialPort::WRITE)) {
                REQUIRE(s.isOpened());
                REQUIRE(s.close());
                REQUIRE_FALSE(s.isOpened());
            }

            REQUIRE_FALSE(s.isOpened());
        } else {
            REQUIRE_EMPTY_SERIAL(s);
        }
    }

    SECTION("SerialPortConfig")
    {
        SerialPortConfig cfg;
        REQUIRE(cfg.baudRate() == -1);
        REQUIRE(cfg.bits() == -1);
        REQUIRE(cfg.stopBits() == -1);
        REQUIRE(cfg.parity() == SP_PARITY_INVALID);
        REQUIRE(cfg.cts() == SP_CTS_INVALID);
        REQUIRE(cfg.dsr() == SP_DSR_INVALID);
        REQUIRE(cfg.dtr() == SP_DTR_INVALID);
        REQUIRE(cfg.rts() == SP_RTS_INVALID);
        REQUIRE(cfg.xOnxOff() == SP_XONXOFF_INVALID);

        cfg.setBaudRate(1024);
        REQUIRE(cfg.baudRate() == 1024);

        cfg.setBits(8);
        REQUIRE(cfg.bits() == 8);

        cfg.setStopBits(2048);
        REQUIRE(cfg.stopBits() == 2048);

        cfg.setParity(SP_PARITY_EVEN);
        REQUIRE(cfg.parity() == SP_PARITY_EVEN);

        cfg.setCts(SP_CTS_FLOW_CONTROL);
        REQUIRE(cfg.cts() == SP_CTS_FLOW_CONTROL);

        cfg.setDsr(SP_DSR_FLOW_CONTROL);
        REQUIRE(cfg.dsr() == SP_DSR_FLOW_CONTROL);

        cfg.setDtr(SP_DTR_FLOW_CONTROL);
        REQUIRE(cfg.dtr() == SP_DTR_FLOW_CONTROL);

        cfg.setRts(SP_RTS_ON);
        REQUIRE(cfg.rts() == SP_RTS_ON);

        cfg.setXOnxOff(SP_XONXOFF_INOUT);
        REQUIRE(cfg.xOnxOff() == SP_XONXOFF_INOUT);

        SECTION("copy")
        {
            SerialPortConfig cfg2(cfg);
            REQUIRE(cfg2.baudRate() == 1024);
            REQUIRE(cfg2.bits() == 8);
            REQUIRE(cfg2.stopBits() == 2048);
            REQUIRE(cfg2.parity() == SP_PARITY_EVEN);
            REQUIRE(cfg2.cts() == SP_CTS_FLOW_CONTROL);
            REQUIRE(cfg2.dsr() == SP_DSR_FLOW_CONTROL);
            REQUIRE(cfg2.dtr() == SP_DTR_FLOW_CONTROL);
            REQUIRE(cfg2.rts() == SP_RTS_ON);
            REQUIRE(cfg2.xOnxOff() == SP_XONXOFF_INOUT);
        }

        SECTION("operator=")
        {
            SerialPortConfig cfg2;
            cfg2 = cfg;
            REQUIRE(cfg2.baudRate() == 1024);
            REQUIRE(cfg2.bits() == 8);
            REQUIRE(cfg2.stopBits() == 2048);
            REQUIRE(cfg2.parity() == SP_PARITY_EVEN);
            REQUIRE(cfg2.cts() == SP_CTS_FLOW_CONTROL);
            REQUIRE(cfg2.dsr() == SP_DSR_FLOW_CONTROL);
            REQUIRE(cfg2.dtr() == SP_DTR_FLOW_CONTROL);
            REQUIRE(cfg2.rts() == SP_RTS_ON);
            REQUIRE(cfg2.xOnxOff() == SP_XONXOFF_INOUT);

            // self assign
            cfg2 = cfg2;
            REQUIRE(cfg2.baudRate() == 1024);
            REQUIRE(cfg2.bits() == 8);
            REQUIRE(cfg2.stopBits() == 2048);
            REQUIRE(cfg2.parity() == SP_PARITY_EVEN);
            REQUIRE(cfg2.cts() == SP_CTS_FLOW_CONTROL);
            REQUIRE(cfg2.dsr() == SP_DSR_FLOW_CONTROL);
            REQUIRE(cfg2.dtr() == SP_DTR_FLOW_CONTROL);
            REQUIRE(cfg2.rts() == SP_RTS_ON);
            REQUIRE(cfg2.xOnxOff() == SP_XONXOFF_INOUT);
        }
    }

    SECTION("list devices")
    {
        DeviceInfoList lst = SerialPort::listDevices();

        for (size_t i = 0; i < lst.size(); i++) {
            const SerialPortInfo& n = lst[i];
            SerialPort p(n.name().c_str(), SerialPort::READ);
        }

        lst = SerialPort::findByPred(findArduino);
        if (!lst.empty()) {
            std::cerr << "Arduino found: " << lst[0].name() << "\n";
        }

        lst = SerialPort::findByPred(findUSB);
        if (!lst.empty()) {
            std::cerr << "USB serial found: " << lst[0].name() << "\n";
        }
    }

    SECTION("wine")
    {
#ifdef _WIN32
        SerialPort p("COM1", SerialPort::READ);
        REQUIRE(p.isOpened());
#endif
    }

    SECTION("simple FIRMATA")
    {
        DeviceInfoList lst = SerialPort::findByPred(findArduino);
        if (!lst.empty()) {
            SerialPort p(lst[0].name().c_str(), SerialPort::WRITE);

            SerialPortConfig c = p.config();
            c.setBaudRate(57600);
            p.setConfig(c);

            sleep(3);

            REQUIRE(p.write("\xF9") == 1);
            //            REQUIRE(p.drain());

            std::vector<char> data(3, 0);
            if (p.read(data) != 3) {
                std::cerr << SerialPort::lastErrorMessage() << "\n";
            } else {
                REQUIRE(data[1] == 2);
                REQUIRE(data[2] == 5);
            }
        }
    }
}
