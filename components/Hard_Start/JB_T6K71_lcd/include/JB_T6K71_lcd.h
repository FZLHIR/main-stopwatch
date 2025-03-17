///@file JB_T6K71_lcd.h
///@brief T6K71 LCD 面板驱动 基于lvgl_esp32_drivers
///@author 苼/FZLHIR
///@version v1.0
///@date 2025.2.16

#ifndef JB_T6K71_LCD_H
#define JB_T6K71_LCD_H

#ifdef __cplusplus
extern "C"
{
#endif
/// INCLUDES
#include <stdbool.h>

#ifndef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#include "esp_err.h"

/// DEFINES
#define T6K71_CS CONFIG_lcd_cs
#define T6K71_RS CONFIG_lcd_rs
#define T6K71_WR CONFIG_lcd_wr
#define T6K71_RST CONFIG_lcd_rst
#define T6K71_BK CONFIG_lcd_bl
#define T6K71_DATA0 CONFIG_lcd_d0
#define T6K71_DATA1 CONFIG_lcd_d1
#define T6K71_DATA2 CONFIG_lcd_d2
#define T6K71_DATA3 CONFIG_lcd_d3
#define T6K71_DATA4 CONFIG_lcd_d4
#define T6K71_DATA5 CONFIG_lcd_d5
#define T6K71_DATA6 CONFIG_lcd_d6
#define T6K71_DATA7 CONFIG_lcd_d7
#define I80_BUS_WIDTH 8
#define DMA_BURST_SIZE 64
#define LCD_CLOCK_HZ (CONFIG_lcd_clock * 1000000) // 2M
#define LCD_BITS 16
#define LCD_X 240
#define LCD_Y 320

    /// GLOBAL PROTOTYPES
    void t6k71_init(lv_disp_drv_t *disp_drv);
    esp_err_t t6k71_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);

#ifdef __cplusplus
}
#endif

#endif /* JB_T6K71_LCD_H */