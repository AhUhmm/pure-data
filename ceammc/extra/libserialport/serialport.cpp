#include "serialport.h"

#include <stdexcept>

SerialPortConfig::SerialPortConfig(const SerialPortConfig& cfg)
    : config_(0)
{
    if (sp_new_config(&config_) != SP_OK)
        throw std::runtime_error("can' allocate SerialPortConfig");

    copyFrom(cfg.config_, config_);
}

SerialPortConfig::SerialPortConfig()
    : config_(0)
{
    if (sp_new_config(&config_) != SP_OK)
        throw std::runtime_error("can't allocate SerialPortConfig");
}

SerialPortConfig::SerialPortConfig(sp_port* port)
    : config_(0)
{
    if (sp_new_config(&config_) != SP_OK)
        throw std::runtime_error("can't allocate SerialPortConfig");

    if (port)
        sp_get_config(port, config_);
}

SerialPortConfig::~SerialPortConfig()
{
    if (config_)
        sp_free_config(config_);
}

void SerialPortConfig::operator=(const SerialPortConfig& cfg)
{
    if (this == &cfg)
        return;

    copyFrom(cfg.config_, config_);
}

#define GET_CONFIG_VALUE(value, type, def)      \
    {                                           \
        if (!config_)                           \
            return def;                         \
                                                \
        type value = def;                       \
        sp_get_config_##value(config_, &value); \
        return value;                           \
    }

int SerialPortConfig::baudRate() const
{
    GET_CONFIG_VALUE(baudrate, int, -1);
}

int SerialPortConfig::bits() const
{
    GET_CONFIG_VALUE(bits, int, -1);
}

int SerialPortConfig::stopBits() const
{
    GET_CONFIG_VALUE(stopbits, int, -1);
}

sp_parity SerialPortConfig::parity() const
{
    GET_CONFIG_VALUE(parity, sp_parity, SP_PARITY_INVALID);
}

sp_rts SerialPortConfig::rts() const
{
    GET_CONFIG_VALUE(rts, sp_rts, SP_RTS_INVALID);
}

sp_cts SerialPortConfig::cts() const
{
    GET_CONFIG_VALUE(cts, sp_cts, SP_CTS_INVALID);
}

sp_dtr SerialPortConfig::dtr() const
{
    GET_CONFIG_VALUE(dtr, sp_dtr, SP_DTR_INVALID);
}

sp_dsr SerialPortConfig::dsr() const
{
    GET_CONFIG_VALUE(dsr, sp_dsr, SP_DSR_INVALID);
}

sp_xonxoff SerialPortConfig::xOnxOff() const
{
    GET_CONFIG_VALUE(xon_xoff, sp_xonxoff, SP_XONXOFF_INVALID);
}

#define SET_CONFIG_VALUE(name, value)             \
    {                                             \
        if (config_)                              \
            sp_set_config_##name(config_, value); \
    }

void SerialPortConfig::setBaudRate(int r)
{
    SET_CONFIG_VALUE(baudrate, r);
}

void SerialPortConfig::setBits(int b)
{
    SET_CONFIG_VALUE(bits, b);
}

void SerialPortConfig::setStopBits(int b)
{
    SET_CONFIG_VALUE(stopbits, b);
}

void SerialPortConfig::setParity(sp_parity p)
{
    SET_CONFIG_VALUE(parity, p);
}

void SerialPortConfig::setRts(sp_rts r)
{
    SET_CONFIG_VALUE(rts, r);
}

void SerialPortConfig::setCts(sp_cts c)
{
    SET_CONFIG_VALUE(cts, c);
}

void SerialPortConfig::setDtr(sp_dtr d)
{
    SET_CONFIG_VALUE(dtr, d);
}

void SerialPortConfig::setDsr(sp_dsr d)
{
    SET_CONFIG_VALUE(dsr, d);
}

void SerialPortConfig::setXOnxOff(sp_xonxoff x)
{
    SET_CONFIG_VALUE(xon_xoff, x);
}

#define COPY_CONFIG_VALUE(name, type)                    \
    {                                                    \
        type name;                                       \
        if (sp_get_config_##name(src, &name) == SP_OK) { \
            sp_set_config_##name(dest, name);            \
        }                                                \
    }

void SerialPortConfig::copyFrom(const sp_port_config* src, sp_port_config* dest)
{
    COPY_CONFIG_VALUE(baudrate, int);
    COPY_CONFIG_VALUE(bits, int);
    COPY_CONFIG_VALUE(stopbits, int);

    COPY_CONFIG_VALUE(parity, sp_parity);
    COPY_CONFIG_VALUE(rts, sp_rts);
    COPY_CONFIG_VALUE(cts, sp_cts);
    COPY_CONFIG_VALUE(dtr, sp_dtr);
    COPY_CONFIG_VALUE(dsr, sp_dsr);
    COPY_CONFIG_VALUE(xon_xoff, sp_xonxoff);
}

