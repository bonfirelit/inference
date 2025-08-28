/**
 * @file lyn_infer_node_p2p.h
 * @brief //todo
 * \cond
 * @author SDK_TEAM
 * @version 0.1
 * @date 2024-03-23
 * 
 * Copyright:
 * © 2018 北京灵汐科技有限公司 版权所有。
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved.
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 * 
 * \endcond
 */

#ifndef LYN_INFER_NODE_P2P_H
#define LYN_INFER_NODE_P2P_H

#include "lyn_infer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* lynNodeP2PModel_t;

typedef struct _lynNodeP2PModelRunAttr_t{
    uint32_t inputNum;
    uint32_t outputNum;
    void *inputList[LYN_MAX_P2P_MEMORY_COUNT];
    void *outputList[LYN_MAX_P2P_MEMORY_COUNT];
} lynNodeP2PModelRunAttr_t;

typedef struct _lynNodeP2PModelLoadAttr_t{
    const char *nodeConfigPath;
} lynNodeP2PModelLoadAttr_t;

lynError_t lynLoadNodeP2PModel(lynNodeP2PModel_t *model, lynNodeP2PModelLoadAttr_t loadParams);

lynError_t lynUnloadNodeP2PModel(lynNodeP2PModel_t model);

lynError_t lynNodeP2PModelGetDesc(lynNodeP2PModel_t model, lynModelDesc_t** ppdesc);

lynError_t lynExecuteNodeP2PModel(lynNodeP2PModel_t model, lynNodeP2PModelRunAttr_t runParams);


#ifdef __cplusplus
}
#endif

#endif