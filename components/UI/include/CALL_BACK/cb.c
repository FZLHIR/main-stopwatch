#include "cb.h"
#include <stdio.h>
void screen_change(lv_obj_t **target, lv_scr_load_anim_t fademode, void (*target_init)(void))
{
    if (*target == NULL)
        target_init();
    lv_scr_load_anim(*target, fademode, 1000, 0, false);
}//todo后面重构

void wtf(lv_event_t *e)
{
   if (lv_event_get_code(e)<18)   
    printf("wtf\nthis is:%d\n",lv_event_get_code(e));

}

void event_scr(lv_event_t *e)
{
    // anim_def(ui_Bicycle, ED);
    // TODO: 屏幕切换动画兼容
    printf("loding:%d\n", ((focus_group_t *)e->user_data)->focus_run);
    if (((focus_group_t *)e->user_data)->focus_run)
    lv_disp_load_scr(((focus_group_t *)e->user_data)->scr_obj ? ((focus_group_t *)e->user_data)->scr_obj : e->target); // 切换屏幕
}

void event_enter_group(lv_event_t *e)
{
    focus_group_t *fg = (focus_group_t *)e->user_data;
    bool editing = lv_group_get_editing(fg->C_group); // 获取当前焦点编辑模式
    if (editing)
    {
        printf("editing\n");
        fg->focus_run = false;
        lv_indev_set_group(get_encoder_indev(), fg->N_group);
        lv_group_focus_obj(fg->N_obj);
    }
}

void event_leave_group(lv_event_t *e)
{
    // !退出编辑模式冒泡到页面执行
    focus_group_t *fg = (focus_group_t *)e->user_data;
    bool editing = lv_group_get_editing(fg->C_group);
    if (editing)
    {
        printf("navigation\n");
        lv_indev_set_group(get_encoder_indev(), fg->C_group);
        lv_group_focus_obj(e->target);
        lv_group_set_editing(fg->C_group, false); // 退出编辑模式
        fg->focus_run = true;
    }
}

void event_enter_con_group(lv_event_t *e)
{
    focus_group_t *fg = (focus_group_t *)e->user_data;
    // bool editing = lv_group_get_editing(fg->N_group); // 获取当前焦点编辑模式
    // if (editing)
    // {
        printf("con_editing\n");
        lv_indev_set_group(get_encoder_indev(), fg->C_group);
    // }
}

// // 启动
// void ui_event_QDimg(lv_event_t *e)
// {
//     if (lv_event_get_code(e) == LV_EVENT_KEY)
//         switch (*(uint8_t *)e->param)
//         {
//         case LV_KEY_ENTER:
//             screen_change(&ui_APP, LV_SCR_LOAD_ANIM_NONE, &ui_APP_screen_init);
//             anim_def(ui_APP, ER);
//             break;
//         case LV_KEY_LEFT:
//             screen_change(&ui_Bicycle, LV_SCR_LOAD_ANIM_NONE, &ui_Bicycle_screen_init);
//             anim_def(ui_Bicycle, ED);
//             break;
//         case LV_KEY_RIGHT:
//             screen_change(&ui_SETING, LV_SCR_LOAD_ANIM_NONE, &ui_SETING_screen_init);
//             anim_def(ui_SETING, EU);
//             break;
//         default:
//             break;
//         }
// }
// // APP
// void ui_event_APP(lv_event_t *e)
// {
//     if (lv_event_get_code(e) == LV_EVENT_LONG_PRESSED)
//     {
//         screen_change(&ui_QDscreen, LV_SCR_LOAD_ANIM_FADE_ON, &ui_QDscreen_screen_init);
//         anim_def(ui_APP, LR);
//     }
// }

// void ui_event_APPseting_Button(lv_event_t *e)
// {
//         screen_change(&ui_SETING, LV_SCR_LOAD_ANIM_NONE, &ui_SETING_screen_init);
//         anim_def(ui_SETING, EU);

// }

