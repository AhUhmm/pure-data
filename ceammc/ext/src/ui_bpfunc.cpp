//
//  ui_bpfunc.cpp
//  pd_ext
//
//  Created by Alex on 20/12/16.
//
//

#include <stdio.h>

#include "lib/ceammc_gui.h"

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

#include <map>
#include <algorithm>

typedef struct t_bpt
{
    int idx;
    float x;
    float y;
    
    float dist;      // distance from mouse. recalculated on hover
    float ldist;    //  distance to line
    bool selected;
    
} t_bpt;

typedef std::vector<t_bpt> bpf_points;


using namespace ceammc;

struct ui_bpfunc : public ceammc_gui::base_pd_object
{
    t_ebox x_gui;
    
    float mouse_x;
    float mouse_y;
    int mouse_dn;
    bool _selected;
    
    t_outlet *out1;
    t_atom *out_list;
    int out_list_count;
    
    AtomList *output;
    
    bpf_points *points;
    
    float _px;
    float _py;
    
    int addidx;
    int delidx;
    
    float addpos;
    float addpos_y;
    bool del_mod;
    
    float range_x;
    float range_y;
    float shift_x;
    float shift_y;
    
    
    t_etext *txt_min;
    t_etext *txt_max;
    t_efont *txt_font;
    
    
    
};

const bool bpf_sort_pred(t_bpt const & a, t_bpt const & b)  {
    return a.x < b.x;
}

namespace ceammc_gui {
    
    inline void bpf_point_sort(t_object *z)
    {
        
        bpf_points *ps = ((ui_bpfunc*)z)->points;
        
        std::sort(ps->begin(), ps->end(), bpf_sort_pred);
        
        
        
        
    }
    
    inline void bpf_point_add(t_object *z,int idx, float x, float y)
    {
        t_bpt pt1;
        pt1.idx = idx;
        pt1.x = x;
        pt1.y = y;
        
        bpf_points *ps = ((ui_bpfunc*)z)->points;
        
        ps->push_back(pt1);
        
        bpf_point_sort(z);
        
        
    }
    
    inline void bpf_point_del(t_object *z,int idx)
    {
        //
        printf("del pt: %i", idx);
        
        bpf_points *ps = ((ui_bpfunc*)z)->points;
        
        if (ps->size()>2)
        {
            ps->erase(ps->begin()+idx);
            bpf_point_sort(z);
        }
    }
    
    inline int bpf_size(t_object *z)
    {
        return (int)((ui_bpfunc*)z)->points->size();
    }
    
    inline void bpf_points_new(t_object *z)
    {
        //check
        if (((ui_bpfunc*)z)->points) free (((ui_bpfunc*)z)->points);
        
        ((ui_bpfunc*)z)->points = new bpf_points;
    }
    
    inline void bpf_points_free(t_object *z)
    {
        //check
        if (((ui_bpfunc*)z)->points) free (((ui_bpfunc*)z)->points);
        
    }
    
    
#pragma mark -
    
