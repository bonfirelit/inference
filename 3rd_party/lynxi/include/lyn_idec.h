/**
 * @file lyn_idec.h
 * @brief 图像解码模块的api定义说明
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

#ifndef LYN_IDEC_C_H
#define LYN_IDEC_C_H

#include "lyn_err.h"
#include "lyn_context.h"
#include "lyn_stream.h"
#include "lyn_codec_com.h"

/**
* @brief 图像解码器句柄
*/
typedef void* lynJdecHandle_t;

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief 获取图像信息
 *
 * @param[in]  para 图像输入参数
 * @param[out] imageInfo 图像信息
 * @param[out] support 硬件是否支持
 * @return lynError_t枚举值
 */
lynError_t lynImageGetInfo(const lynImageDecPara_t *para, lynImageInfo_t *imageInfo, bool *support);

/**
 * @brief 图像软解码
 *
 * @param[in]  attr 图像属性信息，属性中输入输出buf都是Host内存
 * @return lynError_t枚举值
 */
lynError_t lynImageDecodeSoft(const lynImageDecAttr_t *attr);

/**
 * @brief 图像解码异步接口 需保证JPEG图像已被拷贝至Server侧内存
 *
 * @param[in]   stream 流句柄
 * @param[in]   attr 图像属性信息，
 * @return lynError_t枚举值
 * 
 * @note 需要硬件支持的格式
 */
lynError_t lynJpegDecodeAsync(lynStream_t stream, const lynImageDecAttr_t *attr);

/**
 * @brief 打开图像解码器
 *
 * @param[out]   jdecHdl 图像解码器句柄
 * @param[in]   attr 图像属性信息
 * @return lynError_t枚举值
 * 
 * @note 图像解码器最多可以打开16个
 */
lynError_t lynJdecOpen(lynJdecHandle_t *jdecHdl, const lynJdecAttr_t *attr);

/**
 * @brief 发送待解码图像到图像解码器
 *
 * @param[in]   stream 流句柄
 * @param[in]   jdecHdl 图像解码器句柄
 * @param[in]   packet 待解码数据包
 * @return lynError_t枚举值
 */
lynError_t lynJdecSendPacketAsync(lynStream_t stream, const lynJdecHandle_t jdecHdl, const lynPacket_t *packet);

/**
 * @brief 接收图像解码后的数据
 *
 * @param[in]   stream 流句柄
 * @param[in]   jdecHdl 图像解码器句柄
 * @param[out]   frame 图像解码后数据
 * @return lynError_t枚举值
 * 
 * @note 由于KA200硬件解码器的工作特性，为了实现视频解码高效运行，建议lynJdecSendPacketAsync和lynJdecRecvFrameAsync接口创建各自的Stream进行使用。
 */
lynError_t lynJdecRecvFrameAsync(lynStream_t stream, const lynJdecHandle_t jdecHdl, lynFrame_t *frame);

/**
 * @brief 关闭解码器
 *
 * @param[in]   jdecHdl  图像解码器句柄
 * @return lynError_t枚举值
 */
lynError_t lynJdecClose(const lynJdecHandle_t jdecHdl);

#ifdef __cplusplus
}
#endif
#endif