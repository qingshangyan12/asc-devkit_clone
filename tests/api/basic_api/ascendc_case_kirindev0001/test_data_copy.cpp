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

// 目的池 BIAS 1024B / FIXBUF 6144B / PT 256B, 各塔模型写入跨度须保持在池内。

struct TypeCase {
    uint32_t dataSize;
    uint32_t typeSize;
    void (*calFunc)(__gm__ uint8_t*, int32_t);
};

struct BtCase {
    void (*calFunc)();
};

struct FbCase {
    uint32_t dataSize;
    void (*calFunc)(const string&, int32_t);
};

// L1->PT {nBurst, lenBurst, srcGap, dstGap}, 均为 32B 单位。
struct PtParams {
    uint16_t nBurst;
    uint16_t lenBurst;
    uint16_t srcGap;
    uint16_t dstGap;
};

struct PtCase {
    const char* caseName;
    PtParams params;
    void (*calFunc)(const string&, const PtParams&);
};

class TestNd2NzGm2L1Suite : public testing::Test, public testing::WithParamInterface<TypeCase> {};
class TestDn2NzGm2L1Suite : public testing::Test, public testing::WithParamInterface<TypeCase> {};

class TestNz2NdL12GmSuite : public testing::Test, public testing::WithParamInterface<TypeCase> {};
class TestNz2DnL12GmSuite : public testing::Test, public testing::WithParamInterface<TypeCase> {};

class TestL12BtSuite : public testing::Test, public testing::WithParamInterface<BtCase> {};
class TestL12FbSuite : public testing::Test, public testing::WithParamInterface<FbCase> {};
class TestL12PtSuite : public testing::Test, public testing::WithParamInterface<PtCase> {};

// GM(ND)->L1(NZ)
template <typename T>
void nd2nz_gm2l1_kernel(__gm__ uint8_t* srcGm, int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> srcGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);

    TBuf<TPosition::A1> l1Buf;
    tpipe.InitBuffer(l1Buf, 256 * sizeof(T));
    LocalTensor<T> l1Dst = l1Buf.Get<T>();

    Nd2NzParams nd2nzParams{2, 2, 13, 48, 16, 11, 2, 48};
    DataCopy<T, false>(l1Dst, srcGlobal, nd2nzParams);
    PipeBarrier<PIPE_ALL>();
}

// GM(DN)->L1(NZ)
template <typename T>
void dn2nz_gm2l1_kernel(__gm__ uint8_t* srcGm, int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> srcGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);

    TBuf<TPosition::A1> l1Buf;
    tpipe.InitBuffer(l1Buf, 256 * sizeof(T));
    LocalTensor<T> l1Dst = l1Buf.Get<T>();

    Dn2NzParams dn2nzParams{2, 2, 13, 48, 16, 11, 2, 48};
    DataCopy<T, false>(l1Dst, srcGlobal, dn2nzParams);
    PipeBarrier<PIPE_ALL>();
}

// L1(NZ)->GM(ND): dstDStride >= dValue 保证 ND 行不重叠
template <typename T>
void nz2nd_l12gm_kernel(__gm__ uint8_t* dstGm, int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> dstGlobal;
    dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    TBuf<TPosition::A1> l1Buf;
    tpipe.InitBuffer(l1Buf, 256 * sizeof(T));
    LocalTensor<T> l1Src = l1Buf.Get<T>();

    Nz2NdParamsFull nz2ndParams{1, 2, 16, 8, 1, 16, 32};
    DataCopy<T>(dstGlobal, l1Src, nz2ndParams);
    PipeBarrier<PIPE_ALL>();
}

// L1(NZ)->GM(DN):dstNStride >= dValue 保证 DN 行不重叠
template <typename T>
void nz2dn_l12gm_kernel(__gm__ uint8_t* dstGm, int32_t dataSize)
{
    TPipe tpipe;
    GlobalTensor<T> dstGlobal;
    dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    TBuf<TPosition::A1> l1Buf;
    tpipe.InitBuffer(l1Buf, 256 * sizeof(T));
    LocalTensor<T> l1Src = l1Buf.Get<T>();

    Nz2DnParamsFull nz2dnParams{1, 2, 16, 8, 1, 16, 32};
    DataCopy<T>(dstGlobal, l1Src, nz2dnParams);
    PipeBarrier<PIPE_ALL>();
}

