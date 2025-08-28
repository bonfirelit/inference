/**
 * @file lyn_memory_ex.h
 * @brief memory模块的扩展api说明
 * \cond
 * @author Lynxi SDK Team
 * @version v1.0
 * @date 2022-04-28
*
* Copyright:
* © 2018 北京灵汐科技有限公司 版权所有。\n
* 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。\n
* © 2018 Lynxi Technologies Co., Ltd. All rights reserved. \n
* NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 * \endcond
 */

#ifndef LYN_MEMORY_EX_C_H
#define LYN_MEMORY_EX_C_H

#include "lyn_err.h"
#include "lyn_stream.h"
#include "lyn_context.h"
#include "lyn_memory.h"
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct lynDevPtrAttributes_t
 * @brief DevPtr内存的属性
 */
typedef struct {
    bool isDevPtr; /**< 是否是有效的DevPtr,即是否是通过lynMalloc接口申请的 */
    int deviceId; /**< DevPtr对应的设备id，即在哪个设备上申请的内存 */
    uint64_t physcalAddr; /**< DevPtr对应的物理地址 */
    void* virtualAddr; /**< DevPtr对应的虚拟地址 */
} lynDevPtrAttributes_t;

/**
 * @brief 地址转换
 *
 * @param[in]  devPtr 设备地址
 * @param[out]  physAddr 物理地址
 * @return lynError_t
 */
lynError_t lynDevPtr2PhysAddr(void *devPtr, uint64_t *physAddr);

/**
 * @brief 内存拷贝（异步模式）
 *
 * @param[in]  stream 流句柄
 * @param[in]  physAddr device物理地址
 * @param[in]  hostbuf client地址
 * @param[in]  size 字节数
 * @param[in]  dir 拷贝方向
 * @return lynError_t
 */
lynError_t lynMemcpyAsyncEx(lynStream_t stream, uint64_t physAddr, void *hostbuf, uint64_t size, lynMemcpyDir_t dir);

/**
 * @brief 内存拷贝（同步模式）
 *
 * @param[in]  physAddr device物理地址
 * @param[in]  hostbuf client地址
 * @param[in]  size 字节数
 * @param[in]  dir 拷贝方向
 * @return lynError_t
 */
lynError_t lynMemcpyEx(uint64_t physAddr, void *hostbuf, uint64_t size, lynMemcpyDir_t dir);

/**
 * @brief 设置DevPtr格式，设置true之后，支持申请超过1G内存，但是限制同时存在4096个buffer
 *
 * @param[in]  setFlag 设置标志，true:改变格式支持申请超过1G；false:默认就是false;
 * @return lynError_t
*/
lynError_t lynSetDevPtrFormatFlag(bool setFlag);

/**
 * @brief 地址转换, devPtr得到host映射的虚拟地址
 *
 * @param[in]  devPtr   设备地址
 * @param[out]  mapVirtAddr host映射虚拟地址
 * @return lynError_t
 */
lynError_t lynDevPtr2MapVirtAddr(void *devPtr, void **mapVirtAddr);

/**
 * @brief 内存拷贝
 *
 * @param[in]  stream 流句柄
 * @param[in]  dstDeviceId 目的设备id
 * @param[in]  dstPhysAddr 目的物理地址
 * @param[in]  srcDeviceId 源设备id
 * @param[in]  srcPhysAddr 源物理地址
 * @param[in]  size 拷贝字节数
 * @return lynError_t
 */
lynError_t lynMemcpyPhys2PhysAsync(lynStream_t stream, int dstDeviceId, uint64_t dstPhysAddr, int srcDeviceId, uint64_t srcPhysAddr, uint64_t size);

/**
 * @brief 内存拷贝,sysdma物理地址到物理地址
 *
 * @param[in]  dstPhysAddr 目的物理地址
 * @param[in]  srcPhysAddr 源物理地址
 * @param[in]  size 每一块的拷贝字节数
 * @param[in]  blocks 拷贝多少块
 * @param[in]  dstOffset dstPhysAddr到下一块拷贝地址的偏移
 * @param[in]  srcOffset srcPhysAddr到下一块拷贝地址的偏移
 * @return lynError_t
 */
lynError_t lynMemcpySysdmaBlocks(uint64_t dstPhysAddr, uint64_t srcPhysAddr, uint64_t size, uint64_t blocks, uint64_t dstOffset, uint64_t srcOffset);

/**
 * @brief 内存拷贝异步模式,sysdma物理地址到物理地址
 * 
 * @param[in]  stream 流句柄
 * @param[in]  dstPhysAddr 目的物理地址
 * @param[in]  srcPhysAddr 源物理地址
 * @param[in]  size 每一块的拷贝字节数
 * @param[in]  blocks 拷贝多少块
 * @param[in]  dstOffset dstPhysAddr到下一块拷贝地址的偏移
 * @param[in]  srcOffset srcPhysAddr到下一块拷贝地址的偏移
 * @return lynError_t
 */
