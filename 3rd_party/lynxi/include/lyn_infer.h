/**
 * @file lyn_infer.h
 * @brief 模型推理模块的api定义说明
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

#ifndef LYN_INFER_C_H
#define LYN_INFER_C_H

#include "lyn_data_types.h"
#include "lyn_err.h"
#include "lyn_stream.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 模型句柄
 */
typedef void* lynModel_t;

/**
 * @brief 模型描述句柄
 */
typedef void* lynModelDescHandle_t;

/**
 * @brief 模型支持的最大张量维度
 */
#define LYN_MAX_DIMS_COUNT (16)

/**
 * @brief 模型支持的最大张量名长度
 */
#define LYN_MAX_TENSOR_NAME_LEN (128)

/**
 * @brief 模型支持的最多点对点内存数量
 */
#define LYN_MAX_P2P_MEMORY_COUNT (4)

#define LYN_MAX_HW_COUNT (128)

/**
 * @brief 张量属性结构体
 * @private
 */
typedef struct {
    uint32_t batchSize;       /**<一批样本的数量*/
    uint32_t dimCount;        /**<维度的数量*/
    uint32_t dataNum;         /**<数据个数*/
    uint64_t dataLen;         /**<数据在内存中占据的大小（字节）*/
    lynDataType_t dtype;   /**<数据类型*/
    uint32_t dims[LYN_MAX_DIMS_COUNT];  /**<维度数组*/
    char tensorName[LYN_MAX_TENSOR_NAME_LEN];  /**<张量名数组*/
} lynModelTensorAttr_t;


/**
 * @brief 模型描述结构体
 * @private
 */
typedef struct {
    uint64_t inputDataLen;  /**<一个样本输入数据长度，所有输入张量大小的和*/
    uint64_t outputDataLen; /**<一个样本输出数据长度，所有输出张量大小的和*/
    uint32_t inputTensorAttrArrayNum; /**<输入张量数组长度*/
    uint32_t outputTensorAttrArrayNum; /**<输出张量数组长度*/
    const lynModelTensorAttr_t* inputTensorAttrArray; /**<输入张量数组指针*/
    const lynModelTensorAttr_t* outputTensorAttrArray; /**<输出张量数组指针*/
} lynModelDesc_t;

/**
 * @brief 模型描述动态分辨率结构体
 * 
 */
typedef struct {
    uint32_t hwCount; /**模型中支持的分辨率档数*/
    uint64_t hw[LYN_MAX_HW_COUNT][2]; /**每组分档中，数组下标为0代表的是高，下标为1代表的是宽*/
} lynModelHWDesc_t;

/** @cond */
/**
 * @brief 模型任务结构体,模型会有一个或者多个任务，每个任务通过本结构体来描述
 * @private
 */
typedef struct lynModelTaskDesc_t{
    uint32_t deployLibSize;  /**<推理动态库大小*/
    uint32_t coreBinSize;  /**<推理核心文件大小*/
    uint32_t datBinSize;  /**<推理权重文件大小*/
    uint32_t cmdBinSize;  /**<推理命令文件大小*/
    uint64_t coreBinPhyAddr; /**<推理核心文件在内存中的物理地址*/
    uint64_t datBinPhyAddr; /**<推理权重文件在内存中的物理地址*/
    uint64_t cmdBinPhyAddr; /**<推理命令文件在内存中的物理地址*/

    uint64_t inputDataLen; /**<一个样本输入数据长度，所有输入张量大小的和*/
    uint64_t outputDataLen; /**<一个样本输出数据长度，所有输入张量大小的和*/
    uint32_t inputTensorAttrArrayNum; /**<输入张量数组长度*/
    uint32_t outputTensorAttrArrayNum; /**<输出张量数组长度*/
    lynModelTensorAttr_t* inputTensorAttrArray; /**<输入张量数组指针*/
    lynModelTensorAttr_t* outputTensorAttrArray; /**<输出张量数组指针*/
    char modelType[LYN_MAX_TENSOR_NAME_LEN];   /**<模型类型的字符串*/
}lynModelTaskDesc_t ;
/** @endcond */

/**
 * @brief 模型命令枚举结构体
 * @private
 */
