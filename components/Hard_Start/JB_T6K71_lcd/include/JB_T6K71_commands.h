/*
 * JB_T6K71 LCD驱动命令文件。
 */
#pragma once

// LCD命令定义
#define LCD_T_OSC 0X0000 // 退出睡眠模式（开启CR震荡）\震荡设置
#define S_SLPOUT 0X0001  // 退出睡眠模式
// 显示屏设置
#define LCD_T_OUTPUT 0X0001 // 设置源驱动器输出移动的方向和 LCD 上显示的行数
#define S_L 0X0027          // 默认设置（0-128/320行）

#define LCD_T_AC 0X0002 // 设置帧/行液晶翻转
#define S_FRAME 0X0000  // 帧翻转极性
#define S_LINE 0X0200   // 行翻转极性

#define LCD_T_ENTRY 0X0003 // 设置入口模式（颜色格式/高速写入/刷新方向）
#define S_LWAM_L 0X0030    // 低速RAM+竖屏
#define S_HWAM_L 0X0130    // 高速RAM+竖屏
#define S_LWAM_R 0X0000    // 低速RAM+竖屏旋转
#define S_HWAM_R 0X0100    // 高速RAM+竖屏旋转

// NO 4，5
#define LCD_T_WIDE 0X0006 // 显示宽度
#define S_WIDE 0XF010     // 宽度240+后空白周期16CLK

#define LCD_T_DISP_FLIP 0X0007 // 显示设置1（颜色数量/滚动/翻转）
#define S_N 0X0004              // 不翻转
#define S_F 0X0000              // 翻转

#define LCD_T_DISP_SPEC 0X0008 // 显示设置2（前/后空白周期）
#define S_SPEC 0X0202          // 前/后空白周期最小配置2CLK
#define LCD_T_DISP_REFU 0X0009 // 显示设置3（全屏刷新）
#define S_REFU 0X008F          // 每16CLK全屏刷新
#define S_REFU_N 0X0000          // 不启用
// NO A
#define LCD_T_DISP_XY 0X000B       // 显示设置4（XY加倍功能）
#define S_XY_N 0X0000              // 不开启XY加倍功能
#define LCD_T_OUT_DISP_MODE 0X000C // 外部显示信号设置1（信号模式）
#define S_I80_MODE 0X0000          // 8080模式（MPU）其他模式暂不设置
#define LCD_T_FR 0X000D            // 设置FR周期调整（可能是帧率？）
#define S_FR 0X0010                // 最小有效值
#define LCD_T_OUT_DISP_FR 0X000E   // 外部显示信号设置2（1H周期）
#define S_1H 0X0110                // 最小有效值
// F功能未启用,10与1E相同,no 11
#define LCD_T_LTPS_ASW 0X0012  // LTPS设置(AWS时序H与上升沿)
#define S_ASW 0X0100           // 最小有效值
#define LCD_T_LTPS_OE 0X0013   // LTPS设置(OE时序上升与下降)
#define S_OE 0X0103            // 最小有效值
#define LCD_T_LTPS_ASWL 0X0014 // LTPS设置(ASW L)
#define S_ASWL 0X0001          // 最小有效值
#define LCD_T_LTPS_CKV 0X0015  // LTPS设置(CKV)
#define S_CKV 0X0200           // 最小有效值
// no 16,17 18-1B功能未启用

// 其他设置
#define LCD_T_AMP 0X001C // 放大器设置
#define S_AMP_MAX 0X0000 // 最大电流
#define S_AMP_OFF 0X0001 // 关闭/偏置
#define S_AMP_MID 0X0002 // 中电流
#define S_AMP_MIN 0X0003 // 最小电流

