/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*
 * =============================================================================================
 * Pointer-operand UT for the DMA Basic API pointer extension (Ascend 950 / __NPU_ARCH__ == 3510).
 * =============================================================================================
 *
 * APIs under test
 *   DataCopy / DataCopyPad / DataCopyL1ToUB / DataCacheCleanAndInvalid / DataCachePreload
 *
 * Case numbering follows design document section 2.3.2:
 *   PTR-DC-01..07, PTR-PAD-01..02, PTR-L1UB-01, PTR-CACHE-01..02, PTR-MIX-01, PTR-MUTEX-01,
 *   plus PTR-REG-01 (compile-time overload-resolution regression).
 * Shape tiers: 1 / 32 / 1024 / 2048. dtypes: half / float / int32.
 *
 * What each case asserts
 *   Every kernel runs BOTH the pointer path and the equivalent Tensor path over the same global
 *   window, and writes the two results into two disjoint GM regions. The host side then asserts
 *   (1) pointer result == Tensor result, bit-exact, and (2) both == the source window, bit-exact.
 *   DMA performs no numeric transform, so byte equality is the correct criterion. This also
 *   regresses the pre-existing Tensor behaviour in the same run.
 *
 * How the pointer operands are obtained
 *   Under ASCENDC_CPU_DEBUG, `LocalTensor<T>::GetPhyAddr()` returns `PrimType*`, so
 *   `reinterpret_cast<__ubuf__ T*>(local.GetPhyAddr())` yields a genuine `__ubuf__ T*` operand.
 *   This is equivalent to the task-book form `__ubuf__ float xPtr[blockLength]`; the literal
 *   scalar-array form is not used because the CPU stub cannot declare address-space arrays.
 *
 * Compile-time gating
 *   The pointer overloads do not exist until the corresponding implementation lands. Every pointer
 *   call sits behind `if constexpr (HasXxx<...>::value)`, so this TU compiles both before and after
 *   the implementation arrives. Where a pointer overload is absent the case SKIPs with an explicit
 *   message rather than silently passing, so "green" can never be mistaken for "covered".
 */

#include <cstdint>
#include <cstring>
#include <iostream>
#include <type_traits>
#include <vector>

#include <gtest/gtest.h>
#include "kernel_operator.h"
#include "mockcpp/mockcpp.hpp"

using namespace std;
using namespace AscendC;

namespace {

constexpr uint32_t PTR_UT_BLK = ONE_BLK_SIZE; // 32B (renamed: BLK clashes with stub_fun.h Literal::BLK under the CPU-debug regime)

__aicore__ inline constexpr uint32_t BlkCeil(const uint32_t bytes) { return (bytes + PTR_UT_BLK - 1) / PTR_UT_BLK; }

// ---------------------------------------------------------------------------------------------
// Local-memory operand pairs (Tensor side + pointer side) on disjoint storage.
// Two TBufs of the same TPosition give two separate buffers, which is what makes the pointer /
// Tensor comparison meaningful: same call, same shape, different buffers.
// ---------------------------------------------------------------------------------------------

template <typename T, TPosition pos>
struct LocalPair {
    TBuf<pos> ptrBuf;
    TBuf<pos> tensorBuf;
    LocalTensor<T> ptrSide;
    LocalTensor<T> tensorSide;

    __aicore__ inline void Init(TPipe& pipe, const uint32_t elemCount)
    {
        pipe.InitBuffer(ptrBuf, elemCount * sizeof(T));
        pipe.InitBuffer(tensorBuf, elemCount * sizeof(T));
        ptrSide = ptrBuf.template Get<T>();
        tensorSide = tensorBuf.template Get<T>();
    }

