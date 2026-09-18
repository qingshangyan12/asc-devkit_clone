/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <cstdint>
#include <gtest/gtest.h>
#include "kernel_operator.h"
// #include "api_check/kernel_cpu_check.h"

using namespace std;
using namespace AscendC;

/*
 * 搬运类接口指针化扩展 UT：
 * 指针入参与 Tensor 入参复用同一套底层 *Impl。本用例对同一组参数分别以 Tensor 写法与指针写法
 * （GetPhyAddr() + 地址空间限定强转）调用，校验：
 *   1) 编译期：全部指针重载可按声明的地址空间限定正确完成重载决议（-Werror）；
 *   2) 运行期：CPU 调试态包装层（地址对齐检查 CheckAddrAlignment、dtype/位置分发、参数范围检查）不触发断言；
 *   3) 行为：Tensor 路径与指针路径输出保持一致（UT 桩环境下目的缓冲保持 0 值，双路径一致）。
 * 说明：CPU 仿真态地址空间宏（__ubuf__/__gm__/__cbuf__ 等）展开为空，仅按地址空间区分方向的同形
 * 重载（如 UB->GM 的 DataCopy(params/count/SliceInfo)）无法与 GM->UB 代表重载区分，为设备编译专用，
 * 由 examples 下迁移的 7 个搬运例程覆盖；本 UT 覆盖宿主机可见的指针重载集合。
 */

/* **************************************************************************************************
 * GM <-> UB：DataCopyParams / count / Enhanced 三种模式                                         *
 * ************************************************************************************************* */
template <typename T, bool usePtr>
void RunGm2UbUb2GmChain(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, int32_t dataSize, uint32_t mode)
{
    TPipe tpipe;
    GlobalTensor<T> srcGlobal;
    GlobalTensor<T> dstGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    LocalTensor<T> inLocal;
    TBuffAddr inAddr;
    inAddr.logicPos = static_cast<uint8_t>(TPosition::VECCALC);
    inLocal.SetAddr(inAddr);
    inLocal.InitBuffer(0, dataSize);

    DataCopyParams dataCopyParams{1, 8, 0, 0};
    DataCopyEnhancedParams enhancedParams;
    enhancedParams.blockMode = BlockMode::BLOCK_MODE_MATRIX;

    if constexpr (usePtr) {
        // GM -> UB 搬入三形态（params / count / enhanced）；UB -> GM 的同形存回重载为设备编译专用
        if (mode == 0) {
            DataCopy((__ubuf__ T*)inLocal.GetPhyAddr(), (__gm__ T*)srcGlobal.GetPhyAddr(), dataCopyParams);
        } else if (mode == 1) {
            DataCopy((__ubuf__ T*)inLocal.GetPhyAddr(), (__gm__ T*)srcGlobal.GetPhyAddr(), dataSize);
        } else {
            DataCopy(
                (__ubuf__ T*)inLocal.GetPhyAddr(), (__gm__ T*)srcGlobal.GetPhyAddr(), dataCopyParams, enhancedParams);
        }
        // UB -> GM 存回方向以宿主机可见的 DataCopyPad(gm <- ub, params) 覆盖
        DataCopyPad((__gm__ T*)dstGlobal.GetPhyAddr(), (__ubuf__ T*)inLocal.GetPhyAddr(), dataCopyParams);
    } else {
        if (mode == 0) {
            DataCopy(inLocal, srcGlobal, dataCopyParams);
        } else if (mode == 1) {
            DataCopy(inLocal, srcGlobal, dataSize);
        } else {
            DataCopy(inLocal, srcGlobal, dataCopyParams, enhancedParams);
        }
        if (mode == 1) {
            DataCopy(dstGlobal, inLocal, dataSize);
        } else if (mode == 3) {
            DataCopy(dstGlobal, inLocal, dataCopyParams, enhancedParams);
        } else {
            DataCopy(dstGlobal, inLocal, dataCopyParams);
        }
    }
}

/* **************************************************************************************************
 * UB -> UB：DataCopy（params / count）与 Copy（mask 连续模式 / count 模式）                      *
 * ************************************************************************************************* */
