/**
 * @file lyn_ipe.h
 * @brief 图像预处理模块的api定义说明
 * \cond
 * @author Lynxi SDK Team
 * @version v1.0
 * @date 2021-08-10
 *
 * Copyright:
 * © 2018 北京灵汐科技有限公司 版权所有。\n
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。\n
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved. \n
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 * \endcond
 */
#ifndef LYN_IPE_C_H
#define LYN_IPE_C_H

#include "lyn_codec_com.h"
#include "lyn_stream.h"
#include "lyn_err.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

   /**
    * @brief 图像描述句柄
    */
    typedef void *lynIpePicDesc_t;

    /**
    * @brief 图像配置句柄
    */
    typedef void *lynIpeConfigDesc_t;

    /**
    * @brief 图像仿射变换句柄
    */
    typedef void *lynIpeAffineHandle_t;

    /**
    * @brief 图像批处理句柄
    */
    typedef void *lynIpeBatchHandle_t;

    /**
    * @brief 常用图像预处理多batch配置参数
    * @private
    */
    typedef struct {
        bool cropEnable;     /**< 使能裁剪*/
        uint32_t cropX;      /**< 裁剪起始X坐标*/
        uint32_t cropY;      /**< 裁剪起始Y坐标*/
        uint32_t  cropWidth;  /**< 裁剪宽*/
        uint32_t cropHeight;  /**< 裁剪高*/

        bool resizeEnable;    /**< 使能缩放*/
        uint32_t resizeWidth; /**< 缩放后宽度*/
        uint32_t resizeHeight;/**< 缩放后高度*/

        bool  c2cEnable;       /**< 使能颜色空间转换*/
        lynPixelFormat_t c2cFormat;/**< 目标颜色空间类型*/
        uint32_t c2cStandard;  /**< YUV to RGB转换标准*/

        void* outputData; /**< 输出图像地址*/
    } lynIPEParam_t;



/**
 * @brief 创建图像描述句柄
 *
 * @param[out]  pic 图像描述句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeCreatePicDesc(lynIpePicDesc_t *pic);

/**
 * @brief 创建图像配置描述句柄
 *
 * @param[out]  config 图像配置描述句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeCreateConfigDesc(lynIpeConfigDesc_t *config);

/**
 * @brief 重置图像描述
 *
 * @param[in]  pic 图像描述句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeResetPicDesc(lynIpePicDesc_t pic);

/**
 * @brief 重置图像配置描述
 *
 * @param[in]  config 图像配置描述句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeResetConfigDesc(lynIpeConfigDesc_t config);

/**
 * @brief 销毁图像描述
 *
 * @param[in]  config 图像描述句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeDestroyPicDesc(lynIpePicDesc_t pic);

/**
 * @brief 销毁图像配置描述
 *
 * @param[in]  config 图像配置描述句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeDestroyConfigDesc(lynIpeConfigDesc_t config);

/**
 * @brief 设置输入图像描述
 *
 * @param[out]  input 输入图像描述句柄
 * @param[in]  data   输入图像数据地址
 * @param[in]  width   图像宽
 * @param[in]  height  图像高
 * @param[in]  format  图像类型
 *
 * @return lynError_t枚举值
 */

lynError_t lynIpeSetInputPicDesc(lynIpePicDesc_t input, void *data, int32_t width, int32_t height, lynPixelFormat_t format);

/**
 * @brief 设置输出图像描述
 *
 * @param[out] output  输出图像描述句柄
 * @param[in]  data    输出数据地址
 * @param[in]  width   图像宽
 * @param[in]  height  图像高
 * @param[in]  format  图像类型
 *
 * @return lynError_t枚举值
 */

lynError_t lynIpeSetOutputPicDesc(lynIpePicDesc_t output, void *data, int32_t width, int32_t height, lynPixelFormat_t format);

/**
 * @brief 配置裁剪的图像配置描述
 *
 * @param[out]  config 图像配置描述句柄
 * @param[in]  x 裁剪的起始X坐标
 * @param[in]  y 裁剪的起始Y坐标
 * @param[in]  width 裁剪的宽
 * @param[in]  height 裁剪的高
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeSetCropConfig(lynIpeConfigDesc_t config, int32_t x, int32_t y, int32_t width, int32_t height);

/**
 * @brief 配置图像拼接，设置图像在输出图中指定位置
 * 
 * @param[out]  config 图像配置描述句柄
 * @param[in]  dst_x 指定位于输出图像的起始X坐标
 * @param[in]  dst_y 指定位于输出图像的起始Y坐标
 * @param[in]  width 输入ROI期望的输出宽度
 * @param[in]  height 输入ROI期望的输出高度
 * @param[in]  resize_keep_aspectratio  宽高方向是否等比缩放
 * @return lynError_t枚举值
 */
lynError_t lynIpeSetOutputRect(lynIpeConfigDesc_t config, int32_t dst_x, int32_t dst_y, int32_t width, int32_t height, bool resize_keep_aspectratio);


