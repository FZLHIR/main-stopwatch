#include <stdio.h>
#include <math.h>
#include "data_fusion.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

#include "6050.h"
#include "nmea_example.h"
#include "nmea.h"
#include "gpgll.h"
#include "gpgga.h"
#include "gprmc.h"
#include "gpgsa.h"
#include "gpvtg.h"
#include "gptxt.h"
#include "gpgsv.h"
// todo速度
//  状态变量定义

const float def_longitude = 23.730160f; // 经度 (deg)x
const float def_latitude = 19.8897f;    // 纬度 (deg)y
float dec_longitude = 0.0f;
float dec_latitude = 0.0f;
// 卡尔曼预测-更新周期
#define KF_CYCLE_MS 100 // 与MPU采样率同步

int a = 0;
int b = 0;
int c = 0;
float d = 0.0f;
float e = 0.0f;

void fuse_speed(SpeedState *state,
                const float gps_speed,
                const float mpu_accel_x,
                const float mpu_accel_y)
{
    static uint16_t gps_count = 1;
    // const float mpu_accel = dsps_sqrtf_f32_ansi(mpu_accel_x * mpu_accel_x + mpu_accel_y * mpu_accel_y);
    // 预测阶段（基于MPU加速度）
    float dt = KF_CYCLE_MS / 1000.0f;                 // t
    state->speed += (mpu_accel_x + state->bias) * dt; // v=v0+a*t
    state->uncertainty += 0.5f;                       // 状态不确定性增大/imx权值降低  uncertainty=uncertainty0+0.1

    if (gps_speed > 0 || gps_count < 50) // GPS数据可用时更新
    {
        float R = fmaxf(0.3f, 0.1f * fabsf(state->speed));       // 动态观测噪声
        float k = state->uncertainty / (state->uncertainty + R); // GPS噪声  k=1/(1+R)
        state->speed += k * (gps_speed - state->speed);          // v=v0+k*(v1-v0)
        state->bias += k * 0.5f * (gps_speed - state->speed);    // 零偏缓慢跟踪  bias=bias0+k*(v1-v0)
        state->uncertainty *= (1 - k);                           // 状态不确定性减小  uncertainty=uncertainty0*(1-k)
    }
    if (gps_count < 50)
        gps_count++;
}

// todo位置
//  typedef struct
//  {
//      double latitude;  // 纬度 (deg)
//      double longitude; // 经度 (deg)
//      float heading;    // 航向 (deg)
//  } PositionState;

// void update_position(PositionState *pos,
//                      const SpeedState *speed,
//                      const float delta_heading,
//                      const float dt)
// {
//     // 使用融合速度和航向推算位置
//     float distance = speed->speed * dt; // 单位：米
//     float rad_heading = (pos->heading + delta_heading) * M_PI / 180.0f;// 航向转弧度   rad_heading=heading*pi/180

//     // 简化的平面坐标更新（小范围近似）
//     pos->latitude += (distance * cosf(rad_heading)) / 111319.5f; // 1度≈111km
//     pos->longitude += (distance * sinf(rad_heading)) / (111319.5f * cosf(pos->latitude * M_PI / 180.0f));

//     // 航向更新（融合陀螺仪和GPS航向）
//     pos->heading += delta_heading * 0.9f + get_gps_heading() * 0.1f; // 权重可调
// }

// todo坡度
// float fuse_slope(const float gps_altitude_diff,
//                  const float mpu_pitch,
//                  const float distance)
// {
//     // GPS坡度计算（需要连续海拔差）
//     float gps_slope = atan2f(gps_altitude_diff, distance) * 180.0f / M_PI;

//     // 动态加权融合
//     float alpha = fminf(fabsf(gps_altitude_diff) / 10.0f, 1.0f); // 海拔变化越大越信任GPS
//     return alpha * gps_slope + (1 - alpha) * mpu_pitch;
// }

// todoGPS质量
//  typedef enum
//  {
//      GPS_GOOD = 0, // HDOP < 1.5, speed_accuracy < 0.5m/s
//      GPS_MEDIUM,   // HDOP < 3.0, speed_accuracy < 1.0m/s
//      GPS_POOR      // HDOP >=3.0 或速度波动>2m/s
//  } GpsQuality;

// GpsQuality check_gps_quality(const GpsData *gps)
// {
//     if (gps->hdop < 1.5f && gps->speed_accuracy < 0.5f)
//         return GPS_GOOD;
//     if (gps->hdop < 3.0f && gps->speed_accuracy < 1.0f)
//         return GPS_MEDIUM;
//     return GPS_POOR;
// }

// void seee(void)
// {
//     a = b = c = 0;
//     d = 0.0f;
// }

