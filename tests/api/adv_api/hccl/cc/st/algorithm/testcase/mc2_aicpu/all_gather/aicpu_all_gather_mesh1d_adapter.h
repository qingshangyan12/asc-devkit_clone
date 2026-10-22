/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef AICPU_ST_ALL_GATHER_MESH1D_ADAPTER_H
#define AICPU_ST_ALL_GATHER_MESH1D_ADAPTER_H

namespace HcclSim {
namespace AicpuSt {

// 静态注册 InsAllGatherMesh1D 的 golden 构造器（当前与默认拼接语义一致，
// 显式注册以便后续变体（NHR 等）接入时模式统一）。
void RegisterAllGatherMesh1DVariant();

} // namespace AicpuSt
} // namespace HcclSim

#endif // AICPU_ST_ALL_GATHER_MESH1D_ADAPTER_H
