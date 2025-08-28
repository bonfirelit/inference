/**
 * @file lyn_err.h
 * @brief 错误码的定义和描述
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

#ifndef LYN_ERR_C_H
#define LYN_ERR_C_H

/// \cond
/**
 * ISO C standard errors
 */
#include <errno.h>
#include <stdint.h>
/// \endcond

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 错误码句柄
 */
typedef int lynError_t;

/// \cond
#define lynERRNO(no)        (100000 + (no))

#define lyn_OFFSET          (1000)

#define lynDEMUX_OFFSET     1000
#define lynVDEC_OFFSET      2000
#define lynVENC_OFFSET      3000
#define lynAPU_OFFSET       4000
#define lynIPE_OFFSET       5000
#define lynEVENT_OFFSET     6000
#define lynSTREAM_OFFSET    7000
#define lynCONTEXT_OFFSET   8000
#define lynMEMORY_OFFSET    9000
#define lynUTIL_OFFSET      10000
#define lynIPC_OFFSET       11000
#define lynDEVICE_OFFSET    12000
#define lynENGINE_OFFSET    13000
#define lynPROFILER_OFFSET  14000
#define lynPLUGIN_OFFSET    15000
#define lynMUX_OFFSET       16000
#define lynRDMA_OFFSET      17000
#define lynBLAS_OFFSET      18000
/// \endcond

/// \cond
/**
 * @def lynESTART
 * @brief 错误码起始值
 * 
 * 【错误码】100000
 * 
 */
#define lynESTART           lynERRNO(0)     /* lynxi error code start */
/// \endcond

/**
 * @def lynEALIGN
 * @brief 内存地址对齐错误
 * 
 * 【错误码】100001
 * 
 * 【原因分析】内存地址对齐出现错误
 * 
 * 【解决方法】联系技术支持
 */
#define lynEALIGN           lynERRNO(1)     /* buffer width/height alignment error */

/**
 * @def lynEFORMAT
 * @brief 数据格式不支持错误
 * 
 * 【错误码】100002
 * 
 * 【原因分析】数据格式不支持
 * 
 * 【解决方法】核对数据格式是否支持
 */
#define lynEFORMAT          lynERRNO(2)     /* format not support */

/**
 * @def lynEEOF
 * @brief 文件结束标志
 * 
 * 【错误码】100003
 * 
 * 【原因分析】文件结束标志
 * 
 * 【解决方法】-
 */
#define lynEEOF             lynERRNO(3)     /* end of file */

/**
 * @def lynEPTR
 * @brief 内存地址错误
 * 
 * 【错误码】100004
 * 
 * 【原因分析】内存地址错误
 * 
 * 【解决方法】联系技术支持
 */
#define lynEPTR             lynERRNO(4)     /* invalid ptr */

/**
 * @def lynEINVARG
 * @brief 函数参数错误
 * 
 * 【错误码】100005
 * 
 * 【原因分析】函数参数异常
 * 
 * 【解决方法】联系技术支持
 */
#define lynEINVARG          lynERRNO(5)     /* invalid argument */

/**
 * @def lynENOMEM
 * @brief client侧地址分配错误
 * 
 * 【错误码】100006
 * 
 * 【原因分析】client侧地址分配出错
 * 
 * 【解决方法】检查内存是否泄漏
 */
#define lynENOMEM           lynERRNO(6)     /* client malloc error */

/**
 * @def lynEMALLOC
 * @brief 分配内存错误
 * 
 * 【错误码】100007
 * 
 * 【原因分析】分配内存失败
 * 
 * 【解决方法】检查系统内存是否用完
 */
#define lynEMALLOC          lynERRNO(7)     /* new memory failed */

/**
 * @def lynENOSYSMEM
 * @brief device侧内存耗尽错误
 * 
 * 【错误码】100008
 * 
 * 【原因分析】device分配内存失败
 * 
 * 【解决方法】检查系统内存是否用完
 */
#define lynENOSYSMEM        lynERRNO(8)     /* server system out of memory */

////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////split line of commom and module-related error code/////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
/*demux error number*/

/**
 * @def lynEDEMUXOPEN
 * @brief 媒体流解封装错误
 * 
 * 【错误码】101000
 * 
 * 【原因分析】1) 文件不存在；2) 格式不支持
 * 
 * 【解决方法】1) 检查文件路径；2) 查看文件是否损坏，格式是否有误
 */
#define lynEDEMUXOPEN           (lynERRNO(0) + lynDEMUX_OFFSET)     /*101000 demux open error */

/**
 * @def lynEBSFERR
 * @brief 解复用比特流过滤器错误
 * 
 * 【错误码】101001
 * 
 * 【原因分析】mp4 avc1 to annexb失败
 * 
 * 【解决方法】检查文件封装是否有误
 */
#define lynEBSFERR              (lynERRNO(1) + lynDEMUX_OFFSET)     /*101001 demux bitstream fliter error */

/**
 * @def lynEDEMUXHDL
 * @brief 解封装句柄非法错误
 * 
 * 【错误码】101002
 * 
 * 【原因分析】句柄错误或者已销毁
 * 
 * 【解决方法】检查句柄是否正确
 */
#define lynEDEMUXHDL            (lynERRNO(2) + lynDEMUX_OFFSET)     /*101002 demux handle illegal */

/**
 * @def lynEDEMUXRESOLUTION
 * @brief  解封装分辨率变化错误
 * 
 * 【错误码】101003
 * 
 * 【原因分析】流媒体分辨率发生变化
 * 
 * 【解决方法】解码器不支持分辨率变化，需要关闭解码器后重开解码器
 */
#define lynEDEMUXRESOLUTION     (lynERRNO(3) + lynDEMUX_OFFSET)     /*101003 demux resolution change*/

/**
 * @def lynEDEMUXAUDIO
 * @brief 获取音频参数失败错误
 * 
 * 【错误码】101004
 * 
 * 【原因分析】获取音频参数失败
 * 
 * 【解决方法】检查码流是否包含音频
 */
#define lynEDEMUXAUDIO          (lynERRNO(4) + lynDEMUX_OFFSET)     /*101004 demux get audio param error*/

/*decoder error number*/

/**
 * @def lynECODECERR
 * @brief 解码错误
 * 
 * 【错误码】102000
 * 
 * 【原因分析】解码错误
 * 
 * 【解决方法】联系技术支持
 */
#define lynECODECERR            (lynERRNO(0) + lynVDEC_OFFSET)      /*102000 codec normal error */

/**
 * @def lynEPOLLTIMEOUT
 * @brief 超时错误
 * 
 * 【错误码】102001
 * 
 * 【原因分析】内核poll超时
 * 
 * 【解决方法】检查流程是否卡住
 */
#define lynEPOLLTIMEOUT         (lynERRNO(1) + lynVDEC_OFFSET)      /*102001 poll time out */

/**
 * @def lynEPOLLERR
 * @brief 驱动挂死导致内核错误
 * 
 * 【错误码】102002
 * 
 * 【原因分析】驱动挂死导致内核错误
 * 
 * 【解决方法】联系技术支持
 */
#define lynEPOLLERR             (lynERRNO(2) + lynVDEC_OFFSET)      /*102002 poll kernel error */

/**
 * @def lynEVDECHDL
 * @brief 句柄非法错误
 * 
 * 【错误码】102003
 * 
 * 【原因分析】句柄错误或者已销毁
 * 
 * 【解决方法】检查句柄是否正确
 */
#define lynEVDECHDL             (lynERRNO(3) + lynVDEC_OFFSET)      /*102003 vdec handle not illegal */

/**
 * @def lynEBUFINUSE
 * @brief 缓冲区正在使用错误
 * 
 * 【错误码】102004
 * 
 * 【原因分析】这个buf系统正在使用中
 * 
 * 【解决方法】联系技术支持
 */
#define lynEBUFINUSE            (lynERRNO(4) + lynVDEC_OFFSET)      /*102004 buf is in use */

/**
 * @def lynEGETFMT
 * @brief 编解码器获取格式错误
 * 
 * 【错误码】102005
 * 
 * 【原因分析】编码器/解码器未成功打开
 * 
 * 【解决方法】联系技术支持
 */
#define lynEGETFMT              (lynERRNO(5) + lynVDEC_OFFSET)      /*102005 codec get fmt error */

/**
 * @def lynESETFMT
 * @brief 编解码器设置格式错误
 * 
 * 【错误码】102006
 * 
 * 【原因分析】格式不支持
 * 
 * 【解决方法】检查格式是否有误
 */
#define lynESETFMT              (lynERRNO(6) + lynVDEC_OFFSET)      /*102006 codec set fmt error */

/**
 * @def lynEBUFNOTDEVPTR
 * @brief 地址不是设备指针错误
 * 
 * 【错误码】102007
 * 
 * 【原因分析】使用的不是lynMalloc出来的地址
 * 
 * 【解决方法】使用lynMalloc申请设备侧地址
 */
#define lynEBUFNOTDEVPTR        (lynERRNO(7) + lynVDEC_OFFSET)      /*102007 buffer addr is not device ptr */

/**
 * @def lynEBUFUSENOTFULL
 * @brief 缓冲区数据未填满错误
 * 
 * 【错误码】102008
 * 
 * 【原因分析】Buffer过大导致没有填满
 * 
 * 【解决方法】检查buf大小
 */
#define lynEBUFUSENOTFULL       (lynERRNO(8) + lynVDEC_OFFSET)      /*102008 buffer is too large, data is not full of buffer*/

/**
 * @def lynENOBUFS
 * @brief 无可用缓冲区错误
 * 
 * 【错误码】102009
 * 
 * 【原因分析】编码器/解码器需要做地址绑定，不能一直申请新buf
 * 
 * 【解决方法】buf用内存池管理，且buf个数不得超过25个
 */
#define lynENOBUFS              (lynERRNO(9) + lynVDEC_OFFSET)      /*102009 no buffers */

