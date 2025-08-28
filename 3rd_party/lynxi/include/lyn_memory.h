/**
 * @file lyn_memory.h
 * @brief memory模块的api说明
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

#ifndef LYN_MEMORY_C_H
#define LYN_MEMORY_C_H

#include "lyn_err.h"
#include "lyn_stream.h"
#include "lyn_context.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 数据拷贝方向
 */
typedef enum _lynMemcpyDir_t{
    ClientToServer = 1, /**< 内存拷贝主机到设备方向 */
    ServerToClient = 2, /**< 内存拷贝设备到主机方向 */
    ServerToServer = 3, /**< 内存拷贝设备到设备方向，如果是同一个设备的两个DevPtr则进行设备内部的sysdma拷贝，如果是不同设备的DevPtr则进行跨设备的P2P拷贝 */
    MultiProcessP2P = 4, /**< 多进程P2P拷贝 */
} lynMemcpyDir_t;

/**
 * @brief 内存分配
 *
 * @param[in]  size 字节数
 * @param[out]  devPtr 分配到的设备地址
 * @return lynError_t枚举值
 */
lynError_t lynMalloc(void **devPtr, uint64_t size);

/**
 * @brief 内存释放
 *
 * @param[in]  devPtr 设备地址
 * @return lynError_t枚举值
 */
lynError_t lynFree(void *devPtr);

/**
 * @brief 内存拷贝（同步模式）
 *
 * @param[in]  dst 目的地址
 * @param[in]  src 源地址
 * @param[in]  size 字节数
 * @param[in]  dir 拷贝方向
 * @return lynError_t枚举值
 */
lynError_t lynMemcpy(void *dst, const void *src, uint64_t size, lynMemcpyDir_t dir);

/**
 * @brief 内存拷贝（异步模式）
 *
 * @param[in]  stream 流句柄
 * @param[in]  dst 目的地址
 * @param[in]  src 源地址
 * @param[in]  size 字节数
 * @param[in]  dir 拷贝方向
 * @return lynError_t枚举值
 */
lynError_t lynMemcpyAsync(lynStream_t stream, void *dst, const void *src, uint64_t size, lynMemcpyDir_t dir);

/**
 * @brief 内存设置（同步模式）
 *
 * @param[in]  devPtr 设备地址
 * @param[in]  value 设置值
 * @param[in]  size 字节数
 * @return lynError_t枚举值
 */
lynError_t lynMemset(void *devPtr, int32_t value, uint64_t size);

/**
 * @brief 内存设置（异步模式）
 *
 * @param[in]  stream 流句柄
 * @param[in]  devPtr 设备地址
 * @param[in]  value 设置值
 * @param[in]  size 字节数
 * @return lynError_t枚举值
 */
lynError_t lynMemsetAsync(lynStream_t stream, void *devPtr, int32_t value, uint64_t size);

/**
 * @brief 通过devPtr获取对应设备上的虚拟地址
 *
 * @param[in]  devPtr 设备地址
 * @param[out]  virtAddr 虚拟地址
 * @return lynError_t枚举值
 * @note 该接口只在RC模式下可用
 */
lynError_t lynDevPtr2VirtAddr(void *devPtr, void **virtAddr);

/**
 * @brief 内存拷贝（arm32平台，同步模式）
 *
 * @param[in]  dst 目的地址
 * @param[in]  dstOffset 目的地址偏移
 * @param[in]  src 源地址
 * @param[in]  srcOffset 源地址偏移
 * @param[in]  size 字节数
 * @param[in]  dir 拷贝方向
 * @return lynError_t枚举值
 * @note 该接口只在arm32平台下可用
 */
lynError_t lynMemcpyArm32(void *dst, uint64_t dstOffset, const void *src, uint64_t srcOffset, uint64_t size, lynMemcpyDir_t dir);

/**
 * @brief 内存拷贝（arm32平台，异步模式）
 *
 * @param[in]  stream 流句柄
 * @param[in]  dst 目的地址
 * @param[in]  dstOffset 目的地址偏移
 * @param[in]  src 源地址
 * @param[in]  srcOffset 源地址偏移
 * @param[in]  size 字节数
 * @param[in]  dir 拷贝方向
 * @return lynError_t枚举值
 * @note 该接口只在arm32平台下可用
 */
lynError_t lynMemcpyAsyncArm32(lynStream_t stream, void *dst, uint64_t dstOffset, const void *src, uint64_t srcOffset, uint64_t size, lynMemcpyDir_t dir);


#ifdef __cplusplus
}
#endif

#endif // LYN_MEMORY_C_H
