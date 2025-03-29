#include <stdio.h>
#include "lvgl.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "lv_demos.h"

#include "Hard_Start.h"
#include "JB_T6K71_lcd.h"
#include "UI.h"

lv_obj_t *test10;
void test(void);

void app_main(void)
{
    lv_init();
    hard_star();
    ESP_LOGI("MAIN", "启动界面");
    ui_init();
    // test();
    lcd_on(true);
    // lv_demo_music();
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void scr_cb(lv_event_t *e)
{
    static uint8_t c = 0;
    lv_scr_load_anim(e->target, ((c++ % 4) < 2) ? LV_SCR_LOAD_ANIM_MOVE_BOTTOM : LV_SCR_LOAD_ANIM_MOVE_TOP, 1000, 0, false);
}

void scr1_cb(lv_event_t *e)
{
    lv_indev_set_group(get_encoder_indev(), (lv_group_t *)e->user_data);
    lv_group_focus_obj(test10);
}

void test(void)
{
    ESP_LOGI("MAIN", "test");
    set_comp_event_id(lv_event_register_id());
    lv_obj_t *scr1 = comp_scr_create();
    lv_obj_t *scr2 = comp_scr_create();
    lv_obj_t *scr3 = comp_scr_create();

    test10 = create_chrome_btn(scr1);
    lv_obj_align(test10, LV_ALIGN_TOP_MID, 0, 24);
    lv_label_set_text(get_child_comp(test10, C_label), "Text1");
    lv_img_set_src(get_child_comp(test10, C_img), &N_1);

    lv_obj_t *test11 = create_chrome_btn(scr1);
    lv_obj_align_to(test11, test10, LV_ALIGN_OUT_BOTTOM_MID, 0, 12);
    lv_label_set_text(get_child_comp(test11, C_label), "Text2");
    lv_img_set_src(get_child_comp(test11, C_img), &N_2);

    //
    lv_obj_t *test20 = create_chrome_btn(scr2);
    lv_obj_align(test20, LV_ALIGN_TOP_MID, 0, 24);
    lv_label_set_text(get_child_comp(test20, C_label), "Text3");
    lv_img_set_src(get_child_comp(test20, C_img), &N_3);

    lv_obj_t *test21 = create_chrome_btn(scr2);
    lv_obj_align_to(test21, test20, LV_ALIGN_OUT_BOTTOM_MID, 0, 12);
    lv_label_set_text(get_child_comp(test21, C_label), "Text4");
    lv_img_set_src(get_child_comp(test21, C_img), &N_4);

    //
    lv_obj_t *test30 = create_chrome_btn(scr3);
    lv_obj_align(test30, LV_ALIGN_TOP_MID, 0, 24);
    lv_label_set_text(get_child_comp(test30, C_label), "Text5");
    lv_img_set_src(get_child_comp(test30, C_img), &N_5);

    lv_obj_t *test31 = create_chrome_btn(scr3);
    lv_obj_align_to(test31, test30, LV_ALIGN_OUT_BOTTOM_MID, 0, 12);
    lv_label_set_text(get_child_comp(test31, C_label), "Text6");
    lv_img_set_src(get_child_comp(test31, C_img), &N_6);

    //
    lv_group_t *scr_group = lv_group_create();
    lv_indev_set_group(get_encoder_indev(), scr_group);
    lv_group_set_wrap(scr_group, false);
    lv_group_add_obj(scr_group, scr1); // 绑定到设定的组
    lv_group_add_obj(scr_group, scr2); // 绑定到设定的组
    lv_group_add_obj(scr_group, scr3); // 绑定到设定的组
    lv_group_focus_obj(scr1);
    
    lv_group_t *but1_g = lv_group_create();
    lv_group_add_obj(but1_g, test10); 
    lv_group_add_obj(but1_g, test11); 
    
    lv_group_t *but2_g = lv_group_create();
    lv_group_add_obj(but2_g, test20); 
    lv_group_add_obj(but2_g, test21); 
    
    lv_group_t *but3_g = lv_group_create();
    lv_group_add_obj(but3_g, test30); 
    lv_group_add_obj(but3_g, test31); 

    lv_obj_add_event_cb(scr1, scr1_cb, LV_EVENT_KEY, but1_g);
    // lv_obj_add_event_cb(scr2, scr_cb, LV_EVENT_ALL, NULL);
    // lv_obj_add_event_cb(scr3, scr_cb, LV_EVENT_ALL, NULL);

    lv_disp_load_scr(scr1);
    ESP_LOGI("MAIN", "test");
}