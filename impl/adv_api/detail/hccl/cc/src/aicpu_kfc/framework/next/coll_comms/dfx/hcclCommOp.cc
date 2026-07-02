/**
Â * Copyright (c) 2025 Huawei Technologies Co., Ltd.
Â * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
Â * CANN Open Software License Agreement Version 2.0 (the "License").
Â * Please refer to the License for details. You may not use this file except in compliance with the License.
Â * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
Â * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
Â * See LICENSE in the root of the software repository for the full text of the License.
Â */

#include "hcclCommOp.h"
#include "profiling_handler.h"

namespace hccl {

std::shared_ptr<Hccl::DfxOpInfo> ConvertToDfxOpInfo(const HcclDfxOpInfo& dfxOpInfo) {
    auto dfxOpInfoOnce = std::make_shared<Hccl::DfxOpInfo>();
    dfxOpInfoOnce->op_.opMode = static_cast<Hccl::OpMode::Value>(dfxOpInfo.opMode);
    dfxOpInfoOnce->op_.oldOpType = dfxOpInfo.opType; // å­˜A3çš„ç±»åž‹
    dfxOpInfoOnce->op_.oldReduceOp = dfxOpInfo.reduceOp; // å­˜A3çš„ç±»åž‹
    dfxOpInfoOnce->op_.oldDataType = dfxOpInfo.dataType; // å­˜A3çš„ç±»åž‹

    dfxOpInfoOnce->op_.dataCount = dfxOpInfo.dataCount;
    dfxOpInfoOnce->op_.root = dfxOpInfo.root;

    dfxOpInfoOnce->op_.newInputMem =  dfxOpInfo.inputMemAddr;
    dfxOpInfoOnce->op_.newOutputMem = dfxOpInfo.outputMemAddr;

    dfxOpInfoOnce->algTag_ = dfxOpInfo.algTag;
    dfxOpInfoOnce->beginTime_ = dfxOpInfo.beginTime;
    dfxOpInfoOnce->cpuWaitAicpuNotifyId_ = dfxOpInfo.cpuWaitAicpuNotifyId;

    return dfxOpInfoOnce;
}

}