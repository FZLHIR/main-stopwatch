#include <stdio.h>
#include "UI.h"

lv_group_t *scr_group;
lv_group_t *app_group;
lv_group_t *seting_group;
lv_group_t *bike_group;
lv_group_t *map_group;

void ui_init(void)
{
    set_comp_event_id(lv_event_register_id()); // 注册一个id
    scr_group = lv_group_create();
    lv_group_set_wrap(scr_group, false);//关闭屏幕组的循环功能
    app_group = lv_group_create();
    seting_group = lv_group_create();
    bike_group = lv_group_create();
    map_group = lv_group_create();
    // 初始化屏幕
    // todo 可以接入一个独立的页面管理器
    // ui_APPscreen_init();
    ui_SETINGscreen_init();
    lv_obj_t *QD = ui_QDscreen_init();
    ui_Bicyclescreen_init();
    ui_Mapscreen_init();

    lv_indev_set_group(get_encoder_indev(), scr_group);
    lv_group_focus_obj(QD);
    lv_disp_load_scr(QD); // 加载初始屏幕
}