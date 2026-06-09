/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef OP_COMMON_OPS_H
#define OP_COMMON_OPS_H

#include "impl/adv_api/detail/hccl/cc/src/common/adapter_acl.h"
#include "adapter_error_manager_pub.h"
#include "alg_env_config.h"
#include "impl/adv_api/detail/hccl/cc/src/ops/op_common/executor/executor_base.h"
#include "hccl.h"
#include "../../../../../../../../include/adv_api/hccl/internal/hcomm/pkg_inc/hccl/base.h"
#include "../../../../../../../../include/adv_api/hccl/internal/hcomm/pkg_inc/hccl/hccl_inner.h"
#include "mmpa_api.h"
#include "op_common.h"
#include "param_check.h"
#include "sal.h"
#include "../../../../../../../../include/adv_api/hccl/internal/hcomm/pkg_inc/hccl/workflow.h"
#include "../../../../../../../../include/adv_api/hccl/internal/hcomm/hccl/hccl_types.h"
#include "impl/adv_api/detail/hccl/cc/src/common/hcomm_dlsym/hcomm_dlsym.h"
#include "../../../../../../../../include/adv_api/hccl/internal/hcomm/pkg_inc/hccl/hcom.h"

#endif