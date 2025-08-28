/**
 * @file lyn_vdec_gdr.h
 * @brief //todo
 * \cond
 * @author Lynxi SDK Team
 * @version 1.0
 * @date 2022-11-02
 *
 * Copyright:
 * © 2018 北京灵汐科技有限公司 版权所有。
 * 注意：以下内容均为北京灵汐科技有限公司原创，未经本公司允许，不得转载，否则将视为侵权；对于不遵守此声明或者其他违法使用以下内容者，本公司依法保留追究权。
 * © 2018 Lynxi Technologies Co., Ltd. All rights reserved.
 * NOTICE: All information contained here is, and remains the property of Lynxi. This file can not be copied or distributed without the permission of Lynxi Technologies Co., Ltd.
 * \endcond
 */
#include "lyn_vdec.h"

#ifndef LYN_VDEC_GDR_C_H
#define LYN_VDEC_GDR_C_H
/**
 * @brief open gdr decoer
 *
 * @param  vdecHdl
 * @param  attr
 * @param  threadNum
 * @return lynError_t
 */
lynError_t lynVdecOpenGdr(lynVdecHandle_t *vdecHdl, const lynVdecAttr_t *attr, int threadNum);
#endif