
#ifndef LYN_RDMA_LCCL_C_H
#define LYN_RDMA_LCCL_C_H

#include "lyn_context.h"
#include "lyn_data_types.h"
#include "lyn_err.h"
#include "lyn_rdma.h"
#include "lyn_stream.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *lynRdmaCommunicator_t;

typedef void *lynLcclLocalCommunicator_t;

typedef void *lynLcclInitMemoryHandle_t;

typedef int (*lynPartGathervCallBack_t)(void *data, size_t size, int node, void *userdata);

typedef struct _lynRdmaNodeMemoryInfo_t {
    void *buf;
    int node;
    size_t size;
} lynRdmaNodeMemoryInfo_t;

typedef struct _lynLcclUserSetting_t {
    int rank;
    int devId;  // devId 预留，暂时不起作用
} lynLcclUserSetting_t;

typedef enum _lynLcclOp {
    LCCL_OP_MAX = 0,
    LCCL_OP_MIN,
    LCCL_OP_SUM,
    LCCL_OP_INVALID,
} lynLcclOp_e;

/**
 * @brief 获取当前版本的描述。
 *
 * @param[out] describe 用户申请的 buffer，用于存放当前版本的描述
 * @param[in] size 用户申请的 buffer 的长度，注意字符串结尾有 '\0'，实际 buffer 能用的空间是 size - 1
 * @return lynError_t
 */
lynError_t lynLcclGetVersionDescribe(char *describe, int size);

/**
 * @brief 获取当前版本的哈希值。
 *
 * @param[out] sha1 用户申请的 buffer，用于存放当前版本的哈希值，哈希值不同，版本必定不同
 * @param[in] size 用户申请的 buffer 的长度，注意字符串结尾有 '\0'，实际 buffer 能用的空间是 size - 1
 * @return lynError_t
 */
lynError_t lynLcclGetVersionSHA1(char *sha1, int size);

/**
 * @brief 获取当前版本的编译时间。
 *
 * @param[out] time 用户申请的 buffer，用于存放当前版本的编译时间
 * @param[in] size 用户申请的 buffer 的长度，注意字符串结尾有 '\0'，实际 buffer 能用的空间是 size - 1
 * @return lynError_t
 */
lynError_t lynLcclGetVersionTime(char *time, int size);

/**
 * @brief 外部程序如 torch、 mpi 等已经为每一个进程分配了 rank 编号、devId，此时不希望 LCCL 重新分配 rank
 * 编号，可以在连接之前调用该接口，使 LCCL 沿用外部的 rank 编号、devId。
 *
 * @param[in] setting 用户设置
 * @return lynError_t
 * 注意：使用 lynLcclUserSetting_t 结构体前，建议将其 memset(&setting, -1, sizeof(lynLcclUserSetting_t));
 */
lynError_t lynLcclUserSet(const lynLcclUserSetting_t *setting);

/**
 * @brief 使集群上的所有节点连接。
 *
 * @param[out] communicator 集群通信的句柄
 * @param[in] nodesJsonPath 配置文件所在的路径
 * @return lynError_t
 */
lynError_t lynLcclConnect(lynRdmaCommunicator_t *communicator, const char *nodesJsonPath);

/**
 * @brief 取消连接
 *
 * @param[in] communicator 连接时分配的句柄
 * @return lynError_t
 */
lynError_t lynLcclDisconnect(lynRdmaCommunicator_t communicator);

/**
 * @brief 获取当前节点的 rank
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] rank 当前节点的 rank
 * @return lynError_t
 */
lynError_t lynLcclGetRank(lynRdmaCommunicator_t communicator, int *rank);

/**
 * @brief 获取当前 rank 对应的芯片 id
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] devId 当前 rank 对应的 devId
 * @return lynError_t
 */
lynError_t lynLcclGetDeviceId(lynRdmaCommunicator_t communicator, int *devId);

/**
 * @brief 获取参与通信的节点总数
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] count 通信的节点总数
 * @return lynError_t
 */
lynError_t lynLcclGetNodeCount(lynRdmaCommunicator_t communicator, int *count);

/**
 * @brief 根据并行数量为每一组并行任务分配芯片
 *
 * @param[in] parallel 并行数量
 * @param[out] devlist 每一组并行任务使用的芯片的列表，按顺序保存到用户开辟的 devlist 中。用户自己对 devlist
 * 做切片提取出每一个并行任务要使用哪些芯片。
 * @return lynError_t
 * 注意：devlist 的长度必须等于整个节点上的芯片数量。
 * 假设有 24 个芯片，parallel 为 4，则 devlist[0:5] 分配给第一个并行任务，devlist[6:11] 分配给第二个并行任务，依此类推。
 * 不接受芯片数量无法整除并行数量的情况。
 */
