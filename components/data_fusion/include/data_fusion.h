#pragma once

typedef struct
{
    float speed;       // 融合速度 (m/s)
    float bias;        // MPU加速度计零偏 (m/s²)
    float uncertainty; // 状态不确定性
} SpeedState;

void fuse_speed(SpeedState *state,
                const float gps_speed,
                const float mpu_accel_x,
                const float mpu_accel_y);
void data_fusion_task(void *arg);
void get_data_for(int *distance, float *speed, int *hour, int *minute);
void get_gps_data(int *latitude, int *longitude);