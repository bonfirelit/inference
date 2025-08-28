/**
 * @file lyn_context.h
 * @brief context模块的api说明
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

#ifndef LYN_CONTEXT_C_H
#define LYN_CONTEXT_C_H

#include "lyn_err.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief context句柄
 */
typedef void* lynContext_t;

/**
 * @brief 通过指定的芯片ID创建一个context句柄
 * 
 * @param[out]  context  创建完成后的context句柄
 * @param[in]  deviceID  指定芯片ID号
 * @return lynError_t枚举值
 */
lynError_t lynCreateContext(lynContext_t *context, int32_t deviceID);

/**
 * @brief 销毁一个已经存在的context句柄
 * 
 * @param[in]  context 需要销毁的context句柄
 * @return lynError_t枚举值 
 */
lynError_t lynDestroyContext(lynContext_t context);

/**
 * @brief 指定当前线程的context句柄
 * 
 * @param[in]  context 需要指定的context句柄
 * @return lynError_t枚举值
 */
lynError_t lynSetCurrentContext(lynContext_t context);

/**
 * @brief 获取当前线程的context句柄
 * 
 * @param[out]  context 获取到的当前线程的context句柄指针
 * @return lynError_t枚举值 
 */
lynError_t lynGetCurrentContext(lynContext_t *context);

#ifdef __cplusplus
}
#endif

#endif // LYN_CONTEXT_C_H