lynError_t lynLcclParallelDeviceList(int *devlist, int parallel);

/**
 * @brief 创建本节点上的局部通信域
 *
 * @param[out] localCommunicator 本节点上的局部通信域句柄
 * @param[in] communicator 连接时分配的集群通信域的句柄
 * @param[in] devs 局部通信域中参与通信的芯片所在的数组
 * @param[in] count 局部通信域中芯片的个数
 * @return lynError_t
 */
lynError_t lynLcclCreateLocalCommunicator(lynLcclLocalCommunicator_t *localCommunicator,
                                          lynRdmaCommunicator_t communicator, int *devs, int count);

/**
 * @brief 销毁本节点上的局部通信域
 *
 * @param[in] localCommunicator 本节点上的局部通信域句柄
 * @return lynError_t
 */
lynError_t lynLcclDestroyLocalCommunicator(lynLcclLocalCommunicator_t localCommunicator);

/**
 * @brief 通信原语 scatter
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] sendBuf 发送 buffer
 * @param[in] count 接收数据类型为 dataType 的数据的个数
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] node 根节点
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclScatter(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                          lynDataType_t dataType, int node, lynRdmaRegMemType_e memType);

/**
 * @brief 通信原语 scatter，以芯片为最小粒度
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] sendBuf 发送 buffer
 * @param[in] count 接收数据类型为 dataType 的数据的个数
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] root 根节点
 * @param[in] stream 流句柄，为空时，是同步操作
 * @return lynError_t
 */
lynError_t lynLcclChipScatter(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                              lynDataType_t dataType, int root, lynStream_t stream);

/**
 * @brief 本节点内部通信原语 scatter
 *
 * @param[in] localCommunicator 本节点内局部通信域的句柄
 * @param[out] recvBufs 每块内存的接收 buffer 所在的数组
 * @param[in] recvMemType 内存类型
 * @param[in] sendBuf 发送 buffer
 * @param[in] sendMemType 内存类型
 * @param[in] count 接收数据类型为 dataType 的数据的个数
 * @param[in] dataType 数据类型
 * @param[in] devId 设备id。若是芯片侧，则大于等于 0；若为主机侧，则只能是 -1
 * @return lynError_t
 */
lynError_t lynLcclLocalScatter(lynLcclLocalCommunicator_t localCommunicator, void **recvBufs,
                               lynRdmaRegMemType_e recvMemType, void *sendBuf, lynRdmaRegMemType_e sendMemType,
                               int count, lynDataType_t dataType, int devId);

/**
 * @brief 通信原语 gather
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] sendBuf 发送 buffer
 * @param[in] count 发送数据类型为 dataType 的数据的个数
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] node 根节点
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclGather(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                         lynDataType_t dataType, int node, lynRdmaRegMemType_e memType);

/**
 * @brief 通信原语 gather，以芯片为最小粒度
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] sendBuf 发送 buffer
 * @param[in] count 发送数据类型为 dataType 的数据的个数
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] root 根节点
 * @param[in] stream 流句柄，为空时，是同步操作
 * @return lynError_t
 */
lynError_t lynLcclChipGather(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                             lynDataType_t dataType, int root, lynStream_t stream);

/**
 * @brief 本节点内部通信原语 gather
 *
 * @param[in] localCommunicator 本节点内局部通信域的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] recvMemType 内存类型
 * @param[in] sendBufs 每块内存的发送 buffer 所在的数组
 * @param[in] sendMemType 内存类型
 * @param[in] count 发送数据类型为 dataType 的数据的个数
 * @param[in] dataType 数据类型
 * @param[in] devId 设备id。若是芯片侧，则大于等于 0；若为主机侧，则只能是 -1
 * @return lynError_t
 */
lynError_t lynLcclLocalGather(lynLcclLocalCommunicator_t localCommunicator, void *recvBuf,
                              lynRdmaRegMemType_e recvMemType, void **sendBufs, lynRdmaRegMemType_e sendMemType,
                              int count, lynDataType_t dataType, int devId);
