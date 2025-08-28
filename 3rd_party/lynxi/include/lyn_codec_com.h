/**
 * @file lyn_codec_com.h
 * @brief 视频、图像编解码通用数据结构定义说明
 * \cond
 * @author Lynxi SDK Team
 * @version 1.0
 * @date 2021-08-10
 *
 * Copyright:
 * © 2018 北京灵汐科技有限公司 版权所有。
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved.
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 * \endcond
 */

#ifndef LYN_CODEC_COM_C_H
#define LYN_CODEC_COM_C_H

#include "lyn_err.h"
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief 编码协议类型枚举
 *
 */
typedef enum {
    LYN_CODEC_ID_NONE = 0,  /**< 空类型，枚举开始*/
    LYN_CODEC_ID_H263,      /**< H263编码*/
    LYN_CODEC_ID_H264,      /**< H264编码*/
    LYN_CODEC_ID_H265,      /**< H265编码*/
    LYN_CODEC_ID_JPEG,      /**< JPEG编码*/
    LYN_CODEC_ID_PNG,       /**< PNG编码*/
    LYN_CODEC_ID_BMP,       /**< BMP编码*/
    LYN_CODEC_ID_TIFF,      /**< TIFF编码*/
    LYN_CODEC_ID_MPEG4,     /**< MPEG4编码*/
    LYN_CODEC_ID_VP9,       /**< VP9编码*/
    LYN_CODEC_ID_APNG,      /**< APNG编码*/
    LYN_CODEC_ID_FLV,       /**< FLV编码*/
    LYN_CODEC_ID_END        /**< 枚举结束*/
} lynCodecId_t;

/**
 * @brief 像素格式类型枚举
 *
 */
