/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "hccl_alloc_ctx_res.h"
#ifndef ASC_MC2_PROFILING_BINDING_H
#define ASC_MC2_PROFILING_BINDING_H

#include "alg_param.h"
#include "hccl_res_expt.h"

// Host-only, communicator-owned snapshots; no serialized algorithm ABI changes.
HcclResult SaveMc2AlgorithmThreads(HcclComm comm, const mc2_ops_hccl::OpParam& param,
                                  const std::vector<ThreadHandle>& threads);
HcclResult SaveMc2ProfilingResource(HcclComm comm, const void* opResCtx,
                                  const std::vector<mc2_ops_hccl::OpParam>& params);

#endif
