/**
 * @file lyn_plugin_dev.h
 * @brief plugin_dev模块api定义，该模块所有函数只能在Device侧调用，辅助用户编写plugin程序
 * \cond
 * @author Lynxi SDK Team
 * @date 2022-08-19
 *
 * Copyright:
 * © 2018 北京灵汐科技有限公司 版权所有。
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved.
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 *
 * @par 修改日志:
 * Data:2022-08-19
 * Author: siwen.li
 * Description:
 * \endcond
 */

#ifndef LYN_PLUGIN_DEV_H
#define LYN_PLUGIN_DEV_H

#include "lyn_codec_com.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief OSD叠加信息的颜色枚举
 * 
 */
typedef enum {
    DRAW_COLOR_WHITE,  /**< 白色*/
    DRAW_COLOR_BLACK,  /**< 黑色*/
    DRAW_COLOR_RED,    /**< 红色*/
    DRAW_COLOR_GREEN,  /**< 绿色*/
    DRAW_COLOR_BLUE,   /**< 蓝色*/
    DRAW_COLOR_YELLOW, /**< 黄色*/
    DARW_COLOR_CYAN    /**< 青色*/
} lynDrawColor;

/**
 * @brief OSD叠加信息的线宽枚举
 * 
 */
typedef enum {
    DRAW_THICK_2 = 2, /**< 线宽2像素*/
    DRAW_THICK_4 = 4, /**< 线宽4像素*/
    DRAW_THICK_6 = 6, /**< 线宽6像素*/
    DRAW_THICK_8 = 8  /**< 线宽8像素*/
} lynDrawThick;

/**
 * @brief OSD叠加信息的字体大小枚举
 * 
 */
typedef enum {
    FONT_SIZE_18 = 18,  /**< 字号18*/
    FONT_SIZE_24 = 24,  /**< 字号24*/
    FONT_SIZE_36 = 36,  /**< 字号36*/
    FONT_SIZE_48 = 48,  /**< 字号48*/
    FONT_SIZE_64 = 64,  /**< 字号64*/
    FONT_SIZE_96 = 96,  /**< 字号96*/
    FONT_SIZE_128 = 128 /**< 字号128*/
} lynFontSize;

/**
 * @brief OSD叠加信息的画框参数结构体
 * 
 */
typedef struct {
    uint32_t startX;         /**< 画框起点左上角横坐标*/
    uint32_t startY;         /**< 画框起点左上角纵坐标*/
    uint32_t boxW;           /**< 画框宽度*/
    uint32_t boxH;           /**< 画框高度*/
    uint32_t imgW;           /**< 图像宽度*/
    uint32_t imgH;           /**< 图像高度*/
    lynDrawThick thickness;  /**< 线宽*/
    lynDrawColor color;      /**< 颜色*/
    lynPixelFormat_t imgFmt; /**< 像素格式*/
    unsigned char *imgData;  /**< 图像数据*/
#if __WORDSIZE == 32
    uint32_t reserve; /**< 保留字段，不能使用*/
#endif
} __attribute__((packed)) lynDrawBoxAttr;

/**
 * @brief OSD叠加信息的用户自定义颜色框参数结构体
 * 
 */
typedef struct {
    uint32_t startX;         /**< 画框起点左上角横坐标*/
    uint32_t startY;         /**< 画框起点左上角纵坐标*/
    uint32_t boxW;           /**< 画框宽度*/
    uint32_t boxH;           /**< 画框高度*/
    uint32_t imgW;           /**< 图像宽度*/
    uint32_t imgH;           /**< 图像高度*/
    lynDrawThick thickness;  /**< 线宽*/
    uint8_t colorY;          /**< 自定义框的颜色Y分量*/
    uint8_t colorU;          /**< 自定义框的颜色U分量*/
    uint8_t colorV;          /**< 自定义框的颜色V分量*/
    lynPixelFormat_t imgFmt; /**< 像素格式*/
    unsigned char *imgData;  /**< 图像数据*/
#if __WORDSIZE == 32
    uint32_t reserve; /**< 保留字段，不能使用*/
#endif
} __attribute__((packed)) lynCustomColorBoxAttr;

