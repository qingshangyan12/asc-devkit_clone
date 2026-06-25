/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file conv2d_intr.h
 * \brief
 */

#ifndef API_CONV2D_INSTR_H
#define API_CONV2D_INSTR_H

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165)
    #include "dav_5101/conv2d_instr_impl.h"
    #include "dav_5101/conv2d_instr_m_mode_impl.h"
    #include "dav_5101/conv2d_instr_hw_mode_impl.h"
#endif

#endif // API_CONV2D_INSTR_H
