/**
 * Copyright (c) 2026 ddplys.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "basic_api/kernel_tpipe.h"
#include <cstdio>

int main()
{
    AscendC::TPipe pipe;
    AscendC::TBuf<AscendC::TPosition::VECCALC> ub;
    AscendC::TBuf<AscendC::TPosition::A1> l1;
    pipe.InitBuffer(ub, 256);
    pipe.InitBuffer(l1, 256);
    auto u = ub.Get<float>();
    auto c = l1.Get<half>();
    for (int i : {0, 1, 7, 31}) {
        auto ut = u[i];
        auto ct = c[i];
        auto up = reinterpret_cast<float*>(ut.GetPhyAddr());
        auto cp = reinterpret_cast<half*>(ct.GetPhyAddr());
        if (pipe.GetAbsAddr(up) != pipe.GetAbsAddr(ut) ||
            AscendC::GetAbsAddr(&pipe, up) != AscendC::GetAbsAddr(&pipe, ut) ||
            pipe.GetAbsAddr<half, AscendC::Hardware::L1>(cp) != pipe.GetAbsAddr(ct) ||
            AscendC::GetAbsAddr<half, AscendC::Hardware::L1>(&pipe, cp) != AscendC::GetAbsAddr(&pipe, ct)) {
            std::printf("FAIL GetAbsAddr offset=%d\n", i);
            return 1;
        }
    }
    std::puts("PASS GetAbsAddr CPU_DEBUG member/free UB/L1 offsets=0,1,7,31");
}