// L1->BiasTable(C2) dst 恒 s32
// blockLen 计 32B 单位: {1,8,0,0} 搬 256B 入 1024B BIAS
template <typename DstT, typename SrcT>
void l12bt_kernel()
{
    TPipe tpipe;
    TBuf<TPosition::A1> srcBuf;
    tpipe.InitBuffer(srcBuf, 32 * sizeof(SrcT));
    LocalTensor<SrcT> l1Src = srcBuf.Get<SrcT>();

    TBuf<TPosition::C2> dstBuf;
    tpipe.InitBuffer(dstBuf, 32 * sizeof(DstT));
    LocalTensor<DstT> btDst = dstBuf.Get<DstT>();

    DataCopyParams btParams{1, 8, 0, 0};
    DataCopy(btDst, l1Src, btParams);
    PipeBarrier<PIPE_ALL>();
}

// L1->FixpipeBuffer(C2PIPE2GM): blockLen 设计单位 128B, {1,16,0,0} 需 16*128=2048B 于 6144B 池内
template <typename T>
void l12fb_kernel(const string& tag, int32_t dataSize)
{
    const size_t winBytes = static_cast<size_t>(dataSize) * sizeof(T);

    TPipe tpipe;
    TBuf<TPosition::A1> srcBuf;
    tpipe.InitBuffer(srcBuf, winBytes);
    LocalTensor<T> src = srcBuf.Get<T>();

    TBuf<TPosition::C2PIPE2GM> dstBuf;
    tpipe.InitBuffer(dstBuf, winBytes);
    LocalTensor<T> dst = dstBuf.Get<T>();

    uint8_t* srcP = reinterpret_cast<uint8_t*>(src.GetPhyAddr());
    uint8_t* dstP = reinterpret_cast<uint8_t*>(dst.GetPhyAddr());
    for (size_t i = 0; i < winBytes; ++i) {
        srcP[i] = static_cast<uint8_t>(i & 0xFF);
    }
    memset(dstP, 0xCA, winBytes); // 哨兵预填充: 死腿保留, 活腿清零

    DataCopyParams fbParams{1, 16, 0, 0}; // 设计 16*128=2048B / 模型 (硬编码 32B) 仅清 16*32=512B
    DataCopy(dst, src, fbParams);
    PipeBarrier<PIPE_ALL>();

    for (size_t i = 0; i < winBytes; ++i) {
        EXPECT_EQ(srcP[i], static_cast<uint8_t>(i & 0xFF))
            << tag << "/src: window modified at offset " << i << " (read-only on this leg's model path)";
    }
    // dst 全窗清零: 每字节须回读 0x00 -- 死腿保 0xCA 预填充, 混入的真实数据读为其他值。
    for (size_t i = 0; i < winBytes; ++i) {
        EXPECT_EQ(dstP[i], 0x00) << tag << "/dst: window byte @+" << i << " not zeroed (0x" << hex
                                << static_cast<int>(dstP[i])
                                << ") -- dead leg kept the 0xCA prefill or real data crossed the model boundary";
    }
}