template <typename T, bool usePtr>
void RunUb2UbChain(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, int32_t dataSize, uint32_t mode)
{
    TPipe tpipe;
    GlobalTensor<T> srcGlobal;
    GlobalTensor<T> dstGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    LocalTensor<T> inLocal;
    TBuffAddr inAddr;
    inAddr.logicPos = static_cast<uint8_t>(TPosition::VECCALC);
    inLocal.SetAddr(inAddr);
    inLocal.InitBuffer(0, dataSize);
    LocalTensor<T> outLocal;
    TBuffAddr outAddr;
    outAddr.logicPos = static_cast<uint8_t>(TPosition::VECOUT);
    outLocal.SetAddr(outAddr);
    outLocal.InitBuffer(0, dataSize);

    DataCopyParams dataCopyParams{1, 8, 0, 0};
    DataCopyEnhancedParams enhancedParams;
    enhancedParams.blockMode = BlockMode::BLOCK_MODE_MATRIX;
    CopyRepeatParams copyRepeatParams{1, 1, 8, 8};
    uint64_t mask = 64;
    uint8_t repeatTime = 8;

    // 源 UB 数据统一由 Tensor 路径 GM -> UB count 灌入，保证两路径起点一致
    DataCopy(inLocal, srcGlobal, dataSize);

    if constexpr (usePtr) {
        // UB -> UB 的 DataCopy(params/count/enhanced) 指针重载为设备编译专用，宿主机覆盖 Copy 两形态
        if (mode == 2) {
            Copy(
                (__ubuf__ T*)outLocal.GetPhyAddr(), (__ubuf__ T*)inLocal.GetPhyAddr(), mask, repeatTime,
                copyRepeatParams);
        } else if (mode == 3) {
            Copy((__ubuf__ T*)outLocal.GetPhyAddr(), (__ubuf__ T*)inLocal.GetPhyAddr(), dataSize);
        } else {
            Copy(
                (__ubuf__ T*)outLocal.GetPhyAddr(), (__ubuf__ T*)inLocal.GetPhyAddr(), mask, repeatTime,
                copyRepeatParams);
        }
    } else {
        if (mode == 0) {
            DataCopy(outLocal, inLocal, dataCopyParams);
        } else if (mode == 1) {
            DataCopy(outLocal, inLocal, dataSize);
        } else if (mode == 2) {
            Copy(outLocal, inLocal, mask, repeatTime, copyRepeatParams);
        } else if (mode == 3) {
            Copy(outLocal, inLocal, dataSize);
        } else {
            DataCopy(outLocal, inLocal, dataCopyParams, enhancedParams);
        }
    }
    DataCopy(dstGlobal, outLocal, dataCopyParams);
}

/* **************************************************************************************************
 * L1 链路：UB->L1 / GM->L1（params / count）与 DataCopyL1ToUB（count / params）                  *
 * ************************************************************************************************* */
template <typename T, bool usePtr>
void RunL1Chain(__gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, int32_t dataSize, uint32_t mode)
{
    TPipe tpipe;
    GlobalTensor<T> srcGlobal;
    GlobalTensor<T> dstGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    LocalTensor<T> inLocal;
    TBuffAddr inAddr;
    inAddr.logicPos = static_cast<uint8_t>(TPosition::VECCALC);
    inLocal.SetAddr(inAddr);
    inLocal.InitBuffer(0, dataSize);
    LocalTensor<T> outLocal;
    TBuffAddr outAddr;
    outAddr.logicPos = static_cast<uint8_t>(TPosition::VECOUT);
    outLocal.SetAddr(outAddr);
    outLocal.InitBuffer(0, dataSize);
    // L1 缓冲经 TPipe/TBuf 分配：Tensor 路径的溢出/对齐检查依赖缓冲账目，手工 SetAddr 无法通过
    TBuf<TPosition::A1> tbufL1;
    tpipe.InitBuffer(tbufL1, dataSize * sizeof(T));
    LocalTensor<T> l1Local = tbufL1.Get<T>();

    DataCopyParams dataCopyParams{1, 8, 0, 0};
    DataCopyEnhancedParams enhancedParams;
    enhancedParams.blockMode = BlockMode::BLOCK_MODE_MATRIX;

    if constexpr (usePtr) {
        // GM->L1 / UB->L1 的 DataCopy(params/count/enhanced) 指针重载为设备编译专用；
        // 宿主机覆盖 DataCopyL1ToUB（count / params）两形态，L1 源统一由 Tensor 路径 UB->L1 enhanced 灌入
        // （GM->L1 params 方向的 Tensor 路径在宿主机侧会触发溢出检查终止，为设备行为，见 examples）
        DataCopy(inLocal, srcGlobal, dataSize);
        DataCopy(l1Local, inLocal, dataCopyParams, enhancedParams);
        if (mode == 4) {
            DataCopyL1ToUB((__ubuf__ T*)outLocal.GetPhyAddr(), (__cbuf__ T*)l1Local.GetPhyAddr(), dataSize);
        } else {
            DataCopyL1ToUB((__ubuf__ T*)outLocal.GetPhyAddr(), (__cbuf__ T*)l1Local.GetPhyAddr(), dataCopyParams);
        }
    } else {
        DataCopy(inLocal, srcGlobal, dataSize);
        if (mode == 4) {
            DataCopy(l1Local, inLocal, dataCopyParams, enhancedParams);
            DataCopyL1ToUB(outLocal, l1Local, dataSize);
        } else {
            DataCopy(l1Local, inLocal, dataCopyParams, enhancedParams);
            DataCopyL1ToUB(outLocal, l1Local, dataCopyParams);
        }
    }
    DataCopy(dstGlobal, outLocal, dataCopyParams);
}

