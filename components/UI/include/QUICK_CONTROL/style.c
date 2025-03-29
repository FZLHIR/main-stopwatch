#include "style.h"
#include <stdio.h>
#include "src.h"

void set_style_def(lv_obj_t *obj, int type) // 设置样式
{
    switch (type)
    {
    case label:
        break;
    case btn:
        lv_obj_set_style_bg_color(obj, lv_color_hex(0xFF9933), LV_PART_MAIN | LV_STATE_DEFAULT);   // 设置背景颜色
        lv_obj_set_style_text_color(obj, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT); // 设置文本颜色
        lv_obj_set_style_text_font(obj, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);  // 设置字体
        lv_obj_set_style_shadow_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);                    // 移除阴影
        lv_obj_set_style_text_font(obj, &pix, LV_PART_MAIN);
        break;
    case img:
        break;
    case sw:
        break;
    case tab:
        break;
    case list:
        break;
    case con:
        lv_obj_set_style_bg_color(obj, lv_color_hex(0xFFFFCC), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_img_tiled(obj, false, LV_PART_MAIN | LV_STATE_DEFAULT);
        // lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        break;
    default:
        break;
    }
}

/*
?    LV_STATE_DEFAULT (0x0000) 正常，释放状态
?    LV_STATE_CHECKED (0x0001) 切换或检查状态
?    LV_STATE_FOCUSED (0x0002) 通过键盘或编码器聚焦或通过触摸板/鼠标点击
?    LV_STATE_EDITED (0x0008) 由编码器编辑
?    LV_STATE_PRESSED (0x0020) 被按下
?    LV_STATE_SCROLLED (0x0040) 正在滚动
?    LV_STATE_DISABLED (0x0040) 禁用状态
?    LV_PART_MAIN 类似矩形的背景
?    LV_PART_SCROLLBAR 滚动条
?    LV_PART_INDICATOR 指标，例如用于滑块、条、开关或复选框的勾选框
?    LV_PART_KNOB 像手柄一样可以抓取调整值
?    LV_PART_SELECTED 表示当前选择的选项或部分
?    LV_PART_ITEMS 如果小部件具有多个相似元素（例如表格单元格）
?    LV_PART_TICKS 刻度上的刻度，例如对于图表或仪表
?    LV_PART_CURSOR 标记一个特定的地方，例如文本区域或图表的光标
*/