/**
 * @brief 通信原语 all-gather
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] sendBuf 发送 buffer
 * @param[in] count 发送数据类型为 dataType 的数据的个数
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclAllGather(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                            lynDataType_t dataType, lynRdmaRegMemType_e memType);

/**
 * @brief 通信原语 all-gather，以芯片为最小粒度
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] sendBuf 发送 buffer
 * @param[in] count 发送数据类型为 dataType 的数据的个数
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] stream 流句柄，为空时，是同步操作
 * @return lynError_t
 */
lynError_t lynLcclChipAllGather(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                                lynDataType_t dataType, lynStream_t stream);

/**
 * @brief 通信原语 all-gatherv-v
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBufs 每个节点要接收的 buffer 所在的数组
 * @param[in] sendBuf 发送 buffer
 * @param[in] counts 每个节点要发送的元素的个数所在的数组
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclAllGatherv(lynRdmaCommunicator_t communicator, void **recvBufs, void *sendBuf, int *counts,
                             lynDataType_t dataType, lynRdmaRegMemType_e memType);

/**
 * @brief 通信原语 all-gatherv-v-h
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBufs 每个节点要接收的 buffer 所在的数组
 * @param[in] sendBuf 发送 buffer
 * @param[in] counts 每个节点要发送的元素的个数所在的数组
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] memType 内存类型
 * @param[in] handle 调用 lynLcclInitAllGathervh 后返回的加速通信的 handle
 * @return lynError_t
 */
lynError_t lynLcclAllGathervh(lynRdmaCommunicator_t communicator, void **recvBufs, void *sendBuf, int *counts,
                              lynDataType_t dataType, lynRdmaRegMemType_e memType, lynLcclInitMemoryHandle_t handle);

/**
 * @brief 通信原语 broadcast
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[in out] data 对编号为 node 的节点来说是要广播的数据所在的 buffer，对其它节点来说是接收 buffer
 * @param[in] count 数据类型为 dataType 的数据的个数
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] node 根节点
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclBroadcast(lynRdmaCommunicator_t communicator, void *data, int count, lynDataType_t dataType, int node,
                            lynRdmaRegMemType_e memType);

/**
 * @brief 通信原语 broadcast，以芯片为最小粒度
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[in out] data 对编号为 node 的节点来说是要广播的数据所在的 buffer，对其它节点来说是接收 buffer
 * @param[in] count 数据类型为 dataType 的数据的个数
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] root 根节点
 * @param[in] stream 流句柄，为空时，是同步操作
 * @return lynError_t
 */
lynError_t lynLcclChipBroadcast(lynRdmaCommunicator_t communicator, void *data, int count, lynDataType_t dataType,
                                int root, lynStream_t stream);

/**
 * @brief 通信原语 all-to-all
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] recvCount 接收数据类型为 dataType 的数据的个数
 * @param[in] sendBuf 发送 buffer
 * @param[in] sendCount 发送数据类型为 dataType 的数据的个数。该入参暂时用不上，因为 recvCount 默认等于 sendCount
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclAllToAll(lynRdmaCommunicator_t communicator, void *recvBuf, int recvCount, void *sendBuf,
                           int sendCount, lynDataType_t dataType, lynRdmaRegMemType_e memType);
/**
 * @brief 通信原语 all-to-all-v
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBufs 每个节点要接收的 buffer 所在的数组
 * @param[in] recvCounts 每个节点要接收的元素的个数所在的数组
 * @param[in] sendBufs 每个节点要发送的 buffer 所在的数组
 * @param[in] sendCounts 每个节点要发送的元素的个数所在的数组
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclAllToAllv(lynRdmaCommunicator_t communicator, void **recvBufs, int *recvCounts, void **sendBufs,
                            int *sendCounts, lynDataType_t dataType, lynRdmaRegMemType_e memType);

/**
 * @brief 通信原语 all-to-all-v-h
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBufs 每个节点要接收的 buffer 所在的数组
 * @param[in] recvCounts 每个节点要接收的元素的个数所在的数组
 * @param[in] sendBufs 每个节点要发送的 buffer 所在的数组
 * @param[in] sendCounts 每个节点要发送的元素的个数所在的数组
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] memType 内存类型
 * @param[in] handle 调用 lynLcclInitAllToAllvh 后返回的加速通信的 handle
 * @return lynError_t
 */
lynError_t lynLcclAllToAllvh(lynRdmaCommunicator_t communicator, void **recvBufs, int *recvCounts, void **sendBufs,
                             int *sendCounts, lynDataType_t dataType, lynRdmaRegMemType_e memType,
                             lynLcclInitMemoryHandle_t handle);

