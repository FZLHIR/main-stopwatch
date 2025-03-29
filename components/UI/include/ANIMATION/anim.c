#include "anim.h"
#include <stdio.h>

uint8_t val = 0;

void anim_cb_set_val(lv_anim_t *a, int32_t v)
{
    lv_obj_t *cb_obj = (lv_obj_t *)a->user_data;
    if (val == 0)
        lv_obj_set_x(cb_obj, v);
    else if (val == 1)
        lv_obj_set_y(cb_obj, v);
    else if (val == 2)
        lv_obj_set_style_opa(cb_obj, v, 0);
}

int32_t anim_cb_get_val(lv_anim_t *a)
{
    lv_obj_t *cb_obj = (lv_obj_t *)a->user_data;
    if (val == 0)
        return lv_obj_get_x_aligned(cb_obj);
    else if (val == 1)
        return lv_obj_get_y_aligned(cb_obj);
    else if (val == 2)
        return lv_obj_get_style_opa(cb_obj, 0);
    return 0;
}

void anim_cb_del(lv_anim_t *a)
{
    lv_mem_free(a->user_data);
    a->user_data = NULL;
}

bool set_anim(lv_obj_t *obj, int delay, int speed, bool run_now, int path, int value)
{
    lv_anim_t Anim_act;
    lv_anim_set_user_data(&Anim_act, obj); // 动画对象
    lv_anim_init(&Anim_act);
    lv_anim_set_delay(&Anim_act, delay); // 开始动画前延时
    lv_anim_set_time(&Anim_act, 100 * speed);
    lv_anim_set_early_apply(&Anim_act, run_now); // 是否从设定默认值执行/从对象当前位置执行动画
    switch (path)                                // 动画路径
    {
    case 1:
        lv_anim_set_path_cb(&Anim_act, lv_anim_path_step);
        break;
    case 2:
        lv_anim_set_path_cb(&Anim_act, lv_anim_path_ease_in_out);
        break;
    case 3:
        lv_anim_set_path_cb(&Anim_act, lv_anim_path_ease_in);
        break;
    case 4:
        lv_anim_set_path_cb(&Anim_act, lv_anim_path_ease_out);
        break;
    case 5:
        lv_anim_set_path_cb(&Anim_act, lv_anim_path_overshoot);
        break;
    case 6:
        lv_anim_set_path_cb(&Anim_act, lv_anim_path_bounce);
    default:
        lv_anim_set_path_cb(&Anim_act, lv_anim_path_linear);
        break;
    }
    switch (value)
    {
    case 0:
        lv_anim_set_values(&Anim_act, 0, 320);
        val = 1;
        break;
    case 1:
        lv_anim_set_values(&Anim_act, 0, -320);
        val = 1;
        break;
    case 2:
        lv_anim_set_values(&Anim_act, 0, -240);
        val = 0;
        break;
    case 3:
        lv_anim_set_values(&Anim_act, 0, 240);
        val = 0;
        break;
        // 上出屏幕，下进屏幕
    case 4:
        lv_anim_set_values(&Anim_act, 320, 0);
        val = 1;
        break;
    case 5:
        lv_anim_set_values(&Anim_act, -320, 0);
        val = 1;
        break;
    case 6:
        lv_anim_set_values(&Anim_act, -240, 0);
        val = 0;
        break;
    case 7:
        lv_anim_set_values(&Anim_act, 240, 0);
        val = 0;
        break;
    case 10:
        lv_anim_set_values(&Anim_act, 255, 100);
        val = 2;
        break;
    default:
        printf("set_anim error\n");
        return false;
        break;
    }

    lv_anim_set_get_value_cb(&Anim_act, &anim_cb_get_val);  // 动画当前值获取
    lv_anim_set_custom_exec_cb(&Anim_act, anim_cb_set_val); // 动画值修改

    lv_anim_set_deleted_cb(&Anim_act, anim_cb_del); // 动画结束删除
    lv_anim_start(&Anim_act);
    return true;
}

void anim_def(lv_obj_t *obj,int v)
{
    set_anim(obj, 0, 10, 0, 0, v); // 动画对象，延时，速度，是否立即执行，路径，值
}
    /*
    ? lv_anim_path_linear      匀速
    ? lv_anim_path_step        到点闪现
    ? lv_anim_path_ease_in_out 老司机（起步和刹停都很缓慢）
    ? lv_anim_path_ease_in     只会起步
    ? lv_anim_path_ease_out    只会刹停
    ? lv_anim_path_overshoot   会刹不住然后拉回来
    ? lv_anim_path_bounce      反弹
    */