/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file test_operator_mmadmx_bitmode_pointer_arm.cpp
 * \brief Pointer-arm call coverage for MmadMx with **MmadBitModeParams** (the BitMode group).
 *
 * WHY THIS FILE EXISTS
 *   讨论贴 27 的官方答复：「清单内的 API 都需要指针改造并进行验证」；
 *   「表内带 TPosition 模板形参的 **BitMode 签名 —— 不豁免，需要支持**」。
 *   权威清单 basic_api_list_cube_api.xlsx 把 **MmadBitMode 单列为 4 条重载**。
 *   而本补丁此前只给 MmadMx 补了 MmadParams 的指针形，BitMode 形缺失；
 *   并且定义侧多出一个**不可推导的 typename R**，使被声明的那两条**从来没有定义**
 *   （已另行作为 Issue 提交：https://gitcode.com/cann/asc-devkit/issues/1730）。
 *   本文件在**同一棵树上**同时覆盖「指针形已补」与「R 已修」两件事。
 *
 * ★ 为什么仓库自带的 BitMode 用例抓不到那个缺陷（实测，不是推断）
 *   tests/.../test_operator_MM_bitmode_Mx.cpp 与本目录的 test_operator_MM_Mx.cpp
 *   **都在第 47 行定义了同名同签名的模板 `MainCpuCmpMmadMxBiasDemo`** —— 这是一处 ODR 冲突，
 *   链接器只保留其中一个定义，于是**BitMode 那个用例实际执行的是另一条路径的函数体**。
 *   因此自带套件不可能发现 BitMode 形的声明/定义不匹配。本文件不使用那个宏，
 *   而是各自定义独立的核函数，避免被同一个 ODR 冲突吞掉。
 *
 * WHAT THIS FILE PROVES, AND WHAT IT DOES NOT
 *   IT PROVES that the raw-pointer overload of MmadMx with MmadBitModeParams RESOLVES to a
 *   declaration on the tree under test, and that a kernel which ACTUALLY CALLS it reaches the end of
 *   its body.  It makes **no numerical claim** -- numerical equivalence is a different gate on a
 *   different instrument (the device two-arm sweep).
 */
#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>
#include <type_traits>
#include <utility>
#include "kernel_operator.h"
#include "test_utils.h"

using namespace AscendC;

namespace {
constexpr int32_t MX_FEATURE_SIZE = 3584;
constexpr int32_t MX_WEIGHT_SIZE = 4096;
constexpr int32_t MX_OUTPUT_SIZE = 14336;
constexpr int32_t SENTINEL = 0x5A5A1234;
int32_t g_raiseCount = 0;
} // namespace

// 后端 MmadMxCal 在不受支持的 dtype/arch 组合上会 raise(SIGABRT)。与同批 Dump / LoadData3DV1 用例
// 同一做法：把 raise 打桩、只记录不硬断言 —— 否则 CPU 模型下会把整个 UT 进程 abort 掉。
static int32_t RaiseStub(int32_t)
{
    g_raiseCount++;
    return 0;
}

class MmadMxBitModePointerArmTestsuite : public testing::Test {
protected:
    void SetUp()
    {
        MOCKER(raise, int32_t(*)(int32_t)).stubs().will(invoke(RaiseStub));
        g_raiseCount = 0;
        g_coreType = AIC_TYPE;
    }
    void TearDown() { g_coreType = MIX_TYPE; }
};

// ---- compile-time resolution checks ---------------------------------------
static_assert(
    std::is_same<
        decltype(AscendC::MmadMx<float, fp4x2_e1m2_t, fp4x2_e1m2_t>(
            std::declval<LocalTensor<float>&>(), std::declval<LocalTensor<fp4x2_e1m2_t>&>(),
            std::declval<LocalTensor<fp4x2_e1m2_t>&>(), std::declval<const MmadBitModeParams&>())),
        void>::value,
    "MmadMx BitMode 的 Tensor 形必须存在（本判据只证「存在」，不证重载选择）");
static_assert(
    std::is_same<
        decltype(AscendC::MmadMx(
            std::declval<__cc__ float*>(), std::declval<__ca__ fp4x2_e1m2_t*>(), std::declval<__cb__ fp4x2_e1m2_t*>(),
            std::declval<MmadBitModeParams&>())),
        void>::value,
    "MmadMx BitMode 的指针形必须存在（同样只证「存在」；「解析到哪条重载」这一问由 pristine 树上的 "
    "no matching function 诊断承担，见文件头 DISCRIMINATING POWER）");
static_assert(
    std::is_same<
        decltype(AscendC::Mmad(
            std::declval<__cc__ float*>(), std::declval<__ca__ fp4x2_e1m2_t*>(), std::declval<__cb__ fp4x2_e1m2_t*>(),
            std::declval<const MmadBitModeParams&>())),
        void>::value,
    "Mmad BitMode 的指针形必须存在（该形 HEAD 已有，对本次补丁不构成证据）");

