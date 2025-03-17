/**
 * @file lv_conf.h
 * Configuration file for v8.4.0
 * 此处定义比配置中优先级更高，定义后将覆盖配置中的值
 */
#if 1 // 将其设置为“1”以启用内容

#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>
#include <stdio.h>

/*====================
   颜色设置
 *====================*/

/*颜色深度：1（每像素 1 字节）、8 （RGB332）、16 （RGB565）、32 （ARGB8888）*/
#define LV_COLOR_DEPTH 16

/*交换 2 字节的 RGB565 颜色。如果显示器具有 8 位接口（例如 SPI），则很有用*/
#define LV_COLOR_16_SWAP 1

/*启用此功能以在透明背景上绘制。
 *如果要使用 opa 和 transform_* 样式属性，则它是必需的。（透明度与图像变换）
 *如果 UI 位于另一层（例如 OSD 菜单或视频播放器）上方，也可以使用。*/
#define LV_COLOR_SCREEN_TRANSP 1

/* 调整颜色混合功能。GPU 可能会以不同的方式计算颜色（混合）。
 * 0：向下舍入，64：从 x.75 向上舍入，128：从一半向上舍入，192：从 x.25 向上舍入，254：向上舍入*/
#define LV_COLOR_MIX_ROUND_OFS 0

/*如果具有此颜色的图像像素是色度键控的，则不会绘制它们）*/
#define LV_COLOR_CHROMA_KEY lv_color_hex(0x00ff00) /*纯绿色*/

/*=========================
   内存设置
 *=========================*/

/*1: 使用自定义的 malloc/free，0：使用内置的 `lv_mem_alloc()` 和 `lv_mem_free()`*/
#define LV_MEM_CUSTOM 0
#if LV_MEM_CUSTOM == 0
/*可用于 'lv_mem_alloc（）' 的内存大小，以字节为单位 （>= 2kB）*/
#define LV_MEM_SIZE (48U * 1024U) /*[bytes]*/

/*为内存池设置一个地址，而不是将其分配为普通数组。也可以在外部 SRAM 中。*/
#define LV_MEM_ADR 0 /*0: 闲置*/
/*给出一个内存分配器，而不是地址，该分配器将被调用以获取 LVGL 的内存池。例如 my_malloc*/
#if LV_MEM_ADR == 0
#undef LV_MEM_POOL_INCLUDE
#undef LV_MEM_POOL_ALLOC
#endif

#else                                    /*LV_MEM_CUSTOM*/
#define LV_MEM_CUSTOM_INCLUDE <stdlib.h> /*动态内存函数的头文件*/
#define LV_MEM_CUSTOM_ALLOC malloc
#define LV_MEM_CUSTOM_FREE free
#define LV_MEM_CUSTOM_REALLOC realloc
#endif /*LV_MEM_CUSTOM*/

/*渲染和其他内部处理机制期间使用的中间内存缓冲区的编号。
 *如果没有足够的缓冲区，您将看到一条错误日志消息。*/
#define LV_MEM_BUF_MAX_NUM 16

/*使用标准的 'memcpy' 和 'memset' 而不是 LVGL 自己的函数。（可能会更快，也可能不会更快）。*/
#define LV_MEMCPY_MEMSET_STD 0

/*====================
   HAL 设置
 *====================*/

/*默认显示刷新间隔。LVG 将在这段时间内重新绘制更改的区域*/
#define LV_DISP_DEF_REFR_PERIOD 20 /*[ms]*/

/*输入设备读取周期（以毫秒为单位）*/
#define LV_INDEV_DEF_READ_PERIOD 20 /*[ms]*/

/*使用自定义时钟周期源，以毫秒为单位告知经过的时间。
 *这样就无需使用 `lv_tick_inc()` 手动更新tick了。）*/
#define LV_TICK_CUSTOM 1
#if LV_TICK_CUSTOM
// #define LV_TICK_CUSTOM_INCLUDE "Arduino.h"         /*系统时间函数的头文件*/
// #define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())    /*表达式，用于获取系统时间（毫秒）*/
/*如果使用 lvgl 作为 ESP32 组件*/
#define LV_TICK_CUSTOM_INCLUDE "esp_timer.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR ((esp_timer_get_time() / 1000LL))
#endif /*LV_TICK_CUSTOM*/

