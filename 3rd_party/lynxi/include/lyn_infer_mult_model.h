/**
 * @file lyn_infer_mult_model.h
 * @brief //todo
 * \cond
 * @author SDK_TEAM
 * @version 0.1
 * @date 2023-06-20
 * 
 * Copyright:
 * © 2018 北京灵汐科技有限公司 版权所有。
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved.
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 * 
 * \endcond
 */

#ifndef LYN_INFER_MULT_MODEL_C_H
#define LYN_INFER_MULT_MODEL_C_H

#include "lyn_data_types.h"
#include "lyn_err.h"
#include "lyn_stream.h"
#include "lyn_infer.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* lynMultModel_t;

lynError_t lynLoadMultModel(const char* configPath, lynMultModel_t *model);

lynError_t lynExecuteMultModelAsync(lynStream_t stream, const lynMultModel_t model,
                                    void *input, void *output, uint32_t batchSize);

lynError_t lynWaitMultModel(const lynMultModel_t model);

lynError_t lynUnloadMultModel(lynMultModel_t model);

lynError_t lynMultModelGetDesc(lynMultModel_t model,lynModelDesc_t** ppdesc);

#ifdef __cplusplus
}
#endif

#endif
