#include "flag.h"

void set_flag_def(lv_obj_t *obj, int type) // 设置标志位
{
    lv_obj_clear_flag(obj, 0x7F8FFFFF); // 清除所有位确定初始值
    switch (type)
    {
    case label:
        lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_ON_FOCUS); // 聚焦时滚动到可见
        break;
    case btn:
        lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE | // 可点击
                                 LV_OBJ_FLAG_CLICK_FOCUSABLE |// 点击时聚焦
                                 LV_OBJ_FLAG_SCROLL_ON_FOCUS); 
        break;
    case img: // 图片不需要设置
        printf("img type \n");
        break;
    case sw:
        lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE |           // 可点击
                                 LV_OBJ_FLAG_CHECKABLE |       // 可切换
                                 LV_OBJ_FLAG_CLICK_FOCUSABLE | // 点击时聚焦
                                 LV_OBJ_FLAG_SCROLL_ON_FOCUS); // 聚焦时滚动到可见
        break;
    case tab | list:
        lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE |      // 可点击
                                 LV_OBJ_FLAG_SCROLLABLE | // 可滚动
                                 LV_OBJ_FLAG_SCROLL_ONE); // 一次滚一个
        break;
    default:
        printf("Error: Invalid flag type is %d\n", type);
        break;
    }
    lv_obj_add_flag(obj, LV_OBJ_FLAG_SCROLL_CHAIN |     // 滚动传播到父级
                             LV_OBJ_FLAG_SNAPPABLE |    // 可捕捉
                             LV_OBJ_FLAG_EVENT_BUBBLE); // 事件冒泡
}

/*
?    LV_OBJ_FLAG_HIDDEN           = (1L << 0),      < 隐藏对象。（就像它根本不存在一样）
?    LV_OBJ_FLAG_CLICKABLE        = (1L << 1),      < 使对象可被输入设备单击
?    LV_OBJ_FLAG_CLICK_FOCUSABLE  = (1L << 2),      < 单击时向对象添加焦点状态
?    LV_OBJ_FLAG_CHECKABLE        = (1L << 3),      < 单击对象时切换选中状态
?    LV_OBJ_FLAG_SCROLLABLE       = (1L << 4),      < 使对象可滚动
?    LV_OBJ_FLAG_SCROLL_ELASTIC   = (1L << 5),      < 允许在内部滚动，但速度较慢
?    LV_OBJ_FLAG_SCROLL_MOMENTUM  = (1L << 6),      < 使对象在“扔” 时进一步滚动
?    LV_OBJ_FLAG_SCROLL_ONE       = (1L << 7),      < 仅允许滚动一个可对齐的子项
?    LV_OBJ_FLAG_SCROLL_CHAIN_HOR = (1L << 8),      < 允许将水平滚动传播到父项
?    LV_OBJ_FLAG_SCROLL_CHAIN_VER = (1L << 9),      < 允许将垂直滚动传播到父项
     LV_OBJ_FLAG_SCROLL_CHAIN     = (LV_OBJ_FLAG_SCROLL_CHAIN_HOR | LV_OBJ_FLAG_SCROLL_CHAIN_VER),
?    LV_OBJ_FLAG_SCROLL_ON_FOCUS  = (1L << 10),     < 自动滚动对象以使其在聚焦时可见
?    LV_OBJ_FLAG_SCROLL_WITH_ARROW= (1L << 11),     < 允许使用箭头键滚动聚焦对象
     LV_OBJ_FLAG_SNAPPABLE        = (1L << 12),     < 如果在父对象上启用了滚动对齐，则可以对齐到此对象
?    LV_OBJ_FLAG_PRESS_LOCK       = (1L << 13),     < 即使按压从对象上滑落，也要按住对象
     LV_OBJ_FLAG_EVENT_BUBBLE     = (1L << 14),     < 将事件也传播到父级
?    LV_OBJ_FLAG_GESTURE_BUBBLE   = (1L << 15),     < 将手势传播到父级
?    LV_OBJ_FLAG_ADV_HITTEST      = (1L << 16),     < 允许执行更准确的命中（点击）测试。例如，考虑圆角。
?    LV_OBJ_FLAG_IGNORE_LAYOUT    = (1L << 17),     < 使对象可由布局定位
?    LV_OBJ_FLAG_FLOATING         = (1L << 18),     < 当父项滚动时不滚动对象并忽略布局
?    LV_OBJ_FLAG_OVERFLOW_VISIBLE = (1L << 19),     < 不将子项的内容剪切到父项的边界
*/