/*默认DPI。用于初始化默认大小，例如小部件大小、样式填充。
 *（不是很重要，您可以调整它以修改默认大小和间距）*/
#define LV_DPI_DEF 200 /*[px/inch]*/

/*=======================
 * 功能配置
 *=======================*/

/*-------------
 * 绘图
 *-----------*/

/*启用复杂绘制引擎。
 *绘制阴影、渐变、圆角、圆、弧、斜线、图像变换或任何蒙版*/
#define LV_DRAW_COMPLEX 1
#if LV_DRAW_COMPLEX != 0

/*允许缓冲一些阴影计算。
 *LV_SHADOW_CACHE_SIZE 是要缓冲的最大阴影大小，其中阴影大小为 'shadow_width + 半径'
 *缓存具有 LV_SHADOW_CACHE_SIZE^2 RAM 成本*/
#define LV_SHADOW_CACHE_SIZE 1

/*设置最大缓存的圆数据的数量。
 * 保存 1/4 圆的周长用于抗锯齿
 * 半径 * 每个圆使用 4 个字节（保存最常用的半径）
 * 0：禁用缓存 */
#define LV_CIRCLE_CACHE_SIZE 4
#endif /*LV_DRAW_COMPLEX*/

/**
 * 当 Widget 具有 'style_opa < 255' 时，使用“Simple layers” 将 Widget 缓冲到一个图层中
 * 并将其混合为具有给定不透明度的图像。
 * 请注意，'bg_opa'、'text_opa' 等不需要缓冲到 layer）
 * 小部件可以缓冲在较小的块中，以避免使用大缓冲区。
 *
 * - LV_LAYER_SIMPLE_BUF_SIZE：[bytes] 最佳目标缓冲区大小。LVGL 将尝试分配它
 * - LV_LAYER_SIMPLE_FALLBACK_BUF_SIZE：[bytes]如果无法分配 则使用'LV_LAYER_SIMPLE_BUF_SIZE'
 *
 * 两个缓冲区大小均以字节为单位。
 * “变换的图层”（使用变换角度/缩放属性）使用更大的缓冲区
 * 并且不能分块绘制。因此，这些设置仅影响具有不透明度的 widget。
 */
#define LV_LAYER_SIMPLE_BUF_SIZE (24 * 1024)
#define LV_LAYER_SIMPLE_FALLBACK_BUF_SIZE (3 * 1024)

/*默认图像缓存大小。图像缓存使图像保持打开状态。
 *如果只使用内置的图像格式，则缓存没有真正的优势。（即，如果未添加新的图像解码器）
 *使用复杂的图像解码器（e.g. PNG 或 JPG）缓存可以保存图像的连续打开/解码。
 *但是，打开的映像可能会消耗额外的 RAM。
 *0：禁用缓存*/
#define LV_IMG_CACHE_DEF_SIZE 6

/*每个颜色梯度允许的停止数。增加此项以允许更多停止。
 *每个额外的停止点将增加 （sizeof（lv_color_t） + 1） 字节*/
#define LV_GRADIENT_MAX_STOPS 4 // 渐变将包含最多两种颜色（例如，从红色渐变到蓝色）

/*默认渐变缓冲区大小。
 *当 LVGL 计算梯度 “映射” 时，它可以将它们保存到缓存中，以避免再次计算它们。
 *LV_GRAD_CACHE_DEF_SIZE 设置此缓存的大小（以字节为单位）。
 *如果缓存太小，则仅在绘制需要时分配地图。
 *0 表示无缓存*/
#define LV_GRAD_CACHE_DEF_SIZE 1024

/*允许对渐变进行抖动（以在有限的颜色深度显示上实现视觉平滑的颜色渐变）
 *LV_DITHER_GRADIENT意味着再分配一行或两行对象的渲染表面
 *内存消耗的增加是 （32 位 * 对象宽度） 加上 24 位 * 对象宽度（如果使用错误扩散）*/
#define LV_DITHER_GRADIENT 0
#if LV_DITHER_GRADIENT
/*添加对误差扩散抖动的支持。
 *错误扩散抖动可获得更好的视觉效果，但意味着绘制时需要消耗更多的 CPU 和内存。
 *内存消耗的增加是 （24 位 * 对象的宽度）*/