    __aicore__ inline __ubuf__ T* UbPtr()
    {
        return reinterpret_cast<__ubuf__ T*>(const_cast<T*>(ptrSide.GetPhyAddr()));
    }
    __aicore__ inline __cbuf__ T* L1Ptr()
    {
        return reinterpret_cast<__cbuf__ T*>(const_cast<T*>(ptrSide.GetPhyAddr()));
    }
    __aicore__ inline __cc__ T* L0CPtr() { return reinterpret_cast<__cc__ T*>(const_cast<T*>(ptrSide.GetPhyAddr())); }
};

// ---------------------------------------------------------------------------------------------
// Compile-time detection of the pointer overloads (feeds both the skip logic and PTR-REG-01).
// `false` until the pointer extension lands; never a hard error.
// ---------------------------------------------------------------------------------------------

template <typename DstT, typename SrcT, typename = void>
struct HasDataCopyCount : std::false_type {};
template <typename DstT, typename SrcT>
struct HasDataCopyCount<
    DstT, SrcT,
    std::void_t<decltype(AscendC::DataCopy(std::declval<const DstT&>(), std::declval<const SrcT&>(), uint32_t(0)))>>
    : std::true_type {};

template <typename DstT, typename SrcT, typename = void>
struct HasDataCopyParams : std::false_type {};
template <typename DstT, typename SrcT>
struct HasDataCopyParams<
    DstT, SrcT,
    std::void_t<decltype(AscendC::DataCopy(
        std::declval<const DstT&>(), std::declval<const SrcT&>(), std::declval<const DataCopyParams&>()))>>
    : std::true_type {};

template <typename DstT, typename SrcT, typename = void>
struct HasDataCopyNd2Nz : std::false_type {};
template <typename DstT, typename SrcT>
struct HasDataCopyNd2Nz<
    DstT, SrcT,
    std::void_t<decltype(AscendC::DataCopy(
        std::declval<const DstT&>(), std::declval<const SrcT&>(), std::declval<const Nd2NzParams&>()))>>
    : std::true_type {};

template <typename DstT, typename SrcT, typename = void>
struct HasDataCopyNz2Nd : std::false_type {};
template <typename DstT, typename SrcT>
struct HasDataCopyNz2Nd<
    DstT, SrcT,
    std::void_t<decltype(AscendC::DataCopy(
        std::declval<const DstT&>(), std::declval<const SrcT&>(), std::declval<const Nz2NdParamsFull&>()))>>
    : std::true_type {};

template <typename DstT, typename SrcT, typename = void>
struct HasDataCopySlice : std::false_type {};
template <typename DstT, typename SrcT>
struct HasDataCopySlice<
    DstT, SrcT,
    std::void_t<decltype(AscendC::DataCopy(
        std::declval<const DstT&>(), std::declval<const SrcT&>(), std::declval<const SliceInfo*>(),
        std::declval<const SliceInfo*>(), uint32_t(2)))>> : std::true_type {};

/*
 * ND DMA: the overload carries both an element type and a dimension, so both are explicit template
 * parameters of the trait (all of them are deducible from the use site).
 */
template <typename T, uint8_t dim, typename DstT, typename SrcT, typename = void>
struct HasDataCopyNddma : std::false_type {};
template <typename T, uint8_t dim, typename DstT, typename SrcT>
struct HasDataCopyNddma<
    T, dim, DstT, SrcT,
    std::void_t<decltype(AscendC::DataCopy<T, dim>(
        std::declval<const DstT&>(), std::declval<const SrcT&>(), std::declval<const MultiCopyParams<T, dim>&>()))>>
    : std::true_type {};

template <typename T, typename DstT, typename SrcT, typename = void>
struct HasDataCopyPadExtParams : std::false_type {};
template <typename T, typename DstT, typename SrcT>
struct HasDataCopyPadExtParams<
    T, DstT, SrcT,
    std::void_t<decltype(AscendC::DataCopyPad(
        std::declval<const DstT&>(), std::declval<const SrcT&>(), std::declval<const DataCopyExtParams&>(),
        std::declval<const DataCopyPadExtParams<T>&>()))>> : std::true_type {};

template <typename DstT, typename SrcT, typename = void>
struct HasDataCopyPadExtNoPad : std::false_type {};
template <typename DstT, typename SrcT>
struct HasDataCopyPadExtNoPad<
    DstT, SrcT,
    std::void_t<decltype(AscendC::DataCopyPad(
        std::declval<const DstT&>(), std::declval<const SrcT&>(), std::declval<const DataCopyExtParams&>()))>>
    : std::true_type {};

template <typename DstT, typename SrcT, typename = void>
struct HasDataCopyL1ToUbCount : std::false_type {};
template <typename DstT, typename SrcT>
struct HasDataCopyL1ToUbCount<
    DstT, SrcT,
    std::void_t<decltype(AscendC::DataCopyL1ToUB(
        std::declval<const DstT&>(), std::declval<const SrcT&>(), uint32_t(0)))>> : std::true_type {};

template <typename DstT, typename SrcT, typename = void>
struct HasDataCopyL1ToUbParams : std::false_type {};
template <typename DstT, typename SrcT>
struct HasDataCopyL1ToUbParams<
    DstT, SrcT,
    std::void_t<decltype(AscendC::DataCopyL1ToUB(
        std::declval<const DstT&>(), std::declval<const SrcT&>(), std::declval<const DataCopyParams&>()))>>
    : std::true_type {};

/*
 * Cache APIs currently only accept Tensor operands, so the pointer forms must be probed too: until
 * the extension lands these calls would be a hard instantiation error, not a skip.
 */
/*
 * The pointer form of `DataCacheCleanAndInvalid` is declared as `void DataCacheCleanAndInvalid(const T*)`,
 * so the address space is carried by the FIRST template argument rather than the parameter list. The element
 * type is therefore derived from the operand type here, which keeps the call sites' `<T, __gm__ T*>` spelling
 * valid; `T` is retained so existing spellings keep compiling.
 */
template <typename T, typename DstT, typename = void>
struct HasDcciPtr : std::false_type {};
template <typename T, typename DstT>
struct HasDcciPtr<
    T, DstT,
    std::void_t<decltype(AscendC::DataCacheCleanAndInvalid< //
                         std::remove_pointer_t<DstT>, CacheLine::ENTIRE_DATA_CACHE, DcciDst::CACHELINE_OUT>(
        std::declval<const DstT&>()))>> : std::true_type {};

template <typename SrcT, typename OffsetT, typename = void>
struct HasDataCachePreload : std::false_type {};
template <typename SrcT, typename OffsetT>
struct HasDataCachePreload<
    SrcT, OffsetT,
    std::void_t<decltype(AscendC::DataCachePreload(std::declval<const SrcT&>(), std::declval<OffsetT>()))>>
    : std::true_type {};

/*
 * `DataCachePreload`'s pointer form takes `__gm__ uint64_t*`. A literal call with that argument type
 * would be a non-dependent expression, resolved at template-definition time, which defeats the
 * `if constexpr` guard below. The cast target is routed through a dependent alias so name lookup is
 * deferred to instantiation, and instantiation only happens when the pointer overload exists.
 */
template <typename T>
struct PreloadPtrOf {
    using Type = __gm__ uint64_t*;
};

template <typename T>
__aicore__ inline void CallDataCachePreload(__gm__ T* p)
{
    DataCachePreload(reinterpret_cast<typename PreloadPtrOf<T>::Type>(p), static_cast<int16_t>(10));
}

// ---------------------------------------------------------------------------------------------
// Shared comparison helper.
// Kernel output layout: [0, bytes) = pointer path, [bytes, 2*bytes) = Tensor path.
// ---------------------------------------------------------------------------------------------

inline void ExpectPtrEqualsTensor(
    const uint8_t* dstGm, const uint8_t* srcGm, const uint32_t bytes, const uint32_t srcOffsetBytes = 0)
{
    const uint8_t* ptrResult = dstGm;
    const uint8_t* tensorResult = dstGm + bytes;
    const uint8_t* expected = srcGm + srcOffsetBytes;

    EXPECT_EQ(std::memcmp(ptrResult, expected, bytes), 0) << "pointer path diverged from the source window";
    EXPECT_EQ(std::memcmp(tensorResult, expected, bytes), 0) << "tensor path diverged from the source window";
    EXPECT_EQ(std::memcmp(ptrResult, tensorResult, bytes), 0) << "pointer path != tensor path";
}

// Fills a deterministic byte ramp so a mis-ordered or shifted copy cannot pass by accident.
inline void FillRamp(std::vector<uint8_t>& buf, const uint32_t mul, const uint32_t add)
{
    for (uint32_t i = 0; i < buf.size(); i++) {
        buf[i] = static_cast<uint8_t>((i * mul + add) & 0xFF);
    }
}

/*
 * `GlobalTensor<T>::operator[]` returns a `LocalTensor`, not a view onto GM, so a second GM region is
 * addressed by re-binding a GlobalTensor to `base + offset`. These two helpers express the
 * "second half of the output buffer" pattern used by every kernel below.
 */
template <typename T>
__aicore__ inline GlobalTensor<T> MakeGlobal(__gm__ uint8_t* gm, const uint32_t elemCount)
{
    GlobalTensor<T> t;
    t.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(gm), elemCount);
    return t;
}

template <typename T>
__aicore__ inline GlobalTensor<T> MakeGlobalAt(__gm__ uint8_t* gm, const uint32_t elemOffset, const uint32_t elemCount)
{
    GlobalTensor<T> t;
    t.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(gm) + elemOffset, elemCount);
    return t;
}

} // namespace

// =============================================================================================
// PTR-DC-01  DataCopy GM -> UB, count mode:  DataCopy(ubPtr, gmPtr + offset, count)
// PTR-DC-03  DataCopy UB -> GM, count mode:  DataCopy(gmPtr + offset, ubPtr, count)
// =============================================================================================

template <typename T>
__global__ __aicore__ void KernelPtrDcCount(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, const uint32_t count,
    const uint32_t gmOffsetElems, const bool ubToGm)
{
    const uint32_t bytes = count * sizeof(T);
    TPipe tpipe;
    LocalPair<T, TPosition::VECCALC> ub;
    ub.Init(tpipe, count);

    GlobalTensor<T> srcWindow = MakeGlobalAt<T>(srcGm, gmOffsetElems, count);
    GlobalTensor<T> outPtrHalf = MakeGlobal<T>(dstGm, count);
    GlobalTensor<T> outTensorHalf = MakeGlobalAt<T>(dstGm, count, count);

    if (ubToGm) {
        // seed both UB buffers from the source window through the (existing) Tensor path
        DataCopy(ub.ptrSide, srcWindow, count);
        DataCopy(ub.tensorSide, srcWindow, count);
        pipe_barrier(PIPE_ALL);

        // ---- pointer path: UB pointer -> GM pointer ----
        if constexpr (HasDataCopyCount<__gm__ T*, __ubuf__ T*>::value) {
            DataCopy(reinterpret_cast<__gm__ T*>(dstGm), ub.UbPtr(), count);
        }
        // ---- Tensor path: identical UB content -> disjoint GM region ----
        DataCopy(outTensorHalf, ub.tensorSide, count);
        pipe_barrier(PIPE_ALL);
        (void)bytes;
        return;
    }

    // ---- pointer path: GM pointer (+ element offset) -> UB pointer ----
    if constexpr (HasDataCopyCount<__ubuf__ T*, __gm__ T*>::value) {
        DataCopy(ub.UbPtr(), reinterpret_cast<__gm__ T*>(srcGm) + gmOffsetElems, count);
    }
    // ---- Tensor path: same window -> second UB buffer ----
    DataCopy(ub.tensorSide, srcWindow, count);
    pipe_barrier(PIPE_ALL);

    DataCopy(outPtrHalf, ub.ptrSide, count);
    DataCopy(outTensorHalf, ub.tensorSide, count);
    pipe_barrier(PIPE_ALL);
}

