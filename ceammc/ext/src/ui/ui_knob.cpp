//
//  ui_knob.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 19/12/16.
//
//

#include <algorithm>

#include "ceammc_gui.h"

#include "ceammc_atomlist.h"
#include "ceammc_format.h"
#include "ceammc_log.h"

template <typename T>
static T clip(T min, T max, T v)
{
    return std::max(min, std::min(max, v));
}

static t_symbol* SYM_PLUS = gensym("+");
static t_symbol* SYM_MINUS = gensym("-");
static t_symbol* SYM_MUL = gensym("*");
static t_symbol* SYM_DIV = gensym("/");
static t_symbol* SYM_INC = gensym("++");
static t_symbol* SYM_DEC = gensym("--");

struct ui_knob : public ceammc_gui::BaseGuiObject {
    t_outlet* out1;

    float x_min;
    float x_max;

    int show_range;
    int draw_active;

    t_etext* txt_min;
    t_etext* txt_max;
    t_efont* txt_font;

    t_rgba knob_color;
    t_rgba scale_color;

private:
    float x_value;

public:
    t_float range() const { return x_max - x_min; }
    t_float value() const { return x_value; }
    t_float minValue() const { return x_min; }
    t_float maxValue() const { return x_max; }
    void setValue(t_float v) { x_value = clip<t_float>(0, 1, v); }

    t_float realValue() const
    {
        t_float r = range();
        return r < 0 ? (x_value - 1) * r + x_max : x_value * r + x_min;
    }

    void setRealValue(t_float v)
    {
        t_float r = range();
        if (r < 0)
            x_value = clip(x_max, x_min, v) / r;
        else if (r > 0)
            x_value = clip(x_min, x_max, v) / r;
        else
            ceammc::Error(0).stream() << "[ui.knob] zero range";
    }

    void output()
    {
        outlet_float(out1, realValue());
        send(realValue());
    }
};

