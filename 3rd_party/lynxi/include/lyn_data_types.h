/**
 * @file lyn_data_types.h
 * @brief 通用数据类型定义
 * \cond
 * @author Lynxi SDK Team
 * @version v1.0
 * @date 2023-04-24
 *
 * Copyright:
 * © 2018 北京灵汐科技有限公司 版权所有。\n
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。\n
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved. \n
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or
 * distributed without the permission of Lynxi Technologies Co., Ltd.
 * \endcond
 */

#ifndef __LYN_DATA_TYPES__
#define __LYN_DATA_TYPES__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 数据类型
 */
typedef enum {
    DT_UNDEFINED = 15, /**< 未定义的数据类型 */
    DT_FLOAT = 0,      /**< single float32 数据类型 */
    DT_FLOAT16 = 1,    /**< half float16 数据类型 */
    DT_INT8 = 2,       /**< signed char 数据类型 */
    DT_INT16 = 3,      /**< signed short 数据类型 */
    DT_UINT16 = 4,     /**< unsigned short 数据类型 */
    DT_UINT8 = 5,      /**< unsigned char 数据类型 */
    DT_INT32 = 6,      /**< signed int 数据类型 */
    DT_INT64 = 7,      /**< signed long long 数据类型 */
    DT_UINT32 = 8,     /**< unsigned int 数据类型 */
    DT_UINT64 = 9,     /**< unsigned long long 数据类型 */
    DT_BOOL = 10,      /**< bool 数据类型 */
    DT_DOUBLE = 11,    /**< double float 数据类型 */
    DT_BF16 = 12,      /**< bf16 数据类型 */
    DT_TF32 = 13,      /**< tf32 数据类型 */
    DT_INT4 = 14,      /**< int4 数据类型 */
} lynDataType_t;

typedef enum {
    OP_TYPE_NONE, /**< 无效的计算类型 */
    OP_TYPE_ADD,  /**< 求和计算 */
} OperationType;  // 支持的数据计算类型

/**
 * @brief 获取指定数据类型占用的内存大小（以字节为单位）
 *
 * @param[in] type 数据类型
 * @return unsigned int 占用内存的大小
 */
static inline unsigned int lynSizeof(lynDataType_t type)
{
    if (type >= DT_UNDEFINED) {
        return 0;
    }

    static const unsigned int lynDataTypeMap[] = {4, 2, 1, 2, 2, 1, 4, 8, 4, 8, 1, 8, 2, 4, 1};
    return lynDataTypeMap[type];
}

/**
 * @brief lynSizeof 函数的别名。获取指定数据类型占用的内存大小（以字节为单位）
 */
#define lyn_sizeof lynSizeof

#ifdef __cplusplus
}
#endif
#endif