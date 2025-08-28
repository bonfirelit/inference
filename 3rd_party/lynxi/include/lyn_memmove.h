/**
 * @file lyn_memmove.h
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
#ifndef LYN_MEMMOVE_C_H
#define LYN_MEMMOVE_C_H

#include "lyn_err.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 Memmove
 * 
 * @return lynError_t 
 */
lynError_t lynMemmoveInit();

/**
 * @brief 销毁 Memmove 运行时
 * 
 * @return lynError_t 
 */
lynError_t lynMemmoveUninit();

/**
 * @brief 执行模型 memmove 将srcaddr的数据size大小移动到dstaddr，异步接口
 * 
 * @param  stream[in] 需要执行计算的stream
 * @param  srcaddr[in] 输入地址
 * @param  dstaddr[out] 输出地址
 * @param  size[in] 拷贝的字节数
 * @return lynError_t 
 */
lynError_t 	lynMemmoveAsync(lynStream_t stream, void *srcaddr, void *dstaddr, 
                                        uint64_t size);


#ifdef __cplusplus
}
#endif

#endif // LYN_MEMMOVE_C_H