struct PtrCountParams {
    uint32_t count;
    uint32_t typeSize;
    bool ubToGm;
    void (*calFunc)(uint8_t*, uint8_t*, uint32_t, uint32_t, bool);
};

class PtrDataCopyCountSuite : public testing::TestWithParam<PtrCountParams> {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override
    {
        GlobalMockObject::verify();
        SetGCoreType(0);
    }
};

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_PTR_COUNT, PtrDataCopyCountSuite,
    ::testing::Values(
        // shape tiers 1 / 32 / 1024 / 2048 x dtype half / float / int32, both directions
        PtrCountParams{1, 2, false, KernelPtrDcCount<half>}, PtrCountParams{32, 2, false, KernelPtrDcCount<half>},
        PtrCountParams{1024, 2, false, KernelPtrDcCount<half>}, PtrCountParams{2048, 2, false, KernelPtrDcCount<half>},
        PtrCountParams{1, 4, false, KernelPtrDcCount<float>}, PtrCountParams{32, 4, false, KernelPtrDcCount<float>},
        PtrCountParams{1024, 4, false, KernelPtrDcCount<float>},
        PtrCountParams{2048, 4, false, KernelPtrDcCount<float>}, PtrCountParams{1, 4, false, KernelPtrDcCount<int32_t>},
        PtrCountParams{32, 4, false, KernelPtrDcCount<int32_t>},
        PtrCountParams{1024, 4, false, KernelPtrDcCount<int32_t>},
        PtrCountParams{2048, 4, false, KernelPtrDcCount<int32_t>}, PtrCountParams{1, 2, true, KernelPtrDcCount<half>},
        PtrCountParams{32, 2, true, KernelPtrDcCount<half>}, PtrCountParams{1024, 2, true, KernelPtrDcCount<half>},
        PtrCountParams{2048, 2, true, KernelPtrDcCount<half>}, PtrCountParams{1, 4, true, KernelPtrDcCount<float>},
        PtrCountParams{32, 4, true, KernelPtrDcCount<float>}, PtrCountParams{1024, 4, true, KernelPtrDcCount<float>},
        PtrCountParams{2048, 4, true, KernelPtrDcCount<float>}, PtrCountParams{1, 4, true, KernelPtrDcCount<int32_t>},
        PtrCountParams{32, 4, true, KernelPtrDcCount<int32_t>},
        PtrCountParams{1024, 4, true, KernelPtrDcCount<int32_t>},
        PtrCountParams{2048, 4, true, KernelPtrDcCount<int32_t>}));

TEST_P(PtrDataCopyCountSuite, PtrCountModeMatchesTensorPath)
{
    auto param = GetParam();
    const bool available = param.ubToGm ? (HasDataCopyCount<__gm__ half*, __ubuf__ half*>::value ||
                                           HasDataCopyCount<__gm__ float*, __ubuf__ float*>::value ||
                                           HasDataCopyCount<__gm__ int32_t*, __ubuf__ int32_t*>::value) :
                                          (HasDataCopyCount<__ubuf__ half*, __gm__ half*>::value ||
                                           HasDataCopyCount<__ubuf__ float*, __gm__ float*>::value ||
                                           HasDataCopyCount<__ubuf__ int32_t*, __gm__ int32_t*>::value);
    if (!available) {
        GTEST_SKIP()
            << (param.ubToGm ? "pointer DataCopy UB->GM count overload absent (PTR-DC-03)" :
                               "pointer DataCopy GM->UB count overload absent (PTR-DC-01)");
    }

    const uint32_t bytes = param.count * param.typeSize;
    const uint32_t gmOffsetBytes = 64; // "gmPtr + offset": 64B into the source window
    std::vector<uint8_t> srcGm(bytes + gmOffsetBytes * 2, 0x00);
    std::vector<uint8_t> dstGm(static_cast<size_t>(bytes) * 2, 0x5A);
    FillRamp(srcGm, 7, 3);

    param.calFunc(srcGm.data(), dstGm.data(), param.count, gmOffsetBytes / param.typeSize, param.ubToGm);

    ExpectPtrEqualsTensor(dstGm.data(), srcGm.data(), bytes, gmOffsetBytes);
}

// =============================================================================================
// PTR-MIX-01  Tensor / pointer mixed operands: (ubPtr, gmTensor) and (gmTensor, ubPtr)
// =============================================================================================

template <typename T>
__global__ __aicore__ void KernelPtrMix(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, const uint32_t count)
{
    TPipe tpipe;
    LocalPair<T, TPosition::VECCALC> ub;
    ub.Init(tpipe, count);

    GlobalTensor<T> srcGlobal = MakeGlobal<T>(srcGm, count);
    GlobalTensor<T> outPtrHalf = MakeGlobal<T>(dstGm, count);
    GlobalTensor<T> outTensorHalf = MakeGlobalAt<T>(dstGm, count, count);

    // ---- mixed: UB pointer as dst, GlobalTensor as src ----
    if constexpr (HasDataCopyCount<__ubuf__ T*, GlobalTensor<T>>::value) {
        DataCopy(ub.UbPtr(), srcGlobal, count);
    }
    pipe_barrier(PIPE_ALL);
    // ---- all-Tensor path over the same window ----
    DataCopy(ub.tensorSide, srcGlobal, count);
    pipe_barrier(PIPE_ALL);

    // ---- mixed: GlobalTensor (GM) as dst, UB pointer as src ----
    if constexpr (HasDataCopyCount<GlobalTensor<T>, __ubuf__ T*>::value) {
        DataCopy(outPtrHalf, ub.UbPtr(), count);
    }
    // ---- all-Tensor path into a disjoint GM region ----
    DataCopy(outTensorHalf, ub.tensorSide, count);
    pipe_barrier(PIPE_ALL);
}

