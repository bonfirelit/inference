/**
 * @file lyn_vdec.h
 * @brief 视频解码模块的api定义说明
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

#ifndef LYN_VDEC_C_H
#define LYN_VDEC_C_H

#include "lyn_err.h"
#include "lyn_context.h"
#include "lyn_stream.h"
#include "lyn_codec_com.h"

#ifdef __cplusplus
extern "C" {
#endif

 /**
 * @brief 视频解码器句柄
 */
typedef void* lynVdecHandle_t;

/**
 * @brief 打开视频解码器
 *
 * @param[out] vencHdl 视频解码器句柄
 * @param[in]  attr 视频解码器参数
 * @return lynError_t枚举值
 * 
 * @note 最多支持32个解码器句柄
 */
lynError_t lynVdecOpen(lynVdecHandle_t *vdecHdl, const lynVdecAttr_t *attr);

/**
 * @brief 获取视频解码输出信息
 *
 * @param[in]  para 输入源的参数
 * @param[in]  attr 视频解码器参数
 * @param[out]  outInfo 输出信息
 * @return lynError_t枚举值
 */
lynError_t lynVdecGetOutInfo(lynCodecPara_t *para, lynVdecAttr_t *attr, lynVdecOutInfo_t *outInfo);

/**
 * @brief 发送待解码包到视频解码器
 *
 * @param[in]  stream   流句柄
 * @param[in]  vdecHdl  视频解码器句柄
 * @param[in]  packet   待解码数据包
 * @return lynError_t枚举值
 */
lynError_t lynVdecSendPacketAsync(lynStream_t stream, const lynVdecHandle_t vdecHdl, const lynPacket_t *packet);

/**
 * @brief 发送跳过解码结果的待解码包，调用此接口前需要确保lynVdecAttr的userPtrUsed设置为true
 *
 * @param[in]  stream   流句柄
 * @param[in]  vdecHdl  视频解码器句柄
 * @param[in]  packet   不需要解码输出的待解码包
 * @return lynError_t枚举值
 */
lynError_t lynVdecSendSkippedPacketAsync(lynStream_t stream, const lynVdecHandle_t vdecHdl,const lynPacket_t *packet);

/**
 * @brief 接收视频解码器解码后的数据
 *
 * @param[in]  stream   流句柄
 * @param[in]  vdecHdl  视频解码器句柄
 * @param[inout]  frame    接收的视频原始帧数据
 * @return lynError_t枚举值
 */
lynError_t lynVdecRecvFrameAsync(lynStream_t stream, const lynVdecHandle_t vdecHdl, lynFrame_t *frame);

/**
 * @brief 关闭视频解码器
 *
 * @param[in]  vdecHdl 视频解码器句柄
 * @return lynError_t枚举值
 */
lynError_t lynVdecClose(const lynVdecHandle_t vdecHdl);

#ifdef __cplusplus
}
#endif

#endif