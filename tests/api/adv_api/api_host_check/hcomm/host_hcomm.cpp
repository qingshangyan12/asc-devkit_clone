/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include "hcomm/hcomm.h"

void TestHcommWrite0() {
    AscendC::Hcomm<Hcomm::MatmulConfig> obj;
    obj.Write(channelHandle, dst, src, len);
}

void TestHcommRead1() {
    AscendC::Hcomm<Hcomm::MatmulConfig> obj;
    obj.Read(channelHandle, dst, src, len);
}

void TestHcommCommit2() {
    AscendC::Hcomm<Hcomm::MatmulConfig> obj;
    obj.Commit(handleId);
}

void TestHcommWait3() {
    AscendC::Hcomm<Hcomm::MatmulConfig> obj;
    obj.Wait(handleId);
}

