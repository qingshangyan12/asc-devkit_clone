/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_BASIC_API_DAV_9202_KERNEL_OPERATOR_FIXPIPE_IMPL_H
#define IMPL_BASIC_API_DAV_9202_KERNEL_OPERATOR_FIXPIPE_IMPL_H

#include "../dav_9201/kernel_operator_fixpipe_impl.h"

namespace AscendC {
// TODO: implement the 9202-specific MaxTrack configuration.
template <typename T, bool enableCompareWithMaxBuf, bool enableMaxOut2UB, bool enableAbsInput>
__aicore__ inline void SetFixPipeMaxTrackImpl(LocalTensor<T> rowOutput, LocalTensor<T> groupOutput, uint32_t groupN)
{}
} // namespace AscendC

#endif // IMPL_BASIC_API_DAV_9202_KERNEL_OPERATOR_FIXPIPE_IMPL_H
