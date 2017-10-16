#include "hw_firmata.h"
#include "ceammc_factory.h"
#include "firmata.h"

#include <cmath>

typedef unsigned char byte;

Firmata::Firmata(const PdArgs& a)
    : BaseObject(a)
{
    createOutlet();
}

void Firmata::onFloat(t_float f)
{
    byte v = static_cast<byte>(roundf(f));
}

void hw_firmata_setup()
{
    ObjectFactory<Firmata> obj("hw.firmata");
}
