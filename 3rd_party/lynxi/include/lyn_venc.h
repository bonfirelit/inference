/**
 * @file lyn_venc.h
 * @brief 视频编码模块的api定义说明
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

#ifndef LYN_VENC_C_H
#define LYN_VENC_C_H

#include "lyn_stream.h"
#include "lyn_codec_com.h"

#ifdef __cplusplus
extern "C"
{
#endif

 /**
 * @brief 视频编码器句柄
 */
  typedef void *lynVencHandle_t;

  /**
 * @brief 打开视频编码器
 *
 * @param[out] vencHdl 视频编码器句柄
 * @param[in]  attr 视频编码器参数
 * @return lynError_t枚举值
 * @note 视频编码器句柄最多32个
 */
  lynError_t lynVencOpen(lynVencHandle_t *vencHdl, const lynVencAttr_t *attr);

  /**
   * @brief 设置视频编码器输入图像宽高、图像格式
   *
   * @param[in]  vencHdl 视频编码器句柄
   * @param[in]  inputWidth 视频编码器输入图像宽
   * @param[in]  inputHeight 视频编码器输入图像高
   * @param[in]  inputFormat 视频编码器输入图像格式
   * @return lynError_t枚举值
   * @note 若设置视频编码器的attr中的输出图像宽高大小不等于本接口设置的输入图像宽高，
   *       接口内部会调用IPE接口对编码的输入进行缩放，以匹配编码的输出大小。
   */
  lynError_t lynVencSetInputConfig(lynVencHandle_t vencHdl, uint32_t inputWidth, uint32_t inputHeight, lynPixelFormat_t inputFormat);

  /**
 * @brief 获取视频编码码流图像参数集(PPS)和序列参数集(SPS)
 *
 * @param[in]  stream   流句柄
 * @param[in]  vencHdl  视频编码器句柄
 * @param[out] packet   获取到的PPS和SPS码流包
 * @return lynError_t枚举值
 */
  lynError_t lynVencGetParamsSetAsync(lynStream_t stream, const lynVencHandle_t vencHdl, const lynPacket_t *packet);

  /**
 * @brief 发送待编码的原始视频帧数据到视频编码器
 *
 * @param[in]  stream   流句柄
 * @param[in]  vencHdl  视频编码器句柄
 * @param[in]  frame    待发送的原始视频帧数据
 * @return lynError_t枚举值
 */
  lynError_t lynVencSendFrameAsync(lynStream_t stream, const lynVencHandle_t vencHdl, const lynFrame_t *frame);

  /**
 * @brief 接收视频编码器编码后的码流数据
 *
 * @param[in]  stream   流句柄
 * @param[in]  vencHdl  视频编码器句柄
 * @param[out] packet   接收到的视频编码码流包
 * @return lynError_t枚举值
 */
  lynError_t lynVencRecvPacketAsync(lynStream_t stream, const lynVencHandle_t vencHdl, const lynPacket_t *packet);

  /**
 * @brief 强制编码I帧
 *
 * @param[in]  stream   流句柄
 * @param[in]  vencHdl  视频编码器句柄
 * @return lynError_t枚举值
 * @note 在lynVencSendFrameAsync之前调用本接口，发送的帧将强制编码为I帧，
 *       本接口不支持在含有B帧编码时使用
 */
  lynError_t lynVencForceKeyFrame(lynStream_t stream, const lynVencHandle_t vencHdl);

  /**
 * @brief 关闭视频编码器
 *
 * @param[in]  vencHdl 视频编码器句柄
 * @return lynError_t枚举值
 */
  lynError_t lynVencClose(const lynVencHandle_t vencHdl);

  /**
 * @brief 配置默认的视频编码参数，方便用户在默认参数的基础上再配置自己关心的参数
 *
 * @param[out]  params  the default params.
 * @return lynError_t枚举值
 */
  lynError_t lynVencSetDefaultParams(lynVencAttr_t *params);

#ifdef __cplusplus
}
#endif

#endif