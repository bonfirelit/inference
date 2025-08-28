/**
 * @file lyn_stream.h
 * @brief stream模块的api说明
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

#ifndef LYN_STREAM_C_H
#define LYN_STREAM_C_H

#include "lyn_err.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @brief 流句柄
 *
 */
typedef void* lynStream_t;

/**
 * @brief 异步流下的用户回调函数
 *
 */
typedef lynError_t (*lynStreamCallback_t)(void *userData);

/**
 * @brief 用于错误消息处理的函数指针声明
 *
 * @note 异步指令执行出错后，调用用户注册的错误处理函数，一般用户会在回调函数中通知Stream线程，并停止Stream
 */
typedef void (*lynErrMsgHandler_t)(lynStream_t stream, lynErrorMsg_t *errorMsg, void *params);

/**
 * @brief 创建一个stream句柄
 *
 * @param  stream[out] stream句柄的地址
 * @return lynError_t枚举值
 *
 * @note 由调用者保证 *stream 为合法地址，否则系统会进入未定义状态
 */
lynError_t lynCreateStream(lynStream_t *stream);

/**
 * @brief 销毁指定的stream  
 *
 * @param  stream[in] stream句柄
 * @return lynError_t枚举值
 */
lynError_t lynDestroyStream(lynStream_t stream);

/**
 * @brief 同步stream，只到stream中的指令全部执行完毕才返回  
 *
 * @param  stream[in] stream句柄
 * @return lynError_t枚举值
 * 
 * @note 同步流会阻塞调用线程，直到流执行完毕
 */
lynError_t lynSynchronizeStream(lynStream_t stream);

/**
 * @brief 流插入异步回调函数，当流执行到插入位置会触发回调，该回调过程不会阻塞流
 * @param stream[in] 流句柄
 * @param cb[in] 回调函数指针
 * @param userData[in] 用户私有数据指针
 * @return lynError_t枚举值
 */
lynError_t lynStreamAddAsyncCallback(lynStream_t stream, lynStreamCallback_t cb, void *userData);
/**
 * @brief 流插入回调函数，当流执行到插入位置会触发回调，该回调过程阻塞流的执行
 * @param stream[in] 流句柄
 * @param cb[in] 回调函数指针
 * @param userData[in] 用户私有数据指针
 * @return lynError_t枚举值
 */
lynError_t lynStreamAddCallback(lynStream_t stream, lynStreamCallback_t cb, void *userData);

/**
 * @brief 注册错误信息处理函数    
 *
 * @param  handler[in] 回调函数指针，格式参见ErrMsgHandler声明
 * @param  params[in] 调用者传入的处理函数参数指针
 * @return lynError_t枚举值
 *
 * @note 由调用者保证 handler 和params 的合法地址，否则系统会进入未定义状态
 * @note 相同lynContext_t下多次调用，最后一次有效
 */
lynError_t lynRegisterErrorHandler(lynErrMsgHandler_t handler, void *params);

#ifdef __cplusplus
}
#endif

#endif // LYN_STREAM_C_H