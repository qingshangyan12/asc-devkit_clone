/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#include "host_checker.h"

#ifndef VERIFY_SINGLE_HEADER
#include "kernel_operator.h"
#else
#include "kernel_tpipe.h"
#endif

static_assert(sizeof(AscendC::TQueBind<AscendC::TPosition::VECIN, AscendC::TPosition::VECOUT, 1>) > 0,
    "AscendC::TQueBind is not defined");
static_assert(sizeof(AscendC::TQue<AscendC::TPosition::VECIN, 1>) > 0, "AscendC::TQue is not defined");
static_assert(sizeof(AscendC::TBuf<AscendC::TPosition::VECCALC>) > 0, "AscendC::TBuf is not defined");
static_assert(sizeof(AscendC::TBufPool<AscendC::TPosition::VECCALC>) > 0, "AscendC::TBufPool is not defined");
static_assert(sizeof(AscendC::TPipe) > 0, "AscendC::TPipe is not defined");
CHECK_FUNC_EXISTS(AscendC::UseBufIdSync<int32_t>);
