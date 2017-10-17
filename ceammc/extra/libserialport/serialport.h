#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <algorithm>
#include <string>
#include <vector>

#include "libserialport.h"

class SerialPort;
class SerialPortInfo;
typedef std::vector<SerialPortInfo> DeviceInfoList;

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

class SerialPortInfo {
public:
    SerialPortInfo(sp_port* p);

    const std::string& name() const;
    const std::string& description() const;

    // usb
    const std::string& usbSerial() const;
    const std::string& usbManufacturer() const;
    int usbVendorId() const;
    int usbProductId() const;
    int usbBus() const;
    int usbAddr() const;

    // types
    bool isNative() const;
    bool isUSB() const;
    bool isBluetooth() const;

private:
    std::string name_;
    std::string description_;
    std::string usb_serial_;
    std::string usb_manufacturer_;
    int vid_;
    int pid_;
    int usb_bus_;
    int usb_addr_;
    sp_transport transport_;
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

    bool open(const char* name, Mode mode);
    bool close();
    bool isOpened() const;

    sp_return write(const std::string& data);
    sp_return write(const std::vector<char>& data);
    sp_return read(std::vector<char>& data);
    bool drain();

    std::string name() const;
    Mode mode() const;

    // info
    SerialPortInfo info() const;

    // setup
    void setConfig(const SerialPortConfig& cfg);
    SerialPortConfig config() const;

public:
    static std::string lastErrorMessage();
    static int lastErrorCode();

    static DeviceInfoList listDevices();

    template <class Pred>
    static DeviceInfoList findByPred(Pred p);

private:
    sp_port* port_;
    Mode mode_;
};

template <class Pred>
DeviceInfoList SerialPort::findByPred(Pred p)
{
    DeviceInfoList out;
    DeviceInfoList all = listDevices();
    std::copy_if(all.begin(), all.end(), std::back_inserter(out), p);
    return out;
}

#endif // SERIALPORT_H
