/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/


#include "kernel_operator.h"
#include "leaky_relu_custom_tiling.h"

constexpr int32_t BUFFER_NUM = 2;
constexpr int32_t UB_BLOCK_SIZE = 2048;

class KernelLeakyRelu {
public:
    __aicore__ inline KernelLeakyRelu() {}
    __aicore__ inline void Init(__gm__ uint8_t* x, __gm__ uint8_t* y, uint32_t totalLength, uint32_t tileNum, float negativeSlope)
    {
        ascendc_assert(tileNum != 0, "tileNum can not be zero.\n");
        this->blockLength = totalLength / AscendC::GetBlockNum();
        this->tileNum = tileNum;
        this->negativeSlope = static_cast<float>(negativeSlope);
        this->tileLength = this->blockLength / tileNum / BUFFER_NUM;

        xGm.SetGlobalBuffer((__gm__ float *)x + this->blockLength * AscendC::GetBlockIdx(), this->blockLength);
        yGm.SetGlobalBuffer((__gm__ float *)y + this->blockLength * AscendC::GetBlockIdx(), this->blockLength);
    }
    __aicore__ inline void Process()
    {
        int32_t loopCount = this->tileNum * BUFFER_NUM;

        AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
        AscendC::LocalTensor<float> xLocal = ubAllocator.Alloc<float, UB_BLOCK_SIZE>();
        AscendC::LocalTensor<float> yLocal = ubAllocator.Alloc<float, UB_BLOCK_SIZE>();
        AscendC::LocalTensor<float> tmpTensor1 = ubAllocator.Alloc<float, UB_BLOCK_SIZE>();
        AscendC::LocalTensor<float> tmpTensor2 = ubAllocator.Alloc<float, UB_BLOCK_SIZE>();

        float inputVal = 0.0f;

        for (int32_t i = 0; i < loopCount; i++) {
            AscendC::DataCopy(xLocal, xGm[i * this->tileLength], this->tileLength);

            AscendC::SetFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::MTE2_V>(EVENT_ID0);

            AscendC::Maxs(tmpTensor1, xLocal, inputVal, this->tileLength);
            AscendC::Mins(tmpTensor2, xLocal, inputVal, this->tileLength);
            AscendC::Muls(tmpTensor2, tmpTensor2, this->negativeSlope, this->tileLength);
            AscendC::Add(yLocal, tmpTensor1, tmpTensor2, this->tileLength);

            AscendC::SetFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);
            AscendC::WaitFlag<AscendC::HardEvent::V_MTE3>(EVENT_ID0);

            AscendC::DataCopy(yGm[i * this->tileLength], yLocal, this->tileLength);
        }
    }

private:
    AscendC::GlobalTensor<float> xGm;
    AscendC::GlobalTensor<float> yGm;
    uint32_t blockLength;
    uint32_t tileNum;
    uint32_t tileLength;
    float negativeSlope;
};

extern "C" __global__ __aicore__ void leaky_relu_custom(__gm__ uint8_t* x, __gm__ uint8_t* y, __gm__ uint8_t* workspace, __gm__ uint8_t* tiling)
{
    AscendC::InitSocState();

    REGISTER_TILING_DEFAULT(LeakyReluCustomTilingData);
    GET_TILING_DATA(tiling_data, tiling);

    KernelLeakyRelu op;
    op.Init(x, y, tiling_data.totalLength, tiling_data.tileNum, tiling_data.negativeSlope);
    op.Process();

    AscendC::PipeBarrier<PIPE_ALL>();
}