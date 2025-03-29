#pragma once
#include "lvgl.h"

#include "screen.h"
#include "anim.h"
#include "lv_port_indev.h"
typedef struct
{
    uint8_t C_num;       // 当前焦点的组位置(动画用)
    uint8_t N_num;       // 下一焦点的组位置
    lv_obj_t *N_obj;     // 下一焦点的对象
    lv_obj_t *scr_obj;   // 传入的屏幕对象
    lv_group_t *C_group; // 当前焦点的组
    lv_group_t *N_group; // 下一焦点的组
    bool focus_run;
} focus_group_t;

void wtf(lv_event_t *e);

void screen_change(lv_obj_t **target, lv_scr_load_anim_t fademode, void (*target_init)(void));

void event_scr(lv_event_t *e);//切屏（可选：传入obj指定要切换的屏幕）
void event_enter_group(lv_event_t *e);//进组
void event_leave_group(lv_event_t *e);//退组
void event_enter_con_group(lv_event_t *e);//退con组

// void ui_event_QDimg(lv_event_t *e);
// void ui_event_APP(lv_event_t *e);
// void ui_event_APPseting_Button(lv_event_t *e);
// void ui_event_APPmap_Button(lv_event_t *e);
// void ui_event_APPbicycle_Button(lv_event_t *e);
// void ui_event_SETING(lv_event_t *e);
// void ui_event_Bicycle(lv_event_t *e);
// void ui_event_BicycleRunU(lv_event_t *e);
// void ui_event_MapRunD(lv_event_t *e);
// void ui_event_Map(lv_event_t *e);
