#include "serialport.h"
#include "libserialport.h"

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
    : name_(name)
    , port_(0)
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
