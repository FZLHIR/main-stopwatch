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

    ESP_LOGI("MAIN", "right");
    lv_demo_music(); // 运行音乐播放器demo
    while (1)
    {
        lv_task_handler();                   // 通知lvgl运行任务处理
        vTaskDelay(30 / portTICK_PERIOD_MS); // 释放线程 延时10ms
    }
}