/**
 * @def lynEPORTQBUF
 * @brief 端口入队缓冲区错误
 * 
 * 【错误码】102010
 * 
 * 【原因分析】编码器/解码器向内核queue buf错误
 * 
 * 【解决方法】联系技术支持
 */
#define lynEPORTQBUF            (lynERRNO(10) + lynVDEC_OFFSET)     /*102010 port queue buffer error */

/**
 * @def lynEPORTDQUEBUF
 * @brief 端口出队缓冲区错误
 * 
 * 【错误码】102011
 * 
 * 【原因分析】编码器/解码器向内核dequeue buf错误
 * 
 * 【解决方法】联系技术支持
 */
#define lynEPORTDQUEBUF         (lynERRNO(11) + lynVDEC_OFFSET)     /*102011 port dequeue buffer error */

/**
 * @def lynESTREAMON
 * @brief 视频流开启错误
 * 
 * 【错误码】102012
 * 
 * 【原因分析】内核打开视频错误
 * 
 * 【解决方法】联系技术支持
 */
#define lynESTREAMON            (lynERRNO(12) + lynVDEC_OFFSET)     /*102012 stream on error */

/**
 * @def lynESTREAMOFF
 * @brief 视频流关闭错误
 * 
 * 【错误码】102013
 * 
 * 【原因分析】内核关闭视频错误
 * 
 * 【解决方法】联系技术支持
 */
#define lynESTREAMOFF           (lynERRNO(13) + lynVDEC_OFFSET)     /*102013 stream off error */

/**
 * @def lynEOPENDEV
 * @brief 打开设备错误
 * 
 * 【错误码】102014
 * 
 * 【原因分析】文件句柄不足
 * 
 * 【解决方法】检查是否有句柄泄露或者打开设备过多
 */
#define lynEOPENDEV             (lynERRNO(14) + lynVDEC_OFFSET)     /*102014 open device error */

/**
 * @def lynEGETCTRL
 * @brief 获取控制参数错误
 * 
 * 【错误码】102015
 * 
 * 【原因分析】获取参数错误
 * 
 * 【解决方法】联系技术支持
 */
#define lynEGETCTRL             (lynERRNO(15) + lynVDEC_OFFSET)     /*102015 get control error */

/**
 * @def lynERESOLUTION
 * @brief 分辨率不合法错误
 * 
 * 【错误码】102016
 * 
 * 【原因分析】分辨率不合法
 * 
 * 【解决方法】检查图像大小是否合理
 */
#define lynERESOLUTION          (lynERRNO(16) + lynVDEC_OFFSET)     /*102016 resolution not in range */

/**
 * @def lynEBUFSIZE
 * @brief 缓冲大小不合法错误
 * 
 * 【错误码】102017
 * 
 * 【原因分析】buf大小不合法
 * 
 * 【解决方法】检查buf大小是否合理
 */
#define lynEBUFSIZE             (lynERRNO(17) + lynVDEC_OFFSET)     /*102017 buffer size not correct */

/**
 * @def lynESENDBLOCK
 * @brief 发送数据包不足错误
 * 
 * 【错误码】102018
 * 
 * 【原因分析】在有b帧的情况下需要依赖后面的帧才能解码，送的帧数不够解码导致超时
 * 
 * 【解决方法】多发送几帧再接收
 */
#define lynESENDBLOCK           (lynERRNO(18) + lynVDEC_OFFSET)     /*102018 send packet not enough */

/**
 * @def lynESENDMAX
 * @brief 发送数据包过多错误
 * 
 * 【错误码】102019
 * 
 * 【原因分析】发送指令太多，而且没有及时接收，导致解码出的帧达到上限
 * 
 * 【解决方法】及时接收将解码帧取出
 */
#define lynESENDMAX             (lynERRNO(19) + lynVDEC_OFFSET)     /*102019 send packet too many */

/**
 * @def lynESENDTIMEOUT
 * @brief 发送数据包超时错误
 * 
 * 【错误码】102020
 * 
 * 【原因分析】发送的数据解码器无法识别
 * 
 * 【解决方法】检查发送的数据是否有误
 */
#define lynESENDTIMEOUT         (lynERRNO(20) + lynVDEC_OFFSET)     /*102020 send packet timeout */

/**
 * @def lynERECVTIMEOUT
 * @brief 接受数据包超时错误
 * 
 * 【错误码】102021
 * 
 * 【原因分析】1) 发送的数据解码器无法识别；2) 没有发送直接接收
 * 
 * 【解决方法】1) 检查发送的数据是否有误；2) 发送之后再接收
 */
#define lynERECVTIMEOUT         (lynERRNO(21) + lynVDEC_OFFSET)     /*102021 recv frame timeout */

/**
 * @def lynEVDECCORRUPT
 * @brief 解码流损坏错误
 * 
 * 【错误码】102022
 * 
 * 【原因分析】解码流损坏
 * 
 * 【解决方法】检查数据源是否正确
 */
#define lynEVDECCORRUPT         (lynERRNO(22) + lynVDEC_OFFSET)     /*102022 decoder corrupt */

/**
 * @def lynEJDECHDL
 * @brief JPEG解码句柄错误
 * 
 * 【错误码】102023
 * 
 * 【原因分析】JPEG解码句柄错误
 * 
 * 【解决方法】检查此句柄是否打开活或已经关闭
 */
#define lynEJDECHDL             (lynERRNO(23) + lynVDEC_OFFSET)     /*102023 jdec handle not illegal */

/**
 * @def lynEDATACHECK
 * @brief 数据校验错误
 * 
 * 【错误码】102024
 * 
 * 【原因分析】收发两端数据校验出错
 * 
 * 【解决方法】联系技术支持
 */
#define lynEDATACHECK           (lynERRNO(24) + lynVDEC_OFFSET)     /*102024 data check error */

/**
 * @def lynEPASSIVE
 * @brief其他流错误导致被动退出错误
 * 
 * 【错误码】102025
 * 
 * 【原因分析】同一个解码器另一个stream出现错误，导致被动退出
 * 
 * 【解决方法】检查另一个stream的错误
 */
#define lynEPASSIVE             (lynERRNO(25) + lynVDEC_OFFSET)     /*102025 error in other stream cause passive exits */

/**
 * @def lynEWORKLOAD
 * @brief 工作负载超出限制错误
 * 
 * 【错误码】102026
 * 
 * 【原因分析】编解码路数超过限制数量
 * 
 * 【解决方法】检查同一个芯片的编解码路数
 */
#define lynEWORKLOAD            (lynERRNO(26) + lynVDEC_OFFSET)     /*102026 workload exceed limit */
/// \cond
#define lynEQUERYCAP            (lynERRNO(27) + lynVDEC_OFFSET)     /*102027 query cap error*/
/// \endcond


/*encoder error number*/

/**
 * @def lynEINVENCPARAM
 * @brief 编码参数设置错误
 * 
 * 【错误码】103000
 * 
 * 【原因分析】编码参数设置错误
 * 
 * 【解决方法】根据错误提示信息检查编码参数
 */
#define lynEINVENCPARAM         (lynERRNO(0) + lynVENC_OFFSET)      /*103000 input params check failed. */

/**
 * @def lynEINVENCHDL
 * @brief 编码器句柄错误
 * 
 * 【错误码】103001
 * 
 * 【原因分析】1) 编码器已关闭；2) 打开编码器时失败了
 * 
 * 【解决方法】1) 检查代码中编写错误；2) 联系技术支持
 */
#define lynEINVENCHDL           (lynERRNO(1) + lynVENC_OFFSET)      /*103001 handle invalid */

/**
 * @def lynEENCOUTPUTREADY
 * @brief 输出超时错误
 * 
 * 【错误码】103002
 * 
 * 【原因分析】长时间（10s内）没有收到recvpacket指令
 * 
 * 【解决方法】1) 调整编码流程，保证编码过程中sendframe指令比recvpacket指令多1，然后在eos帧接收的时候发送两个recvpacket指令；2) 联系技术支持
 */
#define lynEENCOUTPUTREADY      (lynERRNO(2) + lynVENC_OFFSET)      /*103002 encode: wait output ready time out error. */

/**
 * @def lynEENCINPUTREADY
 * @brief 输入超时错误
 * 
 * 【错误码】103003
 * 
 * 【原因分析】长时间（10s）没有收到recvpacket指令
 * 
 * 【解决方法】1) 检查代码中是否有卡死或耗时较长，导致发送太慢的逻辑；2) 联系技术支持
 */
#define lynEENCINPUTREADY       (lynERRNO(3) + lynVENC_OFFSET)      /*103003 encode: wait input ready time out error. */

/**
 * @def lynEENCGETPKTVALIDSIZE
 * @brief 获取包有效大小错误
 * 
 * 【错误码】103004
 * 
 * 【原因分析】编码流程出错，packet数据无效
 * 
 * 【解决方法】排查前面的编码流程异常或联系技术支持
 */
#define lynEENCGETPKTVALIDSIZE  (lynERRNO(4) + lynVENC_OFFSET)      /*103004 encode: get packet valid size error. */

/**
 * @def lynEENCOUTBUFSIZE
 * @brief 输出缓冲区大小不足错误
 * 
 * 【错误码】103005
 * 
 * 【原因分析】接收packet对应的buffer过小，无法容纳全部输出数据；
 * 
 * 【解决方法】扩大输出buffer的容量大小，然后编码重试；
 */
#define lynEENCOUTBUFSIZE       (lynERRNO(5) + lynVENC_OFFSET)      /*103005 encode: out buffer size not enough. */

/*infer error number*/

/**
 * @def lynEAPUOPEN
 * @brief APU启动错误
 * 
 * 【错误码】104000
 * 
 * 【原因分析】APU启动失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEAPUOPEN             (lynERRNO(0) + lynAPU_OFFSET)       /*104000 apu driver error open*/

/**
 * @def lynEAPULOAD
 * @brief APU模型加载错误
 * 
 * 【错误码】104001
 * 
 * 【原因分析】模型配置未设置
 * 
 * 【解决方法】1) 检查模型配置是否设置；2) 联系技术支持
 */
