#include "ceammc_music_theory_alteration.h"

using namespace ceammc::music;

static const char* ALTERATION_SHORT_NAMES[] = {
    "bb", "b", "", "#", "##"
};

static const char* ALTERATION_FULL_NAMES[] = {
    "double flat", "flat", "natural", "sharp", "double sharp"
};

const Alteration Alteration::DOUBLE_FLAT(ALTERATION_DOUBLE_FLAT);
const Alteration Alteration::FLAT(ALTERATION_FLAT);
const Alteration Alteration::NATURAL(ALTERATION_NATURAL);
const Alteration Alteration::SHARP(ALTERATION_SHARP);
const Alteration Alteration::DOUBLE_SHARP(ALTERATION_DOUBLE_SHARP);

std::ostream& ceammc::music::operator<<(std::ostream& os, const Alteration& a)
{
    os << a.shortName();
    return os;
}

Alteration::Alteration(const Alteration& a)
    : value_(a.value_)
{
}

bool Alteration::operator++()
{
    if (value_ == ALTERATION_DOUBLE_SHARP)
        return false;

    value_++;
    return true;
}

bool Alteration::operator--()
{
    if (value_ == ALTERATION_DOUBLE_FLAT)
        return false;

    value_--;
    return true;
}

bool Alteration::alterate(int n)
{
    if (n == 0)
        return true;

    if ((value_ + n) < ALTERATION_DOUBLE_FLAT || (value_ + n) > ALTERATION_DOUBLE_SHARP)
        return false;

    value_ += n;
    return true;
}

const char* Alteration::fullName() const
{
    return ALTERATION_FULL_NAMES[value_ - ALTERATION_DOUBLE_FLAT];
}

const char* Alteration::shortName() const
{
    return ALTERATION_SHORT_NAMES[value_ - ALTERATION_DOUBLE_FLAT];
}

AlterationType Alteration::type() const
{
    return AlterationType(value_);
}

int Alteration::semitones() const
{
    return value_;
}

double Alteration::cents() const
{
    return value_ * 100;
}

Alteration::Alteration(signed char t)
    : value_(t)
{
}
