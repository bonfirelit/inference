/**
 * @file lyn_ienc.h
 * @brief 图像编码模块的api定义说明
 * \cond
 * @author Lynxi SDK Team
 * @version 1.0
 * @date 2021-08-10
 *
 * Copyright:
 * © 2021 北京灵汐科技有限公司 版权所有。
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved.
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 * \endcond
 */

#ifndef LYN_IENC_C_H
#define LYN_IENC_C_H

#include "lyn_stream.h"
#include "lyn_codec_com.h"

#ifdef __cplusplus
extern "C"
{
#endif

   /**
    * @brief 图像编码器句柄
    */
    typedef void *lynJencHandle_t;

    /**
 * @brief 打开图像编码器
 *
 * @param[out] jencHdl 图像编码器句柄
 * @param[in]  attr 指定打开的图像编码器属性
 * @return lynError_t枚举值
 * @note 图像编码器句柄最多32个
 */
    lynError_t lynJencOpen(lynJencHandle_t *jencHdl, const lynJencAttr_t *attr);

    /**
 * @brief 发送待编码的原始图像帧数据到图像编码器
 *
 * @param[in]  stream   流句柄
 * @param[in]  jencHdl  图像编码器句柄
 * @param[in]  frame    待编码的图像帧数据
 * @return lynError_t枚举值
 */
    lynError_t lynJencSendFrameAsync(lynStream_t stream, const lynJencHandle_t jencHdl, const lynFrame_t *frame);

   /**
    * @brief 设置图像编码器输入图像宽高、图像格式
    *
    * @param[in]  jencHdl 图像编码器句柄
    * @param[in]  inputWidth 图像编码器输入图像宽
    * @param[in]  inputHeight 图像编码器输入图像高
    * @param[in]  inputFormat 图像编码器输入图像格式
    * @return lynError_t枚举值
    * @note 若设置图像编码器的attr中的输出图像宽高大小不等于本接口设置的输入图像宽高，
    *       接口内部会调用IPE接口对编码的输入进行缩放，以匹配编码的输出大小。
    */
   lynError_t lynJencSetInputConfig(lynJencHandle_t jencHdl, uint32_t inputWidth, uint32_t inputHeight, lynPixelFormat_t inputFormat);
    /**
 * @brief 接收图像编码器编码后的码流数据
 *
 * @param[in]  stream   流句柄
 * @param[in]  jencHdl  图像编码器句柄
 * @param[out] packet   接收到的图像编码码流包
 * @return lynError_t枚举值
 */
    lynError_t lynJencRecvPacketAsync(lynStream_t stream, const lynJencHandle_t jencHdl, const lynPacket_t *packet);

    /**
 * @brief 关闭图像编码器
 *
 * @param[in]  jencHdl 图像编码器句柄
 * @return lynError_t枚举值
 */
    lynError_t lynJencClose(const lynJencHandle_t jencHdl);

    /**
 * @brief 使用同步接口完成一帧图像的编码
 *
 * @param[in]  stream   流句柄
 * @param[in]  info 需要编码的图像信息，包括图像输入，编码数据输出和对应的编码参数
 * @return lynError_t枚举值
 * @note  接口帧率较低，通常用于需要变分辨率场景
 */
    lynError_t lynJpegEncodeAsync(lynStream_t stream, const lynJencInfo_t *info);

#ifdef __cplusplus
}
#endif

#endif