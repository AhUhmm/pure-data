#ifndef HW_FIRMATA_H
#define HW_FIRMATA_H

#include "ceammc_object.h"

using namespace ceammc;

class Firmata : public BaseObject {
public:
    Firmata(const PdArgs& a);
    void onFloat(t_float f);
};

void hw_firmata_setup();

#endif // HW_FIRMATA_H
