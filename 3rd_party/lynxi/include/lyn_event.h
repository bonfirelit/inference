/**
 * @file lyn_event.h
 * @brief event模块的api定义说明
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

#ifndef LYN_EVENT_C_H
#define LYN_EVENT_C_H

#include "lyn_err.h"
#include "lyn_stream.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 事件句柄
 */
typedef void* lynEvent_t;

/**
 * @brief 创建事件，同步接口，创建的Event主要用于不同Stream之间的同步。
 * @param[out] event 事件句柄
 * @return lynError_t枚举值
 */
lynError_t lynCreateEvent(lynEvent_t *event);

/**
 * @brief 销毁事件
 * @param[in] event 事件句柄，同步接口，只能销毁用lynCreateEvent创建的Event。
 * @return lynError_t枚举值
 * @note 销毁已经创建的Event，必须确保等待lynSynchronizeStream接口或者lynStreamWaitEvent接口涉及
的任务都结束后才能销毁。
 */
lynError_t lynDestroyEvent(lynEvent_t event);

/**
 * @brief 插入事件到流，流执行到事件插入位置会触发事件
 * @param[in] stream 流句柄
 * @param[in] event 事件句柄
 * @return lynError_t枚举值
 * @note 注意不要将同一个event插入到多个stream，多stream是并行执行的，无法控制唤醒顺序
 *       除非等待者不关心唤醒顺序
 */
lynError_t lynRecordEvent(lynStream_t stream, lynEvent_t event);

/**
 * @brief 流等待事件，流执行到事件插入的位置会被阻塞，直到事件被触发后，流才能继续执行后续的操作.同步接口。支持多个Stream等待同
一个Event。
 * @param[in] stream 流句柄
 * @param[in] event 事件句柄
 * @return lynError_t枚举值
 */
lynError_t lynStreamWaitEvent(lynStream_t stream, lynEvent_t event);

/**
 * @brief 等待事件，调用线程会被阻塞直到事件被触发
 * @param[in] event 事件句柄
 * @return lynError_t枚举值
 */
lynError_t lynSynchronizeEvent(lynEvent_t event);

/**
 * @brief 统计两个事件触发的时间差，必须要调用同步接口等到事件触发后才能使用
 * @param[in] start 开始的事件
 * @param[in] end 结束的事件
 * @param[out] ms 时间差，单位毫秒
 * @return lynError_t枚举值
 * @note 调用lynEventElapsedTime接口必须调用lynSynchronizeStream阻塞当前线程，直到目标Stream中
的所有任务都结束。
 */
lynError_t lynEventElapsedTime(lynEvent_t start, lynEvent_t end, float *ms);

#ifdef __cplusplus
}
#endif

#endif // LYN_EVENT_C_H