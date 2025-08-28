/**
 * @file lyn_reduce.h
 * @brief //todo
 * \cond
 * @author SDK_TEAM
 * @version 0.1
 * @date 20224-08-11
 * 
 * Copyright:
 * © 2018 北京灵汐科技有限公司 版权所有。
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved.
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 * 
 * \endcond
 */
#ifndef LYN_REDUCE_C_H
#define LYN_REDUCE_C_H

#include "lyn_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 Reduce
 * 
 * @return lynError_t 
 */
lynError_t lynReduceInit();

/**
 * @brief 销毁 Reduce 运行时
 * 
 * @return lynError_t 
 */
lynError_t lynReduceUninit();

/**
 * @brief 执行模型 Reduce 对srcaddr1和srcaddr2进行reduce计算，结果存放在dstaddr，直到返回计算结果，异步接口
 * 
 * @param  stream[in] 需要执行计算的stream
 * @param  srcaddr1[in] 第一个输入
 * @param  srcaddr2[in] 第二个输入
 * @param  dstaddr[out] 输出，最终运算结果。
 * @param  size[in] 输入srcaddr1的字节数
 * @param  op[in] 数据计算类型
 * @param  type[in] 数据类型
 * @return lynError_t 
 */
lynError_t lynExecuteReduceAsync(lynStream_t stream, void *srcaddr1, void *srcaddr2, void *dstaddr, 
                                        uint64_t size, OperationType op, lynDataType_t type);

#ifdef __cplusplus
}
#endif

#endif // LYN_REDUCE_C_H