typedef enum  {
	INFER_CMD_IDLE = 0,  /**<模型命令空*/
	INFER_CMD_LOAD,   /**<模型命令加载*/
	INFER_CMD_START, /**<模型命令运行*/
	INFER_CMD_LSTART, /**<模型命令加载并运行*/
	INFER_CMD_MAX, /**<模型命令结束*/
}lynModelCmd_e;

/**
 * @brief 模型运行时间结构体，用来记录模型命令类型以及运行的时间
 * @private
 */
typedef struct {
    lynModelCmd_e   cmdType; /**<模型命令类型枚举*/
    uint32_t        time; /**<模型命令运行时长*/
} lynModelCmdTimeDesc_t;

/**
 * @brief 模型张量描述和张量数据结构体
 * @private
 */
typedef struct 
{
    lynModelTensorAttr_t tensorAttr; /**<张量属性结构体*/
    void *tensorData;             /**<张量数据指针*/
} lynTensor_t;

/**
 * @brief 获取模型一个batch的输入数据在内存中的总长度
 *
 * @param[in]  model 模型句柄
 * @param[out]  totalDataLen 一个batch的模型输入数据在内存中的总长度
 * @return lynError_t枚举值
 */
lynError_t lynModelGetInputDataTotalLen(lynModel_t model, uint64_t *totalDataLen);

/**
 * @brief 获取模型一个batch的输出数据在内存中的总长度
 *
 * @param[in]  model 模型句柄
 * @param[out]  totalDataLen 一个batch的模型输出数据在内存中的总长度
 * @return lynError_t枚举值
 */
lynError_t lynModelGetOutputDataTotalLen(lynModel_t model, uint64_t *totalDataLen);

/**
 * @brief 获取指定index的输入tensor数据在内存中的长度
 *
 * @param[in]  model 模型句柄
 * @param[in]  index tensor序号
 * @param[out]  dataLen 获取指定index的输入tensor数据在内存中的总长度
 * @return lynError_t枚举值
 */
lynError_t lynModelGetInputTensorDataLenByIndex(lynModel_t model, uint32_t index,uint64_t *dataLen);

/**
 * @brief 获取指定index的输入tensor数据在内存中的长度
 *
 * @param[in]  model 模型句柄
 * @param[in]  index tensor序号
 * @param[out]  dataLen 获取指定index的输入tensor数据在内存中的总长度
 * @return lynError_t枚举值
 */
lynError_t lynModelGetOutputTensorDataLenByIndex(lynModel_t model, uint32_t index,uint64_t *dataLen);

/**
 * @brief 获取指定index的输入tensor数据的dimension信息，包括dimension数组长度以及dimension数组
 *
 * @param[in]  model 模型句柄
 * @param[in]  index tensor序号
 * @param[out]  dims dimension数组 用户需保证dims指针长度为sizeof(uint32_t) * LYN_MAX_DIMS_COUNT
 * @param[out]  dimsCount dimension数组元素个数
 * @return lynError_t枚举值
 */
lynError_t lynModelGetInputTensorDimsByIndex(lynModel_t model, uint32_t index,uint32_t *dims,uint32_t *dimsCount);

/**
 * @brief 获取指定index的输出tensor数据的dimension信息，包括dimension数组长度以及dimension数组，用户需保证dims指针长度为sizeof(uint32_t) * LYN_MAX_DIMS_COUNT
 *
 * @param[in]  model 模型句柄
 * @param[in]  index tensor序号
 * @param[out]  dims dimension数组
 * @param[out]  dimsCount dimension数组元素个数
 * @return lynError_t枚举值
 */
lynError_t lynModelGetOutputTensorDimsByIndex(lynModel_t model, uint32_t index,uint32_t *dims,uint32_t *dimsCount);

/**
 * @brief 获取指定index的输入tensor的名称
 *
 * @param[in]  model 模型句柄
 * @param[in]  index tensor序号
 * @param[in,out]   name tensor的名称 用户需保证name指针长度不小于LYN_MAX_TENSOR_NAME_LEN
 * @return lynError_t枚举值
 */
lynError_t lynModelGetInputTensorNameByIndex(lynModel_t model, uint32_t index,char* name);

/**
 * @brief 获取指定index的输出tensor的名称
 *
 * @param[in]  model 模型句柄
 * @param[in]  index[in] tensor序号
 * @param[in,out]  name  tensor的名称 用户需保证name指针长度不小于LYN_MAX_TENSOR_NAME_LEN
 * @return lynError_t枚举值
 */
