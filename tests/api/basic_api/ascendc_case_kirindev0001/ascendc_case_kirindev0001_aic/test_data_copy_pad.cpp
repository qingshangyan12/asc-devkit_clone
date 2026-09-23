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
#include "kernel_tensor.h"
#include "kernel_tpipe.h"
#include "kernel_operator_data_copy_intf.h"

using namespace std;
using namespace AscendC;

struct TestDataCopyPadGm2L1Params {
    uint32_t dataSize;   // element count
    uint32_t typeSize;   // sizeof(T)
    void (*cal_func)(uint8_t*, int32_t);   // (srcGm, dataSize)
};

struct TestDataCopyPadL12GmParams {
    uint32_t dataSize;   // element count
    uint32_t typeSize;   // sizeof(T)
    void (*cal_func)(uint8_t*, int32_t);   // (dstGm, dataSize)
};

// SetGCoreType lives in SetUp/TearDown rather than in the test body so that an
// asserting failure still unwinds through TearDown and restores core type 0.
class TestDataCopyPadGm2L1Suite : public testing::Test,
                                  public testing::WithParamInterface<TestDataCopyPadGm2L1Params> {
protected:
    // 2 = AIV core type: the shape every historical green run of this pad
    // coverage was collected in. The 5161 MTE2 GM->L1 DataCopyPad overload
    // (kernel_operator_data_copy_intf_impl.h:1615-1646) carries no core-type
    // gate, but changing it here would be an unforced variable; keep AIV.
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

class TestDataCopyPadL12GmSuite : public testing::Test,
                                  public testing::WithParamInterface<TestDataCopyPadL12GmParams> {
protected:
    // 2 = AIV core type, deliberately NOT AIC (=1): under CPU debug the 5161
    // DataCopyPad L1->GM overload used below,
    // impl/basic_api/kernel_operator_data_copy_intf_impl.h:1744-1748
    // (gated __NPU_ARCH__ != 5102), early-returns while the current core is
    // AIC -- "correcting" this to 1 silently skips the whole MTE3 leg; the
    // 0xA5 sentinel pre-fill below is what makes such a skip go RED.
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

template <typename T>
void main_data_copy_pad_gm2l1_kernel(__gm__ uint8_t* __restrict__ srcGm, int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> srcGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);

    // L1 destination, cube side.
    LocalTensor<T> l1Local;
    TBuffAddr tbuf;
    tbuf.logicPos = (uint8_t)TPosition::A1;
    l1Local.SetAddr(tbuf);
    l1Local.InitBuffer(0, dataSize);

    // Contiguous single row: blockCount == 1, blockLen == dataSize * sizeof(T).
    // srcStride/dstStride are in bytes for DataCopyExtParams; 0 means tightly packed.
    DataCopyExtParams copyParams{1, static_cast<uint32_t>(dataSize * sizeof(T)), 0, 0, 0};
    DataCopyPadExtParams<T> padParams{false, 0, 0, 0};

    DataCopyPad<T, PaddingMode::Normal>(l1Local, srcGlobal, copyParams, padParams);

    PipeBarrier<PIPE_ALL>();
}

template <typename T>
void main_data_copy_pad_l12gm_kernel(__gm__ uint8_t* __restrict__ dstGm, int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> dstGlobal;
    dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    // L1 staging source, cube side. Freshly allocated, no prior fill: whatever
    // it holds never reaches the host (GetOutput/pv_mem_read clears the
    // destination), so the host contract below is the same as for the
    // nz2nd/nz2dn families in test_data_copy.cpp, whose L1 sources are
    // equally uninitialised.
    LocalTensor<T> l1Local;
    TBuffAddr tbuf;
    tbuf.logicPos = (uint8_t)TPosition::A1;
    l1Local.SetAddr(tbuf);
    l1Local.InitBuffer(0, dataSize);

    // Contiguous single row: blockCount == 1, blockLen == dataSize * sizeof(T).
    DataCopyExtParams copyParams{1, static_cast<uint32_t>(dataSize * sizeof(T)), 0, 0, 0};

    DataCopyPad<T, PaddingMode::Normal>(dstGlobal, l1Local, copyParams);

    PipeBarrier<PIPE_ALL>();
}

// NOTE: L1->GM types are restricted to the 5161 fixpipe L1->GM intrinsic set
// (int8_t/uint8_t/int16_t/uint16_t/half, see cce_intri.json fix_cbuf_to_gm);
// int32_t/float have no fix_cbuf_to_gm overload on this device. The GM->L1
// leg keeps the same four faces: the 5161 libcpudebug dump has only the
// uchar*/ushort* copy_gm_to_cbuf_align_v2 symbols, so the B16 legs
// (half/int16_t/uint16_t) bind the 2-byte face and int8_t binds the 1-byte
// face through the dav_5161 sizeof arms (:130/:134, round-24..32 evidence).

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_PAD_GM_L1, TestDataCopyPadGm2L1Suite,
    ::testing::Values(
        TestDataCopyPadGm2L1Params{64, 2, main_data_copy_pad_gm2l1_kernel<half>},
        TestDataCopyPadGm2L1Params{64, 2, main_data_copy_pad_gm2l1_kernel<int16_t>},
        TestDataCopyPadGm2L1Params{64, 2, main_data_copy_pad_gm2l1_kernel<uint16_t>},
        TestDataCopyPadGm2L1Params{64, 1, main_data_copy_pad_gm2l1_kernel<int8_t>}));