static_assert(
    std::is_same<
        decltype(AscendC::MmadMx<float, fp4x2_e1m2_t, fp4x2_e1m2_t>(
            std::declval<LocalTensor<float>&>(), std::declval<LocalTensor<fp4x2_e1m2_t>&>(),
            std::declval<LocalTensor<fp4x2_e1m2_t>&>(), std::declval<LocalTensor<float>&>(),
            std::declval<const MmadBitModeParams&>())),
        void>::value,
    "MmadMx BitMode **带 bias** 的 Tensor 形必须可解析（权威清单第 61 行的 Tensor 半边）");
static_assert(
    std::is_same<
        decltype(AscendC::MmadMx(
            std::declval<__cc__ float*>(), std::declval<__ca__ fp4x2_e1m2_t*>(), std::declval<__cb__ fp4x2_e1m2_t*>(),
            std::declval<__cc__ float*>(), std::declval<MmadBitModeParams&>())),
        void>::value,
    "MmadMx BitMode **带 bias** 的指针形必须可解析（权威清单第 61 行）");
namespace {
inline MmadParams MakeMmadParams()
{
    MmadParams p;
    p.m = 112;
    p.k = 32;
    p.n = 128;
    p.cmatrixInitVal = true;
    p.cmatrixSource = false;
    return p;
}
} // namespace

// ARM T -- Tensor form with MmadBitModeParams.
__global__ __aicore__ void KernelMmadMxBitModeTensorArm(__gm__ int32_t* result)
{
    LOCAL_TENSOR_REGISTER(featureL0a, fp4x2_e1m2_t, A2, 0, MX_FEATURE_SIZE)
    LOCAL_TENSOR_REGISTER(weightL0b, fp4x2_e1m2_t, B2, 0, MX_WEIGHT_SIZE)
    LOCAL_TENSOR_REGISTER(l0cOut, float, CO1, 0, MX_OUTPUT_SIZE)
    MmadParams mmadParams = MakeMmadParams();
    MmadBitModeParams bitModeParams(mmadParams);

    AscendC::MmadMx<float, fp4x2_e1m2_t, fp4x2_e1m2_t>(l0cOut, featureL0a, weightL0b, bitModeParams);

    result[0] = SENTINEL;
}

// ARM P -- pointer form with MmadBitModeParams.  A2 -> __ca__ (L0A), B2 -> __cb__ (L0B), CO1 -> __cc__ (L0C).
__global__ __aicore__ void KernelMmadMxBitModePointerArm(__gm__ int32_t* result)
{
    LOCAL_TENSOR_REGISTER(featureL0a, fp4x2_e1m2_t, A2, 0, MX_FEATURE_SIZE)
    LOCAL_TENSOR_REGISTER(weightL0b, fp4x2_e1m2_t, B2, 0, MX_WEIGHT_SIZE)
    LOCAL_TENSOR_REGISTER(l0cOut, float, CO1, 0, MX_OUTPUT_SIZE)
    MmadParams mmadParams = MakeMmadParams();
    MmadBitModeParams bitModeParams(mmadParams);

    AscendC::MmadMx(
        (__cc__ float*)l0cOut.GetPhyAddr(), (__ca__ fp4x2_e1m2_t*)featureL0a.GetPhyAddr(),
        (__cb__ fp4x2_e1m2_t*)weightL0b.GetPhyAddr(), bitModeParams);

    result[0] = SENTINEL;
}

// ARM P2 -- the Mmad-side BitMode pointer form (same group; the authoritative list counts it too).
__global__ __aicore__ void KernelMmadBitModePointerArm(__gm__ int32_t* result)
{
    LOCAL_TENSOR_REGISTER(featureL0a, fp4x2_e1m2_t, A2, 0, MX_FEATURE_SIZE)
    LOCAL_TENSOR_REGISTER(weightL0b, fp4x2_e1m2_t, B2, 0, MX_WEIGHT_SIZE)
    LOCAL_TENSOR_REGISTER(l0cOut, float, CO1, 0, MX_OUTPUT_SIZE)
    MmadParams mmadParams = MakeMmadParams();
    MmadBitModeParams bitModeParams(mmadParams);

    AscendC::Mmad(
        (__cc__ float*)l0cOut.GetPhyAddr(), (__ca__ fp4x2_e1m2_t*)featureL0a.GetPhyAddr(),
        (__cb__ fp4x2_e1m2_t*)weightL0b.GetPhyAddr(), bitModeParams);

    result[0] = SENTINEL;
}

