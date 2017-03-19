#include "ceammc.hpp"
#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

static const int MAX_COUNT = 1024;

class ListGenerate : public BaseObject {
    AtomList gen_values_;
    IntProperty* count_;
    bool in_process_;

public:
    ListGenerate(const PdArgs& a)
        : BaseObject(a)
        , count_(0)
        , in_process_(false)
    {
        createInlet();
        createOutlet();
        createOutlet();

        count_ = new IntProperty("@count", 0);
        createProperty(count_);
        parseArguments();

        if (args().size() > 0)
            setCount(atomlistToValue<float>(args(), 0.f));
    }

    bool loopbackDetected()
    {
        if (in_process_)
            OBJ_ERR << "loopback detected... rejected";

        return in_process_;
    }

    void onBang()
    {
        if (loopbackDetected())
            return;

        in_process_ = true;
        gen_values_.clear();

        for (int i = 0; i < count_->value(); i++)
            bangTo(1);

        listTo(0, gen_values_);
        in_process_ = false;
    }

    void onFloat(float v)
    {
        if (loopbackDetected())
            return;

        if (!setCount(v))
            return;

        onBang();
    }

    void onInlet(size_t n, const AtomList& l)
    {
        if (n == 1) {
            gen_values_.append(l);
            return;
        }
    }

private:
    bool setCount(float v)
    {
        if (!math::is_natural(v)) {
            OBJ_ERR << "invalid argument! natural number expected: " << v;
            return false;
        }

        int c = static_cast<int>(v);
        if (c > MAX_COUNT) {
            OBJ_ERR << "count should be less then " << MAX_COUNT << ". Setting to " << MAX_COUNT;
        }

        count_->setValue(std::min(c, MAX_COUNT));
        return true;
    }
};

extern "C" void setup_list0x2egen()
{
    ObjectFactory<ListGenerate> obj("list.gen");
}