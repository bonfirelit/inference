/**
 * @file lyn_ipe_ex.h
 * @brief image process engine c api
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
#ifndef LYN_IPE_EX_C_H
#define LYN_IPE_EX_C_H

#include "lyn_codec_com.h"
#include "lyn_stream.h"
#include "lyn_err.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * @brief run ipe process
 *
 * @param[in]  input input pic desc
 * @param[out]  output output pic desc
 * @param[in]  config config desc
 *
 * @return lynError_t
 */
lynError_t lynIpeProcessPhy(lynIpePicDesc_t input, lynIpePicDesc_t output, lynIpeConfigDesc_t config);


/**
 * @brief run ipe process
 *
 * @param[in]  input input pic desc
 * @param[out]  output output pic desc
 * @param[in]  config config desc
 *
 * @return lynError_t
 */
lynError_t lynIpeProcessPhyAsync(lynStream_t stream, lynIpePicDesc_t input, lynIpePicDesc_t output, lynIpeConfigDesc_t config);

#ifdef __cplusplus
}
#endif

#endif
