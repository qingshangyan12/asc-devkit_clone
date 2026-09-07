/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "kernel_tensor.h"

using AscendC::GlobalTensor;

__aicore__ int32_t GmCheckCase(int32_t testCase, __gm__ int32_t* buffer)
{
    GlobalTensor<int32_t> gm;
    const auto& constGm = gm;
    switch (testCase) {
        case 0:
            return gm.address_ == nullptr && gm.oriAddress_ == nullptr ? 0 : 1;
        case 1:
            return gm.GetValue(17);
        case 2:
            return constGm.GetValue(17);
        case 3:
            gm.SetValue(17, 7);
            return 0;
        case 4:
            return gm(17);
        case 5:
            return constGm(17);
        case 6:
            return gm[17].GetValue(0);
        case 7:
            gm.SetAddr(17);
            return gm.GetValue(0);
        case 8: {
            GlobalTensor<int32_t> other;
            other.SetGlobalBuffer(gm.GetPhyAddr(17));
            return other.GetValue(0);
        }
        case 9: {
            auto copy = gm;
            return copy.GetValue(0);
        }
        case 10:
            gm.SetGlobalBuffer(nullptr, 32);
            return gm.GetValue(1);
        case 11:
            gm.SetGlobalBuffer(nullptr);
            gm.SetValue(1, 7);
            return 0;
        case 12:
            gm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
            return gm.GetValue(0);
        case 13: {
            gm.SetGlobalBuffer(buffer, 32);
            gm.SetValue(0, 7);
            gm.SetValue(16, 9);
            if (gm.GetValue(0) != 7 || constGm.GetValue(16) != 9 || gm(0) != 7 || constGm(16) != 9) {
                return 1;
            }
            auto view = gm[16];
            if (view.GetValue(0) != 9) {
                return 2;
            }
            auto copy = gm;
            copy.SetAddr(16);
            if (copy.GetValue(0) != 9 || *gm.GetPhyAddr(16) != 9) {
                return 3;
            }
            gm.SetGlobalBuffer(buffer + 16);
            gm.SetValue(1, 11);
            return gm.GetValue(1) == 11 ? 0 : 4;
        }
        case 14: {
            GlobalTensor<int4b_t> packed;
            return packed.GetValue(3).storage;
        }
        case 15: {
            GlobalTensor<int4b_t> packed;
            int4b_t value;
            value.storage = 1;
            packed.SetValue(3, value);
            return 0;
        }
        case 16: {
            GlobalTensor<int32_t> array[2];
            return array[1].GetValue(0);
        }
        case 17: {
            struct Holder {
                GlobalTensor<int32_t> member;
            } holder;
            holder.member.SetValue(0, 1);
            return 0;
        }
#if (__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102)
        case 18:
            return gm.ReinterpretCast<uint32_t>().GetValue(0);
#endif
        default:
            return 5;
    }
}

// Keep separate symbols for the unbound read and write compilation checks.
extern "C" __aicore__ int32_t GmCheckUnboundRead()
{
    GlobalTensor<int32_t> gm;
    return gm.GetValue(17);
}

extern "C" __aicore__ void GmCheckUnboundWrite()
{
    GlobalTensor<int32_t> gm;
    gm.SetValue(17, 7);
}