/**
 * @brief 通信原语 reduce
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] sendBuf 发送 buffer
 * @param[in] count 数据类型为 dataType 的数据的个数
 * @param[in] dataType 数据类型
 * @param[in] node 根节点
 * @param[in] op 规约操作
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclReduce(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                         lynDataType_t dataType, int node, lynLcclOp_e op, lynRdmaRegMemType_e memType);

/**
 * @brief 通信原语 reduce，以芯片为最小粒度
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] sendBuf 发送 buffer
 * @param[in] count 数据类型为 dataType 的数据的个数
 * @param[in] dataType 数据类型
 * @param[in] root 根节点
 * @param[in] op 规约操作
 * @param[in] stream 流句柄，为空时，是同步操作
 * @return lynError_t
 */
lynError_t lynLcclChipReduce(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                             lynDataType_t dataType, int root, lynLcclOp_e op, lynStream_t stream);

/**
 * @brief 通信原语 all-reduce
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] sendBuf 发送 buffer
 * @param[in] count 数据类型为 dataType 的数据的个数
 * @param[in] dataType 数据类型
 * @param[in] op 规约操作
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclAllReduce(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                            lynDataType_t dataType, lynLcclOp_e op, lynRdmaRegMemType_e memType);

/**
 * @brief 通信原语 all-reduce，以芯片为最小粒度
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] sendBuf 发送 buffer
 * @param[in] count 数据类型为 dataType 的数据的个数
 * @param[in] dataType 数据类型
 * @param[in] op 规约操作
 * @param[in] stream 流句柄，为空时，是同步操作
 * @return lynError_t
 */
lynError_t lynLcclChipAllReduce(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                                lynDataType_t datatype, lynLcclOp_e op, lynStream_t stream);

/**
 * @brief 通信原语 reduce-scatter
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] sendBuf 发送 buffer
 * @param[in] count recvBuf 中的数据类型为 dataType 的数据的个数
 * @param[in] dataType 数据类型
 * @param[in] op 规约操作
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclReduceScatter(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                                lynDataType_t dataType, lynLcclOp_e op, lynRdmaRegMemType_e memType);

/**
 * @brief 通信原语 reduce-scatter，以芯片为最小粒度
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[out] recvBuf 接收 buffer
 * @param[in] sendBuf 发送 buffer
 * @param[in] count recvBuf 中的数据类型为 dataType 的数据的个数
 * @param[in] dataType 数据类型
 * @param[in] op 规约操作
 * @param[in] stream 流句柄，为空时，是同步操作
 * @return lynError_t
 */
lynError_t lynLcclChipReduceScatter(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                                    lynDataType_t dataType, lynLcclOp_e op, lynStream_t stream);

/**
 * @brief 本节点从 src 指向的内存处发送 count 个类型为 dataType 的数据给节点 peer
 *
 * @param communicator 连接时分配的句柄
 * @param src 发送的数据的首地址
 * @param count 发送的元素的个数
 * @param dataType 发送的数据类型
 * @param peer 接收该数据的节点的 rank 号
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclSend(lynRdmaCommunicator_t communicator, void *src, int count, lynDataType_t dataType, int peer,
                       lynRdmaRegMemType_e memType);

/**
 * @brief 本节点从 src 指向的内存处发送 count 个类型为 dataType 的数据给节点 peer，以芯片为最小粒度
 *
 * @param communicator 连接时分配的句柄
 * @param src 发送的数据的首地址
 * @param count 发送的元素的个数
 * @param dataType 发送的数据类型
 * @param peer 接收该数据的节点的 rank 号
 * @param[in] stream 流句柄，为空时，是同步操作
 * @return lynError_t
 */
lynError_t lynLcclChipSend(lynRdmaCommunicator_t communicator, void *src, int count, lynDataType_t dataType, int peer,
                           lynStream_t stream);