// L1->PaddingTable(C2PT) 经 copy_cbuf_to_pt
template <typename T>
void l12pt_kernel(const string& tag, const PtParams& pt)
{
    constexpr size_t kPtPoolBytes = 256; // 整个 cpuPT 池 == V516 ISA 侧 Padding Table 大小
    const size_t srcSpanBytes =
        static_cast<size_t>(pt.nBurst) * pt.lenBurst * 32 + static_cast<size_t>(pt.nBurst - 1) * pt.srcGap * 32;

    TPipe tpipe;
    TBuf<TPosition::A1> srcBuf;
    tpipe.InitBuffer(srcBuf, srcSpanBytes);
    LocalTensor<T> src = srcBuf.Get<T>();

    TBuf<TPosition::C2PT> dstBuf;
    tpipe.InitBuffer(dstBuf, kPtPoolBytes);
    LocalTensor<T> dst = dstBuf.Get<T>();

    uint8_t* srcP = reinterpret_cast<uint8_t*>(src.GetPhyAddr());
    uint8_t* dstP = reinterpret_cast<uint8_t*>(dst.GetPhyAddr());
    for (size_t i = 0; i < srcSpanBytes; ++i) {
        srcP[i] = static_cast<uint8_t>(i & 0xFF);
    }
    memset(dstP, 0xCA, kPtPoolBytes);

    DataCopyParams ptDc{pt.nBurst, pt.lenBurst, pt.srcGap, pt.dstGap};
    DataCopy(dst, src, ptDc);
    PipeBarrier<PIPE_ALL>();

    for (size_t i = 0; i < srcSpanBytes; ++i) {
        EXPECT_EQ(srcP[i], static_cast<uint8_t>(i & 0xFF))
            << tag << "/src: window modified at offset " << i << " (read-only on this leg's model path)";
    }

    // dst : [0, span) 清零, [span, pool) 哨兵完好 -- 死腿留预填充, 跨度回归毁界外哨兵。
    const size_t spanBytes =
        static_cast<size_t>(pt.nBurst) * pt.lenBurst * 32 + static_cast<size_t>(pt.nBurst - 1) * pt.dstGap * 32;
    for (size_t i = 0; i < kPtPoolBytes; ++i) {
        if (i < spanBytes) {
            EXPECT_EQ(dstP[i], 0x00) << tag << "/dst: footprint byte @+" << i << " not zeroed (0x" << hex
                                     << static_cast<int>(dstP[i]) << ") -- dead leg kept the 0xCA prefill";
        } else {
            EXPECT_EQ(dstP[i], 0xCA) << tag << "/dst: sentinel beyond the footprint @+" << i
                                     << " violated -- model write extent regression";
        }
    }
}

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_ND2NZ_GM2L1, TestNd2NzGm2L1Suite,
    ::testing::Values(
        TypeCase{512, 2, nd2nz_gm2l1_kernel<half>},
        TypeCase{1024, 1, nd2nz_gm2l1_kernel<int8_t>},
        TypeCase{512, 2, nd2nz_gm2l1_kernel<int16_t>},
        TypeCase{512, 2, nd2nz_gm2l1_kernel<uint16_t>}));

TEST_P(TestNd2NzGm2L1Suite, Nd2NzGm2L1Cases)
{
    auto param = GetParam();
    // GM->L1 腿只读宿主 GM; L1 目的在模型内部, 递增值存续即全部契约。
    const size_t byteSize = static_cast<size_t>(param.dataSize) * param.typeSize;
    uint8_t srcGm[byteSize];
    for (size_t i = 0; i < byteSize; ++i) {
        srcGm[i] = static_cast<uint8_t>(i & 0xFF);
    }

    param.calFunc(srcGm, param.dataSize);

    for (size_t i = 0; i < byteSize; ++i) {
        EXPECT_EQ(srcGm[i], static_cast<uint8_t>(i & 0xFF))
            << "nd2nz: srcGm modified at offset " << i
            << " (GM->L1 leg wrote the read-only source buffer)";
    }
}

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_DN2NZ_GM2L1, TestDn2NzGm2L1Suite,
    ::testing::Values(
        TypeCase{512, 2, dn2nz_gm2l1_kernel<half>},
        TypeCase{1024, 1, dn2nz_gm2l1_kernel<int8_t>},
        TypeCase{512, 2, dn2nz_gm2l1_kernel<int16_t>},
        TypeCase{512, 2, dn2nz_gm2l1_kernel<uint16_t>}));

TEST_P(TestDn2NzGm2L1Suite, Dn2NzGm2L1Cases)
{
    auto param = GetParam();
    const size_t byteSize = static_cast<size_t>(param.dataSize) * param.typeSize;
    uint8_t srcGm[byteSize];
    for (size_t i = 0; i < byteSize; ++i) {
        srcGm[i] = static_cast<uint8_t>(i & 0xFF);
    }

    param.calFunc(srcGm, param.dataSize);

    for (size_t i = 0; i < byteSize; ++i) {
        EXPECT_EQ(srcGm[i], static_cast<uint8_t>(i & 0xFF))
            << "dn2nz: srcGm modified at offset " << i
            << " (GM->L1 leg wrote the read-only source buffer)";
    }
}

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_NZ2ND_L12GM, TestNz2NdL12GmSuite,
    ::testing::Values(
        TypeCase{512, 2, nz2nd_l12gm_kernel<half>},
        TypeCase{1024, 1, nz2nd_l12gm_kernel<int8_t>},
        TypeCase{512, 2, nz2nd_l12gm_kernel<int16_t>},
        TypeCase{512, 2, nz2nd_l12gm_kernel<uint16_t>}));

