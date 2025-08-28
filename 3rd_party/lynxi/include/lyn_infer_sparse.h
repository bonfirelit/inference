/**
 * @file lyn_infer_sparse.h
 * @brief snn model inference c api
 * \cond
 * @author Lynxi SDK Team
 * @version v1.0
 * @date 2022-02-22
*
* Copyright:
* © 2018 北京灵汐科技有限公司 版权所有。\n
* 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。\n
* © 2018 Lynxi Technologies Co., Ltd. All rights reserved. \n
* NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 * \endcond
 */

#ifndef LYN_INFER_SPARSE_C_H
#define LYN_INFER_SPARSE_C_H

#include "lyn_err.h"
#include "lyn_stream.h"


#ifdef __cplusplus
extern "C" {
#endif

#define LYN_MAX_SPARSE_NAME_LEN (128)

typedef void* lynSparseModel_t;

/**
 * @brief 读取数据类型
 */
typedef enum lynSparseModelReadDataType_s{
    SPARSE_READ_SPIKE = 0,
    SPARSE_READ_MEMBRANE = 1,
    SPARSE_READ_MAX,
}lynSparseModelReadDataType_t;

/**
 * @brief snn模型类型
 */
typedef enum lynModelType_s{
    SNN_SMALL_SCHIP = 1,
    SNN_M_CHIP = 2,
    SNN_MAX,
}lynSparseModelType_t;

typedef struct lynSparseModelLoadPara_s{
    char* modelPath;
    lynSparseModelType_t modelType;
    char rsvd[LYN_MAX_SPARSE_NAME_LEN];
}lynSparseModelLoadPara_t;

typedef struct lynRunSpareModelPara_s{
    char rsvd[LYN_MAX_SPARSE_NAME_LEN];
}lynRunSpareModelPara_t;

typedef struct _lynSparseModelDesc_t{
    uint32_t timeStep;  // batchsize, once run can run timeStep's
    uint32_t runStepNum;  //limit of runstep
    uint32_t spikeDataSize;
    uint32_t membraneDataSize;

    uint64_t outputMembraneDdrAddr_addr;
    uint32_t outputMembraneDdrAddr_size;
    uint64_t outputSpikeDdrAddr_addr;
    uint32_t outputSpikeDdrAddr_size;
    uint32_t output_mode;
    uint32_t pause_mode;
    uint64_t sourceDCDataAddr;
    uint64_t sourcePossionDataAddr;
    uint32_t spikeData_size_nest;
    double   el_data;
} lynSparseModelDesc_t;

/**
 * @brief 加载模型上半部分
 *
 * @param  model[inout] 模型句柄
 * @param  loadPara[in] 获取硬件
 * @return lynError_t
 */
lynError_t lynLoadSpareModelTopHalf(lynSparseModel_t *model,lynSparseModelLoadPara_t loadPara);
/**
 * @brief 加载模型上半部分
 *
 * @param  model[inout] 模型句柄
 * @param  loadPara[in] 加载模型
 * @return lynError_t
 */
lynError_t lynLoadSpareModelBottomHalf(lynSparseModel_t model);
/**
 * @brief 加载模型
 *
 * @param  model[inout] 模型句柄
 * @param  loadPara[in] 加载参数
 * @return lynError_t
 */
lynError_t lynLoadSpareModel(lynSparseModel_t *model,lynSparseModelLoadPara_t loadPara);
lynError_t lynUnloadSparseModel(lynSparseModel_t model);

/**
 * @brief 从模型句柄获取模型描述文件
 *
 * @param  model[in] 模型句柄
 * @param  ppdesc[inout] lynSparseModelDesc_t指针的地址
 * @return lynError_t
 */
lynError_t lynGetSpareModelDesc(lynSparseModel_t model, lynSparseModelDesc_t** ppdesc);

/**
 * @brief 执行模型推理任务，异步接口
 * @param  stream[in] 需要执行推理的stream
 * @param  model[in] 需要执行推理的模型。调用lynLoadSpareModel*接口获取到
 * @return lynError_t
 */
lynError_t lynRunSpareModelAsync(lynStream_t stream, const lynSparseModel_t model, lynRunSpareModelPara_t para);

/**
 * @brief 获取当前模型已经跑完了多少时间步。
 * @param  stream[in] 需要执行推理的stream
 * @param  model[in] 模型句柄
 * @param  pTimestepDone[inout] 指针的地址
 * @return lynError_t
 */
lynError_t lynGetSpareModelDoneTimestepAsync(lynStream_t stream, lynSparseModel_t model, uint32_t* pTimestepDone);

/**
 * @brief 获取当前模型上一次运行了多少步。
 * @param  stream[in] 需要执行推理的stream
 * @param  model[in] 模型句柄
 * @param  pTimestepDone[inout] 指针的地址
 * @return lynError_t
 */
lynError_t lynGetSpareModelLatestTimestepAsync(lynStream_t stream, lynSparseModel_t model, uint32_t* pTimestepLatest);

/**
 * @brief 设定前模型跑多少次：需要在加载模型之后，运行模型之前调用
 *
 * @param  model[in] 模型句柄
 * @param  limit[inout] 当前模型能跑多少次
 * @return lynError_t
 */
lynError_t lynSetSparseModelRunStepNum(lynSparseModel_t model,uint64_t limit);

/**
 * @brief 设定前模型跑多少次：需要在加载模型之后，运行模型之前调用
 * @param  stream[in] 需要执行推理的stream
 * @param  model[in] 模型句柄
 * @param  stop[inout] stop时跑多少帧
 * @return lynError_t
 */
lynError_t lynSetSparseModelStopAsync(lynStream_t stream,lynSparseModel_t model,uint32_t stop);

/**
 * @brief Get the Mem Async from CR core
 *
 * @param  stream
 * @param  model
 * @param  outBufPtr
 * @param  count
 * @param  type
 * @return lynError_t
 */
lynError_t lynReadSparseModelResultAsync(lynStream_t stream, lynSparseModel_t model, void *outBufPtr, uint64_t count, lynSparseModelReadDataType_t type);

#ifdef __cplusplus
}
#endif

#endif