#define LCD_T_STANDBY 0X001D // 待机设置
#define S_ST 0X0004          // 进入st(正常/睡眠->待机)
#define S_DEEP 0X0000        // 深度待机(待机->深度待机)
#define S_DEEP_RESET 0X0005  // 清除待机(待机->睡眠)
/*
上电->深度待机->待机->睡眠->正常
正常->睡眠->待机->深度待机->断电
待机状态都能断电,正常可直接进入待机(跳过睡眠),此处的命令只能在待机与睡眠转换时使用
*/
#define LCD_T_OFFLINE 0X001E  // 断电序列设置
#define LCD_T_OFFLINER 0X0010 // 断电序列设置(复位后位置)
#define S_OFF 0X0001          // 最小有效值
// no 1F-FF

// 显示控制（！！！初始化最后设置！！！）睡眠与正常转换
#define LCD_T_CON_DISP 0x0100 // 显示控制(应在电源设置启用后再设置)
#define S_CON_ON 0XFFFE       // 开启显示(在AUTO之后设置)
#define S_CON_C 0XC010
#define S_CON_OFF 0X0000      // 关闭显示

#define LCD_T_AUTO 0x0101 // 自动序列控制
#define S_AUTO 0X0001     // 开启

// 电源设置
#define LCD_T_POWV 0X0102 // 设置电源1(VCOMD,VCS,VGM)
#define S_POWV 0X00F6     // 出厂值
#define LCD_T_POWX 0X0103 // 设置电源2(XVDD,AVDD与XVDD升压器时钟)
#define S_POWX 0X0007     // 出厂值
#define LCD_T_POWB 0X0104 // 设置电源3(升压级数)
#define S_POWB 0X0000     // 缺省值(?)
#define LCD_T_POWE 0X0105 // 设置电源4
#define S_POWE 0X0111     // 出厂值
// no 106-107,108未启用,no 109-1FF

// 数据指令
#define LCD_T_XADDR 0X0200  // 设置X起始地址
#define S_XADDR_init 0X0000 // 初始化值
#define LCD_T_YADDR 0X0201  // 设置Y起始地址
#define S_YADDR_init 0X0000 // 初始化值
#define LCD_T_WR 0X0202     // 写数据
// 无初始化值
// 203,204未启用,no 205-2FF

// 灰度设置
#define LCD_T_GAMM1 0X0300    // 设置GAMMA1
#define S_GAMM1_init 0X0200   // 出厂值
#define LCD_T_GAMM2 0X0301    // 设置GAMMA2
#define S_GAMM2_init 0X0002   // 出厂值
#define LCD_T_GAMM3 0X0302    // 设置GAMMA3
#define S_GAMM3_init 0X0000   // 出厂值
#define LCD_T_GAMM4 0X0303    // 设置GAMMA4
#define S_GAMM4_init 0X0300   // 出厂值
#define LCD_T_GAMM5 0X0304    // 设置GAMMA5
#define S_GAMM5_init 0X0700   // 出厂值
#define LCD_T_GAMMOFFS 0X0305 // 设置灰度偏移
#define S_GAMMOFFS_init 0X0070   // 出厂值
// no 306-3FF, 400,401未启用

// 窗口设置
#define LCD_T_SCREE1 0X0402  // 设置屏幕1起点
#define S_SCREE1_init 0X0000 // 出厂值
#define LCD_T_SCRE1 0X0403   // 设置屏幕1终点
#define S_SCRE1_init 0X013F  // 出厂值(320行)
// no 404,405未启用
#define LCD_T_X_START 0X0406  // 设置行地址起点（开窗用）
#define S_X_START_init 0X0000 // 初始值
#define LCD_T_X_END 0X0407    // 设置行地址终点
#define S_X_END_init 0X00EF   // 初始值(240)
#define LCD_T_Y_START 0X0408  // 设置列地址起点
#define S_Y_START_init 0X0000 // 初始值
#define LCD_T_Y_END 0X0409    // 设置列地址终点
#define S_Y_END_init 0X013F   // 初始值(320)
// no 40A-4FF
#define LCD_T_OSD 0X0500 // OSD设置
#define S_OSD_OFF 0X0000 // 关闭OSD
// no 501-503, 504,505未启用 no 506-7FF
#define LCD_T_NOP 0X05FF // 空操作
#define S_NOP 0X0000     // 无操作