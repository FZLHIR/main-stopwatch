#pragma once
#include "chrome_btn.h"
#include "comp_scr.h"

#include "def.h"
#include "lvgl.h"

void set_comp_event_id(uint32_t id);
uint32_t get_comp_event_id();

lv_obj_t *get_child_comp(lv_obj_t *comp, uint32_t child_idx);

void get_child_event_cb(lv_event_t *e);
void del_child_event_cb(lv_event_t *e);
