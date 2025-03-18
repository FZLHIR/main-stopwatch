#include <stdio.h>
#include "Hard_Start.h"
#include "lvgl.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "lv_demos.h"

void app_main(void)
{
    lv_init();
    hard_star();

    ESP_LOGI("MAIN", "开始运行lvgl");
    lv_demo_music(); // 运行音乐播放器demo
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}