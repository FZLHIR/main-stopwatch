#include "chrome_btn.h"
#include "flag.h"
#include "style.h"



lv_obj_t **set_child(lv_obj_t *father)
{
    lv_obj_t *c_Label; // 标签名
    c_Label = lv_label_create(father);
    lv_obj_set_size(c_Label, LV_SIZE_CONTENT, LV_SIZE_CONTENT); // 自动设置标签大小
    lv_obj_set_align(c_Label, LV_ALIGN_CENTER);
    lv_label_set_text(c_Label, "Button1");


    lv_obj_t *c_Con; // 图片背景容器
    c_Con = lv_obj_create(father);
    lv_obj_set_size(c_Con, 32, 32);
    lv_obj_set_align(c_Con, LV_ALIGN_LEFT_MID);
    lv_obj_set_style_pad_all(c_Con, 0, LV_PART_MAIN);
    set_style_def(c_Con, con);

    lv_obj_t *c_Img; // 图片
    c_Img = lv_img_create(c_Con);
    lv_obj_set_size(c_Img, 32, 32);
    lv_obj_set_align(c_Img, LV_ALIGN_CENTER);

    lv_obj_t **children = lv_mem_alloc(sizeof(lv_obj_t *) * 3); // 创建子组件指针数组
    children[0] = c_Label;
    children[1] = c_Con;
    children[2] = c_Img;
    return children;
}

lv_obj_t *create_chrome_btn(lv_obj_t *father) // 创建预定义的按键组件，传入父级
{
    /// 创建
    lv_obj_t *c_Btn;               // 创建按键对象指针
    c_Btn = lv_btn_create(father); // 绑定到创建的按钮上
    /// 设置基础属性 大小/位置
    lv_obj_set_size(c_Btn, 200, 36);           // 设置200x36的大小/pix
    /// 事件
    /// 标志
    set_flag_def(c_Btn, btn);
    /// 样式
    set_style_def(c_Btn, btn); // 设置默认样式

    /// 子组件
    lv_obj_t **children = set_child(c_Btn);
    uint32_t ID = get_comp_event_id();
    lv_obj_add_event_cb(c_Btn, get_child_event_cb, ID, children);              // 绑定子组件获取事件回调函数
    lv_obj_add_event_cb(c_Btn, del_child_event_cb, LV_EVENT_DELETE, children); // 绑定子组件删除事件回调函数
    return c_Btn;
}
