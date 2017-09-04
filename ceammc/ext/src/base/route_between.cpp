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
#include "route_between.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"

static bool is_sorted(const std::vector<t_float>& v)
{
    t_float cur;

    if (v.empty())
        return false;

    cur = v[0];
    for (size_t i = 1; i < v.size(); i++) {
        if (cur > v[i])
            return false;

        cur = v[i];
    }

    return true;
}

RouteBetween::RouteBetween(const PdArgs& a)
    : BaseObject(a)
{
    const AtomList& pos_args = positionalArguments();
    for (size_t i = 0; i < pos_args.size(); i++) {
        if (pos_args[i].isFloat()) {
            createOutlet();
            args_.push_back(pos_args[i].asFloat());
        }
    }

    if (!is_sorted(args_)) {
        OBJ_ERR << "unsorted list" << pos_args;
    }
}

void RouteBetween::onFloat(t_float f)
{
    if (args_.empty())
        return;

    for (size_t i = 0; i < args_.size() - 1; i++) {
        if (args_[i] <= f && f < args_[i + 1]) {
            floatTo(i, f);
            return;
        }
    }

    floatTo(numOutlets() - 1, f);
}

void setup_route_between()
{
    ObjectFactory<RouteBetween> obj("route.between");
}
