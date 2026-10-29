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
#include "test_reg_compat.h"
#include "kernel_tensor.h"
#include "kernel_tpipe.h"
#include "basic_api/kernel_operator_data_copy_intf.h"
#include "basic_api/reg_compute/kernel_reg_compute_maskreg_intf.h"
#include "basic_api/reg_compute/kernel_reg_compute_vec_binary_scalar_intf.h"
#include "basic_api/reg_compute/kernel_reg_compute_datacopy_intf.h"
#include "basic_api/kernel_operator_utils_intf.h"

using namespace std;
using namespace AscendC;

template <typename T, int Mode>
class KernelMicroMask {
public:
    __aicore__ inline KernelMicroMask() {}
    __aicore__ inline void Init(GM_ADDR srcGm, GM_ADDR dstGm, uint32_t count)
    {
        const int alginSize = 32 / sizeof(T);
        dstSize = (count + alginSize - 1) / alginSize * alginSize;
        srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dstSize);
        dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dstSize);
        pipe.InitBuffer(inQueue, 1, dstSize * sizeof(T));
        pipe.InitBuffer(outQueue, 1, dstSize * sizeof(T));
    }

    __aicore__ inline void Process()
    {
        CopyIn();
        Compute();
        CopyOut();
    }

private:
    __aicore__ __simd_vf__ inline void VecMaskCompute(__ubuf__ T* dst, __ubuf__ T* src)
    {
        if constexpr (Mode < 100) {
            ComputeMode0(dst, src);
        }
    }

    __aicore__ inline void CopyIn()
    {
        LocalTensor<T> srcLocal = inQueue.AllocTensor<T>();
        DataCopy(srcLocal, srcGlobal, dstSize);
        inQueue.EnQue<T>(srcLocal);
    }

    __aicore__ inline void ComputeMode0(__ubuf__ T* dst, __ubuf__ T* src)
    {
        Reg::RegTensor<T> vreg0;
        uint32_t sreg = (uint32_t)dstSize;
        // Pat { ALL, VL1, VL2, VL3, VL4, VL8, VL16, VL32, VL64, VL128, M3, M4, H, Q, ALLF = 15 }
        Reg::MaskReg preg = Reg::CreateMask<T, static_cast<Reg::MaskPattern>(Mode)>();
        constexpr uint32_t sregLower = (uint32_t)(VECTOR_REG_WIDTH / sizeof(T));
        uint16_t repeatTimes = CeilDivision(dstSize, sregLower);
        for (uint16_t i = 0; i < (uint16_t)repeatTimes; ++i) {
            Reg::MaskReg cur = Reg::UpdateMask<T>(sreg);
            Reg::LoadAlign<T>(vreg0, src + i * sregLower);
            Adds(vreg0, vreg0, 0, preg);
            Reg::StoreAlign<T>(dst + i * sregLower, vreg0, cur);
        }
    }

    __aicore__ inline void Compute()
    {
        LocalTensor<T> dstLocal = outQueue.AllocTensor<T>();
        LocalTensor<T> srcLocal = inQueue.DeQue<T>();

        __ubuf__ T* src = (__ubuf__ T*)srcLocal.GetPhyAddr();
        __ubuf__ T* dst = (__ubuf__ T*)dstLocal.GetPhyAddr();
        VecMaskCompute(dst, src);
        outQueue.EnQue<T>(dstLocal);
        inQueue.FreeTensor(srcLocal);
    }

    __aicore__ inline void CopyOut()
    {
        LocalTensor<T> dstLocal = outQueue.DeQue<T>();
        DataCopy(dstGlobal, dstLocal, dstSize);
        outQueue.FreeTensor(dstLocal);
    }

private:
    GlobalTensor<T> srcGlobal;
    GlobalTensor<T> dstGlobal;

    TPipe pipe;
    TQue<TPosition::VECIN, 1> inQueue;
    TQue<TPosition::VECOUT, 1> outQueue;
    uint32_t dstSize;
};

template <typename T, int Mode>
__global__ __aicore__ void MicroMask(uint8_t* dstGm, uint8_t* srcGm, uint32_t size)
{
    KernelMicroMask<T, Mode> op;
    op.Init(srcGm, dstGm, size);
    op.Process();
}

template <int dim>
struct MicroMaskModeTestParams {
    void (*cal_func)(uint8_t*, uint8_t*, uint32_t);
    uint32_t size;
};

template <int Mode>
class MicroMaskTestsuite : public testing::Test, public testing::WithParamInterface<MicroMaskModeTestParams<Mode>> {};

#define MICRO_MASK_TEST_CASE(MODE)                                            \
    using MicroMaskTestsuite_mode##MODE = MicroMaskTestsuite<MODE>;           \
    INSTANTIATE_TEST_CASE_P(                                                  \
        TEST_MicroMask, MicroMaskTestsuite_mode##MODE,                        \
        ::testing::Values(                                                    \
            MicroMaskModeTestParams<MODE>{MicroMask<uint8_t, MODE>, 1024},    \
            MicroMaskModeTestParams<MODE>{MicroMask<int8_t, MODE>, 1024},     \
            MicroMaskModeTestParams<MODE>{MicroMask<uint16_t, MODE>, 1024},   \
            MicroMaskModeTestParams<MODE>{MicroMask<int16_t, MODE>, 1024},    \
            MicroMaskModeTestParams<MODE>{MicroMask<uint32_t, MODE>, 1024},   \
            MicroMaskModeTestParams<MODE>{MicroMask<int32_t, MODE>, 1024},    \
            MicroMaskModeTestParams<MODE>{MicroMask<float, MODE>, 1024},      \
            MicroMaskModeTestParams<MODE>{MicroMask<half, MODE>, 1024}));     \
                                                                              \
    TEST_P(MicroMaskTestsuite_mode##MODE, MicroMaskTestCase)                  \
    {                                                                         \
        auto param = GetParam();                                              \
        uint8_t* srcGm = new uint8_t[param.size * 4 * 10]{0};                 \
        uint8_t* dstGm = new uint8_t[param.size * 4 * 10]{0};                 \
        param.cal_func(dstGm, srcGm, param.size);                             \
        for (int32_t i = 0; i < (sizeof(dstGm) / sizeof(dstGm[0])); i++) {    \
            EXPECT_EQ(dstGm[i], 0x00);                                        \
        }                                                                     \
        delete[] srcGm;                                                       \
        delete[] dstGm;                                                       \
    }

MICRO_MASK_TEST_CASE(0);
MICRO_MASK_TEST_CASE(1);
MICRO_MASK_TEST_CASE(2);
MICRO_MASK_TEST_CASE(3);
MICRO_MASK_TEST_CASE(4);
MICRO_MASK_TEST_CASE(5);
MICRO_MASK_TEST_CASE(6);
MICRO_MASK_TEST_CASE(7);
MICRO_MASK_TEST_CASE(8);
MICRO_MASK_TEST_CASE(9);
MICRO_MASK_TEST_CASE(10);
MICRO_MASK_TEST_CASE(11);
MICRO_MASK_TEST_CASE(12);
MICRO_MASK_TEST_CASE(13);
MICRO_MASK_TEST_CASE(15);