lynError_t lynMemcpySysdmaBlocksAsync(lynStream_t stream, uint64_t dstPhysAddr, uint64_t srcPhysAddr, uint64_t size, uint64_t blocks, uint64_t dstOffset, uint64_t srcOffset);

/**
 * @brief 映射物理地址到主机侧能访问的虚拟地址
 * 
 * @param[in]  dev_id 芯片 id
 * @param[in]  size 映射的地址大小
 * @param[in]  offset 物理地址
 * @return void * 虚拟地址
 */
void *lynPmemMalloc(uint32_t dev_id, size_t size, off_t offset);

/**
 * @brief 映射物理地址到主机侧能访问的虚拟地址
 * 
 * @param[in]  addr 设备侧地址在主机上的虚拟地址
 * @param[in]  size 映射的地址大小
 * @return lynError_t
 */
lynError_t lynPmemFree(void *addr, size_t size);

/**
 * @brief 设置内存占用区域
 * 
 * @param[in]  physAddr 起始物理地址
 * @param[in]  size 占用大小
 * @return lynError_t
 */
lynError_t lynSetMemUsedZone(uint64_t physAddr, uint64_t size);

/**
 * @brief 清空内存占用区域
 * 
 * @param[in]  physAddr 起始物理地址
 * @param[in]  size 占用大小需要和SetMemUsedZone的size一致,成对使用
 * @return lynError_t
 */
lynError_t lynResetMemUsedZone(uint64_t physAddr, uint64_t size);

/**
 * @brief 同步接口：从多个Device指定地址汇聚数据到Host侧：dstData指针长度需要=arrayNum*phyAddrLen    把arrayNum个芯片的phyAddr数据的拷贝到host侧的dstData
 * 
 * @param[in]  streamArray 每个芯片的stream，可以为空：执行同步拷贝；可以非空：则对streamArray中的stream执行异步拷贝，最终在本接口内同步这个拷贝命令
 * @param[in]  deviceIdArray 芯片 id数组指针
 * @param[in]  arrayNum =deviceIdArray数组指针的个数=streamArray数组指针的个数
 * @param[in]  dstData 目的地址
 * @param[in]  phyAddr 物理地址
 * @param[in]  phyAddrLen 物理地址指针长度
 * @return lynError_t 
 */
lynError_t  lynMemoryGather(const lynStream_t*streamArray,const int32_t*deviceIdArray ,uint32_t arrayNum, void* dstData, uint64_t phyAddr, uint64_t phyAddrLen);

/**
 * @brief 同步接口：将一份数据从Host分发到多个Device的指定地址：srcData指针长度需要=phyAddrLen
 * 
 * @param[in]  streamArray 每个芯片的stream，可以为空：执行同步拷贝；可以非空：则对streamArray中的stream执行异步拷贝，最终在本接口内同步这个拷贝命令
 * @param[in]  deviceIdArray 芯片 id数组指针
 * @param[in]  arrayNum 芯片 id数组指针的个数
 * @param[in]  srcData 源地址
 * @param[in]  phyAddr 物理地址(目的地址)
 * @param[in]  phyAddrLen 物理地址指针长度
 * @return lynError_t 
 */
lynError_t  lynMemoryBroadcast(const lynStream_t*streamArray,const int32_t*deviceIdArray ,uint32_t arrayNum, const void* srcData, uint64_t phyAddr, uint64_t phyAddrLen);

/**
 * @brief 同步接口：将一份数据从Host切分之后，发到多个Device的指定地址：dstData指针长度需要=arrayNum*phyAddrLen
 * 
 * @param[in]  streamArray 每个芯片的stream，可以为空：执行同步拷贝；可以非空：则对streamArray中的stream执行异步拷贝，最终在本接口内同步这个拷贝命令
 * @param[in]  deviceIdArray 芯片 id数组指针
 * @param[in]  arrayNum 芯片 id数组指针的个数
 * @param[in]  srcData 源地址
 * @param[in]  phyAddr 物理地址(目的地址)
 * @param[in]  phyAddrLen 物理地址指针长度
 * @return lynError_t 
 */
lynError_t  lynMemoryScatter(const lynStream_t*streamArray,const int32_t*deviceIdArray ,uint32_t arrayNum, const void* srcData, uint64_t phyAddr, uint64_t phyAddrLen);

/**
 * @brief 获取devPtr内存属性
 * 
 * @param[out]  attributes 内存属性
 * @param[in]  ptr 设备内存devPtr
 * @return lynErrot_t
 */
lynError_t lynDevPtrGetAttributes(lynDevPtrAttributes_t* attributes, const void* ptr);

/**
 * @brief 申请host大页内存
 * 
 * @param[out]  hostPtr host侧内存指针
 * @param[in]  size 申请内存大小
 * @return lynErrot_t
 */
lynError_t lynHostMalloc(void** hostPtr, uint64_t size);

/**
 * @brief 释放host大页内存
 * 
 * @param[in]  hostPtr host侧内存指针
 * @return lynErrot_t
 */
lynError_t lynHostFree(void* hostPtr);
#ifdef __cplusplus
}
#endif

#endif // LYN_MEMORY_C_H