SerialPort::SerialPort()
    : port_(0)
    , mode_(NONE)
{
}

SerialPort::SerialPort(const char* name, Mode mode)
    : port_(0)
    , mode_(mode)
{
    open(name, mode);
}

SerialPort::~SerialPort()
{
    close();
}

SerialPort::Mode SerialPort::mode() const
{
    return mode_;
}

SerialPortInfo SerialPort::info() const
{
    return SerialPortInfo(port_);
}

std::string SerialPort::name() const
{
    return port_ ? sp_get_port_name(port_) : "";
}

SerialPortInfo::SerialPortInfo(sp_port* p)
    : vid_(-1)
    , pid_(-1)
    , usb_bus_(-1)
    , usb_addr_(-1)
    , transport_(SP_TRANSPORT_NATIVE)
{
    if (!p)
        return;

    char* name = sp_get_port_name(p);
    name_ = (name) ? name : "";

    char* descr = sp_get_port_description(p);
    description_ = (descr) ? descr : "";

    char* mnf = sp_get_port_usb_manufacturer(p);
    usb_manufacturer_ = (mnf) ? mnf : "";

    // get USB vendor and product
    sp_get_port_usb_vid_pid(p, &vid_, &pid_);
    // get USB address
    sp_get_port_usb_bus_address(p, &usb_bus_, &usb_addr_);
    // get USB serial
    char* serial = sp_get_port_usb_serial(p);
    usb_serial_ = (serial) ? serial : "";

    transport_ = sp_get_port_transport(p);
}

const std::string& SerialPortInfo::name() const
{
    return name_;
}

const std::string& SerialPortInfo::description() const
{
    return description_;
}

const std::string& SerialPortInfo::usbSerial() const
{
    return usb_serial_;
}

const std::string& SerialPortInfo::usbManufacturer() const
{
    return usb_manufacturer_;
}

int SerialPortInfo::usbVendorId() const
{
    return vid_;
}

int SerialPortInfo::usbProductId() const
{
    return pid_;
}

int SerialPortInfo::usbBus() const
{
    return usb_bus_;
}

int SerialPortInfo::usbAddr() const
{
    return usb_addr_;
}

bool SerialPortInfo::isNative() const
{
    return transport_ == SP_TRANSPORT_NATIVE;
}

bool SerialPortInfo::isUSB() const
{
    return transport_ == SP_TRANSPORT_USB;
}

bool SerialPortInfo::isBluetooth() const
{
    return transport_ == SP_TRANSPORT_BLUETOOTH;
}

static sp_mode ceammc2sp(SerialPort::Mode mode)
{
    return static_cast<sp_mode>(mode);
}

bool SerialPort::open(const char* name, Mode mode)
{
    // close port
    if (port_)
        close();

    mode_ = mode;

    if (sp_get_port_by_name(name, &port_) != SP_OK) {
        mode_ = NONE;
        return false;
    }

    // try to open
    if (sp_open(port_, ceammc2sp(mode_)) != SP_OK) {
        close();
        return false;
    }

    return true;
}

bool SerialPort::close()
{
    if (!port_) {
        mode_ = NONE;
        return false;
    }

    if (sp_close(port_) != SP_OK) {
        port_ = 0;
        return false;
    }

    sp_free_port(port_);
    port_ = 0;
    mode_ = NONE;
    return true;
}

void SerialPort::setConfig(const SerialPortConfig& cfg)
{
    if (port_)
        sp_set_config(port_, cfg.config_);
}

SerialPortConfig SerialPort::config() const
{
    return SerialPortConfig(port_);
}

std::string SerialPort::lastErrorMessage()
{
    char* msg = sp_last_error_message();
    std::string res(msg);
    sp_free_error_message(msg);
    return res;
}

int SerialPort::lastErrorCode()
{
    return sp_last_error_code();
}

DeviceInfoList SerialPort::listDevices()
{
    DeviceInfoList res;

    sp_port** ports = 0;
    if (sp_list_ports(&ports) == SP_OK) {
        for (sp_port** pp = ports; pp && *pp; ++pp)
            res.push_back(SerialPortInfo(*pp));

        sp_free_port_list(ports);
    }

    return res;
}

bool SerialPort::isOpened() const
{
    return port_ != 0;
}

sp_return SerialPort::write(const std::string& data)
{
    if (!port_)
        return SP_ERR_ARG;

    return sp_blocking_write(port_, data.data(), data.size(), 5);
}

sp_return SerialPort::write(const std::vector<char>& data)
{
    if (!port_)
        return SP_ERR_ARG;

    return sp_blocking_write(port_, data.data(), data.size(), 55);
}

sp_return SerialPort::read(std::vector<char>& data)
{
    if (!port_)
        return SP_ERR_ARG;

    return sp_blocking_read(port_, data.data(), data.size(), 0);
}

bool SerialPort::drain()
{
    if (!port_)
        return false;

    return sp_drain(port_) == SP_OK;
}