#define lynEAPULOAD             (lynERRNO(1) + lynAPU_OFFSET)       /*104001 apu driver error load*/

/**
 * @def lynEAPUSTART
 * @brief APU推理起始错误
 * 
 * 【错误码】104002
 * 
 * 【原因分析】开始神经网络推理失败
 * 
 * 【解决方法】1) 检查神经网络推理输入参数是否正确；2) 联系技术支持
 */
#define lynEAPUSTART            (lynERRNO(2) + lynAPU_OFFSET)       /*104002 apu driver error start*/

/**
 * @def lynEAPULS
 * @brief APU加载推理错误
 * 
 * 【错误码】104003
 * 
 * 【原因分析】加载模型并直接开始神经网络推理失败
 * 
 * 【解决方法】1) 检查神经网络推理输入参数是否正确；2) 联系技术支持
 */
#define lynEAPULS               (lynERRNO(3) + lynAPU_OFFSET)       /*104003 apu driver error loadStart*/

/**
 * @def lynEAPURCV
 * @brief APU获取计算结果超时错误
 * 
 * 【错误码】104004
 * 
 * 【原因分析】1)运算结果回返超时；2)运算状态返回异常
 * 
 * 【解决方法】联系技术支持
 */
#define lynEAPURCV              (lynERRNO(4) + lynAPU_OFFSET)       /*104004 apu driver error recv*/

/**
 * @def lynEAPURESET
 * @brief APU重载模型错误
 * 
 * 【错误码】104005
 * 
 * 【原因分析】重试加载神经网络模型失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEAPURESET            (lynERRNO(5) + lynAPU_OFFSET)       /*104005 apu driver error reset*/

/**
 * @def lynEAPUUNL
 * @brief 模型卸载错误
 * 
 * 【错误码】104006
 * 
 * 【原因分析】模型卸载失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEAPUUNL              (lynERRNO(6) + lynAPU_OFFSET)       /*104006 apu driver error unload*/

/**
 * @def lynEAPUMAP
 * @brief APU映射物理地址错误
 * 
 * 【错误码】104007
 * 
 * 【原因分析】Core映射物理地址失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEAPUMAP              (lynERRNO(7) + lynAPU_OFFSET)       /*104007 apu driver error mmap*/

/**
 * @def lynEAPUUNMAP
 * @brief APU取消映射物理地址错误
 * 
 * 【错误码】104008
 * 
 * 【原因分析】Core解映射物理地址失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEAPUUNMAP            (lynERRNO(8) + lynAPU_OFFSET)       /*104008 apu driver error unmap*/

/**
 * @def lynEAPUTASKS
 * @brief APU任务超限错误
 * 
 * 【错误码】104009
 * 
 * 【原因分析】APU模型超过限制
 * 
 * 【解决方法】检查在折叠模型中调用lynModelSendInputAsync或者lynModelRecvOutputAsync接口
 */
#define lynEAPUTASKS            (lynERRNO(9) + lynAPU_OFFSET)       /*104009 too many apu tasks*/
/// \cond
#define lynEAPUAPICONFL         (lynERRNO(10) + lynAPU_OFFSET)      /*104010 apu api conflicted*/
/// \endcond

/**
 * @def lynEFILEXIST
 * @brief 文件不存在错误
 * 
 * 【错误码】104011
 * 
 * 【原因分析】读取文件环节，文件不存在
 * 
 * 【解决方法】1) 检查接口调用流程，判断是哪个文件读取时异常；2) 联系技术支持
 */
#define lynEFILEXIST            (lynERRNO(11) + lynAPU_OFFSET)      /*104011 file dot not exist */

/**
 * @def lynEFILCONT
 * @brief 文件内容解析错误
 * 
 * 【错误码】104012
 * 
 * 【原因分析】读取文件环节，文件解析异常
 * 
 * 【解决方法】1) 检查接口调用流程，判断是哪个文件读取时异常；2) 联系技术支持
 */
#define lynEFILCONT             (lynERRNO(12) + lynAPU_OFFSET)      /*104012 file content error */

/**
 * @def lynEMODELID
 * @brief 混合推理错误
 * 
 * 【错误码】104013
 * 
 * 【原因分析】Hybrid推理异常，无法执行CPU段推理
 * 
 * 【解决方法】1) 检查输入模型是否正确；2) 联系技术支持
 */
#define lynEMODELID             (lynERRNO(13) + lynAPU_OFFSET)      /*104013 infer model id error */

/**
 * @def lynEHWID
 * @brief 推理硬件资源释放错误
 * 
 * 【错误码】104014
 * 
 * 【原因分析】推理结束提出释放硬件资源失败
 * 
 * 【解决方法】1) 检查是否多次释放资源；2) 联系技术支持
 */
#define lynEHWID                (lynERRNO(14) + lynAPU_OFFSET)      /*104014 infer hwid error */

/**
 * @def lynEIDOVFL
 * @brief 模型推理句柄耗尽错误
 * 
 * 【错误码】104015
 * 
 * 【原因分析】模型推理句柄耗尽
 * 
 * 【解决方法】1) 检查创建模型推理数量是否超过最大值；2) 联系技术支持
 */
#define lynEIDOVFL              (lynERRNO(15) + lynAPU_OFFSET)      /*104015 infer id overflow */

/**
 * @def lynETENSOROVFL
 * @brief 模型Tensor数量超限错误
 * 
 * 【错误码】104016
 * 
 * 【原因分析】模型Tensor数量超出限制
 * 
 * 【解决方法】1) 检查输入模型Tensor数量；2) 联系技术支持
 */
#define lynETENSOROVFL          (lynERRNO(16) + lynAPU_OFFSET)      /*104016 tensor number overflow */

/**
 * @def lynEINVBATCH
 * @brief batch数不合法错误
 * 
 * 【错误码】104017
 * 
 * 【原因分析】模型后处理接口传入Batch数错误
 * 
 * 【解决方法】1) 输入Batch数大于等于1，小于等于32；2) 联系技术支持
 */
#define lynEINVBATCH            (lynERRNO(17) + lynAPU_OFFSET)      /*104017 invalid batch number */

/**
 * @def lynEYOLOV3PP
 * @brief yolov3 post process failed 
 * 
 * 【错误码】104018
 * 
 * 【原因分析】Yolov3模型后处理错误
 * 
 * 【解决方法】1) 检查输入参数及数据正确性；2) 联系技术支持
 */
#define lynEYOLOV3PP            (lynERRNO(18) + lynAPU_OFFSET)      /*104018 yolov3 post process failed */

/**
 * @def lynESETAPUPARA
 * @brief APU参数设定错误
 * 
 * 【错误码】104019
 * 
 * 【原因分析】APU参数设定失败
 * 
 * 【解决方法】1) 检查APU模型是否正常；2) 联系技术支持
 */
#define lynESETAPUPARA          (lynERRNO(19) + lynAPU_OFFSET)      /*104019 set apu common param failed */

/**
 * @def lynEOPLIBOPEN
 * @brief ARM算子库打开错误
 * 
 * 【错误码】104020
 * 
 * 【原因分析】ARM算子库打开错误
 * 
 * 【解决方法】1) 检查编译器生成物及读取权限2) 联系技术支持
 */
#define lynEOPLIBOPEN           (lynERRNO(20) + lynAPU_OFFSET)      /*104020 open arm op lib failed */

/**
 * @def lynEOPEXIST
 * @brief ARM算子库不存在错误
 * 
 * 【错误码】104021
 * 
 * 【原因分析】ARM算子库不存在
 * 
 * 【解决方法】1） 检查模型版本及模型；2） 联系技术支持
 */
#define lynEOPEXIST             (lynERRNO(21) + lynAPU_OFFSET)      /*104021 arm op not exist */

/**
 * @def lynEOPRUN
 * @brief ARM算子库运行错误
 * 
 * 【错误码】104022
 * 
 * 【原因分析】ARM算子库运行错误
 * 
 * 【解决方法】联系技术支持
 */
#define lynEOPRUN               (lynERRNO(22) + lynAPU_OFFSET)      /*104022 arm op run failed */

/**
 * @def lynEAPUOFFLINE
 * @brief APU不可用错误
 * 
 * 【错误码】104023
 * 
 * 【原因分析】APU不可用
 * 
 * 【解决方法】使用SMI使能APU
 */
#define lynEAPUOFFLINE          (lynERRNO(23) + lynAPU_OFFSET)      /*104023 apu offline */

/**
 * @def lynEAPUGETINFO
 * @brief 获取APU信息失败错误
 * 
 * 【错误码】104024
 * 
 * 【原因分析】获取APU信息失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEAPUGETINFO          (lynERRNO(24) + lynAPU_OFFSET)      /*104024 apu driver error getInfo */

/**
 * @def lynEAPUBATCH
 * @brief 推理动态尺寸错误
 * 
 * 【错误码】104025
 * 
 * 【原因分析】推理动态尺寸错误
 * 
 * 【解决方法】确定动态尺寸大小是否符合要求
 */
#define lynEAPUBATCH            (lynERRNO(25) + lynAPU_OFFSET)      /*104025 apu batch size error */

/**
 * @def lynEMODELTYPE
 * @brief 获取batch基准值的模型类型错误
 * 
 * 【错误码】104026
 * 
 * 【原因分析】获取batch基准值的模型类型错误
 * 
 * 【解决方法】1）确定模型生成lyngor版本为1.7.0以上；2）联系技术支持
 */
#define lynEMODELTYPE           (lynERRNO(26) + lynAPU_OFFSET)      /*104026 error model type for getting batch base value */

/**
 * @def lynEAPUMEMORYMODE
 * @brief 运行环境内存布局和模型内存布局不匹配错误
 * 
 * 【错误码】104027
 * 
 * 【原因分析】获取运行环境内存布局，和模型内存布局对比
 * 
 * 【解决方法】1）以模型内存布局为准，设定运行环境内存布局；2）联系技术支持
 */