#define LV_DITHER_ERROR_DIFFUSION 0
#endif

/*为旋转分配的最大缓冲区大小。
 *仅当在显示驱动程序中启用软件旋转时使用*/
#define LV_DISP_ROT_MAX_BUF (10 * 1024)

/*-------------
 * GPU
 *-----------*/

/*使用 TSi（又名 Think Silicon）的加速库 NemaGFX */
#define LV_USE_NEMA_GFX 0

/*使用 Arm 的 2D 加速库 Arm-2D */
#define LV_USE_GPU_ARM2D 0

/*使用 STM32 的 DMA2D（又名 Chrom Art）GPU*/
#define LV_USE_GPU_STM32_DMA2D 0
#if LV_USE_GPU_STM32_DMA2D
/*必须定义为包含目标处理器的 CMSIS 标头的路径
例如 "stm32f7xx.h" or "stm32f4xx.h"*/
#define LV_GPU_DMA2D_CMSIS_INCLUDE
#endif

/*启用 RA6M3 G2D GPU*/
#define LV_USE_GPU_RA6M3_G2D 0
#if LV_USE_GPU_RA6M3_G2D
/*目标处理器的包含路径
例如 "hal_data.h"*/
#define LV_GPU_RA6M3_G2D_INCLUDE "hal_data.h"
#endif

/*使用 SWM341 的 DMA2D GPU*/
#define LV_USE_GPU_SWM341_DMA2D 0
#if LV_USE_GPU_SWM341_DMA2D
#define LV_GPU_SWM341_DMA2D_INCLUDE "SWM341.h"
#endif

/*使用 NXP 的 PXP GPU iMX RTxxx 平台*/
#define LV_USE_GPU_NXP_PXP 0
#if LV_USE_GPU_NXP_PXP
/*1: 为 PXP 添加默认裸机和 FreeRTOS 中断处理程序 （lv_gpu_nxp_pxp_osa.c）
 *  并在 lv_init（） 期间自动调用 lv_gpu_nxp_pxp_init（）。请注意，symbol SDK_OS_FREE_RTOS
 *   必须定义才能使用 FreeRTOS OSA，否则选择裸机实现。
 *0: 必须先手动调用 lv_gpu_nxp_pxp_init（） lv_init（）
 */
#define LV_USE_GPU_NXP_PXP_AUTO_INIT 0
#endif

/*使用 NXP 的 VG-Lite GPU iMX RTxxx 平台*/
#define LV_USE_GPU_NXP_VG_LITE 0

/*使用 SDL 渲染器 API*/
#define LV_USE_GPU_SDL 0
#if LV_USE_GPU_SDL
#define LV_GPU_SDL_INCLUDE_PATH <SDL2/SDL.h>
/*纹理缓存大小，默认为 8MB*/
#define LV_GPU_SDL_LRU_SIZE (1024 * 1024 * 8)
/*蒙版绘制的自定义混合模式，如果需要与较旧的 SDL2 库链接，请禁用*/
#define LV_GPU_SDL_CUSTOM_BLEND_MODE (SDL_VERSION_ATLEAST(2, 0, 6))
#endif

/*-------------
 * 日志
 *-----------*/

/*启用 log 模块*/
#define LV_USE_LOG 1
#if LV_USE_LOG

/*应添加多重要的日志：
 *LV_LOG_LEVEL_TRACE      大量日志提供详细信息
 *LV_LOG_LEVEL_INFO       记录重要事件
 *LV_LOG_LEVEL_WARN        记录是否发生了不需要但未导致问题的情况
 *LV_LOG_LEVEL_ERROR      仅当系统可能发生故障时，才出现严重问题
 *LV_LOG_LEVEL_USER       仅用户添加的日志
 *LV_LOG_LEVEL_NONE       不记录任何内容*/
#define LV_LOG_LEVEL LV_LOG_LEVEL_WARN

/*1: 使用 'printf' 打印日志;
 *0: 用户需要使用 'lv_log_register_print_cb（）' 注册回调*/
#define LV_LOG_PRINTF 0

