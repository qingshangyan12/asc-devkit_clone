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
#include "basic_api/reg_compute/kernel_reg_compute_vec_binary_intf.h"
#include "basic_api/reg_compute/kernel_reg_compute_vec_duplicate_intf.h"
#include "basic_api/reg_compute/kernel_reg_compute_datacopy_intf.h"
#include "basic_api/reg_compute/kernel_reg_compute_maskreg_intf.h"
#include "basic_api/reg_compute/kernel_reg_compute_vec_fused_intf.h"
using namespace std;
using namespace AscendC;

#define DType half
#define DType1 half
#define Mode 8

template <typename T, typename SCR1_T, int32_t MD>
class KernelBinary {
public:
    __aicore__ inline KernelBinary() {}
    __aicore__ inline void Init(
        GM_ADDR dst0_gm, GM_ADDR dst1_gm, GM_ADDR src0_gm, GM_ADDR src1_gm, uint32_t nums, uint32_t vec_mask)
    {
        src0_global.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(src0_gm), nums);
        src1_global.SetGlobalBuffer(reinterpret_cast<__gm__ SCR1_T*>(src1_gm), nums);
        dst0_global.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dst0_gm), nums);
        dst1_global.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dst1_gm), nums);

        pipe.InitBuffer(inQueueX, 1, nums * sizeof(T));
        pipe.InitBuffer(inQueueX2, 1, nums * sizeof(SCR1_T));
        pipe.InitBuffer(outQueue, 1, nums * sizeof(T));
        pipe.InitBuffer(outQueue2, 1, nums * sizeof(T));
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
    __aicore__ __simd_vf__ inline void ZeroInit(__ubuf__ T* dstPtr, __ubuf__ T* dst1Ptr, uint16_t one_rep_size,
        uint16_t rep)
    {
        Reg::RegTensor<T> vZeroReg;
        uint32_t sreg = (uint32_t)one_rep_size;
        Reg::MaskReg preg;
        preg = Reg::UpdateMask<T>(sreg);
        Reg::Duplicate(vZeroReg, (T)0, preg);
        for (uint16_t i = 0; i < (uint16_t)rep; i++) {
            Reg::StoreAlign<T>(dstPtr + i * one_rep_size, vZeroReg, preg);
            Reg::StoreAlign<T>(dst1Ptr + i * one_rep_size, vZeroReg, preg);
        }
    }

    __aicore__ __simd_vf__ inline void VecBinaryCompute(__ubuf__ T* dstPtr, __ubuf__ T* dst1Ptr,
        __ubuf__ T* src0Ptr, __ubuf__ SCR1_T* src1Ptr, uint16_t one_rep_size, uint16_t rep)
    {
        static constexpr Reg::DivSpecificMode divMode = {Reg::MaskMergeMode::ZEROING, true};
        Reg::RegTensor<T> vSrcReg0;
        Reg::RegTensor<SCR1_T> vSrcReg1;
        Reg::RegTensor<int32_t> vSrcReg2;
        Reg::RegTensor<T> vDstReg0;
        Reg::RegTensor<T> vDstReg1;
        Reg::RegTensor<float> fp32DstReg;
        vector_bool carryOut;
        vector_bool carrySrc;
        uint32_t sreg = (uint32_t)mask;
        Reg::MaskReg maskReg;
        maskReg = Reg::UpdateMask<T>(sreg);
        for (uint16_t i = 0; i < (uint16_t)rep; i++) {
            Reg::LoadAlign<T>(vSrcReg0, src0Ptr + i * one_rep_size);
            Reg::LoadAlign<SCR1_T>(vSrcReg1, src1Ptr + i * one_rep_size);
            if constexpr (MD == 0) {
                Reg::Add(vDstReg0, vSrcReg0, vSrcReg1, maskReg);
            } else if constexpr (MD == 1) {
                Reg::Sub(vDstReg0, vSrcReg0, vSrcReg1, maskReg);
            } else if constexpr (MD == 2) {
                Reg::Mul(vDstReg0, vSrcReg0, vSrcReg1, maskReg);
            } else if constexpr (MD == 3) {
                Reg::Div(vDstReg0, vSrcReg0, vSrcReg1, maskReg);
            } else if constexpr (MD == 4) {
                Reg::Max(vDstReg0, vSrcReg0, vSrcReg1, maskReg);
            } else if constexpr (MD == 5) {
                Reg::Min(vDstReg0, vSrcReg0, vSrcReg1, maskReg);
            } else if constexpr (MD == 9) {
                Reg::Or(vDstReg0, vSrcReg0, vSrcReg1, maskReg);
            } else if constexpr (MD == 12) {
                carryOut = pset_b8(PAT_ALL);
                vaddc(carryOut, vDstReg1, vSrcReg0, vSrcReg1, maskReg);
                psts(carryOut, (__ubuf__ uint32_t*)dst1Ptr, i * one_rep_size, NORM);
            } else if constexpr (MD == 13) {
                carryOut = pset_b8(PAT_ALL);
                vsubc(carryOut, vDstReg1, vSrcReg0, vSrcReg1, maskReg);
                psts(carryOut, (__ubuf__ uint32_t*)dst1Ptr, i * one_rep_size, NORM);
            } else if constexpr (MD == 14) {
                carryOut = pset_b8(PAT_ALL);
                vsubcs(carryOut, vDstReg1, vSrcReg0, vSrcReg1, carrySrc, maskReg);
                psts(carryOut, (__ubuf__ uint32_t*)dst1Ptr, i * one_rep_size, NORM);
            } else if constexpr (MD == 17) {
                Reg::Div<T, &divMode>(vDstReg0, vSrcReg0, vSrcReg1, maskReg);
            } else if constexpr (MD == 20) {
                Reg::ExpSub(vDstReg0, vSrcReg0, vSrcReg1, maskReg);
            } else if constexpr (MD == 22) {
                Reg::Prelu(vDstReg0, vSrcReg0, vSrcReg1, maskReg);
            } else if constexpr (MD == 23) {
                Reg::MulAddDst(vDstReg0, vSrcReg0, vSrcReg1, maskReg);
            }
            Reg::StoreAlign<T>(dstPtr + i * one_rep_size, vDstReg0, maskReg);
            if constexpr (MD == 15) {
                Reg::StoreAlign<T>(dst1Ptr + i * one_rep_size, vDstReg1, maskReg);
            }
        }
    }

    __aicore__ inline void CopyIn()
    {
        LocalTensor<T> src0Local = inQueueX.AllocTensor<T>();
        LocalTensor<SCR1_T> src1Local = inQueueX2.AllocTensor<SCR1_T>();
        DataCopy(src0Local, src0_global, dataSize);
        DataCopy(src1Local, src1_global, dataSize);
        inQueueX.EnQue(src0Local);
        inQueueX2.EnQue(src1Local);
    }
    __aicore__ inline void Compute()
    {
        LocalTensor<T> dst0Local = outQueue.AllocTensor<T>();
        LocalTensor<T> dst1Local = outQueue2.AllocTensor<T>();
        LocalTensor<T> src0Local = inQueueX.DeQue<T>();
        LocalTensor<SCR1_T> src1Local = inQueueX2.DeQue<SCR1_T>();
        uint16_t mask_bit_size = 256;
        uint16_t one_rep_size = mask_bit_size / sizeof(T);
        uint16_t rep = dataSize / one_rep_size;
        __ubuf__ T* dstPtr = (__ubuf__ T*)dst0Local.GetPhyAddr();
        __ubuf__ T* dst1Ptr = (__ubuf__ T*)dst1Local.GetPhyAddr();
        __ubuf__ T* src0Ptr = (__ubuf__ T*)src0Local.GetPhyAddr();
        __ubuf__ SCR1_T* src1Ptr = (__ubuf__ SCR1_T*)src1Local.GetPhyAddr();
        ZeroInit(dstPtr, dst1Ptr, one_rep_size, rep);
        VecBinaryCompute(dstPtr, dst1Ptr, src0Ptr, src1Ptr, one_rep_size, rep);
        outQueue.EnQue<T>(dst0Local);
        outQueue2.EnQue<T>(dst1Local);
        inQueueX.FreeTensor(src0Local);
        inQueueX2.FreeTensor(src1Local);
    }
    __aicore__ inline void CopyOut()
    {
        LocalTensor<T> dst0Local = outQueue.DeQue<T>();
        LocalTensor<T> dst1Local = outQueue2.DeQue<T>();
        DataCopy(dst0_global, dst0Local, dataSize);
        DataCopy(dst1_global, dst1Local, dataSize);
        outQueue.FreeTensor(dst0Local);
        outQueue2.FreeTensor(dst1Local);
    }

