#ifndef LV_PORT_FS_H
#define LV_PORT_FS_H

#ifdef __cplusplus
extern "C"
{
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

    void lv_port_fs_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*LV_PORT_FS_H*/