/**
 * @brief 本节点从节点 peer 接收 count 个类型为 dataType 的数据存到 dst 指向的内存处
 *
 * @param communicator 连接时分配的句柄
 * @param dst 接收的数据的首地址
 * @param count 接收的元素的个数
 * @param dataType 接收的数据类型
 * @param peer 发送该数据的节点的 rank 号
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclRecv(lynRdmaCommunicator_t communicator, void *dst, int count, lynDataType_t dataType, int peer,
                       lynRdmaRegMemType_e memType);

/**
 * @brief 本节点从节点 peer 接收 count 个类型为 dataType 的数据存到 dst 指向的内存处，以芯片为最小粒度
 *
 * @param communicator 连接时分配的句柄
 * @param dst 接收的数据的首地址
 * @param count 接收的元素的个数
 * @param dataType 接收的数据类型
 * @param peer 发送该数据的节点的 rank 号
 * @param[in] stream 流句柄，为空时，是同步操作
 * @return lynError_t
 */
lynError_t lynLcclChipRecv(lynRdmaCommunicator_t communicator, void *dst, int count, lynDataType_t dataType, int peer,
                           lynStream_t stream);

/**
 * @brief 开始组操作
 *
 * @param[in] communicator 连接时分配的句柄
 * @return lynError_t
 */
lynError_t lynLcclChipGroupStart(lynRdmaCommunicator_t communicator);

/**
 * @brief 结束组操作
 *
 * @param[in] communicator 连接时分配的句柄
 * @return lynError_t
 */
lynError_t lynLcclChipGroupEnd(lynRdmaCommunicator_t communicator);

/**
 * @brief 节点间同步
 *
 * @param[in] communicator 连接时分配的句柄
 * @return lynError_t
 */
lynError_t lynLcclBarrier(lynRdmaCommunicator_t communicator);

/**
 * @brief 自定义通信原语 part-gatherv
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[in] sendInfos 本节点向外发送的数据所在的数组
 * @param[in] sendInfoSize 本节点向外发送的数据所在的数组的大小
 * @param[in] recvInfos 本节点接收的数据所在的数组
 * @param[in] recvInfoSize 本节点接收的数据所在的数组的大小
 * @param[in] cb 用于处理接收到的数据的回调函数，注意回调函数是并发处理的，用户需要保证回调函数的线程安全
 * @param[in] userdata 用户传入的数据，回调函数使用
 * @param[in] memType 内存类型
 * @return lynError_t
 * 注意： sendInfos, sendInfoSize, recvInfos, recvInfoSize 可以同时为空，此时，该节点既无发送任务也无接收任务。
 */
lynError_t lynLcclPartGatherv(lynRdmaCommunicator_t communicator, lynRdmaNodeMemoryInfo_t *sendInfos, int sendInfoSize,
                              lynRdmaNodeMemoryInfo_t *recvInfos, int recvInfoSize, lynPartGathervCallBack_t cb,
                              void *userdata, lynRdmaRegMemType_e memType);

/** 非标准接口，针对特定通信原语的优化 */
/**
 * @brief 在通信原语 AllGatherv 通信前，创建必要的资源，用于加速通信。参数与 AllGatherv 完全相同。
 * 注意调用该 init 接口后，后续再调 AllGatherv 接口，需要保证传入的内存地址与 init 接口传入的内存地址相同。
 * 多次调用 lynLcclInitAllGatherv 接口，只会以最后一次 init 的内存为准。
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[in] recvBufs 每个节点要接收的 buffer 所在的数组
 * @param[in] sendBuf 发送 buffer
 * @param[in] counts 每个节点要发送的元素的个数所在的数组
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclInitAllGatherv(lynRdmaCommunicator_t communicator, void **recvBufs, void *sendBuf, int *counts,
                                 lynDataType_t dataType, lynRdmaRegMemType_e memType);

/** 非标准接口，针对特定通信原语的优化 */
/**
 * @brief 在通信原语 AllGathervh 通信前，创建必要的资源，用于加速通信。参数与 AllGathervh 完全相同。
 * 注意调用该 init 接口后，后续再调 AllGathervh 接口，需要保证传入的内存地址与 init 接口传入的内存地址相同。
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[in] recvBufs 每个节点要接收的 buffer 所在的数组
 * @param[in] sendBuf 发送 buffer
 * @param[in] counts 每个节点要发送的元素的个数所在的数组
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] memType 内存类型
 * @param[out] handle 加速指定 recvBufs 与 sendBuf 通信的 handle
 * @return lynError_t
 */
lynError_t lynLcclInitAllGathervh(lynRdmaCommunicator_t communicator, void **recvBufs, void *sendBuf, int *counts,
                                  lynDataType_t dataType, lynRdmaRegMemType_e memType,
                                  lynLcclInitMemoryHandle_t *handle);