/**
 * @brief OSD叠加信息的文字参数结构体
 * @note  只支持英文字符，中文字符请使用lynDrawTextAttrV2
 */
typedef struct {
    uint32_t startX;         /**< 文字起点左上角横坐标*/
    uint32_t startY;         /**< 文字起点左上角纵坐标*/
    uint32_t imgW;           /**< 图像宽度*/
    uint32_t imgH;           /**< 图像高度*/
    lynPixelFormat_t imgFmt; /**< 像素格式*/
    lynFontSize fontSize;    /**< 字号*/
    unsigned char *imgData;  /**< 图像数据*/
#if __WORDSIZE == 32
    uint32_t reserve; /**< 保留字段，不能使用*/
#endif
    char *text; /**< 字符数据*/
#if __WORDSIZE == 32
    uint32_t reserve_1; /**< 保留字段，不能使用*/
#endif
} __attribute__((packed)) lynDrawTextAttr;

/**
 * @brief OSD叠加信息的文字参数结构体
 * @note  支持中英文
 */
typedef struct {
    uint32_t startX;         /**< 写字起点左上角横坐标*/
    uint32_t startY;         /**< 写字起点左上角纵坐标*/
    uint32_t imgW;           /**< 图像宽度*/
    uint32_t imgH;           /**< 图像高度*/
    lynPixelFormat_t imgFmt; /**< 像素格式*/
    lynFontSize fontSize;    /**< 字号*/
    lynDrawColor color;      /**< 文字颜色*/
    unsigned char *imgData;  /**< 图像数据*/
#if __WORDSIZE == 32
    uint32_t reserve; /**< 保留字段，不能使用*/
#endif
    wchar_t *text; /**< 字符数据*/
#if __WORDSIZE == 32
    uint32_t reserve_1; /**< 保留字段，不能使用*/
#endif
} __attribute__ ((packed))lynDrawTextAttrV2;

/**
 * @brief 像素点
 * 
 */
typedef struct {
    uint32_t x; /**< 点横坐标*/
    uint32_t y; /**< 点纵坐标*/
} lynDarwPoint;

/**
 * @brief 画线参数结构体
 * 
 */
typedef struct {
    lynDarwPoint startPoint; /**< 画线起点坐标*/
    lynDarwPoint endPoint;   /**< 画线终点坐标*/
    uint32_t imgW;           /**< 图像宽度*/
    uint32_t imgH;           /**< 图像高度*/
    lynPixelFormat_t imgFmt; /**< 像素格式*/
    lynDrawThick thickness;  /**< 线宽*/
    lynDrawColor color;      /**< 颜色*/
    unsigned char *imgData;  /**< 图像数据*/
#if __WORDSIZE == 32
    uint32_t reserve; /**< 保留字段，不能使用*/
#endif
} __attribute__ ((packed))lynDrawLineAttr;

/**
 * @brief plugin log等级
 * 
 */
typedef enum {
    LOG_DEBUG, /**< 调试*/
    LOG_INFO, /**< 提示*/
    LOG_WARNING, /**< 警告*/
    LOG_ERROR, /**< 错误*/
    LOG_FATAL /**< 致命*/
} lynLogLevel;

#ifdef __aarch64__
/**
 * @brief 获取lynMalloc出来的devptr的虚拟地址
 *
 * @param[in]  devptr lynMalloc内存地址
 * @return void*  devptr的虚拟地址
 */
void *lynPluginGetVirtAddr(void *devptr);

/**
 * @brief 获取lynMalloc出来的devptr的虚拟地址，并使能Cache。
 * EP形态时KA200上ARMplugin通过使能Cache后读取内存数据时可以加速，访问内存更快。
 * RC形态时功能同lynPluginGetVirtAddr。
 * 用户在EP形态上使用该接口后在本次操作完数据后需要使用lynPluginReleaseCache接口释放Cache映射。
 *
 * @param[in]  devptr lynMalloc内存地址
 * @return void*  devptr的虚拟地址
 */
void *lynPluginGetVirtAddrCache(void *devptr);