lynError_t lynModelGetOutputTensorNameByIndex(lynModel_t model, uint32_t index,char* name);

/**
 * @brief 获取指定index的输入tensor数据类型
 *
 * @param[in]  model 模型句柄
 * @param[in]  index tensor序号
 * @param[out]  dataTpye tensor的数据类型
 * @return lynError_t枚举值
 */
lynError_t lynModelGetInputTensorDataTypeByIndex(lynModel_t model, uint32_t index,lynDataType_t* dataTpye);

/**
 * @brief 获取指定index的输出tensor数据类型
 *
 * @param[in]  model 模型句柄
 * @param[in]  index tensor序号
 * @param[out]  dataTpye tensor的数据类型
 * @return lynError_t枚举值
 */
lynError_t lynModelGetOutputTensorDataTypeByIndex(lynModel_t model, uint32_t index,lynDataType_t* dataTpye);

/**
 * @brief 获取指定index的输入tensor数据个数
 *
 * @param[in]  model 模型句柄
 * @param[in]  index tensor序号
 * @param[out]  dataTpye tensor的数据个数
 * @return lynError_t枚举值
 */
lynError_t lynModelGetInputTensorDataNumByIndex(lynModel_t model, uint32_t index,uint32_t *dataNum);

/**
 * @brief 获取指定index的输出tensor数据个数
 *
 * @param[in]  model 模型句柄
 * @param[in]  index tensor序号
 * @param[out]  dataTpye tensor的数据个数
 * @return lynError_t枚举值
 */
lynError_t lynModelGetOutputTensorDataNumByIndex(lynModel_t model, uint32_t index,uint32_t *dataNum);

/**
 * @brief 获取输入tensor数量
 *
 * @param[in] model 模型句柄
 * @param[out]  tensorNum tensor数量
 * @return lynError_t枚举值
 */
lynError_t lynModelGetInputTensorNum(lynModel_t model, uint32_t* tensorNum);

/**
 * @brief 获取输出tensor数量
 *
 * @param[in]  model 模型句柄
 * @param[out]  tensorNum tensor数量
 * @return lynError_t枚举值
 */
lynError_t lynModelGetOutputTensorNum(lynModel_t model, uint32_t* tensorNum);

/**
 * @brief 获取模型支持的动态分辨率信息
 * 
 * @param[in] model 模型句柄
 * @param[out] hwDesc 描述动态分辨率结构体
 * @return lynError_t 枚举值
 * @note 获取的hwDesc中hwCount等于0时，表示不支持设置档位信息
 */
lynError_t lynModelGetDynamicHWDesc(lynModel_t model, lynModelHWDesc_t** hwDesc);

/**
 * @brief 从文件加载模型
 *
 * @param[in]  modelPath 模型路径
 * @param[out]  model 模型句柄
 * @return lynError_t枚举值
 */
lynError_t lynLoadModel(const char* modelPath, lynModel_t *model);

/**
 * @brief 从内存加载模型
 *
 * @param[in] mem 模型(MDL文件) 在内存中的起始地址
 * @param[in] size 数据大小
 * @param[out] model 模型句柄
 * @return lynError_t枚举值
 */
lynError_t lynLoadModelFromMem(void *mem, size_t size, lynModel_t *model);

/**
 * @brief 从模型句柄获取模型描述文件
 *
 * @param[in]  model 模型句柄
 * @param[in,out]  ppdesc lynModelDesc_t指针的地址
 * @return lynError_t枚举值
 */
lynError_t lynModelGetDesc(lynModel_t model, lynModelDesc_t** ppdesc);

/** @cond */
/**
 * @brief 从模型句柄获取模型task描述文件
 *
 * @param[in]  model 模型句柄
 * @param[in,out]  modelTaskNum lynModelTaskDesc_t指针长度
 * @param[in,out]  ppdesc lynModelTaskDesc_t指针的地址
 * @return lynError_t枚举值
 */
lynError_t lynModelGetTaskDesc(lynModel_t inferModel,uint32_t* modelTaskNum, lynModelTaskDesc_t** ppdesc);
/**
 * @brief 从模型获取loadnn命令和startnn命令的时间
 *
 * @param[in]  model 模型句柄
 * @param[in,out]  cmdTimeNum 获取cmd时间的个数
 * @param[in,out]  ppdesc lynModelTaskDesc_t指针的地址
 * @return lynError_t枚举值
 */
