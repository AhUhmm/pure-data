#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>

struct sp_port;

class SerialPort {
public:
    enum Mode {
        NONE = 0,
        READ = 1,
        WRITE = 2,
        READ_WRITE = 3
    };

private:
    SerialPort(const SerialPort&);
    void operator=(const SerialPort&);

public:
    SerialPort();
    SerialPort(const char* name, Mode mode);
    ~SerialPort();

    Mode mode() const;

    const std::string& name() const;
    std::string description() const;

    // usb
    std::string usbManufacturer() const;
    int usbVendorId() const;
    int usbProductId() const;
    int usbBus() const;
    int usbAddr() const;
    const std::string& usbSerial() const;

    bool open(const char* name, Mode mode);
    bool close();

    bool isOpened() const;
    // types
    bool isNative() const;
    bool isUSB() const;
    bool isBluetooth() const;

public:
    static std::string lastErrorMessage();
    static int lastErrorCode();

private:
    void reset();

private:
    sp_port* port_;
    std::string name_;
    std::string usb_serial_;
    int vid_;
    int pid_;
    int usb_bus_;
    int usb_addr_;
    Mode mode_;
};

#endif // SERIALPORT_H
