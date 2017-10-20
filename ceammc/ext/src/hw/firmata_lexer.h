#ifndef FIRMATA_LEXER_H
#define FIRMATA_LEXER_H

#include "firmata_proto.h"

#include <string>

struct LexerToken {
    int type0, type1;
    unsigned char v0, v1;
    unsigned char packed;
};

class FirmataLexer {
    void* parser_;
    FirmataParseFn fn_;

public:
    FirmataLexer(void* parser, FirmataParseFn fn);
    void push(unsigned char ch);
    void parse(const std::string& str);

    LexerToken token(unsigned char ch) const;
};

#endif // FIRMATA_LEXER_H
