/**
 * @file lyn_demux.h
 * @brief 视频解封装模块的api定义说明
 * \cond
 * @author Lynxi SDK Team
 * @version v1.0
 * @date 2021-08-10
 *
 * Copyright:
 * © 2018 北京灵汐科技有限公司 版权所有。
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved.
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 * \endcond
 */

#ifndef LYN_DEMUX_C_H
#define LYN_DEMUX_C_H

#include "lyn_err.h"
#include "lyn_context.h"
#include "lyn_stream.h"
#include "lyn_vdec.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 视频解封装器句柄
 */
typedef void * lynDemuxHandle_t;

/**
 * @brief 打开解封装器
 *
 * @param[out] demuxHdl 解封装器句柄
 * @param[in]  url 本地文件路径或者媒体文件的网址
 * @param[in]  attr 属性设置, 默认设置为空指针;值为-support_audio时，支持获取音频包;值为-support_udp时，指定UDP协议拉取实时流
 * @return lynError_t枚举值
 * 
 * @note 不支持tcp协议推流时指定udp拉取实时流
 */
lynError_t lynDemuxOpen(lynDemuxHandle_t *demuxHdl, const char *url, const char *attr);

/**
 * @brief 获取视频流的信息
 *
 * @param[in]  demuxHdl 解封装器句柄
 * @param[out]  codecPara 参数信息
 * @return lynError_t枚举值
 */
lynError_t lynDemuxGetCodecPara(lynDemuxHandle_t demuxHdl, lynCodecPara_t *codecPara);


/**
 * @brief 获取音频流的信息
 *
 * @param[in]  demuxHdl 解封装器句柄
 * @param[out]  audioPara 参数信息
 * @return lynError_t枚举值
 */
lynError_t lynDemuxGetAudioPara(lynDemuxHandle_t demuxHdl, lynAudioPara_t *audioPara);

/**
 * @brief 获取流的帧率
 *
 * @param[in]  demuxHdl 解封装器句柄
 * @param[out]  fps 流的平均帧率
 * @return lynError_t枚举值
 */
lynError_t lynDemuxGetFrameRate(lynDemuxHandle_t demuxHdl, uint32_t *fps);

/**
 * @brief 从解封装器中获取一个编码包
 *
 * @param[in]  demuxHdl 解封装器句柄
 * @param[out]  packet 数据包
 * @return lynError_t枚举值
 */
lynError_t lynDemuxReadPacket(lynDemuxHandle_t demuxHdl, lynPacket_t *packet);

/**
 * @brief 从解封装器中获取包类型
 *
 * @param[in]  demuxHdl 解封装器句柄
 * @param[in]  packet 数据包
 * @param[out]  type 包类型，I帧包为1，非I帧包为0，音频包为99
 * @return lynError_t枚举值
 */
lynError_t lynDemuxGetPacketType(lynDemuxHandle_t demuxHdl, lynPacket_t *packet,uint32_t *type);

/**
 * @brief 从解封装器中获取包时间戳
 *
 * @param[in]  demuxHdl 解封装器句柄
 * @param[in]  packet 数据包
 * @param[out] timestamp 时间戳
 * @return lynError_t枚举值
 */
lynError_t lynDemuxGetPacketTimeStamp(lynDemuxHandle_t demuxHdl, lynPacket_t *packet, int64_t *timestamp);

/**
 * @brief 释放数据包内存
 *
 * @param[in]  packet 数据包
 * @return lynError_t枚举值
 */
lynError_t lynDemuxFreePacket(lynPacket_t *packet);

/**
 * @brief 关闭解封装器
 *
 * @param[in]  demuxHdl 解封装器句柄
 * @return lynError_t枚举值
 */
lynError_t lynDemuxClose(lynDemuxHandle_t demuxHdl);

#ifdef __cplusplus
}
#endif

#endif