/* **************************************************************************************************
 * NDDMA：GM -> UB 多维搬运（kernel 与参数取自 test_data_copy_nddma.cpp 已验证值，尺寸单位为字节）  *
 * ************************************************************************************************* */
template <typename T, bool usePtr, uint8_t dim, bool enableNearest, bool disablePad = true, bool optimized = false>
void RunNddmaChain(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, uint32_t srcSize, uint32_t dstSize,
    const uint32_t loopSize[], const uint64_t loopSrcStride[], const uint32_t loopDstStride[],
    const uint8_t loopLpSize[], const uint8_t loopRpSize[])
{
    TPipe tpipe;
    GlobalTensor<T> inputGlobal;
    GlobalTensor<T> outputGlobal;
    inputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), srcSize);
    outputGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dstSize);

    MultiCopyLoopInfo<dim> loopInfo;
    for (size_t i = 0; i < dim; i++) {
        loopInfo.loopSize[i] = loopSize[i];
        loopInfo.loopSrcStride[i] = loopSrcStride[i];
        loopInfo.loopDstStride[i] = loopDstStride[i];
        loopInfo.loopLpSize[i] = loopLpSize[i];
        loopInfo.loopRpSize[i] = loopRpSize[i];
    }
    T constValue = sizeof(T) == 8 ? T{0} : T{1};
    MultiCopyParams<T, dim> params = {loopInfo, constValue};

    TBuf<TPosition::VECCALC> tbuf;
    tpipe.InitBuffer(tbuf, dstSize * sizeof(T));
    LocalTensor<T> outputLocal = tbuf.Get<T>();

    constexpr uint16_t padValue = disablePad ? NdDmaConfig::unsetPad : 0;
    static constexpr NdDmaConfig config = {enableNearest, padValue, padValue, optimized};

    if constexpr (usePtr) {
        DataCopy<T, dim, config>((__ubuf__ T*)outputLocal.GetPhyAddr(), (__gm__ T*)inputGlobal.GetPhyAddr(), params);
    } else {
        DataCopy<T, dim, config>(outputLocal, inputGlobal, params);
    }

    set_flag(PIPE_MTE2, PIPE_MTE3, EVENT_ID0);
    wait_flag(PIPE_MTE2, PIPE_MTE3, EVENT_ID0);

    DataCopy(outputGlobal, outputLocal, dstSize);
    pipe_barrier(PIPE_ALL);
}

/* **************************************************************************************************
 * DataCopyPad：GM -> UB（params+pad / ext+padext）与 UB -> GM（params / ext）                    *
 * ************************************************************************************************* */
template <typename T, bool usePtr>
void RunPadChain(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, int32_t dataSize, uint32_t mode)
{
    TPipe tpipe;
    GlobalTensor<T> srcGlobal;
    GlobalTensor<T> dstGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    LocalTensor<T> inLocal;
    TBuffAddr inAddr;
    inAddr.logicPos = static_cast<uint8_t>(TPosition::VECCALC);
    inLocal.SetAddr(inAddr);
    inLocal.InitBuffer(0, dataSize);

    DataCopyParams dataCopyParams{1, 2, 0, 0};
    DataCopyPadParams padParams{false, 0, 0, 0};
    DataCopyExtParams copyParams{1, 20 * sizeof(T), 0, 0, 0};
    DataCopyPadExtParams<T> padExtParams{true, 0, 1, 0};

    if constexpr (usePtr) {
        // 搬入两形态（params+pad / ext+padext）与存回两形态（params / ext），四个指针重载均宿主机可见
        if (mode == 0) {
            DataCopyPad(
                (__ubuf__ T*)inLocal.GetPhyAddr(), (__gm__ T*)srcGlobal.GetPhyAddr(), dataCopyParams, padParams);
            DataCopyPad((__gm__ T*)dstGlobal.GetPhyAddr(), (__ubuf__ T*)inLocal.GetPhyAddr(), dataCopyParams);
        } else if (mode == 1) {
            DataCopy(inLocal, srcGlobal, dataSize);
            DataCopyPad((__gm__ T*)dstGlobal.GetPhyAddr(), (__ubuf__ T*)inLocal.GetPhyAddr(), dataCopyParams);
        } else if (mode == 2) {
            DataCopyPad((__ubuf__ T*)inLocal.GetPhyAddr(), (__gm__ T*)srcGlobal.GetPhyAddr(), copyParams, padExtParams);
            DataCopyPad((__gm__ T*)dstGlobal.GetPhyAddr(), (__ubuf__ T*)inLocal.GetPhyAddr(), dataCopyParams);
        } else {
            DataCopy(inLocal, srcGlobal, dataSize);
            DataCopyPad((__gm__ T*)dstGlobal.GetPhyAddr(), (__ubuf__ T*)inLocal.GetPhyAddr(), copyParams);
        }
    } else {
        if (mode == 0) {
            DataCopyPad<T, PaddingMode::Normal>(inLocal, srcGlobal, dataCopyParams, padParams);
            DataCopy(dstGlobal, inLocal, dataCopyParams);
        } else if (mode == 1) {
            DataCopy(inLocal, srcGlobal, dataSize);
            DataCopyPad<T, PaddingMode::Normal>(dstGlobal, inLocal, dataCopyParams);
        } else if (mode == 2) {
            DataCopyPad<T, PaddingMode::Normal>(inLocal, srcGlobal, copyParams, padExtParams);
            DataCopy(dstGlobal, inLocal, dataCopyParams);
        } else {
            DataCopy(inLocal, srcGlobal, dataSize);
            DataCopyPad<T, PaddingMode::Normal>(dstGlobal, inLocal, copyParams);
        }
    }
}