#define lynEAPUMEMORYMODE           (lynERRNO(27) + lynAPU_OFFSET)      /*104027 error check bsp memory mode 5gor7g, model memory mode 5gor7g*/

/*ipe error number*/

/**
 * @def lynECOLORCVTSTD
 * @brief 换标准参数设置错误
 * 
 * 【错误码】105000
 * 
 * 【原因分析】Yuv to RGB转换标准参数设置错误
 * 
 * 【解决方法】参考C2C参数说明设置参数
 */
#define lynECOLORCVTSTD         (lynERRNO(0) + lynIPE_OFFSET)       /*105000 ipe error color convert standard */

/**
 * @def lynECOLORSPACE
 * @brief C2C目标类型设置错误
 * 
 * 【错误码】105001
 * 
 * 【原因分析】C2C目标类型设置错误
 * 
 * 【解决方法】参考C2C参数说明设置参数
 */
#define lynECOLORSPACE          (lynERRNO(1) + lynIPE_OFFSET)       /*105001 ipe error colorspace */

/**
 * @def lynEIMGSIZE
 * @brief 图像尺寸错误
 * 
 * 【错误码】105002
 * 
 * 【原因分析】输入或输出图像不符合要求
 * 
 * 【解决方法】参考表 15 IPE模块支持与限制说明调整设置
 */
#define lynEIMGSIZE             (lynERRNO(2) + lynIPE_OFFSET)       /*105002 ipe image size error */

/**
 * @def lynECROP
 * @brief 裁剪参数错误
 * 
 * 【错误码】105003
 * 
 * 【原因分析】裁剪参数设置错误
 * 
 * 【解决方法】参考表 15 IPE模块支持与限制说明调整设置
 */
#define lynECROP                (lynERRNO(3) + lynIPE_OFFSET)       /*105003 ipe crop params error */

/**
 * @def lynEPADCOLOR
 * @brief 填充颜色错误
 * 
 * 【错误码】105004
 * 
 * 【原因分析】填充颜色设置错误
 * 
 * 【解决方法】参考表 15 IPE模块支持与限制说明调整设置
 */
#define lynEPADCOLOR            (lynERRNO(4) + lynIPE_OFFSET)       /*105004 ipe padding color error */

/**
 * @def lynEPADNUM
 * @brief 填充行列错误
 * 
 * 【错误码】105005
 * 
 * 【原因分析】填充行列设置错误
 * 
 * 【解决方法】参考表 15 IPE模块支持与限制说明调整设置
 */
#define lynEPADNUM              (lynERRNO(5) + lynIPE_OFFSET)       /*105005 ipe padding num error */

/**
 * @def lynEIPEHW
 * @brief IPE设备启动失败错误
 * 
 * 【错误码】105006
 * 
 * 【原因分析】IPE设备启动失败
 * 
 * 【解决方法】重启设备并重试。如问题仍然存在，请联系技术支持
 */
#define lynEIPEHW               (lynERRNO(6) + lynIPE_OFFSET)       /*105006 ipe device init failed */

/**
 * @def lynEIPEIDLE
 * @brief IPE设备闲置错误
 * 
 * 【错误码】105007
 * 
 * 【原因分析】IPE设备未工作
 * 
 * 【解决方法】重启设备并重试。如问题仍然存在，请联系技术支持
 */
#define lynEIPEIDLE             (lynERRNO(7) + lynIPE_OFFSET)       /*105007 ipe device idle */

/**
 * @def lynEIPEPROCESS
 * @brief IPE处理错误
 * 
 * 【错误码】105008
 * 
 * 【原因分析】IPE设备繁忙
 * 
 * 【解决方法】重启设备并重试。如问题仍然存在，请联系技术支持
 */
#define lynEIPEPROCESS          (lynERRNO(8) + lynIPE_OFFSET)       /*105008 ipe process failed */

/**
 * @def lynEIPERCVDRV
 * @brief IPE驱动接收数据错误
 * 
 * 【错误码】105009
 * 
 * 【原因分析】IPE驱动异常
 * 
 * 【解决方法】重启设备并重试。如问题仍然存在，请联系技术支持
 */
#define lynEIPERCVDRV           (lynERRNO(9) + lynIPE_OFFSET)       /*105009 ipe driver recv failed */

/**
 * @def lynEIPEVPSSTIMEOUT
 * @brief IPE除旋转外图像处理超时错误
 * 
 * 【错误码】105010
 * 
 * 【原因分析】IPE除旋转外其他6种图像处理超时
 * 
 * 【解决方法】重启设备并重试。如问题仍然存在，请联系技术支持
 */
#define lynEIPEVPSSTIMEOUT      (lynERRNO(10) + lynIPE_OFFSET)      /*105010 ipe recv vpss process time out*/

/**
 * @def lynEIPEROTTIMEOUT
 * @brief IPE旋转处理超时错误
 * 
 * 【错误码】105011
 * 
 * 【原因分析】IPE旋转处理超时
 * 
 * 【解决方法】重启设备并重试。如问题仍然存在，请联系技术支持
 */
#define lynEIPEROTTIMEOUT       (lynERRNO(11) + lynIPE_OFFSET)      /*105011 ipe recv rotation process time out */

/**
 * @def lynEIPESEND
 * @brief IPE图像加载错误
 * 
 * 【错误码】105012
 * 
 * 【原因分析】IPE图像加载失败
 * 
 * 【解决方法】重启设备并重试。如问题仍然存在，请联系技术支持
 */
#define lynEIPESEND             (lynERRNO(12) + lynIPE_OFFSET)      /*105012 ipe send failed */

/**
 * @def lynEIPEPIXELFORMAT
 * @brief IPE图像格式错误
 * 
 * 【错误码】105013
 * 
 * 【原因分析】IPE输入或者输出图像格式错误
 * 
 * 【解决方法】参考表 15 IPE模块支持与限制说明调整设置
 */
#define lynEIPEPIXELFORMAT      (lynERRNO(13) + lynIPE_OFFSET)      /*105013 ipe pixel format error */

/**
 * @def lynEIPEAFFINE
 * @brief IPE仿射变换错误
 * 
 * 【错误码】105014
 * 
 * 【原因分析】IPE affine变换失败，矩阵计算超过最大分辨率或者最小分辨率
 * 
 * 【解决方法】需要减小或者放大输入图像，调整矩阵的缩放系数
 */
#define lynEIPEAFFINE           (lynERRNO(14) + lynIPE_OFFSET)      /*105014 ipe affine failed */

/**
 * @def lynEIPEMATRIX
 * @brief IPE仿射变换矩阵错误
 * 
 * 【错误码】105015
 * 
 * 【原因分析】IPE affine变换矩阵输入错误
 * 
 * 【解决方法】检查仿射变换的输入矩阵参数
 */
#define lynEIPEMATRIX           (lynERRNO(15) + lynIPE_OFFSET)      /*105015 ipe affine matrix error */

/**
 * @def lynEIPETRANSLATION
 * @brief IPE仿射变换平移分量错误
 * 
 * 【错误码】105016
 * 
 * 【原因分析】IPE affine变换矩阵平移分量大于或者等于设置输出图像大小，输出无效
 * 
 * 【解决方法】需要修改平移分量或者是输出图像大小
 */
#define lynEIPETRANSLATION      (lynERRNO(16) + lynIPE_OFFSET)      /*105016 ipe affine translation error */

/**
 * @def lynEIPEMERGE
 * @brief IPE merge功能参数错误
 * 
 * 【错误码】105017
 * 
 * 【问题原因】IPE merge功能参数输入错误
 * 
 * 【解决方法】检查图像拼接功能的输入参数
 */
#define lynEIPEMERGE            (lynERRNO(17) + lynIPE_OFFSET)      /*105017 ipe merge image parameter error */

/*event error number*/

/**
 * @def lynEEVENTCREATE
 * @brief Event创建错误
 * 
 * 【错误码】106000
 * 
 * 【原因分析】Event创建失败
 * 
 * 【解决方法】1) 检查进程内存占用多；2) 释放没用的Event
 */
#define lynEEVENTCREATE         (lynERRNO(0) + lynEVENT_OFFSET)     /*106000 create event fail */

/**
 * @def lynEINVALEVENT
 * @brief 非法Event对象错误
 * 
 * 【错误码】106001
 * 
 * 【原因分析】Event对象不存在
 * 
 * 【解决方法】联系技术支持
 */
#define lynEINVALEVENT          (lynERRNO(1) + lynEVENT_OFFSET)     /*106001 invalid event object */

/**
 * @def lynEEVENTID
 * @brief 找不到Event对象错误
 * 
 * 【错误码】106002
 * 
 * 【原因分析】找不到ID对应的Event对象
 * 
 * 【解决方法】联系技术支持
 */
#define lynEEVENTID             (lynERRNO(2) + lynEVENT_OFFSET)     /*106002 can't find event by id */

/**
 * @def LynEEVENTTIMEOUT
 * @brief Event等待超时错误
 * 
 * 【错误码】106003
 * 
 * 【原因分析】Event等待出现超时
 * 
 * 【解决方法】联系技术支持
 */
#define LynEEVENTTIMEOUT        (lynERRNO(3) + lynEVENT_OFFSET)     /*106003 event wait timeout */

/*stream error number*/

/**
 * @def lynESTREAM
 * @brief Stream句柄错误
 * 
 * 【错误码】107000
 * 
 * 【原因分析】使用非法Stream句柄
 * 
 * 【解决方法】检查以Stream句柄作为入参的SDK API调用代码
 */
#define lynESTREAM              (lynERRNO(0) + lynSTREAM_OFFSET)    /*107000  used in wrong stream */

/**
 * @def lynESTREAMMNG
 * @brief Stream管理器错误
 * 
 * 【错误码】107001
 * 
 * 【原因分析】Stream管理器错误
 * 
 * 【解决方法】1) 检查Context句柄是否正确使用；2) 联系技术支持
 */
