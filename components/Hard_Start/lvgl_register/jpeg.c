#include "jpeg.h"
#include "lvgl.h"
#include "esp_jpeg_common.h"
#include "esp_jpeg_dec.h"
#include "esp_log.h"

#define TAG "jpeg"

bool jpeg_check(jpeg_error_t err)
{
    switch (err)
    {
    case JPEG_ERR_OK:
        ESP_LOGI(TAG, "打开JPEG解码器成功");
        return true;
        break;
    case JPEG_ERR_FAIL:
        ESP_LOGE(TAG, "设备错误或输入流错误终止");
        return false;
        break;
    case JPEG_ERR_NO_MEM:
        ESP_LOGE(TAG, "映像内存不足");
        return false;
        break;
    case JPEG_ERR_NO_MORE_DATA:
        ESP_LOGE(TAG, "输入数据不足");
        return false;
        break;
    case JPEG_ERR_INVALID_PARAM:
        ESP_LOGE(TAG, "参数无效");
        return false;
        break;
    case JPEG_ERR_BAD_DATA:
        ESP_LOGE(TAG, "数据格式错误（可能损坏数据）");
        return false;
        break;
    case JPEG_ERR_UNSUPPORT_FMT:
        ESP_LOGE(TAG, "格式正确但不受支持");
        return false;
        break;
    case JPEG_ERR_UNSUPPORT_STD:
        ESP_LOGE(TAG, "不支持 JPEG 标准");
        return false;
        break;
    default:
        ESP_LOGE(TAG, "期望外的返回值");
        break;
    }
    return false;
}
/// 基于esp-iot-solution项目的解码器接入配置改写
static lv_fs_res_t load_image_file(const char *filename, uint8_t **buffer, size_t *size, bool read_head)
{
    uint32_t len;
    lv_fs_file_t f;
    lv_fs_res_t res = lv_fs_open(&f, filename, LV_FS_MODE_RD);
    if (res != LV_FS_RES_OK)
    {
        ESP_LOGE(TAG, "无法打开文件: %s", filename);
        return res;
    }

    lv_fs_seek(&f, 0, LV_FS_SEEK_END);
    lv_fs_tell(&f, &len);
    lv_fs_seek(&f, 0, LV_FS_SEEK_SET);

    if (read_head && len > 1024)
    {
        len = 1024;
    }
    else if (len <= 0)
    {
        lv_fs_close(&f);
        return LV_FS_RES_FS_ERR;
    }

    *buffer = malloc(len);
    if (!*buffer)
    {
        ESP_LOGE(TAG, "无法为文件分配内存: %s", filename);
        lv_fs_close(&f);
        return LV_FS_RES_OUT_OF_MEM;
    }

    uint32_t rn = 0;
    res = lv_fs_read(&f, *buffer, len, &rn);
    lv_fs_close(&f);

    if (res != LV_FS_RES_OK || rn != len)
    {
        free(*buffer);
        *buffer = NULL;
        ESP_LOGE(TAG, "读取文件失败: %s", filename);
        return LV_FS_RES_UNKNOWN;
    }
    *size = len;

    return LV_FS_RES_OK;
}
/// 封装好的jepg解码器函数
static lv_res_t jpeg_decode(uint8_t **out, uint32_t *w, uint32_t *h, const uint8_t *in, uint32_t insize)
{
    jpeg_error_t ret;
    jpeg_dec_config_t config = {
#if LV_COLOR_DEPTH == 32
        .output_type = JPEG_PIXEL_FORMAT_RGB888,
#elif LV_COLOR_DEPTH == 16
#if LV_BIG_ENDIAN_SYSTEM == 1 || LV_COLOR_16_SWAP == 1
        .output_type = JPEG_PIXEL_FORMAT_RGB565_BE,
#else
        .output_type = JPEG_PIXEL_FORMAT_RGB565_LE,
#endif
#else
#error Unsupported LV_COLOR_DEPTH
#endif
        .rotate = JPEG_ROTATE_0D,
    };

    jpeg_dec_handle_t jpeg_dec;
    jpeg_dec_open(&config, &jpeg_dec);
    if (!jpeg_dec)
    {
        ESP_LOGE(TAG, "Failed to open jpeg decoder");
        return LV_RES_INV;
    }

    jpeg_dec_io_t *jpeg_io = malloc(sizeof(jpeg_dec_io_t));
    jpeg_dec_header_info_t *out_info = malloc(sizeof(jpeg_dec_header_info_t));
    if (!jpeg_io || !out_info)
    {
        if (jpeg_io)
        {
            free(jpeg_io);
        }
        if (out_info)
        {
            free(out_info);
        }
        jpeg_dec_close(jpeg_dec);
        ESP_LOGE(TAG, "Failed to allocate memory for jpeg decoder");
        return LV_RES_INV;
    }

    jpeg_io->inbuf = (unsigned char *)in;
    jpeg_io->inbuf_len = insize;

    ret = jpeg_dec_parse_header(jpeg_dec, jpeg_io, out_info);
    if (ret == JPEG_ERR_OK)
    {

        *w = out_info->width;
        *h = out_info->height;
    }
    else
    {
        free(jpeg_io);
        free(out_info);
        jpeg_dec_close(jpeg_dec);
        ESP_LOGE(TAG, "Failed to parse jpeg header");
        return LV_RES_INV;
    }

    free(jpeg_io);
    free(out_info);
    jpeg_dec_close(jpeg_dec);

    return LV_RES_OK;
}
/// 封装好颜色格式函数
static void convert_color_depth(uint8_t *img, uint32_t px_cnt)
{
#if LV_COLOR_DEPTH == 32
    lv_color32_t *img_argb = (lv_color32_t *)img;
    lv_color_t c;
    lv_color_t *img_c = (lv_color_t *)img;
    uint32_t i;
    for (i = 0; i < px_cnt; i++)
    {
        c = lv_color_make(img_argb[i].ch.red, img_argb[i].ch.green, img_argb[i].ch.blue);
        img_c[i].ch.red = c.ch.blue;
        img_c[i].ch.blue = c.ch.red;
    }
#elif LV_COLOR_DEPTH == 16
    lv_color32_t *img_argb = (lv_color32_t *)img;
    lv_color_t c;
    uint32_t i;
    for (i = 0; i < px_cnt; i++)
    {
        c = lv_color_make(img_argb[i].ch.blue, img_argb[i].ch.green, img_argb[i].ch.red);
        img[i * 3 + 2] = img_argb[i].ch.alpha;
        img[i * 3 + 1] = c.full >> 8;
        img[i * 3 + 0] = c.full & 0xFF;
    }
#elif LV_COLOR_DEPTH == 8
    lv_color32_t *img_argb = (lv_color32_t *)img;
    lv_color_t c;
    uint32_t i;
    for (i = 0; i < px_cnt; i++)
    {
        c = lv_color_make(img_argb[i].ch.red, img_argb[i].ch.green, img_argb[i].ch.blue);
        img[i * 2 + 1] = img_argb[i].ch.alpha;
        img[i * 2 + 0] = c.full;
    }
#elif LV_COLOR_DEPTH == 1
    lv_color32_t *img_argb = (lv_color32_t *)img;
    uint8_t b;
    uint32_t i;
    for (i = 0; i < px_cnt; i++)
    {
        b = img_argb[i].ch.red | img_argb[i].ch.green | img_argb[i].ch.blue;
        img[i * 2 + 1] = img_argb[i].ch.alpha;
        img[i * 2 + 0] = b > 128 ? 1 : 0;
    }
#endif
}
/// 封装好jpg判断函数
static int is_jpg(const uint8_t *raw_data, size_t len)
{
    const uint8_t jpg_signature_JFIF[] = {0xFF, 0xD8, 0xFF, 0xE0, 0x00, 0x10, 0x4A, 0x46, 0x49, 0x46};
    const uint8_t jpg_signature_Adobe[] = {0xFF, 0xD8, 0xFF, 0xEE, 0x00, 0x0E, 0x41, 0x64, 0x6F, 0x62};
    if (len < sizeof(jpg_signature_JFIF))
    {
        return false;
    }
    return ((memcmp(jpg_signature_JFIF, raw_data, sizeof(jpg_signature_JFIF)) == 0) | (memcmp(jpg_signature_Adobe, raw_data, sizeof(jpg_signature_Adobe)) == 0));
}

