#include <stdio.h>
#include "lvgl.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "lv_demos.h"

#include "driver/i2c.h"
#include "Hard_Start.h"
#include "JB_T6K71_lcd.h"
#include "UI.h"

#include "nmea_example.h"
#include "nmea.h"
#include "gpgll.h"
#include "gpgga.h"
#include "gprmc.h"
#include "gpgsa.h"
#include "gpvtg.h"
#include "gptxt.h"
#include "gpgsv.h"

#include "6050.h"

#include "data_fusion.h"

lv_obj_t *test10;
void test_lvgl(void);
void test_6050(void);
void test_gps(void);
void test_battery(void);

void app_main(void)
{
    lv_init();
    hard_star();
    ESP_LOGI("MAIN", "启动界面");
    ui_init();
    // test_lvgl();
    // test_6050();
    // test_gps();
        lcd_on(true);
    // xTaskCreatePinnedToCore(data_fusion_task, "data_fusion_task", 4096, NULL, 5, NULL, 1);
    while (1)
        vTaskDelay(1000 / portTICK_PERIOD_MS);
}

void scr_cb(lv_event_t *e)
{
    static uint8_t c = 0;
    lv_scr_load_anim(e->target, ((c++ % 4) < 2) ? LV_SCR_LOAD_ANIM_MOVE_BOTTOM : LV_SCR_LOAD_ANIM_MOVE_TOP, 1000, 0, false);
}

void scr1_cb(lv_event_t *e)
{
    lv_indev_set_group(get_encoder_indev(), (lv_group_t *)e->user_data);
    lv_group_focus_obj(test10);
}

void test_lvgl(void)
{
    ESP_LOGI("test", "test");
    set_comp_event_id(lv_event_register_id());
    lv_obj_t *scr1 = comp_scr_create();
    lv_obj_t *scr2 = comp_scr_create();
    lv_obj_t *scr3 = comp_scr_create();

    test10 = create_chrome_btn(scr1);
    lv_obj_align(test10, LV_ALIGN_TOP_MID, 0, 24);
    lv_label_set_text(get_child_comp(test10, C_label), "Text1");
    lv_img_set_src(get_child_comp(test10, C_img), &N_1);

    lv_obj_t *test11 = create_chrome_btn(scr1);
    lv_obj_align_to(test11, test10, LV_ALIGN_OUT_BOTTOM_MID, 0, 12);
    lv_label_set_text(get_child_comp(test11, C_label), "Text2");
    lv_img_set_src(get_child_comp(test11, C_img), &N_2);

    //
    lv_obj_t *test20 = create_chrome_btn(scr2);
    lv_obj_align(test20, LV_ALIGN_TOP_MID, 0, 24);
    lv_label_set_text(get_child_comp(test20, C_label), "Text3");
    lv_img_set_src(get_child_comp(test20, C_img), &N_3);

    lv_obj_t *test21 = create_chrome_btn(scr2);
    lv_obj_align_to(test21, test20, LV_ALIGN_OUT_BOTTOM_MID, 0, 12);
    lv_label_set_text(get_child_comp(test21, C_label), "Text4");
    lv_img_set_src(get_child_comp(test21, C_img), &N_4);

    //
    lv_obj_t *test30 = create_chrome_btn(scr3);
    lv_obj_align(test30, LV_ALIGN_TOP_MID, 0, 24);
    lv_label_set_text(get_child_comp(test30, C_label), "Text5");
    lv_img_set_src(get_child_comp(test30, C_img), &N_5);

    lv_obj_t *test31 = create_chrome_btn(scr3);
    lv_obj_align_to(test31, test30, LV_ALIGN_OUT_BOTTOM_MID, 0, 12);
    lv_label_set_text(get_child_comp(test31, C_label), "Text6");
    lv_img_set_src(get_child_comp(test31, C_img), &N_6);

    //
    lv_group_t *scr_group = lv_group_create();
    lv_indev_set_group(get_encoder_indev(), scr_group);
    lv_group_set_wrap(scr_group, false);
    lv_group_add_obj(scr_group, scr1); // 绑定到设定的组
    lv_group_add_obj(scr_group, scr2); // 绑定到设定的组
    lv_group_add_obj(scr_group, scr3); // 绑定到设定的组
    lv_group_focus_obj(scr1);

    lv_group_t *but1_g = lv_group_create();
    lv_group_add_obj(but1_g, test10);
    lv_group_add_obj(but1_g, test11);

    lv_group_t *but2_g = lv_group_create();
    lv_group_add_obj(but2_g, test20);
    lv_group_add_obj(but2_g, test21);

    lv_group_t *but3_g = lv_group_create();
    lv_group_add_obj(but3_g, test30);
    lv_group_add_obj(but3_g, test31);

    lv_obj_add_event_cb(scr1, scr1_cb, LV_EVENT_KEY, but1_g);
    // lv_obj_add_event_cb(scr2, scr_cb, LV_EVENT_ALL, NULL);
    // lv_obj_add_event_cb(scr3, scr_cb, LV_EVENT_ALL, NULL);

    lv_disp_load_scr(scr1);
    ESP_LOGI("test", "test");
}

