#pragma once
#include "lvgl.h"
#include "cb.h"
#include "comp_help.h"
#include "src.h"
#include "drive_g.h"


#include "freertos/FreeRTOS.h"

// SCREEN: ui_QDscreen
lv_obj_t * ui_QDscreen_init(void);

// SCREEN: ui_APP
lv_obj_t *ui_APPscreen_init(void);

// SCREEN: ui_SETING
lv_obj_t *ui_SETINGscreen_init(void);

// SCREEN: ui_Bicycle
lv_obj_t *ui_Bicyclescreen_init(void);
void re_data(void *arg);
// SCREEN: ui_Map
lv_obj_t *ui_Mapscreen_init(void);
void refresh_img(int pix_x, int pix_y);
