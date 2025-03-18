#include "lv_port_fs.h"
#include "SD.h"
#include <stdio.h>
#include <dirent.h>
#include <limits.h>

static void *fs_open(lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode);
static lv_fs_res_t fs_close(lv_fs_drv_t *drv, void *file_p);
static lv_fs_res_t fs_read(lv_fs_drv_t *drv, void *file_p, void *buf, uint32_t btr, uint32_t *br);
static lv_fs_res_t fs_write(lv_fs_drv_t *drv, void *file_p, const void *buf, uint32_t btw, uint32_t *bw);
static lv_fs_res_t fs_seek(lv_fs_drv_t *drv, void *file_p, uint32_t pos, lv_fs_whence_t whence);
static lv_fs_res_t fs_tell(lv_fs_drv_t *drv, void *file_p, uint32_t *pos_p);

static void *fs_dir_open(lv_fs_drv_t *drv, const char *path);
static lv_fs_res_t fs_dir_read(lv_fs_drv_t *drv, void *rddir_p, char *fn);
static lv_fs_res_t fs_dir_close(lv_fs_drv_t *drv, void *rddir_p);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/

/**********************
 *      MACROS
 **********************/
#define MOUNT_POINT "/sdcard"
// 路径转换：将LVGL路径转换为VFS路径（如"V:/img.jpg" -> "/sdcard/img.jpg"）
static void path_convert(char *buf, const char *path)
{
    snprintf(buf, LV_FS_MAX_PATH_LENGTH, MOUNT_POINT "/%s", path + 2); // 跳过"V:/"
}
/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_port_fs_init(void)
{
    #if CONFIG_SD_MODE_SDIO
    SD_Init();
    #endif
    #if CONFIG_SD_MODE_SPI
    SD_Init_SPI();
    #endif
    static lv_fs_drv_t fs_drv;
    lv_fs_drv_init(&fs_drv);
    fs_drv.letter = 'V';
    fs_drv.open_cb = fs_open;
    fs_drv.close_cb = fs_close;
    fs_drv.read_cb = fs_read;
    fs_drv.write_cb = fs_write;
    fs_drv.seek_cb = fs_seek;
    fs_drv.tell_cb = fs_tell;

    fs_drv.dir_close_cb = fs_dir_close;
    fs_drv.dir_open_cb = fs_dir_open;
    fs_drv.dir_read_cb = fs_dir_read;

    lv_fs_drv_register(&fs_drv);
}

/**
 * 打开文件
 * @param drv      指向此函数所属的驱动程序的指针
 * @param path      以驱动程序字母开头的文件路径（例如 S：/folder/file.txt）
 * @param mode      读取：FS_MODE_RD，写入：FS_MODE_WR，两者：FS_MODE_RD |FS_MODE_WR
 * @return          文件描述符或 NULL 出错
 */
static void *fs_open(lv_fs_drv_t *drv, const char *path, lv_fs_mode_t mode)
{
    char vfs_path[LV_FS_MAX_PATH_LENGTH + 1];
    path_convert(vfs_path, path);

    const char *fmode = "rb";
    if (mode & LV_FS_MODE_WR)
    {
        if (mode & LV_FS_MODE_RD)
            fmode = "rb+";
        else
            fmode = "wb";
    }

    return fopen(vfs_path, fmode);
}

/**
 * 关闭打开的文件
 * @param drv       指向此函数所属的驱动程序的指针
 * @param file_p    指向 file_t 变量的指针。（以 fs_open 打开）
 * @return          LV_FS_RES_OK：没有错误或来自 @lv_fs_res_t 枚举的任何错误
 */
static lv_fs_res_t fs_close(lv_fs_drv_t *drv, void *file_p)
{
    fclose((FILE *)file_p);
    return LV_FS_RES_OK;
}

/**
 * 从打开的文件中读取数据
 * @param drv       指向此函数所属的驱动程序的指针
 * @param file_p    指向 file_t 变量的指针。
 * @param buf       指向存储读取数据的内存块的指针
 * @param btr       要读取的字节数
 * @param br        读取字节的实际数量 （Byte Read）
 * @return          LV_FS_RES_OK：没有错误或来自 @lv_fs_res_t 枚举的任何错误
 */
static lv_fs_res_t fs_read(lv_fs_drv_t *drv, void *file_p, void *buf, uint32_t btr, uint32_t *br)
{
    *br = fread(buf, 1, btr, (FILE *)file_p);
    return ferror((FILE *)file_p) ? LV_FS_RES_UNKNOWN : LV_FS_RES_OK;
}