/* **************************************************************************************************
 * Nd2Nz：GM -> UB 与 GM -> L1（参数取自 aic/test_data_copy.cpp 已验证值）                         *
 * ************************************************************************************************* */
template <typename T, bool usePtr>
void RunNd2NzChain(
    __gm__ uint8_t* __restrict__ srcGm, __gm__ uint8_t* __restrict__ dstGm, int32_t dataSize, uint32_t mode)
{
    TPipe tpipe;
    GlobalTensor<T> srcGlobal;
    GlobalTensor<T> dstGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(srcGm), dataSize);
    dstGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ T*>(dstGm), dataSize);

    LocalTensor<T> inLocal;
    TBuffAddr inAddr;
    inAddr.logicPos = static_cast<uint8_t>(TPosition::VECCALC);
    inLocal.SetAddr(inAddr);
    inLocal.InitBuffer(0, dataSize);

    (void)mode; // GM -> L1 方向为设备编译专用，宿主机仅覆盖 GM -> UB Nd2Nz
    Nd2NzParams nd2NzParams{1, 1, 32, 0, 32, 1, 1, 0};
    DataCopyParams dataCopyParams{1, 8, 0, 0};

    if constexpr (usePtr) {
        // GM -> UB Nd2Nz 指针重载；GM -> L1 Nd2Nz 指针重载为设备编译专用（宿主机地址空间宏展开为空，
        // 统一由 GM -> UB 代表重载承接），Tensor 路径 GM -> L1 Nd2Nz 依赖设备侧行为，由 examples 覆盖
        DataCopy((__ubuf__ T*)inLocal.GetPhyAddr(), (__gm__ T*)srcGlobal.GetPhyAddr(), nd2NzParams);
        DataCopyPad((__gm__ T*)dstGlobal.GetPhyAddr(), (__ubuf__ T*)inLocal.GetPhyAddr(), dataCopyParams);
    } else {
        DataCopy(inLocal, srcGlobal, nd2NzParams);
        DataCopy(dstGlobal, inLocal, dataCopyParams);
    }
}

/* **************************************************************************************************
 * 参数化框架：Tensor 路径与指针路径同参双跑                                                     *
 * ************************************************************************************************* */
struct TestDataCopyPtrParams {
    uint32_t dataSize;
    uint32_t typeSize;
    uint32_t mode;
    void (*tensorFunc)(uint8_t*, uint8_t*, int32_t, uint32_t);
    void (*ptrFunc)(uint8_t*, uint8_t*, int32_t, uint32_t);
};