/**
 * @brief 释放lynPluginGetVirtAddrCache接口使能的Cache
 * @param[in]  devptr lynMalloc内存地址
 * @return void
 */
void lynPluginReleaseCache(void *devptr);

/**
 * @brief 在图像中画一个矩形框
 *
 * @param[in]  attr 画框参数
 * @return int  返回0代表成功，非0代表失败
 */
int lynPluginDrawBox(lynDrawBoxAttr *attr);

/**
 * @brief 在图像中画一个自定义颜色的矩形框。
 * 
 * @param[in] attr 画框参数
 * @return int 返回0代表成功，非0代表失败
 */
int lynPluginDrawCustomColorBox(lynCustomColorBoxAttr *attr);

/**
 * @brief 在图像中写一段文字
 *
 * @param[in]  attr 写字参数
 * @return int 返回0代表成功，非0代表失败
 */
int lynPluginDrawText(lynDrawTextAttr *attr);

/**
 * @brief 在图像中写一段文字，支持宽字符
 *
 * @param[in]  attr 写字参数
 * @return int 返回0代表成功，非0代表失败
 */
int lynPluginDrawTextV2(lynDrawTextAttrV2 *attr);

/**
 * @brief 在图像中画一条直线
 *
 * @param[in]  attr 画线参数
 * @return int 返回0代表成功，非0代表失败
 */
int lynPluginDrawLine(lynDrawLineAttr *attr);

/**
 * @brief 插件日志打印，此打印会通过PCIe回传到Host显示
 *
 * @param[in]  level 日志等级
 * @param[in]  file  文件名
 * @param[in]  func  函数名
 * @param[in]  line  行数
 * @param[in]  fmt   打印日志内容
 * @param  ...
 * @return void
 */
void lynPluginPrint(lynLogLevel level, const char * file, const char *func, int line, const char *fmt, ...);

/**
 * @brief 插件日志初始化等级，默认是warning
 *
 * @param[in]  level 日志等级
 * @return void
 */
void lynPluginLogInit(lynLogLevel level);

/**
 * @brief 宏定义，调用lynPluginLogPrint函数打印插件日志，此打印会通过PCIe回传到Host显示
 * 
 */
#define LOG_PLUGIN(level, fmt, ...) lynPluginPrint(level, __FILE__, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

/**
 * @brief 宏定义，调用lynPluginLogPrint函数打印FATAL级插件日志，此打印会通过PCIe回传到Host显示。
 * 
 */
#define LOG_PLUGIN_F(fmt, ...) LOG_PLUGIN(LOG_FATAL, fmt, ##__VA_ARGS__)

/**
 * @brief 宏定义，调用lynPluginLogPrint函数打印ERROR级插件日志，此打印会通过PCIe回传到Host显示。
 * 
 */
#define LOG_PLUGIN_E(fmt, ...) LOG_PLUGIN(LOG_ERROR, fmt, ##__VA_ARGS__)

/**
 * @brief 宏定义，调用lynPluginLogPrint函数打印WARNING级插件日志，此打印会通过PCIe回传到Host显示。
 * 
 */
#define LOG_PLUGIN_W(fmt, ...) LOG_PLUGIN(LOG_WARNING, fmt, ##__VA_ARGS__)

/**
 * @brief 宏定义，调用lynPluginLogPrint函数打印INFO级插件日志，此打印会通过PCIe回传到Host显示。
 * 
 */
#define LOG_PLUGIN_I(fmt, ...) LOG_PLUGIN(LOG_INFO, fmt, ##__VA_ARGS__)

/**
 * @brief 宏定义，调用lynPluginLogPrint函数打印DEBUG级插件日志，此打印会通过PCIe回传到Host显示。
 * 
 */
#define LOG_PLUGIN_D(fmt, ...) LOG_PLUGIN(LOG_DEBUG, fmt, ##__VA_ARGS__)

#endif

/**
 * @brief 通知Client侧调用plugin回调函数，参数会通过PCIe回传到Host侧。
 *
 * @param[in]  vaddr 参数指针
 * @param[in]  len   参数大小
 * @return int 返回0代表成功，非0代表失败
 */
int lynPluginNotifyHost(const void* vaddr , uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