#define lynESTREAMMNG           (lynERRNO(1) + lynSTREAM_OFFSET)    /*107001 stream manager object not exist */

/**
 * @def lynESTREAMCREATE
 * @brief Stream创建数量超限错误
 * 
 * 【错误码】107002
 * 
 * 【原因分析】超过一个Context句柄下创建Stream个数的上限
 * 
 * 【解决方法】确认创建Stream个数已经达到上限
 */
#define lynESTREAMCREATE        (lynERRNO(2) + lynSTREAM_OFFSET)    /*107002 create stream fail */

/**
 * @def lynECMD
 * @brief 不支持的Stream指令错误
 * 
 * 【错误码】107003
 * 
 * 【原因分析】Stream执行不支持的指令
 * 
 * 【解决方法】联系技术支持
 */
#define lynECMD                 (lynERRNO(3) + lynSTREAM_OFFSET)    /*107003 not supported command */

/**
 * @def lynERUNSIDE
 * @brief Stream指令执行错误
 * 
 * 【错误码】107004
 * 
 * 【原因分析】Stream指令异常
 * 
 * 【解决方法】联系技术支持
 */
#define lynERUNSIDE             (lynERRNO(4) + lynSTREAM_OFFSET)    /*107004 error in code run-side specification */

/// \cond
/**
 * @def lynELAUNCHERRHNDLTHR
 * @brief 保留
 * 
 * 【错误码】107005
 * 
 * -
 */
#define lynELAUNCHERRHNDLTHR    (lynERRNO(5) + lynSTREAM_OFFSET)    /*107005 failed to launch error handle thread */
/// \endcond

/*context error number*/

/**
 * @def lynECONTEXT
 * @brief 无效Context错误
 * 
 * 【错误码】108000
 * 
 * 【原因分析】当前线程没有显示创建Context或者没有被设置过Context
 * 
 * 【解决方法】1)调用lynCreateContext显示创建Context；2)调用lynSetCurrentContext设置当前线程的Context，且调用该接口的入参必须是已经创建并且存在的Context
 */
#define lynECONTEXT             (lynERRNO(0) + lynCONTEXT_OFFSET)   /*108000 used in wrong context */

/**
 * @def lynELAUNCH
 * @brief Context启动失败错误
 * 
 * 【错误码】108001
 * 
 * 【原因分析】IPC通道断开或设备侧出现异常
 * 
 * 【解决方法】联系技术支持
 */
#define lynELAUNCH              (lynERRNO(1) + lynCONTEXT_OFFSET)   /*108001 error launch */

/**
 * @def lynESTOP
 * @brief Context异常退出错误
 * 
 * 【错误码】108002
 * 
 * 【原因分析】IPC通道断开或设备侧出现异常
 * 
 * 【解决方法】联系技术支持
 */
#define lynESTOP                (lynERRNO(2) + lynCONTEXT_OFFSET)   /*108002 errno stop */

/**
 * @def lynENOCTX
 * @brief 未创建Context错误
 * 
 * 【错误码】108003
 * 
 * 【原因分析】Context未创建
 * 
 * 【解决方法】检查是否已调用Context相关接口创建Context
 */
#define lynENOCTX               (lynERRNO(3) + lynCONTEXT_OFFSET)   /*108003 context not created */

/**
 * @def lynECTXCREATE
 * @brief Context创建失败错误
 * 
 * 【错误码】108004
 * 
 * 【原因分析】Context创建失败
 * 
 * 【解决方法】1) 查看主机或服务器内存空间是否满载；2) 联系技术支持
 */
#define lynECTXCREATE           (lynERRNO(4) + lynCONTEXT_OFFSET)   /*108004 context create failed */

/**
 * @def lynEPNAMEQUERY
 * @brief 获取进程名称错误
 * 
 * 【错误码】108005
 * 
 * 【原因分析】系统接口调用失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEPNAMEQUERY          (lynERRNO(5) + lynCONTEXT_OFFSET)   /*108005 get process name failed */

/**
 * @def lynECTXINIT
 * @brief Context初始化错误
 * 
 * 【错误码】108006
 * 
 * 【原因分析】1)其他Runtime模块初始化失败；2)IPC通道断开
 * 
 * 【解决方法】1)根据其他可能出现的错误码查找具体失败的模块；2)联系技术支持
 */
#define lynECTXINIT             (lynERRNO(6) + lynCONTEXT_OFFSET)   /*108006 context init failed */

/**
 * @def lynECTXPNUM
 * @brief Context进程超限错误
 * 
 * 【错误码】108007
 * 
 * 【原因分析】Context中进程打开数量超过限制，每个Context打开的进程最大数量是16个
 * 
 * 【解决方法】减少打开进程数量或者使用其他Context
 */
#define lynECTXPNUM             (lynERRNO(7) + lynCONTEXT_OFFSET)   /*108007 process num exceed limit in the context */

/*memory error number*/

/**
 * @def lynEDMAOPEN
 * @brief 打开DMA驱动错误
 * 
 * 【错误码】109000
 * 
 * 【原因分析】DMA驱动接口打开失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEDMAOPEN             (lynERRNO(0) + lynMEMORY_OFFSET)    /*109000 DMA driver open fail */

/**
 * @def lynEDMACLOSE
 * @brief DMA驱动关闭错误
 * 
 * 【错误码】109001
 * 
 * 【原因分析】DMA驱动接口关闭失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEDMACLOSE            (lynERRNO(1) + lynMEMORY_OFFSET)    /*109001 DMA driver close fail */

/**
 * @def lynEDMAREAD
 * @brief DMA driver read fail 
 * 
 * 【错误码】109002
 * 
 * 【原因分析】1)DMA驱动接口从Server读数据到Client失败；2)Host或Device上内存耗光
 * 
 * 【解决方法】1) 联系技术支持；2) 确保有足够内存进行拷贝
 */
#define lynEDMAREAD             (lynERRNO(2) + lynMEMORY_OFFSET)    /*109002 DMA driver read fail */

/**
 * @def lynEDMAWRITE
 * @brief DMA驱动写入错误
 * 
 * 【错误码】109003
 * 
 * 【原因分析】1)DMA驱动接口从Client写数据到Server失败；2)Host或Device上内存耗光
 * 
 * 【解决方法】1) 联系技术支持；2) 确保有足够内存进行拷贝
 */
#define lynEDMAWRITE            (lynERRNO(3) + lynMEMORY_OFFSET)    /*109003 DMA driver write fail */

/**
 * @def lynEMEMORYHNDL
 * @brief 内存句柄错误
 * 
 * 【错误码】109004
 * 
 * 【原因分析】1)从已lynMalloc的内存handle中，通过devPtr没获取到对应的handle；2)传入的devPtr对应的内存已经被lynFree
 * 
 * 【解决方法】1) 检查传入的devPtr是否正确且未被lynFree；2) 联系技术支持
 */
#define lynEMEMORYHNDL          (lynERRNO(4) + lynMEMORY_OFFSET)    /*109004 memory handle error */

/**
 * @def lynEMEMORYADDR
 * @brief 内存地址获取错误
 * 
 * 【错误码】109005
 * 
 * 【原因分析】未能获取到正确的PhysAddr或者VirtAddr
 * 
 * 【解决方法】1) 检查传入的devPtr；2) 联系技术支持
 */
#define lynEMEMORYADDR          (lynERRNO(5) + lynMEMORY_OFFSET)    /*109005 memory address error */

/**
 * @def lynEMEMORYCROSS
 * @brief 内存复制越界错误
 * 
 * 【错误码】109006
 * 
 * 【原因分析】DMA拷贝时src的size大于dst的size
 * 
 * 【解决方法】检查传入接口中src和dst的size
 */
#define lynEMEMORYCROSS         (lynERRNO(6) + lynMEMORY_OFFSET)    /*109006 memory use out of bound */

/**
 * @def lynEALLOCATOROPEN
 * @brief 打开分配器错误
 * 
 * 【错误码】0x109007
 * 
 * 【原因分析】allocator驱动接口open失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEALLOCATOROPEN       (lynERRNO(7) + lynMEMORY_OFFSET)    /*109007 allocator driver handle open fail */

/**
 * @def lynEALLOCATORCLOSE
 * @brief 关闭分配器错误
 * 
 * 【错误码】109008
 * 
 * 【原因分析】allocator驱动接口close失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEALLOCATORCLOSE      (lynERRNO(8) + lynMEMORY_OFFSET)    /*109008 allocator driver handle close fail */

/**
 * @def lynEALLOCATORMMBMAL
 * @brief MMB分配内存错误
 * 
 * 【错误码】109009
 * 
 * 【原因分析】allocator驱动接口mmb malloc失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEALLOCATORMMBMAL     (lynERRNO(9) + lynMEMORY_OFFSET)    /*109009 allocator driver mmb malloc fail */

/**
 * @def lynEALLOCATORMMBFREE
 * @brief MMB释放内存错误
 * 
 * 【错误码】109010
 * 
 * 【原因分析】allocator驱动接口mmb free失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEALLOCATORMMBFREE    (lynERRNO(10) + lynMEMORY_OFFSET)   /*109010 allocator driver mmb free fail */

/**
 * @def lynEALLOCATORMMBVIR
 * @brief MMB获取虚拟内存错误
 * 
 * 【错误码】109011
 * 
 * 【原因分析】allocator驱动接口get mmb virtual address失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEALLOCATORMMBVIR     (lynERRNO(11) + lynMEMORY_OFFSET)   /*109011 allocator driver get mmb virtual address fail */

/**
 * @def lynEALLOCATORMMBREL
 * @brief MMB释放虚拟内存错误
 * 
 * 【错误码】109012
 * 
 * 【原因分析】allocator驱动接口release mmb virtual address失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEALLOCATORMMBREL     (lynERRNO(12) + lynMEMORY_OFFSET)   /*109012 allocator driver release mmb virtual address fail */