TEST_P(TestDataCopyPadGm2L1Suite, DataCopyPadGm2L1Cases)
{
    auto param = GetParam();
    const size_t byteSize = static_cast<size_t>(param.dataSize) * param.typeSize;

    uint8_t srcGm[byteSize];
    for (size_t i = 0; i < byteSize; ++i) {
        srcGm[i] = static_cast<uint8_t>(i & 0xFF);
    }

    param.cal_func(srcGm, param.dataSize);

    // srcGm canary (one-way veto): the MTE2 leg may only READ host GM; nothing
    // writes the source, so the ramp survives -- same contract as the
    // nd2nz/dn2nz faces in test_data_copy.cpp. It cannot prove the leg ran
    // (the L1 destination is model-internal and pv_mem_write is a no-op); the
    // compensating control is the per-round gcov watch on the dav_5161
    // copy_gm_to_cbuf_align_v2 arms.
    for (size_t i = 0; i < byteSize; ++i) {
        EXPECT_EQ(srcGm[i], static_cast<uint8_t>(i & 0xFF))
            << "srcGm canary violated at offset " << i
            << " (GM->L1 leg wrote the read-only source buffer)";
    }
}

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_PAD_L1_GM, TestDataCopyPadL12GmSuite,
    ::testing::Values(
        TestDataCopyPadL12GmParams{64, 2, main_data_copy_pad_l12gm_kernel<half>},
        TestDataCopyPadL12GmParams{64, 2, main_data_copy_pad_l12gm_kernel<int16_t>},
        TestDataCopyPadL12GmParams{64, 2, main_data_copy_pad_l12gm_kernel<uint16_t>},
        TestDataCopyPadL12GmParams{64, 1, main_data_copy_pad_l12gm_kernel<int8_t>}));

TEST_P(TestDataCopyPadL12GmSuite, DataCopyPadL12GmCases)
{
    auto param = GetParam();
    const size_t byteSize = static_cast<size_t>(param.dataSize) * param.typeSize;

    // Single allocation: [0, byteSize) = destination, [byteSize, 2*byteSize)
    // = canary. Deterministic adjacency replaces the stack-layout luck of the
    // former two-array round trip.
    uint8_t gm[2 * byteSize];
    for (size_t i = 0; i < byteSize; ++i) {
        gm[i] = 0xA5;  // sentinel pre-fill: a skipped leg leaves dst == 0xA5 and fails below
        gm[byteSize + i] = static_cast<uint8_t>(i & 0xFF);
    }

    param.cal_func(gm, param.dataSize);

    // Zeroing contract (pv stub harness): pv_mem_write is a no-op and GetOutput
    // (pv_mem_read) clears the host destination, so no data crosses the model
    // boundary. The checked span equals the copy extent exactly (blockCount ==
    // 1, blockLen == dataSize * sizeof(T)), so dst must read back all 0x00 --
    // the same convention as the 910B family (test_operator_datacopy.cpp /
    // test_operator_fixpipe.cpp) and as the round trip this suite was split
    // from (round-24..32 green evidence).
    size_t zeroBytes = 0;
    for (size_t i = 0; i < byteSize; ++i) {
        EXPECT_EQ(gm[i], 0x00)
            << "zeroing contract violated at offset " << i << " (dataSize=" << param.dataSize
            << ", typeSize=" << param.typeSize << ")";
        if (gm[i] == 0x00) {
            ++zeroBytes;
        }
    }
    // Tripwire: guards a silent early-return of the whole leg (the interface
    // AIC early-return trap, see the fixture note above). With the sentinel
    // pre-fill this is belt-and-braces with the per-byte EXPECT_EQ, but gives
    // a leg-level failure message.
    EXPECT_GT(zeroBytes, 0)
        << "L1->GM leg produced no zeroed byte -- the whole leg was skipped?";

    // canary -- regression tripwire for the loop2Size = blockLen
    // (bytes-as-elements) bug in dav_5161/kernel_operator_data_copy_impl.h.
    // The latent overflow made the host clear run loop2Size * sizeof(T) bytes
    // long (2-byte types: 2x the 128B extent) and wiped whatever sat past
    // dstGm, zeroing the adjacent buffer and letting the then-0x00-prefill
    // zeroing assertions pass vacuously before the fix (loop2Size =
    // blockLen / sizeof(T), :961). The canary occupies the same allocation
    // right after dst, so a return of that regression fires here for the
    // 2-byte types (round-24/26 evidence); with the fix nothing may write
    // past byteSize, so the ramp survives.
    for (size_t i = 0; i < byteSize; ++i) {
        EXPECT_EQ(gm[byteSize + i], static_cast<uint8_t>(i & 0xFF))
            << "dstGm canary violated at canary offset " << i
            << " (dst write overflow wiped the adjacent canary)";
    }
}
