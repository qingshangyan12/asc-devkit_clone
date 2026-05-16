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
#include "kernel_operator_vec_duplicate_intf.h"
#endif

using DuplicateTensorFunc = void (*)(const AscendC::LocalTensor<float>&, const AscendC::LocalTensor<float>&,
    const int32_t&);
using InterleaveFunc = void (*)(const AscendC::LocalTensor<float>&, const AscendC::LocalTensor<float>&,
    const AscendC::LocalTensor<float>&, const AscendC::LocalTensor<float>&, const int32_t);
using DeInterleavePairFunc = void (*)(const AscendC::LocalTensor<float>&, const AscendC::LocalTensor<float>&,
    const AscendC::LocalTensor<float>&, const AscendC::LocalTensor<float>&, const int32_t);
using DeInterleaveSingleFunc = void (*)(const AscendC::LocalTensor<float>&, const AscendC::LocalTensor<float>&,
    const AscendC::LocalTensor<float>&, const int32_t);

CHECK_FUNC_EXISTS((*static_cast<DuplicateTensorFunc>(&AscendC::Duplicate<float>)));
CHECK_FUNC_EXISTS((*static_cast<InterleaveFunc>(&AscendC::Interleave<float>)));
CHECK_FUNC_EXISTS((*static_cast<DeInterleavePairFunc>(&AscendC::DeInterleave<float>)));
CHECK_FUNC_EXISTS((*static_cast<DeInterleaveSingleFunc>(&AscendC::DeInterleave<float>)));
