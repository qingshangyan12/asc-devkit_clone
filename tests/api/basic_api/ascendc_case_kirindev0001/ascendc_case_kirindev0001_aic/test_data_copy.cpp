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
#include <cstring>
#include <string>
#include "kernel_tensor.h"
#include "kernel_tpipe.h"
#include "kernel_operator_data_copy_intf.h"

using namespace std;
using namespace AscendC;

namespace {
constexpr size_t kGmBytes = 1024;       // capacity of the host GM staging arrays
constexpr size_t kHardBoundBytes = 512; // beyond ANY legal model write extent for these towers
constexpr size_t kFbBytes = 128;        // FIXBUF src-canary / dst-observation window at pool offset 0

void FillRamp(uint8_t* buf, size_t bytes)
{
    for (size_t i = 0; i < bytes; ++i) {
        buf[i] = static_cast<uint8_t>(i & 0xFF);
    }
}

void FillSentinel(uint8_t* buf, size_t bytes)
{
    memset(buf, 0xA5, bytes);
}

// Read-only src-window canary: GM->L1 host arrays AND the L1->FIXBUF A1 pool
// window are read-only on the model leg -- the per-byte ramp must survive.
void ExpectRampIntact(const string& tag, const uint8_t* buf, size_t bytes)
{
    for (size_t i = 0; i < bytes; ++i) {
        EXPECT_EQ(buf[i], static_cast<uint8_t>(i & 0xFF))
            << tag << ": src window modified at offset " << i << " (read-only on this leg's model path)";
    }
}

// L1->GM zero-or-sentinel invariant (see the file header). Observed zeroing
// morphology from the probe rounds (stable side effect of the stub's
// block-wise clearing, recorded here as evidence, NOT asserted -- the exact
// span is a harness artifact, not a product contract):
//   nz2nd: 64B span [0,63] for 2-byte types, 32B span [0,31] for int8_t
//   nz2dn:  4B span [0,3]  for 2-byte types,  2B span [0,1]  for int8_t
void ExpectZeroedOrSentinel(const string& tag, const uint8_t* buf, size_t bytes)
{
    size_t zeroBytes = 0;
    for (size_t i = 0; i < bytes; ++i) {
        if (buf[i] == 0x00) {
            ++zeroBytes;
        }
        if (i < kHardBoundBytes) {
            EXPECT_TRUE(buf[i] == 0x00 || buf[i] == 0xA5)
                << tag << ": dst byte at offset " << i << " is neither zeroed nor sentinel (0x" << hex
                << static_cast<int>(buf[i]) << ") -- real data crossed the model boundary";
        } else {
            EXPECT_EQ(buf[i], 0xA5) << tag << ": sentinel violated BEYOND the hard bound at offset " << i
                                    << " -- model write extent regression";
        }
    }
    EXPECT_GT(zeroBytes, static_cast<size_t>(0))
        << tag << ": dst was never zeroed -- leg early-returned without reaching the read-back path";
}

// L1->FIXBUF full-zeroing trap (round-35 probe D8 contract -- see the file
// header): the 0xA5-prefilled dst window must come back 100% zeroed (the
// stub's block-clearing side effect). Dead-leg detector: a skipped leg keeps
// the sentinel mass, rogue real data reads as neither zero nor sentinel.
void ExpectFullyZeroed(const string& tag, const uint8_t* buf, size_t bytes)
{
    size_t zeros = 0;
    size_t sentinels = 0;
    size_t firstOtherOff = static_cast<size_t>(-1);
    uint8_t firstOtherVal = 0;
    for (size_t i = 0; i < bytes; ++i) {
        if (buf[i] == 0x00) {
            ++zeros;
        } else if (buf[i] == 0xA5) {
            ++sentinels;
        } else if (firstOtherOff == static_cast<size_t>(-1)) {
            firstOtherOff = i;
            firstOtherVal = buf[i];
        }
    }
    EXPECT_EQ(zeros, bytes) << tag << ": dst not fully zeroed (zeros=" << zeros << " sentinels=" << sentinels
                            << " others=" << (bytes - zeros - sentinels) << " of " << bytes << "B"
                            << ") -- dead leg kept the 0xA5 prefill / leg never reached the read-back path";
    if (firstOtherOff != static_cast<size_t>(-1)) {
        ADD_FAILURE() << tag << ": first neither-zero-nor-sentinel byte @+" << firstOtherOff << " val=0x" << hex
                     << static_cast<int>(firstOtherVal) << " -- real data crossed the model boundary";
    }
}
} // namespace