void data_fusion_task(void *arg)
{
    // 初始化状态变量
    SpeedState speed_state = {0.0f, 0.0f, 0.0f};
    int8_t gps_rade = 0;
    int32_t time = 0;
    float gps_speed = 0.0f;
    // seee();

    while (1)
    {
        // 读取GPS数据
        if (gps_rade++ == 10)
        {
            gps_rade = 0;
            char fmt_buf[32] = {0};
            nmea_s *data;
            char *start;
            size_t length;
            nmea_example_read_line(&start, &length, 100 /* ms */);
            if (length == 0)
                continue;
            /* handle data */
            data = nmea_parse(start, length, 0);
            if (data == NULL)
            {
                ESP_LOGW("MAIN", "无法解析句子!\n");
                ESP_LOGW("MAIN", "  Type: %.5s (%d)\n", start + 1, nmea_get_type(start));
            }
            else
            {
                if (data->errors != 0)
                    ESP_LOGW("MAIN", "句子结构包含解析错误!\n");

                if (NMEA_GPGGA == data->type)
                {
                    ESP_LOGI("MAIN", "GPGGA 句子\n");
                    nmea_gpgga_s *gpgga = (nmea_gpgga_s *)data;
                    ESP_LOGI("MAIN", "卫星数: %d\n", gpgga->n_satellites);
                    ESP_LOGI("MAIN", "高度: %f %c\n", gpgga->altitude,
                             gpgga->altitude_unit);
                }
                if (NMEA_GPRMC == data->type)
                {
                    ESP_LOGI("MAIN", "GPRMC 句子\n");
                    nmea_gprmc_s *pos = (nmea_gprmc_s *)data;
                    ESP_LOGI("MAIN", "经度:\n");
                    ESP_LOGI("MAIN", "度: %d\n", pos->longitude.degrees);
                    ESP_LOGI("MAIN", "分: %f\n", pos->longitude.minutes);
                    ESP_LOGI("MAIN", "经度: %c\n", (char)pos->longitude.cardinal);
                    ESP_LOGI("MAIN", "纬度:\n");
                    ESP_LOGI("MAIN", "度: %d\n", pos->latitude.degrees);
                    ESP_LOGI("MAIN", "分: %f\n", pos->latitude.minutes);
                    ESP_LOGI("MAIN", "纬度: %c\n", (char)pos->latitude.cardinal);
                    strftime(fmt_buf, sizeof(fmt_buf), "%d %b %T %Y", &pos->date_time);
                    ESP_LOGI("MAIN", "日期 & 时间: %s\n", fmt_buf);
                    ESP_LOGI("MAIN", "速度，以节为单位: %f\n", pos->gndspd_knots);
                    ESP_LOGI("MAIN", "轨道，以度为单位: %f\n", pos->track_deg);
                    gps_speed = (float)pos->gndspd_knots * 0.514;
                    if (pos->longitude.minutes > 10)
                    {
                        dec_longitude = def_longitude - pos->longitude.minutes;
                        dec_latitude = def_latitude - pos->latitude.minutes;
                    }
                }
            }
        }

        // 读取MPU6050数据
        float acce_g[3],
            gyro_dps[3], roll, pitch;
        get_processed_data(acce_g, gyro_dps, &roll, &pitch);
        // 融合速度
        static bool bias = true;
        if (bias)
            speed_state.bias = -acce_g[0]; // 零偏初始化
        bias = false;
        fuse_speed(&speed_state, gps_speed ? gps_speed : 0.0f, acce_g[0], acce_g[1]);

        // 输出融合结果
        ESP_LOGI("MAIN", "x轴加速度：%.2f m/s^2", acce_g[0]);
        ESP_LOGI("MAIN", "GPS 速度：%.2f m/s， 速度：%.2f m/s，MPU零偏：%.2f，不确定性：%.2f\n",
                 gps_speed, speed_state.speed, speed_state.bias, speed_state.uncertainty);
        d = speed_state.speed;
        e += (d * 0.1f);
        a = (int)e;
        // ESP_LOGW("MAIN", "%d,%d,%d,%.2f,%.2f\n", a, b, c, d, e);
        if (time++ == 600)
        {
            c++;
            if (c % 60 == 0)
            {
                b++;
                c = 0; // 重置c为0，以便每60分钟重新计数
            }
            time = 0;
            ESP_LOGW("MAIN", "一分钟");
        }
        // 延时
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void get_data_for(int *distance, float *speed, int *hour, int *minute)
{
    *distance = a;
    *speed = d;
    *hour = b;
    *minute = c;
}

void get_gps_data(int *latitude, int *longitude)
{
    ESP_LOGW("gps", "dlat%f,dlon%f", dec_latitude, dec_longitude);
    *latitude = (int)(dec_latitude / 0.00064f);
    *longitude = (int)(dec_longitude / 0.00054f);
}