#include "SD.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "esp_vfs_fat.h"
#include "esp_err.h"
#include "esp_log.h"
#define MOUNT_POINT "/sdcard" // VFS挂载路径
static sdmmc_card_t *sdcard;

#if CONFIG_SD_MODE_SDIO
void SD_Init()
{
    // 1. 配置SDMMC主机
    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    host.flags = SDMMC_HOST_FLAG_4BIT;
    // host.max_freq_khz = SDMMC_FREQ_HIGHSPEED;
    ESP_LOGI("SD", "SD_Init");
    // 2. 定义GPIO配置（4线模式）
    sdmmc_slot_config_t slot_config = {
        .clk = SD_CLK,
        .cmd = SD_CMD,
        .d0 = SD_D0,
        .d1 = SD_D1,
        .d2 = SD_D2,
        .d3 = SD_D3,
        .cd = -1,
        .wp = SDMMC_SLOT_NO_WP,
        .width = 4,
        .flags = SDMMC_SLOT_FLAG_INTERNAL_PULLUP};
    // 3. 挂载文件系统
    esp_vfs_fat_mount_config_t mount_config = VFS_FAT_MOUNT_DEFAULT_CONFIG();
    mount_config.disk_status_check_enable = true;
    // ESP_ERROR_CHECK(sdmmc_host_init());//初始化SDMMC主机
    ESP_ERROR_CHECK(esp_vfs_fat_sdmmc_mount(MOUNT_POINT, &host, &slot_config, &mount_config, &sdcard));
}
#endif

#if CONFIG_SD_MODE_SPI
void SD_Init_SPI()
{
    spi_bus_config_t bus_config = {
        .mosi_io_num = SD_MOSI,
        .miso_io_num = SD_MISO,
        .sclk_io_num = SD_SCLK,
        .quadwp_io_num = -1, // 未使用
        .quadhd_io_num = -1, // 未使用
        .max_transfer_sz = 4096,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(SPI2_HOST, &bus_config, SPI_DMA_CH_AUTO));

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    sdspi_device_config_t device_config = {
        .host_id = SPI2_HOST,
        .gpio_cs = SD_CS,
        .gpio_cd = -1,
        .gpio_wp = -1,
        .gpio_int = -1,
        .duty_cycle_pos = 128};
    esp_vfs_fat_mount_config_t mount_config = VFS_FAT_MOUNT_DEFAULT_CONFIG();
    mount_config.disk_status_check_enable = true;
    mount_config.use_one_fat = true;
    ESP_ERROR_CHECK(esp_vfs_fat_sdspi_mount(MOUNT_POINT, &host, &device_config, &mount_config, &sdcard));

    // 4. 打印SD卡信息（可选）
    // sdmmc_card_print_info(stdout, sdcard);
}
#endif