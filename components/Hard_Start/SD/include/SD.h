#pragma once
#include "sdkconfig.h"
// SDIO
#if CONFIG_SD_MODE_SDIO
#define SD_CMD CONFIG_SD_CMD
#define SD_CLK CONFIG_SD_CLK
#define SD_D0 CONFIG_SD_D0
#define SD_D1 CONFIG_SD_D1
#define SD_D2 CONFIG_SD_D2
#define SD_D3 CONFIG_SD_D3
#endif
// SPI
#if CONFIG_SD_MODE_SPI
#define SD_CS CONFIG_SD_CS
#define SD_MOSI CONFIG_SD_MOSI
#define SD_MISO CONFIG_SD_MISO
#define SD_SCLK CONFIG_SD_SCLK
#endif
// 插入检测
#define SD_INSERT_CHECK CONFIG_SD_insertion
// 初始化
void SD_Init(void);
void SD_Init_SPI(void);