// SetGCoreType lives in SetUp/TearDown rather than in the test body so that an
// asserting failure still unwinds through TearDown and restores core type 0.

// Core type 1 = AIC: the 5161 GM->L1 Nd2Nz/Dn2Nz backends are AIC-gated, and
// the L1->BT family stays on the AIC fixture for uniformity with the MTE2
// family (its backend has no core gate).
class TestNd2NzGm2L1Suite : public testing::Test, public testing::WithParamInterface<struct TypeCase> {
protected:
    void SetUp() override { SetGCoreType(1); }
    void TearDown() override { SetGCoreType(0); }
};

class TestDn2NzGm2L1Suite : public testing::Test, public testing::WithParamInterface<struct TypeCase> {
protected:
    void SetUp() override { SetGCoreType(1); }
    void TearDown() override { SetGCoreType(0); }
};

// Core type 2 = AIV for the MTE3 L1->GM legs (see the file header: do NOT
// change to 1 -- a skipped leg goes RED via the 0xA5 prefill + zeroBytes>0
// trap, and the NZ overloads carry no core-type gate at the intf layer).
class TestNz2NdL12GmSuite : public testing::Test, public testing::WithParamInterface<struct TypeCase> {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

class TestNz2DnL12GmSuite : public testing::Test, public testing::WithParamInterface<struct TypeCase> {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

class TestL12BtSuite : public testing::Test, public testing::WithParamInterface<struct BtCase> {
protected:
    void SetUp() override { SetGCoreType(1); }
    void TearDown() override { SetGCoreType(0); }
};

// L1->FIXBUF keeps the AIC fixture (same MTE2 L1-source family as L1->BT).
// The round-35 probe ran this leg under core type 1 and the dst window DID
// get zeroed -- the leg is live under AIC, no core-type gate on this path.
class TestL12FbSuite : public testing::Test, public testing::WithParamInterface<struct FbCase> {
protected:
    void SetUp() override { SetGCoreType(1); }
    void TearDown() override { SetGCoreType(0); }
};

// ---------------------------------------------------------------- kernel legs

// GM(ND) -> L1(NZ). Tower adapted from the 910B Nd2Nz case (ascendc_case_common
// test_data_copy.cpp): {ndNum=2, nValue=2, dValue=13, srcNdMatrixStride=48,
// srcDValue=16, dstNzC0Stride=11, dstNzNStride=2, dstNzMatrixStride=48}; all
// values inside the CheckNd2NzParams ranges. GetGMLen walk = 77 elements, well
// inside the 1024-byte host array.
template <typename T>
void nd2nz_gm2l1_kernel(__gm__ uint8_t* srcGm)
{
    TPipe tpipe;
    GlobalTensor<T> srcGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), kGmBytes / sizeof(T));

    LocalTensor<T> l1Dst;
    TBuffAddr tbuf;
    tbuf.logicPos = (uint8_t)TPosition::A1;
    l1Dst.SetAddr(tbuf);
    l1Dst.InitBuffer(0, 256);

    Nd2NzParams nd2nzParams{2, 2, 13, 48, 16, 11, 2, 48};
    DataCopy<T, false>(l1Dst, srcGlobal, nd2nzParams);
    PipeBarrier<PIPE_ALL>();
}

// GM(DN) -> L1(NZ). Same tower with srcDnMatrixStride/srcDValue. GetGMLen(DN)
// walk = (2-1)*48 + 2 + (13-1)*16 = 242 elements = 484 bytes at most, inside
// the 1024-byte host array.
template <typename T>
void dn2nz_gm2l1_kernel(__gm__ uint8_t* srcGm)
{
    TPipe tpipe;
    GlobalTensor<T> srcGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), kGmBytes / sizeof(T));

    LocalTensor<T> l1Dst;
    TBuffAddr tbuf;
    tbuf.logicPos = (uint8_t)TPosition::A1;
    l1Dst.SetAddr(tbuf);
    l1Dst.InitBuffer(0, 256);

    Dn2NzParams dn2nzParams{2, 2, 13, 48, 16, 11, 2, 48};
    DataCopy<T, false>(l1Dst, srcGlobal, dn2nzParams);
    PipeBarrier<PIPE_ALL>();
}

