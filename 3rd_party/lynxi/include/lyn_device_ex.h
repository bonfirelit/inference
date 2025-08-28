/**
 * @file lyn_device_ex.h
 * @brief device模块的扩展api说明
 * \cond
 * @author Lynxi SDK Team
 * @version v1.0
 * @date 2024-06-27
*
* Copyright:
* © 2018 北京灵汐科技有限公司 版权所有。\n
* 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。\n
* © 2018 Lynxi Technologies Co., Ltd. All rights reserved. \n
* NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 * \endcond
 */

#ifndef LYN_DEVICE_EX_C_H
#define LYN_DEVICE_EX_C_H

#include "lyn_err.h"
#include "lyn_event.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_NUMBER_ON_BOARD 64
#define LYN_COMMON_LEN 16

/**
 * @struct lynBoardDeviceList_t
 * @brief 板卡的设备Id列表
 */
typedef struct {
    int32_t deviceNumber; /**< 板卡上的设备数量 */
    int deviceList[MAX_NUMBER_ON_BOARD]; /**< 板卡上的设备列表 */
} lynBoardDeviceList_t;

/**
 * @struct lynDeviceProp_t
 * @brief 获取设备上指定的一些属性
 */
typedef struct {  
    char productName[LYN_COMMON_LEN]; /**< 设备的产品名 */
    char busId[LYN_COMMON_LEN]; /**< 设备的busID */
    uint64_t mmzTotalSize; /**< 设备上的mmz总共内存大小 */
    int coreNums; /**< 设备的APU core的总数量 */
    int apuClockFreq; /**< 设备的APU 时钟频率 */
    bool rdmaSupport; /**< 设备是否支持RDMA */   
} lynDeviceProp_t;

/**
 * @brief stream和event状态
 */
typedef enum{  
    lynSuccess,
    lynErrorNotReady,
    lynErrorInvaildValue,
} lynStatus_t;

/**
 * @brief 获取指定boardId上的device id列表
 * @param[in] boardID 板卡id
 * @param[out] devInfo 设备列表
 * @return lynError_t
*/
lynError_t lynGetDeviceListByBoardId(int32_t boardId, lynBoardDeviceList_t* devInfo);

/**
 * @brief 通过busId获取指定deviceId，busId 格式为 0000:67:00.0
 * @param[in] pciBusId pci总线id
 * @param[out] deviceId 设备id
 * @return lynError_t
*/
lynError_t lynDeviceGetByPCIBusId(int* deviceId, const char* pciBusId);

/**
 * @brief 通过deviceId获取busId
 * @param[in] deviceId 设备id
 * @param[in] len 字符串长度
 * @param[out] pciBusId pci总线id
 * @return lynError_t
*/
lynError_t lynDeviceGetPCIBusId(char* pciBusId, int len, int deviceId);

/**
 * @brief 通过deviceId获取设备属性
 * @param[in] deviceId 设备id
 * @param[out] prop 设备属性
 * @return lynError_t
*/
lynError_t lynGetDeviceProperity(lynDeviceProp_t* prop, int deviceId);

/**
 * @brief 同步当前device上所有stream
 * 
 * @return lynError_t
*/
lynError_t lynDeviceSynchronize();

/**
 * @brief 查询event状态
 * @param[in] event event句柄
 * @param[out] status event状态
 * @return lynError_t
*/
lynError_t lynEventQuery(lynStatus_t* status, lynEvent_t event);

/**
 * @brief 查询stream状态
 * @param[in] stream stream句柄
 * @param[out] status stream状态
 * @return lynError_t
*/
lynError_t lynStreamQuery(lynStatus_t* status, lynStream_t stream);

#ifdef __cplusplus
}
#endif

#endif // LYN_DEVICE_EX_C_H
