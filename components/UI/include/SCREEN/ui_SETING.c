#include "screen.h"

focus_group_t Epage_S = {
    .C_group = NULL,
    .N_group = NULL,
    .N_obj = NULL,
    .focus_run = true,
};

lv_obj_t *ui_SETINGscreen_init(void)
{
    lv_obj_t *ui_SETING = comp_scr_create();
    lv_obj_set_style_pad_all(ui_SETING, 0, LV_PART_MAIN); // 设置内边距

    // 按键列表容器
    lv_obj_t *ui_SETINGSet = lv_obj_create(ui_SETING);
    lv_obj_set_style_pad_all(ui_SETINGSet, 0, LV_PART_MAIN); // 设置内边距
    lv_obj_set_size(ui_SETINGSet, lv_pct(100), lv_pct(100));
    lv_obj_add_flag(ui_SETINGSet, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_style_radius(ui_SETINGSet, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_scrollbar_mode(ui_SETINGSet, LV_SCROLLBAR_MODE_OFF); // 关闭滚动条

    lv_obj_set_scroll_dir(ui_SETINGSet, LV_DIR_VER);         // 设置滚动方向
    lv_obj_set_flex_flow(ui_SETINGSet, LV_FLEX_FLOW_COLUMN); // 纵向排列子对象
    lv_obj_set_flex_align(ui_SETINGSet, LV_FLEX_ALIGN_START, // 子对象在父对象上的对齐方式
                          LV_FLEX_ALIGN_CENTER,              // 子对象在父对象上的交叉轴对齐方式
                          LV_FLEX_ALIGN_CENTER);             // 子对象在父对象上的跟踪轴对齐方式
    lv_obj_set_style_pad_row(ui_SETINGSet, 12, 0);           // 布局变化时的过度时间/ms
    //*创建子组件
    lv_obj_t *ui_SETINGXTong = create_chrome_btn(ui_SETINGSet);
    lv_label_set_text(get_child_comp(ui_SETINGXTong, C_label), "系统");
    lv_img_set_src(get_child_comp(ui_SETINGXTong, C_img), &Server);

    lv_obj_t *ui_SETINGLJie = create_chrome_btn(ui_SETINGSet);
    lv_label_set_text(get_child_comp(ui_SETINGLJie, C_label), "连接");
    lv_img_set_src(get_child_comp(ui_SETINGLJie, C_img), &WiFi);

    lv_obj_t *ui_SETINGWZi = create_chrome_btn(ui_SETINGSet);
    lv_label_set_text(get_child_comp(ui_SETINGWZi, C_label), "位置");
    lv_img_set_src(get_child_comp(ui_SETINGWZi, C_img), &Signpost);

    lv_obj_t *ui_SETINGXSi = create_chrome_btn(ui_SETINGSet);
    lv_label_set_text(get_child_comp(ui_SETINGXSi, C_label), "显示");
    lv_img_set_src(get_child_comp(ui_SETINGXSi, C_img), &LCD);

    lv_obj_t *ui_SETINGAQuan = create_chrome_btn(ui_SETINGSet);
    lv_label_set_text(get_child_comp(ui_SETINGAQuan, C_label), "安全");
    lv_img_set_src(get_child_comp(ui_SETINGAQuan, C_img), &Contact);

    lv_obj_t *ui_SETINGCXu = create_chrome_btn(ui_SETINGSet);
    lv_label_set_text(get_child_comp(ui_SETINGCXu, C_label), "程序");
    lv_img_set_src(get_child_comp(ui_SETINGCXu, C_img), &Code);

    lv_obj_t *ui_SETINGDCi = create_chrome_btn(ui_SETINGSet);
    lv_label_set_text(get_child_comp(ui_SETINGDCi, C_label), "电池");
    lv_img_set_src(get_child_comp(ui_SETINGDCi, C_img), &Battery);

    lv_obj_t *ui_SETINGGYu = create_chrome_btn(ui_SETINGSet);
    lv_label_set_text(get_child_comp(ui_SETINGGYu, C_label), "关于");
    lv_img_set_src(get_child_comp(ui_SETINGGYu, C_img), &Info);

    lv_obj_t *ui_SETINGQTa = create_chrome_btn(ui_SETINGSet);
    lv_label_set_text(get_child_comp(ui_SETINGQTa, C_label), "更多");
    lv_img_set_src(get_child_comp(ui_SETINGQTa, C_img), &View_More);

    //*添加访问组
    lv_group_add_obj(scr_group, ui_SETING); // 屏幕

    lv_group_add_obj(seting_group, ui_SETINGXTong);
    lv_group_add_obj(seting_group, ui_SETINGLJie);
    lv_group_add_obj(seting_group, ui_SETINGWZi);
    lv_group_add_obj(seting_group, ui_SETINGXSi);
    lv_group_add_obj(seting_group, ui_SETINGAQuan);
    lv_group_add_obj(seting_group, ui_SETINGCXu);
    lv_group_add_obj(seting_group, ui_SETINGDCi);
    lv_group_add_obj(seting_group, ui_SETINGGYu);
    lv_group_add_obj(seting_group, ui_SETINGQTa);

    
    //*添加事件
    // TODO:子组件待定
    Epage_S.C_group = scr_group;
    Epage_S.N_group = seting_group;
    Epage_S.N_obj = ui_SETINGXTong;
    lv_obj_add_event_cb(ui_SETING, event_scr, LV_EVENT_FOCUSED, &Epage_S);
    lv_obj_add_event_cb(ui_SETING, event_enter_group, LV_EVENT_KEY, &Epage_S);
    lv_obj_add_event_cb(ui_SETING, event_leave_group, LV_EVENT_LONG_PRESSED, &Epage_S);

    return ui_SETING;
}