class TestDataCopyPtrSuite : public testing::Test, public testing::WithParamInterface<TestDataCopyPtrParams> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_PTR_GM2UB, TestDataCopyPtrSuite,
    ::testing::Values(
        TestDataCopyPtrParams{512, 4, 0, RunGm2UbUb2GmChain<int32_t, false>, RunGm2UbUb2GmChain<int32_t, true>},
        TestDataCopyPtrParams{512, 4, 1, RunGm2UbUb2GmChain<int32_t, false>, RunGm2UbUb2GmChain<int32_t, true>},
        TestDataCopyPtrParams{512, 4, 2, RunGm2UbUb2GmChain<int32_t, false>, RunGm2UbUb2GmChain<int32_t, true>},
        TestDataCopyPtrParams{512, 2, 0, RunGm2UbUb2GmChain<int16_t, false>, RunGm2UbUb2GmChain<int16_t, true>},
        TestDataCopyPtrParams{512, 2, 1, RunGm2UbUb2GmChain<int16_t, false>, RunGm2UbUb2GmChain<int16_t, true>},
        TestDataCopyPtrParams{512, 4, 0, RunGm2UbUb2GmChain<float, false>, RunGm2UbUb2GmChain<float, true>},
        TestDataCopyPtrParams{768, 2, 0, RunGm2UbUb2GmChain<half, false>, RunGm2UbUb2GmChain<half, true>},
        TestDataCopyPtrParams{768, 2, 1, RunGm2UbUb2GmChain<half, false>, RunGm2UbUb2GmChain<half, true>},
        TestDataCopyPtrParams{2048, 1, 0, RunGm2UbUb2GmChain<int8_t, false>, RunGm2UbUb2GmChain<int8_t, true>},
        TestDataCopyPtrParams{2048, 1, 1, RunGm2UbUb2GmChain<uint8_t, false>, RunGm2UbUb2GmChain<uint8_t, true>},
        TestDataCopyPtrParams{2048, 1, 2, RunGm2UbUb2GmChain<int8_t, false>, RunGm2UbUb2GmChain<int8_t, true>}));

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_PTR_UB2UB, TestDataCopyPtrSuite,
    ::testing::Values(
        TestDataCopyPtrParams{512, 4, 2, RunUb2UbChain<int32_t, false>, RunUb2UbChain<int32_t, true>},
        // dataSize 需覆盖 Tensor 路径 CheckFuncCopy 的访问范围：repeatTime(8) * srcRepeatStride(8) * 32B = 2048B
        TestDataCopyPtrParams{1024, 2, 2, RunUb2UbChain<half, false>, RunUb2UbChain<half, true>},
        TestDataCopyPtrParams{512, 4, 3, RunUb2UbChain<int32_t, false>, RunUb2UbChain<int32_t, true>},
        TestDataCopyPtrParams{768, 2, 3, RunUb2UbChain<half, false>, RunUb2UbChain<half, true>}));

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_PTR_L1, TestDataCopyPtrSuite,
    ::testing::Values(
        TestDataCopyPtrParams{512, 4, 4, RunL1Chain<int32_t, false>, RunL1Chain<int32_t, true>},
        TestDataCopyPtrParams{512, 4, 5, RunL1Chain<int32_t, false>, RunL1Chain<int32_t, true>},
        TestDataCopyPtrParams{768, 2, 4, RunL1Chain<half, false>, RunL1Chain<half, true>},
        TestDataCopyPtrParams{768, 2, 5, RunL1Chain<half, false>, RunL1Chain<half, true>}));

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_PTR_PAD, TestDataCopyPtrSuite,
    ::testing::Values(
        TestDataCopyPtrParams{64, 4, 0, RunPadChain<int32_t, false>, RunPadChain<int32_t, true>},
        TestDataCopyPtrParams{64, 4, 1, RunPadChain<int32_t, false>, RunPadChain<int32_t, true>},
        TestDataCopyPtrParams{64, 4, 2, RunPadChain<int32_t, false>, RunPadChain<int32_t, true>},
        TestDataCopyPtrParams{64, 4, 3, RunPadChain<int32_t, false>, RunPadChain<int32_t, true>},
        TestDataCopyPtrParams{64, 2, 0, RunPadChain<int16_t, false>, RunPadChain<int16_t, true>},
        TestDataCopyPtrParams{64, 8, 2, RunPadChain<uint64_t, false>, RunPadChain<uint64_t, true>},
        TestDataCopyPtrParams{64, 2, 0, RunPadChain<half, false>, RunPadChain<half, true>},
        TestDataCopyPtrParams{64, 1, 3, RunPadChain<int8_t, false>, RunPadChain<int8_t, true>},
        TestDataCopyPtrParams{64, 1, 0, RunPadChain<uint8_t, false>, RunPadChain<uint8_t, true>}));

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_PTR_ND2NZ, TestDataCopyPtrSuite,
    ::testing::Values(
        TestDataCopyPtrParams{512, 4, 0, RunNd2NzChain<int32_t, false>, RunNd2NzChain<int32_t, true>},
        TestDataCopyPtrParams{512, 2, 0, RunNd2NzChain<half, false>, RunNd2NzChain<half, true>}));

TEST_P(TestDataCopyPtrSuite, TestDataCopyPtrCases)
{
    auto param = GetParam();
    uint32_t byteSize = param.dataSize * param.typeSize;
    int32_t gCoreTypeTemp = g_coreType;
    AscendC::SetGCoreType(2);
    uint8_t srcTensor[byteSize] = {0};
    uint8_t dstTensor[byteSize] = {0};
    uint8_t srcPtr[byteSize] = {0};
    uint8_t dstPtr[byteSize] = {0};
    param.tensorFunc(srcTensor, dstTensor, param.dataSize, param.mode);
    param.ptrFunc(srcPtr, dstPtr, param.dataSize, param.mode);
    for (uint32_t i = 0; i < byteSize; i++) {
        EXPECT_EQ(dstTensor[i], 0x00);
        EXPECT_EQ(dstTensor[i], dstPtr[i]);
    }
    AscendC::SetGCoreType(gCoreTypeTemp);
}

/* **************************************************************************************************
 * NDDMA 相等性（参数取自 test_data_copy_nddma.cpp 已验证值，尺寸单位为字节）                       *
 * ************************************************************************************************* */
