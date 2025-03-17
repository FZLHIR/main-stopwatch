///@file JB_T6K71_lcd.c
/// INCLUDES
#include "JB_T6K71_lcd.h"
#include "lvgl.h"
#include "esp_heap_caps.h" //堆分配
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_dev.h"
#include "esp_lcd_panel_ops.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include <stdlib.h>
#include <sys/cdefs.h> //条件编译
#include "sdkconfig.h"
#include "JB_T6K71_commands.h"
#include "ROM/ets_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_interface.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_commands.h"
#include "esp_check.h"
#if CONFIG_LCD_ENABLE_DEBUG_LOG
// 在包含 esp_log.h 之前，必须定义本地日志级别
// 设置此源文件的最大日志级别
#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG
#endif

/// DEFINES
#define TAG "LCD.t6k71"
#define reset_level 0
/// STATIC PROTOTYPES
void init_i80_bus(esp_lcd_panel_io_handle_t *io_handle, void *user_ctx);
static esp_err_t panel_t6k71_reset(void);
static esp_err_t panel_t6k71_standby(esp_lcd_panel_io_handle_t io, bool sleep);
static esp_err_t panel_t6k71_init(esp_lcd_panel_io_handle_t io);
esp_err_t send(esp_lcd_panel_io_handle_t io, uint16_t cmd, uint16_t data);
static bool lvgl_flush_ok(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx);
static esp_err_t panel_t6k71_sleep(esp_lcd_panel_io_handle_t io, bool sleep);
/// TYPEDEFS

/// GLOBAL FUNCTIONS
void t6k71_init(lv_disp_drv_t *disp_drv)
{
    esp_lcd_panel_io_handle_t panel_handle = NULL;
    gpio_config_t RST_gpio_config = {
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pin_bit_mask = 1ULL << T6K71_RST};
    
    ESP_ERROR_CHECK(gpio_config(&RST_gpio_config));
    init_i80_bus(&panel_handle, disp_drv);
    disp_drv->user_data = panel_handle;
    ESP_LOGD(TAG, "io %p", disp_drv->user_data);
    ESP_LOGD(TAG, "io %p", disp_drv);
    panel_t6k71_init(panel_handle);
}

esp_err_t t6k71_flush(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    ESP_LOGD(TAG, "io %p", drv->user_data);
    esp_lcd_panel_io_handle_t io = drv->user_data;
    int x_start = area->x1;
    int x_end = area->x2;
    int y_start = area->y1-1;
    int y_end = area->y2 + 4; // 加4是为了解决显示不全的问题
    // 旋转180度方向
    send(io, LCD_T_X_START, 239 - x_end);     // 设置行地址起点
    send(io, LCD_T_X_END, 239 - x_start);     // 设置行地址终点
    send(io, LCD_T_Y_START, 319 - y_end + 4); // 设置列地址起点
    send(io, LCD_T_Y_END, 319 - y_start);     // 设置列地址终点
    send(io, LCD_T_XADDR, 239 - x_start);     // 设置X起始地址
    send(io, LCD_T_YADDR, 319 - y_start +3); // 设置Y起始地址

    // 面板默认方向
    // send(io, LCD_T_ENTRY, S_HWAM_L);  // 旋转刷新方向
    // send(io, LCD_T_X_START, x_start); // 设置行地址起点
    // send(io, LCD_T_X_END, x_end);     // 设置行地址终点
    // send(io, LCD_T_Y_START, y_start); // 设置列地址起点
    // send(io, LCD_T_Y_END, y_end);     // 设置列地址终点
    // send(io, LCD_T_XADDR, x_start);   // 设置X起始地址
    // send(io, LCD_T_YADDR, y_start);   // 设置Y起始地址
    size_t len = (x_end - x_start) * (y_end - y_start) * 2; // 设置缓冲区
    // ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_color(io, LCD_T_WR, NULL, 0), TAG, "io tx color failed");
    // ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(io, LCD_T_WR, NULL, 0), TAG, "io tx 参数错误");
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_color(io, LCD_T_WR, color_map, len), TAG, "io tx color failed");
    ESP_LOGW(TAG, "发送坐标为: %d,%d,%d,%d", x_start, x_end, y_start, y_end);
    return ESP_OK;
}

/// STATIC FUNCTIONS

