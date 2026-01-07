/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#include <gtest/gtest.h>
#include <type_traits>
#include "simt_compiler_stub.h"
#include "kernel_operator.h"
#include "simt_api/asc_bf16.h"
using namespace std;
using namespace AscendC;
using namespace AscendC::Simt;

#define THREAD_DIM 128

template <typename DstType, typename SrcType>
class KernelCast {
public:
    __aicore__ KernelCast() {}

public:
    __aicore__ inline void Process(__gm__ DstType* dst, const int mode);
};

template <typename DstType, typename SrcType>
__simt_vf__ LAUNCH_BOUND(1024) inline __aicore__ void KernelCastCompute(__gm__ DstType* dst, const int mode)
{
    for (int idx = GetThreadIdx<0>() + block_idx * GetThreadNum<0>(); idx < 128;
         idx += block_num * GetThreadNum<0>()) {

        if (mode == 1) {
            DstType srcNum = idx;
            if (idx == 1022) {
                srcNum = -30144;
            }
            if (idx == 1023) {
                srcNum = -87825;
            }
            dst[idx] = hrint(srcNum);
        } else if (mode == 2) {
            DstType srcNum = idx;
            if (idx == 1022) {
                srcNum = -30144;
            }
            if (idx == 1023) {
                srcNum = -87825;
            }
            dst[idx] = hfloor(srcNum);
        } else if (mode == 3) {
            DstType srcNum = idx;
            if (idx == 1022) {
                srcNum = -30144;
            }
            if (idx == 1023) {
                srcNum = -87825;
            }
            dst[idx] = hceil(srcNum);
        } else if (mode == 7) {
            DstType srcNum = idx;
            if (idx == 1022) {
                srcNum = -30144;
            }
            if (idx == 1023) {
                srcNum = -87825;
            }
            dst[idx] = htrunc(srcNum);
        }
    }
}

template <typename DstType, typename SrcType>
__aicore__ inline void KernelCast<DstType, SrcType>::Process(__gm__ DstType* dst, const int mode)
{
    asc_vf_call<KernelCastCompute<DstType, SrcType>>(AscendC::Simt::Dim3(THREAD_DIM, 1, 1), dst, mode);
}

// ================================ Test half start ================================
struct CastParams_half {
    int32_t mode;
};

class CastTestsuite_half : public testing::Test, public testing::WithParamInterface<CastParams_half> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(CastTestCase_half, CastTestsuite_half,
                        ::testing::Values(CastParams_half{1}, CastParams_half{2}, CastParams_half{3},
                                          CastParams_half{7}));

TEST_P(CastTestsuite_half, CastTestCase_half)
{
    auto param = GetParam();
    int32_t mode = mode = param.mode;
    int fpByteSize = 4;
    int shapeSize = 128;

    uint8_t dstGm[shapeSize * fpByteSize] = {0};
    KernelCast<half, int> op;
    op.Process((__gm__ half*)dstGm, mode);

    half expectValues[shapeSize] = {0};
    for (int i = 0; i < shapeSize; i += 1) {
        expectValues[i] = i;
        if (i == 1022) {
            expectValues[i] = -30144;
        }
        if (i == 1023) {
            expectValues[i] = -87825;
        }
        if (mode == 1) {
            expectValues[i] = hrint(expectValues[i]);
        } else if (mode == 2) {
            expectValues[i] = hfloor(expectValues[i]);
        } else if (mode == 3) {
            expectValues[i] = hceil(expectValues[i]);
        } else if (mode == 7) {
            expectValues[i] = htrunc(expectValues[i]);
        }
    }

    half *actualValue = reinterpret_cast<half *>(dstGm);
    for (int i = 0; i < shapeSize; i += 1) {
        ASSERT_EQ(expectValues[i], actualValue[i]);
        ASSERT_EQ(expectValues[i], actualValue[i]);
        ASSERT_EQ(expectValues[i], actualValue[i]);
        ASSERT_EQ(expectValues[i], actualValue[i]);
    }
}
// ================================ Test half end ==================================

// ================================ Test bfloat16_t start ================================
struct CastParams_bfloat16t {
    int32_t mode;
};

class CastTestsuite_bfloat16t : public testing::Test, public testing::WithParamInterface<CastParams_bfloat16t> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(CastTestCase_bfloat16t, CastTestsuite_bfloat16t,
                        ::testing::Values(CastParams_bfloat16t{1}, CastParams_bfloat16t{2}, CastParams_bfloat16t{3},
                                          CastParams_bfloat16t{7}));

TEST_P(CastTestsuite_bfloat16t, CastTestCase_bfloat16t)
{
    auto param = GetParam();
    int32_t mode = mode = param.mode;
    int fpByteSize = 4;
    int shapeSize = 128;

    uint8_t dstGm[shapeSize * fpByteSize] = {0};
    KernelCast<bfloat16_t, int> op;
    op.Process((__gm__ bfloat16_t*)dstGm, mode);

    bfloat16_t expectValues[shapeSize] = {0};
    for (int i = 0; i < shapeSize; i += 1) {
        expectValues[i] = i;
        if (i == 1022) {
            expectValues[i] = -30144;
        }
        if (i == 1023) {
            expectValues[i] = -87825;
        }
        if (mode == 1) {
            expectValues[i] = hrint(expectValues[i]);
        } else if (mode == 2) {
            expectValues[i] = hfloor(expectValues[i]);
        } else if (mode == 3) {
            expectValues[i] = hceil(expectValues[i]);
        } else if (mode == 7) {
            expectValues[i] = htrunc(expectValues[i]);
        }
    }

    bfloat16_t *actualValue = reinterpret_cast<bfloat16_t *>(dstGm);
    for (int i = 0; i < shapeSize; i += 1) {
        ASSERT_EQ(expectValues[i], actualValue[i]);
        ASSERT_EQ(expectValues[i], actualValue[i]);
        ASSERT_EQ(expectValues[i], actualValue[i]);
        ASSERT_EQ(expectValues[i], actualValue[i]);
    }
}
// ================================ Test bfloat16_t end ==================================