    UI_fun(ui_bpfunc)::wx_paint(t_object *z, t_object *view)
    {
        //UI_Prop
        
        t_symbol *bgl = gensym("background_layer");
        //float size;
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        t_elayer *g = ebox_start_layer((t_ebox *)z, bgl, rect.width, rect.height);
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        if(g)
        {
            egraphics_set_color_hex(g, gensym("#E0E0E0"));
            
            egraphics_rectangle(g, 0, 0, rect.width, rect.height);
            egraphics_fill(g);
            
            egraphics_set_color_hex(g, gensym("#C0C0C0"));
            
            egraphics_line(g, 0, .25*rect.height, rect.width, .25*rect.height);
            egraphics_line(g, 0, .5*rect.height, rect.width, .5*rect.height);
            egraphics_line(g, 0, .75*rect.height, rect.width, .75*rect.height);
            
            egraphics_stroke(g);
            
            egraphics_set_color_hex(g, gensym("#505050"));
            
            
            bpf_points::iterator it = zx->points->begin();
            
            if (zx->addpos>0)
            {
                egraphics_set_color_hex(g, gensym("#00C0FF"));
                
                //egraphics_rectangle(g, zx->addpos-7, zx->addpos_y-7, 15, 15);
                egraphics_line(g,zx->addpos , 0, zx->addpos, rect.height);
                egraphics_line(g,0 , zx->addpos_y, rect.width, zx->addpos_y);
                
                egraphics_stroke(g);
                
            }
            
            //for (; it != zx->points->end(); ++it)
            {
                
                //egraphics_circle(g, it->x * rect.width, (1-it->y) * rect.height, 6);
                egraphics_rectangle(g, it->x * rect.width -3, (1-it->y) * rect.height -3, 6, 6);
                egraphics_fill(g);
                
                egraphics_set_color_hex(g, gensym("#505050"));
                
                float px = it->x * rect.width;
                float py = (1-it->y) * rect.height;
                
                //egraphics_move_to(g, it->x * rect.width, (1-it->y) * rect.height);
                
                if (it->dist < .1)
                {
                    egraphics_rectangle(g, it->x * rect.width -6, (1-it->y) * rect.height -6, 11, 11);
                    egraphics_stroke(g);
                    //egraphics_circle(g, it->x * rect.width, (1-it->y) * rect.height, 10. * it->dist);
                }
                
                
                //yet disabled
                //            float lw = (it->ldist<.4);// && (it->ldist>.2);
                //            if (lw<0) lw = 0;
                //            if (lw>1) lw=1;
                
                egraphics_set_line_width(g, 1);    //yet disabled
                
                ++it;
                
                for ( ; it != zx->points->end(); ++it)
                {
                    egraphics_set_color_hex(g, gensym("#505050"));
                    
                    egraphics_set_line_width(g, 1);
                    egraphics_line(g, px, py, it->x * rect.width, (1-it->y) * rect.height);
                    egraphics_stroke(g);
                    
                    px = it->x * rect.width;
                    py = (1-it->y) * rect.height;
                    
                    //yet disabled
                    //                lw = (it->ldist<.4);
                    //                if (lw<0) lw = 0;
                    //                if (lw>1) lw=1;
                    
                    egraphics_set_line_width(g, 1);
                    egraphics_rectangle(g, it->x * rect.width -3, (1-it->y) * rect.height -3, 6, 6);
                    egraphics_fill(g);
                    
                    if (it->dist < .1)
                    {
                        egraphics_set_color_hex(g, gensym((zx->del_mod)?"#FF0000":"#505050"));
                        
                        egraphics_rectangle(g, it->x * rect.width -6, (1-it->y) * rect.height -6, 11, 11);
                        egraphics_stroke(g);
                        
                    }
                    
                    
                }
            }
            
            char c_min[10];
            sprintf(c_min, "%.2f", zx->shift_y);
            
            char c_max[10];
            sprintf(c_max, "%.2f", zx->range_y+zx->shift_y);
            
            etext_layout_set(zx->txt_min, c_min, zx->txt_font, 3, rect.height-12, rect.width*2, rect.height/2, ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
            etext_layout_draw(zx->txt_min, g);
            
            etext_layout_set(zx->txt_max, c_max, zx->txt_font, 3, 12, rect.width*2, rect.height/2, ETEXT_DOWN_LEFT, ETEXT_JLEFT, ETEXT_WRAP);
            etext_layout_draw(zx->txt_max, g);
            
        }
        
        ebox_end_layer((t_ebox*)z, bgl);
        ebox_paint_layer((t_ebox *)z, bgl, 0., 0.);
        
    }
    
#pragma mark -
#pragma mark mouse
    
    UI_fun(ui_bpfunc)::wx_mousemove_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
    {
        //calc distances
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        float nx = pt.x / rect.width;
        float ny = pt.y / rect.height;
        
//        float nlen = sqrtf(nx*nx+ny*ny);
//        float nnx = (nlen!=0) ? nx/ nlen : 0;
//        float nny = (nlen!=0) ? ny/ nlen : 0;
        
        
        //bpf_points::iterator it;
        //bpf_points::iterator itn;
        
        for (int i = 0; i<zx->points->size();i++)   //it = zx->points->begin(); it != zx->points->end(); ++it
        {
            t_bpt * it = &(zx->points)->at(i);
            t_bpt * itn = ((i-1) == zx->points->size()) ? &(zx->points)->at(i+1) : &(zx->points)->at(i);
            
            float dx = nx - it->x;
            float dy =  ( ny - (1 -it->y) );
            
            it->dist = sqrtf(dx*dx + dy*dy);
            
            it->selected = (it->dist<.1);
            
            it->ldist = 1;//abs(dot1);
            
            if ( (it->x < (pt.x / rect.width)) && ((pt.x / rect.width )< itn->x) )
            {
                zx->addidx = i;
            }
            else
            {
                zx->addidx = (int)zx->points->size();
                
            }
            
            if (modifiers==EMOD_SHIFT)
            {
                zx->addpos = pt.x;
                zx->addpos_y = pt.y;
            }
            else
            {
                zx->addpos = -1;
                zx->del_mod = false;
            }
            
            if (it->selected) zx->delidx = i;
            
            if (modifiers == EMOD_ALT)
            {
                zx->del_mod = true;
            }
            
            //printf ("(%f,%f) %f : ", dx,dy, it->dist);
            //printf("%f : ", it->ldist);
        }
        //printf("\n");
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
    }
    
    
    UI_fun(ui_bpfunc)::wx_mouseleave_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        zx->addpos = -1;
    }
    
