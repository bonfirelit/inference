/**
 * @file lyn_device.h
 * @brief device模块的api定义说明
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

#ifndef LYN_DEVICE_C_H
#define LYN_DEVICE_C_H

#include "lyn_err.h"

#ifdef __cplusplus
extern "C" {
#endif
/*** @cond */
#define LYN_SDK_VERSION "1.22.0"
/*** @endcond*/
/**
 * @brief 当前设备运行模式
 */
typedef enum _lynRunMode_t{
    EP = 1, /**< EP模式，又称“卡模式”，指Server端设备作为EP，通过PCIe配合Client端进行工作 */
    RC = 2, /**< RC模式，又称“盒子模式”，指Server端设备上的CPU作为主控，用户应用程序运行在Server端的CPU上 */
} lynRunMode_t;

/**
 * @brief 主机上设备之间的PCIe Topo结构，即P2P拷贝两个设备之间的连接模式
 */
typedef enum _lynP2PMode_t{
    NonSupport = 0, /**< 不支持P2P */
    P2P_LINK_PIX, /**< 两个设备之间经过最多一个pcie switch */   
	P2P_LINK_PXB, /**< 两个设备之间经过了多个pcie switch，但不经过cpu */
    P2P_LINK_PHB, /**< 两个设备之间经过了cpu，但不跨numa */
    P2P_LINK_SYS, /**< 两个设备之间经过了不同numa */
} lynP2PMode_t;

/**
 * @brief 获取设备数量
 *
 * @param[out]  count 设备数量
 * @return lynError_t枚举值
 */
lynError_t lynGetDeviceCount(int32_t *count);

/**
 * @brief 指定运算设备
 *
 * @param[in]  deviceID 设备ID
 * @return lynError_t枚举值
 */
lynError_t lynSetDevice(int32_t deviceID);

/**
 * @brief 释放设备资源
 *
 * @param[in]  deviceID 设备ID
 * @return lynError_t枚举值
 */
lynError_t lynResetDevice(int32_t deviceID);

/**
 * @brief 获取软件版本号
 *
 * @param[out]  majorVersion 主版本号
 * @param[out]  minorVersion 从版本号
 * @param[out]  patchVersion 补丁版本号
 * @return lynError_t枚举值
 */
lynError_t lynGetSwVersion(int32_t *majorVersion, int32_t *minorVersion, int32_t *patchVersion);

/**
 * @brief 获取运行模式
 *
 * @param[out]  runMode 运行模式
 * @return lynError_t枚举值
 */
lynError_t lynGetRunMode(lynRunMode_t *runMode);

/**
 * @brief 获取两个设备之间的P2P连接属性
 *
 * @param[in]  srcDevice 设备ID
 * @param[in]  dstDevice 设备ID
 * @param[out]  p2pMode P2P模式属性
 * @param[out]  dist switch跳数或者距离
 * @return lynError_t枚举值
 */
lynError_t lynGetDeviceP2PAttr(int srcDevice, int dstDevice, lynP2PMode_t* p2pMode, int* dist);

/**
 * @brief 切换到指定设备上的context句柄, 如果当前设备上没有，则自动创建context
 * 
 * @param[in]  deviceID 设备ID
 * @return lynError_t 枚举值
 */
lynError_t lynSetCurrentDevice(int32_t deviceID);

/**
 * @brief 获取当前context的设备ID
 * 
 * @param[out]  deviceID 设备ID指针
 * @return lynError_t枚举值
 */
lynError_t lynGetCurrentDevice(int32_t *deviceID);

/**
 * @brief 显示当前节点的芯片拓扑结构
 *
 * @return lynError_t枚举值
 */
lynError_t lynDeviceShowTopology();

#ifdef __cplusplus
}
#endif

#endif // LYN_DEVICE_C_H