/**
 * Get info about a PNG image
 * @param decoder 指向此函数所属的解码器的指针
 * @param src 可以是文件名或指向 C 数组的指针
 * @param header 在此处存储信息
 * @return LV_RES_OK：无错误;LV_RES_INV：无法获取信息
 */

static lv_res_t decoder_info(struct _lv_img_decoder_t *decoder, const void *src, lv_img_header_t *header)
{
    (void)decoder;                                    /*Unused*/
    lv_img_src_t src_type = lv_img_src_get_type(src); /*Get the source type*/

    lv_res_t lv_ret = LV_RES_OK;

    if (src_type == LV_IMG_SRC_VARIABLE)
        return LV_RES_INV;
    else if (src_type == LV_IMG_SRC_FILE)
    {
        const char *fn = src;
        uint8_t *load_img_data = NULL; /*Pointer to the loaded data. Same as the original file just loaded into the RAM*/
        size_t load_img_size;          /*Size of `load_img_data` in bytes*/

        if (!strcmp(lv_fs_get_ext(fn), "jpg"))
        {

            if (load_image_file(fn, &load_img_data, &load_img_size, true) != LV_FS_RES_OK)
            {
                if (load_img_data)
                    free(load_img_data);
                return LV_RES_INV;
            }

            uint32_t width, height;
            lv_ret = jpeg_decode(NULL, &width, &height, load_img_data, load_img_size);

            header->cf = LV_IMG_CF_TRUE_COLOR;
            header->w = width;
            header->h = height;
            free(load_img_data);
            return lv_ret;
        }
        return LV_RES_INV;
    }
    return LV_RES_INV; /*If didn't succeeded earlier then it's an error*/
}