typedef enum
{
    LYN_PIX_FMT_NONE           = 0,  /**< 空类型，枚举开始*/
    LYN_PIX_FMT_YUV420P           ,  /**< 平面模式,三个平面,分别存储Y/U/V分量,一组UV分量对应2x2的Y分量，每个像素12位*/
    LYN_PIX_FMT_NV12              ,  /**< 半平面模式(SP)的420P，两个平面,分别存储Y和UV分量,其中UV分量共用一个平面并且以U,V,U,V的顺序交错排列,一组UV分量对应2x2的Y分量,每个像素12位*/
    LYN_PIX_FMT_NV21              ,  /**< 半平面模式(SP)的420P，两个平面,分别存储Y和UV分量,其中UV分量共用一个平面并且以V,U,V,U的顺序交错排列,一组UV分量对应2x2的Y分量,每个像素12位*/
    LYN_PIX_FMT_RGB24             ,  /**< 交错模式，R/G/B分别占用一字节，以RGBRGB的顺序存储，每个像素24位*/
    LYN_PIX_FMT_BGR24             ,  /**< 交错模式，R/G/B分别占用一字节，以BGRBGR的顺序存储，每个像素24位*/
    LYN_PIX_FMT_NV12_10BIT        ,  /**< 和LYN_PIX_FMT_NV12类似，只是一个分量占用10bit,在补齐情况下，占用16bit，每个像素24位*/
    LYN_PIX_FMT_NV21_10BIT        ,  /**< 和LYN_PIX_FMT_NV21类似，只是一个分量占用10bit,在补齐情况下，占用16bit，每个像素24位*/

    LYN_PIX_FMT_YUVJ420P      = 10,  /**< 使用了JPEG颜色范围的LYN_PIX_FMT_YUV420P，LYN_PIX_FMT_YUVJ420P的像素颜色取值范围是[0,255]，0表示黑色，255表示白色，而LYN_PIX_FMT_YUV420P像素颜色范围为[16,235]，16表示黑色，235表示白色*/
    LYN_PIX_FMT_YUVJ422P          ,  /**< 使用了JPEG颜色范围的LYN_PIX_FMT_YUV422P，LYN_PIX_FMT_YUVJ422P的像素颜色取值范围是[0,255]，0表示黑色，255表示白色，而LYN_PIX_FMT_YUV422P像素颜色范围为[16,235]，16表示黑色，235表示白色*/
    LYN_PIX_FMT_YUVJ444P          ,  /**< 使用了JPEG颜色范围的LYN_PIX_FMT_YUV444P，LYN_PIX_FMT_YUVJ444P的像素颜色取值范围是[0,255]，0表示黑色，255表示白色，而LYN_PIX_FMT_YUV444P像素颜色范围为[16,235]，16表示黑色，235表示白色*/
    LYN_PIX_FMT_YUVJ440P          ,  /**< 使用了JPEG颜色范围的LYN_PIX_FMT_YUV440P，LYN_PIX_FMT_YUVJ440P的像素颜色取值范围是[0,255]，0表示黑色，255表示白色，而LYN_PIX_FMT_YUV440P像素颜色范围为[16,235]，16表示黑色，235表示白色*/
    LYN_PIX_FMT_YUVJ411P          ,  /**< 使用了JPEG颜色范围的LYN_PIX_FMT_YUV411P，LYN_PIX_FMT_YUVJ411P的像素颜色取值范围是[0,255]，0表示黑色，255表示白色，而LYN_PIX_FMT_YUV411P像素颜色范围为[16,235]，16表示黑色，235表示白色*/

    LYN_PIX_FMT_NV16          = 20,   /**< 半平面模式(SP)的422P，两个平面,分别存储Y和UV分量,其中UV分量共用一个平面并且以U,V,U,V的顺序交错排列,一组UV分量对应2x1的Y分量，每个像素占16位*/
    LYN_PIX_FMT_NV61              ,   /**< 半平面模式(SP)的422P，两个平面,分别存储Y和UV分量,其中UV分量共用一个平面并且以V,U,V,U的顺序交错排列,一组UV分量对应2x1的Y分量，每个像素占16位*/
    LYN_PIX_FMT_NV24              ,   /**< 半平面模式(SP)的444P，两个平面,分别存储Y和UV分量,其中UV分量共用一个平面并且以U,V,U,V的顺序交错排列,一组UV分量对应1x1的Y分量，每个像素占24位*/
    LYN_PIX_FMT_NV42              ,   /**< 半平面模式(SP)的444P，两个平面,分别存储Y和UV分量,其中UV分量共用一个平面并且以V,U,V,U的顺序交错排列,一组UV分量对应1x1的Y分量，每个像素占24位*/
    LYN_PIX_FMT_HSV24             ,  /**< 使用HSV模型描述一个像素，色调(Hue)、饱和度(Saturation)和亮度(Value)分别占用8位，每个像素占24位*/
    LYN_PIX_FMT_LAB24             ,  /**< 使用CIE Lab模型描述一个像素，L(亮度)、a(从绿色到红色的分量)和b(从蓝色到黄色的分量)分别占用8位，每个像素占24位*/

    LYN_PIX_FMT_YUV400        = 30,  /**< 只含有Y分量，每个像素占16位*/
    LYN_PIX_FMT_YUYV422           ,  /**< 交错模式，以Y0 Cb Y1 Cr的顺序存储，每个像素占16位*/
    LYN_PIX_FMT_YUV422P           ,  /**< 平面模式,三个平面,分别存储Y/U/V分量,一组UV分量对应2x1的Y分量，每个像素占16位*/
    LYN_PIX_FMT_YUV444P           ,  /**< 平面模式,三个平面,分别存储Y/U/V分量,一组UV分量对应1x1的Y分量，每个像素占24位*/
    LYN_PIX_FMT_YUV410P           ,  /**< 平面模式,三个平面,分别存储Y/U/V分量,一组UV分量对应4x4的Y分量，每个像素占9位*/
    LYN_PIX_FMT_YUV411P           ,  /**< 平面模式,三个平面,分别存储Y/U/V分量,一组UV分量对应4x1的Y分量，每个像素占12位*/
    LYN_PIX_FMT_YUV440P           ,  /**< 平面模式,三个平面,分别存储Y/U/V分量,一组UV分量对应1x2的Y分量*/
    LYN_PIX_FMT_YUVA420P          ,  /**< 平面模式,三个平面,分别存储Y/U/V分量,一组UV分量对应2x2的Y分量和一个A分量，每个像素占20位*/


    LYN_PIX_FMT_GRAY8         = 40,  /**< 8位Y分量，每个像素占8位*/
    LYN_PIX_FMT_GRAY16BE          ,  /**< 16位Y分量，按CPU大端模式存储，每个像素占16位*/
    LYN_PIX_FMT_GRAY16LE          ,  /**< 16位Y分量，按CPU小端模式存储，每个像素占16位*/
    LYN_PIX_FMT_MONOWHITE         ,  /**< 1位Y分量，0表示白色，1表示黑色,每个像素占1位*/
    LYN_PIX_FMT_MONOBLACK         ,  /**< 1位Y分量，1表示白色，0表示黑色,每个像素占1位*/
    LYN_PIX_FMT_PAL8              ,  /**< 使用一个调色板存储颜色信息，每个像素占8位，像素值是一个索引值，指向调色板中的一个颜色条目*/
    LYN_PIX_FMT_UYVY422           ,  /**< 交错模式，以Cb Y0 Cr Y1的顺序存储，每个像素占16位*/
    LYN_PIX_FMT_UYYVYY411         ,  /**< 交错模式，以 Cb Y0 Y1 Cr Y2 Y3的顺序存储，每个像素占12位*/

    LYN_PIX_FMT_BGR8          = 50,   /**< 交错模式，B占用2位，G占用3位，R占用3位，以(msb)2B 3G 3R(lsb)的顺序存储，每个像素共占8位*/
    LYN_PIX_FMT_BGR4              ,   /**< 交错模式，B占用1位，G占用2位，R占用1位，以(msb)1B 2G 1R(lsb)的顺序存储，每个像素共4位*/
    LYN_PIX_FMT_BGR4_BYTE         ,   /**< 交错模式，B占用1位，G占用2位，R占用1位，以(msb)1B 2G 1R(lsb)的顺序存储，剩余位0补齐，每个像素共8位*/
    LYN_PIX_FMT_RGB8              ,   /**< 交错模式，R占用2位，G占用3位，B占用3位，以(msb)2R 3G 3B(lsb)的顺序存储，每个像素共8位*/
    LYN_PIX_FMT_RGB4              ,   /**< 交错模式，R占用1位，G占用2位，B占用1位，以(msb)1R 2G 1B(lsb)的顺序存储，每个像素共4位*/
    LYN_PIX_FMT_RGB4_BYTE         ,   /**< 交错模式，R占用1位，G占用2位，B占用1位，以(msb)1R 2G 1B(lsb)的顺序存储，剩余位0补齐,每个像素共4位*/

    LYN_PIX_FMT_ARGB          = 60,  /**< 交错模式，A占用8位，R占用8位，G占用8位,B占用8位，以ARGBARGB...顺序存储，每个像素占32位*/
    LYN_PIX_FMT_RGBA              ,  /**< 交错模式，R占用8位，G占用8位，B占用8位,A占用8位，以RGBARGBA...顺序存储，每个像素占32位*/
    LYN_PIX_FMT_ABGR              ,  /**< 交错模式，A占用8位，B占用8位，G占用8位,R占用8位，以ABGRABGR...顺序存储，每个像素占32位*/
    LYN_PIX_FMT_BGRA              ,  /**< 交错模式，B占用8位，G占用8位，R占用8位,A占用8位，以BGRABGRA...顺序存储，每个像素占32位*/
    LYN_PIX_FMT_GBRAP             ,  /**< 平面模式，G/B/R/A各使用一个平面，各占8位，每个像素占32位*/

    LYN_PIX_FMT_RGB48BE       = 70,  /**< 交错模式，R占用16位，G占用16位,B占用16位，以RGBRGB顺序按照CPU大端序存储，每个像素占48位*/
    LYN_PIX_FMT_RGB48LE           ,  /**< 交错模式，R占用16位，G占用16位,B占用16位，以RGBRGB顺序按照CPU小端序存储，每个像素占48位*/
    LYN_PIX_FMT_RGB565BE          ,  /**< 交错模式，R占用5位，G占用6位,B占用5位，以(msb)5R 6G 5B(lsb)顺序按照CPU大端序存储，每个像素占16位*/
    LYN_PIX_FMT_RGB565LE          ,  /**< 交错模式，R占用5位，G占用6位,B占用5位，以(msb)5R 6G 5B(lsb)顺序按照CPU小端序存储，每个像素占16位*/
    LYN_PIX_FMT_RGB555BE          ,  /**< 交错模式，R占用5位，G占用5位,B占用5位，以((msb)1X 5R 5G 5B(lsb)顺序按照CPU大端序存储，X位未使用，每个像素占16位*/
    LYN_PIX_FMT_RGB555LE          ,  /**< 交错模式，R占用5位，G占用5位,B占用5位，以((msb)1X 5R 5G 5B(lsb)顺序按照CPU小端序存储，X位未使用，每个像素占16位*/
    LYN_PIX_FMT_BGR565BE          ,  /**< 交错模式，B占用5位，G占用6位,R占用5位，以(msb)5B 6G 5R(lsb)顺序按照CPU大端序存储，每个像素占16位*/
    LYN_PIX_FMT_BGR565LE          ,  /**< 交错模式，B占用5位，G占用6位,R占用5位，以(msb)5B 6G 5R(lsb)顺序按照CPU小端序存储，每个像素占16位*/
    LYN_PIX_FMT_BGR555BE          ,  /**< 交错模式，B占用5位，G占用5位,R占用5位，以(msb)1X 5B 5G 5R(lsb)顺序按照CPU大端序存储，X位未使用，每个像素占16位*/
    LYN_PIX_FMT_BGR555LE          ,  /**< 交错模式，B占用5位，G占用5位,R占用5位，以(msb)1X 5B 5G 5R(lsb)顺序按照CPU小端序存储，X位未使用，每个像素占16位*/

    LYN_PIX_FMT_END                  /**< 枚举结束*/
} lynPixelFormat_t;