private:
    GlobalTensor<T> src0_global;
    GlobalTensor<SCR1_T> src1_global;
    GlobalTensor<T> dst0_global;
    GlobalTensor<T> dst1_global;

    TPipe pipe;
    TQue<TPosition::VECIN, 1> inQueueX;
    TQue<TPosition::VECIN, 1> inQueueX2;
    TQue<TPosition::VECOUT, 1> outQueue;
    TQue<TPosition::VECOUT, 1> outQueue2;
    uint32_t dataSize = 0;
    uint32_t mask;
};

struct MicroBinaryParams {
    void (*CallFunc)();
};

template <typename T, typename T2, int32_t mode>
void MicroBinaryRunCase()
{
    int byte_size = sizeof(T);
    int shape_size = 1024;
    int mask = 256;
    int dataSize = 1024;
    uint8_t dst0Gm[shape_size * byte_size] = {0};
    uint8_t dst1Gm[shape_size * byte_size] = {0};
    uint8_t src0Gm[shape_size * byte_size] = {0};
    uint8_t src1Gm[shape_size * byte_size] = {0};

    KernelBinary<T, T2, mode> op;
    op.Init(dst0Gm, dst1Gm, src0Gm, src1Gm, dataSize, mask);
    op.Process();
}

class MicroBinaryTestsuite : public testing::Test, public testing::WithParamInterface<MicroBinaryParams> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(
    MicroBinaryTestCase, MicroBinaryTestsuite,
    ::testing::Values(
        MicroBinaryParams{MicroBinaryRunCase<uint8_t, uint8_t, 0>},
        MicroBinaryParams{MicroBinaryRunCase<int8_t, int8_t, 0>},
        MicroBinaryParams{MicroBinaryRunCase<int32_t, int32_t, 1>},
        MicroBinaryParams{MicroBinaryRunCase<uint32_t, uint32_t, 1>},
        MicroBinaryParams{MicroBinaryRunCase<uint16_t, uint16_t, 1>},
        MicroBinaryParams{MicroBinaryRunCase<int16_t, int16_t, 1>},
        MicroBinaryParams{MicroBinaryRunCase<uint8_t, uint8_t, 1>},
        MicroBinaryParams{MicroBinaryRunCase<int8_t, int8_t, 1>},
        MicroBinaryParams{MicroBinaryRunCase<uint32_t, uint32_t, 2>},
        MicroBinaryParams{MicroBinaryRunCase<int32_t, int32_t, 2>},
        MicroBinaryParams{MicroBinaryRunCase<half, half, 3>}, MicroBinaryParams{MicroBinaryRunCase<float, float, 3>},
        MicroBinaryParams{MicroBinaryRunCase<int32_t, int32_t, 4>},
        MicroBinaryParams{MicroBinaryRunCase<uint32_t, uint32_t, 4>},
        MicroBinaryParams{MicroBinaryRunCase<uint16_t, uint16_t, 4>},
        MicroBinaryParams{MicroBinaryRunCase<int16_t, int16_t, 4>},
        MicroBinaryParams{MicroBinaryRunCase<uint8_t, uint8_t, 4>},
        MicroBinaryParams{MicroBinaryRunCase<int8_t, int8_t, 4>},
        MicroBinaryParams{MicroBinaryRunCase<int32_t, int32_t, 5>},
        MicroBinaryParams{MicroBinaryRunCase<uint32_t, uint32_t, 5>},
        MicroBinaryParams{MicroBinaryRunCase<uint16_t, uint16_t, 5>},
        MicroBinaryParams{MicroBinaryRunCase<int16_t, int16_t, 5>},
        MicroBinaryParams{MicroBinaryRunCase<uint8_t, uint8_t, 5>},
        MicroBinaryParams{MicroBinaryRunCase<int8_t, int8_t, 5>},
        MicroBinaryParams{MicroBinaryRunCase<int32_t, int32_t, 9>},
        MicroBinaryParams{MicroBinaryRunCase<uint32_t, uint32_t, 9>},
        MicroBinaryParams{MicroBinaryRunCase<uint16_t, uint16_t, 9>},
        MicroBinaryParams{MicroBinaryRunCase<int16_t, int16_t, 9>},
        MicroBinaryParams{MicroBinaryRunCase<uint8_t, uint8_t, 9>},
        MicroBinaryParams{MicroBinaryRunCase<int8_t, int8_t, 9>}, MicroBinaryParams{MicroBinaryRunCase<bool, bool, 9>},
        MicroBinaryParams{MicroBinaryRunCase<uint32_t, uint32_t, 13>},
        MicroBinaryParams{MicroBinaryRunCase<uint32_t, uint32_t, 14>},
        MicroBinaryParams{MicroBinaryRunCase<int32_t, int32_t, 13>},
        MicroBinaryParams{MicroBinaryRunCase<int32_t, int32_t, 14>},
        MicroBinaryParams{MicroBinaryRunCase<float, float, 22>}, MicroBinaryParams{MicroBinaryRunCase<half, half, 22>},
        MicroBinaryParams{MicroBinaryRunCase<int32_t, int32_t, 15>},
        MicroBinaryParams{MicroBinaryRunCase<int32_t, int32_t, 16>},
        MicroBinaryParams{MicroBinaryRunCase<int32_t, int32_t, 19>},
        MicroBinaryParams{MicroBinaryRunCase<uint32_t, uint32_t, 19>},
        MicroBinaryParams{MicroBinaryRunCase<int16_t, int16_t, 19>},
        MicroBinaryParams{MicroBinaryRunCase<uint16_t, uint16_t, 19>},
        MicroBinaryParams{MicroBinaryRunCase<float, float, 19>}, MicroBinaryParams{MicroBinaryRunCase<half, half, 19>},
        MicroBinaryParams{MicroBinaryRunCase<float, float, 20>}, MicroBinaryParams{MicroBinaryRunCase<half, half, 20>},
        MicroBinaryParams{MicroBinaryRunCase<half, half, 23>},
        MicroBinaryParams{MicroBinaryRunCase<float, float, 23>}));

TEST_P(MicroBinaryTestsuite, MicroBinaryTestCase)
{
    auto param = GetParam();
    param.CallFunc();
}
