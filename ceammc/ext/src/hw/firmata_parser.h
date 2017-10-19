#ifndef FIRMATA_PARSER_H
#define FIRMATA_PARSER_H

#include <iostream>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

class FirmataParser {
public:
    FirmataParser();
    FirmataParser& operator<<(uint8_t byte);
};

#endif // FIRMATA_PARSER_H
