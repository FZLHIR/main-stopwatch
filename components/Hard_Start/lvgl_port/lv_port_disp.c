#include <stdbool.h>
#include "lv_port_disp.h"
#include "esp_heap_caps.h"
#include "JB_T6K71_lcd.h"

#define DISP_BUF_SIZE (240 * 320 )

void lv_port_disp_init(void)
{

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv); // 声明设备驱动到lvgl
    t6k71_init(&disp_drv);       // 屏幕初始化
    disp_drv.hor_res = 240;
    disp_drv.ver_res = 320;
    disp_drv.flush_cb = (void *)t6k71_flush; // 设置刷新函数

    static lv_disp_draw_buf_t draw_buf; // 分配缓存
    lv_color_t *buf1 = (lv_color_t *)heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t),  MALLOC_CAP_DMA);
    lv_color_t *buf2 = (lv_color_t *)heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t),  MALLOC_CAP_DMA);
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, DISP_BUF_SIZE); // 声明缓存到lvgl

    disp_drv.draw_buf = &draw_buf;
    // disp_drv.full_refresh = 1; // 强制全屏刷新
    lv_disp_drv_register(&disp_drv); // 注册设备驱动到lvgl
}