lynError_t lynModelGetTaskCmdTime(lynModel_t inferModel,uint32_t taskIndex , uint32_t cmdTimeNum, lynModelCmdTimeDesc_t* cmdTimeArray);
/** @endcond */

/**
 * @brief 卸载模型
 *
 * @param[in]  model 模型句柄
 * @return lynError_t枚举值
 */
lynError_t lynUnloadModel(lynModel_t model);
/**
 * @brief 获取模型推理时动态batch设定的基准大小
 *
 * @param[in] model 模型句柄
 * @param[out] batchBase batch设定的基准大小
 * @return lynError_t枚举值
 * @note 模型推理时设置的动态batch大小必须是batchBase的整数倍大小，否则设定会失败
 */
lynError_t lynModelGetBatchBase(const lynModel_t model, uint32_t *batchBase);

/**
 * @brief 执行模型推理，直到返回推理结果，异步接口
 * @note 对同一个model的模型，不能调用lynExecuteModelAsync接口执行多Stream并发场景下的模型推理。
 * @param[in]  stream 需要执行推理的stream
 * @param[in]  model 需要执行推理的模型。调用lynLoadModel*接口获取到
 * @param[in]  devPtrIn 需要执行推理的输入数据，调用lynMalloc申请，内存大小为lynModelDesc_t:inputDataLen*batch
 * @param[in,out]  devPtrOut 需要执行推理的输出数据，调用lynMalloc申请，内存大小为lynModelDesc_t:outputDataLen*batch
 * @param[in]  batchSize 输入图片张数
 * @return lynError_t枚举值
 */
lynError_t lynExecuteModelAsync(lynStream_t stream, const lynModel_t model, void *devPtrIn, void *devPtrOut, uint32_t batchSize);

/**
 * @brief 模型推理的异步模式, 发送待推理数据,只支持推理纯apu的模型，对于含有cpu的混合模型，使用lynExecuteModelAsync接口
 *
 * @param[in]  stream 发送待推理数据的stream
 * @param[in]  model 需要执行推理的模型。调用lynLoadModel*接口获取到
 * @param[in]  devPtrIn 需要执行推理的输入数据，调用lynMalloc申请，内存大小为lynModelDesc_t:inputDataLen*batch
 * @param[in]  devPtrOut 需要执行推理的输出数据，调用lynMalloc申请，内存大小为lynModelDesc_t:outputDataLen*batch
 * @param[in]  batch 输入数据batch数
 * @return lynError_t枚举值
 */
lynError_t lynModelSendInputAsync(lynStream_t stream, const lynModel_t model, void *devPtrIn, void *devPtrOut, uint32_t batch);

/**
 * @brief 执行模型推理，直到返回推理结果，支持多个输入输出，异步接口
 * 
 * @param[in] stream  发送待推理数据的stream
 * @param[in] model  需要执行推理的模型。调用lynLoadModel*接口获取到
 * @param[in] inputList 需要执行推理的输入数据指针数组
 * @param[in] inputNum  需要执行推理的输入数据指针个数
 * @param[in,out] outputList 需要执行推理的输出数据指针数组
 * @param[in] outputNum  需要执行推理的输出数据指针个数
 * @param[in] batchSize  输入数据batch数
 * @return lynError_t 枚举值
 */
lynError_t lynExecuteModelAsyncv2(lynStream_t stream, const lynModel_t model, void **inputList,
                                  uint32_t inputNum, void **outputList, uint32_t outputNum,
                                  uint32_t batchSize);

/**
 * @brief 执行模型推理，支持多个输入输出，异步接口
 * 
 * @param[in] stream  发送待推理数据的stream
 * @param[in] model  需要执行推理的模型。调用lynLoadModel*接口获取到
 * @param[in] inputList 需要执行推理的输入数据指针数组
 * @param[in] inputNum  需要执行推理的输入数据指针个数
 * @param[in,out] outputList 需要执行推理的输出数据指针数组
 * @param[in] outputNum  需要执行推理的输出数据指针个数
 * @param[in] batchSize  输入数据batch数
 * @return lynError_t 枚举值
 */