TEST_P(TestNz2NdL12GmSuite, Nz2NdL12GmCases)
{
    auto param = GetParam();
    const size_t byteSize = static_cast<size_t>(param.dataSize) * param.typeSize;
    constexpr size_t kHardBoundBytes = 512; // L1->GM 塔合法模型写入跨度不得越过此上界
    uint8_t dstGm[byteSize];
    memset(dstGm, 0xCA, byteSize);

    param.calFunc(dstGm, param.dataSize);

    // L1->GM 清零或哨兵契约: kHardBoundBytes 内每字节读 0x00 (已清) 或 0xCA (未触);
    // 界外哨兵须存续 (越界绊线), 至少一个 0x00 字节证明腿已运行。
    size_t zeroBytes = 0;
    for (size_t i = 0; i < byteSize; ++i) {
        if (dstGm[i] == 0x00) {
            ++zeroBytes;
        }
        if (i < kHardBoundBytes) {
            EXPECT_TRUE(dstGm[i] == 0x00 || dstGm[i] == 0xCA)
                << "nz2nd: dst byte at offset " << i << " is neither zeroed nor sentinel (0x"
                << hex << static_cast<int>(dstGm[i]) << ") -- real data crossed the model boundary";
        } else {
            EXPECT_EQ(dstGm[i], 0xCA)
                << "nz2nd: sentinel violated BEYOND the hard bound at offset " << i
                << " -- model write extent regression";
        }
    }
    EXPECT_GT(zeroBytes, static_cast<size_t>(0))
        << "nz2nd: dst was never zeroed -- leg early-returned without reaching the model";
}

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_NZ2DN_L12GM, TestNz2DnL12GmSuite,
    ::testing::Values(
        TypeCase{512, 2, nz2dn_l12gm_kernel<half>},
        TypeCase{1024, 1, nz2dn_l12gm_kernel<int8_t>},
        TypeCase{512, 2, nz2dn_l12gm_kernel<int16_t>},
        TypeCase{512, 2, nz2dn_l12gm_kernel<uint16_t>}));

TEST_P(TestNz2DnL12GmSuite, Nz2DnL12GmCases)
{
    auto param = GetParam();
    const size_t byteSize = static_cast<size_t>(param.dataSize) * param.typeSize;
    constexpr size_t kHardBoundBytes = 512; // L1->GM 塔合法模型写入跨度不得越过此上界
    uint8_t dstGm[byteSize];
    memset(dstGm, 0xCA, byteSize);

    param.calFunc(dstGm, param.dataSize);

    size_t zeroBytes = 0;
    for (size_t i = 0; i < byteSize; ++i) {
        if (dstGm[i] == 0x00) {
            ++zeroBytes;
        }
        if (i < kHardBoundBytes) {
            EXPECT_TRUE(dstGm[i] == 0x00 || dstGm[i] == 0xCA)
                << "nz2dn: dst byte at offset " << i << " is neither zeroed nor sentinel (0x"
                << hex << static_cast<int>(dstGm[i]) << ") -- real data crossed the model boundary";
        } else {
            EXPECT_EQ(dstGm[i], 0xCA)
                << "nz2dn: sentinel violated BEYOND the hard bound at offset " << i
                << " -- model write extent regression";
        }
    }
    EXPECT_GT(zeroBytes, static_cast<size_t>(0))
        << "nz2dn: dst was never zeroed -- leg early-returned without reaching the model";
}

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_L12BT, TestL12BtSuite,
    ::testing::Values(
        BtCase{l12bt_kernel<int32_t, int32_t>},
        BtCase{l12bt_kernel<int32_t, half>},
        BtCase{l12bt_kernel<int32_t, float>}));

TEST_P(TestL12BtSuite, L12BtCases)
{
    auto param = GetParam();
    param.calFunc();
}

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_L12FB, TestL12FbSuite,
    ::testing::Values(
        FbCase{64, l12fb_kernel<half>}));

TEST_P(TestL12FbSuite, L12FbCases)
{
    auto param = GetParam();
    param.calFunc(string("l12fb"), param.dataSize);
}

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_L12PT, TestL12PtSuite,
    ::testing::Values(
        PtCase{"single_burst_min_32B", PtParams{1, 1, 0, 0}, l12pt_kernel<half>},
        PtCase{"single_burst_pool_fit_256B", PtParams{1, 8, 0, 0}, l12pt_kernel<half>},
        PtCase{"burst2_dst_gap_160B", PtParams{2, 2, 0, 1}, l12pt_kernel<half>},
        PtCase{"burst2_src_gap_256B", PtParams{2, 4, 1, 0}, l12pt_kernel<half>}));

TEST_P(TestL12PtSuite, L12PtCases)
{
    auto param = GetParam();
    param.calFunc(string("l12pt/") + param.caseName, param.params);
}