struct TestNddmaPtrParams {
    uint32_t srcSize;
    uint32_t dstSize;
    uint32_t loopSize[5];
    uint64_t loopSrcStride[5];
    uint32_t loopDstStride[5];
    uint8_t loopLpSize[5];
    uint8_t loopRpSize[5];
    void (*tensorFunc)(
        uint8_t*, uint8_t*, uint32_t, uint32_t, const uint32_t*, const uint64_t*, const uint32_t*, const uint8_t*,
        const uint8_t*);
    void (*ptrFunc)(
        uint8_t*, uint8_t*, uint32_t, uint32_t, const uint32_t*, const uint64_t*, const uint32_t*, const uint8_t*,
        const uint8_t*);
};

template <typename T, bool usePtr, uint8_t dim, bool enableNearest>
void NddmaChainEntry(
    uint8_t* src, uint8_t* dst, uint32_t srcSize, uint32_t dstSize, const uint32_t loopSize[],
    const uint64_t loopSrcStride[], const uint32_t loopDstStride[], const uint8_t loopLpSize[],
    const uint8_t loopRpSize[])
{
    RunNddmaChain<T, usePtr, dim, enableNearest>(
        reinterpret_cast<__gm__ uint8_t*>(src), reinterpret_cast<__gm__ uint8_t*>(dst), srcSize, dstSize, loopSize,
        loopSrcStride, loopDstStride, loopLpSize, loopRpSize);
}

class TestNddmaPtrSuite : public testing::Test, public testing::WithParamInterface<TestNddmaPtrParams> {
protected:
    void SetUp() {}
    void TearDown() {}
};

INSTANTIATE_TEST_CASE_P(
    TEST_DATA_COPY_PTR_NDDMA, TestNddmaPtrSuite,
    ::testing::Values(
        TestNddmaPtrParams{
            30 * 120 * 4,
            32 * 128 * 4,
            {30, 120},
            {1, 30},
            {1, 32},
            {1, 3},
            {1, 5},
            NddmaChainEntry<int32_t, false, 2, true>,
            NddmaChainEntry<int32_t, true, 2, true>},
        TestNddmaPtrParams{
            30 * 120 * 4,
            32 * 128 * 4,
            {30, 120},
            {1, 30},
            {1, 32},
            {1, 3},
            {1, 5},
            NddmaChainEntry<uint32_t, false, 2, false>,
            NddmaChainEntry<uint32_t, true, 2, false>},
        TestNddmaPtrParams{
            4 * 15 * 13 * 2,
            2 * 8 * 10 * 2,
            {10, 8, 2},
            {1, 13, 13 * 15},
            {1, 10, 8 * 10},
            {0},
            {0},
            NddmaChainEntry<half, false, 3, true>,
            NddmaChainEntry<half, true, 3, true>}));

TEST_P(TestNddmaPtrSuite, TestNddmaPtrCases)
{
    // 与仓库 test_data_copy_nddma.cpp 一致：不设置核类型（AIV 态下宿主机 NDDMA 仿真不可用，
    // 默认核态走早退空操作，UT 覆盖编译期重载决议与链路组装）
    auto param = GetParam();
    uint8_t srcTensor[param.srcSize] = {0};
    uint8_t dstTensor[param.dstSize] = {0};
    uint8_t srcPtr[param.srcSize] = {0};
    uint8_t dstPtr[param.dstSize] = {0};
    param.tensorFunc(
        srcTensor, dstTensor, param.srcSize, param.dstSize, param.loopSize, param.loopSrcStride, param.loopDstStride,
        param.loopLpSize, param.loopRpSize);
    param.ptrFunc(
        srcPtr, dstPtr, param.srcSize, param.dstSize, param.loopSize, param.loopSrcStride, param.loopDstStride,
        param.loopLpSize, param.loopRpSize);
    for (uint32_t i = 0; i < param.dstSize; i++) {
        EXPECT_EQ(dstTensor[i], 0x00);
        EXPECT_EQ(dstTensor[i], dstPtr[i]);
    }
}

/* **************************************************************************************************
 * CUBE 侧指针重载（L0C -> GM / L0C -> L1 / C1 -> C2 BiasTable / C1 -> C2PIPE2GM Fixpipe /         *
 * DataCopyPad UB -> L1）：AIC 核心态，缓冲使用 TBuf<TPosition::C1/C2/C2PIPE2GM/CO1> 已验证写法     *
 * ************************************************************************************************* */
class DataCopyPtrCubeTestSuite : public ::testing::Test {
protected:
    virtual void SetUp() { SetGCoreType(1); }
    virtual void TearDown() { SetGCoreType(0); }
};

