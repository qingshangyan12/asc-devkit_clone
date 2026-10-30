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
#include "test_reg_compat.h"
#include "kernel_tensor.h"
#include "kernel_tpipe.h"
#include "basic_api/kernel_operator_data_copy_intf.h"
#include "basic_api/reg_compute/kernel_reg_compute_datacopy_intf.h"
#include "basic_api/reg_compute/kernel_reg_compute_vec_duplicate_intf.h"
#include "basic_api/reg_compute/kernel_reg_compute_maskreg_intf.h"
using namespace std;
using namespace AscendC;

#define DType half
#define DType1 half
#define Mode 8

template <typename T, typename IndexT>
class KernelScatter {
public:
    __aicore__ inline KernelScatter() {}
    __aicore__ inline void Init(GM_ADDR dst0_gm, GM_ADDR src0_gm, GM_ADDR src1_gm, uint32_t nums, uint32_t vec_mask)
    {
        src0_global.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(src0_gm), nums);
        src1_global.SetGlobalBuffer(reinterpret_cast<__gm__ IndexT*>(src1_gm), nums);
        dst0_global.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dst0_gm), nums);

        pipe.InitBuffer(inQueueX, 1, nums * sizeof(T));
        pipe.InitBuffer(inQueueX2, 1, nums * sizeof(IndexT));
        pipe.InitBuffer(outQueue, 1, nums * sizeof(T));
        dataSize = nums;
        mask = vec_mask;
    }
    __aicore__ inline void Process()
    {
        CopyIn();
        Compute();
        CopyOut();
    }

private:
    __aicore__ __simd_vf__ inline void ZeroInit(__ubuf__ T* dstPtr, uint16_t one_rep_size, uint16_t rep)
    {
        Reg::RegTensor<T> vZeroReg;
        uint32_t sreg = (uint32_t)one_rep_size;
        Reg::MaskReg preg;
        preg = Reg::UpdateMask<T>(sreg);
        Reg::Duplicate(vZeroReg, (T)0, preg);
        for (uint16_t i = 0; i < (uint16_t)rep; i++) {
            Reg::StoreAlign<T>(dstPtr + i * one_rep_size, vZeroReg, preg);
        }
    }

    __aicore__ __simd_vf__ inline void VecScatterCompute(__ubuf__ T* dstPtr, __ubuf__ T* src0Ptr,
        __ubuf__ IndexT* src1Ptr, uint16_t one_rep_size, uint16_t rep)
    {
        using Reg::RegTensor;
        RegTensor<T> vSrcReg;
        RegTensor<IndexT> vIndexReg;
        uint32_t sreg = (uint32_t)one_rep_size;
        Reg::MaskReg preg = Reg::UpdateMask<T>(sreg);
        for (uint16_t i = 0; i < (uint16_t)rep; i++) {
            Reg::LoadAlign<T>(vSrcReg, src0Ptr + i * one_rep_size);
            Reg::LoadAlign<IndexT>(vIndexReg, src1Ptr + i * one_rep_size);
            Reg::Scatter<T>(dstPtr + i * one_rep_size, vSrcReg, vIndexReg, preg);
        }
    }

    __aicore__ inline void CopyIn()
    {
        LocalTensor<T> src0Local = inQueueX.AllocTensor<T>();
        LocalTensor<IndexT> src1Local = inQueueX2.AllocTensor<IndexT>();
        DataCopy(src0Local, src0_global, dataSize);
        DataCopy(src1Local, src1_global, dataSize);
        inQueueX.EnQue(src0Local);
        inQueueX2.EnQue(src1Local);
    }
    __aicore__ inline void Compute()
    {
        LocalTensor<T> dst0Local = outQueue.AllocTensor<T>();
        LocalTensor<T> src0Local = inQueueX.DeQue<T>();
        LocalTensor<IndexT> src1Local = inQueueX2.DeQue<IndexT>();
        uint16_t mask_bit_size = 256;
        uint16_t one_rep_size = mask_bit_size / sizeof(T);
        uint16_t rep = dataSize / one_rep_size;
        __ubuf__ T* dstPtr = (__ubuf__ T*)dst0Local.GetPhyAddr();
        __ubuf__ T* src0Ptr = (__ubuf__ T*)src0Local.GetPhyAddr();
        __ubuf__ IndexT* src1Ptr = (__ubuf__ IndexT*)src1Local.GetPhyAddr();
        ZeroInit(dstPtr, one_rep_size, rep);
        VecScatterCompute(dstPtr, src0Ptr, src1Ptr, one_rep_size, rep);
        outQueue.EnQue<T>(dst0Local);
        inQueueX.FreeTensor(src0Local);
        inQueueX2.FreeTensor(src1Local);
    }
    __aicore__ inline void CopyOut()
    {
        LocalTensor<T> dst0Local = outQueue.DeQue<T>();
        DataCopy(dst0_global, dst0Local, dataSize);
        outQueue.FreeTensor(dst0Local);
    }

private:
    GlobalTensor<T> src0_global;
    GlobalTensor<IndexT> src1_global;
    GlobalTensor<T> dst0_global;

    TPipe pipe;
    TQue<TPosition::VECIN, 1> inQueueX;
    TQue<TPosition::VECIN, 1> inQueueX2;
    TQue<TPosition::VECOUT, 1> outQueue;
    uint32_t dataSize = 0;
    uint32_t mask;
};

struct MicroScatterParams {
    void (*CallFunc)();
};

template <typename T, typename IndexT>
void RunCase()
{
    int byte_size = sizeof(T);
    int shape_size = 1024;
    int mask = 256;
    int dataSize = 1024;
    uint8_t dst0Gm[shape_size * byte_size] = {0};
    uint8_t src0Gm[shape_size * byte_size] = {0};
    uint8_t src1Gm[shape_size * sizeof(IndexT)] = {0};

    KernelScatter<T, IndexT> op;
    op.Init(dst0Gm, src0Gm, src1Gm, dataSize, mask);
    op.Process();
}

class MicroScatterTestsuite : public testing::Test, public testing::WithParamInterface<MicroScatterParams> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(
    MicroScatterTestCase, MicroScatterTestsuite,
    ::testing::Values(
        MicroScatterParams{RunCase<int8_t, uint16_t>}, MicroScatterParams{RunCase<uint8_t, uint16_t>},
        MicroScatterParams{RunCase<int16_t, uint16_t>}, MicroScatterParams{RunCase<uint16_t, uint16_t>},
        MicroScatterParams{RunCase<int32_t, uint32_t>}, MicroScatterParams{RunCase<uint32_t, uint32_t>},
        MicroScatterParams{RunCase<half, uint16_t>}, MicroScatterParams{RunCase<float, uint32_t>}));

TEST_P(MicroScatterTestsuite, MicroScatterTestCase)
{
    auto param = GetParam();
    param.CallFunc();
}
