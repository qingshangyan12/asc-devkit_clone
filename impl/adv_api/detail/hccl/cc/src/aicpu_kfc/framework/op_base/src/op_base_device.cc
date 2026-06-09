/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include <algorithm>
#include <future>
#include <map>
#include <string>
#include "../../../../../../../../../../include/adv_api/hccl/internal/hcomm/hccl/hccl_types.h"

#include "../../../../../../../../../../include/adv_api/hccl/internal/hcomm/pkg_inc/hccl/base.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/pub_inc/workflow_pub.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/framework/common/src/param_check_pub.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/common/health/rank_consistentcy_checker.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/pub_inc/externalinput_pub.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/framework/common/src/config/env_config.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/framework/cluster_maintenance/detect/detect_connect_anomalies/detect_connect_anomalies.h"
#include "../common/src/topo/topoinfo_detect.h"
#include "../common/src/topo/topoinfo_ranktable_partition.h"
#include "../common/src/state_guard.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/pub_inc/sal_pub.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/common/debug/profiling/inc/profiling_manager_pub.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/common/debug/profiling/inc/adapter_prof.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/pub_inc/adapter_rts_common.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/pub_inc/device_capacity.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/pub_inc/mem_host_pub.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/framework/hcom/hcom_common.h"
#include "impl/adv_api/detail/hccl/cc/src/aicpu_kfc/framework/inc/comm_config_pub.h"
#include "kernel_tiling/kernel_tiling.h"
#include "error_codes/rt_error_codes.h"
#include "mmpa_api.h"
#include "op_base.h"
#include "op_base_v2.h"

using namespace std;
using namespace hccl;

HcclResult GetCaptureInfo(aclrtStream stream, aclmdlRICaptureStatus &captureStatus, uint64_t &modelId, bool &isCapture)
{
   HCCL_WARNING("[%s]Stream capture does not support!", __func__);
   return HCCL_SUCCESS;
}

HcclResult HcclAllReduceInner(void *sendBuf, void *recvBuf, uint64_t count, HcclDataType dataType,
                         HcclReduceOp op, HcclComm comm, aclrtStream stream)
{
   HCCL_WARNING("[%s]HcclAllReduceInner does not support!", __func__);
   return HCCL_SUCCESS;
}
