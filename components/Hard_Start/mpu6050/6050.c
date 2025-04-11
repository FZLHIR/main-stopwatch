#include "6050.h"
#include "mpu6050.h"
#include "driver/i2c.h"

// MPU6050全局句柄
static mpu6050_handle_t mpu = NULL;

// 传感器校准参数
static mpu6050_raw_acce_value_t acce_offset = {0};
static mpu6050_raw_gyro_value_t gyro_offset = {0};

// 灵敏度参数
static float acce_sensitivity = 0;
static float gyro_sensitivity = 0;

static void calibrate_mpu6050(void);
// 初始化MPU6050
esp_err_t mpu6050_init(i2c_port_t i2c_port)
{
    // 创建I2C配置（用户已实现总线初始化）
    // 假设用户已完成i2c_param_config()和i2c_driver_install()

    // 创建MPU6050实例
    mpu = mpu6050_create(i2c_port, MPU6050_I2C_ADDRESS);
    if (!mpu)
        return ESP_FAIL;

    // 配置量程（示例使用±8g加速度计和±250dps陀螺仪）
    ESP_ERROR_CHECK(mpu6050_config(mpu, ACCE_FS_4G, GYRO_FS_500DPS));

    // 唤醒设备
    ESP_ERROR_CHECK(mpu6050_wake_up(mpu));

    // 获取灵敏度
    ESP_ERROR_CHECK(mpu6050_get_acce_sensitivity(mpu, &acce_sensitivity));
    ESP_ERROR_CHECK(mpu6050_get_gyro_sensitivity(mpu, &gyro_sensitivity));

    // 校准传感器（需在静止状态下执行）
    calibrate_mpu6050();

    return ESP_OK;
}

// 传感器校准（需在水平静止状态下调用）
static void calibrate_mpu6050(void)
{
    const int samples = 200;
    mpu6050_raw_acce_value_t raw_acce;
    mpu6050_raw_gyro_value_t raw_gyro;

    // 采集加速度计偏移
    for (int i = 0; i < samples; i++)
    {
        mpu6050_get_raw_acce(mpu, &raw_acce);
        acce_offset.raw_acce_x += raw_acce.raw_acce_x;
        acce_offset.raw_acce_y += raw_acce.raw_acce_y;
        acce_offset.raw_acce_z += raw_acce.raw_acce_z;
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
    acce_offset.raw_acce_x /= samples;
    acce_offset.raw_acce_y /= samples;
    acce_offset.raw_acce_z /= samples - (int)(acce_sensitivity * 1.0f); // 补偿重力

    // 采集陀螺仪偏移
    for (int i = 0; i < samples; i++)
    {
        mpu6050_get_raw_gyro(mpu, &raw_gyro);
        gyro_offset.raw_gyro_x += raw_gyro.raw_gyro_x;
        gyro_offset.raw_gyro_y += raw_gyro.raw_gyro_y;
        gyro_offset.raw_gyro_z += raw_gyro.raw_gyro_z;
        vTaskDelay(5 / portTICK_PERIOD_MS);
    }
    gyro_offset.raw_gyro_x /= samples;
    gyro_offset.raw_gyro_y /= samples;
    gyro_offset.raw_gyro_z /= samples;
}

// 获取预处理后的传感器数据
void get_processed_data(float *acce_g, float *gyro_dps, float *roll, float *pitch)
{
    // 原始数据读取
    mpu6050_raw_acce_value_t raw_acce;
    mpu6050_raw_gyro_value_t raw_gyro;
    mpu6050_get_raw_acce(mpu, &raw_acce);
    mpu6050_get_raw_gyro(mpu, &raw_gyro);

    // 加速度处理（去偏移 + 转换单位）
    acce_g[0] = (raw_acce.raw_acce_x - acce_offset.raw_acce_x) / acce_sensitivity;
    acce_g[1] = (raw_acce.raw_acce_y - acce_offset.raw_acce_y) / acce_sensitivity;
    acce_g[2] = (raw_acce.raw_acce_z - acce_offset.raw_acce_z) / acce_sensitivity;

    // 陀螺仪处理（去偏移 + 转换单位）
    gyro_dps[0] = (raw_gyro.raw_gyro_x - gyro_offset.raw_gyro_x) / gyro_sensitivity;
    gyro_dps[1] = (raw_gyro.raw_gyro_y - gyro_offset.raw_gyro_y) / gyro_sensitivity;
    gyro_dps[2] = (raw_gyro.raw_gyro_z - gyro_offset.raw_gyro_z) / gyro_sensitivity;

    // 计算姿态角（使用官方互补滤波）
    mpu6050_acce_value_t acce = {acce_g[0], acce_g[1], acce_g[2]};
    mpu6050_gyro_value_t gyro = {gyro_dps[0], gyro_dps[1], gyro_dps[2]};
    complimentary_angle_t angle;
    mpu6050_complimentory_filter(mpu, &acce, &gyro, &angle);

    *roll = angle.roll;
    *pitch = angle.pitch;
}