/**
 * 打开 PNG 图像并返回确定的图像
 * @param decoder 指向此函数所属的解码器的指针
 * @param dsc 指向描述此解码会话的描述符的指针
 * @return LV_RES_OK：无错误;LV_RES_INV：无法获取信息
 */
static lv_res_t decoder_open(lv_img_decoder_t *decoder, lv_img_decoder_dsc_t *dsc)
{

    (void)decoder;               /*Unused*/
    lv_res_t lv_ret = LV_RES_OK; /*For the return values of PNG decoder functions*/

    uint8_t *img_data = NULL;
    uint32_t png_width;
    uint32_t png_height;

    if (dsc->src_type == LV_IMG_SRC_VARIABLE)
    {
        const lv_img_dsc_t *img_dsc = dsc->src;
        if (is_jpg((uint8_t *)((lv_img_dsc_t *)(dsc->src))->data, ((lv_img_dsc_t *)(dsc->src))->data_size) == true)
            lv_ret = jpeg_decode(&img_data, &png_width, &png_height, img_dsc->data, img_dsc->data_size);
        if (lv_ret != LV_RES_OK)
        {
            ESP_LOGE(TAG, "解码错误:%d", lv_ret);
            if (img_data != NULL)
                free(img_data);
            return LV_RES_INV;
        }
        else
        {
            convert_color_depth(img_data, png_width * png_height);
            dsc->img_data = img_data;
        }
        return lv_ret;
    }
    else if (dsc->src_type == LV_IMG_SRC_FILE)
    {
        const char *fn = dsc->src;

        if (!strcmp(lv_fs_get_ext(fn), "jpg"))
        {
            uint8_t *load_img_data = NULL; /*Pointer to the loaded data. Same as the original file just loaded into the RAM*/
            size_t load_img_size;          /*Size of `load_img_data` in bytes*/

            if (load_image_file(fn, &load_img_data, &load_img_size, false) != LV_FS_RES_OK)
            {
                if (load_img_data)
                    free(load_img_data);
                return LV_RES_INV;
            }
            lv_ret = jpeg_decode(&img_data, &png_width, &png_height, load_img_data, load_img_size);
            free(load_img_data);
            if (lv_ret != LV_RES_OK)
            {
                ESP_LOGE(TAG, "解码错误:%d", lv_ret);
                if (img_data != NULL)
                    free(img_data);
                return LV_RES_INV;
            }
            else
            {
                if (strcmp(lv_fs_get_ext(fn), "jpg"))
                    convert_color_depth(img_data, png_width * png_height);
                dsc->img_data = img_data;
            }
            return lv_ret;
        }
    }
    return LV_RES_INV; /*If not returned earlier then it failed*/
}
/**
 * 释放分配的资源
 * @param decoder 指向此函数所属的解码器的指针
 * @param dsc 指向描述此解码会话的描述符的指针
 */