lynError_t lynModelSendInputAsyncv2(lynStream_t stream, const lynModel_t model, void **inputList,
                                    uint32_t inputNum, void **outputList, uint32_t outputNum,
                                    uint32_t batch);

/**
 * @brief 模型推理的异步模式, 接收推理完之后的数据，跟lynModelSendInputAsync配对使用
 *
 * @param[in]  stream 接收推理结果的stream
 * @param[in]  model 需要执行推理的模型。调用lynLoadModel*接口获取到
 * @param[in]  devPtrOut 需要执行推理的输出数据，调用lynMalloc申请，内存大小为lynModelDesc_t:outputDataLen*batch
 * @param[in]  batch 输出数据batch数
 * @return lynError_t枚举值
 */
lynError_t lynModelRecvOutputAsync(lynStream_t stream, const lynModel_t model);


/**
 * @brief 准备推理输入数据：将arrayBuf指针的内容复制到intPtr
 *
 * @param[in] modelDesc 模型描述结构体指针
 * @param[in] arrayBuf 源地址数组：多个输入张量的指针数组
 * @param[in,out] intPtr 目的地址
 * @return lynError_t枚举值
 * @deprecated  该函数已经废弃，请使用多输入多输出模型及推理接口支持
 */

lynError_t lynModelPrepareInputBuf(const lynModelDesc_t *modelDesc, void *arrayBuf[], void *intPtr);

/**
 * @brief 收集推理输出数据：将arrayTensor指针的内容复制到outPtr
 *
 * @param[in] modelDesc 模型描述结构体指针
 * @param[in] outPtr 目的地址
 * @param[inout] arrayTenSor 源地址数组：多个输出张量的指针数组
 * @return lynError_t枚举值
 * @deprecated  该函数已经废弃，请使用多输入多输出模型及推理接口支持
 */

lynError_t lynModelParseOutBuf(const lynModelDesc_t *modelDesc, void *outPtr, lynTensor_t *arrayTensor[]);

/**
 * @brief 动态分辨率场景下，设置模型推理时输入图片的高和宽
 * 此处设置的分辨率只能是模型编译过程中设置的多个分辨率档位中的某一档
 *
 * @param[in] stream 指定stream句柄
 * @param[in] model 模型句柄
 * @param[in] height 图像高度
 * @param[in] width 图像宽度
 * @return lynError_t枚举值
 */
lynError_t lynModelSetDynamicHWSizeAsync(lynStream_t stream, const lynModel_t model, uint32_t height, uint32_t width);

/**
 * @brief 设置模型推理时动态batch的尺寸
 *
 * @param[in] stream 指定stream句柄
 * @param[in] model 模型句柄
 * @param[in] batchsize batch尺寸
 * @return lynError_t枚举值
 * @note batchsize的值必须是模型batchBase的整数倍，否则该接口在执行时会报错
 */
lynError_t lynModelSetDynamicBatchSizeAsync(lynStream_t stream, const lynModel_t model, uint32_t batchSize);

/**
 * @brief
 *
 * @param[in] stream 指定stream句柄
 * @param[in] model 模型句柄
 * @return lynError_t枚举值
 */
lynError_t lynModelResetAsync(lynStream_t stream, const lynModel_t model);

/** 
 * @brief DP(data parallel)模型句柄
 */
typedef void* lynDPModel_t;

/**
 * @brief 从文件中加载模型
 * 
 * @param[out] model DP模型句柄
 * @param[in] modelPath 模型路径
 * @param[in] deviceList DeviceID数组
 * @param[in] devicesNum DeviceID数组长度
 * @param[in] lyndpBatch 设置的lyndpBatch
 * @return lynError_t 枚举值
 * @note 指明模型需要在哪些芯片编号上面同时数据并行推理
 */
lynError_t lynDPLoadModel(lynDPModel_t *model, const char *modelPath, int32_t *deviceList,
                          int32_t devicesNum, int32_t lyndpBatch);

/**
 * @brief 从文件中加载模型
 * 
 * @param[in] model 模型句柄
 * @param[in] modelPath 模型路径
 * @param[in] boardID 指定的板卡ID
 * @param[in] lyndpBatch 设置的lyndpBatch
 * @return lynError_t 枚举值
 * @note 指明模型需要在哪个板卡上面同时数据并行推理
 */
lynError_t lynDPLoadModelWithBoard(lynDPModel_t *model, const char *modelPath, int32_t boardID,
                          int32_t lyndpBatch);

