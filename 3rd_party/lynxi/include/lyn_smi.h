/**
 * @file: lyn_smi.h
 * @author: hang.dong (hang.dong@lynxi.com)
 * @brief
 * @version: 1
 * @date 2022-06-08
 *
 * Copyright:
 * © 2018 北京灵汐科技有限公司 版权所有。
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved.
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 *
 * @par 修改日志:
 * Data:2022-06-08
 * Author: hang.dong
 * Description:
 */
#ifndef __LYN_SMI_H__
#define __LYN_SMI_H__

#include <stdint.h>

#define ARRAY_MAX_LEN 40
#define PROCESS_NAME_LEN 64
#define PROCESS_COUNT_LIMIT 16

#ifdef __cplusplus
extern "C" {
#endif

typedef int lynError_t;

/**
 * @brief device all parameters
 *
 */
typedef struct{
    char boardProductName[ARRAY_MAX_LEN];
    char boardBrand[ARRAY_MAX_LEN];
    char boardFirmwareVersion[ARRAY_MAX_LEN];
    char boardSerialNumber[ARRAY_MAX_LEN];
    uint32_t boardId;
    uint32_t boardChipCount;
    float boardPowerDraw;
    float boardPowerLimit;
    float boardVoltage;

    char deviceName[ARRAY_MAX_LEN];
    char deviceUuid[ARRAY_MAX_LEN];
    uint64_t deviceApuClockFrequency;
    uint64_t deviceApuClockFrequencyLimit;
    uint64_t deviceArmClockFrequency;
    uint64_t deviceArmClockFrequencyLimit;
    uint64_t deviceMemClockFrequency;
    uint64_t deviceMemClockFrequencyLimit;
    uint64_t deviceMemoryUsed;
    uint64_t deviceMemoryTotal;
    uint64_t deviceMmzTotalSize;
    int32_t deviceTemperatureCurrent;
    int32_t deviceTemperatureSlowdown;
    int32_t deviceTemperatureLimit;
    uint32_t deviceApuUsageRate;
    uint32_t deviceArmUsageRate;
    uint32_t deviceVicUsageRate;
    uint32_t deviceIpeUsageRate;
    uint32_t deviceEccStat;
    uint32_t deviceDdrErrorCount;
    uint32_t deviceDdrNoErrorCount;
    float deviceVoltage;
    uint32_t deviceApuVoltage;
    double pcieReadBandwidth;
    double pcieWriteBandwidth;
    double cpuDdrReadBandwidth;
    double cpuDdrWriteBandwidth;
    double apuDdrReadBandwidth;
    double apuDdrWriteBandwidth;
    float pcieReadBandwidthUsageRate;
    float pcieWriteBandwidthUsageRate;
    float apuDdrReadBandwidthUsageRate;
    float apuDdrWriteBandwidthUsageRate;

    uint32_t processCount;
    uint32_t pid[PROCESS_COUNT_LIMIT];
    char processName[PROCESS_COUNT_LIMIT][PROCESS_NAME_LEN];
    uint64_t processUseMemory[PROCESS_COUNT_LIMIT];
} lynDeviceProperties_t;

typedef enum{
    LowPowerMode = 0,
    StandardPowerMode
}lynPowerMode_t;

typedef enum{
    EP_MODE = 1,
    RC_MODE = 2,
}lynRunModeS_t;

typedef struct{
    char driverVersion[32];
}lynDriverVersionS_t;

/**
 * @brief P2P模式
 */
typedef enum{
    NonSupport_S = 0,
    P2P_LINK_PIX_S,    
    P2P_LINK_PXB_S,
    P2P_LINK_PHB_S,
    P2P_LINK_SYS_S,
}lynP2PModeS_t;

/**
 * @brief get device all properties
 *
 * @param[in]  devId
 * @param[out]  deviceProp
 * @return lynError_t
 */
lynError_t lynGetDeviceProperties(int32_t devId, lynDeviceProperties_t *deviceProp);

/**
 * @brief get device count
 *
 * @param[out]  deviceCount
 * @return lynError_t
 */
lynError_t lynGetDeviceCountSmi(int32_t *deviceCount);

/**
 * @brief get power mode
 *
 * @param[in]  devId
 * @param[out]  powerMode
 * @return lynError_t
 */
lynError_t lynGetPowerMode(int32_t devId, lynPowerMode_t* powerMode);

/**
 * @brief set power mode
 *
 * @param[in]  devId
 * @param[in]  powerMode
 * @return lynError_t
 */
lynError_t lynSetPowerMode(int32_t devId, lynPowerMode_t powerMode);

/**
 * @brief 获取运行模式
 *
 * @param[out]  runMode 运行模式
 * @return lynError_t
 */
lynError_t lynGetRunModeS(lynRunModeS_t *runMode);

/**
 * @brief 获取驱动版本
 *
 * @param[out]  driverVersion 驱动版本
 * @return lynError_t
 */
lynError_t lynGetDriverVersionS(lynDriverVersionS_t* driverVersion);

/**
 * @brief 获取两个device间的P2P的连接属性
 *
 * @param[in]  srcDevice 设备ID
 * @param[in]  dstDevice 设备ID
 * @param[out]  p2pMode P2P模式属性
 * @param[out]  dist switch跳数
 * @return lynError_t
 */
lynError_t lynGetDeviceP2PAttrS(int srcDevice, int dstDevice, lynP2PModeS_t* p2pMode, int* dist);

/**
 * @brief 显示当前节点的芯片拓扑结构
 *
 * @return lynError_t
 */
lynError_t lynDeviceShowTopologyS();

#ifdef __cplusplus
}
#endif

#endif