static void decoder_close(lv_img_decoder_t *decoder, lv_img_decoder_dsc_t *dsc)
{
    LV_UNUSED(decoder);
    // 所有的编码器都在执行函数中自动关闭了，这里没有要做的事情了
}

void jpeg_decoder_register(void)
{
    lv_img_decoder_t *esp_new_jpeg = lv_img_decoder_create();
    lv_img_decoder_set_info_cb(esp_new_jpeg, decoder_info);
    lv_img_decoder_set_open_cb(esp_new_jpeg, decoder_open);
    lv_img_decoder_set_close_cb(esp_new_jpeg, decoder_close);
}

// static lv_res_t decoder_read_line(lv_img_decoder_t *decoder, lv_img_decoder_dsc_t *dsc, lv_coord_t x, lv_coord_t y,
//                                   lv_coord_t len, uint8_t *buf)
// {
//     LV_UNUSED(decoder);

//     lv_res_t error = LV_RES_INV;
//     uint8_t *img_data = NULL;

//     uint8_t RGBA_depth = 0;
//     uint8_t RGB_depth = 0;

//     uint32_t png_width;
//     uint32_t png_height;

//     uint8_t *img_block_data;
//     uint32_t img_block_size;

// #if LV_COLOR_DEPTH == 32
//     RGBA_depth = 4;
//     RGB_depth = 4;
// #elif LV_COLOR_DEPTH == 16
//     RGBA_depth = 3;
//     RGB_depth = 2;
// #elif LV_COLOR_DEPTH == 8
//     RGBA_depth = 2;
// #elif LV_COLOR_DEPTH == 1
//     RGBA_depth = 2;
// #endif

//     if (dsc->src_type == LV_IMG_SRC_FILE)
//     {
//         uint32_t rn = 0;
//         lv_fs_res_t res;

//         image_decoder_t *img_dec = (image_decoder_t *)dsc->user_data;

//         lv_fs_file_t *lv_file_p = &(img_dec->io.lv_file);
//         if (!lv_file_p)
//         {
//             return LV_RES_INV;
//         }

//         int png_req_frame_index = y / img_dec->img_single_frame_height;
//         /*If line not from cache, refresh cache */
//         if (png_req_frame_index != img_dec->img_cache_frame_index)
//         {

//             if (png_req_frame_index == (img_dec->img_total_frames - 1))
//             {
//                 /*This is the last frame. */
//                 img_block_size = img_dec->dsc_size - (uint32_t)(img_dec->frame_base_offset[png_req_frame_index]);
//             }
//             else
//             {
//                 img_block_size =
//                     (uint32_t)(img_dec->frame_base_offset[png_req_frame_index + 1] - (uint32_t)(img_dec->frame_base_offset[png_req_frame_index]));
//             }

//             int next_read_pos = (int)(img_dec->frame_base_offset[png_req_frame_index]);
//             lv_fs_seek(lv_file_p, next_read_pos, LV_FS_SEEK_SET);
//             res = lv_fs_read(lv_file_p, img_dec->frame_cache, img_block_size, &rn);
//             if (res != LV_FS_RES_OK || rn != img_block_size)
//             {
//                 lv_fs_close(lv_file_p);
//                 return LV_RES_INV;
//             }

