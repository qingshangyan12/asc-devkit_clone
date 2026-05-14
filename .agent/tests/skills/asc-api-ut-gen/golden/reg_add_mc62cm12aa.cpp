/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/


#include <gtest/gtest.h>
#include <type_traits>
#include "kernel_operator.h"

using namespace std;
using namespace AscendC;


template <typename T, typename Src1T, int32_t mD>
class KernelRegAdd {
public:
    __aicore__ inline KernelRegAdd() {}

    __aicore__ inline void Init(GM_ADDR dstGm, GM_ADDR src0Gm, GM_ADDR src1Gm,
                                 uint32_t nums, uint32_t vecMask) {
        this->nums = nums;
        this->vecMask = vecMask;

        dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), nums);
        src0Global.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(src0Gm), nums);
        src1Global.SetGlobalBuffer(reinterpret_cast<__gm__ Src1T*>(src1Gm), nums);
    }

    __aicore__ inline void Process() {
        __VEC_SCOPE__ {
            // TODO: 实现寄存器级计算
        }
        CopyOut();
    }

private:
    __aicore__ inline void CopyOut() {
        for (uint32_t i = 0; i < nums; i++) {
            dstGlobal.SetValue(i, src0Global.GetValue(i));
        }
    }

    GlobalTensor<T> dstGlobal;
    GlobalTensor<T> src0Global;
    GlobalTensor<Src1T> src1Global;
    uint32_t nums;
    uint32_t vecMask;
};



class TEST_REGADD : public testing::Test {
protected:
    void SetUp() { AscendC::SetGCoreType(2); }
    void TearDown() { AscendC::SetGCoreType(0); }
};



TEST_F(TEST_REGADD, RegAdd_Basic) {
    constexpr int32_t dataSize = 256;

    uint8_t dstGm[dataSize * sizeof(half)] = {0};
    uint8_t src0Gm[dataSize * sizeof(half)] = {0};
    uint8_t src1Gm[dataSize * sizeof(half)] = {0};

    // 初始化输入数据
    for (int i = 0; i < dataSize; i++) {
        reinterpret_cast<half*>(src0Gm)[i] = static_cast<half>(i % 100);
        reinterpret_cast<half*>(src1Gm)[i] = static_cast<half>((i + 1) % 100);
    }

    // 执行 Kernel
    KernelRegAdd<half, half, 1> op;
    op.Init(dstGm, src0Gm, src1Gm, dataSize, 256);
    op.Process();

    // 验证结果
    // TODO: 添加验证逻辑
}