void test_6050(void)
{
    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // ESP_LOGI("test", "test_6050");
        float acce_g[3], gyro_dps[3], roll, pitch;
        get_processed_data(acce_g, gyro_dps, &roll, &pitch);
        ESP_LOGI("test", "加速度X轴:%f Y轴:%f Z轴:%f...角速度X轴:%f Y轴:%f Z轴:%f...roll:%f pitch:%f", acce_g[0], acce_g[1], acce_g[2], gyro_dps[0], gyro_dps[1], gyro_dps[2], roll, pitch);
    }
}

void test_gps(void)
{

    while (1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        char fmt_buf[32];
        nmea_s *data;

        char *start;
        size_t length;
        nmea_example_read_line(&start, &length, 100 /* ms */);
        if (length == 0)
        {
            continue;
        }

        /* handle data */
        data = nmea_parse(start, length, 0);
        if (data == NULL)
        {
            printf("Failed to parse the sentence!\n");
            printf("  Type: %.5s (%d)\n", start + 1, nmea_get_type(start));
        }
        else
        {
            if (data->errors != 0)
            {
                printf("WARN: The sentence struct contains parse errors!\n");
            }

            if (NMEA_GPGGA == data->type)
            {
                printf("GPGGA sentence\n");
                nmea_gpgga_s *gpgga = (nmea_gpgga_s *)data;
                printf("Number of satellites: %d\n", gpgga->n_satellites);
                printf("Altitude: %f %c\n", gpgga->altitude,
                       gpgga->altitude_unit);
            }

            if (NMEA_GPGLL == data->type)
            {
                printf("GPGLL sentence\n");
                nmea_gpgll_s *pos = (nmea_gpgll_s *)data;
                printf("Longitude:\n");
                printf("  Degrees: %d\n", pos->longitude.degrees);
                printf("  Minutes: %f\n", pos->longitude.minutes);
                printf("  Cardinal: %c\n", (char)pos->longitude.cardinal);
                printf("Latitude:\n");
                printf("  Degrees: %d\n", pos->latitude.degrees);
                printf("  Minutes: %f\n", pos->latitude.minutes);
                printf("  Cardinal: %c\n", (char)pos->latitude.cardinal);
                strftime(fmt_buf, sizeof(fmt_buf), "%H:%M:%S", &pos->time);
                printf("Time: %s\n", fmt_buf);
            }

            if (NMEA_GPRMC == data->type)
            {
                printf("GPRMC sentence\n");
                nmea_gprmc_s *pos = (nmea_gprmc_s *)data;
                printf("Longitude:\n");
                printf("  Degrees: %d\n", pos->longitude.degrees);
                printf("  Minutes: %f\n", pos->longitude.minutes);
                printf("  Cardinal: %c\n", (char)pos->longitude.cardinal);
                printf("Latitude:\n");
                printf("  Degrees: %d\n", pos->latitude.degrees);
                printf("  Minutes: %f\n", pos->latitude.minutes);
                printf("  Cardinal: %c\n", (char)pos->latitude.cardinal);
                strftime(fmt_buf, sizeof(fmt_buf), "%d %b %T %Y", &pos->date_time);
                printf("Date & Time: %s\n", fmt_buf);
                printf("Speed, in Knots: %f\n", pos->gndspd_knots);
                printf("Track, in degrees: %f\n", pos->track_deg);
                printf("Magnetic Variation:\n");
                printf("  Degrees: %f\n", pos->magvar_deg);
                printf("  Cardinal: %c\n", (char)pos->magvar_cardinal);
                double adjusted_course = pos->track_deg;
                if (NMEA_CARDINAL_DIR_EAST == pos->magvar_cardinal)
                {
                    adjusted_course -= pos->magvar_deg;
                }
                else if (NMEA_CARDINAL_DIR_WEST == pos->magvar_cardinal)
                {
                    adjusted_course += pos->magvar_deg;
                }
                else
                {
                    printf("Invalid Magnetic Variation Direction!\n");
                }

                printf("Adjusted Track (heading): %f\n", adjusted_course);
            }

            if (NMEA_GPGSA == data->type)
            {
                nmea_gpgsa_s *gpgsa = (nmea_gpgsa_s *)data;

                printf("GPGSA Sentence:\n");
                printf("  Mode: %c\n", gpgsa->mode);
                printf("  Fix:  %d\n", gpgsa->fixtype);
                printf("  PDOP: %.2lf\n", gpgsa->pdop);
                printf("  HDOP: %.2lf\n", gpgsa->hdop);
                printf("  VDOP: %.2lf\n", gpgsa->vdop);
            }

            if (NMEA_GPGSV == data->type)
            {
                nmea_gpgsv_s *gpgsv = (nmea_gpgsv_s *)data;

                printf("GPGSV Sentence:\n");
                printf("  Num: %d\n", gpgsv->sentences);
                printf("  ID:  %d\n", gpgsv->sentence_number);
                printf("  SV:  %d\n", gpgsv->satellites);
                printf("  #1:  %d %d %d %d\n", gpgsv->sat[0].prn, gpgsv->sat[0].elevation, gpgsv->sat[0].azimuth, gpgsv->sat[0].snr);
                printf("  #2:  %d %d %d %d\n", gpgsv->sat[1].prn, gpgsv->sat[1].elevation, gpgsv->sat[1].azimuth, gpgsv->sat[1].snr);
                printf("  #3:  %d %d %d %d\n", gpgsv->sat[2].prn, gpgsv->sat[2].elevation, gpgsv->sat[2].azimuth, gpgsv->sat[2].snr);
                printf("  #4:  %d %d %d %d\n", gpgsv->sat[3].prn, gpgsv->sat[3].elevation, gpgsv->sat[3].azimuth, gpgsv->sat[3].snr);
            }

            if (NMEA_GPTXT == data->type)
            {
                nmea_gptxt_s *gptxt = (nmea_gptxt_s *)data;

                printf("GPTXT Sentence:\n");
                printf("  ID: %d %d %d\n", gptxt->id_00, gptxt->id_01, gptxt->id_02);
                printf("  %s\n", gptxt->text);
            }

            if (NMEA_GPVTG == data->type)
            {
                nmea_gpvtg_s *gpvtg = (nmea_gpvtg_s *)data;

                printf("GPVTG Sentence:\n");
                printf("  Track [deg]:   %.2lf\n", gpvtg->track_deg);
                printf("  Speed [kmph]:  %.2lf\n", gpvtg->gndspd_kmph);
                printf("  Speed [knots]: %.2lf\n", gpvtg->gndspd_knots);
            }

            nmea_free(data);
        }
    }
}

void test_battery(void)
{
    uint8_t buf[1] = {0x02};
    uint8_t buff[2]={0,0};
   esp_err_t ret = i2c_master_write_read_device(I2C_NUM_0, 0x64, buf, 1, buff, 2, 100 / portTICK_PERIOD_MS);
    ESP_LOGW("LCD", "电荷量16进制:%x %x,有效性:%d", buff[0], buff[1], ret);
}