// L1(NZ) -> GM(ND). Params inside the CheckNz2NdParams ranges (nValue/dValue
// in [1,8192], srcNdMatrixStride in [1,512], srcNStride in [0,4095]).
// dstDStride >= dValue keeps ND rows non-overlapping; plausible model write
// extent <= (dValue-1)*dstDStride*sizeof + nValue*sizeof = 484 bytes for
// 2-byte types, below the 512-byte hard bound.
template <typename T>
void nz2nd_l12gm_kernel(__gm__ uint8_t* dstGm)
{
    TPipe tpipe;
    GlobalTensor<T> dstGlobal;
    dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), kGmBytes / sizeof(T));

    LocalTensor<T> l1Src;
    TBuffAddr tbuf;
    tbuf.logicPos = (uint8_t)TPosition::A1;
    l1Src.SetAddr(tbuf);
    l1Src.InitBuffer(0, 256);

    Nz2NdParamsFull nz2ndParams{1, 2, 16, 8, 1, 16, 32};
    DataCopy<T>(dstGlobal, l1Src, nz2ndParams);
    PipeBarrier<PIPE_ALL>();
}

// L1(NZ) -> GM(DN). No param checker on 5161; same conservative tower as
// Nz2Nd (dstNStride >= dValue keeps DN rows non-overlapping).
template <typename T>
void nz2dn_l12gm_kernel(__gm__ uint8_t* dstGm)
{
    TPipe tpipe;
    GlobalTensor<T> dstGlobal;
    dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), kGmBytes / sizeof(T));

    LocalTensor<T> l1Src;
    TBuffAddr tbuf;
    tbuf.logicPos = (uint8_t)TPosition::A1;
    l1Src.SetAddr(tbuf);
    l1Src.InitBuffer(0, 256);

    Nz2DnParamsFull nz2dnParams{1, 2, 16, 8, 1, 16, 32};
    DataCopy<T>(dstGlobal, l1Src, nz2dnParams);
    PipeBarrier<PIPE_ALL>();
}

// L1 -> BiasTable (C2), same-type face. Same-shape call as the mc62cm12aa C2
// fixture (DataCopyParams{1, 32*sizeof(T), 0, 0}); dst align 64B / src align
// 32B are checked by the intf (CheckTensorAlign) and satisfied by the
// InitBuffer(0, ..) pool offsets. Type face restricted by the dav_5161 backend
// gate (half/float/int32 only) -- uint8/int8 here would ASSERT(false) at
// runtime.
template <typename T>
void l12bt_kernel()
{
    TPipe tpipe;
    LocalTensor<T> l1Src;
    TBuffAddr srcBuf;
    srcBuf.logicPos = (uint8_t)TPosition::A1;
    l1Src.SetAddr(srcBuf);
    l1Src.InitBuffer(0, 32);

    LocalTensor<T> btDst;
    TBuffAddr dstBuf;
    dstBuf.logicPos = (uint8_t)TPosition::C2;
    btDst.SetAddr(dstBuf);
    btDst.InitBuffer(0, 32);

    DataCopyParams btParams{1, 32 * sizeof(T), 0, 0};
    DataCopy(btDst, l1Src, btParams);
    PipeBarrier<PIPE_ALL>();
}