    UI_fun(ui_bpfunc)::wx_mousedown_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        zx->_px = pt.x;
        zx->_py = pt.y;
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        if (modifiers==EMOD_SHIFT)
        {
            bpf_point_add(z, zx->addidx, pt.x/rect.width, 1-(pt.y/rect.height));
            ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
            
        }
        
        if (modifiers==EMOD_ALT)
        {
            bpf_point_del(z, zx->delidx);
            printf("del %i\n", zx->delidx);
            ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
            
        }
    }
    
    UI_fun(ui_bpfunc)::wx_mouseup_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        zx->addpos = -1;
    }
    
    UI_fun(ui_bpfunc)::wx_mousedrag_ext(t_object *z, t_object *view, t_pt pt, long modifiers)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        bpf_points::iterator it = zx->points->begin();
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        for ( ; it != zx->points->end(); ++it)
        {
            if (it->selected)
            {
                it->x += (pt.x - zx->_px) / rect.width;
                it->y += ((1-pt.y) - (1-zx->_py)) / rect.height;
                
                if (it->x>1) it->x=1;
                if (it->x<0) it->x=0;
                
                if (it->y>1) it->y=1;
                if (it->y<0) it->y=0;
                
            }
        }
        
        zx->_px = pt.x;
        zx->_py = pt.y;
        
        bpf_point_sort(z);
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
        
    }
    
    
    
#pragma mark -
#pragma mark messages
    
    UI_fun(ui_bpfunc)::m_bang(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        zx->out_list_count = (int)zx->points->size() * 2;
        
        if (zx->out_list) {free(zx->out_list);}
        
        zx->out_list = (t_atom*)malloc(sizeof(t_atom)*zx->out_list_count);
        
        int j=0;
        for (int i=0;i<zx->out_list_count;i+=2)
        {
            zx->out_list[i].a_type = A_FLOAT;
            zx->out_list[i].a_w.w_float = zx->points->at(j).x * zx->range_x + zx->shift_x;
            
            zx->out_list[i+1].a_type = A_FLOAT;
            zx->out_list[i+1].a_w.w_float = zx->points->at(j).y * zx->range_y + zx->shift_y;
            
            j++;
        }
        
        outlet_list(zx->out1, &s_list, zx->out_list_count, zx->out_list);
    }
    
    void bpf_m_range_x(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        zx->range_x = argv[0].a_w.w_float;
    }
    
    void bpf_m_range_y(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        zx->range_y = argv[0].a_w.w_float;
    }
    
    void bpf_m_shift_x(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        zx->shift_x = argv[0].a_w.w_float;
    }
    
    void bpf_m_shift_y(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        zx->shift_y = argv[0].a_w.w_float;
    }
    
    void bpf_m_raw(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        zx->out_list_count = (int)zx->points->size() * 2;
        
        if (zx->out_list) {free(zx->out_list);}
        
        zx->out_list = (t_atom*)malloc(sizeof(t_atom)*zx->out_list_count);
        
        int j=0;
        for (int i=0;i<zx->out_list_count;i+=2)
        {
            zx->out_list[i].a_type = A_FLOAT;
            zx->out_list[i].a_w.w_float = zx->points->at(j).x ;
            
            zx->out_list[i+1].a_type = A_FLOAT;
            zx->out_list[i+1].a_w.w_float = zx->points->at(j).y ;
            
            j++;
        }
        
        outlet_list(zx->out1, &s_list, zx->out_list_count, zx->out_list);
    }
    
    void bpf_m_clear(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        
        //ui_bpfunc *zx = (ui_bpfunc*)z;
        
        bpf_points_new(z);
        
        bpf_point_add(z, 0, 0, 0);
        bpf_point_add(z, 1, .5, .75);
        bpf_point_add(z, 2, 1, 1);
        
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
        
    }
    
    void bpf_m_add(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        if (argc <2) return;
        if ( (argv[0].a_type != A_FLOAT) && (argv[1].a_type != A_FLOAT) ) return;
        
        float xx = argv[0].a_w.w_float;
        float yy = argv[1].a_w.w_float;
        
        t_rect rect;
        ebox_get_rect_for_view((t_ebox *)z, &rect);
        
        bpf_point_add(z, zx->addidx, (xx-zx->shift_x)/zx->range_x, (yy-zx->shift_y)/zx->range_y);
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
        
    }
    
    void bpf_m_del(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        if (argc <1) return;
        if (argv[0].a_type != A_FLOAT) return;
        
        int del_i = (int)argv[0].a_w.w_float;
        
        bpf_point_del(z, del_i);
        ceammc_gui::object<ceammc_gui::base_pd_object>::ws_redraw(z);
        
        
    }
    
    void bpf_m_vline(t_object *z, t_symbol *s, int argc, t_atom *argv)
    {
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        
        float last_time = 0;
        
        //memory dealloc???
        for (int j=0;j<zx->points->size();j++)      //i is on vacation
        {
            t_atom *out_list = (t_atom*)malloc(sizeof(t_atom)*3);
            
            float this_time = zx->points->at(j).x * zx->range_x + zx->shift_x;
            
            out_list[1].a_type = A_FLOAT;
            out_list[1].a_w.w_float = this_time;
            
            out_list[0].a_type = A_FLOAT;
            out_list[0].a_w.w_float = zx->points->at(j).y * zx->range_y + zx->shift_y;
            
            out_list[2].a_type = A_FLOAT;
            out_list[2].a_w.w_float = last_time;
            
            last_time += this_time;
            
            outlet_list(zx->out1, &s_list, 3, out_list);
            
        }
        
        
    }
    