/**
 * @def lynEMEMSWAPSIZE
 * @brief 内存交换尺寸不同错误
 * 
 * 【错误码】109013
 * 
 * 【原因分析】memorySwap接口传入的两个devPtr对应的内存大小不同
 * 
 * 【解决方法】检查传入devPtr的内存大小
 */
#define lynEMEMSWAPSIZE         (lynERRNO(13) + lynMEMORY_OFFSET)   /*109013 memory swap size is diffrent */

/**
 * @def lynEMEMSWAPADDR
 * @brief 内存交换获取物理地址错误
 * 
 * 【错误码】109014
 * 
 * 【原因分析】memorySwap接口传入的两个devPtr无法获取正确的物理地址
 * 
 * 【解决方法】1) 检查devPtr是否lynFree；2) 联系技术支持
 */
#define lynEMEMSWAPADDR         (lynERRNO(14) + lynMEMORY_OFFSET)   /*109014 memory swap device address is error */

/**
 * @def lynESYSDMAOPEN
 * @brief sysdma接口打开错误
 * 
 * 【错误码】109015
 * 
 * 【原因分析】sysdma驱动接口open失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynESYSDMAOPEN          (lynERRNO(15) + lynMEMORY_OFFSET)   /*109015 SYSDMA driver open fail */

/**
 * @def lynESYSDMACOPY
 * @brief sysdma接口复制错误
 * 
 * 【错误码】109016
 * 
 * 【原因分析】sysdma驱动接口copy失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynESYSDMACOPY          (lynERRNO(16) + lynMEMORY_OFFSET)   /*109016 SYSDMA driver copy fail */

/**
 * @def lynESYSDMACLOSE
 * @brief sysdma接口关闭错误
 * 
 * 【错误码】109017
 * 
 * 【原因分析】sysdma驱动接口close失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynESYSDMACLOSE         (lynERRNO(17) + lynMEMORY_OFFSET)   /*109017 SYSDMA driver close fail */

/**
 * @def lynEBUFFEROVERLIMIT
 * @brief buffer数量超限错误
 * 
 * 【错误码】109018
 * 
 * 【原因分析】lynMalloc申请buffer数量在context上超过限制
 * 
 * 【解决方法】1) 检查lynMalloc是否频繁调用，且忘记lynFree；2) 联系技术支持
 */
#define lynEBUFFEROVERLIMIT     (lynERRNO(18) + lynMEMORY_OFFSET)   /*109018 memory buffer id counts in one context over 65536 */

/**
 * @def lynEWORKMODECALL
 * @brief 工作模式不支持P2P错误
 * 
 * 【错误码】109020
 * 
 * 【原因分析】当前工作模式不支持P2P拷贝
 * 
 * 【解决方法】请在约束范围内使用P2P
 */
#define lynEWORKMODECALL        (lynERRNO(19) + lynMEMORY_OFFSET)   /*109019 API unavaliable call in the work mode */

/**
 * @def lynEP2PDMAWRITE
 * @brief P2P传输写错误
 * 
 * 【错误码】109020
 * 
 * 【原因分析】P2P传输写错误
 * 
 * 【解决方法】1）检查板卡类型和操作系统；2) 联系技术支持
 */

#define lynEP2PDMAWRITE         (lynERRNO(20) + lynMEMORY_OFFSET)   /*109020 P2P DMA write fail */
/**
 * @def lynEP2PDMAREAD
 * @brief P2P传输读错误
 * 
 * 【错误码】109021
 * 
 * 【原因分析】lynMalloc申请buffer数量在context上超过限制
 * 
 * 【解决方法】1) 检查板卡类型和操作系统；2) 联系技术支持
 */
#define lynEP2PDMAREAD          (lynERRNO(21) + lynMEMORY_OFFSET)   /*109021 P2P DMA read fail */

/*util error number*/

/**
 * @def lynECOREINIT
 * @brief 核心初始化错误
 * 
 * 【错误码】110000
 * 
 * 【原因分析】板卡固件异常
 * 
 * 【解决方法】联系技术支持
 */
#define lynECOREINIT            (lynERRNO(0) + lynUTIL_OFFSET)      /*110000 core config init error*/

/**
 * @def lynECHILDPROC
 * @brief 无法创建子进程错误
 * 
 * 【错误码】110001
 * 
 * 【原因分析】板卡系统内存耗尽
 * 
 * 【解决方法】检查板卡内存使用情况
 */
#define lynECHILDPROC           (lynERRNO(1) + lynUTIL_OFFSET)      /*110001 failed to create child process */

/**
 * @def lynESCRIPTEXE
 * @brief 脚本执行错误
 * 
 * 【错误码】110002
 * 
 * 【原因分析】用户没有执行权限
 * 
 * 【解决方法】检查用户执行权限
 */
#define lynESCRIPTEXE           (lynERRNO(2) + lynUTIL_OFFSET)      /*110002 script execution failed */

/**
 * @def lynESCRIPTRETVAL
 * @brief 脚本执行结果错误
 * 
 * 【错误码】110003
 * 
 * 【原因分析】执行脚本程序失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynESCRIPTRETVAL        (lynERRNO(3) + lynUTIL_OFFSET)      /*110003 script return value failed */

/*ipc error number*/

/**
 * @def lynEIPCOPEN
 * @brief 打开IPC错误
 * 
 * 【错误码】111000
 * 
 * 【原因分析】未正确插入IPC驱动
 * 
 * 【解决方法】检查IPC驱动是否正确插入
 */
#define lynEIPCOPEN             (lynERRNO(0) + lynIPC_OFFSET)       /*111000 open ipc fail */

/**
 * @def lynEIPCCONNECT
 * @brief 连接IPC错误
 * 
 * 【错误码】111001
 * 
 * 【原因分析】建立Host到Device通信通道失败
 * 
 * 【解决方法】检查接口入参
 */
#define lynEIPCCONNECT          (lynERRNO(1) + lynIPC_OFFSET)       /*111001 connect ipc fail */

/**
 * @def lynEIPCUNCONNECT
 * @brief 销毁IPC链接错误
 * 
 * 【错误码】111002
 * 
 * 【原因分析】销毁Host到Device通信通道时失败
 * 
 * 【解决方法】检查接口入参
 */
#define lynEIPCUNCONNECT        (lynERRNO(2) + lynIPC_OFFSET)       /*111002 unconnect ipc fail */

/// \cond
/**
 * @def lynEIPCPREPARE
 * @brief 保留
 * 
 * 【错误码】111003
 * 
 * -
 */
#define lynEIPCPREPARE          (lynERRNO(3) + lynIPC_OFFSET)       /*111003 prepare ipc fail */


/**
 * @def lynEIPCUNPREPARE
 * @brief 保留
 * 
 * 【错误码】111004
 * 
 * -
 */
#define lynEIPCUNPREPARE        (lynERRNO(4) + lynIPC_OFFSET)       /*111004 unprepare ipc fail */
/// \endcond

/**
 * @def lynEIPCSEND
 * @brief IPC发送错误
 * 
 * 【错误码】111005
 * 
 * 【原因分析】通信通道断开
 * 
 * 【解决方法】尝试重新打开通信通道
 */
#define lynEIPCSEND             (lynERRNO(5) + lynIPC_OFFSET)       /*111005 ipc send data fail */

/**
 * @def lynEIPCRCV
 * @brief IPC接收错误
 * 
 * 【错误码】111006
 * 
 * 【原因分析】通信通道断开
 * 
 * 【解决方法】尝试重新打开通信通道
 */
#define lynEIPCRCV              (lynERRNO(6) + lynIPC_OFFSET)       /*111006 ipc receive data fail */

/**
 * @def lynEINVIPCOBJ
 * @brief IPC句柄错误
 * 
 * 【错误码】111007
 * 
 * 【原因分析】通信对象句柄失效
 * 
 * 【解决方法】确认通信对象句柄有效性
 */
#define lynEINVIPCOBJ           (lynERRNO(7) + lynIPC_OFFSET)       /*111007 invalid ipc obj */

/**
 * @def lynESYNCWALLTIME
 * @brief 同步壁钟时间失败
 * 
 * 【错误码】111008
 * 
 * 【原因分析】1)通信通道异常；2)板卡连接异常
 * 
 * 【解决方法】1)确认通信通道正常；2)确认板卡正常连接
 */
#define lynESYNCWALLTIME        (lynERRNO(8) + lynIPC_OFFSET)       /*111008 sync walltime fail */

/**
 * @def lynEGETMONODIFF
 * @brief 获取启动时间失败
 * 
 * 【错误码】111009
 * 
 * 【原因分析】1) 通信通道异常；2) 板卡连接异常
 * 
 * 【解决方法】1) 确认通信通道正常；2) 确认板卡正常连接
 */
#define lynEGETMONODIFF         (lynERRNO(9) + lynIPC_OFFSET)       /*111009 get the boot time fail */

/*device error number*/

/**
 * @def lynEDEVICECNT
 * @brief 获取Device数量错误
 * 
 * 【错误码】112000
 * 
 * 【原因分析】通过Device驱动接口获取Device数量失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEDEVICECNT           (lynERRNO(0) + lynDEVICE_OFFSET)    /*112000 device driver get count fail */

/**
 * @def lynEDEVICELIST
 * @brief 获取Device列表错误
 * 
 * 【错误码】112001
 * 
 * 【原因分析】通过Device驱动接口获取Device ID列表失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEDEVICELIST          (lynERRNO(1) + lynDEVICE_OFFSET)    /*112001 device driver get list fail */

/**
 * @def lynEDEVICEID
 * @brief 设备ID错误
 * 
 * 【错误码】112002
 * 
 * 【原因分析】传入的Device ID不是有效的
 * 
 * 【解决方法】检查Device ID的有效性
 */
#define lynEDEVICEID            (lynERRNO(2) + lynDEVICE_OFFSET)    /*112002 device id error */

/**
 * @def lynEDEVICESTATUS
 * @brief 获取设备状态错误
 * 
 * 【错误码】112003
 * 
 * 【原因分析】设备初始化失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEDEVICESTATUS        (lynERRNO(3) + lynDEVICE_OFFSET)    /*112003 device status fail */