// void ui_event_APPmap_Button(lv_event_t *e)
// {
//     if (lv_event_get_code(e) == LV_EVENT_CLICKED)
//     {
//         screen_change(&ui_Map, LV_SCR_LOAD_ANIM_NONE, &ui_Map_screen_init);
//         anim_def(ui_Map, EL);
//         anim_def(ui_APP, LR);
//     }
// }

// void ui_event_APPbicycle_Button(lv_event_t *e)
// {
//     if (lv_event_get_code(e) == LV_EVENT_CLICKED)
//     {
//         screen_change(&ui_Bicycle, LV_SCR_LOAD_ANIM_NONE, &ui_Bicycle_screen_init);
//         anim_def(ui_Bicycle, EL);
//         anim_def(ui_APP, LR);
//     }
// }
// // 设置
// void ui_event_SETING(lv_event_t *e)
// {
//     static int num = 1;
//     lv_event_code_t event_code = lv_event_get_code(e);
//     if (event_code == LV_EVENT_MSG_RECEIVED)
//     {
//         lv_msg_t *m = lv_event_get_msg(e);
//         num = *(int *)lv_msg_get_payload(m);
//     }
//     if (lv_event_get_code(e) == LV_EVENT_LONG_PRESSED)
//     {
//         anim_def(ui_SETING, LU);
//         if (num == 1)
//             screen_change(&ui_QDscreen, LV_SCR_LOAD_ANIM_FADE_OUT, &ui_QDscreen_screen_init);
//         if (num == 2)
//             screen_change(&ui_APP, LV_SCR_LOAD_ANIM_FADE_ON, &ui_APP_screen_init);
//     }
// }
// // 骑行
// void ui_event_Bicycle(lv_event_t *e)
// {
//     if (lv_event_get_code(e) == LV_EVENT_LONG_PRESSED)
//     {
//         screen_change(&ui_APP, LV_SCR_LOAD_ANIM_FADE_ON, &ui_APP_screen_init);
//         anim_def(ui_Bicycle, LL);
//     }

//     if (lv_event_get_code(e) == LV_EVENT_KEY)
//     {
//         switch (*(uint8_t *)e->param)
//         {
//         case LV_KEY_LEFT:
//             screen_change(&ui_QDscreen, LV_SCR_LOAD_ANIM_FADE_OUT, &ui_QDscreen_screen_init);
//             anim_def(ui_Bicycle, LD);
//             break;
//         case LV_KEY_RIGHT:
//             screen_change(&ui_Map, LV_SCR_LOAD_ANIM_NONE, &ui_Map_screen_init);
//             anim_def(ui_Map, ED);
//             break;
//         default:
//             break;
//         }
//     }
// }
// void ui_event_BicycleRunU(lv_event_t *e)
// {
//     if (lv_event_get_code(e) == LV_EVENT_CLICKED)
//     {
//         screen_change(&ui_Map, LV_SCR_LOAD_ANIM_NONE, &ui_Map_screen_init);
//         anim_def(ui_Map, ED);
//     }
// }

// // 地图
// void ui_event_Map(lv_event_t *e)
// {
//     if (lv_event_get_code(e) == LV_EVENT_LONG_PRESSED)
//     {
//         screen_change(&ui_APP, LV_SCR_LOAD_ANIM_FADE_ON, &ui_APP_screen_init);
//         anim_def(ui_Map, LL);
//     }
//     if (lv_event_get_code(e) == LV_EVENT_KEY)
//         if (*(uint8_t *)e->param == LV_KEY_LEFT)
//         {
//             screen_change(&ui_Bicycle, LV_SCR_LOAD_ANIM_FADE_OUT, &ui_Bicycle_screen_init);
//             anim_def(ui_Map, LD);
//         }
// }

// void ui_event_MapRunD(lv_event_t *e)
// {
//     if (lv_event_get_code(e) == LV_EVENT_CLICKED)
//     {
//         screen_change(&ui_Bicycle, LV_SCR_LOAD_ANIM_FADE_OUT, &ui_Bicycle_screen_init);
//         anim_def(ui_Map, LD);
//     }
// }