/** 非标准接口，针对特定通信原语的优化 */
/**
 * @brief 在通信原语 AllToAllv 通信前，创建必要的资源，用于加速通信。参数与 AllToAllv 完全相同。
 * 注意调用该 init 接口后，后续再调 AllToAllv 接口，需要保证传入的内存地址与 init 接口传入的内存地址相同。
 * 多次调用 lynLcclInitAllToAllv 接口，只会以最后一次 init 的内存为准。
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[in] recvBufs 每个节点要接收的 buffer 所在的数组
 * @param[in] recvCounts 每个节点要接收的元素的个数所在的数组
 * @param[in] sendBufs 每个节点要发送的 buffer 所在的数组
 * @param[in] sendCounts 每个节点要发送的元素的个数所在的数组
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] memType 内存类型
 * @return lynError_t
 */
lynError_t lynLcclInitAllToAllv(lynRdmaCommunicator_t communicator, void **recvBufs, int *recvCounts, void **sendBufs,
                                int *sendCounts, lynDataType_t dataType, lynRdmaRegMemType_e memType);

/** 非标准接口，针对特定通信原语的优化 */
/**
 * @brief 在通信原语 AllToAllvh 通信前，创建必要的资源，用于加速通信。参数与 AllToAllvh 完全相同。
 * 注意调用该 init 接口后，后续再调 AllToAllvh 接口，需要保证传入的内存地址与 init 接口传入的内存地址相同。
 *
 * @param[in] communicator 连接时分配的句柄
 * @param[in] recvBufs 每个节点要接收的 buffer 所在的数组
 * @param[in] recvCounts 每个节点要接收的元素的个数所在的数组
 * @param[in] sendBufs 每个节点要发送的 buffer 所在的数组
 * @param[in] sendCounts 每个节点要发送的元素的个数所在的数组
 * @param[in] dataType 发送的数据的数据类型
 * @param[in] memType 内存类型
 * @param[out] handle 加速指定 recvBufs 与 sendBufs 通信的 handle
 * @return lynError_t
 */
lynError_t lynLcclInitAllToAllvh(lynRdmaCommunicator_t communicator, void **recvBufs, int *recvCounts, void **sendBufs,
                                 int *sendCounts, lynDataType_t dataType, lynRdmaRegMemType_e memType,
                                 lynLcclInitMemoryHandle_t *handle);

/* 旧的接口命名，新接口都以 lynLccl 开头，未来逐渐淘汰*/
lynError_t lynRdmaLcclConnect(lynRdmaCommunicator_t *communicator, const char *nodesJsonPath);

lynError_t lynRdmaLcclDisconnect(lynRdmaCommunicator_t communicator);

lynError_t lynRdmaLcclGetRank(lynRdmaCommunicator_t communicator, int *rank);

lynError_t lynRdmaLcclGetNodeCount(lynRdmaCommunicator_t communicator, int *count);

lynError_t lynRdmaScatter(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                          lynDataType_t dataType, int node);

lynError_t lynRdmaGather(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                         lynDataType_t dataType, int node);

lynError_t lynRdmaAllGather(lynRdmaCommunicator_t communicator, void *recvBuf, void *sendBuf, int count,
                            lynDataType_t dataType);

lynError_t lynRdmaAllGatherv(lynRdmaCommunicator_t communicator, void **recvBufs, void *sendBuf, int *counts,
                             lynDataType_t dataType);

lynError_t lynRdmaBroadcast(lynRdmaCommunicator_t communicator, void *data, int count, lynDataType_t dataType,
                            int node);

lynError_t lynRdmaAllToAll(lynRdmaCommunicator_t communicator, void *recvBuf, int recvCount, void *sendBuf,
                           int sendCount, lynDataType_t dataType);

lynError_t lynRdmaAllToAllv(lynRdmaCommunicator_t communicator, void **recvBufs, int *recvCounts, void **sendBufs,
                            int *sendCounts, lynDataType_t dataType);

lynError_t lynRdmaLcclBarrier(lynRdmaCommunicator_t communicator);

lynError_t lynRdmaLcclPartGatherv(lynRdmaCommunicator_t communicator, lynRdmaNodeMemoryInfo_t *sendInfos,
                                  int sendInfoSize, lynRdmaNodeMemoryInfo_t *recvInfos, int recvInfoSize,
                                  lynPartGathervCallBack_t cb, void *userdata);

#ifdef __cplusplus
}
#endif

#endif  // LYN_RDMA_LCCL_C_H