/**
 * @def lynESWVER
 * @brief 获取软件版本错误
 * 
 * 【错误码】112004
 * 
 * 【原因分析】无法获取SDK接口版本
 * 
 * 【解决方法】联系技术支持
 */
#define lynESWVER               (lynERRNO(4) + lynDEVICE_OFFSET)    /*112004 get software version failed */

#define lynEDEVICEOPEN          (lynERRNO(5) + lynDEVICE_OFFSET)    /*112005 device driver handle open failed */

/*engine error number*/

/**
 * @def lynEENGINEINIT
 * @brief 初始化引擎错误
 * 
 * 【错误码】113000
 * 
 * 【原因分析】内部Engine初始化失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEENGINEINIT          (lynERRNO(0) + lynENGINE_OFFSET)    /*113000 engine init error*/

/**
 * @def lynEENGINEUNINIT
 * @brief 反初始化引擎错误
 * 
 * 【错误码】113001
 * 
 * 【原因分析】内部Engine反初始化失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEENGINEUNINIT        (lynERRNO(1) + lynENGINE_OFFSET)    /*113001 engine uninit error*/

/**
 * @def lynEENGINEREGISTER
 * @brief 引擎注册错误
 * 
 * 【错误码】113002
 * 
 * 【原因分析】内部引擎注册失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEENGINEREGISTER      (lynERRNO(2) + lynENGINE_OFFSET)    /*113002 engine register error*/

/*profiler error number*/

/**
 * @def lynETRACEFILEEXIST
 * @brief 采集文件已存在错误
 * 
 * 【错误码】114000
 * 
 * 【原因分析】trace-cmd保存trace文件失败，检查采集进程和目标进程是否有root权限。
 * 
 * 【解决方法】使用sudo运行采集进程和目标进程。
 */
#define lynETRACEFILEEXIST      (lynERRNO(0) + lynPROFILER_OFFSET)      /*114000 trace file exist */

/**
 * @def lynEENABLETRACE
 * @brief 使能采集错误
 * 
 * 【错误码】114001
 * 
 * 【原因分析】使能目标进程采集开关失败。检查指定的进程PID是否正确，或者开始采集时目标进程是否已经创建Context。
 * 
 * 【解决方法】输入正确的目标进程PID，确定目标进程创建Context之后才开采集。
 */
#define lynEENABLETRACE         (lynERRNO(1) + lynPROFILER_OFFSET)      /*114001 enable trace function failed */

/**
 * @def lynEDISABLETRACE
 * @brief 关闭采集错误
 * 
 * 【错误码】114002
 * 
 * 【原因分析】同 @ref lynEENABLETRACE 错误
 * 
 * 【解决方法】同 @ref lynEENABLETRACE 错误
 */
#define lynEDISABLETRACE        (lynERRNO(2) + lynPROFILER_OFFSET)      /*114002 disable trace function failed */

/**
 * @def lynETRACECMDEXE
 * @brief 采集命令执行错误
 * 
 * 【错误码】114003
 * 
 * 【原因分析】trace-cmd运行失败，建议检查是否正确安装trace-cmd
 * 
 * 【解决方法】安装trace-cmd程序
 */
#define lynETRACECMDEXE         (lynERRNO(3) + lynPROFILER_OFFSET)      /*114003 trace command execute failed */

/**
 * @def lynEPROFILERPARA
 * @brief 性能分析器参数错误
 * 
 * 【错误码】114004
 * 
 * 【原因分析】lynProfilerConfig配置错误
 * 
 * 【解决方法】检查相关参数是否在合理范围内
 */
#define lynEPROFILERPARA        (lynERRNO(4) + lynPROFILER_OFFSET)      /*114004 profiler params invalid */

/**
 * @def lynEPROFILERCALLSEQ
 * @brief 性能分析器调用顺序错误
 * 
 * 【错误码】114005
 * 
 * 【原因分析】Profiler API调用顺序错误
 * 
 * 【解决方法】检查API的调用顺序是否正确
 */
#define lynEPROFILERCALLSEQ     (lynERRNO(5) + lynPROFILER_OFFSET)      /*114005 profiler calling sequnence wrong */

/**
 * @def lynEDEVNOPROFILER
 * @brief 设备未运行性能分析器错误
 * 
 * 【错误码】114006
 * 
 * 【原因分析】指定的Device没有profiler模块运行
 * 
 * 【解决方法】确认目标Device是否创建了Context
 */
#define lynEDEVNOPROFILER       (lynERRNO(6) + lynPROFILER_OFFSET)      /*114006 profiler calling sequnence wrong */

/*plugin error number*/

/**
 * @def lynEREGISTER
 * @brief 插件注册错误
 * 
 * 【错误码】115000
 * 
 * 【原因分析】plugin注册失败
 * 
 * 【解决方法】1） 检查plugin文件是否存在，权限能否访问；2） 联系技术支持
 */
#define lynEREGISTER            (lynERRNO(0) + lynPLUGIN_OFFSET)        /*115000 plugin register error */

/**
 * @def lynELIBLOAD
 * @brief 加载插件错误
 * 
 * 【错误码】115001
 * 
 * 【原因分析】加载plugin失败
 * 
 * 【解决方法】1）查看插件库文件是否是ARM可执行；2）查看插件库是否完整
 */
#define lynELIBLOAD             (lynERRNO(1) + lynPLUGIN_OFFSET)        /*115001 load lib error */

/**
 * @def lynEFUNCLOAD
 * @brief 检索插件函数错误
 * 
 * 【错误码】115002
 * 
 * 【原因分析】加载plugin中函数失败
 * 
 * 【解决方法】确定插件库中存在该函数名
 */
#define lynEFUNCLOAD            (lynERRNO(2) + lynPLUGIN_OFFSET)        /*115002 can not find function in lib */

/**
 * @def lynEFUNCEXEC
 * @brief 插件函数执行错误
 * 
 * 【错误码】115003
 * 
 * 【原因分析】执行plugin中函数失败
 * 
 * 【解决方法】确定函数正常执行且返回值为0
 */
#define lynEFUNCEXEC            (lynERRNO(3) + lynPLUGIN_OFFSET)        /*115003 func exec error */
/// \cond
#define lynEDRAWTEXT            (lynERRNO(4) + lynPLUGIN_OFFSET)        /*115004 draw text error */
/// \endcond

/*mux error number*/
/// \cond
#define lynEMUXALLOCCTX         (lynERRNO(0) + lynMUX_OFFSET)           /*116000 alloc codec context error */
#define lynEMUXALLOCSTREAM      (lynERRNO(1) + lynMUX_OFFSET)           /*116001 alloc stream error. */
#define lynEMUXOPENCTX          (lynERRNO(2) + lynMUX_OFFSET)           /*116002 open codec context failed. */
#define lynEMUXWRHDR            (lynERRNO(3) + lynMUX_OFFSET)           /*116003 write file header error. */
#define lynEMUXSENDPKT          (lynERRNO(4) + lynMUX_OFFSET)           /*116004 mux send packet error. */
#define lynEMUXPIPE             (lynERRNO(5) + lynMUX_OFFSET)           /*116005 pipe broken error. */
#define lynEMUXNOTSUPPORT       (lynERRNO(6) + lynMUX_OFFSET)           /*116006 mux not support error. */
/// \endcond


/*rdma error number*/

/**
 * @def lynERDMAIBDEVICE
 * @brief IB设备错误
 * 
 * 【错误码】117000
 * 
 * 【原因分析】找不到正确的IB网卡设备或者打开失败
 * 
 * 【解决方法】1）检查IB板卡是否安装成功或者是否有指定名字的IB设备；2）联系技术支持
 */
#define lynERDMAIBDEVICE        (lynERRNO(0) + lynRDMA_OFFSET)           /*117000 no ib device or open failed*/

/**
 * @def lynERDMAIBCONNECT
 * @brief IB链接错误
 * 
 * 【错误码】117001
 * 
 * 【原因分析】IB连接失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynERDMAIBCONNECT       (lynERRNO(1) + lynRDMA_OFFSET)           /*117001 ib connect failed. */

/**
 * @def lynERDMATCPCONNECT
 * @brief TCP连接错误
 * 
 * 【错误码】117002
 * 
 * 【原因分析】TCP连接失败，首先检查是否有网络，其次大概率是IP端口被占用
 * 
 * 【解决方法】1）更换IP端口；2）联系技术支持
 */
#define lynERDMATCPCONNECT      (lynERRNO(2) + lynRDMA_OFFSET)           /*117002 tcp connect failed. */

/**
 * @def lynERDMAPD
 * @brief 保护域错误
 * 
 * 【错误码】117003
 * 
 * 【原因分析】分配/释放保护域（PD）失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynERDMAPD              (lynERRNO(3) + lynRDMA_OFFSET)           /*117003 pd error. */

/**
 * @def lynERDMACQ
 * @brief 完成队列错误
 * 
 * 【错误码】117004
 * 
 * 【原因分析】轮询完成队列中的任务执行结果失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynERDMACQ              (lynERRNO(4) + lynRDMA_OFFSET)           /*117004 cq error. */

/**
 * @def lynERDMAQP
 * @brief 队列对错误
 * 
 * 【错误码】117005
 * 
 * 【原因分析】QP创建/连接/状态切换失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynERDMAQP              (lynERRNO(5) + lynRDMA_OFFSET)           /*117005 qp error. */

/**
 * @def lynERDMAREGISTERMEMORY
 * @brief 内存注册错误
 * 
 * 【错误码】117006
 * 
 * 【原因分析】内存注册/取消注册失败
 * 
 * 【解决方法】1）确保内存指针是指定的设备上的；2）确保size大小在约束范围，或解除约束，具体操作参考11.4；3）联系技术支持
 */
#define lynERDMAREGISTERMEMORY  (lynERRNO(6) + lynRDMA_OFFSET)           /*117006 register/unregister memory failed.*/

