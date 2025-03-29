#include "screen.h"

focus_group_t Epage_B = {
    .C_group = NULL,
    .N_group = NULL,
    .N_obj = NULL,
    .focus_run = true,
};

lv_obj_t *ui_Bicyclescreen_init(void)
{
    lv_obj_t *ui_Bicycle = comp_scr_create();
    // lv_obj_clear_flag(ui_Bicycle, LV_OBJ_FLAG_SCROLLABLE);        // 禁用对象滚动
    lv_obj_set_scrollbar_mode(ui_Bicycle, LV_SCROLLBAR_MODE_OFF); // 关闭滚动条
    // lv_obj_set_style_pad_all(ui_Bicycle, 0, LV_PART_MAIN); // 设置内边距

    //*子组件
    // 状态栏
    lv_obj_t *ui_BicycleBAR = lv_obj_create(ui_Bicycle);
    lv_obj_set_size(ui_BicycleBAR, lv_pct(100), 34);
    lv_obj_set_align(ui_BicycleBAR, LV_ALIGN_TOP_MID);
    lv_obj_set_style_radius(ui_BicycleBAR, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_BicycleBAR, 0, LV_PART_MAIN); // 设置内边距

    lv_obj_set_flex_flow(ui_BicycleBAR, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_BicycleBAR, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_AROUND);

    lv_obj_set_style_bg_color(ui_BicycleBAR, lv_color_hex(0x333333), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_BicycleBAR, 127, LV_PART_MAIN | LV_STATE_DEFAULT); // 透明度50%

    lv_obj_set_style_border_color(ui_BicycleBAR, lv_color_hex(0xD9D9D9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_BicycleBAR, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_BicycleBAR, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_DEFAULT);

    // 标志图
    lv_obj_t *ui_img1 = lv_img_create(ui_BicycleBAR);
    lv_img_set_src(ui_img1, &PNG);
    lv_obj_set_size(ui_img1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

    lv_obj_t *ui_img2 = lv_img_create(ui_BicycleBAR);
    lv_img_set_src(ui_img2, &PNG);
    lv_obj_set_size(ui_img2, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

    lv_obj_t *ui_img3 = lv_img_create(ui_BicycleBAR);
    lv_img_set_src(ui_img3, &PNG);
    lv_obj_set_size(ui_img3, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

    lv_obj_t *ui_img4 = lv_img_create(ui_BicycleBAR);
    lv_img_set_src(ui_img4, &PNG);
    lv_obj_set_size(ui_img4, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

    lv_obj_t *ui_img5 = lv_img_create(ui_BicycleBAR);
    lv_img_set_src(ui_img5, &PNG);
    lv_obj_set_size(ui_img5, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

    lv_obj_t *ui_img6 = lv_img_create(ui_BicycleBAR);
    lv_img_set_src(ui_img6, &PNG);
    lv_obj_set_size(ui_img6, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

    lv_obj_t *ui_img7 = lv_img_create(ui_BicycleBAR);
    lv_img_set_src(ui_img7, &PNG);
    lv_obj_set_size(ui_img7, LV_SIZE_CONTENT, LV_SIZE_CONTENT);

    // 数据容器
    lv_obj_t *ui_B_Con1 = lv_obj_create(ui_Bicycle);
    lv_obj_set_size(ui_B_Con1, 200, 70);
    lv_obj_align(ui_B_Con1, LV_ALIGN_TOP_MID, 0, 40);
    lv_obj_add_flag(ui_B_Con1, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_style_radius(ui_B_Con1, 7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_B_Con1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_B_Con1, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_B_Con1, lv_color_hex(0x0000FF), LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui_B_Con1, lv_color_hex(0xFF0000), LV_STATE_EDITED);
    lv_obj_set_style_border_width(ui_B_Con1, 4, LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_B_Con1, 64, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *ui_B_Con2 = lv_obj_create(ui_Bicycle);
    lv_obj_set_size(ui_B_Con2, 96, 150);
    lv_obj_set_pos(ui_B_Con2, 20, 120);
    lv_obj_add_flag(ui_B_Con2, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_style_radius(ui_B_Con2, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_B_Con2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_B_Con2, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_B_Con2, lv_color_hex(0x0000FF), LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui_B_Con2, lv_color_hex(0xFF0000), LV_STATE_EDITED);
    lv_obj_set_style_border_width(ui_B_Con2, 4, LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_B_Con2, 64, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *ui_B_Con3 = lv_obj_create(ui_Bicycle);
    lv_obj_set_size(ui_B_Con3, 96, 150);
    lv_obj_set_pos(ui_B_Con3, 124, 120);
    lv_obj_add_flag(ui_B_Con3, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_style_radius(ui_B_Con3, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_B_Con3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_B_Con3, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_B_Con3, lv_color_hex(0x0000FF), LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui_B_Con3, lv_color_hex(0xFF0000), LV_STATE_EDITED);
    lv_obj_set_style_border_width(ui_B_Con3, 4, LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_B_Con3, 64, LV_PART_MAIN | LV_STATE_DEFAULT);
    // 下顶栏
    lv_obj_t *ui_BicycleRunU = lv_obj_create(ui_Bicycle);
    lv_obj_set_size(ui_BicycleRunU, lv_pct(100), 45);
    lv_obj_align(ui_BicycleRunU, LV_ALIGN_BOTTOM_MID, 0, 15);
    lv_obj_add_flag(ui_BicycleRunU, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_style_radius(ui_BicycleRunU, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_BicycleRunU, lv_color_hex(0xECECDA), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_BicycleRunU, lv_color_hex(0x0000FF), LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui_BicycleRunU, lv_color_hex(0xFF0000), LV_STATE_EDITED);
    lv_obj_set_style_border_width(ui_BicycleRunU, 4, LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_BicycleRunU, 0, LV_PART_MAIN); // 设置内边距

    lv_obj_t *ui_B_RunU_img1 = lv_img_create(ui_BicycleRunU);
    lv_img_set_src(ui_B_RunU_img1, &Double_Up);
    lv_obj_set_size(ui_B_RunU_img1, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_B_RunU_img1, LV_TEXT_ALIGN_CENTER);

    //*控制组
    lv_group_add_obj(scr_group, ui_Bicycle); // 屏幕

    lv_group_add_obj(bike_group, ui_B_Con1);
    lv_group_add_obj(bike_group, ui_B_Con2);
    lv_group_add_obj(bike_group, ui_B_Con3);
    lv_group_add_obj(bike_group, ui_BicycleRunU);
    //*添加事件
    // TODO:子组件待定
    Epage_B.C_group = scr_group;
    Epage_B.N_group = bike_group;
    Epage_B.N_obj = ui_B_Con1;
    // lv_obj_add_event_cb(ui_Bicycle, wtf, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Bicycle, event_scr, LV_EVENT_FOCUSED, &Epage_B);
    lv_obj_add_event_cb(ui_Bicycle, event_enter_group, LV_EVENT_KEY, &Epage_B);
    lv_obj_add_event_cb(ui_Bicycle, event_leave_group, LV_EVENT_LONG_PRESSED_REPEAT, &Epage_B);

    // lv_obj_add_event_cb(ui_BicycleRunU, event_enter_con_group, LV_EVENT_KEY, &Epage_B);
    return ui_Bicycle;
}
