#include "comp_scr.h"

lv_obj_t *comp_scr_create(void)
{
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_border_color(scr, lv_color_hex(0xFF0000), LV_STATE_EDITED);
    lv_obj_set_style_border_color(scr, lv_color_hex(0x0000FF), LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(scr, 5, LV_STATE_DEFAULT); 
    return scr;
}