//             if (is_png(img_dec->frame_cache, rn) == true)
//             {
//                 error = libpng_decode32(&img_data, &png_width, &png_height, img_dec->frame_cache, rn);
//             }
//             else if (is_qoi(img_dec->frame_cache, rn) == true)
//             {
//                 error = qoi_decode32(&img_data, &png_width, &png_height, img_dec->frame_cache, rn);
//             }
//             else if (is_jpg(img_dec->frame_cache, rn) == true)
//             {
//                 error = jpeg_decode(&img_data, &png_width, &png_height, img_dec->frame_cache, rn);
//             }
//             if (error != LV_RES_OK)
//             {
//                 ESP_LOGE(TAG, "Decode error:%d", error);
//                 if (img_data != NULL)
//                 {
//                     free(img_data);
//                 }
//                 return LV_RES_INV;
//             }
//             else
//             {
//                 if (is_jpg(img_dec->frame_cache, rn) == false)
//                 {
//                     convert_color_depth(img_data, png_width * png_height);
//                     img_dec->img_depth = RGBA_depth;
//                 }
//                 else
//                 {
//                     img_dec->img_depth = RGB_depth;
//                 }
//                 memcpy(img_dec->frame_cache, img_data, png_width * png_height * img_dec->img_depth);
//                 if (img_data != NULL)
//                 {
//                     free(img_data);
//                 }
//             }
//             img_dec->img_cache_frame_index = png_req_frame_index;
//         }

//         uint8_t *cache = (uint8_t *)img_dec->frame_cache + x * img_dec->img_depth + (y % img_dec->img_single_frame_height) * img_dec->img_x_res * img_dec->img_depth;
//         memcpy(buf, cache, img_dec->img_depth * len);
//         return LV_RES_OK;
//     }
//     else if (dsc->src_type == LV_IMG_SRC_VARIABLE)
//     {
//         image_decoder_t *img_dec = (image_decoder_t *)dsc->user_data;

//         int spng_req_frame_index = y / img_dec->img_single_frame_height;

//         /*If line not from cache, refresh cache */
//         if (spng_req_frame_index != img_dec->img_cache_frame_index)
//         {
//             img_block_data = img_dec->frame_base_array[spng_req_frame_index];
//             if (spng_req_frame_index == (img_dec->img_total_frames - 1))
//             {
//                 /*This is the last frame. */
//                 const uint32_t frame_offset = (uint32_t)(img_block_data - img_dec->dsc_data);
//                 img_block_size = img_dec->dsc_size - frame_offset;
//             }
//             else
//             {
//                 img_block_size =
//                     (uint32_t)(img_dec->frame_base_array[spng_req_frame_index + 1] - img_block_data);
//             }

//             if (is_png(img_block_data, img_block_size) == true)
//             {
//                 error = libpng_decode32(&img_data, &png_width, &png_height, img_block_data, img_block_size);
//             }
//             else if (is_qoi(img_block_data, img_block_size) == true)
//             {
//                 error = qoi_decode32(&img_data, &png_width, &png_height, img_block_data, img_block_size);
//             }
//             else if (is_jpg(img_block_data, img_block_size) == true)
//             {
//                 error = jpeg_decode(&img_data, &png_width, &png_height, img_block_data, img_block_size);
//             }
//             if (error != LV_RES_OK)
//             {
//                 ESP_LOGE(TAG, "Decode error:%d", error);
//                 if (img_data != NULL)
//                 {
//                     free(img_data);
//                 }
//                 return LV_RES_INV;
//             }
//             else
//             {
//                 if (is_jpg(img_block_data, img_block_size) == false)
//                 {
//                     convert_color_depth(img_data, png_width * png_height);
//                     img_dec->img_depth = RGBA_depth;
//                 }
//                 else
//                 {
//                     img_dec->img_depth = RGB_depth;
//                 }
//                 memcpy(img_dec->frame_cache, img_data, img_dec->img_single_frame_height * img_dec->img_x_res * img_dec->img_depth);
//                 if (img_data != NULL)
//                 {
//                     free(img_data);
//                 }
//             }
//             img_dec->img_cache_frame_index = spng_req_frame_index;
//         }

//         uint8_t *cache = (uint8_t *)img_dec->frame_cache + x * img_dec->img_depth + (y % img_dec->img_single_frame_height) * img_dec->img_x_res * img_dec->img_depth;
//         memcpy(buf, cache, img_dec->img_depth * len);
//         return LV_RES_OK;
//     }

//     return LV_RES_INV;
// }
