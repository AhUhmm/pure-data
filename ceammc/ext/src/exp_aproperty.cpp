//
//  exp_aproperty.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 20/01/17.
//
//

#include "oop_common.h"

#define OBJ_NAME "exp.aproperty"

t_eclass* exp_method_class;

typedef ceammc::GlobalData<ceammc::AtomList> LocalList;

using namespace ceammc;

struct t_exp_method {
    t_ebox e_box;
    
    t_canvas *parent_canvas;
    
    t_symbol *property_name;
    
    OPInstance *instance;
    
    t_outlet *out1, *out2, *out3;
};

static void *exp_method_new(t_symbol *id, int argc, t_atom *argv)
    {
        
        if (argc<1)
        {
            error("missing method name argument");
            return 0;
        }
        
        t_exp_method* x = reinterpret_cast<t_exp_method*>(eobj_new(exp_method_class));
        x->parent_canvas = canvas_getcurrent();
        
        t_binbuf* d = binbuf_via_atoms(argc,argv);
        
        if (x && d)
        {
            ebox_attrprocess_viabinbuf(x, d);
        }
        
        Atom a = (argv[0]);
        x->property_name = a.asSymbol();
        
        x->instance = OPInstance::findByCanvas(x->parent_canvas);
        
        x->e_box.b_boxparameters.d_boxfillcolor = rgba_greylight;
        x->e_box.b_boxparameters.d_bordercolor = rgba_green;
        
        x->out1 = outlet_new((t_object*)x, &s_anything);
        x->out2 = outlet_new((t_object*)x, &s_anything);    //setter bang
        x->out3 = outlet_new((t_object*)x, &s_anything);    //getter bang
        
        if (x->instance)
            x->instance->addPropertyBox(x->property_name, (t_object*)x);
        
        ebox_ready((t_ebox *)x);
        
        
        return static_cast<void*>(x);

}


static void exp_method_free(t_exp_method* x, t_symbol*id, int argc, t_atom* argv)
{
    if (x->instance)
        x->instance->freeMethod(x->property_name);
    
}

#pragma mark -

static void exp_method_set(t_exp_method* x, t_symbol*id, int argc, t_atom* argv)
{
    
    Atom name = Atom(argv[0]);
    
    if (name.asSymbol() == x->property_name)
    {
        AtomList list2(argc-1,&argv[1]);
        x->instance->setAtomListProperty(x->property_name, list2);
        
        outlet_bang(x->out2);
    }
}


static void exp_method_output(t_exp_method* x)
{
    ceammc::to_outlet(x->out1, (x->instance->getAtomListProperty(x->property_name)));
}

static void exp_method_bang(t_exp_method* x)
{
    exp_method_output(x);
}

//rename
static void exp_method_list(t_exp_method* x, t_symbol* s, int argc, t_atom* argv)
{
    x->instance->setAtomListProperty(x->property_name, AtomList(argc,argv));
    exp_method_output(x);
}


static void exp_method_get(t_exp_method* x, t_symbol*id, int argc, t_atom* argv)
{
    
    if (x->instance)
    {
        AtomList list((Atom(argv[0])));
        list.append(x->instance->getAtomListProperty(x->property_name));
        
        x->instance->multipleOutput(list);
        
    }
    
    outlet_bang(x->out3);
    
}



extern "C" void setup_exp0x2eaproperty()
{
    
    exp_method_class = eclass_new((OBJ_NAME),
                                    reinterpret_cast<t_typ_method>(exp_method_new),
                                    reinterpret_cast<t_typ_method>(exp_method_free),
                                    sizeof(t_exp_method), CLASS_PATCHABLE, A_GIMME,0);
    
    eclass_addmethod(exp_method_class, (method)(exp_method_set), ("set"), A_GIMME, 0);
    eclass_addmethod(exp_method_class, (method)(exp_method_get), ("get"), A_GIMME, 0);
    
    eclass_addmethod(exp_method_class, (method)(exp_method_bang), ("bang"), A_GIMME, 0);
    eclass_addmethod(exp_method_class, (method)(exp_method_list), ("anything"), A_GIMME, 0);
    
    eclass_register(CLASS_BOX, exp_method_class);

}