// L1 -> BiasTable with on-the-way type conversion. Binds the dual-template
// DataCopy overload (kernel_operator_data_copy_intf_impl.h:449); on 5161 the
// arm selection at intf_impl.h:498-515 yields isenableConv=2 for
// <int32_t, half>/<int32_t, float> and =1 for <float, half>. Tower derived
// from the size checker: blockLen is in src bytes, and for L1->BIAS mixed-size
// legs the dst burst unit doubles (kernel_check_data_copy_util.h:376-378), so
// blockLen = 32*sizeof(SrcT) converts to exactly 32 dst elements: src
// InitBuffer(0, 32) = blockLen bytes, dst InitBuffer(0, 32) = 32 DstT
// elements, an exact fit on both sides. The overflow checker whose biasConv
// semantics this tower derives from is compiled out on 5161 --
// ASCENDC_REPORT_OVERFLOW_MEM expands to nothing there (its raise(SIGABRT)
// arm builds for 2002/2201/3510 only, kernel_log.h:82-84) -- so the exact
// fit rests on probe evidence, not runtime enforcement.
template <typename DstT, typename SrcT>
void l12bt_conv_kernel()
{
    TPipe tpipe;
    LocalTensor<SrcT> l1Src;
    TBuffAddr srcBuf;
    srcBuf.logicPos = (uint8_t)TPosition::A1;
    l1Src.SetAddr(srcBuf);
    l1Src.InitBuffer(0, 32);

    LocalTensor<DstT> btDst;
    TBuffAddr dstBuf;
    dstBuf.logicPos = (uint8_t)TPosition::C2;
    btDst.SetAddr(dstBuf);
    btDst.InitBuffer(0, 32);

    DataCopyParams btParams{1, 32 * sizeof(SrcT), 0, 0};
    DataCopy(btDst, l1Src, btParams);
    PipeBarrier<PIPE_ALL>();
}

// L1 -> Fixpipe (C2PIPE2GM), same-type face, DataCopyParams payload with
// blockLen = 32*sizeof(T) and zero strides -- the exact tower the round-35
// r9 probe proved live. Alive thanks to the dav_5161 dual-world gate in
// DataCopyL12FBImpl (kernel_operator_data_copy_impl.h:286-311): the
// ASCENDC_CPU_DEBUG arm folds intriParams.postProcBufBlock into the dst
// address and calls the 6-param stub form copy_cbuf_to_fbuf_v2 -- this
// OVERTURNED the historic ASSERT(false) blocked state (probe D2: exit 0,
// clean teardown, src canary intact, dst 128B fully zeroed). Both positions
// are model-internal, but GetPhyAddr() exposes both windows to the host, so
// this leg asserts the probe D8 contract (see the file header) instead of
// settling for L1->BT's dispatch-only form.
template <typename T>
void l12fb_kernel(const string& tag)
{
    TPipe tpipe;
    LocalTensor<T> src;
    TBuffAddr srcBuf;
    srcBuf.logicPos = (uint8_t)TPosition::A1;
    src.SetAddr(srcBuf);
    src.InitBuffer(0, kFbBytes / sizeof(T));

    LocalTensor<T> dst;
    TBuffAddr dstBuf;
    dstBuf.logicPos = (uint8_t)TPosition::C2PIPE2GM;
    dst.SetAddr(dstBuf);
    dst.InitBuffer(0, kFbBytes / sizeof(T)); // pool offset 0 -> the 128B dst-align gate passes

    uint8_t* srcP = reinterpret_cast<uint8_t*>(src.GetPhyAddr());
    uint8_t* dstP = reinterpret_cast<uint8_t*>(dst.GetPhyAddr());
    FillRamp(srcP, kFbBytes);
    FillSentinel(dstP, kFbBytes);

    DataCopyParams fbParams{1, 32 * sizeof(T), 0, 0};
    DataCopy(dst, src, fbParams);
    PipeBarrier<PIPE_ALL>();

    ExpectRampIntact(tag + "/src", srcP, kFbBytes);
    ExpectFullyZeroed(tag + "/dst", dstP, kFbBytes);
}

// --------------------------------------------------------------------- params

struct TypeCase {
    const char* typeName;
    void (*calFunc)(__gm__ uint8_t*);
};

struct BtCase {
    const char* caseName;
    void (*calFunc)();
};

struct FbCase {
    const char* caseName;
    void (*calFunc)(const string&); // takes a diagnostic tag; asserts the D8 contract internally
};

// ---------------------------------------------------------------------- tests

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_ND2NZ_GM2L1, TestNd2NzGm2L1Suite,
    ::testing::Values(
        TypeCase{"half", nd2nz_gm2l1_kernel<half>},
        TypeCase{"int8_t", nd2nz_gm2l1_kernel<int8_t>},
        TypeCase{"int16_t", nd2nz_gm2l1_kernel<int16_t>},
        TypeCase{"uint16_t", nd2nz_gm2l1_kernel<uint16_t>}));