// 总线
void init_i80_bus(esp_lcd_panel_io_handle_t *io_handle, void *user_ctx)
{
    ESP_LOGI(TAG, "安装i80总线");
    esp_lcd_i80_bus_handle_t i80_handle = NULL;
    esp_lcd_i80_bus_config_t i80_config = {
        .clk_src = LCD_CLK_SRC_DEFAULT,
        .dc_gpio_num = T6K71_RS,
        .wr_gpio_num = T6K71_WR,
        .data_gpio_nums = {
            T6K71_DATA0,
            T6K71_DATA1,
            T6K71_DATA2,
            T6K71_DATA3,
            T6K71_DATA4,
            T6K71_DATA5,
            T6K71_DATA6,
            T6K71_DATA7,
        },
        .bus_width = I80_BUS_WIDTH,
        .max_transfer_bytes = LCD_X * LCD_Y * sizeof(uint16_t),
        .dma_burst_size = DMA_BURST_SIZE,
    };
    ESP_ERROR_CHECK(esp_lcd_new_i80_bus(&i80_config, &i80_handle));

    esp_lcd_panel_io_i80_config_t io_config = {
        .cs_gpio_num = T6K71_CS,
        .pclk_hz = LCD_CLOCK_HZ,
        .trans_queue_depth = 4,
        .dc_levels = {
            .dc_idle_level = 0,
            .dc_cmd_level = 0,
            .dc_dummy_level = 0,
            .dc_data_level = 1,
        },
        .flags = {
            .swap_color_bytes = !LV_COLOR_16_SWAP, // Swap can be done in LvGL (default) or DMA
        },
        .on_color_trans_done = lvgl_flush_ok,
        .user_ctx = user_ctx,
        .lcd_cmd_bits = LCD_BITS,
        .lcd_param_bits = LCD_BITS,
    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i80(i80_handle, &io_config, io_handle));
}

// 初始化
static esp_err_t panel_t6k71_reset(void) // 复位 t6k71 面板
{
    gpio_set_level(T6K71_RST, reset_level);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(T6K71_RST, !reset_level);
    vTaskDelay(pdMS_TO_TICKS(100));
    return ESP_OK;
}

static esp_err_t panel_t6k71_standby(esp_lcd_panel_io_handle_t io, bool sleep) // 待机模式
{
    if (sleep)
    {
        send(io, LCD_T_STANDBY, S_ST);   // 进入待机模式
        send(io, LCD_T_STANDBY, S_DEEP); // 进入深度待机
        ESP_LOGD(TAG, "T6K71 进入深度待机");
    }
    else
    {
        send(io, LCD_T_OSC, 0xffff); // 退出深度待机
        ets_delay_us(1000);
        send(io, LCD_T_OSC, 0xffff);
        ets_delay_us(1000);
        send(io, LCD_T_OSC, 0xffff);
        ets_delay_us(1000);
        ESP_LOGD(TAG, "T6K71 退出深度待机");
    }
    return ESP_OK;
}