/**
 * @brief 解码输出缩放倍数枚举
 * 
 */
typedef enum {
    SCALE_NONE = 0,    /**< 空类型，枚举开始*/
    SCALE_DOWN_2X = 2, /**< 2倍缩小*/
    SCALE_DOWN_4X = 4, /**< 4倍缩小*/
    SCALE_DOWN_8X = 8  /**< 8倍缩小*/
} lynScale_t;

/**
 * @brief 待解码包类型枚举
 *
 */
typedef enum {
    TYPE_AUDIO = 0, /**< 音频包*/
    TYPE_VIDEO,     /**< 视频包*/
} lynPacketTpye_t;


/**
 * @struct lynVdecAttr_t
 * @brief 这是一个视频解码器属性参数的结构体
 * @private
*/
typedef struct {
    lynCodecId_t codecId; /**< 待解码的码流编码协议格式 */
    lynPixelFormat_t outputFmt; /**< 待解码的码流图像格式 */
    lynScale_t scale; /**< 解码输出图像缩放倍数 */
    bool userPtrUsed; /**< 用户自定义输入帧信息启用标识 */

} lynVdecAttr_t;

/**
 * @struct lynJdecAttr_t
 * @brief 这是一个图像解码器属性参数的结构体
 * @private
*/
typedef struct {
    lynPixelFormat_t outputFmt; /**< 待解码的图像格式 */
    lynScale_t scale;  /**< 解码图像缩放倍数 */
} lynJdecAttr_t;