TEST_P(TestNd2NzGm2L1Suite, Nd2NzGm2L1Cases)
{
    auto param = GetParam();
    uint8_t srcGm[kGmBytes];
    FillRamp(srcGm, sizeof(srcGm));
    param.calFunc(srcGm);
    ExpectRampIntact(string("nd2nz/") + param.typeName, srcGm, sizeof(srcGm));
}

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_DN2NZ_GM2L1, TestDn2NzGm2L1Suite,
    ::testing::Values(
        TypeCase{"half", dn2nz_gm2l1_kernel<half>},
        TypeCase{"int8_t", dn2nz_gm2l1_kernel<int8_t>},
        TypeCase{"int16_t", dn2nz_gm2l1_kernel<int16_t>},
        TypeCase{"uint16_t", dn2nz_gm2l1_kernel<uint16_t>}));

TEST_P(TestDn2NzGm2L1Suite, Dn2NzGm2L1Cases)
{
    auto param = GetParam();
    uint8_t srcGm[kGmBytes];
    FillRamp(srcGm, sizeof(srcGm));
    param.calFunc(srcGm);
    ExpectRampIntact(string("dn2nz/") + param.typeName, srcGm, sizeof(srcGm));
}

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_NZ2ND_L12GM, TestNz2NdL12GmSuite,
    ::testing::Values(
        TypeCase{"half", nz2nd_l12gm_kernel<half>},
        TypeCase{"int8_t", nz2nd_l12gm_kernel<int8_t>},
        TypeCase{"int16_t", nz2nd_l12gm_kernel<int16_t>},
        TypeCase{"uint16_t", nz2nd_l12gm_kernel<uint16_t>}));

TEST_P(TestNz2NdL12GmSuite, Nz2NdL12GmCases)
{
    auto param = GetParam();
    uint8_t dstGm[kGmBytes];
    FillSentinel(dstGm, sizeof(dstGm));
    param.calFunc(dstGm);
    ExpectZeroedOrSentinel(string("nz2nd/") + param.typeName, dstGm, sizeof(dstGm));
}

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_NZ2DN_L12GM, TestNz2DnL12GmSuite,
    ::testing::Values(
        TypeCase{"half", nz2dn_l12gm_kernel<half>},
        TypeCase{"int8_t", nz2dn_l12gm_kernel<int8_t>},
        TypeCase{"int16_t", nz2dn_l12gm_kernel<int16_t>},
        TypeCase{"uint16_t", nz2dn_l12gm_kernel<uint16_t>}));

TEST_P(TestNz2DnL12GmSuite, Nz2DnL12GmCases)
{
    auto param = GetParam();
    uint8_t dstGm[kGmBytes];
    FillSentinel(dstGm, sizeof(dstGm));
    param.calFunc(dstGm);
    ExpectZeroedOrSentinel(string("nz2dn/") + param.typeName, dstGm, sizeof(dstGm));
}

// Same-type face (isenableConv=0) plus the on-the-way conversion faces
// (isenableConv=2 / =1) of the L1->BT intf. All legs are model-internal (C2
// position): dispatch and run-without-crash is the observable contract under
// the no-op pv harness.
INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_L12BT, TestL12BtSuite,
    ::testing::Values(
        BtCase{"half", l12bt_kernel<half>},
        BtCase{"float", l12bt_kernel<float>},
        BtCase{"int32_t", l12bt_kernel<int32_t>},
        BtCase{"half_to_int32", l12bt_conv_kernel<int32_t, half>},
        BtCase{"half_to_float", l12bt_conv_kernel<float, half>},
        BtCase{"float_to_int32", l12bt_conv_kernel<int32_t, float>}));

TEST_P(TestL12BtSuite, L12BtCases)
{
    auto param = GetParam();
    param.calFunc();
    SUCCEED() << "L1->BT leg '" << param.caseName << "' dispatched and returned";
}

// The FIXBUF family. Type face restricted to half: the round-35 probe proved
// exactly this one face; widening the tower (float/int32_t like L1->BT) needs
// a fresh probe first. Unlike L1->BT, no dispatch-only SUCCEED here -- the
// calFunc asserts the full D8 contract (src canary + dst full zeroing).
INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_L12FB, TestL12FbSuite,
    ::testing::Values(
        FbCase{"half", l12fb_kernel<half>}));

TEST_P(TestL12FbSuite, L12FbCases)
{
    auto param = GetParam();
    param.calFunc(string("l12fb/") + param.caseName);
}
