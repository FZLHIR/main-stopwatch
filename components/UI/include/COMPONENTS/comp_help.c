#include "comp_help.h"
#include <stdint.h>

uint32_t COMP_EVENT_GET_CHILD_ID;

typedef struct
{
    uint32_t c_id;   // 子对象索引
    lv_obj_t *child; // 子对象指针
} ui_comp_get_child_t;

void set_comp_event_id(uint32_t id) // 设置组件事件ID
{
    COMP_EVENT_GET_CHILD_ID = id;
}

uint32_t get_comp_event_id() // 获取组件事件ID
{
    return COMP_EVENT_GET_CHILD_ID;
}

lv_obj_t *get_child_comp(lv_obj_t *comp, uint32_t child_id) // 从给定的组件对象中获取指定索引位置的子对象
{
    ui_comp_get_child_t id;                            // 用于获取子对象信息的结构体
    id.child = NULL;                                   // 初始化子对象指针为空
    id.c_id = child_id;                                // 子对象索引
    lv_event_send(comp, COMP_EVENT_GET_CHILD_ID, &id); // 发送获取子对象事件
    return id.child;                                   // 返回子对象指针
}

void get_child_event_cb(lv_event_t *e) // 处理获取组件子对象的事件
{
    lv_obj_t **c = lv_event_get_user_data(e);        // 获取事件的组件输入数据
    ui_comp_get_child_t *id = lv_event_get_param(e); // 获取事件来源传入的参数
    id->child = c[id->c_id];                         // 获取子对象指针
}

void del_child_event_cb(lv_event_t *e) // 处理删除组件子对象的事件
{
    lv_obj_t **c = lv_event_get_user_data(e); // 获取事件的用户数据
    lv_mem_free(c);                           // 释放内存
}