/**
 * @struct lynVdecOutInfo_t
 * @brief 这是一个视频解码器输出信息的结构体
 * @private
*/
typedef struct {
    uint32_t width;  /**< 视频解码输出图像宽 */
    uint32_t height; /**< 视频解码输出图像高 */
    uint32_t predictBufSize; /**< 视频解码输出图像大小 */
} lynVdecOutInfo_t;


/**
 * @struct lynCodecPara_t
 * @brief 这是一个视频、图像编解码参数的结构体
 * @private
*/
typedef struct {
    uint32_t width;   /**< 视频、图像宽 */
    uint32_t height;  /**< 视频、图像高 */
    lynCodecId_t codecId; /**< 视频、图像编码协议类型 */
    lynPixelFormat_t format; /**< 视频、图像的图像格式 */
} lynCodecPara_t;


/**
 * @struct lynAudioPara_t
 * @brief 这是一个音频参数的结构体
 * @private
*/
typedef struct {
    uint32_t codecId;  /**< 音频编码协议类型 */
    uint32_t samplesRate; /**< 音频采样率 */
    uint32_t bitrate; /**< 音频编码比特率 */
    uint32_t waveFormat; /**< 音频文件格式 */
    uint32_t channelNumber; /**< 音频声道数 */
    uint32_t blockAlign;    /**< 音频数据对齐方式 */
    uint32_t bitsPerSample; /**< 音频采样位宽 */
    uint32_t samplePerFrame; /**< 音频帧采样大小 */
    uint32_t frameInterval;  /**< 音频帧间隔 */
} lynAudioPara_t;


#ifndef LYN_PYTHON_API

