#include "Hard_Start.h"
#include <esp_err.h>
#include <esp_log.h>
#include "lv_port_disp.h"
#include "lv_port_indev.h"

static const char *TAG = "硬件启动";

esp_err_t hard_star(void)
{
    ESP_LOGI(TAG, "显示部分启动");
    lv_port_disp_init();  // lvgl硬件接口初始化
    lv_port_indev_init(); // lvgl输入设备接口初始化
    return ESP_OK;
}