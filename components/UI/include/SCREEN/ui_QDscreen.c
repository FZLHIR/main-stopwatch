#include "screen.h"

focus_group_t Epage_Q = {
    .focus_run = true,
};

lv_obj_t *ui_QDscreen_init(void)
{
    lv_obj_t *ui_QDscreen = comp_scr_create();
    //*子组件
    lv_obj_t *QDimg = lv_img_create(ui_QDscreen);
    lv_img_set_src(QDimg, &Si_SI_Ta);
    lv_obj_set_size(QDimg, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(QDimg, LV_ALIGN_CENTER);
    //*添加访问组
    lv_group_add_obj(scr_group, ui_QDscreen); // 绑定到设定的组
    //*添加事件
    lv_obj_add_event_cb(ui_QDscreen, event_scr, LV_EVENT_FOCUSED, &Epage_Q);

    return ui_QDscreen;
}