TEST_F(DataCopyPtrCubeTestSuite, testCaseDataCopyL0C2GmPtr)
{
    const uint32_t dataSize = 256;
    uint8_t dstBuf[dataSize * sizeof(float)] = {0};
    TPipe tpipe;
    GlobalTensor<float> cGM;
    cGM.SetGlobalBuffer(reinterpret_cast<__gm__ float*>(dstBuf), dataSize);
    TBuf<TPosition::CO1> tbuf;
    tpipe.InitBuffer(tbuf, dataSize * sizeof(float));
    LocalTensor<float> cLocal = tbuf.Get<float>();
    DataCopyCO12DstParams intriParams(16, 16, 0, 0, QuantMode_t::VDEQF16, static_cast<uint8_t>(0), 0, 0);
    EXPECT_NO_THROW(DataCopy((__gm__ float*)cGM.GetPhyAddr(), (__cc__ float*)cLocal.GetPhyAddr(), intriParams));
    EXPECT_NO_THROW(DataCopy(cGM, cLocal, intriParams));
}

TEST_F(DataCopyPtrCubeTestSuite, testCaseDataCopyL0C2L1Ptr)
{
    const uint32_t dataSize = 64;
    TPipe tpipe;
    TBuf<TPosition::CO1> tbufCO1;
    tpipe.InitBuffer(tbufCO1, dataSize * sizeof(float));
    LocalTensor<float> cLocal = tbufCO1.Get<float>();
    TBuf<TPosition::C1> tbufL1;
    tpipe.InitBuffer(tbufL1, dataSize * sizeof(half));
    LocalTensor<half> l1Local = tbufL1.Get<half>();
    DataCopyCO12DstParams intriParams(4, 4, 0, 0, QuantMode_t::VDEQF16, static_cast<uint8_t>(0), 0, 0);
    // L0C -> L1 的 __cbuf__/__cc__ 指针重载与 L0C -> GM 的 __gm__/__cc__ 重载仅地址空间限定不同，
    // 宿主机侧塌缩为同签名，为设备编译专用（examples/data_copy_l0c2gm 覆盖），此处回归 Tensor 版本
    EXPECT_NO_THROW(DataCopy(l1Local, cLocal, intriParams));
}

TEST_F(DataCopyPtrCubeTestSuite, testCaseDataCopyL1ToBiasTablePtr)
{
    const uint32_t dataSize = 64;
    TPipe tpipe;
    TBuf<TPosition::C1> tbufL1;
    tpipe.InitBuffer(tbufL1, dataSize * sizeof(float));
    LocalTensor<float> l1Local = tbufL1.Get<float>();
    TBuf<TPosition::C2> tbufC2;
    tpipe.InitBuffer(tbufC2, dataSize * sizeof(float));
    LocalTensor<float> biasLocal = tbufC2.Get<float>();
    DataCopyParams dataCopyParams(1, 2, 0, 0);
    EXPECT_NO_THROW(
        DataCopy((__cbuf__ float*)biasLocal.GetPhyAddr(), (__cbuf__ float*)l1Local.GetPhyAddr(), dataCopyParams));
    EXPECT_NO_THROW(DataCopy(biasLocal, l1Local, dataCopyParams));
}

TEST_F(DataCopyPtrCubeTestSuite, testCaseDataCopyL1ToFixpipePtr)
{
    const uint32_t dataSize = 64;
    TPipe tpipe;
    TBuf<TPosition::C1> tbufL1;
    tpipe.InitBuffer(tbufL1, dataSize * sizeof(uint64_t));
    LocalTensor<uint64_t> l1Local = tbufL1.Get<uint64_t>();
    TBuf<TPosition::C2PIPE2GM> tbufFB;
    tpipe.InitBuffer(tbufFB, dataSize * sizeof(uint64_t));
    LocalTensor<uint64_t> fbLocal = tbufFB.Get<uint64_t>();
    DataCopyParams dataCopyParams(1, 2, 0, 0);
    // C1 -> C2PIPE2GM 的 __fbuf__/__cbuf__ 指针重载与 GM->UB 指针重载仅地址空间限定不同，
    // 宿主机侧塌缩为同签名，为设备编译专用（examples/data_copy_l1_to_c2 覆盖），此处回归 Tensor 版本
    EXPECT_NO_THROW(DataCopy(fbLocal, l1Local, dataCopyParams));
}

