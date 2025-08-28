/**
 * @file lyn_blas.h
 * @brief blas模块的api定义说明
 * \cond
 * @author SDK_TEAM
 * @version 0.1
 * @date 2022-11-11
 * 
 * Copyright:
 * © 2018 北京灵汐科技有限公司 版权所有。
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved.
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 * 
 * \endcond
 */
#ifndef LYN_BLAS_C_H
#define LYN_BLAS_C_H

#include "lyn_err.h"
#include "lyn_infer.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 矩阵是否转置
 * 
 */
typedef enum {
    LYN_NO_TRANS, /**< 非转置*/
    LYN_TRANS     /**< 转置*/
} lynTransType_t;

/**
 * @brief 矩阵n的取值
 * 
 */
typedef enum {
    LYN_DISTANCE_MODEL_NONE, /**< 无效值 */
    LYN_DISTANCE_MODEL_2048, /**< n = 2048*/
} lynDistanceModelType_t;


/**
 * @brief 初始化 GEMM 运行时
 * 
 * @return lynError_t 枚举值 
 */
lynError_t lynGemmInit();

/**
 * @brief 销毁 GEMM 运行时
 * 
 * @return lynError_t 枚举值
 */
lynError_t lynGemmUninit();

/**
 * @brief 执行模型 GEMM 计算(C = alpha * A * B + beta * C)，直到返回计算结果，异步接口
 * 
 * @param[in]  stream 需要执行计算的stream
 * @param[in]  transB 矩阵B 是否转置
 * @param[in]  m 矩阵A与C的行
 * @param[in]  n 矩阵A的列与矩阵B的行
 * @param[in]  k 矩阵B与C的列
 * @param[in]  alpha GEMM系数
 * @param[in]  beta GEMM系数
 * @param[in]  A 矩阵A
 * @param[in]  dataTypeA 矩阵A的数据类型
 * @param[in]  B 矩阵B
 * @param[in]  dataTypeB 矩阵B的数据类型
 * @param[in,out]  C 矩阵C，最终运算结果。
 * @param[in]  dataTypeC 矩阵C的数据类型
 * @return lynError_t 枚举值
 */
lynError_t lynExecuteGemmAsync(lynStream_t stream, lynTransType_t transB,
            uint32_t m, uint32_t n, uint32_t k, float alpha, float beta,
            void *A, lynDataType_t dataTypeA, void *B, lynDataType_t dataTypeB, void *C, lynDataType_t dataTypeC);


/**
 * @brief 初始化 topK 模型
 * 
 * @return lynError_t 枚举值
 */
lynError_t lynTopKInit();

/**
 * @brief 卸载 topK 模型
 * 
 * @return lynError_t 枚举值
 */
lynError_t lynTopKUninit();

/**
 * @brief 执行模型 topK 计算，异步接口
 * 
 * @param[in] stream 需要执行计算的 stream
 * @param[in] m 矩阵的行
 * @param[in] k 矩阵的列
 * @param[in] topK 选择前 topK 的数据
 * @param[in] in 矩阵所在的内存地址, device 端内存
 * @param[in] dataTypeIn 矩阵 in 的数据类型
 * @param[out] out topK 后输出的数据所在的地址
 * @param[in] dataTypeOut 矩阵 out 的数据类型, device 端内存
 * 
 * @return lynError_t 枚举值
 */
lynError_t lynExecuteTopKAsync(lynStream_t stream, int m, int k, int topK, 
                               void *in, lynDataType_t dataTypeIn, void *out, lynDataType_t dataTypeOut);



/**
 * @brief 初始化余弦相似度计算模型
 * 
 * @return lynError_t 枚举值
 */
lynError_t lynCosineInit();

/**
 * @brief 销毁余弦相似度计算模型
 * 
 * @return lynError_t 枚举值
 */
lynError_t lynCosineUninit();

/**
 * @brief 执行模型 余弦相似度模型 计算(A * B /(||A||)，对输出相似度从高到低进行排序，返回topk个相似度的值和对应索引)，异步接口
 * 
 * @param[in]  stream 需要执行计算的stream
 * @param[in]  transB 矩阵B 是否转置
 * @param[in]  m 矩阵A的行
 * @param[in]  k 矩阵B的列
 * @param[in]  topK 选择前 topK 的数据
 * @param[in]  A 矩阵A
 * @param[in]  dataTypeA 矩阵A的数据类型
 * @param[in]  B 矩阵B
 * @param[in]  dataTypeB 矩阵B的数据类型
 * @param[out]  C 矩阵C，最终运算结果 内存大小m* topK *(sizeof(fp16)+sizeof(float))。
 * @return lynError_t 枚举值
 */
lynError_t lynExecuteCosineAsync(lynStream_t stream, lynTransType_t transB,
                               uint32_t m, uint32_t k, uint32_t topK,
                               void *A, lynDataType_t dataTypeA, void *B, lynDataType_t dataTypeB, void *C);


/**
 * @brief 初始化相似度计算模型
 * 
 * @param[in]  type 矩阵的n取值类型
 * @return lynError_t 枚举值
 */
lynError_t lynDistanceInit(lynDistanceModelType_t type);

/**
 * @brief 销毁相似度计算模型
 * 
 * @param[in]  type 矩阵的n取值类型
 * @return lynError_t 枚举值
 */
lynError_t lynDistanceUninit(lynDistanceModelType_t type);

/**
 * @brief 执行模型 相似度计算，并对对输出相似度从高到低进行排序，返回topk个相似度的值和对应索引)，异步接口
 * 
 * @param[in]  stream 需要执行计算的stream
 * @param[in]  m 矩阵A的行
 * @param[in]  k 矩阵B的列
 * @param[in]  topK 选择前 topK 的数据
 * @param[in]  A 矩阵A
 * @param[in]  dataTypeA 矩阵A的数据类型
 * @param[in]  B 矩阵B
 * @param[in]  dataTypeB 矩阵B的数据类型
 * @param[out]  output 矩阵C，最终运算结果 内存大小m* topK *(sizeof(fp16)+sizeof(float))。
 * @return lynError_t 枚举值
 */

lynError_t lynExecuteCalcDistanceAsync(lynStream_t stream, uint32_t m, uint32_t k, uint32_t topK,
                               void *A, lynDataType_t dataTypeA, void *B, lynDataType_t dataTypeB, void *output);
                               

#ifdef __cplusplus
}
#endif

#endif // LYN_BLAS_C_H