/**
 * @struct lynCodecBuf_t
 * @brief 这是一个音视频buffer参数定义的结构体
 * @private
*/
typedef struct {
    /** @cond INTERNAL_UNION */
    union {
    /** @endcond */
        uint8_t *data;         /**< buffer地址,与dataResever互斥使用 */
        uint64_t dataResever;  /**< buffer地址，与data互斥使用*/
    /** @cond INTERNAL_UNION */
    };
    /** @endcond */
    uint32_t size;            /**< buffer大小 */
    bool eos;                 /**< 该buffer是否为结束buffer标识 */

    /**
     * @cond
     * Internal SDK usage
     */
    /** @cond INTERNAL_UNION */
    union {
    /** @endcond */
        void *privateData; /**< sdk内部使用，与 privateDataResever互斥使用*/
        uint64_t privateDataResever; /**< sdk内部使用，与privateData互斥使用 */
    /** @cond INTERNAL_UNION */
    };
    /** @endcond */
    /** @endcond */

    void *userPtr;            /**< 用户自定义数据地址 */
} lynCodecBuf_t;

/**
 * @brief 编码码流buffer
 *
 */
typedef lynCodecBuf_t lynPacket_t;

/**
 * @brief 解码帧buffer
 *
 */
typedef lynCodecBuf_t lynFrame_t;

/**
 * @struct lynImageDecPara_t
 * @brief 这是一个图像解码参数的结构体
 * @private
*/
typedef struct {
    const char *imgUrl; /**< 待解码图像的url地址 */
    lynScale_t scale;   /**< 解码图像输出缩放倍数 */
    lynPixelFormat_t outputFmt; /**< 解码图像输出的图像格式 */
    bool align;   /**< 解码输出图像是否启用宽高对齐 */
    uint8_t *data;  /**< 待解码图像的内存地址 */
    uint32_t size;  /**< 待解码图像的大小 */
} lynImageDecPara_t;

#endif

/**
 * @struct lynImageOutput_t
 * @brief 这是一个图像解码输出参数的结构体
 * @private
*/
typedef struct {
    uint32_t width;  /**< 图像解码输出图像宽 */
    uint32_t height; /**< 图像解码输出图像高 */
    lynScale_t scale; /**< 图像解码输出图像缩放倍数 */
    lynPixelFormat_t outputFmt; /**< 图像解码输出图像格式 */
    bool align; /**< 图像解码输出图像宽高是否启用对齐*/
    uint32_t predictBufSize; /**< 图像解码输出图像大小 */
} lynImageOutput_t;

/**
 * @struct lynImageInfo_t
 * @brief 这是一个图像解码信息的结构体
 * @private
*/
typedef struct {
    lynCodecPara_t input; /**< 输入信息 */
    lynImageOutput_t output; /**< 输出信息 */
} lynImageInfo_t;


/**
 * @struct lynImageDecAttr_t
 * @brief 这是一个图像解码属性的结构体
 * @private
*/
typedef struct {
    lynImageInfo_t info;/**< 输入图像属性 */
    lynCodecBuf_t inBuf; /**< 输入图像buffer */
    lynCodecBuf_t outBuf;/**< 输出图像buffer */
} lynImageDecAttr_t;

/**
 * @brief 码率控制类型枚举
 *
*/
typedef enum
{
    LYN_CODEC_RATE_CONTROL_MODE_OFF = 0, /**< 关闭码率控制 */
    LYN_CODEC_RATE_CONTROL_MODE_STANDARD, /**< 标准码率控制 */
    LYN_CODEC_RATE_CONTROL_MODE_VARIABLE, /**< 变码率控制 */
    LYN_CODEC_RATE_CONTROL_MODE_CONSTANT, /**< 定码率控制 */
} lynRateControlType_t;

/**
 * @brief gop类型枚举
 * 
*/
typedef enum
{
    LYN_CODEC_GOP_TYPE_NONE = 0, /**< 空类型，枚举开始*/
    LYN_CODEC_GOP_TYPE_BIDI, /**< BIDI类型*/
    LYN_CODEC_GOP_TYPE_LOW_DELAY, /**< LOW DELAT类型*/
    LYN_CODEC_GOP_TYPE_PYRAMID  /**< PYRAMID类型*/
} lynGopType_t;

/**
 * @brief 编码熵方式枚举
 *
*/
typedef enum
{
    LYN_ECM_TYPE_CAVLC = 0, /**< 基于上下文自适应可变长编码 */
    LYN_ECM_TYPE_CABAC /**< 基于下文的自适应二进制算术编码 */
} lynEcmType_t;

