#include "screen.h"

focus_group_t Epage_M = {
    .C_group = NULL,
    .N_group = NULL,
    .N_obj = NULL,
    .focus_run = true,
};

lv_obj_t *ui_Map_img;

lv_obj_t *
ui_Mapscreen_init(void)
{
    lv_obj_t *ui_Map = comp_scr_create();

    ui_Map_img = lv_img_create(ui_Map);
    lv_img_set_src(ui_Map_img, "V:/lvgl/jpg/map17.jpg");
    lv_obj_set_size(ui_Map_img, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align(ui_Map_img, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(ui_Map_img, LV_OBJ_FLAG_EVENT_BUBBLE);

    //* 子组件
    // 上横条
    lv_obj_t *ui_MapRunD = lv_obj_create(ui_Map);
    lv_obj_set_size(ui_MapRunD, lv_pct(100), 45);
    lv_obj_align(ui_MapRunD, LV_ALIGN_TOP_MID, 0, -15);
    lv_obj_add_flag(ui_MapRunD, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_style_pad_all(ui_MapRunD, 0, LV_PART_MAIN); // 设置内边距
    lv_obj_set_style_radius(ui_MapRunD, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_MapRunD, lv_color_hex(0x0000FF), LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui_MapRunD, lv_color_hex(0xFF0000), LV_STATE_EDITED);
    lv_obj_set_style_border_width(ui_MapRunD, 4, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_MapRunD, lv_color_hex(0xECECDA), LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *ui_MapImage14 = lv_img_create(ui_MapRunD);
    lv_img_set_src(ui_MapImage14, &Double_Up);
    lv_obj_set_size(ui_MapImage14, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_MapImage14, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_MapImage14, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_img_set_angle(ui_MapImage14, 1800);
    // 速度
    lv_obj_t *ui_MapPanel2 = lv_obj_create(ui_Map);
    lv_obj_set_size(ui_MapPanel2, 220, 60);
    lv_obj_set_pos(ui_MapPanel2, -20, 220);
    lv_obj_add_flag(ui_MapPanel2, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_style_radius(ui_MapPanel2, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_MapPanel2, lv_color_hex(0xD9D9D9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_MapPanel2, lv_color_hex(0x0000FF), LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui_MapPanel2, lv_color_hex(0xFF0000), LV_STATE_EDITED);
    lv_obj_set_style_border_width(ui_MapPanel2, 4, LV_STATE_DEFAULT);

    lv_obj_t *ui_MapLabel3 = lv_label_create(ui_MapPanel2);
    lv_obj_set_size(ui_MapLabel3, 150, 40);
    lv_obj_align(ui_MapLabel3, LV_ALIGN_CENTER, 0, 3);
    lv_label_set_text(ui_MapLabel3, "-- KM/H");
    lv_obj_set_style_text_align(ui_MapLabel3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_MapLabel3, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_t *ui_MapImage13 = lv_img_create(ui_MapPanel2);
    lv_img_set_src(ui_MapImage13, &Eject);
    lv_obj_set_size(ui_MapImage13, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_align(ui_MapImage13, LV_ALIGN_RIGHT_MID);
    lv_obj_add_flag(ui_MapImage13, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_img_set_angle(ui_MapImage13, -900);
    // 缩放
    lv_obj_t *ui_MapPanel4 = lv_obj_create(ui_Map);
    lv_obj_set_size(ui_MapPanel4, 26, 80);
    lv_obj_set_pos(ui_MapPanel4, 206, 210);
    lv_obj_add_flag(ui_MapPanel4, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_style_bg_color(ui_MapPanel4, lv_color_hex(0xD9D9D9), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_MapPanel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(ui_MapPanel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_MapPanel4, lv_color_hex(0x0000FF), LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui_MapPanel4, lv_color_hex(0xFF0000), LV_STATE_EDITED);
    lv_obj_set_style_border_width(ui_MapPanel4, 4, LV_STATE_DEFAULT);

    lv_obj_t *ui_MapImage15 = lv_img_create(ui_MapPanel4);
    lv_img_set_src(ui_MapImage15, &Plus);
    lv_obj_set_size(ui_MapImage15, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align(ui_MapImage15, LV_ALIGN_TOP_MID, 0, 2);
    lv_obj_add_flag(ui_MapImage15, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_img_set_zoom(ui_MapImage15, 191);

    lv_obj_t *ui_MapImage16 = lv_img_create(ui_MapPanel4);
    lv_img_set_src(ui_MapImage16, &Minus);
    lv_obj_set_size(ui_MapImage16, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align(ui_MapImage16, LV_ALIGN_BOTTOM_MID, 0, -2);
    lv_obj_add_flag(ui_MapImage16, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_img_set_zoom(ui_MapImage16, 191);
    // 导航标
    lv_obj_t *ui_Map_nav = lv_img_create(ui_Map);
    lv_img_set_src(ui_Map_nav, &Navigation_c);
    lv_obj_set_size(ui_Map_nav, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_align(ui_Map_nav, LV_ALIGN_CENTER, 0, 0);
    //*访问组
    lv_group_add_obj(scr_group, ui_Map); // 屏幕

    lv_group_add_obj(map_group, ui_MapRunD);
    lv_group_add_obj(map_group, ui_MapPanel2);
    lv_group_add_obj(map_group, ui_MapPanel4);

    lv_group_t *map_scale_group = lv_group_create(); // 缩放组
    lv_group_add_obj(map_scale_group, ui_MapImage15);
    lv_group_add_obj(map_scale_group, ui_MapImage16);

    //*添加事件
    // TODO:子组件待定
    Epage_M.C_group = scr_group;
    Epage_M.N_group = map_group;
    Epage_M.N_obj = ui_MapRunD;
    lv_obj_add_event_cb(ui_Map, event_scr, LV_EVENT_FOCUSED, &Epage_M);
    lv_obj_add_event_cb(ui_Map, refresh_scr, LV_EVENT_FOCUSED, NULL);
    lv_obj_add_event_cb(ui_Map, event_enter_group, LV_EVENT_KEY, &Epage_M);
    lv_obj_add_event_cb(ui_Map, event_leave_group, LV_EVENT_LONG_PRESSED_REPEAT, &Epage_M);

    return ui_Map;
}

void refresh_img(int pix_x, int pix_y)
{
    // lv_img_set_src(ui_Map_img, "V:/lvgl/jpg/map17.jpg");
    printf("refresh_img: %d, %d", pix_x, pix_y);
    lv_obj_align(ui_Map_img, LV_ALIGN_CENTER, pix_x, pix_y);
}