/*在产生大量日志的模块中启用/禁用 LV_LOG_TRACE*/
#define LV_LOG_TRACE_MEM 1
#define LV_LOG_TRACE_TIMER 1
#define LV_LOG_TRACE_INDEV 1
#define LV_LOG_TRACE_DISP_REFR 1
#define LV_LOG_TRACE_EVENT 1
#define LV_LOG_TRACE_OBJ_CREATE 1
#define LV_LOG_TRACE_LAYOUT 1
#define LV_LOG_TRACE_ANIM 1

#endif /*LV_USE_LOG*/

/*-------------
 * 断言
 *-----------*/

/*Enable 在作失败或找到无效数据时断言。
 *如果启用了 LV_USE_LOG ，则失败时将打印错误消息*/
#define LV_USE_ASSERT_NULL 1          /*检查参数是否为 NULL。（非常快，推荐）*/
#define LV_USE_ASSERT_MALLOC 1        /*检查内存是否分配成功。（非常快，推荐）*/
#define LV_USE_ASSERT_STYLE 1         /*检查样式是否已正确初始化。（非常快，推荐）*/
#define LV_USE_ASSERT_MEM_INTEGRITY 0 /*在关键作后检查“lv_mem”的完整性。（慢速）*/
#define LV_USE_ASSERT_OBJ 1           /*检查对象的类型和存在性（例如，未删除）。（慢速）*/

/*在发生 assert 时添加自定义处理程序，例如重新启动 MCU*/
#define LV_ASSERT_HANDLER_INCLUDE <stdint.h>
#define LV_ASSERT_HANDLER \
  while (1)               \
    printf("LVGL ASSERTION FAILED"); /*默认暂停*/

/*-------------
 * 其他
 *-----------*/

/*1: 显示 CPU 使用率和 FPS 计数*/
#define LV_USE_PERF_MONITOR 1
#if LV_USE_PERF_MONITOR
#define LV_USE_PERF_MONITOR_POS LV_ALIGN_BOTTOM_RIGHT
#endif

/*1: 显示已使用的内存和内存碎片
 * 需求 LV_MEM_CUSTOM = 0*/
#define LV_USE_MEM_MONITOR 1
#if LV_USE_MEM_MONITOR
#define LV_USE_MEM_MONITOR_POS LV_ALIGN_BOTTOM_LEFT
#endif

/*1: 在重新绘制的区域上绘制随机彩色矩形*/
#define LV_USE_REFR_DEBUG 0

/*更改内置的 （v）snprintf 函数*/
#define LV_SPRINTF_CUSTOM 0
#if LV_SPRINTF_CUSTOM
#define LV_SPRINTF_INCLUDE <stdio.h>
#define lv_snprintf snprintf
#define lv_vsnprintf vsnprintf
#else /*LV_SPRINTF_CUSTOM*/
#define LV_SPRINTF_USE_FLOAT 0
#endif /*LV_SPRINTF_CUSTOM*/

#define LV_USE_USER_DATA 1

/*垃圾回收器设置
 *如果 lvgl 绑定到更高级别的语言，并且内存由该语言管理，则使用*/
#define LV_ENABLE_GC 0
#if LV_ENABLE_GC != 0
#define LV_GC_INCLUDE "gc.h" /*包括 Garbage Collector 相关内容*/
#endif                       /*LV_ENABLE_GC*/

/*=====================
 *  编译器设置
 *====================*/

/*为大端序系统设置为1*/
#define LV_BIG_ENDIAN_SYSTEM 0

/*定义 'lv_tick_inc' 函数的自定义属性*/
#define LV_ATTRIBUTE_TICK_INC

/*定义 'lv_timer_handler' 函数的自定义属性*/
#define LV_ATTRIBUTE_TIMER_HANDLER

/*定义 'lv_disp_flush_ready' 函数的自定义属性*/
#define LV_ATTRIBUTE_FLUSH_READY

/*缓冲区所需的对齐大小*/
#define LV_ATTRIBUTE_MEM_ALIGN_SIZE 16

/*将添加到需要对齐内存的位置（默认情况下，-Os 数据可能不与 boundary 对齐）。
 * E.g. __attribute__((aligned(4)))*/
#define LV_ATTRIBUTE_MEM_ALIGN

/*用于标记大型常量数组的属性，例如字体的位图*/
#define LV_ATTRIBUTE_LARGE_CONST

