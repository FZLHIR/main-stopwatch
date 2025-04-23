#include "Hard_Start.h"
#include <esp_err.h>
#include <esp_log.h>
#include "esp_timer.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_port_fs.h"
#include "jpeg.h"
#include "nmea_example.h"
#include "6050.h"
#include "driver/i2c.h"
static const char *TAG = "硬件";

void lvgl_task_handler(void *arg)
{
    lv_task_handler(); // 调用 LVGL 的任务处理函数
}

esp_err_t hard_star(void)
{
    ESP_LOGW(TAG, "lvgl启动");
    ESP_LOGI(TAG, "显示部分启动");
    lv_port_disp_init(); // lvgl硬件接口初始化
    ESP_LOGI(TAG, "输入部分启动");
    lv_port_indev_init(); // lvgl输入设备接口初始化
    ESP_LOGI(TAG, "文件系统部分启动");
    lv_port_fs_init();       // lvgl文件系统接口初始化
    jpeg_decoder_register(); // jpeg解码器注册

    const esp_timer_create_args_t lvgl_timer_args = {
        .callback = &lvgl_task_handler,  // 回调函数
        .name = "lvgl_task_processing"}; // 定时器名称

    esp_timer_handle_t lvgl_timer;                                    // 创建定时器任务来定期处理 LVGL 事务
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_timer_args, &lvgl_timer)); // 创建定时器
    ESP_LOGI(TAG, "显示事件处理部分启动");
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_timer, 5000)); // 每 5ms 处理一次 LVGL 事务
    ESP_LOGI(TAG, "NMEA部分启动");
    nmea_example_init_interface(); // NMEA 部分初始化（读取见nmea—main）

    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)CONFIG_MPU6050_SDA;
    conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
    conf.scl_io_num = (gpio_num_t)CONFIG_MPU6050_SCL;
    conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
    conf.master.clk_speed = 100000; // 400kHz
    conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    ESP_LOGI(TAG, "6050部分启动");
    mpu6050_init(I2C_NUM_0); // 6050 部分初始化
    uint8_t buffer[2] = {0x01, 0x20};
    i2c_master_write_to_device(I2C_NUM_0, 0x64, buffer, 2, 1000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "硬件部分启动完成");
    return ESP_OK;
}