#include "lv_port_indev.h"
#include "Encoder.h"
#include "esp_log.h"
lv_indev_t *indev;

lv_obj_t *rect;
lv_obj_t *circle;
/*将由库调用以读取编码器*/
static void encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data)
{
    data->enc_diff = get_pcnt_count_diff();
    data->state = (get_encoder_but_value() ? LV_INDEV_STATE_PR : LV_INDEV_STATE_REL);
    ESP_LOGD("lvgl_read", "编码器取值%d，按下状态%d", data->enc_diff, data->state);
}

void lv_port_indev_init(void)
{
    static lv_indev_drv_t indev_drv;
    
    encoder_pcnt_init();
    encoder_but_init();

    lv_indev_drv_init(&indev_drv); // 声明输入设备驱动
    indev_drv.type = LV_INDEV_TYPE_ENCODER;
    indev_drv.read_cb = encoder_read;
    indev = lv_indev_drv_register(&indev_drv); // 注册输入设备
    lv_group_t *g = lv_group_create();
    lv_group_set_default(g);
    lv_indev_set_group(indev, g); // 将触摸屏绑定到分组
}

lv_indev_t *get_encoder_indev(void)
{
    return indev;
}