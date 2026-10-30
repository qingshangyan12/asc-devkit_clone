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
#include "basic_api/reg_compute/kernel_reg_compute_vec_reduce_intf.h"
#include "basic_api/reg_compute/kernel_reg_compute_vec_duplicate_intf.h"
#include "basic_api/reg_compute/kernel_reg_compute_maskreg_intf.h"
using namespace std;
using namespace AscendC;

#define DType half
#define DType1 half
#define Mode 8

template <typename DstT, typename SrcT, int32_t MD>
class KernelReduce {
public:
    __aicore__ inline KernelReduce() {}
    __aicore__ inline void Init(
        GM_ADDR dst0_gm, GM_ADDR dst1_gm, GM_ADDR src0_gm, GM_ADDR src1_gm, uint32_t nums, uint32_t vec_mask)
    {
        src0_global.SetGlobalBuffer(reinterpret_cast<__gm__ SrcT*>(src0_gm), nums);
        dst0_global.SetGlobalBuffer(reinterpret_cast<__gm__ DstT*>(dst0_gm), nums);

        pipe.InitBuffer(inQueueX, 1, nums * sizeof(SrcT));
        pipe.InitBuffer(outQueue, 1, nums * sizeof(DstT));
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
    __aicore__ __simd_vf__ inline void ZeroInit(__ubuf__ DstT* dstPtr, uint16_t one_rep_size, uint16_t rep)
    {
        Reg::RegTensor<DstT> vZeroReg;
        uint32_t sreg = (uint32_t)one_rep_size;
        Reg::MaskReg preg;
        preg = Reg::UpdateMask<DstT>(sreg);
        Reg::Duplicate(vZeroReg, (DstT)0, preg);
        for (uint16_t i = 0; i < (uint16_t)rep; i++) {
            Reg::StoreAlign<DstT>(dstPtr + i * one_rep_size, vZeroReg, preg);
        }
    }

    __aicore__ __simd_vf__ inline void VecReduceCompute(
        __ubuf__ DstT* dstPtr, __ubuf__ SrcT* src0Ptr, uint16_t one_rep_size, uint16_t rep)
    {
        using Reg::RegTensor;
        RegTensor<SrcT> vSrcReg0;
        RegTensor<DstT> vDstReg0;
        uint32_t sreg = (uint32_t)mask;
        Reg::MaskReg maskReg;
        maskReg = Reg::UpdateMask<SrcT>(sreg);
        for (uint16_t i = 0; i < (uint16_t)rep; i++) {
            Reg::LoadAlign<SrcT>(vSrcReg0, src0Ptr + i * one_rep_size);
            if constexpr (MD == 0) {
                Reg::Reduce<ReduceType::SUM, DstT, SrcT>(vDstReg0, vSrcReg0, maskReg);
            } else if constexpr (MD == 1) {
                Reg::Reduce<ReduceType::MAX, DstT>(vDstReg0, vSrcReg0, maskReg);
            } else if constexpr (MD == 2) {
                Reg::Reduce<ReduceType::MIN, DstT>(vDstReg0, vSrcReg0, maskReg);
            }
            Reg::StoreAlign<DstT>(dstPtr + i * one_rep_size, vDstReg0, maskReg);
        }
    }

    __aicore__ inline void CopyIn()
    {
        LocalTensor<SrcT> src0Local = inQueueX.AllocTensor<SrcT>();
        DataCopy(src0Local, src0_global, dataSize);
        inQueueX.EnQue(src0Local);
    }
    __aicore__ inline void Compute()
    {
        LocalTensor<DstT> dst0Local = outQueue.AllocTensor<DstT>();
        LocalTensor<SrcT> src0Local = inQueueX.DeQue<SrcT>();
        uint16_t mask_bit_size = 256;
        uint16_t one_rep_size = mask_bit_size / sizeof(SrcT);
        uint16_t rep = dataSize / one_rep_size;
        __ubuf__ DstT* dstPtr = (__ubuf__ DstT*)dst0Local.GetPhyAddr();
        __ubuf__ SrcT* src0Ptr = (__ubuf__ SrcT*)src0Local.GetPhyAddr();
        ZeroInit(dstPtr, one_rep_size, rep);
        VecReduceCompute(dstPtr, src0Ptr, one_rep_size, rep);
        outQueue.EnQue<DstT>(dst0Local);
        inQueueX.FreeTensor(src0Local);
    }
    __aicore__ inline void CopyOut()
    {
        LocalTensor<DstT> dst0Local = outQueue.DeQue<DstT>();
        DataCopy(dst0_global, dst0Local, dataSize);
        outQueue.FreeTensor(dst0Local);
    }

private:
    GlobalTensor<SrcT> src0_global;
    GlobalTensor<DstT> dst0_global;

    TPipe pipe;
    TQue<TPosition::VECIN, 1> inQueueX;
    TQue<TPosition::VECOUT, 1> outQueue;
    uint32_t dataSize = 0;
    uint32_t mask;
};

struct MicroReduceParams {
    void (*CallFunc)();
};

template <typename DstT, typename SrcT, int32_t mode>
void RunCase()
{
    int byte_size = sizeof(DstT);
    int shape_size = 1024;
    int mask = 256;
    int dataSize = 1024;
    uint8_t dst0Gm[shape_size * byte_size] = {0};
    uint8_t dst1Gm[shape_size * byte_size] = {0};
    uint8_t src0Gm[shape_size * sizeof(SrcT)] = {0};
    uint8_t src1Gm[shape_size * byte_size] = {0};

    KernelReduce<DstT, SrcT, mode> op;
    op.Init(dst0Gm, dst1Gm, src0Gm, src1Gm, dataSize, mask);
    op.Process();
}

class MicroReduceTestsuite : public testing::Test, public testing::WithParamInterface<MicroReduceParams> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(
    MicroReduceTestCase, MicroReduceTestsuite,
    ::testing::Values(
        MicroReduceParams{RunCase<half, half, 0>}, MicroReduceParams{RunCase<int32_t, int32_t, 0>},
        MicroReduceParams{RunCase<uint32_t, uint32_t, 0>}, MicroReduceParams{RunCase<int32_t, int16_t, 0>},
        MicroReduceParams{RunCase<uint32_t, uint16_t, 0>}, MicroReduceParams{RunCase<float, float, 0>},
        MicroReduceParams{RunCase<half, half, 1>}, MicroReduceParams{RunCase<int32_t, int32_t, 1>},
        MicroReduceParams{RunCase<uint32_t, uint32_t, 1>}, MicroReduceParams{RunCase<int16_t, int16_t, 1>},
        MicroReduceParams{RunCase<uint16_t, uint16_t, 1>}, MicroReduceParams{RunCase<float, float, 1>},
        MicroReduceParams{RunCase<half, half, 2>}, MicroReduceParams{RunCase<int32_t, int32_t, 2>},
        MicroReduceParams{RunCase<uint32_t, uint32_t, 2>}, MicroReduceParams{RunCase<int16_t, int16_t, 2>},
        MicroReduceParams{RunCase<uint16_t, uint16_t, 2>}, MicroReduceParams{RunCase<float, float, 2>}));

TEST_P(MicroReduceTestsuite, MicroReduceTestCase)
{
    auto param = GetParam();
    param.CallFunc();
}