/**
 * @def lynERDMAMMAP
 * @brief 内存映射错误
 * 
 * 【错误码】117007
 * 
 * 【原因分析】内存映射/取消映射失败
 * 
 * 【解决方法】1）驱动版本大于1.5.0，操作系统内核版本大于5；2）节点/dev/lynd/<device id>/pmem不存在，导致打开该节点失败。p2p以及RDMA功能依赖4G bar空间，当前仅HP系列板卡支持4G bar，HE200开发板不支持。请更换板卡；3）联系技术支持
 */
#define lynERDMAMMAP            (lynERRNO(7) + lynRDMA_OFFSET)           /*117007 mmap/munmap memory failed. */

/**
 * @def lynERDMAADDRESS
 * @brief 注册地址错误
 * 
 * 【错误码】117008
 * 
 * 【原因分析】无效的地址
 * 
 * 【解决方法】1）注册地址再使用；2）地址越界；3）联系技术支持
 */
#define lynERDMAADDRESS         (lynERRNO(8) + lynRDMA_OFFSET)           /*117008 invalid register address.*/

/**
 * @def lynERDMAPARAM
 * @brief RDMA参数错误
 * 
 * 【错误码】117009
 * 
 * 【原因分析】函数的参数有非法值
 * 
 * 【解决方法】1）用户保证输入正确的参数；2）联系技术支持
 */
#define lynERDMAPARAM           (lynERRNO(9) + lynRDMA_OFFSET)           /*117009 param error. */

/**
 * @def lynERDMA
 * @brief RDMA管理器或实例错误
 * 
 * 【错误码】117010
 * 
 * 【原因分析】创建Rdma引擎实例失败或者找不到Rdma引擎实例
 * 
 * 【解决方法】1）输入错误的handle；2）断开连接后依旧执行了Rdma操作；3）联系技术支持
 */
#define lynERDMA                (lynERRNO(10) + lynRDMA_OFFSET)          /*117010 not found rdma manager instance or rdma engine. */

/**
 * @def lynERDMANOTSUPPORT
 * @brief 系统不支持RDMA错误
 * 
 * 【错误码】117011
 * 
 * 【原因分析】系统架构不支持
 * 
 * 【解决方法】联系技术支持
 */
#define lynERDMANOTSUPPORT      (lynERRNO(11) + lynRDMA_OFFSET)          /*117011 current system arch not support lynxi rdma. */

/**
 * @def lynERDMATCPSYNC
 * @brief TCP数据交换错误
 * 
 * 【错误码】117012
 * 
 * 【原因分析】TCP控制连接的数据交换失败
 * 
 * 【解决方法】1）网络原因；2）相关接口未成对调用；3）联系技术支持
 */
#define lynERDMATCPSYNC         (lynERRNO(12) + lynRDMA_OFFSET)          /*117012 tcp exchange data failed. */

/**
 * @def lynERDMAVATOPA
 * @brief 虚拟地址到物理地址的转换错误
 * 
 * 【错误码】117013
 * 
 * 【原因分析】虚拟地址到物理地址的转换失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynERDMAVATOPA          (lynERRNO(13) + lynRDMA_OFFSET)          /*117013 virtual address to physical address failed. */

/**
 * @def lynERDMAPATOVA
 * @brief 物理地址到虚拟地址的转换错误
 * 
 * 【错误码】117014
 * 
 * 【原因分析】物理地址到虚拟地址的转换失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynERDMAPATOVA          (lynERRNO(14) + lynRDMA_OFFSET)          /*117014 physical address to virtual address failed. */

/**
 * @def lynERDMAMR
 * @brief RDMA内存错误
 * 
 * 【错误码】117015
 * 
 * 【原因分析】无效的Rdma内存区域
 * 
 * 【解决方法】联系技术支持
 */
#define lynERDMAMR              (lynERRNO(15) + lynRDMA_OFFSET)          /*117015 invalid mr. */

/**
 * @def lynERDMAIBPOSTSEND
 * @brief IB发送错误
 * 
 * 【错误码】117016
 * 
 * 【原因分析】IB接口发送数据失败
 * 
 * 【解决方法】1）本端无对端的访问权限；2）对端没有提前准备内存空间接收数据；3）对端没有足够的内存空间接收数据4）联系技术支持
 */
#define lynERDMAIBPOSTSEND      (lynERRNO(16) + lynRDMA_OFFSET)          /*117016 ib send failed. */

/**
 * @def lynERDMAIBPOSTRECV
 * @brief IB接收错误
 * 
 * 【错误码】117017
 * 
 * 【原因分析】IB接口接收数据失败
 * 
 * 【解决方法】1）本端没有提前准备内存空间接收数据；2）本端没有足够的内存空间接收数据；3）联系技术支持
 */
#define lynERDMAIBPOSTRECV      (lynERRNO(17) + lynRDMA_OFFSET)          /*117017 ib recv failed. */

/**
 * @def lynERDMADISCONNECT
 * @brief IB断连错误
 * 
 * 【错误码】117018
 * 
 * 【原因分析】断开连接失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynERDMADISCONNECT      (lynERRNO(18) + lynRDMA_OFFSET)          /*117018 ib disconnect. */
/// \cond
#define lynERDMADEVPTRTOVA      (lynERRNO(19) + lynRDMA_OFFSET)          /*117019 get map virtual address from devPtr failed. */
#define lynERDMAWAITTIMEOUT     (lynERRNO(20) + lynRDMA_OFFSET)          /*117020 wait timeout, the expected control information was not received*/
#define lynERDMARECVADDR        (lynERRNO(21) + lynRDMA_OFFSET)          /*117021 wait timeout, the expected remote addr information was not received*/
#define lynERDMASENDADDR        (lynERRNO(22) + lynRDMA_OFFSET)          /*117022 send remote addr information failed*/
#define lynERDMAPEERDISCONNECT  (lynERRNO(23) + lynRDMA_OFFSET)          /*117023 the opposite server is disconnected, resulting in an error in the local server */
#define lynERDMATHREAD          (lynERRNO(24) + lynRDMA_OFFSET)          /*117024 error in internal thread scheduling*/
#define lynERDMASYSTEM          (lynERRNO(25) + lynRDMA_OFFSET)          /*117025 system call error.*/
#define lynELCCLCONNECT         (lynERRNO(50) + lynRDMA_OFFSET)          /*117050 rdma lccl connect failed. */
#define lynELCCLCONFIG          (lynERRNO(51) + lynRDMA_OFFSET)          /*117051 rdma lccl incorrect configuration. */
#define lynELCCLFORK            (lynERRNO(52) + lynRDMA_OFFSET)          /*117052 failed to fork the process. */
#define lynELCCLCOMM            (lynERRNO(53) + lynRDMA_OFFSET)          /*117053 using a mismatched communication domain. */
#define lynELCCLGROUP           (lynERRNO(54) + lynRDMA_OFFSET)          /*117054 bad group operation. */
#define lynECCLBASE             (lynERRNO(200) + lynRDMA_OFFSET)         /*117200 the start error code of CCL */
/// \endcond


/*blas error number*/

/**
 * @def lynEBLASOPEN
 * @brief BLAS启动错误
 * 
 * 【错误码】118000
 * 
 * 【原因分析】APU启动失败
 * 
 * 【解决方法】联系技术支持
 */
#define lynEBLASOPEN        (lynERRNO(0) + lynBLAS_OFFSET)           /*118000 BLAS load model failed*/

/**
 * @def lynEBLASUninit
 * @brief BLAS反初始化错误
 * 
 * 【错误码】118001
 * 
 * 【原因分析】APU启动失败
 * 
 * 【解决方法】1）检查是否初始化Gemm运行环境；2）联系技术支持
 */
#define lynEBLASUninit      (lynERRNO(1) + lynBLAS_OFFSET)           /*118001 BLAS uninit failed*/

/**
 * @def lynEBLASNoInit
 * @brief BLAS未初始化错误
 * 
 * 【错误码】118002
 * 
 * 【原因分析】APU启动失败
 * 
 * 【解决方法】1）检查是否初始化Gemm运行环境；2）联系技术支持
 */
#define lynEBLASNoInit      (lynERRNO(2) + lynBLAS_OFFSET)           /*118002 BLAS not init now*/

/**
 * @def lynEBLASRUN
 * @brief BLAS加载与启动错误
 * 
 * 【错误码】118003
 * 
 * 【原因分析】加载模型并直接开始BLAS计算失败
 * 
 * 【解决方法】1）输入参数是否正确；2）联系技术支持
 */
#define lynEBLASRUN         (lynERRNO(3) + lynBLAS_OFFSET)           /*118003 BLAS run apu devices failed*/

/**
 * @def lynEBLASRCV
 * @brief BLAS计算错误
 * 
 * 【错误码】118004
 * 
 * 【原因分析】1）运算结果返回超时；2）运算状态返回异常
 * 
 * 【解决方法】联系技术支持
 */
#define lynEBLASRCV         (lynERRNO(4) + lynBLAS_OFFSET)           /*118004 BLAS rec apu devices failed*/

/**
 * @def lynEBLASSETPARAM
 * @brief BLAS参数设置错误 
 * 
 * 【错误码】118005
 * 
 * 【原因分析】Gemm模型配置未设置
 * 
 * 【解决方法】1）检查模型配置是否设置；2）联系技术支持
 */
#define lynEBLASSETPARAM    (lynERRNO(5) + lynBLAS_OFFSET)           /*118005 BLAS apu devices set param failed*/

/**
 * @typedef lynErrorMsg_t
 * @brief 错误信息结构体
 */
typedef struct lynErrorMsg_t {
    lynError_t errCode;     /** 错误码 */
    const char *errMsg;     /** 错误信息 */
    const char *errModule;  /** 错误所在模块 */
    const char *errFunction;/** 错误函数 */
    int32_t errDeviceId;    /** 错误所在设备 */
} lynErrorMsg_t;


#ifdef __cplusplus
}
#endif

#endif // LYN_ERR_C_H
