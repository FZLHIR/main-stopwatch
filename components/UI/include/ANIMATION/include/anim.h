#pragma once
#include "lvgl.h"

#define LU 0
#define LD 1
#define LL 2
#define LR 3
#define EU 4
#define ED 5
#define EL 6
#define ER 7

#define OPA 10

bool set_anim(lv_obj_t *obj, int delay, int speed, bool run_now, int path, int value);
void anim_def(lv_obj_t *obj, int v);
