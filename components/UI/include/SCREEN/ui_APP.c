#include "screen.h"

focus_group_t Epage_A = {
    .C_group = NULL,
    .N_group = NULL,
    .N_obj = NULL,
    .scr_obj = NULL,
    .focus_run = true,
};

lv_obj_t *ui_APPscreen_init(void)
{
    lv_obj_t *ui_APP = comp_scr_create(); // 创建屏幕
    //*子组件
    lv_obj_t *ui_APPseting = create_chrome_btn(ui_APP);
    lv_obj_align(ui_APPseting, LV_ALIGN_TOP_MID, 0, 24);
    lv_label_set_text(get_child_comp(ui_APPseting, C_label), "Text1");
    lv_img_set_src(get_child_comp(ui_APPseting, C_img), &Settings);

    lv_obj_t *ui_APPfolder = create_chrome_btn(ui_APP);
    lv_obj_align_to(ui_APPfolder, ui_APPseting, LV_ALIGN_OUT_BOTTOM_MID, 0, 12);
    lv_label_set_text(get_child_comp(ui_APPfolder, C_label), "Text2");
    lv_img_set_src(get_child_comp(ui_APPfolder, C_img), &Folders);

    lv_obj_t *ui_APPbicycle = create_chrome_btn(ui_APP);
    lv_obj_align_to(ui_APPbicycle, ui_APPfolder, LV_ALIGN_OUT_BOTTOM_MID, 0, 12);
    lv_label_set_text(get_child_comp(ui_APPbicycle, C_label), "Text4");
    lv_img_set_src(get_child_comp(ui_APPbicycle, C_img), &Bicycle);

    lv_obj_t *ui_APPmap = create_chrome_btn(ui_APP);
    lv_obj_align_to(ui_APPmap, ui_APPbicycle, LV_ALIGN_OUT_BOTTOM_MID, 0, 12);
    lv_label_set_text(get_child_comp(ui_APPmap, C_label), "Text3");
    lv_img_set_src(get_child_comp(ui_APPmap, C_img), &Radio_Tower);
    //*添加访问组
    lv_group_add_obj(scr_group, ui_APP); // 屏幕

    lv_group_add_obj(app_group, ui_APPseting);
    lv_group_add_obj(app_group, ui_APPfolder);
    lv_group_add_obj(app_group, ui_APPbicycle);
    lv_group_add_obj(app_group, ui_APPmap);
    //*添加事件
    Epage_A.C_group = scr_group;
    Epage_A.N_group = app_group;
    Epage_A.N_obj = ui_APPseting;
    
    lv_obj_add_event_cb(ui_APP, event_scr, LV_EVENT_FOCUSED, &Epage_A);
    lv_obj_add_event_cb(ui_APP, event_enter_group, LV_EVENT_KEY, &Epage_A);
    lv_obj_add_event_cb(ui_APP, event_leave_group, LV_EVENT_LONG_PRESSED, &Epage_A);
    // lv_obj_add_event_cb(ui_APPseting, event_APPseting, LV_EVENT_SHORT_CLICKED, NULL);
    // lv_obj_add_event_cb(ui_APPfolder, event_APPfolder, LV_EVENT_SHORT_CLICKED, NULL);
    // lv_obj_add_event_cb(ui_APPbicycle, event_APPbicycle, LV_EVENT_SHORT_CLICKED, NULL);
    // lv_obj_add_event_cb(ui_APPmap, event_APPmap, LV_EVENT_SHORT_CLICKED, NULL);

    return ui_APP;
}