/*RAM 中大数组声明的编译器前缀*/
#define LV_ATTRIBUTE_LARGE_RAM_ARRAY

/*将性能关键功能放入更快的内存（例如 RAM）中*/
#define LV_ATTRIBUTE_FAST_MEM

/*GPU 加速作中使用的前缀变量，通常这些变量需要放置在 DMA 可访问的 RAM 部分中*/
#define LV_ATTRIBUTE_DMA

/*将整数常量导出到绑定。此宏与常量一起使用，其形式为 LV_<CONST>
 *还应出现在 LVGL 绑定 API 上，例如 Micropython.*/
#define LV_EXPORT_CONST_INT(int_value) struct _silence_gcc_warning /*默认值仅阻止 GCC 警告*/

/*扩展默认的 -32k..32k 坐标范围到 -4M..4M，使用 int32_t 而不是 int16_t 作为坐标*/
#define LV_USE_LARGE_COORD 0

/*==================
 *   字体使用
 *===================*/

/*具有 ASCII 范围和一些使用 bpp = 4 的符号的 Montserrat 字体
 *https://fonts.google.com/specimen/Montserrat*/
#define LV_FONT_MONTSERRAT_8 0
#define LV_FONT_MONTSERRAT_10 0
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0
#define LV_FONT_MONTSERRAT_26 0
#define LV_FONT_MONTSERRAT_28 0
#define LV_FONT_MONTSERRAT_30 0
#define LV_FONT_MONTSERRAT_32 1
#define LV_FONT_MONTSERRAT_34 0
#define LV_FONT_MONTSERRAT_36 0
#define LV_FONT_MONTSERRAT_38 0
#define LV_FONT_MONTSERRAT_40 0
#define LV_FONT_MONTSERRAT_42 0
#define LV_FONT_MONTSERRAT_44 0
#define LV_FONT_MONTSERRAT_46 0
#define LV_FONT_MONTSERRAT_48 0

/*演示特殊功能*/
#define LV_FONT_MONTSERRAT_12_SUBPX 0
#define LV_FONT_MONTSERRAT_28_COMPRESSED 0 /*bpp = 3*/
#define LV_FONT_DEJAVU_16_PERSIAN_HEBREW 0 /*希伯来语、阿拉伯语、波斯语字母及其所有形式*/
#define LV_FONT_SIMSUN_16_CJK 0            /*1000 个最常见的 CJK 自由基*/

/*像素完美的等宽字体*/
#define LV_FONT_UNSCII_8 0
#define LV_FONT_UNSCII_16 0

/*（可选）在此处声明自定义字体。
 *您也可以将这些字体用作默认字体，它们将在全球范围内可用。
 *例 #define LV_FONT_CUSTOM_DECLARE LV_FONT_DECLARE（my_font_1） LV_FONT_DECLARE（my_font_2）*/
#define LV_FONT_CUSTOM_DECLARE

/*始终设置默认字体*/
#define LV_FONT_DEFAULT &lv_font_montserrat_14

/*启用处理大字体和/或具有大量字符的字体。
 *限制取决于字体大小、字体和 bpp。
 *如果字体需要，将触发编译器错误.*/
#define LV_FONT_FMT_TXT_LARGE 0

/*启用/禁用对压缩字体的支持.*/
#define LV_USE_FONT_COMPRESSED 0

/*启用子像素渲染*/
#define LV_USE_FONT_SUBPX 1
#if LV_USE_FONT_SUBPX
/*设置显示器的像素顺序。RGB 通道的物理顺序。与 “normal” 字体无关紧要.*/
#define LV_FONT_SUBPX_BGR 0 /*0: RGB; 1:BGR order*/
#endif

/*在找不到字形 dsc 时启用绘图占位符*/
#define LV_USE_FONT_PLACEHOLDER 1

/*=================
 *  文本设置
 *=================*/

/**
 * 选择字符串的字符编码.
 * 您的 IDE 或编辑器应具有相同的字符编码
 * - LV_TXT_ENC_UTF8
 * - LV_TXT_ENC_ASCII
 */
#define LV_TXT_ENC LV_TXT_ENC_UTF8

/*可以在这些字符上断开（换行）文本*/
#define LV_TXT_BREAK_CHARS " ,.;:-_?!，。；：—？！"

