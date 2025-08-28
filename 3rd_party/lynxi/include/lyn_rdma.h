/**
 * @file lyn_rdma.h
 * @brief rdma 模块的 api 定义说明
 * \cond
 * @author Lynxi SDK Team
 * @version v1.0
 * @date 2024-11-01
 *
 * Copyright:
 * © 2018 北京灵汐科技有限公司 版权所有。\n
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。\n
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved. \n
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or
 * distributed without the permission of Lynxi Technologies Co., Ltd.
 * \endcond
 */

#ifndef LYN_RDMA_C_H
#define LYN_RDMA_C_H

#include "lyn_context.h"
#include "lyn_err.h"
#include "lyn_stream.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 连接句柄
 */
typedef void *lynRdma_t;

/**
 * @brief 注册的内存类型
 */
typedef enum {
    RDMA_REG_HOST_MEMORY = 0, /**< 主机侧内存 */
    RDMA_REG_DEVICE_MEMORY,   /**< 设备侧内存 */
    RDMA_REG_MEMORY_MAX,      /**< 无效的内存类型 */
} lynRdmaRegMemType_e;

/**
 * @struct lynRdmaParam_t
 * @brief 建立连接的参数
 * @private
 */
typedef struct {
    char mlx_dev[32]; /**< IB 网卡名，例如 mlx5_0, ibp101s0 */
    int32_t lyn_dev;  /**< 灵汐设备编号，该字段已被弃用 */
    char ip[32];      /**< 服务端主机的 IP 地址 */
    uint16_t ip_port; /**< 服务端主机的 IP 端口号, 例如 19668 */
    uint16_t ib_port; /**< IB 网卡的端口号，一般为 1 */
} lynRdmaParam_t;

/**
 * @brief 连接 host 两端，注意，服务端和客户端必须都调用，且服务端必须比客户端先调用该接口。
 *
 * @param[in] param 连接时需要的参数
 * @param[out] handle 返回连接的 handle
 * @return lynError_t 枚举值
 */
lynError_t lynRdmaConnect(lynRdmaParam_t param, lynRdma_t *handle);

/**
 * @brief 断开连接
 *
 * @param[in] handle 连接时分配的 handle
 * @return lynError_t 枚举值
 */
lynError_t lynRdmaDisconnect(lynRdma_t handle);

/**
 * @brief 注册内存
 *
 * @param[in] handle 连接时分配的 handle
 * @param[in] vaddr malloc/lynMalloc 分配的地址
 * @param[in] size 空间大小
 * @param[in] type 默认是 RDMA_REG_DEVICE_MEMORY，表明注册的是 device 上的内存，即通过 lynMalloc 分配的内存。
 *            type 为 RDMA_REG_HOST_MEMORY，表明注册的是 host 上的内存，即通过 malloc 分配的内存。
 * @return lynError_t 枚举值
 */
lynError_t lynRdmaRegisterMemory(lynRdma_t handle, void *vaddr, size_t size, lynRdmaRegMemType_e type);

/**
 * @brief 取消内存注册
 *
 * @param[in] handle 连接时分配的 handle
 * @param[in] vaddr malloc/lynMalloc 分配的地址
 * @return lynError_t 枚举值
 */
lynError_t lynRdmaUnregisterMemory(lynRdma_t handle, void *vaddr);

/**
 * @brief 发送数据到对端
 *
 * @param[in] handle 连接时分配的 handle
 * @param[in] src 从地址 src 处发送数据
 * @param[in] size 发送的数据大小
 * @return lynError_t 枚举值
 */
lynError_t lynRdmaSend(lynRdma_t handle, void *src, size_t size);

/**
 * @brief 接收对端的数据
 *
 * @param[in] handle 连接时分配的 handle
 * @param[in] dst 从地址 dst 处接收 lynRdmaSend 发送的数据，用户保证预留的空间能够存放将要接收的数据
 * @return lynError_t 枚举值
 */
lynError_t lynRdmaReceive(lynRdma_t handle, void *dst);

/**
 * @brief 发送数据到对端
 *
 * @param[in] handle 连接时分配的 handle
 * @param[in] src 从地址 src 处发送数据
 * @param[in] size 发送的数据大小
 * @return lynError_t 枚举值
 */
lynError_t lynRdmaSendV2(lynRdma_t handle, void *src, size_t size);

/**
 * @brief 接收对端的数据
 *
 * @param[in] handle 连接时分配的 handle
 * @param[in] dst 从地址 dst 处接收 lynRdmaSendV2 发送的数据，用户保证预留的空间能够存放将要接收的数据
 * @param[in] size 接收的数据大小，size 最好与发送时的 size 大小一致，不然可能会导致发送超时或者接收失败
 * @return lynError_t 枚举值
 */
lynError_t lynRdmaReceiveV2(lynRdma_t handle, void *dst, size_t size);

/**
 * @brief 发送数据到对端（异步）
 *
 * @param[in] handle 连接时分配的 handle
 * @param[in] stream 流句柄
 * @param[in] src 从地址 src 处发送数据
 * @param[in] size 发送的数据大小
 * @return lynError_t 枚举值
 */
lynError_t lynRdmaSendAsync(lynRdma_t handle, lynStream_t stream, void *src, size_t size);

/**
 * @brief 接收对端的数据（异步）
 *
 * @param[in] handle 连接时分配的 handle
 * @param[in] stream 流句柄
 * @param[in] dst 从地址 dst 处接收 lynRdmaSend 发送的数据，用户保证预留的空间能够存放将要接收的数据
 * @return lynError_t 枚举值
 */
lynError_t lynRdmaReceiveAsync(lynRdma_t handle, lynStream_t stream, void *dst);

/**
 * @brief 发送数据到对端（异步）
 *
 * @param[in] handle 连接时分配的 handle
 * @param[in] stream 流句柄
 * @param[in] src 从地址 src 处发送数据
 * @param[in] size 发送的数据大小
 * @return lynError_t 枚举值
 */
lynError_t lynRdmaSendAsyncV2(lynRdma_t handle, lynStream_t stream, void *src, size_t size);

/**
 * @brief 接收对端的数据（异步）
 *
 * @param[in] handle 连接时分配的 handle
 * @param[in] stream 流句柄
 * @param[in] dst 从地址 dst 处接收 lynRdmaSend 发送的数据，用户保证预留的空间能够存放将要接收的数据
 * @param[in] size 接收的数据大小，size 最好与发送时的 size 大小一致，不然可能会导致发送超时或者接收失败
 * @return lynError_t 枚举值
 */
lynError_t lynRdmaReceiveAsyncV2(lynRdma_t handle, lynStream_t stream, void *dst, size_t size);

#ifdef __cplusplus
}
#endif

#endif  // LYN_RDMA_C_H