#pragma mark -
    
    UI_fun(ui_bpfunc)::new_ext(t_object *z, t_symbol *s, int argcl, t_atom *argv)
    {
        ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.red = 0.75;
        ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.green = 0.75;
        ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.blue   = 0.75;
        ((t_ebox*)z)->b_boxparameters.d_boxfillcolor.alpha = 1.;
        
        bpf_points_new(z);
        
        bpf_point_add(z, 0, 0, 0);
        bpf_point_add(z, 1, .5, .75);
        bpf_point_add(z, 2, 1, 1);
        
        ui_bpfunc *zx = (ui_bpfunc*)z;
        
        zx->_px = 0;
        zx->_py = 0;
        
        zx->out1 = outlet_new(z, &s_list);
        
        zx->range_x = 1000;
        zx->range_y = 1;
        zx->shift_x = 0;
        zx->shift_y = 0;
        
        zx->output = new AtomList;
        
        zx->txt_max = etext_layout_create();
        zx->txt_min = etext_layout_create();
        zx->txt_font = efont_create(gensym("Helvetica"), gensym("light"), gensym("normal"), 8);
        
    }
    
    UI_fun(ui_bpfunc)::init_ext(t_eclass *z)
    {
        CLASS_ATTR_DEFAULT (z, "size", 0, "200. 150.");
        
        CLASS_ATTR_FLOAT(z, "shift_x", 0, ui_bpfunc, shift_x);
        CLASS_ATTR_DEFAULT(z, "shift_x", 0, "0");
        CLASS_ATTR_LABEL(z, "shift_x", 0, "shift_x");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "shift_x", 0, "0");
        
        CLASS_ATTR_FLOAT(z, "range_x", 0, ui_bpfunc, range_x);
        CLASS_ATTR_DEFAULT(z, "range_x", 0, "1");
        CLASS_ATTR_LABEL(z, "range_x", 0, "range_x");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "range_x", 0, "1");
        
        CLASS_ATTR_FLOAT(z, "shift_y", 0, ui_bpfunc, shift_y);
        CLASS_ATTR_DEFAULT(z, "shift_y", 0, "0");
        CLASS_ATTR_LABEL(z, "shift_y", 0, "shift_y");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "shift_y", 0, "0");
        
        CLASS_ATTR_FLOAT(z, "range_y", 0, ui_bpfunc, range_y);
        CLASS_ATTR_DEFAULT(z, "range_y", 0, "1");
        CLASS_ATTR_LABEL(z, "range_y", 0, "range_y");
        CLASS_ATTR_DEFAULT_SAVE_PAINT(z, "range_y", 0, "1");
        
        eclass_addmethod(z, (method)(bpf_m_range_x), ("range_x"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_range_y), ("range_y"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_shift_x), ("shift_x"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_shift_y), ("shift_y"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_raw), ("raw"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_clear), ("clear"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_add), ("add"), A_GIMME,0);
        eclass_addmethod(z, (method)(bpf_m_del), ("del"), A_GIMME,0);
        
        eclass_addmethod(z, (method)(bpf_m_vline), ("vline"), A_GIMME,0);
        
    }
    
}

extern "C" void setup_ui0x2ebpfunc()
{
    ceammc_gui::object<ui_bpfunc> class1;
    class1.setup("ui.bpfunc");
}