static esp_err_t panel_t6k71_init(esp_lcd_panel_io_handle_t io) // 初始化 t6k71 面板
{
    // LCD 初始化
    panel_t6k71_reset();
    panel_t6k71_standby(io, false);        // 唤醒 LCD 面板
    send(io, LCD_T_NOP, S_NOP);            // 空操作
    send(io, LCD_T_STANDBY, S_DEEP_RESET); // 退出待机
    ets_delay_us(1000);
    send(io, LCD_T_OSC, 0x0001); // 开启震荡
    ets_delay_us(1000);
    // 显示设置
    send(io, LCD_T_OUTPUT, S_L);               // 设置输出  默认设置（0-128/320行）
    send(io, LCD_T_AC, S_FRAME);               // 设置帧翻转
    send(io, LCD_T_ENTRY, S_LWAM_L);           // 设置入口模式
    send(io, LCD_T_WIDE, S_WIDE);              // 设置宽度240
    send(io, LCD_T_DISP_FLIP, S_N);            // 设置颜色翻转
    send(io, LCD_T_DISP_SPEC, S_SPEC);         // 设置前/后空白周期
    send(io, LCD_T_DISP_REFU, S_REFU_N);       // 设置全屏刷新
    send(io, LCD_T_DISP_XY, S_XY_N);           // 设置XY加倍功能
    send(io, LCD_T_OUT_DISP_MODE, S_I80_MODE); // 设置外部显示信号模式
    send(io, LCD_T_FR, S_FR);                  // 设置FR周期调整
    ets_delay_us(1000);

    send(io, LCD_T_LTPS_ASW, S_ASW);   // 设置LTPS设置(AWS时序H与上升沿)
    send(io, LCD_T_LTPS_OE, S_OE);     // 设置LTPS设置(OE时序上升与下降)
    send(io, LCD_T_LTPS_ASWL, S_ASWL); // 设置LTPS设置(ASW L)
    send(io, LCD_T_LTPS_CKV, S_CKV);   // 设置LTPS设置(CKV)
    send(io, LCD_T_AMP, S_AMP_MID);    // 设置放大器设置
    send(io, LCD_T_OFFLINE, S_OFF);    // 设置断电序列设置
    send(io, LCD_T_OFFLINER, S_OFF);   // 设置断电序列设置
    // 电源设置
    send(io, LCD_T_POWV, S_POWV); // 设置电源1
    ets_delay_us(1000);
    send(io, LCD_T_POWX, S_POWX); // 设置电源2
    ets_delay_us(1000);
    send(io, LCD_T_POWB, S_POWB); // 设置电源3
    ets_delay_us(1000);
    send(io, LCD_T_POWE, S_POWE); // 设置电源4
    ets_delay_us(1000);
    // 伽马设置
    send(io, LCD_T_GAMM1, S_GAMM1_init);       // 设置Gamma1
    send(io, LCD_T_GAMM2, S_GAMM2_init);       // 设置Gamma2
    send(io, LCD_T_GAMM3, S_GAMM3_init);       // 设置Gamma3
    send(io, LCD_T_GAMM2, S_GAMM4_init);       // 设置Gamma2
    send(io, LCD_T_GAMM5, S_GAMM5_init);       // 设置Gamma5
    send(io, LCD_T_GAMMOFFS, S_GAMMOFFS_init); // 设置灰度偏移
    // 窗口设置
    send(io, LCD_T_SCREE1, S_SCREE1_init);   // 设置屏幕1起点
    send(io, LCD_T_SCRE1, S_SCRE1_init);     // 设置屏幕1终点
    send(io, LCD_T_X_START, S_X_START_init); // 设置行地址起点
    send(io, LCD_T_X_END, S_X_END_init);     // 设置行地址终点
    send(io, LCD_T_Y_START, S_Y_START_init); // 设置列地址起点
    send(io, LCD_T_Y_END, S_Y_END_init);     // 设置列地址终点
    send(io, LCD_T_XADDR, S_XADDR_init);     // 设置X起始地址
    send(io, LCD_T_YADDR, S_YADDR_init);     // 设置Y起始地址
    send(io, LCD_T_OSD, S_OSD_OFF);          // 设置OSD关闭
    // 显示控制
    send(io, LCD_T_AUTO, S_AUTO); // 设置自动序列控制
    panel_t6k71_sleep(io, false); // 显示开启
    ets_delay_us(1000);
    ESP_LOGW(TAG, "T6K71 面板初始化完成");
    send(io, LCD_T_ENTRY, S_HWAM_R); // 旋转刷新方向
    ESP_LOGW(TAG, "显示旋转180度");
    vTaskDelay(pdMS_TO_TICKS(1000));
    return ESP_OK;
}

esp_err_t send(esp_lcd_panel_io_handle_t io, uint16_t cmd, uint16_t data)
{
    ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(io, cmd, NULL, 0), TAG, "io tx 参数错误");
    if (data != 0xffff)
    {
        ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(io, -1, (uint16_t[]){data}, 2), TAG, "io tx 参数错误");
    }
    // ESP_RETURN_ON_ERROR(esp_lcd_panel_io_tx_param(io, cmd, (uint16_t[]){data}, 2), TAG, "io tx 参数错误");
    return ESP_OK; //(uint8_t[]){(data >> 8) & 0xFF,data & 0xFF,}
}

static bool lvgl_flush_ok(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
{
    // 屏幕刷新完成通知lvgl
    lv_disp_drv_t *disp_driver = user_ctx;
    lv_disp_flush_ready(disp_driver);
    return false;
}

static esp_err_t panel_t6k71_sleep(esp_lcd_panel_io_handle_t io, bool on_off) // 显示开关
{
    ESP_LOGW(TAG, "T6K71睡眠模式,%s", on_off ? "开启" : "关闭");
    if (on_off)
    {
        send(io, LCD_T_CON_DISP, S_CON_OFF);
        ESP_LOGD(TAG, "T6K71 睡眠");
    }
    else
    {
        send(io, LCD_T_CON_DISP, S_CON_ON);
        ESP_LOGD(TAG, "T6K71 显示");
    }
    return ESP_OK;
}
