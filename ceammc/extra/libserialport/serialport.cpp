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
    , vid_(0)
    , pid_(0)
    , usb_bus_(0)
    , usb_addr_(0)
    , mode_(NONE)
{
}

SerialPort::SerialPort(const char* name, Mode mode)
    : port_(0)
    , name_(name)
    , vid_(0)
    , pid_(0)
    , usb_bus_(0)
    , usb_addr_(0)
    , usb_serial_(0)
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

const std::string& SerialPort::name() const
{
    return name_;
}

std::string SerialPort::description() const
{
    if (!port_)
        return "";

    char* descr = sp_get_port_description(port_);
    return descr ? descr : "";
}

std::string SerialPort::usbManufacturer() const
{
    if (!port_)
        return "";

    char* mnf = sp_get_port_usb_manufacturer(port_);

    return mnf ? mnf : "";
}

int SerialPort::usbVendorId() const
{
    return vid_;
}

int SerialPort::usbProductId() const
{
    return pid_;
}

int SerialPort::usbBus() const
{
    return usb_bus_;
}

int SerialPort::usbAddr() const
{
    return usb_addr_;
}

const std::string& SerialPort::usbSerial() const
{
    return usb_serial_;
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

    name_ = name;
    mode_ = mode;

    if (sp_get_port_by_name(name_.c_str(), &port_) != SP_OK) {
        reset();
        return false;
    }

    // get USB vendor and product
    sp_get_port_usb_vid_pid(port_, &vid_, &pid_);
    // get USB address
    sp_get_port_usb_bus_address(port_, &usb_bus_, &usb_addr_);
    // get USB serial
    char* serial = sp_get_port_usb_serial(port_);
    if (serial)
        usb_serial_ = serial;

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
        reset();
        return false;
    }

    if (sp_close(port_) != SP_OK) {
        reset();
        return false;
    }

    sp_free_port(port_);
    reset();
    return true;
}

bool SerialPort::isNative() const
{
    return !port_ ? false : sp_get_port_transport(port_) == SP_TRANSPORT_NATIVE;
}

bool SerialPort::isUSB() const
{
    return !port_ ? false : sp_get_port_transport(port_) == SP_TRANSPORT_USB;
}

bool SerialPort::isBluetooth() const
{
    return !port_ ? false : sp_get_port_transport(port_) == SP_TRANSPORT_BLUETOOTH;
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

void SerialPort::reset()
{
    port_ = 0;
    name_ = "";
    vid_ = 0;
    pid_ = 0;
    usb_bus_ = 0;
    usb_addr_ = 0;
    usb_serial_ = "";
    mode_ = NONE;
}

bool SerialPort::isOpened() const
{
    return port_ != 0;
}