/**
 * 写入文件
 * @param drv       指向此函数所属的驱动程序的指针
 * @param file_p    指向 file_t 变量的指针
 * @param buf       指向包含要写入的字节的缓冲区的指针
 * @param btw       要写入的字节数
 * @param bw        实际写入的字节数 （Bytes Written）。如果未使用，则为 NULL。
 * @return          LV_FS_RES_OK：没有错误或来自 @lv_fs_res_t 枚举的任何错误
 */
static lv_fs_res_t fs_write(lv_fs_drv_t *drv, void *file_p, const void *buf, uint32_t btw, uint32_t *bw)
{
    *bw = fwrite(buf, 1, btw, (FILE *)file_p);
    return ferror((FILE *)file_p) ? LV_FS_RES_UNKNOWN : LV_FS_RES_OK;
}

/**
 * 设置读写指针。如有必要，还可以扩展文件大小。
 * @param drv       指向此函数所属的驱动程序的指针
 * @param file_p    指向 file_t 变量的指针。（以 fs_open 开头）
 * @param pos       读写指针的新位置
 * @param whence    告诉从哪里解释 'POS'。查看 @lv_fs_whence_t
 * @return          LV_FS_RES_OK：没有错误或来自 @lv_fs_res_t 枚举的任何错误
 */
static lv_fs_res_t fs_seek(lv_fs_drv_t *drv, void *file_p, uint32_t pos, lv_fs_whence_t whence)
{
    /* 将LVGL的whence映射到标准C的seek模式 */
    int c_whence;
    switch (whence)
    {
    case LV_FS_SEEK_SET:
        c_whence = SEEK_SET;
        break;
    case LV_FS_SEEK_CUR:
        c_whence = SEEK_CUR;
        break;
    case LV_FS_SEEK_END:
        c_whence = SEEK_END;
        break;
    default:
        return LV_FS_RES_INV_PARAM;
    }
    /* 注意：fseek的offset类型为long，需要处理32位平台的潜在截断 */
    if (pos > LONG_MAX)
        return LV_FS_RES_INV_PARAM; // 偏移量超过系统支持范围
    return fseek((FILE *)file_p, (long)pos, c_whence) ?  LV_FS_RES_UNKNOWN: LV_FS_RES_OK;
}

/**
 * 给出读写指针的位置
 * @param drv       指向此函数所属的驱动程序的指针
 * @param file_p    指向 file_t 变量的指针。
 * @param pos_p     用于存储结果的指针
 * @return          LV_FS_RES_OK：没有错误或来自 @lv_fs_res_t 枚举的任何错误
 */
static lv_fs_res_t fs_tell(lv_fs_drv_t *drv, void *file_p, uint32_t *pos_p)
{
    *pos_p = ftell((FILE *)file_p);
    return (*pos_p != (uint32_t)-1) ? LV_FS_RES_OK : LV_FS_RES_UNKNOWN;
}

/// 文件夹操作
/**
 *初始化 'lv_fs_dir_t' 变量以进行目录读取
 * @param drv       指向此函数所属的驱动程序的指针
 * @param path      目录的路径
 * @return          指向目录读取描述符的指针或出错时为 NULL
 */
static void *fs_dir_open(lv_fs_drv_t *drv, const char *path)
{
    char vfs_path[LV_FS_MAX_PATH_LENGTH + 1];
    path_convert(vfs_path, path);
    return opendir(vfs_path);
}

/**
 * 从目录中读取下一个文件名。
 * 目录的名称将以 '/' 开头
 * @param drv       指向此函数所属的驱动程序的指针
 * @param rddir_p   指向已初始化的 'lv_fs_dir_t' 变量的指针
 * @param fn        指向缓冲区的指针以存储文件名
 * @return          LV_FS_RES_OK：没有错误或来自 @lv_fs_res_t 枚举的任何错误
 */
static lv_fs_res_t fs_dir_read(lv_fs_drv_t *drv, void *rddir_p, char *fn)
{
    struct dirent *entry;
    while ((entry = readdir((DIR *)rddir_p)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        strcpy(fn, entry->d_name);
        if (entry->d_type == DT_DIR)
            strcat(fn, "/"); // 目录添加斜杠
        return LV_FS_RES_OK;
    }
    fn[0] = '\0';        // 没有更多文件
    return LV_FS_RES_OK; // 没有更多文件时也返回OK，LVGL通过fn[0] == '\0'判断结束
}

/**
 * 关闭读取的目录
 * @param drv       指向此函数所属的驱动程序的指针
 * @param rddir_p   指向已初始化的 'lv_fs_dir_t' 变量的指针
 * @return          LV_FS_RES_OK：没有错误或来自 @lv_fs_res_t 枚举的任何错误
 */
static lv_fs_res_t fs_dir_close(lv_fs_drv_t *drv, void *rddir_p)
{
    closedir((DIR *)rddir_p);
    return LV_FS_RES_OK;
}