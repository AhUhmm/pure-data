/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#include "string_str.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"

StringStr::StringStr(const PdArgs& a)
    : BaseObject(a)
    , str_(new String(new DataString(positionalArguments())))
{
    createOutlet();
}

void StringStr::onBang()
{
    if (!str_)
        return;

    listTo(0, str_->toAtom());
}

void StringStr::onSymbol(t_symbol* s)
{
    str_->data()->str() = s->s_name;
    onBang();
}

void StringStr::onList(const AtomList& l)
{
    if (l.empty())
        return;

    StringPtr p = String::fromAtom(l[0]);
    if (!p)
        return;

    str_ = p;
    onBang();
}

void StringStr::dump() const
{
    BaseObject::dump();
    OBJ_DBG << "id: " << str_->id();
    OBJ_DBG << str_->data()->str();
}

void StringStr::m_append(t_symbol* s, const AtomList& lst)
{
    str_->data()->str() += to_string(lst, " ");
}

void string_setup()
{
    ObjectFactory<StringStr> obj("string");
    obj.mapFloatToList();
    obj.addAlias("str");
    obj.addMethod("append", &StringStr::m_append);
}