/**
 * @brief 卸载DP模型
 * 
 * @param[in] model 模型句柄
 * @return lynError_t 
 */
lynError_t lynDPUnloadModel(lynDPModel_t model);
 
/**
*   @brief 创建DPFuture句柄
*   @note DPFuture句柄用于获取DP模型异步推理接口中的执行状态
*/
typedef void* lynDPFuture_t;

/**
 * @brief DP异步接口下的用户回调函数
 *
 */
typedef lynError_t (*lynDPCallback_t)(void *userData);

/**
 * @brief DP模型功能信息结构体
 * @private
 */
typedef struct {
    uint32_t batch;  /**<一批样本的大小*/
    void *hostPtrIn;  /**<用户侧输入数据指针*/
    void *hostPtrOut; /**<用户侧输出数据指针*/
    double beginTime; /**<开始时间*/
    double endTime; /**<结束时间*/
}lynDPFutureInfo_t;

/**
 * @brief 创建DPFuture句柄
 * 
 * @param[out] future DPFuture句柄
 * @return lynError_t枚举值
 */
lynError_t lynCreateDPFuture(lynDPFuture_t *future);

/**
 * @brief 销毁DPFuture
 * 
 * @param[in] future DPFuture句柄
 * @return lynError_t 枚举值
 */
lynError_t lynDestroyDPFuture(lynDPFuture_t future);

/**
 * @brief 获取lynDPExecuteModelAsync异步推理接口的结果
 * 
 * @param[in] future DPFuture句柄
 * @param[out] futureInfo DPFuture信息
 * @return lynError_t 枚举值
 */
lynError_t lynDPFutureGetResult(lynDPFuture_t future, lynDPFutureInfo_t *futureInfo);

/**
 * @brief DP模型执行推理，异步接口
 * 
 * @param[in] model 模型句柄
 * @param[in] batchSize 设置的inputBatch数
 * @param[in] hostPtrIn 需要执行推理的输入数据。Host内存
 * @param[out] hostPtrOut 需要执行推理的输出数据。Host内存
 * @param[in] future DPFuture句柄
 * @param[in] recordApuTime 是否记录apu推理时间
 * @param[in] cb 回调函数指针，在推理结束时执行，不会阻塞推理，当设置为空指针时，不执行回调函数
 * @param[in] userData 用户私有数据指针
 * @return lynError_t  枚举值
 */
lynError_t lynDPExecuteModelAsync(const lynDPModel_t model, uint32_t batchSize, void *hostPtrIn,
                                  void *hostPtrOut, lynDPFuture_t future, bool recordApuTime,
                                  lynDPCallback_t cb, void *userData);

/**
 * @brief 执行模型推理，直到返回推理结果 同步接口
 * 
 * @param[in] model 模型句柄
 * @param[in] batchSize 设置的inputBatch数
 * @param[in] hostPtrIn 需要执行推理的输入数据。Host内存
 * @param[out] hostPtrOut 需要执行推理的输出数据。Host内存
 * @param[out] fps 整个接口的帧率
 * @param[out] modelFps 模型推理的帧率
 * @return lynError_t 枚举值
 */
lynError_t lynDPExecuteModel(const lynDPModel_t model, uint32_t batchSize, void *hostPtrIn,
                             void *hostPtrOut, float *fps, float *modelFps);

/**
 * @brief 执行模型推理，直到返回推理结果 同步接口
 * 
 * @param[in] model 模型句柄
 * @param[in] batchSize 设置的inputBatch数
 * @param[in] hostPtrIn 需要执行推理的输入数据。Host内存
 * @param[out] hostPtrOut 需要执行推理的输出数据。Host内存
 * @return lynError_t 枚举值
 */
lynError_t lynDPExecuteModelWithoutFps(const lynDPModel_t model, uint32_t batchSize,
                                       void *hostPtrIn, void *hostPtrOut);

/**
 * @brief 从模型句柄获取模型描述文件
 * 
 * @param[in] model 模型句柄
 * @param[out] ppdesc lynModelDesc_t指针的地址
 * @return lynError_t 枚举值
 */
lynError_t lynDPGetDesc(const lynDPModel_t model, lynModelDesc_t** ppdesc);

#ifdef __cplusplus
}
#endif

#endif