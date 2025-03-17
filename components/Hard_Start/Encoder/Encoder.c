#include "driver/pulse_cnt.h"
#include "driver/gpio.h"
#include "driver/gpio_filter.h"
#include "esp_err.h"
#include "esp_log.h"

// 定义编码器连接的GPIO
#define ENCODER_A_GPIO CONFIG_ENCODER_A
#define ENCODER_B_GPIO CONFIG_ENCODER_B
#define ENCODER_S_GPIO CONFIG_ENCODER_S
pcnt_unit_handle_t pcnt_unit = NULL;

void encoder_pcnt_init(void)
{
    gpio_config_t io_conf = {
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pin_bit_mask = (1ULL << ENCODER_A_GPIO) | (1ULL << ENCODER_B_GPIO)};
    ESP_ERROR_CHECK(gpio_config(&io_conf));
    // 创建PCNT单元
    pcnt_unit_config_t unit_config = {
        .high_limit = 10000,       // 计数上限（防溢出）
        .low_limit = -10000,       // 计数下限
        .flags.accum_count = true, // 启用累计计数模式
        .intr_priority = 3         // 中断优先级
    };
    ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &pcnt_unit));

    // 配置通道A
    pcnt_chan_config_t chan_config_a = {
        .edge_gpio_num = ENCODER_A_GPIO, // A相作为脉冲输入
        .level_gpio_num = ENCODER_B_GPIO // B相作为控制方向
    };
    pcnt_channel_handle_t pcnt_chan_a = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_config_a, &pcnt_chan_a));
    // 设置通道A的计数模式
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_a, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_DECREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_a, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

    // 配置通道B
    pcnt_chan_config_t chan_config_b = {
        .edge_gpio_num = ENCODER_B_GPIO, // B相作为脉冲输入
        .level_gpio_num = ENCODER_A_GPIO // A相作为控制方向
    };
    pcnt_channel_handle_t pcnt_chan_b = NULL;
    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_unit, &chan_config_b, &pcnt_chan_b));
    // 设置通道B的计数模式
    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_chan_b, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_HOLD));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_chan_b, PCNT_CHANNEL_LEVEL_ACTION_INVERSE, PCNT_CHANNEL_LEVEL_ACTION_KEEP));

    // 设置毛刺滤波器
    pcnt_glitch_filter_config_t filter_config = {
        .max_glitch_ns = 10000,
    };
    ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(pcnt_unit, &filter_config));
    // 启用PCNT单元
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_unit));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_unit));
}

void encoder_but_init(void)
{
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_LOW_LEVEL,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pin_bit_mask = (1ULL << ENCODER_S_GPIO)};
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    gpio_glitch_filter_handle_t filter_handle = NULL;
    gpio_pin_glitch_filter_config_t filter = {
        .clk_src = GLITCH_FILTER_CLK_SRC_DEFAULT,
        .gpio_num = ENCODER_S_GPIO};
    gpio_new_pin_glitch_filter(&filter, &filter_handle);
    gpio_glitch_filter_enable(filter_handle);
}

int get_pcnt_count_diff(void)
{
    static int last_count = 0; // 静态变量保存上一次的计数值
    int current_count = 0;
    ESP_ERROR_CHECK(pcnt_unit_get_count(pcnt_unit, &current_count));
    current_count /= 4;                          // 四倍频率计数，实际值/4
    int count_diff = current_count - last_count; // 计算当前值与上次值的差值
    ESP_LOGD("Encoder", "count_diff: %d, current_count: %d,last_count: %d", count_diff,current_count,last_count);
    last_count = current_count;                  // 更新上次的计数值为当前值
    return count_diff;
}

bool get_encoder_but_value(void)
{
    bool but_value = !gpio_get_level(ENCODER_S_GPIO);
    return but_value;
}