namespace ceammc_gui {

static const int KNOB_MIN_SIZE = 20;

static void draw_knob_arc(t_elayer* g, float cx, float cy, float r, float angle0, float angle1, float width, const t_rgba& color)
{
    egraphics_set_line_capstyle(g, ECAPSTYLE_ROUND);
    egraphics_set_line_width(g, width);
    egraphics_set_color_rgba(g, &color);
    egraphics_arc_oval(g, cx, cy, r, r, angle0, angle1);
    egraphics_stroke(g);
    egraphics_set_line_capstyle(g, ECAPSTYLE_BUTT);
}

void draw_knob_line(t_elayer* g, ui_knob* zx, float cx, float cy, float r, float angle, float width)
{
    egraphics_set_line_width(g, width);
    egraphics_set_line_capstyle(g, ECAPSTYLE_ROUND);
    egraphics_set_color_rgba(g, &zx->knob_color);

    const float lx = r * cosf(angle);
    const float ly = r * sinf(angle);
    egraphics_line_fast(g, cx, cy, cx + lx, cy - ly);
    egraphics_set_line_capstyle(g, ECAPSTYLE_BUTT);
}

UI_fun(ui_knob)::wx_paint(ui_knob* zx, t_object* view)
{
    t_rect rect;
    zx->getRect(&rect);

    t_elayer* g = ebox_start_layer(asBox(zx), BG_LAYER, rect.width, rect.height);

    if (g) {
        const float cx = rect.width * 0.5f;
        const float cy = rect.height * 0.5f;

        float radius_scale = 0.85f;
        const float radius = cx * radius_scale;
        const float arc_scale = 0.78f;
        const float arc_full = -(EPD_2PI)*arc_scale;
        const float arc_angle_offset = -(EPD_PI2 + (1 - arc_scale) * EPD_PI);
        const float arc_begin = arc_angle_offset;
        const float arc_end = arc_full + arc_angle_offset;
        const float value_angle = zx->value() * arc_full + arc_angle_offset;

        // adjust knob
        float line_width = int(rect.height / 20) + 1;

#ifdef __WIN32
        line_width *= 0.5;
#endif

        if (rect.height < 30) {
            radius_scale = 0.55f;
        }

        if (zx->draw_active) {
            // draw active arc
            draw_knob_arc(g, cx, cy, radius, arc_begin, value_angle, line_width, zx->knob_color);

            // draw passive arc
            draw_knob_arc(g, cx, cy, radius, value_angle, arc_end, line_width, zx->scale_color);
        } else {
            // draw full arc
            draw_knob_arc(g, cx, cy, radius, arc_begin, arc_end, line_width, zx->scale_color);
        }

        // draw knob line
        draw_knob_line(g, zx, cx, cy, radius, value_angle, line_width);

        if (zx->show_range) {
            const float xoff = 3 * ebox_getzoom(asBox(zx));
            const float yoff = 12 * ebox_getzoom(asBox(zx));

            char buf[10];
            sprintf(buf, "%g", zx->minValue());

            etext_layout_set(zx->txt_min, buf, zx->txt_font, xoff, rect.height - yoff, rect.width * 2, rect.height / 2, ETEXT_UP_LEFT, ETEXT_JLEFT, ETEXT_NOWRAP);
            etext_layout_draw(zx->txt_min, g);

            sprintf(buf, "%g", zx->maxValue());
            etext_layout_set(zx->txt_max, buf, zx->txt_font, rect.width - xoff, rect.height - yoff, rect.width, rect.height / 2, ETEXT_UP_RIGHT, ETEXT_JRIGHT, ETEXT_NOWRAP);
            etext_layout_draw(zx->txt_max, g);
        }

        ebox_end_layer(asBox(zx), BG_LAYER);
    }

    ebox_paint_layer(asBox(zx), BG_LAYER, 0, 0);
}

UI_fun(ui_knob)::wx_oksize(ui_knob*, t_rect* newrect)
{
    newrect->width = pd_clip_min(newrect->width, KNOB_MIN_SIZE);
    newrect->height = pd_clip_min(newrect->height, KNOB_MIN_SIZE);
}

UI_fun(ui_knob)::wx_mousedrag_ext(ui_knob* zx, t_object*, t_pt pt, long)
{
    t_rect rect;
    zx->getRect(&rect);

    float val;
    val = 1 - pt.y / rect.height;

    if (val > 1)
        val = 1;
    if (val < 0)
        val = 0;

    zx->setValue(val);

    ws_redraw(zx);

    zx->output();
}

UI_fun(ui_knob)::wx_mousedown_ext(ui_knob* zx, t_object* view, t_pt pt, long modifiers)
{
    wx_mousedrag_ext(zx, view, pt, modifiers);
}

UI_fun(ui_knob)::m_float(ui_knob* zx, t_float f)
{
    zx->setRealValue(f);
    ws_redraw(zx);
    zx->output();
}

UI_fun(ui_knob)::m_bang(ui_knob* zx)
{
    zx->output();
}

UI_fun(ui_knob)::wx_attr_changed_ext(ui_knob* z, t_symbol*)
{
    ws_redraw(z);
}

static void ui_kn_getdrawparams(ui_knob* x, t_object*, t_edrawparams* params)
{
    params->d_borderthickness = 1;
    params->d_cornersize = 2;
    params->d_bordercolor = x->b_color_border;
    params->d_boxfillcolor = x->b_color_background;
}

static void knob_get_value(ui_knob* x, t_object* /*attr*/, long* ac, t_atom** av)
{
    *ac = 1;
    *av = reinterpret_cast<t_atom*>(calloc(1, sizeof(t_atom)));
    atom_setfloat(*av, x->realValue());
}

static t_pd_err knob_set_value(ui_knob* x, t_object* /*attr*/, int ac, t_atom* av)
{
    if (ac > 0 && av) {
        x->setRealValue(atom_getfloat(av));
        return 0;
    }

    return 1;
}

static void knob_set(ui_knob* x, t_float f)
{
    x->setRealValue(f);
    GuiFactory<ui_knob>::ws_redraw(x);
}

static void knob_modify(ui_knob* z, t_symbol* s, int argc, t_atom* argv)
{
    if (argc < 1 || !argv) {
        pd_error(z, "[%s] %s: float argument required", eobj_getclassname(z)->s_name, s->s_name);
        return;
    }

    if (s == SYM_PLUS) {
        knob_set(z, z->realValue() + atom_getfloat(argv));
    } else if (s == SYM_MINUS) {
        knob_set(z, z->realValue() - atom_getfloat(argv));
    } else if (s == SYM_MUL) {
        knob_set(z, z->realValue() * atom_getfloat(argv));
    } else if (s == SYM_DIV) {
        t_float v = atom_getfloat(argv);
        if (v == 0.f) {
            pd_error(z, "[%s] division by zero attempt.", eobj_getclassname(z)->s_name);
            return;
        }

        knob_set(z, z->realValue() / v);
    }
}

static void knob_modify_single(ui_knob* z, t_symbol* s, int, t_atom*)
{
    if (s == SYM_INC) {
        knob_set(z, z->realValue() + 1);
    } else if (s == SYM_DEC) {
        knob_set(z, z->realValue() - 1);
    }
}

UI_fun(ui_knob)::init_ext(t_eclass* z)
{
    // clang-format off
    CLASS_ATTR_DEFAULT (z, "size", 0, "40. 40.");

    CLASS_ATTR_RGBA                 (z, "knob_color", 0, ui_knob, knob_color);
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "knob_color", 0, DEFAULT_ACTIVE_COLOR);
    CLASS_ATTR_LABEL                (z, "knob_color", 0, _("Knob Color"));
    CLASS_ATTR_STYLE                (z, "knob_color", 0, "color");