/*如果一个单词至少有这么长，就会在“最漂亮”的地方换行
 *要禁用，请设置为值 <= 0*/
#define LV_TXT_LINE_BREAK_LONG_LEN 12

/*在换行之前放在一行中的长单词的最小字符数。
 *取决于 LV_TXT_LINE_BREAK_LONG_LEN.*/
#define LV_TXT_LINE_BREAK_LONG_PRE_MIN_LEN 3

/*在中断后放在一行上的长单词的最小字符数。
 *取决于 LV_TXT_LINE_BREAK_LONG_LEN。*/
#define LV_TXT_LINE_BREAK_LONG_POST_MIN_LEN 3

/*用于表示文本重新着色的控制字符.*/
#define LV_TXT_COLOR_CMD "#"

/*支持双向文本。允许混合 Left-to-Right 和 Right-to-Left 文本。
 *方向将根据 Unicode 双向算法进行处理：
 *https://www.w3.org/International/articles/inline-bidi-markup/uba-basics*/
#define LV_USE_BIDI 0
#if LV_USE_BIDI
/*设置默认方向。支持的值：
 *`LV_BASE_DIR_LTR` 从左到右
 *`LV_BASE_DIR_RTL` 从右到左
 *`LV_BASE_DIR_AUTO` 检测文本基本方向*/
#define LV_BIDI_BASE_DIR_DEF LV_BASE_DIR_AUTO
#endif

/*启用阿拉伯语/波斯语处理
 *在这些语言中，字符应根据它们在文本中的位置替换为其他形式*/
#define LV_USE_ARABIC_PERSIAN_CHARS 0

/*==================
 *  组件使用情况
 *================*/

/*组件的文档：https://docs.lvgl.io/latest/en/html/widgets/index.html*/

#define LV_USE_ARC 1

#define LV_USE_BAR 1

#define LV_USE_BTN 1

#define LV_USE_BTNMATRIX 1

#define LV_USE_CANVAS 1

#define LV_USE_CHECKBOX 1

#define LV_USE_DROPDOWN 1 /*要求： lv_label*/

#define LV_USE_IMG 1 /*要求： lv_label*/

#define LV_USE_LABEL 1
#if LV_USE_LABEL
#define LV_LABEL_TEXT_SELECTION 1 /*启用标签文本选择*/
#define LV_LABEL_LONG_TXT_HINT 1  /*在标签中存储一些额外的信息以加快绘制非常长的文本*/
#endif

#define LV_USE_LINE 1

#define LV_USE_ROLLER 1 /*要求： lv_label*/
#if LV_USE_ROLLER
#define LV_ROLLER_INF_PAGES 7 /*当滚轮为无限时的额外“页数”*/
#endif

#define LV_USE_SLIDER 1 /*要求： lv_bar*/

#define LV_USE_SWITCH 1

#define LV_USE_TEXTAREA 1 /*要求： lv_label*/
#if LV_USE_TEXTAREA != 0
#define LV_TEXTAREA_DEF_PWD_SHOW_TIME 1500 /*ms*/
#endif

#define LV_USE_TABLE 1

/*==================
 * 额外组件
 *==================*/

/*-----------
 * 部件
 *----------*/
#define LV_USE_ANIMIMG 1

#define LV_USE_CALENDAR 1
#if LV_USE_CALENDAR
#define LV_CALENDAR_WEEK_STARTS_MONDAY 0
#if LV_CALENDAR_WEEK_STARTS_MONDAY
#define LV_CALENDAR_DEFAULT_DAY_NAMES {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"}
#else
#define LV_CALENDAR_DEFAULT_DAY_NAMES {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"}
#endif

#define LV_CALENDAR_DEFAULT_MONTH_NAMES {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"}
#define LV_USE_CALENDAR_HEADER_ARROW 1
#define LV_USE_CALENDAR_HEADER_DROPDOWN 1
#endif /*LV_USE_CALENDAR*/

#define LV_USE_CHART 1

#define LV_USE_COLORWHEEL 1

#define LV_USE_IMGBTN 1

#define LV_USE_KEYBOARD 1

#define LV_USE_LED 1

#define LV_USE_LIST 1

#define LV_USE_MENU 1

#define LV_USE_METER 1

#define LV_USE_MSGBOX 1

