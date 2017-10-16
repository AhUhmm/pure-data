#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <string>

#include "libserialport.h"

class SerialPort;

class SerialPortConfig {
public:
    SerialPortConfig();
    ~SerialPortConfig();

    SerialPortConfig(const SerialPortConfig&);
    void operator=(const SerialPortConfig&);

    int baudRate() const;
    int bits() const;
    int stopBits() const;
    sp_parity parity() const;
    sp_rts rts() const;
    sp_cts cts() const;
    sp_dtr dtr() const;
    sp_dsr dsr() const;
    sp_xonxoff xOnxOff() const;

    void setBaudRate(int r);
    void setBits(int b);
    void setStopBits(int b);
    void setParity(sp_parity p);
    void setRts(sp_rts r);
    void setCts(sp_cts c);
    void setDtr(sp_dtr d);
    void setDsr(sp_dsr d);
    void setXOnxOff(sp_xonxoff x);

    friend class SerialPort;

private:
    SerialPortConfig(sp_port* port);
    static void copyFrom(const sp_port_config* str, sp_port_config* dest);

private:
    sp_port_config* config_;
};

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

    // setup
    void setConfig(const SerialPortConfig& cfg);
    SerialPortConfig config() const;

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