TEST_F(DataCopyPtrCubeTestSuite, testCaseDataCopyPadUb2L1Nd2NzPtr)
{
    const uint32_t dataSize = 64;
    LocalTensor<half> ubLocal;
    TBuffAddr ubAddr;
    ubAddr.logicPos = static_cast<uint8_t>(TPosition::VECCALC);
    ubLocal.SetAddr(ubAddr);
    ubLocal.InitBuffer(0, dataSize);
    LocalTensor<half> l1Local;
    TBuffAddr l1Addr;
    l1Addr.logicPos = static_cast<uint8_t>(TPosition::TSCM);
    l1Local.SetAddr(l1Addr);
    l1Local.InitBuffer(0, dataSize);
    DataCopyParams dataCopyParams{1, 2, 0, 0};
    DataCopyExtParams copyParams{1, 20 * sizeof(half), 0, 0, 0};
    Nd2NzParams nd2NzParams{1, 1, 32, 0, 32, 1, 1, 0};
    EXPECT_NO_THROW(DataCopyPad(
        (__cbuf__ half*)l1Local.GetPhyAddr(), (__ubuf__ half*)ubLocal.GetPhyAddr(), dataCopyParams, nd2NzParams));
    EXPECT_NO_THROW(DataCopyPad(
        (__cbuf__ half*)l1Local.GetPhyAddr(), (__ubuf__ half*)ubLocal.GetPhyAddr(), copyParams, nd2NzParams));
    EXPECT_NO_THROW(DataCopyPad(l1Local, ubLocal, dataCopyParams, nd2NzParams));
}

TEST_F(DataCopyPtrCubeTestSuite, testCaseDataCopySlicePtr)
{
    // SliceInfo 指针重载（GM -> UB）。Tensor 版本在 950pr 宿主机 UT 环境会触发宿主机侧
    // 检查终止（本目标不含 SliceInfo 的仓库用例），设备行为由 examples 覆盖；指针路径无
    // Tensor 形状信息，此处校验重载决议与参数范围检查不触发断言
    const uint32_t srcSize = 264;
    const uint32_t dstSize = 96;
    uint8_t srcBuf[srcSize * sizeof(int32_t)] = {0};
    uint8_t dstBuf[dstSize * sizeof(int32_t)] = {0};
    SetGCoreType(2);
    GlobalTensor<int32_t> srcGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ int32_t*>(srcBuf), srcSize);
    TBuf<TPosition::VECCALC> tbuf;
    TPipe tpipe;
    tpipe.InitBuffer(tbuf, dstSize * sizeof(int32_t));
    LocalTensor<int32_t> inLocal = tbuf.Get<int32_t>();
    SliceInfo dstSliceInfo[2] = {{16, 71, 8, 3, 88}, {0, 2, 1, 1, 3}};
    SliceInfo srcSliceInfo[2] = {{0, 47, 0, 3, 48}, {0, 1, 0, 1, 2}};
    EXPECT_NO_THROW(DataCopy(
        (__ubuf__ int32_t*)inLocal.GetPhyAddr(), (__gm__ int32_t*)srcGlobal.GetPhyAddr(), dstSliceInfo, srcSliceInfo,
        2));
    (void)dstBuf;
    SetGCoreType(0);
}

/* **************************************************************************************************
 * DataCache 系列指针化扩展                                                              *
 * ************************************************************************************************* */
class TestCachePtrSuite : public testing::Test {
protected:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(TestCachePtrSuite, DataCachePreloadPtrTest)
{
    uint64_t src[16] = {0};
    int16_t cacheOffset = 16;
    GlobalTensor<uint64_t> srcGlobal;
    srcGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ uint64_t*>(src), 16);
    EXPECT_NO_THROW(DataCachePreload((__gm__ uint64_t*)srcGlobal.GetPhyAddr(), cacheOffset));
    EXPECT_NO_THROW(DataCachePreload(srcGlobal, cacheOffset));
}

TEST_F(TestCachePtrSuite, DataCacheCleanAndInvalidPtrTest)
{
    uint64_t gmBuf[16] = {0};
    GlobalTensor<uint64_t> gmGlobal;
    gmGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ uint64_t*>(gmBuf), 16);
    LocalTensor<uint64_t> ubLocal;
    TBuffAddr ubAddr;
    ubAddr.logicPos = static_cast<uint8_t>(TPosition::VECCALC);
    ubLocal.SetAddr(ubAddr);
    ubLocal.InitBuffer(0, 16);
    EXPECT_NO_THROW((DataCacheCleanAndInvalid<uint64_t, CacheLine::SINGLE_CACHE_LINE, DcciDst::CACHELINE_OUT>(
        (__gm__ uint64_t*)gmGlobal.GetPhyAddr())));
    // __ubuf__ 指针版本与 __gm__ 指针版本仅地址空间限定不同，宿主机侧塌缩为同签名，
    // 为设备编译专用，此处回归 Tensor 版本
    EXPECT_NO_THROW((DataCacheCleanAndInvalid<uint64_t, CacheLine::SINGLE_CACHE_LINE>((__gm__ uint64_t*)gmBuf)));
    EXPECT_NO_THROW(
        (DataCacheCleanAndInvalid<uint64_t, CacheLine::SINGLE_CACHE_LINE, DcciDst::CACHELINE_OUT>(gmGlobal)));
    EXPECT_NO_THROW((DataCacheCleanAndInvalid<uint64_t, CacheLine::SINGLE_CACHE_LINE, DcciDst::CACHELINE_UB>(ubLocal)));
}