    CLASS_ATTR_RGBA                 (z, "scale_color", 0, ui_knob, scale_color);
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "scale_color", 0, "0.6 0.6 0.6 1.0");
    CLASS_ATTR_LABEL                (z, "scale_color", 0, _("Scale Color"));
    CLASS_ATTR_STYLE                (z, "scale_color", 0, "color");

    CLASS_ATTR_INT                  (z, "show_range", 0, ui_knob, show_range);
    CLASS_ATTR_LABEL                (z, "show_range", 0, _("Show range"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "show_range", 0, "0");
    CLASS_ATTR_STYLE                (z, "show_range", 0, "onoff");

    CLASS_ATTR_INT                  (z, "draw_active", 0, ui_knob, draw_active);
    CLASS_ATTR_LABEL                (z, "draw_active", 0, _("Draw active scale"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "draw_active", 0, "0");
    CLASS_ATTR_STYLE                (z, "draw_active", 0, "onoff");

    CLASS_ATTR_FLOAT                (z, "min", 0, ui_knob, x_min);
    CLASS_ATTR_LABEL                (z, "min", 0, _("Minimum Value"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "min", 0, "0");
    CLASS_ATTR_STYLE                (z, "min", 0, "number");

    CLASS_ATTR_FLOAT                (z, "max", 0, ui_knob, x_max);
    CLASS_ATTR_LABEL                (z, "max", 0, _("Maximum Value"));
    CLASS_ATTR_DEFAULT_SAVE_PAINT   (z, "max", 0, "1");
    CLASS_ATTR_STYLE                (z, "max", 0, "number");

    CLASS_ATTR_VIRTUAL              (z, "value",   knob_get_value, knob_set_value);
    // clang-format on

    eclass_addmethod(z, reinterpret_cast<t_typ_method>(ui_kn_getdrawparams), "getdrawparams", A_NULL, 0);
    eclass_addmethod(z, reinterpret_cast<t_typ_method>(knob_set), "set", A_FLOAT, 0);
    eclass_addmethod(z, reinterpret_cast<t_typ_method>(knob_modify), "+", A_GIMME, 0);
    eclass_addmethod(z, reinterpret_cast<t_typ_method>(knob_modify), "-", A_GIMME, 0);
    eclass_addmethod(z, reinterpret_cast<t_typ_method>(knob_modify), "*", A_GIMME, 0);
    eclass_addmethod(z, reinterpret_cast<t_typ_method>(knob_modify), "/", A_GIMME, 0);
    eclass_addmethod(z, reinterpret_cast<t_typ_method>(knob_modify_single), "++", A_GIMME, 0);
    eclass_addmethod(z, reinterpret_cast<t_typ_method>(knob_modify_single), "--", A_GIMME, 0);
}

UI_fun(ui_knob)::new_ext(ui_knob* zx, t_symbol*, int, t_atom*)
{
    zx->out1 = create_outlet(zx, &s_float);

    zx->setValue(0);

    zx->txt_max = etext_layout_create();
    zx->txt_min = etext_layout_create();
    zx->txt_font = efont_create(FONT_FAMILY, FONT_STYLE, FONT_WEIGHT, FONT_SIZE_SMALL);
}

UI_fun(ui_knob)::free_ext(ui_knob* zx)
{
    outlet_free(zx->out1);

    etext_layout_destroy(zx->txt_max);
    etext_layout_destroy(zx->txt_min);
    efont_destroy(zx->txt_font);
}

UI_fun(ui_knob)::m_preset(ui_knob* zx, t_binbuf* b)
{
    binbuf_addv(b, "sf", &s_float, zx->realValue());
}
}

extern "C" void setup_ui0x2eknob()
{
    ceammc_gui::GuiFactory<ui_knob> class1;
    class1.use_presets(true);
    class1.setup("ui.knob", EBOX_GROWLINK);
}
