/**
 * @file lyn_mux.h
 * @brief 视频封装模块的api定义说明
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

#ifndef LYN_MUX_C_H
#define LYN_MUX_C_H

#include "lyn_err.h"
#include "lyn_codec_com.h"
#include "lyn_venc.h"

#ifdef __cplusplus
extern "C"
{
#endif
    /**
     * @struct lynMuxParam_t
     * @brief 这是一个表示视频封装器参数的结构体
     * @private
    */
    typedef struct
    {
        lynCodecId_t codecType;/**< 待封装的码流编码协议格式 */
        float fps; /**< 需要封装的视频帧率  */
    } lynMuxParam_t;

    /**
     * @struct lynMuxPacket_t
     * @brief 这是一个表示视频码流数据的结构体，该数据为待封装的编码码流数据
     * @private
    */
    typedef struct
    {
        lynCodecBuf_t *pkt;/**< 待封装的编码码流包  */
    } lynMuxPacket_t;

    /**
     * @brief 视频封装器句柄
    */
    typedef void *lynMuxHandle_t;
    
    /**
     * @brief 打开视频封装器，一个视频封装器只支持对一路视频流进行封装
     *
     * @param[out] handle 视频封装器句柄
     * @param[in] vencHdl 视频编码器句柄
     * @param[in] url   视频封装后的包含绝对路径的文件名
     * @return lynError_t枚举值
     */
    lynError_t lynMuxOpen(lynMuxHandle_t *handle,lynVencHandle_t vencHdl , const char *url);

    /**
     * @brief 向视频封装器中写入带封装的视频码流数据
     *
     * @param[in] handle 视频封装器句柄
     * @param[in] packet 待封装的视频编码码流包
     * @return lynError_t枚举值
     */
    lynError_t lynMuxWritePacket(lynMuxHandle_t handle, lynMuxPacket_t *packet);

    /**
     * @brief 关闭视频封装器
     *
     * @param[in] handle 视频封装器句柄
     * @return lynError_t枚举值
     */
    lynError_t lynMuxClose(lynMuxHandle_t handle);

#ifdef __cplusplus
}
#endif

#endif