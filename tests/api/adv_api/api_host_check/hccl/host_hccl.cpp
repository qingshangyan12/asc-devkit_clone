/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "hccl/hccl.h"

__aicore__ void TestHcclAllReduce0() {
    AscendC::Hccl<Hccl::MatmulConfig> obj;
    obj.AllReduce(sendBuf, recvBuf, count, dataType, op, repeat);
}

__aicore__ void TestHcclAllGather1() {
    AscendC::Hccl<Hccl::MatmulConfig> obj;
    obj.AllGather(sendBuf, recvBuf, sendCount, dataType, strideCount, repeat);
}

__aicore__ void TestHcclReduceScatter2() {
    AscendC::Hccl<Hccl::MatmulConfig> obj;
    obj.ReduceScatter(sendBuf, recvBuf, recvCount, dataType, op, strideCount, repeat);
}

__aicore__ void TestHcclInit3() {
    AscendC::Hccl<Hccl::MatmulConfig> obj;
    obj.Init(context, initTiling);
}

__aicore__ void TestHcclCommit4() {
    AscendC::Hccl<Hccl::MatmulConfig> obj;
    obj.Commit(handleId);
}

__aicore__ void TestHcclWait5() {
    AscendC::Hccl<Hccl::MatmulConfig> obj;
    obj.Wait(handleId);
}

__aicore__ void TestHcclFinalize6() {
    AscendC::Hccl<Hccl::MatmulConfig> obj;
    obj.Finalize();
}

__aicore__ void TestHcclGetRankId7() {
    AscendC::Hccl<Hccl::MatmulConfig> obj;
    obj.GetRankId();
}