#define LV_USE_SPAN 1
#if LV_USE_SPAN
/*行文本可以包含最大数量的 span 描述符 */
#define LV_SPAN_SNIPPET_STACK_SIZE 64
#endif

#define LV_USE_SPINBOX 1

#define LV_USE_SPINNER 1

#define LV_USE_TABVIEW 1

#define LV_USE_TILEVIEW 1

#define LV_USE_WIN 1

/*-----------
 * 主题
 *----------*/

/*一个简单、令人印象深刻且非常完整的主题*/
#define LV_USE_THEME_DEFAULT 1
#if LV_USE_THEME_DEFAULT

/*0：灯光模式;1：暗模式*/
#define LV_THEME_DEFAULT_DARK 0

/*1：启用按压时增长*/
#define LV_THEME_DEFAULT_GROW 1

/*默认过渡时间（以 [ms] 为单位）*/
#define LV_THEME_DEFAULT_TRANSITION_TIME 80
#endif /*LV_USE_THEME_DEFAULT*/

/*一个非常简单的主题，是自定义主题的一个很好的起点*/
#define LV_USE_THEME_BASIC 1

/*专为单色显示器设计的主题*/
#define LV_USE_THEME_MONO 1

/*-----------
 * 布局
 *----------*/

/*类似于 CSS 中的 Flexbox 的布局.*/
#define LV_USE_FLEX 1

/*类似于 CSS 中的 Grid 的布局.*/
#define LV_USE_GRID 1

/*---------------------
 * 第三方库
 *--------------------*/

/*常见 API 的文件系统接口 */

/*用于 fopen、fread 等的 API*/
#define LV_USE_FS_STDIO 0
#if LV_USE_FS_STDIO
#define LV_FS_STDIO_LETTER '\0'  /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_STDIO_PATH ""      /*Set the working directory. File/directory paths will be appended to it.*/
#define LV_FS_STDIO_CACHE_SIZE 0 /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*用于打开、读取等的 API*/
#define LV_USE_FS_POSIX 0
#if LV_USE_FS_POSIX
#define LV_FS_POSIX_LETTER '\0'  /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_POSIX_PATH ""      /*Set the working directory. File/directory paths will be appended to it.*/
#define LV_FS_POSIX_CACHE_SIZE 0 /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*CreateFile、ReadFile 等的 API*/
#define LV_USE_FS_WIN32 0
#if LV_USE_FS_WIN32
#define LV_FS_WIN32_LETTER '\0'  /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_WIN32_PATH ""      /*Set the working directory. File/directory paths will be appended to it.*/
#define LV_FS_WIN32_CACHE_SIZE 0 /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*FATFS 的 API（需要单独添加）。使用 f_open、f_read 等*/
#define LV_USE_FS_FATFS 0
#if LV_USE_FS_FATFS
#define LV_FS_FATFS_LETTER '\0'  /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_FATFS_CACHE_SIZE 0 /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*LittleFS 的 API（库需要单独添加）。使用 lfs_file_open、lfs_file_read 等*/
#define LV_USE_FS_LITTLEFS 0
#if LV_USE_FS_LITTLEFS
#define LV_FS_LITTLEFS_LETTER '\0'  /*Set an upper cased letter on which the drive will accessible (e.g. 'A')*/
#define LV_FS_LITTLEFS_CACHE_SIZE 0 /*>0 to cache this number of bytes in lv_fs_read()*/
#endif

/*PNG 解码器库*/
#define LV_USE_PNG 0

/*BMP 解码器库*/
#define LV_USE_BMP 0

/* JPG + 拆分 JPG 解码器库。
 * Split JPG 是一种针对嵌入式系统优化的自定义格式. */
#define LV_USE_SJPG 0

/*GIF 解码器库*/
#define LV_USE_GIF 0

/*二维码库*/
#define LV_USE_QRCODE 0

