/**
 * @file lyn_plugin.h
 * @brief plugin模块的api定义说明
 * \cond
 * @author Lynxi SDK Team
 * @version 1.0
 * @date 2022-05-19
 *
 * Copyright:
 * © 2018 北京灵汐科技有限公司 版权所有。
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved.
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 * \endcond
 */
#ifndef LYN_PLUGIN_C_H
#define LYN_PLUGIN_C_H
#include "lyn_stream.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 插件句柄
 * 
 */
typedef void* lynPlugin_t;

/**
 * @brief 用户plugin回调函数
 * 
 */
typedef lynError_t (*lynPluginCallback_t)(void *devData , void* userData);

/**
 * @brief 将编写好的插件程序注册到Server侧，Server端会读取动态库中的符号表并集成到程序中
 *
 * @param[out]  plugin 插件句柄
 * @param[in]  libName 插件库路径
 * @return lynError_t 枚举值
 */
lynError_t lynPluginRegister(lynPlugin_t *plugin, const char *libName);

/**
 * @brief 异步执行插件库中的函数
 *
 * @param[in]  stream 流句柄
 * @param[in]  plugin 插件句柄
 * @param[in]  funcName 需要运行的函数函数名
 * @param[in]  args 传递给func的参数函数参数
 * @param[in]  argSize 函数参数大小
 * @return lynError_t 枚举值
 */
lynError_t lynPluginRunAsync(lynStream_t stream, lynPlugin_t plugin, const char *funcName, void *args, uint32_t argSize);

/**
 * @brief 将插件库从程序中剥离，并删除Server侧插件库
 *
 * @param[in]  plugin 插件句柄
 * @return lynError_t 枚举值
 */
lynError_t lynPluginUnregister(lynPlugin_t plugin);

/**
 * @brief 
 * 
 * @param[in] plugin 插件句柄
 * @param[in] func 注册回调函数名
 * @param[in] userData 用户数据
 * @return lynError_t 枚举值
 */
lynError_t lynPluginRegisterCallback(lynPlugin_t plugin, lynPluginCallback_t func, void *userData);

/**
 * @brief 
 * 
 * @param[in] plugin 插件句柄
 * @param[in] func 注册回调函数名
 * @param[in] userData 用户数据
 * @return lynError_t 枚举值
 */
lynError_t lynPluginUnregisterCallback(lynPlugin_t plugin, lynPluginCallback_t func, void *userData);

#ifdef __cplusplus
}
#endif

#endif