// ARM P3 -- the **带 bias** 的指针形（权威清单第 61 行）。bias 放在 C2（Hardware::BIAS），
// 与仓库自带 test_operator_MM_bitmode_Mx.cpp 的用法一致；后端会按 bias 的位置区分 C2 / L0C。
__global__ __aicore__ void KernelMmadMxBitModeBiasPointerArm(__gm__ int32_t* result)
{
    LOCAL_TENSOR_REGISTER(featureL0a, fp4x2_e1m2_t, A2, 0, MX_FEATURE_SIZE)
    LOCAL_TENSOR_REGISTER(weightL0b, fp4x2_e1m2_t, B2, 0, MX_WEIGHT_SIZE)
    LOCAL_TENSOR_REGISTER(l0cOut, float, CO1, 0, MX_OUTPUT_SIZE)
    LOCAL_TENSOR_REGISTER(biasC2, float, C2, 0, MX_OUTPUT_SIZE)
    MmadParams mmadParams = MakeMmadParams();
    MmadBitModeParams bitModeParams(mmadParams);

    AscendC::MmadMx(
        (__cc__ float*)l0cOut.GetPhyAddr(), (__ca__ fp4x2_e1m2_t*)featureL0a.GetPhyAddr(),
        (__cb__ fp4x2_e1m2_t*)weightL0b.GetPhyAddr(), (__cc__ float*)biasC2.GetPhyAddr(), bitModeParams);

    result[0] = SENTINEL;
}

// ARM P4 -- 权威清单第 60 行：**Mmad** 的 4 参（带 bias）BitMode **指针**形。
// 只设指针臂：该形的**张量**形在上游「有声明无定义」（include:303 非 const vs 四个定义全 const），
// 张量臂会链接失败 —— 那是上游既有缺陷（已报 #1737），不属本任务的指针化范围。
// 指针形自身转发到 MmadCal 的非 const BitMode-bias 形（dav_3510/..._mm_impl.h:566），是完好的。
__global__ __aicore__ void KernelMmadBitModeBiasPointerArm(__gm__ int32_t* result)
{
    LOCAL_TENSOR_REGISTER(featureL0a, fp4x2_e1m2_t, A2, 0, MX_FEATURE_SIZE)
    LOCAL_TENSOR_REGISTER(weightL0b, fp4x2_e1m2_t, B2, 0, MX_WEIGHT_SIZE)
    LOCAL_TENSOR_REGISTER(l0cOut, float, CO1, 0, MX_OUTPUT_SIZE)
    LOCAL_TENSOR_REGISTER(biasC2, float, C2, 0, MX_OUTPUT_SIZE)
    MmadParams mmadParams = MakeMmadParams();
    MmadBitModeParams bitModeParams(mmadParams);

    AscendC::Mmad(
        (__cc__ float*)l0cOut.GetPhyAddr(), (__ca__ fp4x2_e1m2_t*)featureL0a.GetPhyAddr(),
        (__cb__ fp4x2_e1m2_t*)weightL0b.GetPhyAddr(), (__cc__ float*)biasC2.GetPhyAddr(), bitModeParams);

    result[0] = SENTINEL;
}

#define MMADMX_BITMODE_RUN(name)                                          \
    static int32_t result[1] = {0};                                       \
    name(result);                                                         \
    EXPECT_EQ(result[0], SENTINEL) << #name " did not run to completion"; \
    (void)printf(                                                         \
        "[mmadmx-bitmode] %-30s sentinel=%d raiseCount=%d (recorded, not asserted)\n", #name, result[0], g_raiseCount)

TEST_F(MmadMxBitModePointerArmTestsuite, MmadMxBitModeTensorArmRuns)
{
    MMADMX_BITMODE_RUN(KernelMmadMxBitModeTensorArm);
}
TEST_F(MmadMxBitModePointerArmTestsuite, MmadMxBitModePointerArmRuns)
{
    MMADMX_BITMODE_RUN(KernelMmadMxBitModePointerArm);
}
TEST_F(MmadMxBitModePointerArmTestsuite, MmadMxBitModeBiasPointerArmRuns)
{
    MMADMX_BITMODE_RUN(KernelMmadMxBitModeBiasPointerArm);
}
// 如实标注：Mmad 的 BitMode 指针形在 **HEAD 就已存在**，本臂对**本次补丁**不构成证据，
// 保留它只是同族冒烟（独立审计判定其为装饰品，此处按审计意见写明）。
TEST_F(MmadMxBitModePointerArmTestsuite, MmadBitModeBiasPointerArmRuns)
{
    MMADMX_BITMODE_RUN(KernelMmadBitModeBiasPointerArm);
}
TEST_F(MmadMxBitModePointerArmTestsuite, MmadBitModePointerArmRuns) { MMADMX_BITMODE_RUN(KernelMmadBitModePointerArm); }