/*FreeType 库*/
#define LV_USE_FREETYPE 0
#if LV_USE_FREETYPE
/*FreeType 用于缓存字符的内存 [字节]（-1：无缓存）*/
#define LV_FREETYPE_CACHE_SIZE (16 * 1024)
#if LV_FREETYPE_CACHE_SIZE >= 0
/* 1：位图缓存使用 sbit 缓存，0：位图缓存使用图像缓存。 */
/* sbit cache：对于小位图（字体大小< 256），内存效率要高得多。 */
/* 如果字体大小 >= 256，则必须配置为图像缓存 */
#define LV_FREETYPE_SBIT_CACHE 0
/* 此缓存实例管理的最大已打开 FT_Face/FT_Size 对象数. */
/* （0：使用系统默认值） */
#define LV_FREETYPE_CACHE_FT_FACES 0
#define LV_FREETYPE_CACHE_FT_SIZES 0
#endif
#endif

/*微型 TTF 库*/
#define LV_USE_TINY_TTF 0
#if LV_USE_TINY_TTF
/*从文件加载 TTF 数据*/
#define LV_TINY_TTF_FILE_SUPPORT 0
#endif

/*Rlottie 库*/
#define LV_USE_RLOTTIE 0

/*用于图像解码和播放视频的 FFmpeg 库
 *支持所有主要图像格式，因此不要使用它启用其他图像解码器*/
#define LV_USE_FFMPEG 0
#if LV_USE_FFMPEG
/*将输入信息转储到 stderr*/
#define LV_FFMPEG_DUMP_FORMAT 0
#endif

/*-----------
 * 其他
 *----------*/

/*1：启用 API 为对象拍摄快照*/
#define LV_USE_SNAPSHOT 0

/*1：启用 Monkey 测试*/
#define LV_USE_MONKEY 0

/*1：启用网格导航*/
#define LV_USE_GRIDNAV 0

/*1：启用 lv_obj fragment*/
#define LV_USE_FRAGMENT 0

/*1：支持在标签或跨度小部件中使用图像作为字体 */
#define LV_USE_IMGFONT 0

/*1：启用基于已发布订阅者的消息传送系统 */
#define LV_USE_MSG 1

/*1：启用拼音输入法*/
/*要求： lv_keyboard*/
#define LV_USE_IME_PINYIN 1
#if LV_USE_IME_PINYIN
/*1: 使用默认同音词库*/
/*如果您不使用默认同音词库，请务必在设置同音词库后使用 'lv_ime_pinyin'*/
#define LV_IME_PINYIN_USE_DEFAULT_DICT 1
/*设置可以显示的候选面板的最大数量*/
/*这需要根据屏幕的大小进行调整*/
#define LV_IME_PINYIN_CAND_TEXT_NUM 6

/*使用 9 键输入 （k9）*/
#define LV_IME_PINYIN_USE_K9_MODE 1
#if LV_IME_PINYIN_USE_K9_MODE == 1
#define LV_IME_PINYIN_K9_CAND_TEXT_NUM 3
#endif // LV_IME_PINYIN_USE_K9_MODE
#endif

/*==================
 * 例子
 *==================*/

/*启用要使用库构建的示例*/
#define LV_BUILD_EXAMPLES 0

/*===================
 * DEMO 使用
 ====================*/

/*显示一些小部件。可能需要增加 'LV_MEM_SIZE' */
#define LV_USE_DEMO_WIDGETS 0
#if LV_USE_DEMO_WIDGETS
#define LV_DEMO_WIDGETS_SLIDESHOW 0
#endif

/*演示编码器和键盘的用法*/
#define LV_USE_DEMO_KEYPAD_AND_ENCODER 0

/*对您的系统进行基准测试*/
#define LV_USE_DEMO_BENCHMARK 1
#if LV_USE_DEMO_BENCHMARK
/*使用具有 16 位颜色深度的 RGB565A8 图像，而不是 ARGB8565*/
#define LV_DEMO_BENCHMARK_RGB565A8 0
#endif

/*LVGL 的压力测试*/
#define LV_USE_DEMO_STRESS 0

/*音乐播放器演示*/
#define LV_USE_DEMO_MUSIC 1
#if LV_USE_DEMO_MUSIC
#define LV_DEMO_MUSIC_SQUARE 0
#define LV_DEMO_MUSIC_LANDSCAPE 0
#define LV_DEMO_MUSIC_ROUND 0
#define LV_DEMO_MUSIC_LARGE 0
#define LV_DEMO_MUSIC_AUTO_PLAY 1
#endif

/*--END OF LV_CONF_H--*/

#endif /*LV_CONF_H*/

#endif /*“内容启用”结束*/
