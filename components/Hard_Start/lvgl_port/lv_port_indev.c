#include "lv_port_indev.h"
#include "Encoder.h"
#include "esp_log.h"
lv_indev_t *indev_encoder;

/*将由库调用以读取编码器*/
static void encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    data->enc_diff = get_pcnt_count_diff();
    data->state = (get_encoder_but_value() ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL);
    ESP_LOGD("lvgl_read", "encoder_read，diff·%d，state·%d", data->enc_diff, data->state);
}

void lv_port_indev_init(void)
{
    static lv_indev_drv_t indev_drv;

    encoder_pcnt_init();
    encoder_but_init();

    lv_indev_drv_init(&indev_drv); // 声明输入设备驱动
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    indev_encoder = lv_indev_drv_register(&indev_drv); // 注册输入设备

    /*稍后，您应该使用 'lv_group_t * group = lv_group_create（）' 创建组，
     *使用 'lv_group_add_obj（group, obj）' 将对象添加到组中
     *并将此输入设备分配给组以在其中导航：
     *'lv_indev_set_group（indev_encoder,group）;'*/
}