/**
 * @brief h264 profile类型枚举
 *
*/
typedef enum
{
    LYN_PROFILE_H264_BASELINE = 0, /**< h264 baseline编码 */
    LYN_PROFILE_H264_MAIN, /**< h264 main编码 */
    LYN_PROFILE_H264_HIGH, /**< h264 high编码 */
    LYN_PROFILE_H264_HIGH10, /**< h264 high10编码 */
} lynH264Profile_t;

/**
 * @brief h265 profile类型枚举
 *
*/
typedef enum
{
    LYN_PROFILE_H265_MAIN_STILL = 0, /**< h265 still编码 */
    LYN_PROFILE_H265_MAIN, /**< h265 main编码 */
    LYN_PROFILE_H265_MAIN10, /**< h265 main10编码 */
    LYN_PROFILE_H265_MAIN_INTRA, /**< h265 main intra编码 */
} lynH265Profile_t;


/**
 * @brief h265 tier类型枚举
 *
*/
typedef enum
{
    LYN_H265_TIER_MAIN = 0,  /**< h265 main tier编码 */
    LYN_H265_TIER_HIGH = 1,  /**< h265 high tier编码 */
} lynH265Tier_t;

/**
 * @struct lynVencAttr_t
 * @brief 这是一个视频编码参数的结构体
 * @private
*/
typedef struct
{
    lynCodecId_t codecType; /**< 视频编码协议类型 */
    lynPixelFormat_t inputFormat; /**< 视频编码输入图像格式 */
    uint32_t width; /**< 视频编码输出图像宽 */
    uint32_t height; /**< 视频编码输出图像高 */
    uint32_t profile; /**< 视频编码输出码流的编码profile */
    int32_t level; /**< 视频编码输出码流的编码level*/
    uint32_t tier; /**< 视频编码输出H265编码码流的tier*/
    float fps; /**< 视频编码输出码流的帧率*/
    lynRateControlType_t rateControl; /**< 视频编码码率控制方式*/
    uint32_t targetBitrate; /**< 视频编码输出码流的码率*/
    lynGopType_t gopType; /**< 视频编码输出码流GOP类型*/
    uint32_t pframesNum; /**< 视频编码输出码流每个GOP的P帧数量*/
    uint32_t bframesNum; /**< 视频编码输出码流每个GOP的B帧数量*/
    uint32_t fixedqpIFrame; /**< 视频编码输出码流的I帧量化步长(I帧QP值)*/
    uint32_t fixedqpPFrame; /**< 视频编码输出码流的P帧量化步长*/
    uint32_t fixedqpBFrame; /**< 视频编码输出码流的B帧量化步长*/
    uint32_t minQp;  /**< 视频编码输出码流的最小量化步长*/
    uint32_t maxQp;  /**< 视频编码输出码流的最大量化步长*/
    uint32_t bitdepth;   /**< 视频编码输入图像的位宽*/
    uint32_t ecm; /**< 视频编码熵编码方式*/
} lynVencAttr_t;

/**
 * @struct lynJencAttr_t
 * @brief 这是一个图像编码参数的结构体
 * @private
*/
typedef struct
{
    lynPixelFormat_t inputFormat; /**< 图像编码输入图像格式 */
    uint32_t width;  /**< 图像编码输出图像宽 */
    uint32_t height;  /**< 图像编码输出图像高 */
    uint32_t jpegQuality; /**< 图像编码输出图像的编码质量，一般情况下，qulity越大，图像编码质量越好*/
} lynJencAttr_t;

/**
 * @struct lynJencInfo_t
 * @brief 这是一个图像编码信息的结构体
 * @private
*/
typedef struct
{
    lynJencAttr_t params;  /**< 图像编码器参数 */
    lynCodecBuf_t input;   /**< 输入图像buffer */
    lynCodecBuf_t output;  /**< 输出图像buffer */
} lynJencInfo_t;

/*** @cond */
#define VALUE_1K 1024
#define VALUE_1M (1024 * 1024)
/*** @endcond*/


/**
 * @brief 获取编码码流包的实际大小
 * @param[in] packet   编码码流包
 * @param[out] validSize 码流包的实际大小
 * @return lynError_t枚举值
 */
lynError_t lynEncGetRemotePacketValidSize(const lynPacket_t *packet, uint32_t *validSize);

#ifdef __cplusplus
}
#endif
#endif
