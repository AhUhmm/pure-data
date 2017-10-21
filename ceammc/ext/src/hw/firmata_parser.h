#ifndef FIRMATA_PARSER_H
#define FIRMATA_PARSER_H

#include <iostream>
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif

#include "firmata_proto.h"

struct ParserToken {
    int type0, type1;
    int v0, v1;
    unsigned char packed;
};

class FirmataParser {
    void* parser_state_;
    int status_;
    FirmataMessage msg_;

private:
    FirmataParser(const FirmataParser& fp);
    void operator=(const FirmataParser& fp);

public:
    FirmataParser();
    ~FirmataParser();

    FirmataParser& operator<<(uint8_t byte);

    bool isDone() const;
    bool isError() const;
    void reset();

    ParserToken token(uint8_t ch) const;

    FirmataMessage& msg() { return msg_; }
};

#endif // FIRMATA_PARSER_H