class PtrMixOperandSuite : public testing::Test {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

TEST_F(PtrMixOperandSuite, PTR_MIX_01_TensorAndPointerMixedOperands)
{
    if (!HasDataCopyCount<__ubuf__ float*, GlobalTensor<float>>::value) {
        GTEST_SKIP() << "mixed Tensor/pointer DataCopy overload absent (PTR-MIX-01)";
    }
    constexpr uint32_t count = 1024;
    const uint32_t bytes = count * sizeof(float);
    std::vector<uint8_t> srcGm(bytes, 0x00);
    std::vector<uint8_t> dstGm(static_cast<size_t>(bytes) * 2, 0x5A);
    FillRamp(srcGm, 5, 1);

    KernelPtrMix<float>(srcGm.data(), dstGm.data(), count);

    ExpectPtrEqualsTensor(dstGm.data(), srcGm.data(), bytes);
}

// =============================================================================================
// PTR-DC-02  GM -> L1 ND2NZ:   DataCopy(l1Ptr, gmPtr, nd2nzParams)
// PTR-DC-05  UB -> L1 ND2NZ:   DataCopy(l1Ptr, ubPtr, nd2nzParams)   (TSCM)
// =============================================================================================

template <typename T>
__global__ __aicore__ void KernelPtrDcGm2L1Nd2Nz(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, const uint32_t ndNum, const uint32_t nValue,
    const uint32_t dValue)
{
    const uint32_t elems = ndNum * nValue * dValue;
    TPipe tpipe;
    LocalPair<T, TPosition::A1> l1;
    l1.Init(tpipe, elems);

    GlobalTensor<T> srcGlobal = MakeGlobal<T>(srcGm, elems);
    GlobalTensor<T> outPtrHalf = MakeGlobal<T>(dstGm, elems);
    GlobalTensor<T> outTensorHalf = MakeGlobalAt<T>(dstGm, elems, elems);

    Nd2NzParams intriParams{
        static_cast<uint16_t>(ndNum),  static_cast<uint16_t>(nValue), dValue, static_cast<uint64_t>(nValue * dValue),
        static_cast<uint64_t>(dValue), static_cast<uint16_t>(dValue), 1,      0};

    // ---- pointer path: L1 pointer as dst, GM pointer as src ----
    if constexpr (HasDataCopyNd2Nz<__cbuf__ T*, __gm__ T*>::value) {
        DataCopy(l1.L1Ptr(), reinterpret_cast<__gm__ T*>(srcGm), intriParams);
    }
    pipe_barrier(PIPE_ALL);
    // ---- Tensor path over the identical GM window ----
    DataCopy(l1.tensorSide, srcGlobal, intriParams);
    pipe_barrier(PIPE_ALL);

    // Both L1 buffers are drained to GM through the Tensor path; only the GM bytes are compared.
    DataCopy(outPtrHalf, l1.ptrSide, elems);
    DataCopy(outTensorHalf, l1.tensorSide, elems);
    pipe_barrier(PIPE_ALL);
}

template <typename T>
__global__ __aicore__ void KernelPtrDcUb2L1Nd2Nz(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, const uint32_t nValue,
    const uint32_t dValue)
{
    const uint32_t elems = nValue * dValue;
    TPipe tpipe;
    LocalPair<T, TPosition::VECCALC> ub;
    ub.Init(tpipe, elems);
    LocalPair<T, TPosition::TSCM> l1; // UB -> L1 goes through the TSCM position
    l1.Init(tpipe, elems);

    GlobalTensor<T> srcGlobal = MakeGlobal<T>(srcGm, elems);
    GlobalTensor<T> outPtrHalf = MakeGlobal<T>(dstGm, elems);
    GlobalTensor<T> outTensorHalf = MakeGlobalAt<T>(dstGm, elems, elems);

    DataCopy(ub.ptrSide, srcGlobal, elems);
    DataCopy(ub.tensorSide, srcGlobal, elems);
    pipe_barrier(PIPE_ALL);

    Nd2NzParams intriParams{
        1,
        static_cast<uint16_t>(nValue),
        dValue,
        static_cast<uint64_t>(nValue * dValue),
        static_cast<uint64_t>(dValue),
        static_cast<uint16_t>(dValue),
        1,
        0};

    // ---- pointer path: UB pointer -> L1 (TSCM) pointer ----
    if constexpr (HasDataCopyNd2Nz<__cbuf__ T*, __ubuf__ T*>::value) {
        DataCopy(l1.L1Ptr(), ub.UbPtr(), intriParams);
    }
    pipe_barrier(PIPE_ALL);
    // ---- Tensor path: identical UB content -> second L1 buffer ----
    DataCopy(l1.tensorSide, ub.tensorSide, intriParams);
    pipe_barrier(PIPE_ALL);

    DataCopy(outPtrHalf, l1.ptrSide, elems);
    DataCopy(outTensorHalf, l1.tensorSide, elems);
    pipe_barrier(PIPE_ALL);
}

class PtrDcNd2NzSuite : public testing::Test {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

TEST_F(PtrDcNd2NzSuite, PTR_DC_02_Gm2L1Nd2NzHalf32x32)
{
    if (!HasDataCopyNd2Nz<__cbuf__ half*, __gm__ half*>::value) {
        GTEST_SKIP() << "pointer DataCopy GM->L1 ND2NZ overload absent (PTR-DC-02)";
    }
    constexpr uint32_t ndNum = 1;
    constexpr uint32_t nValue = 32;
    constexpr uint32_t dValue = 32;
    const uint32_t bytes = ndNum * nValue * dValue * sizeof(half);
    std::vector<uint8_t> srcGm(bytes, 0x00);
    std::vector<uint8_t> dstGm(static_cast<size_t>(bytes) * 2, 0x5A);
    FillRamp(srcGm, 3, 11);

    KernelPtrDcGm2L1Nd2Nz<half>(srcGm.data(), dstGm.data(), ndNum, nValue, dValue);

    ExpectPtrEqualsTensor(dstGm.data(), srcGm.data(), bytes);
}

TEST_F(PtrDcNd2NzSuite, PTR_DC_05_Ub2L1Nd2NzTscm)
{
    if (!HasDataCopyNd2Nz<__cbuf__ float*, __ubuf__ float*>::value) {
        GTEST_SKIP() << "pointer DataCopy UB->L1 ND2NZ overload absent (PTR-DC-05)";
    }
    constexpr uint32_t nValue = 16;
    constexpr uint32_t dValue = 16;
    const uint32_t bytes = nValue * dValue * sizeof(float);
    std::vector<uint8_t> srcGm(bytes, 0x00);
    std::vector<uint8_t> dstGm(static_cast<size_t>(bytes) * 2, 0x5A);
    FillRamp(srcGm, 13, 7);

    KernelPtrDcUb2L1Nd2Nz<float>(srcGm.data(), dstGm.data(), nValue, dValue);

    ExpectPtrEqualsTensor(dstGm.data(), srcGm.data(), bytes);
}

// =============================================================================================
// PTR-DC-04  L0C -> GM NZ2ND:  DataCopy(gmPtr, l0cPtr, nz2ndParams)
// =============================================================================================

template <typename T>
__global__ __aicore__ void KernelPtrDcL0c2GmNz2Nd(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, const uint32_t nValue,
    const uint32_t dValue)
{
    const uint32_t elems = nValue * dValue;
    TPipe tpipe;
    LocalPair<T, TPosition::VECCALC> ub;
    ub.Init(tpipe, elems);
    LocalPair<T, TPosition::CO1> l0c;
    l0c.Init(tpipe, elems);

    GlobalTensor<T> srcGlobal = MakeGlobal<T>(srcGm, elems);
    GlobalTensor<T> outTensorHalf = MakeGlobalAt<T>(dstGm, elems, elems);

    DataCopy(ub.ptrSide, srcGlobal, elems);
    DataCopy(ub.tensorSide, srcGlobal, elems);
    pipe_barrier(PIPE_ALL);
    // materialise the same L0C content on both sides through the Tensor path (L0C transfers use the
    // block-parameter form, not the count form)
    DataCopyParams l0cParams{static_cast<uint16_t>(BlkCeil(elems * sizeof(T))), 1, 0, 0};
    DataCopy(l0c.ptrSide, ub.ptrSide, l0cParams);
    DataCopy(l0c.tensorSide, ub.tensorSide, l0cParams);
    pipe_barrier(PIPE_ALL);

    Nz2NdParamsFull nz2ndParams{1, static_cast<uint16_t>(nValue), static_cast<uint16_t>(dValue), 1, 0, 0, 1};

    // ---- pointer path: L0C pointer -> GM pointer ----
    if constexpr (HasDataCopyNz2Nd<__gm__ T*, __cc__ T*>::value) {
        DataCopy(reinterpret_cast<__gm__ T*>(dstGm), l0c.L0CPtr(), nz2ndParams);
    }
    pipe_barrier(PIPE_ALL);
    // ---- Tensor path: identical L0C content -> disjoint GM region ----
    DataCopy(outTensorHalf, l0c.tensorSide, nz2ndParams);
    pipe_barrier(PIPE_ALL);
}

class PtrDcL0c2GmSuite : public testing::Test {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

TEST_F(PtrDcL0c2GmSuite, PTR_DC_04_L0c2GmNz2NdFloat16x16)
{
    if (!HasDataCopyNz2Nd<__gm__ float*, __cc__ float*>::value) {
        GTEST_SKIP() << "pointer DataCopy L0C->GM NZ2ND overload absent (PTR-DC-04)";
    }
    constexpr uint32_t nValue = 16;
    constexpr uint32_t dValue = 16;
    const uint32_t bytes = nValue * dValue * sizeof(float);
    std::vector<uint8_t> srcGm(bytes, 0x00);
    std::vector<uint8_t> dstGm(static_cast<size_t>(bytes) * 2, 0x5A);
    FillRamp(srcGm, 17, 5);

    KernelPtrDcL0c2GmNz2Nd<float>(srcGm.data(), dstGm.data(), nValue, dValue);

    ExpectPtrEqualsTensor(dstGm.data(), srcGm.data(), bytes);
}

// =============================================================================================
// PTR-DC-06  multi-dim slice GM -> UB:  DataCopy(ubPtr, gmPtr, dstSlice, srcSlice, dim)
// =============================================================================================

template <typename T>
__global__ __aicore__ void KernelPtrDcSlice(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, const uint32_t srcSize,
    const uint32_t dstSize, __gm__ SliceInfo* dstSliceList, __gm__ SliceInfo* srcSliceList)
{
    TPipe tpipe;
    LocalPair<T, TPosition::VECCALC> ub;
    ub.Init(tpipe, dstSize);

    GlobalTensor<T> srcGlobal = MakeGlobal<T>(srcGm, srcSize);
    GlobalTensor<T> outPtrHalf = MakeGlobal<T>(dstGm, dstSize);
    GlobalTensor<T> outTensorHalf = MakeGlobalAt<T>(dstGm, dstSize, dstSize);

    // ---- pointer path: UB pointer as dst, GM pointer as src ----
    if constexpr (HasDataCopySlice<__ubuf__ T*, __gm__ T*>::value) {
        DataCopy(ub.UbPtr(), reinterpret_cast<__gm__ T*>(srcGm), dstSliceList, srcSliceList, 2);
    }
    pipe_barrier(PIPE_ALL);
    // ---- Tensor path over the same slices ----
    DataCopy(ub.tensorSide, srcGlobal, dstSliceList, srcSliceList, 2);
    pipe_barrier(PIPE_ALL);

    DataCopy(outPtrHalf, ub.ptrSide, dstSize);
    DataCopy(outTensorHalf, ub.tensorSide, dstSize);
    pipe_barrier(PIPE_ALL);
}

class PtrDcSliceSuite : public testing::Test {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

TEST_F(PtrDcSliceSuite, PTR_DC_06_MultiDimSliceGm2Ub)
{
    if (!HasDataCopySlice<__ubuf__ float*, __gm__ float*>::value) {
        GTEST_SKIP() << "pointer DataCopy slice overload absent (PTR-DC-06)";
    }
    constexpr uint32_t srcSize = 264;
    constexpr uint32_t dstSize = 96;
    const uint32_t bytes = dstSize * sizeof(float);
    std::vector<uint8_t> srcGm(srcSize * sizeof(float), 0x00);
    std::vector<uint8_t> dstGm(static_cast<size_t>(bytes) * 2, 0x5A);
    FillRamp(srcGm, 19, 2);

    __gm__ SliceInfo dstSliceInfo[2] = {{0, 47, 0, 3, 48}, {0, 1, 0, 1, 2}};
    __gm__ SliceInfo srcSliceInfo[2] = {{16, 71, 8, 3, 88}, {0, 2, 1, 1, 3}};

    KernelPtrDcSlice<float>(srcGm.data(), dstGm.data(), srcSize, dstSize, dstSliceInfo, srcSliceInfo);

    // A slice result is not a contiguous copy of the source, so the meaningful assertion is
    // pointer path == Tensor path over the produced regions.
    EXPECT_EQ(std::memcmp(dstGm.data(), dstGm.data() + bytes, bytes), 0) << "pointer path != tensor path";
}

// =============================================================================================
// PTR-DC-07  ND DMA GM -> UB:  DataCopy<T, dim, config>(ubPtr, gmPtr, params)
// =============================================================================================

template <typename T, uint8_t dim, bool enableNearest>
__global__ __aicore__ void KernelPtrDcNdDma(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, const uint32_t srcSize,
    const uint32_t dstSize, __gm__ MultiCopyLoopInfo<dim>* loopInfoPtr)
{
    TPipe tpipe;
    LocalPair<T, TPosition::VECCALC> ub;
    ub.Init(tpipe, dstSize);

    GlobalTensor<T> srcGlobal = MakeGlobal<T>(srcGm, srcSize);
    GlobalTensor<T> outPtrHalf = MakeGlobal<T>(dstGm, dstSize);
    GlobalTensor<T> outTensorHalf = MakeGlobalAt<T>(dstGm, dstSize, dstSize);

    MultiCopyLoopInfo<dim> loopInfo = *loopInfoPtr;
    constexpr uint16_t padValue = NdDmaConfig::unsetPad;
    static constexpr NdDmaConfig config = {enableNearest, padValue, padValue, false};
    MultiCopyParams<T, dim> params = {loopInfo, static_cast<T>(0)};

    // ---- pointer path (guarded: this overload is part of the extension) ----
    if constexpr (HasDataCopyNddma<T, dim, __ubuf__ T*, __gm__ T*>::value) {
        DataCopy<T, dim, config>(ub.UbPtr(), reinterpret_cast<__gm__ T*>(srcGm), params);
    }
    pipe_barrier(PIPE_ALL);
    // ---- Tensor path with identical params ----
    DataCopy<T, dim, config>(ub.tensorSide, srcGlobal, params);
    pipe_barrier(PIPE_ALL);

    DataCopy(outPtrHalf, ub.ptrSide, dstSize);
    DataCopy(outTensorHalf, ub.tensorSide, dstSize);
    pipe_barrier(PIPE_ALL);
}

class PtrDcNdDmaSuite : public testing::Test {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

TEST_F(PtrDcNdDmaSuite, PTR_DC_07_NdDmaGm2Ub2Dim)
{
    constexpr uint32_t srcSize = 30 * 120;
    constexpr uint32_t dstSize = 32 * 128;
    std::vector<uint8_t> srcGm(srcSize * sizeof(int32_t), 0x00);
    std::vector<uint8_t> dstGm(static_cast<size_t>(dstSize) * sizeof(int32_t) * 2, 0x5A);
    FillRamp(srcGm, 23, 9);

    MultiCopyLoopInfo<2> loopInfo{};
    loopInfo.loopSize[0] = 30;
    loopInfo.loopSize[1] = 120;
    loopInfo.loopSrcStride[0] = 1;
    loopInfo.loopSrcStride[1] = 30;
    loopInfo.loopDstStride[0] = 1;
    loopInfo.loopDstStride[1] = 32;
    loopInfo.loopLpSize[0] = 1;
    loopInfo.loopLpSize[1] = 3;
    loopInfo.loopRpSize[0] = 1;
    loopInfo.loopRpSize[1] = 5;

    if (!HasDataCopyNddma<int32_t, 2, __ubuf__ int32_t*, __gm__ int32_t*>::value) {
        GTEST_SKIP() << "pointer ND DMA DataCopy overload absent (PTR-DC-07)";
    }

    KernelPtrDcNdDma<int32_t, 2, true>(srcGm.data(), dstGm.data(), srcSize, dstSize, &loopInfo);

    const uint32_t bytes = dstSize * sizeof(int32_t);
    EXPECT_EQ(std::memcmp(dstGm.data(), dstGm.data() + bytes, bytes), 0) << "pointer path != tensor path";
}

// =============================================================================================
// PTR-PAD-01  DataCopyPad GM -> UB, non-aligned tail (+ pad value)
// PTR-PAD-02  DataCopyPad UB -> GM, non-aligned tail
// =============================================================================================

template <typename T>
__global__ __aicore__ void KernelPtrPadGm2Ub(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, const uint32_t elemCount,
    const uint32_t tailLen)
{
    TPipe tpipe;
    LocalPair<T, TPosition::VECCALC> ub;
    ub.Init(tpipe, elemCount + PTR_UT_BLK / sizeof(T));

    GlobalTensor<T> srcGlobal = MakeGlobal<T>(srcGm, elemCount);
    GlobalTensor<T> outPtrHalf = MakeGlobal<T>(dstGm, elemCount);
    GlobalTensor<T> outTensorHalf = MakeGlobalAt<T>(dstGm, elemCount, elemCount);

    DataCopyExtParams extParams{1, static_cast<uint32_t>(tailLen * sizeof(T)), 0, 0, 0};
    DataCopyPadExtParams<T> padParams{true, 0, 1, static_cast<T>(0)};

    // ---- pointer path ----
    if constexpr (HasDataCopyPadExtParams<T, __ubuf__ T*, __gm__ T*>::value) {
        DataCopyPad(ub.UbPtr(), reinterpret_cast<__gm__ T*>(srcGm), extParams, padParams);
    }
    pipe_barrier(PIPE_ALL);
    // ---- Tensor path with identical params ----
    DataCopyPad(ub.tensorSide, srcGlobal, extParams, padParams);
    pipe_barrier(PIPE_ALL);

    // Drain both valid regions back to GM. The tail is not 32B-aligned, so a whole number of
    // 32B blocks is written back; only [0, tailLen) is asserted by the host side.
    DataCopyParams outParams{1, static_cast<uint16_t>(BlkCeil(tailLen * sizeof(T))), 0, 0};
    DataCopy(outPtrHalf, ub.ptrSide, outParams);
    DataCopy(outTensorHalf, ub.tensorSide, outParams);
    pipe_barrier(PIPE_ALL);
}

template <typename T>
__global__ __aicore__ void KernelPtrPadUb2Gm(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, const uint32_t elemCount,
    const uint32_t tailLen)
{
    TPipe tpipe;
    LocalPair<T, TPosition::VECCALC> ub;
    ub.Init(tpipe, elemCount);

    GlobalTensor<T> srcGlobal = MakeGlobal<T>(srcGm, elemCount);
    GlobalTensor<T> outTensorHalf = MakeGlobalAt<T>(dstGm, elemCount, elemCount);

    DataCopy(ub.ptrSide, srcGlobal, elemCount);
    DataCopy(ub.tensorSide, srcGlobal, elemCount);
    pipe_barrier(PIPE_ALL);

    DataCopyExtParams extParams{1, static_cast<uint32_t>(tailLen * sizeof(T)), 0, 0, 0};

    // ---- pointer path: UB pointer -> GM pointer ----
    if constexpr (HasDataCopyPadExtNoPad<__gm__ T*, __ubuf__ T*>::value) {
        DataCopyPad(reinterpret_cast<__gm__ T*>(dstGm), ub.UbPtr(), extParams);
    }
    pipe_barrier(PIPE_ALL);
    // ---- Tensor path into a disjoint GM region ----
    DataCopyPad(outTensorHalf, ub.tensorSide, extParams);
    pipe_barrier(PIPE_ALL);
}

class PtrPadSuite : public testing::Test {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

TEST_F(PtrPadSuite, PTR_PAD_01_Gm2UbNonAlignedTailWithPadValue)
{
    if (!HasDataCopyPadExtParams<float, __ubuf__ float*, __gm__ float*>::value) {
        GTEST_SKIP() << "pointer DataCopyPad GM->UB overload absent (PTR-PAD-01)";
    }
    constexpr uint32_t elemCount = 64;
    constexpr uint32_t tailLen = 21; // 84B: deliberately not 32B-aligned for float
    const uint32_t validBytes = tailLen * sizeof(float);
    std::vector<uint8_t> srcGm(elemCount * sizeof(float), 0x00);
    std::vector<uint8_t> dstGm(static_cast<size_t>(elemCount) * sizeof(float) * 2, 0x5A);
    FillRamp(srcGm, 29, 4);

    KernelPtrPadGm2Ub<float>(srcGm.data(), dstGm.data(), elemCount, tailLen);

    ExpectPtrEqualsTensor(dstGm.data(), srcGm.data(), validBytes);
}

TEST_F(PtrPadSuite, PTR_PAD_02_Ub2GmNonAlignedTail)
{
    if (!HasDataCopyPadExtNoPad<__gm__ half*, __ubuf__ half*>::value) {
        GTEST_SKIP() << "pointer DataCopyPad UB->GM overload absent (PTR-PAD-02)";
    }
    constexpr uint32_t elemCount = 64;
    constexpr uint32_t tailLen = 21; // 42B: not 32B-aligned for half
    const uint32_t validBytes = tailLen * sizeof(half);
    std::vector<uint8_t> srcGm(elemCount * sizeof(half), 0x00);
    std::vector<uint8_t> dstGm(static_cast<size_t>(elemCount) * sizeof(half) * 2, 0x5A);
    FillRamp(srcGm, 31, 6);

    KernelPtrPadUb2Gm<half>(srcGm.data(), dstGm.data(), elemCount, tailLen);

    ExpectPtrEqualsTensor(dstGm.data(), srcGm.data(), validBytes);
}

// =============================================================================================
// PTR-L1UB-01  DataCopyL1ToUB, count form and DataCopyParams form
// =============================================================================================

template <typename T>
__global__ __aicore__ void KernelPtrL1ToUb(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, const uint32_t count, const bool paramsForm)
{
    TPipe tpipe;
    LocalPair<T, TPosition::VECCALC> ub;
    ub.Init(tpipe, count);
    LocalPair<T, TPosition::A1> l1;
    l1.Init(tpipe, count);

    GlobalTensor<T> srcGlobal = MakeGlobal<T>(srcGm, count);
    GlobalTensor<T> outPtrHalf = MakeGlobal<T>(dstGm, count);
    GlobalTensor<T> outTensorHalf = MakeGlobalAt<T>(dstGm, count, count);

    DataCopy(l1.ptrSide, srcGlobal, count);
    DataCopy(l1.tensorSide, srcGlobal, count);
    pipe_barrier(PIPE_ALL);

    if (paramsForm) {
        DataCopyParams repeatParams(1, static_cast<uint16_t>(count * sizeof(T) / PTR_UT_BLK), 0, 0);
        // ---- pointer path: L1 pointer -> UB pointer (DataCopyParams form) ----
        if constexpr (HasDataCopyL1ToUbParams<__ubuf__ T*, __cbuf__ T*>::value) {
            DataCopyL1ToUB(ub.UbPtr(), l1.L1Ptr(), repeatParams);
        }
        pipe_barrier(PIPE_ALL);
        DataCopyL1ToUB(ub.tensorSide, l1.tensorSide, repeatParams);
    } else {
        // ---- pointer path: L1 pointer -> UB pointer (count form) ----
        if constexpr (HasDataCopyL1ToUbCount<__ubuf__ T*, __cbuf__ T*>::value) {
            DataCopyL1ToUB(ub.UbPtr(), l1.L1Ptr(), count);
        }
        pipe_barrier(PIPE_ALL);
        DataCopyL1ToUB(ub.tensorSide, l1.tensorSide, count);
    }
    pipe_barrier(PIPE_ALL);

    DataCopy(outPtrHalf, ub.ptrSide, count);
    DataCopy(outTensorHalf, ub.tensorSide, count);
    pipe_barrier(PIPE_ALL);
}

class PtrL1ToUbSuite : public testing::TestWithParam<bool> {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

INSTANTIATE_TEST_CASE_P(TEST_PTR_L1UB, PtrL1ToUbSuite, ::testing::Values(false, true));

TEST_P(PtrL1ToUbSuite, PTR_L1UB_01_CountAndParamsForms)
{
    const bool paramsForm = GetParam();
    const bool available = paramsForm ? HasDataCopyL1ToUbParams<__ubuf__ float*, __cbuf__ float*>::value :
                                        HasDataCopyL1ToUbCount<__ubuf__ float*, __cbuf__ float*>::value;
    if (!available) {
        GTEST_SKIP() << "pointer DataCopyL1ToUB overload absent (PTR-L1UB-01), paramsForm=" << paramsForm;
    }
    constexpr uint32_t count = 512;
    const uint32_t bytes = count * sizeof(float);
    std::vector<uint8_t> srcGm(bytes, 0x00);
    std::vector<uint8_t> dstGm(static_cast<size_t>(bytes) * 2, 0x5A);
    FillRamp(srcGm, 37, 8);

    KernelPtrL1ToUb<float>(srcGm.data(), dstGm.data(), count, paramsForm);

    ExpectPtrEqualsTensor(dstGm.data(), srcGm.data(), bytes);
}

// =============================================================================================
// PTR-CACHE-01  DataCacheCleanAndInvalid on GM / UB pointers (data must stay intact)
// PTR-CACHE-02  DataCachePreload on a GM pointer
// =============================================================================================

template <typename T>
__global__ __aicore__ void KernelPtrCacheOps(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, const uint32_t count)
{
    TPipe tpipe;
    LocalPair<T, TPosition::VECCALC> ub;
    ub.Init(tpipe, count);

    GlobalTensor<T> srcGlobal = MakeGlobal<T>(srcGm, count);
    GlobalTensor<T> outPtrHalf = MakeGlobal<T>(dstGm, count);
    GlobalTensor<T> outTensorHalf = MakeGlobalAt<T>(dstGm, count, count);

    DataCopy(ub.ptrSide, srcGlobal, count);
    DataCopy(ub.tensorSide, srcGlobal, count);
    pipe_barrier(PIPE_ALL);

    // PTR-CACHE-01, GM specialisation on the raw global pointer
    if constexpr (HasDcciPtr<T, __gm__ T*>::value) {
        DataCacheCleanAndInvalid<__gm__ T, CacheLine::ENTIRE_DATA_CACHE, DcciDst::CACHELINE_OUT>(
            const_cast<__gm__ T*>(srcGlobal.GetPhyAddr()));
    }
    // PTR-CACHE-01, UB specialisation on the raw UB pointer
    if constexpr (HasDcciPtr<T, __ubuf__ T*>::value) {
        DataCacheCleanAndInvalid<__ubuf__ T, CacheLine::ENTIRE_DATA_CACHE, DcciDst::CACHELINE_OUT>(
            const_cast<__ubuf__ T*>(ub.ptrSide.GetPhyAddr()));
    }
    // PTR-CACHE-02, preload from a raw global pointer (see CallDataCachePreload for why it is wrapped)
    if constexpr (HasDataCachePreload<__gm__ uint64_t*, int16_t>::value) {
        CallDataCachePreload(srcGm);
    }
    pipe_barrier(PIPE_ALL);

    DataCopy(outPtrHalf, ub.ptrSide, count);
    DataCopy(outTensorHalf, ub.tensorSide, count);
    pipe_barrier(PIPE_ALL);
}

class PtrCacheSuite : public testing::Test {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

TEST_F(PtrCacheSuite, PTR_CACHE_01_02_PointerOpsKeepDataIntact)
{
    if (!HasDcciPtr<float, __gm__ float*>::value) {
        GTEST_SKIP() << "pointer DataCacheCleanAndInvalid overload absent (PTR-CACHE-01)";
    }
    constexpr uint32_t count = 256;
    const uint32_t bytes = count * sizeof(float);
    std::vector<uint8_t> srcGm(bytes, 0x00);
    std::vector<uint8_t> dstGm(static_cast<size_t>(bytes) * 2, 0x5A);
    FillRamp(srcGm, 41, 12);

    KernelPtrCacheOps<float>(srcGm.data(), dstGm.data(), count);

    ExpectPtrEqualsTensor(dstGm.data(), srcGm.data(), bytes);
}

// =============================================================================================
// PTR-MUTEX-01  task-book paradigm: Mutex::Lock/Unlock<PIPE_MTE2/MTE3> + pointer movement
// =============================================================================================

template <typename T>
__global__ __aicore__ void KernelPtrMutexDma(
    __gm__ uint8_t* __restrict__ x, __gm__ uint8_t* __restrict__ z, const uint32_t blockLength)
{
    TPipe tpipe;
    LocalPair<T, TPosition::VECCALC> ub;
    ub.Init(tpipe, blockLength);

    GlobalTensor<T> xGlobal = MakeGlobal<T>(x, blockLength);

    const uint32_t offset = 0; // single logical block (block_idx == 0 on CPU debug)
    const uint8_t mutexId = 0;

    Mutex::Lock<PIPE_MTE2>(mutexId);
    if constexpr (HasDataCopyCount<__ubuf__ T*, __gm__ T*>::value) {
        DataCopy(ub.UbPtr(), reinterpret_cast<__gm__ T*>(x) + offset, blockLength);
    }
    Mutex::Unlock<PIPE_MTE2>(mutexId);

    // Tensor-path equivalent, filling the second UB buffer for comparison
    DataCopy(ub.tensorSide, xGlobal, blockLength);
    pipe_barrier(PIPE_ALL);

    Mutex::Lock<PIPE_MTE3>(mutexId);
    if constexpr (HasDataCopyCount<__gm__ T*, __ubuf__ T*>::value) {
        DataCopy(reinterpret_cast<__gm__ T*>(z) + offset, ub.UbPtr(), blockLength);
    }
    Mutex::Unlock<PIPE_MTE3>(mutexId);

    // write the Tensor-path result into the second half of z for the pointer/Tensor comparison
    DataCopy(MakeGlobalAt<T>(z, blockLength, blockLength), ub.tensorSide, blockLength);
    pipe_barrier(PIPE_ALL);
}

class PtrMutexDmaSuite : public testing::Test {
protected:
    void SetUp() override { SetGCoreType(2); }
    void TearDown() override { SetGCoreType(0); }
};

TEST_F(PtrMutexDmaSuite, PTR_MUTEX_01_TaskBookParadigmPointerMovement)
{
    if (!HasDataCopyCount<__ubuf__ float*, __gm__ float*>::value) {
        GTEST_SKIP() << "pointer DataCopy count overload absent (PTR-MUTEX-01)";
    }
    constexpr uint32_t blockLength = 1024;
    const uint32_t bytes = blockLength * sizeof(float);
    std::vector<uint8_t> x(bytes, 0x00);
    std::vector<uint8_t> z(static_cast<size_t>(bytes) * 2, 0x5A);
    FillRamp(x, 43, 15);

    KernelPtrMutexDma<float>(x.data(), z.data(), blockLength);

    // pointer path result (first block) vs Tensor path result (second block), and vs the source
    ExpectPtrEqualsTensor(z.data(), x.data(), bytes);
}

// =============================================================================================
// PTR-REG-01  compile-time overload-resolution regression
//
// The Tensor signatures must keep resolving exactly as before the extension, and every pointer
// overload from design section 2.2.1 must become reachable once it lands. The probes below are
// enabled only when the corresponding overload exists, so this TU compiles in both states.
// =============================================================================================

namespace {

/*
 * Overload-resolution probes. These are ordinary templates: their bodies instantiate only when
 * reached, and every call site below is inside `if constexpr (HasXxx<...>::value)`. A pointer
 * overload that exists but is ambiguous or internally broken therefore fails to compile here.
 */
template <typename T>
__aicore__ inline void ResolutionProbeCount()
{
    __ubuf__ T* ubPtr = reinterpret_cast<__ubuf__ T*>(0x2000);
    __gm__ T* gmPtr = reinterpret_cast<__gm__ T*>(0x1000);
    DataCopy(ubPtr, gmPtr, uint32_t(32)); // GM -> UB
    DataCopy(gmPtr, ubPtr, uint32_t(32)); // UB -> GM
}

template <typename T>
__aicore__ inline void ResolutionProbeParams()
{
    __ubuf__ T* ubPtr = reinterpret_cast<__ubuf__ T*>(0x2000);
    __gm__ T* gmPtr = reinterpret_cast<__gm__ T*>(0x1000);
    __cbuf__ T* l1Ptr = reinterpret_cast<__cbuf__ T*>(0x3000);
    DataCopyParams repeatParams{1, 4, 0, 0};
    DataCopy(ubPtr, gmPtr, repeatParams);
    DataCopy(gmPtr, ubPtr, repeatParams);
    DataCopy(l1Ptr, gmPtr, repeatParams);
}

template <typename T>
__aicore__ inline void ResolutionProbeNd2Nz()
{
    __cbuf__ T* l1Ptr = reinterpret_cast<__cbuf__ T*>(0x3000);
    __ubuf__ T* ubPtr = reinterpret_cast<__ubuf__ T*>(0x2000);
    __gm__ T* gmPtr = reinterpret_cast<__gm__ T*>(0x1000);
    Nd2NzParams nd2nzParams{1, 16, 16, 16, 16, 16, 1, 16};
    DataCopy(l1Ptr, gmPtr, nd2nzParams);
    DataCopy(l1Ptr, ubPtr, nd2nzParams);
}

template <typename T>
__aicore__ inline void ResolutionProbeNz2Nd()
{
    __gm__ T* gmPtr = reinterpret_cast<__gm__ T*>(0x1000);
    __cc__ T* l0cPtr = reinterpret_cast<__cc__ T*>(0x4000);
    Nz2NdParamsFull nz2ndParams{1, 16, 16, 1, 0, 0, 1};
    DataCopy(gmPtr, l0cPtr, nz2ndParams);
}

template <typename T>
__aicore__ inline void ResolutionProbePad()
{
    __ubuf__ T* ubPtr = reinterpret_cast<__ubuf__ T*>(0x2000);
    __gm__ T* gmPtr = reinterpret_cast<__gm__ T*>(0x1000);
    DataCopyExtParams extParams{1, 32, 0, 0, 0};
    DataCopyPadExtParams<T> padParams{false, 0, 0, 0};
    DataCopyPad(ubPtr, gmPtr, extParams, padParams);
}

template <typename T>
__aicore__ inline void ResolutionProbePadOut()
{
    __ubuf__ T* ubPtr = reinterpret_cast<__ubuf__ T*>(0x2000);
    __gm__ T* gmPtr = reinterpret_cast<__gm__ T*>(0x1000);
    DataCopyExtParams extParams{1, 32, 0, 0, 0};
    DataCopyPad(gmPtr, ubPtr, extParams);
}

template <typename T>
__aicore__ inline void ResolutionProbeL1ToUb()
{
    __ubuf__ T* ubPtr = reinterpret_cast<__ubuf__ T*>(0x2000);
    __cbuf__ T* l1Ptr = reinterpret_cast<__cbuf__ T*>(0x3000);
    DataCopyParams repeatParams{1, 4, 0, 0};
    DataCopyL1ToUB(ubPtr, l1Ptr, uint32_t(32));
    DataCopyL1ToUB(ubPtr, l1Ptr, repeatParams);
}

template <typename T>
__aicore__ inline void ResolutionProbeSlice()
{
    __ubuf__ T* ubPtr = reinterpret_cast<__ubuf__ T*>(0x2000);
    __gm__ T* gmPtr = reinterpret_cast<__gm__ T*>(0x1000);
    static __gm__ SliceInfo dstSlice[2] = {{0, 47, 0, 3, 48}, {0, 1, 0, 1, 2}};
    static __gm__ SliceInfo srcSlice[2] = {{16, 71, 8, 3, 88}, {0, 2, 1, 1, 3}};
    DataCopy(ubPtr, gmPtr, dstSlice, srcSlice, 2);
}

} // namespace

class PtrResolutionRegressionSuite : public testing::Test {};

TEST_F(PtrResolutionRegressionSuite, PTR_REG_01_TensorFormsStillResolveUnchanged)
{
    // Same shapes as the pre-existing suites (test_data_copy.cpp / test_copy.cpp / ...): the
    // extension must not disturb any of these.
    EXPECT_TRUE((HasDataCopyCount<LocalTensor<float>, GlobalTensor<float>>::value));
    EXPECT_TRUE((HasDataCopyCount<GlobalTensor<float>, LocalTensor<float>>::value));
    EXPECT_TRUE((HasDataCopyParams<LocalTensor<float>, GlobalTensor<float>>::value));
    EXPECT_TRUE((HasDataCopyParams<GlobalTensor<float>, LocalTensor<float>>::value));
    EXPECT_TRUE((HasDataCopyNd2Nz<LocalTensor<half>, GlobalTensor<half>>::value));
    EXPECT_TRUE((HasDataCopyL1ToUbCount<LocalTensor<float>, LocalTensor<float>>::value));
    EXPECT_TRUE((HasDataCopyL1ToUbParams<LocalTensor<float>, LocalTensor<float>>::value));
}

TEST_F(PtrResolutionRegressionSuite, PTR_REG_01_PointerOverloadReachabilityReport)
{
    const bool countPath = HasDataCopyCount<__ubuf__ float*, __gm__ float*>::value;
    const bool paramsPath = HasDataCopyParams<__ubuf__ float*, __gm__ float*>::value;
    const bool nd2nzPath = HasDataCopyNd2Nz<__cbuf__ half*, __gm__ half*>::value;
    const bool nz2ndPath = HasDataCopyNz2Nd<__gm__ float*, __cc__ float*>::value;
    const bool padPath = HasDataCopyPadExtParams<float, __ubuf__ float*, __gm__ float*>::value;
    const bool padOutPath = HasDataCopyPadExtNoPad<__gm__ float*, __ubuf__ float*>::value;
    const bool slicePath = HasDataCopySlice<__ubuf__ float*, __gm__ float*>::value;
    const bool l1ubPath = HasDataCopyL1ToUbCount<__ubuf__ float*, __cbuf__ float*>::value;

    std::cout << "[PTR-REG-01] pointer overload availability:"
              << " count=" << countPath << " params=" << paramsPath << " nd2nz=" << nd2nzPath << " nz2nd=" << nz2ndPath
              << " pad=" << padPath << " padOut=" << padOutPath << " slice=" << slicePath << " l1toub=" << l1ubPath
              << " dcciGm=" << HasDcciPtr<float, __gm__ float*>::value
              << " preload=" << HasDataCachePreload<__gm__ uint64_t*, int16_t>::value << std::endl;

    // Instantiate every probe whose overload exists; a reachable-but-broken overload set fails to
    // compile here rather than silently skipping.
    if constexpr (HasDataCopyCount<__ubuf__ float*, __gm__ float*>::value) {
        (void)&ResolutionProbeCount<float>;
    }
    if constexpr (HasDataCopyParams<__ubuf__ float*, __gm__ float*>::value) {
        (void)&ResolutionProbeParams<float>;
    }
    if constexpr (HasDataCopyNd2Nz<__cbuf__ half*, __gm__ half*>::value) {
        (void)&ResolutionProbeNd2Nz<half>;
    }
    if constexpr (HasDataCopyNz2Nd<__gm__ float*, __cc__ float*>::value) {
        (void)&ResolutionProbeNz2Nd<float>;
    }
    if constexpr (HasDataCopyPadExtParams<float, __ubuf__ float*, __gm__ float*>::value) {
        (void)&ResolutionProbePad<float>;
    }
    if constexpr (HasDataCopyPadExtNoPad<__gm__ float*, __ubuf__ float*>::value) {
        (void)&ResolutionProbePadOut<float>;
    }
    if constexpr (HasDataCopyL1ToUbCount<__ubuf__ float*, __cbuf__ float*>::value) {
        (void)&ResolutionProbeL1ToUb<float>;
    }
    if constexpr (HasDataCopySlice<__ubuf__ float*, __gm__ float*>::value) {
        (void)&ResolutionProbeSlice<float>;
    }

    if (!countPath) {
        GTEST_SKIP() << "DMA pointer extension not compiled in yet: all PTR-* cases above are skipped";
    }
}