/**
 * @brief 使能上下翻转
 *
 * @param[out]  config 图像配置描述句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeEnableFlip(lynIpeConfigDesc_t config);

/**
 * @brief 设置缩放
 *
 * @param[out]  config 图像配置描述句柄
 * @param[in]  width 缩放后的宽度
 * @param[in]  height 缩放后的高度
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeSetResizeConfig(lynIpeConfigDesc_t config, int32_t width, int32_t height);

/**
 * @brief 使能颜色空间转换
 *
 * @param[out]  config 图像配置描述句柄
 * @param[in]  format 目标类型
 * @param[in]  standard YUV to RGB 转换标准
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeSetC2CConfig(lynIpeConfigDesc_t config, lynPixelFormat_t format, int32_t standard);

/**
 * @brief 使能边界填充
 *
 * @param[out]  config 图像配置描述句柄
 * @param[in]  top 向上填充的行数
 * @param[in]  right 向右填充的列数
 * @param[in]  bottom 向下填充的行数
 * @param[in]  left 向左填充的列数
 * @param[in]  xColor 填充的灰度值x
 * @param[in]  yColor 填充灰度值y
 * @param[in]  zColor 填充灰度值z
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeSetPadConfig(lynIpeConfigDesc_t config, int32_t top, int32_t right, int32_t bottom, int32_t left, uint8_t xColor, uint8_t yColor, uint8_t zColor);

/**
 * @brief 使能镜像模块
 *
 * @param[out]  config 图像配置描述句柄
 * @return lynError_t枚举值
 */
lynError_t lynIpeEnableMirror(lynIpeConfigDesc_t config);

/**
 * @brief 使能旋转模块
 *
 * @param[out]  config 图像配置描述句柄
 * @param[in]  angle   旋转角度
 * @param[in]  mode    设置旋转模式
 * @param[in]  xColor  填充灰度值x
 * @param[in]  yColor  填充灰度值y
 * @param[in]  zColor  填充灰度值z
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeSetRotateConfig(lynIpeConfigDesc_t config, float angle, int32_t mode, uint8_t xColor, uint8_t yColor, uint8_t zColor);

/**
 * @brief 使能仿射变换
 *
 * @param[out]  config 图像配置描述句柄
 * @param[in]  matrix  2*3 的仿射变换矩阵
 * @param[in]  xColor  填充灰度值x
 * @param[in]  yColor  填充灰度值y
 * @param[in]  zColor  填充灰度值z
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeSetAffineConfig(lynIpeConfigDesc_t config, float (*matrix)[3], uint8_t xColor, uint8_t yColor, uint8_t zColor);


/**
 * @brief 计算输出图像描述
 *
 * @param[out]  output 输出图像描述句柄
 * @param[in]  input   输入图像描述句柄
 * @param[in]  config  图像配置描述句柄
 * @param[in]  passageID 选择高速通路ID,取值0～4
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeCalOutputPicDesc(lynIpePicDesc_t output, lynIpePicDesc_t input, lynIpeConfigDesc_t config, int32_t passageID);

/**
 * @brief 设置输出图像数据地址
 *
 * @param[out]  output 输出图像描述句柄
 * @param[in]  data 图像数据地址
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeSetOutputPicData(lynIpePicDesc_t output, void *data);

/**
 * @brief 执行图像异步处理
 *
 * @param[in]  stream 流句柄 
 * @param[in]  input 输入图像描述句柄
 * @param[out]  output 输出图像描述句柄
 * @param[in]  config 图像配置描述句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeProcessAsync(lynStream_t stream, lynIpePicDesc_t input, lynIpePicDesc_t output, lynIpeConfigDesc_t config);

/**
 * @brief 获取图像描述信息
 *
 * @param[in]  pic     图像描述句柄
 * @param[out]  data   图像数据地址
 * @param[out]  width  图像宽
 * @param[out]  height 图像高
 * @param[out]  format 图像类型
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeGetPicDesc(lynIpePicDesc_t pic, void **data, int32_t *width, int32_t *height, lynPixelFormat_t *format);


/**
 * @brief 打开仿射变换
 *
 * @param[out]  handle 仿射变换句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeOpenAffineHandle(lynIpeAffineHandle_t* handle);

/**
 * @brief 关闭仿射变换
 *
 * @param[in]  handle 仿射变换句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeCloseAffineHandle(lynIpeAffineHandle_t handle);

/**
 * @brief 使能仿射变换,必须先调用lynIpeOpenAffineHandle打开句柄，相较lynIpeSetAffineConfig，
 * lynIpeSetAffineConfigV2性能损耗更低
 *
 * @param[out]  config 图像配置描述句柄
 * @param[in]  matrix  2*3 的仿射变换矩阵
 * @param[in]  xColor  填充灰度值x
 * @param[in]  yColor  填充灰度值y
 * @param[in]  zColor  填充灰度值z
 * @param[in]  handle  仿射变换句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeSetAffineConfigV2(lynIpeConfigDesc_t config, float (*matrix)[3], uint8_t xColor, uint8_t yColor, uint8_t zColor, lynIpeAffineHandle_t handle);


/**
 * @brief 执行图像处理的同步接口
 *
 * @param[in]  input 输入图像描述句柄
 * @param[out]  output 输出图像描述句柄
 * @param[in]  config 图像配置句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeProcess(lynIpePicDesc_t input, lynIpePicDesc_t output, lynIpeConfigDesc_t config);

/**
 * @brief 创建图像批处理句柄
 *
 * @param[out]  ipeBatchHandle 图像批处理句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeCreateBatchHandle(lynIpeBatchHandle_t *ipeBatchHandle);

/**
 * @brief 销毁图像批处理句柄
 *
 * @param[in]  handle 图像批处理句柄
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeDestroyBatchHandle(lynIpeBatchHandle_t ipeBatchHandle);

/**
 * @brief 执行图像批处理异步接口
 *
 * @param[in]  stream 流句柄
 * @param[in]  ipeBatchHandle 图像批处理句柄
 * @param[in]  input 输入图像描述
 * @param[in]  param  图像配置参数
 * @param[in]  roiNum 处理图像个数
 *
 * @return lynError_t枚举值
 */
lynError_t lynIpeBatchProcessAsync(lynStream_t stream, lynIpeBatchHandle_t ipeBatchHandle, lynIpePicDesc_t input,  lynIPEParam_t* param, uint32_t roiNum);


#ifdef __cplusplus
}
#endif

#endif
