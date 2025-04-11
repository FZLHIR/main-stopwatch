#pragma once
#include "esp_err.h"
#include "driver/i2c.h"

esp_err_t mpu6050_init(i2c_port_t i2c_port);
void get_processed_data(float *acce_g, float *gyro_dps, float *roll, float *pitch);//前两个为float[3]