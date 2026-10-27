# Fixpipe（L0C→UB）适配报告

> 适配对象：V220 兼容入口的 L0C→L1/UB `Fixpipe` 变体（`LocalTensor` dst 3 参/4 参，intf.h L77-104 宏控块）
> 适配版本：3003（dav_l300）、3103（dav_l310）、3113（dav_l311）；5101（dav_5161）保持显式"不支持"桩
> 移植来源：3510（dav_3510）的 dtype 矩阵与 FixpipeL0cToUB 结构；实现模式严格仿各版本存量 L0C→L1 路径
> 前置分析：`Fixpipe(L0C-UB)-analysis.md`（适配前支持矩阵、各版本适配前代码见该报告）

---

## 1. 适配后支持矩阵

| 版本 | 实现文件 | L0C→L1（3/4 参） | L0C→UB（3/4 参） | L0C→GM（3/4 参） | 本次变化 |
|---|---|---|---|---|---|
| 2201 | dav_c220（未改动） | 维持 | 维持 | 维持 | 无（宏控与实现均未改动） |
| 3002 | dav_m300（未改动） | 完整（维持） | 完整（维持） | 完整（维持） | 无（走 M300 原生声明） |
| 3102 | dav_m310（未改动） | 完整（维持） | 完整（维持） | 完整（维持） | 无（走 M310 原生声明） |
| 3003 | dav_l300（**已修改**） | 完整 → 完整（维持） | **Stub（断言报错）→ 完整** | 完整（维持） | UB 激活 |
| 3103 | dav_l310（**已修改**） | **死代码 → 完整**（宏控放行激活存量实现） | **死代码 Stub → 完整**（宏控放行 + 新增实现） | **死代码 → 完整**（宏控放行激活存量实现） | 宏控放行（用户）+ UB 新增实现 |
| 3113 | dav_l311（**已修改**） | 完整（维持） | **Stub（断言报错）→ 完整** | 完整（维持） | UB 激活 |
| 3510 | dav_3510（未改动，移植来源） | 完整（维持） | 完整（维持） | 完整（维持） | 无 |
| 5101 | dav_5161（**用户修改**） | 无实现（维持，声明不可见） | **无 → 显式桩**（`ASCENDC_REPORT_NOT_SUPPORT`） | 无实现（维持，声明不可见） | UB 显式桩（用户） |
| 5102 | dav_m510（未改动） | 维持 | 维持 | 维持 | 无 |

说明：
- "L0C→L1 / L0C→UB" 共用同一组 `LocalTensor` dst 声明，由共享包装按 dst 物理位置（L1/UB）分发；"L0C→GM" 为 `GlobalTensor` dst 声明（本次宏控放行使 3103 的 GM 声明一并可见，存量 GM 实现随之激活）。
- 3103 适配前：V220 宏控（intf.h L77-78）不含 3103，M300/M310 原生声明也不可见 → 3103 上无任何 Fixpipe 声明，dav_l310 中完整的 L1/GM 适配器与本地实现均为死代码，UB 为 stub（不可达）。
- 2201/5102：宏控维持，dav_c220/dav_m510 本次未改动，其实现细节未在本次分析范围内（见分析报告）。
- 5101 无 L0C，不引入 L0C→UB 实现；新增显式桩仅为防御性兜底（当前 5101 上 Fixpipe 声明不可见，桩不会被实例化）。

---

## 2. 修改的文件清单

| # | 文件 | 修改内容 | 行号（适配后） | 修改者 |
|---|---|---|---|---|
| 1 | `include/basic_api/kernel_operator_fixpipe_intf.h` | V220 宏控增加 `\|\| __NPU_ARCH__ == 3103`（声明层放行 3103，整块 L1/UB/GM 变体生效） | L77-78 | 用户（会话内外部修改，已验证） |
| 2 | `impl/basic_api/dav_l300/kernel_operator_fixpipe_impl.h`（550 → 667 行） | ① 新增 `FixpipeL0cToUB`（22 参 intrinsic，dtype 矩阵不含 bf16）② 新增 `FixpipeL0C2UBImplN` ③ 新增 `FixpipeL0C2UBImpl` 3 参/4 参（4 参非向量量化新增断言）④ V220 UB 适配器 3 参/4 参替换原 stub | L255-287、L374-385、L463-489、L491-527、L645-660 | 助手 |
| 3 | `impl/basic_api/dav_l310/kernel_operator_fixpipe_impl.h`（557 → 678 行） | ① 新增 `FixpipeL0cToUB`（24 参 intrinsic，3 分支含 nz2dnEn，dtype 矩阵不含 bf16）② 新增 `FixpipeL0C2UBImplN` ③ 新增 `FixpipeL0C2UBImpl` 3 参/4 参 ④ V220 UB 适配器替换 stub ⑤ 类型一致性修复：新增代码 8 处 `FixpipeParamsL311`→`FixpipeParamsL310`（对齐文件内 L310 本地类型及 `TransFixpipeParamsV220ToFixpipeParamsL310`） | L259-295、L385-396、L474-500、L502-538、L656-671 | 助手（用户会话内亦对该文件做过 L311→L310 类型重构） |
| 4 | `impl/basic_api/dav_l311/kernel_operator_fixpipe_impl.h`（557 → 679 行） | ① 新增 `FixpipeL0cToUB`（24 参 intrinsic，dtype 矩阵**含 bf16**）② 新增 `FixpipeL0C2UBImplN` ③ 新增 `FixpipeL0C2UBImpl` 3 参/4 参 ④ V220 UB 适配器替换 stub | L259-296、L386-397、L475-501、L503-539、L657-672 | 助手 |
| 5 | `impl/basic_api/dav_5161/kernel_operator_fixpipe_impl.h`（86 → 106 行） | 新增 `FixpipeL0C2UBImpl` 3 参/4 参显式桩（`ASCENDC_REPORT_NOT_SUPPORT(false, "Fixpipe L0C to UB")` 格式） | L81-88、L90-99 | 用户（会话内外部修改，已验证） |
| 6 | 其余文件 | 未改动：`kernel_struct_fixpipe.h`、`kernel_operator_fixpipe_intf_impl.h`（共享包装）、dav_m300 / dav_m310 / dav_3510 / dav_m510 / dav_c220 等全部维持版本实现 | — | — |

---

## 3. 适配后各版本原始代码

### 3.0 声明层（intf.h，节选 L77-104，**修改**：宏控增加 3103）

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3103 || \
                              __NPU_ARCH__ == 3113 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102)
// L0C->L1
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams);

// L0C->L1 deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsV220& intriParams);

// L0C->GM
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams);

// L0C->GM deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsV220& intriParams);
#endif
```

M300/M310 原生声明段（L106-157，宏控 3002/3102）与 Arch3510 专属段（L158-180）**维持**，不在本次适配范围。

### 3.0.1 共享包装层（intf_impl.h，**维持**）

include 分发（L30-54，按 `__NPU_ARCH__` 选择版本实现文件）：

```cpp
#if __NPU_ARCH__ == 1001
#include "dav_c100/kernel_operator_fixpipe_impl.h"
#elif __NPU_ARCH__ == 2002
#include "dav_m200/kernel_operator_fixpipe_impl.h"
#elif __NPU_ARCH__ == 2201
#include "dav_c220/kernel_operator_fixpipe_impl.h"
#include "dav_c220/kernel_operator_fixpipe_v2_impl.h"
#elif __NPU_ARCH__ == 3002
#include "dav_m300/kernel_operator_fixpipe_impl.h"
#include "dav_m300/kernel_operator_fixpipe_v2_impl.h"
#elif __NPU_ARCH__ == 3102
#include "dav_m310/kernel_operator_fixpipe_impl.h"
#elif __NPU_ARCH__ == 3510
#include "dav_3510/kernel_operator_fixpipe_impl.h"
#elif (__NPU_ARCH__ == 5102)
#include "dav_m510/kernel_operator_fixpipe_impl.h"
#elif __NPU_ARCH__ == 3003
#include "dav_l300/kernel_operator_fixpipe_impl.h"
#elif __NPU_ARCH__ == 3113
#include "dav_l311/kernel_operator_fixpipe_impl.h"
#elif (__NPU_ARCH__ == 3103)
#include "dav_l310/kernel_operator_fixpipe_impl.h"
#elif (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
#include "dav_5161/kernel_operator_fixpipe_impl.h"
#endif
```

L1/UB 包装（L109-147，无架构宏控，按 dst 物理位置分发；L1 分支落到 `FixpipeL0C2L1Impl`，UB 分支落到 `FixpipeL0C2UBImpl`）：

```cpp
// L0C -> L1 for v220
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsV220& intriParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckFixpipeTensor<T, U, config>(dst, src, intriParams, "Fixpipe");
#endif

    if ((GetPhyType((TPosition)dst.GetPosition()) == Hardware::L1)) {
        FixpipeL0C2L1Impl<PrimT<T>, PrimT<U>, config>(
            (__cbuf__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams);
    } else if ((GetPhyType((TPosition)dst.GetPosition()) == Hardware::UB)) {
        FixpipeL0C2UBImpl<PrimT<T>, PrimT<U>, config>(
            (__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams);
    }
}

// L0C->L1 deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config, typename S,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsV220& intriParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckFixpipeTensor<T, U, config, S>(dst, src, cbufWorkspace, intriParams, "Fixpipe");
#endif
    if ((GetPhyType((TPosition)dst.GetPosition()) == Hardware::L1)) {
        FixpipeL0C2L1Impl<PrimT<T>, PrimT<U>, config>(
            (__cbuf__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(),
            (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
    } else if ((GetPhyType((TPosition)dst.GetPosition()) == Hardware::UB)) {
        FixpipeL0C2UBImpl<PrimT<T>, PrimT<U>, config>(
            (__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(),
            (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr(), intriParams);
    }
}
```

GM 包装（L149-188）与 Arch3510 专属包装（L190-298）**维持**，本报告不重复。

### 3.1 3003（dav_l300/kernel_operator_fixpipe_impl.h，667 行）

函数修改状态：

| 函数/结构 | 行号（适配后） | 状态 |
|---|---|---|
| SetFixPipeClipReluImpl / SetFixPipeAddrImpl | L29-39 | 维持 |
| SetFixPipeConfigImpl ×2 / SetFixpipeNz2ndFlagImpl / SetFixpipePreQuantFlagImpl | L44-78 | 维持 |
| TransformParamsL300 / FixpipeParamsL300 | L83-131 | 维持 |
| FixpipeTiling / GenFixpipeTiling / IsVectorQuantMode / IsScalarQuantMode | L133-176 | 维持 |
| CopyDeqTensorToFbuf | L178-195 | 维持 |
| SetLoop3Para | L197-219 | 维持 |
| FixpipeL0cToL1（→ 22 参 copy_matrix_cc_to_cbuf） | L221-253 | 维持 |
| **FixpipeL0cToUB（→ 22 参 copy_matrix_cc_to_ub）** | **L255-287** | **新增** |
| GetGMLen | L289-308 | 维持 |
| FixpipeL0cToOut（→ copy_matrix_cc_to_gm） | L310-359 | 维持 |
| FixpipeL0C2L1ImplN | L361-372 | 维持 |
| **FixpipeL0C2UBImplN** | **L374-385** | **新增** |
| FixpipeL0C2GMImplN | L387-398 | 维持 |
| FixpipeL0C2L1Impl 3 参 / 4 参 | L400-426 / L428-461 | 维持 |
| **FixpipeL0C2UBImpl 3 参** | **L463-489** | **新增** |
| **FixpipeL0C2UBImpl 4 参**（非向量量化新增断言） | **L491-527** | **新增** |
| FixpipeL0C2GMImpl 3 参 / 4 参 | L529-552 / L554-585 | 维持 |
| TransFixpipeParamsV220ToFixpipeParamsL300 | L587-609 | 维持 |
| V220 适配器：L1 3/4 参、GM 3/4 参 | L611-643 | 维持 |
| **V220 适配器：UB 3 参 / 4 参**（替换原 stub） | **L645-651 / L653-660** | **新增** |

完整代码：

```cpp
/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_operator_fixpipe_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_l300/kernel_operator_fixpipe_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_FIXPIPE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_FIXPIPE_IMPL_H

#include "kernel_operator_set_spr_impl.h"
#include "../../../include/basic_api/kernel_struct_fixpipe.h"

namespace AscendC {

__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}

template <typename T>
__aicore__ inline void SetFixPipeAddrImpl(const LocalTensor<T>& eleWiseTensor, uint16_t c0ChStride)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeAddr");
}

/* **************************************************************************************************
 * SPR                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void SetFixPipeConfigImpl(
    const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false)
{
    uint64_t config = 0;
    config = config | ((uint64_t)reluPre.GetPhyAddr() >> 6);         // align with 64bit, FPC[7:0], ReluPreAddr
    config = config | (((uint64_t)quantPre.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    config = config | (static_cast<uint64_t>(isUnitFlag) << 63);     // FPC[63], UnitFlag.
    set_fpc(config);
}

template <typename T, bool setRelu = false>
__aicore__ inline void SetFixPipeConfigImpl(const LocalTensor<T>& preTensor, bool isUnitFlag = false)
{
    uint64_t config = 0;
    if constexpr (setRelu) {
        config = config | ((uint64_t)preTensor.GetPhyAddr() >> 6); // align with 64bit, FPC[7:0], ReluPreAddr.
    } else {
        config = config | (((uint64_t)preTensor.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    }
    config = config | (static_cast<uint64_t>(isUnitFlag) << 63); // FPC[63], UnitFlag.
    set_fpc(config);
}

__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    // ND_PARA[63:32], dst nd stride in unit of element
    // ND_PARA[31:16], src nd stride in uint of C0_SIZE
    // ND_PARA[15:0], nd number.
    uint64_t config = (static_cast<uint64_t>(dstNdStride) << 32) | (static_cast<uint64_t>(srcNdStride) << 16) |
                      (static_cast<uint64_t>(ndNum));
    set_loop3_para(config);
}

__aicore__ inline void SetFixpipePreQuantFlagImpl(uint64_t config) { set_quant_pre(config); }

/* **************************************************************************************************
 * Fixpipe                                             *
 * ************************************************************************************************* */
template <CO2Layout format>
struct TransformParamsL300 {};

template <>
struct TransformParamsL300<CO2Layout::NZ> {
    __aicore__ inline TransformParamsL300(){};
    using PARAMS = uint8_t;
};

template <>
struct TransformParamsL300<CO2Layout::ROW_MAJOR> {
    __aicore__ inline TransformParamsL300(){};
    using PARAMS = Nz2NdParams;
};

template <>
struct TransformParamsL300<CO2Layout::COLUMN_MAJOR> {
    __aicore__ inline TransformParamsL300(){};
    using PARAMS = uint8_t;
};

template <CO2Layout format = CO2Layout::ROW_MAJOR>
struct FixpipeParamsL300 {
    __aicore__ FixpipeParamsL300() {}

    __aicore__ FixpipeParamsL300(
        const uint16_t nSizeIn, const uint16_t mSizeIn, const uint16_t srcStrideIn, const uint32_t dstStrideIn)
    {
        nSize = nSizeIn;
        mSize = mSizeIn;
        srcStride = srcStrideIn;
        dstStride = dstStrideIn;
    }

    uint16_t nSize = 0;
    uint16_t mSize = 0; // M-DirectionSize
    uint16_t srcStride = 0;
    uint32_t dstStride = 0;
    // Params: used for Quant
    QuantMode_t quantPre = QuantMode_t::NoQuant;
    uint64_t deqScalar;
    bool reluEn = false;
    uint8_t unitFlag = 0;
    // extend param
    uint8_t dualDstCtl = 0;
    bool subBlockId = false;
    typename TransformParamsL300<format>::PARAMS params;
    bool isChannelSplit = false;
};

// tiling params
struct FixpipeTiling {
    uint16_t nIterNum = 0;
    uint16_t nSize = 0;
    bool isDb = false;
    uint16_t tailNSize = 0;
};

// fixpipe tiling calculating
__aicore__ inline FixpipeTiling GenFixpipeTiling(uint16_t n)
{
    FixpipeTiling tiling;
    // deqTensor/reluTensor in FB valid num is 1024
    uint16_t maxDeqNums = 1024;
    if (n <= maxDeqNums) {
        tiling.nIterNum = 1;
        tiling.nSize = n;
        tiling.isDb = false;
        tiling.tailNSize = 0;
    } else {
        tiling.isDb = true;
        uint16_t dbMaxDeqNums = maxDeqNums / 2;
        tiling.nIterNum = n / dbMaxDeqNums;
        tiling.nSize = dbMaxDeqNums;
        tiling.tailNSize = n % dbMaxDeqNums;
    }
    return tiling;
}

__aicore__ inline bool IsVectorQuantMode(QuantMode_t quantPre)
{
    return (
        quantPre == QuantMode_t::VDEQF16 || quantPre == QuantMode_t::VQF162B8_PRE || quantPre == QuantMode_t::VREQ8 ||
        quantPre == QuantMode_t::VQF162S4_PRE || quantPre == QuantMode_t::VREQ4 || quantPre == QuantMode_t::VDEQS16 ||
        quantPre == QuantMode_t::VQF162S16_PRE);
}

__aicore__ inline bool IsScalarQuantMode(QuantMode_t quantPre)
{
    return (
        quantPre == QuantMode_t::DEQF16 || quantPre == QuantMode_t::QF162B8_PRE || quantPre == QuantMode_t::REQ8 ||
        quantPre == QuantMode_t::QF162S4_PRE || quantPre == QuantMode_t::REQ4 || quantPre == QuantMode_t::DEQS16 ||
        quantPre == QuantMode_t::QF162S16_PRE);
}

__aicore__ inline void CopyDeqTensorToFbuf(
    __cbuf__ uint64_t* cbufWorkspace, const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex)
{
    constexpr uint32_t deqTensorAddrAlignValue = 128;
    uint16_t deqDataSize = DivCeil(calNSize * sizeof(uint64_t), deqTensorAddrAlignValue) * deqTensorAddrAlignValue;
    __fbuf__ uint64_t* deqTensorTempBuf =
        AscendCUtils::GetTemporaryFbBufferAddr<uint64_t>(0, deqDataSize / sizeof(uint64_t));
    uint32_t deqValueOffset = nIterIndex * fixpipeTiling.nSize;

    // L1 -> FB
    constexpr uint16_t fbufBurstLenUnit = 128;
    uint16_t fbufBurstLen = deqDataSize / fbufBurstLenUnit; // copy from cbuf to fbuf, burst_len unit is 128Bytes
    copy_cbuf_to_fbuf(deqTensorTempBuf, cbufWorkspace + deqValueOffset, 1, fbufBurstLen, 0, 0);
    // FPC of fixpipe buffer for Quant_PRE is FPC[15:8], unit is 128Bytes
    uint64_t deqTensorAddr = ((uint64_t)deqTensorTempBuf >> static_cast<uint64_t>(7)) << 8;
    set_fpc(deqTensorAddr);
    AscendCUtils::FreeTemporaryFbBuffer<uint64_t>(deqTensorTempBuf);
}

template <const FixpipeConfig& config>
__aicore__ inline void SetLoop3Para(const FixpipeParamsL300<config.format>& intriParams)
{
    // the loop3DstStride range has increased.
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        ASCENDC_ASSERT((intriParams.params.ndNum > 0), { KERNEL_LOG(KERNEL_ERROR, "ndNum must be larger than 0"); });
        // Loop3_dst_stride in uint of element
        uint64_t loop3Para = static_cast<uint64_t>(intriParams.params.dstNdStride) << 32; // LOOP3_PARA[63:32]
        // original src_nd_stride in uint of fractal_size(1024B = 16 * 16 * sizeof(SrcT))
        // src_nd_stride in unit of C0_SIZE, Loop3_src_stride
        loop3Para |= static_cast<uint64_t>(intriParams.params.srcNdStride) << 16; // LOOP3_PARA[31:16]
        loop3Para |= static_cast<uint64_t>(intriParams.params.ndNum);             // LOOP3_PARA[15:0]
        set_loop3_para(loop3Para);
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        ASCENDC_ASSERT((intriParams.params.dnNum > 0), { KERNEL_LOG(KERNEL_ERROR, "dnNum must be larger than 0"); });
        // Loop3_dst_stride in uint of element
        uint64_t loop3Para = static_cast<uint64_t>(intriParams.params.dstDnMatrixStride) << 32; // LOOP3_PARA[63:32]
        // src_nd_stride in unit of C0_SIZE, Loop3_src_stride
        loop3Para |= static_cast<uint64_t>(intriParams.params.srcNzMatrixStride) << 16; // LOOP3_PARA[31:16]
        loop3Para |= static_cast<uint64_t>(intriParams.params.dnNum);                   // LOOP3_PARA[15:0]
        set_loop3_para(loop3Para);
    }
}

// contains loop info and cal n size for each loop
// move data L0C->L1
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToL1(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL300<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    ASCENDC_DEBUG_ASSERT(
        (!(intriParams.isChannelSplit)),
        KERNEL_LOG_INTERNAL(
            KERNEL_ERROR, "Failed to check isChannelSplit in Fixpipe, when src position is "
                          "L0C Buffer(CO1) and dst position is L1 Buffer(C1), isChannelSplit must be set as false \n"));

    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    // Loop1_src_stride in unit of C0_size
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    // If NZ2ND/NZ2DN is enabled, it is the dst_D value in unit of element. Loop2_dst_stride
    // If NZ2ND/NZ2DN is disabled, it is in unit of element, Loop1_dst_stride
    bool nz2ndEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    // LOC -> L1 only n direction need fixpipeTiling, m no need fixpipeTiling
    return copy_matrix_cc_to_cbuf(
        (__cbuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag,
        static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn), false, nz2ndEn,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false, false);
}

// contains loop info and cal n size for each loop
// move data L0C->UB
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToUB(
    __ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL300<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    static_assert(
        SupportType<
            Tuple<SrcT, DstT>, Tuple<float, half>, Tuple<float, int8_t>, Tuple<float, uint8_t>, Tuple<float, float>,
            Tuple<int32_t, half>, Tuple<int32_t, int8_t>, Tuple<int32_t, uint8_t>, Tuple<int32_t, int32_t>>(),
        "Failed to check dtype in Fixpipe");
    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    // Loop1_src_stride in unit of C0_size
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    // If NZ2ND/NZ2DN is enabled, it is the dst_D value in unit of element. Loop2_dst_stride
    // If NZ2ND/NZ2DN is disabled, it is in unit of element, Loop1_dst_stride
    bool nz2ndEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    // LOC -> UB only n direction need fixpipeTiling, m no need fixpipeTiling
    return copy_matrix_cc_to_ub(
        (__ubuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag,
        static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn),
        intriParams.isChannelSplit, nz2ndEn, static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0,
        false, false, false, false);
}

template <const FixpipeConfig& config>
__aicore__ inline uint64_t GetGMLen(
    const FixpipeParamsL300<config.format>& intriParams, const uint16_t calNSize, const uint16_t dstEleSize)
{
    constexpr uint16_t fractalNsize = 16;
    uint64_t cburstNum = calNSize / fractalNsize;
    uint64_t gmLen =
        (cburstNum - 1) * intriParams.dstStride * dstEleSize + intriParams.mSize * fractalNsize * dstEleSize;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        // dstStride is dst_D Loop2_dst_stride
        gmLen = (static_cast<uint64_t>(intriParams.params.ndNum) - 1) * dstEleSize * intriParams.params.dstNdStride +
                (intriParams.mSize - 1) * intriParams.dstStride * dstEleSize + cburstNum * fractalNsize * dstEleSize;
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        gmLen =
            (static_cast<uint64_t>(intriParams.params.dnNum) - 1) * dstEleSize * intriParams.params.dstDnMatrixStride +
            (intriParams.nSize - 1) * intriParams.dstStride * dstEleSize +
            intriParams.mSize / fractalNsize * fractalNsize * dstEleSize;
    }
    return gmLen;
}

// contains loop info and cal n size for each loop
// move data L0C->GM
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToOut(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL300<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    if (intriParams.isChannelSplit) {
        ASCENDC_DEBUG_ASSERT(
            (IsSameType<SrcT, float>::value && IsSameType<DstT, float>::value),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR, "Failed to check "
                              "isChannelSplit value in Fixpipe, isChannelSplit can be set true only when src and dst "
                              "are both float \n"));
        ASCENDC_DEBUG_ASSERT(
            (config.format == CO2Layout::NZ),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR, "Failed to check format value in Fixpipe, "
                              "when isChannelSplit is set true, format must be set as NZ \n"));
    }
    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    bool nz2ndEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    if constexpr (g_gm_overflow_check) {
        bool isSrc = false;
        uint16_t dstEleSize = sizeof(DstT);
        uint64_t gmLen = GetGMLen<config>(intriParams, calNSize, dstEleSize);
        AscendCUtils::CheckGmMemOverflow((__gm__ DstT*)(dst + dstOffset), isSrc, gmLen);
    }

    // LOC -> GM only n direction need fixpipeTiling, m no need fixpipeTiling
    if constexpr (!(IsSameType<SrcT, float>::value || IsSameType<DstT, float>::value)) {
        return copy_matrix_cc_to_gm(
            (__gm__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
            intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag,
            static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn),
            intriParams.isChannelSplit, nz2ndEn, static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0,
            false, false, false, false);
    }
    ASCENDC_DEBUG_ASSERT(
        !(IsSameType<SrcT, float>::value || IsSameType<DstT, float>::value),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "unsupported float L0c 2 GM "));
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1ImplN(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL300<config.format>& intriParams, const FixpipeTiling& fixpipeTiling, uint16_t calNSize,
    uint16_t nIterIndex)
{
    // mov deq tensor from L1 to FB
    CopyDeqTensorToFbuf(cbufWorkspace, fixpipeTiling, calNSize, nIterIndex);
    PipeBarrier<PIPE_FIX>();
    // L0C->L1
    FixpipeL0cToL1<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, calNSize, nIterIndex);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImplN(
    __ubuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL300<config.format>& intriParams, const FixpipeTiling& fixpipeTiling, uint16_t calNSize,
    uint16_t nIterIndex)
{
    // mov deq tensor from L1 to FB
    CopyDeqTensorToFbuf(cbufWorkspace, fixpipeTiling, calNSize, nIterIndex);
    PipeBarrier<PIPE_FIX>();
    // L0C->UB
    FixpipeL0cToUB<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, calNSize, nIterIndex);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImplN(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL300<config.format>& intriParams, const FixpipeTiling& fixpipeTiling, uint16_t calNSize,
    uint16_t nIterIndex)
{
    // mov deq tensor from L1 to FB
    CopyDeqTensorToFbuf(cbufWorkspace, fixpipeTiling, calNSize, nIterIndex);
    PipeBarrier<PIPE_FIX>();
    // L0C->GM
    FixpipeL0cToOut<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, calNSize, nIterIndex);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL300<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to l1
    */
    if (IsScalarQuantMode(intriParams.quantPre)) {
        // deq factor of uint64 bits describe: bits[31:13] is deq value of fp32,
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    FixpipeTiling fixpipeTiling;
    // LOC -> L1
    FixpipeL0cToL1<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL300<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for vector quant mode:
    1. generate fixpipeTiling
    2. copy deq tensor from l1 to fb1 (l1 -> fb1)
    3. code gen: move data from l0c to l1
    */
    if (IsVectorQuantMode(intriParams.quantPre)) {
        FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        // deal with the tail, it also need copy deq/relu tensor from L1 to fb1
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL300<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to ub
    */
    if (IsScalarQuantMode(intriParams.quantPre)) {
        // deq factor of uint64 bits describe: bits[31:13] is deq value of fp32,
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    FixpipeTiling fixpipeTiling;
    // LOC -> UB
    FixpipeL0cToUB<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL300<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for vector quant mode:
    1. generate fixpipeTiling
    2. copy deq tensor from l1 to fb1 (l1 -> fb1)
    3. code gen: move data from l0c to ub
    */
    if (IsVectorQuantMode(intriParams.quantPre)) {
        FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2UBImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        // deal with the tail, it also need copy deq/relu tensor from L1 to fb1
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2UBImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(KERNEL_ERROR, "Fixpipe with cbufWorkspace only supports vector quant mode");
    });
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL300<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);
    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to gm
    */
    if (IsScalarQuantMode(intriParams.quantPre)) {
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    // LOC -> GM
    FixpipeTiling fixpipeTiling;
    FixpipeL0cToOut<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL300<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);
    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for vector quant mode:
    1. generate fixpipeTiling
    2. copy deq tensor from l1 to fb1 (l1 -> fb1)
    3. code gen: move data from l0c to gm
    */
    FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
    if (IsVectorQuantMode(intriParams.quantPre)) {
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2GMImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        // deal with the tail, it also need copy deq/relu tensor from L1 to fb1
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2GMImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
}

template <CO2Layout format>
__aicore__ inline void TransFixpipeParamsV220ToFixpipeParamsL300(
    const FixpipeParamsV220& intriParams, FixpipeParamsL300<format>& dstParams)
{
    dstParams.nSize = intriParams.nSize;
    dstParams.mSize = intriParams.mSize;
    dstParams.srcStride = intriParams.srcStride;
    dstParams.dstStride = intriParams.dstStride;
    dstParams.quantPre = intriParams.quantPre;
    dstParams.deqScalar = intriParams.deqScalar;
    dstParams.reluEn = intriParams.reluEn;
    dstParams.unitFlag = intriParams.unitFlag;
    dstParams.isChannelSplit = intriParams.isChannelSplit;
    if constexpr (format == CO2Layout::ROW_MAJOR) {
        dstParams.params.ndNum = intriParams.ndNum;
        dstParams.params.srcNdStride = intriParams.srcNdStride;
        dstParams.params.dstNdStride = intriParams.dstNdStride;
    } else if constexpr (format == CO2Layout::COLUMN_MAJOR) {
        dstParams.params.ndNum = intriParams.ndNum;
        dstParams.params.srcNdStride = intriParams.srcNdStride;
        dstParams.params.dstNdStride = intriParams.srcNdStride;
    }
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(__gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2UBImpl<DstT, SrcT, config>(dst, src, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2UBImpl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}

} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_FIXPIPE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_IMPL_H__
#endif
```

### 3.2 3103（dav_l310/kernel_operator_fixpipe_impl.h，678 行）

函数修改状态：

| 函数/结构 | 行号（适配后） | 状态 |
|---|---|---|
| SetFixPipeClipReluImpl / SetFixPipeAddrImpl / SetFixPipeConfigImpl ×2 / SetFixpipeNz2ndFlagImpl / SetFixpipePreQuantFlagImpl | L29-78 | 维持 |
| TransformParamsL310 / FixpipeParamsL310 | L83-131 | 维持（会话中经用户 L311→L310 本地类型重构） |
| FixpipeTiling / GenFixpipeTiling / IsVectorQuantMode / IsScalarQuantMode | L133-176 | 维持 |
| CopyDeqTensorToFbuf（FB burst 64B） | L178-195 | 维持 |
| SetLoop3Para | L197-219 | 维持 |
| FixpipeL0cToL1（→ 24 参 copy_matrix_cc_to_cbuf，含 nz2dnEn） | L221-257 | 维持 |
| **FixpipeL0cToUB（→ 24 参 copy_matrix_cc_to_ub，含 nz2dnEn；dtype 矩阵不含 bf16）** | **L259-295** | **新增** |
| GetGMLen | L297-315 | 维持 |
| FixpipeL0cToOut（→ 24 参 copy_matrix_cc_to_gm） | L317-370 | 维持 |
| FixpipeL0C2L1ImplN | L372-383 | 维持 |
| **FixpipeL0C2UBImplN** | **L385-396** | **新增** |
| FixpipeL0C2GMImplN | L398-409 | 维持 |
| FixpipeL0C2L1Impl 3 参 / 4 参 | L411-437 / L439-472 | 维持 |
| **FixpipeL0C2UBImpl 3 参** | **L474-500** | **新增** |
| **FixpipeL0C2UBImpl 4 参**（非向量量化新增断言） | **L502-538** | **新增** |
| FixpipeL0C2GMImpl 3 参 / 4 参 | L540-563 / L565-596 | 维持 |
| TransFixpipeParamsV220ToFixpipeParamsL310 | L598-620 | 维持（类型名经用户重构） |
| V220 适配器：L1 3/4 参、GM 3/4 参 | L622-637 / L639-654 | 维持 |
| **V220 适配器：UB 3 参 / 4 参**（替换原 stub） | **L656-662 / L664-671** | **新增** |

注：本文件新增代码最初以 `FixpipeParamsL311`/`TransFixpipeParamsV220ToFixpipeParamsL311` 写入（基于适配前读取，当时该文件与 dav_l311 逐行一致），后发现文件已在会话中被重构为本地类型 `FixpipeParamsL310`（含 `TransFixpipeParamsV220ToFixpipeParamsL310`），已将新增代码 8 处类型引用统一对齐为 L310，当前文件类型一致。

完整代码：

```cpp
/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_operator_fixpipe_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_l310/kernel_operator_fixpipe_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_FIXPIPE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_FIXPIPE_IMPL_H

#include "kernel_operator_set_spr_impl.h"
#include "../../../include/basic_api/kernel_struct_fixpipe.h"

namespace AscendC {

__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}

template <typename T>
__aicore__ inline void SetFixPipeAddrImpl(const LocalTensor<T>& eleWiseTensor, uint16_t c0ChStride)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeAddr");
}

/* **************************************************************************************************
 * SPR                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void SetFixPipeConfigImpl(
    const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false)
{
    uint64_t config = 0;
    config = config | ((uint64_t)reluPre.GetPhyAddr() >> 6);         // align with 64bit, FPC[7:0], ReluPreAddr
    config = config | (((uint64_t)quantPre.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    config = config | (static_cast<uint64_t>(isUnitFlag) << 63);     // FPC[63], UnitFlag.
    set_fpc(config);
}

template <typename T, bool setRelu = false>
__aicore__ inline void SetFixPipeConfigImpl(const LocalTensor<T>& preTensor, bool isUnitFlag = false)
{
    uint64_t config = 0;
    if constexpr (setRelu) {
        config = config | ((uint64_t)preTensor.GetPhyAddr() >> 6); // align with 64bit, FPC[7:0], ReluPreAddr.
    } else {
        config = config | (((uint64_t)preTensor.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    }
    config = config | (static_cast<uint64_t>(isUnitFlag) << 63); // FPC[63], UnitFlag.
    set_fpc(config);
}

__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    // ND_PARA[63:32], dst nd stride in unit of element
    // ND_PARA[31:16], src nd stride in uint of C0_SIZE
    // ND_PARA[15:0], nd number.
    uint64_t config = (static_cast<uint64_t>(dstNdStride) << 32) | (static_cast<uint64_t>(srcNdStride) << 16) |
                      (static_cast<uint64_t>(ndNum));
    set_loop3_para(config);
}

__aicore__ inline void SetFixpipePreQuantFlagImpl(uint64_t config) { set_quant_pre(config); }

/* **************************************************************************************************
 * Fixpipe                                             *
 * ************************************************************************************************* */
template <CO2Layout format>
struct TransformParamsL310 {};

template <>
struct TransformParamsL310<CO2Layout::NZ> {
    __aicore__ inline TransformParamsL310(){};
    using PARAMS = uint8_t;
};

template <>
struct TransformParamsL310<CO2Layout::ROW_MAJOR> {
    __aicore__ inline TransformParamsL310(){};
    using PARAMS = Nz2NdParams;
};

template <>
struct TransformParamsL310<CO2Layout::COLUMN_MAJOR> {
    __aicore__ inline TransformParamsL310(){};
    using PARAMS = uint8_t;
};

template <CO2Layout format = CO2Layout::ROW_MAJOR>
struct FixpipeParamsL310 {
    __aicore__ FixpipeParamsL310() {}

    __aicore__ FixpipeParamsL310(
        const uint16_t nSizeIn_, const uint16_t mSizeIn_, const uint16_t srcStrideIn_, const uint32_t dstStrideIn_)
    {
        nSize = nSizeIn_;
        mSize = mSizeIn_;
        srcStride = srcStrideIn_;
        dstStride = dstStrideIn_;
    }

    uint16_t nSize = 0;
    uint16_t mSize = 0; // M-DirectionSize
    uint16_t srcStride = 0;
    uint32_t dstStride = 0;
    // Params: used for Quant
    uint64_t deqScalar;
    QuantMode_t quantPre = QuantMode_t::NoQuant;
    bool reluEn = false;
    uint8_t unitFlag = 0;
    // extend param
    bool subBlockId = false;
    uint8_t dualDstCtl = 0;
    typename TransformParamsL310<format>::PARAMS params;
    bool isChannelSplit = false;
};

// tiling params
struct FixpipeTiling {
    uint16_t nIterNum = 0;
    uint16_t nSize = 0;
    bool isDb = false;
    uint16_t tailNSize = 0;
};

// fixpipe tiling calculating
__aicore__ inline FixpipeTiling GenFixpipeTiling(uint16_t n)
{
    FixpipeTiling tiling;
    // deqTensor/reluTensor in FB valid num is 1024
    uint16_t maxDeqNums = 1024;
    if (n <= maxDeqNums) {
        tiling.nIterNum = 1;
        tiling.nSize = n;
        tiling.isDb = false;
        tiling.tailNSize = 0;
    } else {
        tiling.isDb = true;
        uint16_t dbMaxDeqNums = maxDeqNums / 2;
        tiling.nIterNum = n / dbMaxDeqNums;
        tiling.nSize = dbMaxDeqNums;
        tiling.tailNSize = n % dbMaxDeqNums;
    }
    return tiling;
}

__aicore__ inline bool IsVectorQuantMode(QuantMode_t quantPre)
{
    return (
        quantPre == QuantMode_t::VDEQF16 || quantPre == QuantMode_t::VQF162B8_PRE || quantPre == QuantMode_t::VREQ8 ||
        quantPre == QuantMode_t::VQF162S4_PRE || quantPre == QuantMode_t::VREQ4 || quantPre == QuantMode_t::VDEQS16 ||
        quantPre == QuantMode_t::VQF162S16_PRE);
}

__aicore__ inline bool IsScalarQuantMode(QuantMode_t quantPre)
{
    return (
        quantPre == QuantMode_t::DEQF16 || quantPre == QuantMode_t::QF162B8_PRE || quantPre == QuantMode_t::REQ8 ||
        quantPre == QuantMode_t::QF162S4_PRE || quantPre == QuantMode_t::REQ4 || quantPre == QuantMode_t::DEQS16 ||
        quantPre == QuantMode_t::QF162S16_PRE);
}

__aicore__ inline void CopyDeqTensorToFbuf(
    __cbuf__ uint64_t* cbufWorkspace, const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex)
{
    constexpr uint32_t deqTensorAddrAlignValue = 128;
    uint16_t deqDataSize = DivCeil(calNSize * sizeof(uint64_t), deqTensorAddrAlignValue) * deqTensorAddrAlignValue;
    __fbuf__ uint64_t* deqTensorTempBuf =
        AscendCUtils::GetTemporaryFbBufferAddr<uint64_t>(0, deqDataSize / sizeof(uint64_t));
    uint32_t deqValueOffset = nIterIndex * fixpipeTiling.nSize;

    // L1 -> FB
    constexpr uint16_t fbufBurstLenUnit = 64;
    uint16_t fbufBurstLen = deqDataSize / fbufBurstLenUnit; // copy from cbuf to fbuf, burst_len unit is 64Bytes
    copy_cbuf_to_fbuf(deqTensorTempBuf, cbufWorkspace + deqValueOffset, 1, fbufBurstLen, 0, 0);
    // FPC of fixpipe buffer for Quant_PRE is FPC[15:8], unit is 128Bytes
    uint64_t deqTensorAddr = ((uint64_t)deqTensorTempBuf >> static_cast<uint64_t>(7)) << 8;
    set_fpc(deqTensorAddr);
    AscendCUtils::FreeTemporaryFbBuffer<uint64_t>(deqTensorTempBuf);
}

template <const FixpipeConfig& config>
__aicore__ inline void SetLoop3Para(const FixpipeParamsL310<config.format>& intriParams)
{
    // the loop3DstStride range has increased.
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        ASCENDC_ASSERT((intriParams.params.ndNum > 0), { KERNEL_LOG(KERNEL_ERROR, "ndNum must be larger than 0"); });
        // Loop3_dst_stride in uint of element
        uint64_t loop3Para = static_cast<uint64_t>(intriParams.params.dstNdStride) << 32; // LOOP3_PARA[63:32]
        // original src_nd_stride in uint of fractal_size(1024B = 16 * 16 * sizeof(SrcT))
        // src_nd_stride in unit of C0_SIZE, Loop3_src_stride
        loop3Para |= static_cast<uint64_t>(intriParams.params.srcNdStride) << 16; // LOOP3_PARA[31:16]
        loop3Para |= static_cast<uint64_t>(intriParams.params.ndNum);             // LOOP3_PARA[15:0]
        set_loop3_para(loop3Para);
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        ASCENDC_ASSERT((intriParams.params.dnNum > 0), { KERNEL_LOG(KERNEL_ERROR, "dnNum must be larger than 0"); });
        // Loop3_dst_stride in uint of element
        uint64_t loop3Para = static_cast<uint64_t>(intriParams.params.dstDnMatrixStride) << 32; // LOOP3_PARA[63:32]
        // src_nd_stride in unit of C0_SIZE, Loop3_src_stride
        loop3Para |= static_cast<uint64_t>(intriParams.params.srcNzMatrixStride) << 16; // LOOP3_PARA[31:16]
        loop3Para |= static_cast<uint64_t>(intriParams.params.dnNum);                   // LOOP3_PARA[15:0]
        set_loop3_para(loop3Para);
    }
}

// contains loop info and cal n size for each loop
// move data L0C->L1
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToL1(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL310<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    ASCENDC_DEBUG_ASSERT(
        (!(intriParams.isChannelSplit)),
        KERNEL_LOG_INTERNAL(
            KERNEL_ERROR, "Failed to check isChannelSplit in Fixpipe, when src position is "
                          "L0C Buffer(CO1) and dst position is L1 Buffer(C1), isChannelSplit must be set as false \n"));

    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    // Loop1_src_stride in unit of C0_size
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    // If NZ2ND/NZ2DN is enabled, it is the dst_D value in unit of element. Loop2_dst_stride
    // If NZ2ND/NZ2DN is disabled, it is in unit of element, Loop1_dst_stride
    bool nz2ndEn = false;
    bool nz2dnEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize * intriParams.dstStride;
        nz2dnEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    // LOC -> L1 only n direction need fixpipeTiling, m no need fixpipeTiling
    return copy_matrix_cc_to_cbuf(
        (__cbuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag,
        static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn), false, nz2ndEn,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false, false, false, nz2dnEn);
}

// contains loop info and cal n size for each loop
// move data L0C->UB
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToUB(
    __ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL310<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    static_assert(
        SupportType<
            Tuple<SrcT, DstT>, Tuple<float, half>, Tuple<float, int8_t>, Tuple<float, uint8_t>, Tuple<float, float>,
            Tuple<int32_t, half>, Tuple<int32_t, int8_t>, Tuple<int32_t, uint8_t>, Tuple<int32_t, int32_t>>(),
        "Failed to check dtype in Fixpipe");
    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    // Loop1_src_stride in unit of C0_size
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    // If NZ2ND/NZ2DN is enabled, it is the dst_D value in unit of element. Loop2_dst_stride
    // If NZ2ND/NZ2DN is disabled, it is in unit of element, Loop1_dst_stride
    bool nz2ndEn = false;
    bool nz2dnEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize * intriParams.dstStride;
        nz2dnEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    // LOC -> UB only n direction need fixpipeTiling, m no need fixpipeTiling
    return copy_matrix_cc_to_ub(
        (__ubuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag,
        static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn),
        intriParams.isChannelSplit, nz2ndEn, static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0,
        false, false, false, false, false, nz2dnEn);
}

template <const FixpipeConfig& config>
__aicore__ inline uint64_t GetGMLen(
    const FixpipeParamsL310<config.format>& intriParams, const uint16_t calNSize, const uint16_t dstEleSize)
{
    constexpr uint16_t fracNsize = 16;
    uint64_t cburstNum = calNSize / fracNsize;
    uint64_t gmLen = (cburstNum - 1) * intriParams.dstStride * dstEleSize + intriParams.mSize * fracNsize * dstEleSize;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        // dstStride is dst_D Loop2_dst_stride
        gmLen = (static_cast<uint64_t>(intriParams.params.ndNum) - 1) * dstEleSize * intriParams.params.dstNdStride +
                (intriParams.mSize - 1) * intriParams.dstStride * dstEleSize + cburstNum * fracNsize * dstEleSize;
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        gmLen =
            (static_cast<uint64_t>(intriParams.params.dnNum) - 1) * dstEleSize * intriParams.params.dstDnMatrixStride +
            (intriParams.nSize - 1) * intriParams.dstStride * dstEleSize +
            intriParams.mSize / fracNsize * fracNsize * dstEleSize;
    }
    return gmLen;
}

// contains loop info and cal n size for each loop
// move data L0C->GM
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToOut(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL310<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    if (intriParams.isChannelSplit) {
        ASCENDC_DEBUG_ASSERT(
            (IsSameType<SrcT, float>::value && IsSameType<DstT, float>::value),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR, "Failed to check "
                              "isChannelSplit value in Fixpipe, isChannelSplit can be set true only when src and dst "
                              "are both float \n"));
        ASCENDC_DEBUG_ASSERT(
            (config.format == CO2Layout::NZ),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR, "Failed to check format value in Fixpipe, "
                              "when isChannelSplit is set true, format must be set as NZ \n"));
    }
    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    bool nz2ndEn = false;
    bool nz2dnEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize * intriParams.dstStride;
        nz2dnEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    if constexpr (g_gm_overflow_check) {
        bool isSrc = false;
        uint16_t dstEleSize = sizeof(DstT);
        uint64_t gmLen = GetGMLen<config>(intriParams, calNSize, dstEleSize);
        AscendCUtils::CheckGmMemOverflow((__gm__ DstT*)(dst + dstOffset), isSrc, gmLen);
    }

    // LOC -> GM only n direction need fixpipeTiling, m no need fixpipeTiling
    if constexpr (!(IsSameType<SrcT, float>::value || IsSameType<DstT, float>::value)) {
        return copy_matrix_cc_to_gm(
            (__gm__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
            intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag,
            static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn),
            intriParams.isChannelSplit, nz2ndEn, static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0,
            false, false, false, false, false, nz2dnEn);
    }
    ASCENDC_DEBUG_ASSERT(
        !(IsSameType<SrcT, float>::value || IsSameType<DstT, float>::value),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "unsupported float L0c 2 GM "));
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1ImplN(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL310<config.format>& intriParams, const FixpipeTiling& fixpipeTiling, uint16_t calNSize,
    uint16_t nIterIndex)
{
    // mov deq tensor from L1 to FB
    CopyDeqTensorToFbuf(cbufWorkspace, fixpipeTiling, calNSize, nIterIndex);
    PipeBarrier<PIPE_FIX>();
    // L0C->L1
    FixpipeL0cToL1<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, calNSize, nIterIndex);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImplN(
    __ubuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL310<config.format>& intriParams, const FixpipeTiling& fixpipeTiling, uint16_t calNSize,
    uint16_t nIterIndex)
{
    // mov deq tensor from L1 to FB
    CopyDeqTensorToFbuf(cbufWorkspace, fixpipeTiling, calNSize, nIterIndex);
    PipeBarrier<PIPE_FIX>();
    // L0C->UB
    FixpipeL0cToUB<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, calNSize, nIterIndex);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImplN(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL310<config.format>& intriParams, const FixpipeTiling& fixpipeTiling, uint16_t calNSize,
    uint16_t nIterIndex)
{
    // mov deq tensor from L1 to FB
    CopyDeqTensorToFbuf(cbufWorkspace, fixpipeTiling, calNSize, nIterIndex);
    PipeBarrier<PIPE_FIX>();
    // L0C->GM
    FixpipeL0cToOut<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, calNSize, nIterIndex);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL310<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to l1
    */
    if (IsScalarQuantMode(intriParams.quantPre)) {
        // deq factor of uint64 bits describe: bits[31:13] is deq value of fp32,
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    FixpipeTiling fixpipeTiling;
    // LOC -> L1
    FixpipeL0cToL1<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL310<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for vector quant mode:
    1. generate fixpipeTiling
    2. copy deq tensor from l1 to fb1 (l1 -> fb1)
    3. code gen: move data from l0c to l1
    */
    if (IsVectorQuantMode(intriParams.quantPre)) {
        FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        // deal with the tail, it also need copy deq/relu tensor from L1 to fb1
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL310<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to ub
    */
    if (IsScalarQuantMode(intriParams.quantPre)) {
        // deq factor of uint64 bits describe: bits[31:13] is deq value of fp32,
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    FixpipeTiling fixpipeTiling;
    // LOC -> UB
    FixpipeL0cToUB<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL310<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for vector quant mode:
    1. generate fixpipeTiling
    2. copy deq tensor from l1 to fb1 (l1 -> fb1)
    3. code gen: move data from l0c to ub
    */
    if (IsVectorQuantMode(intriParams.quantPre)) {
        FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2UBImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        // deal with the tail, it also need copy deq/relu tensor from L1 to fb1
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2UBImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(KERNEL_ERROR, "Fixpipe with cbufWorkspace only supports vector quant mode");
    });
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL310<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);
    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to gm
    */
    if (IsScalarQuantMode(intriParams.quantPre)) {
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    // LOC -> GM
    FixpipeTiling fixpipeTiling;
    FixpipeL0cToOut<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL310<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);
    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for vector quant mode:
    1. generate fixpipeTiling
    2. copy deq tensor from l1 to fb1 (l1 -> fb1)
    3. code gen: move data from l0c to gm
    */
    FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
    if (IsVectorQuantMode(intriParams.quantPre)) {
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2GMImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        // deal with the tail, it also need copy deq/relu tensor from L1 to fb1
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2GMImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
}

template <CO2Layout format>
__aicore__ inline void TransFixpipeParamsV220ToFixpipeParamsL310(
    const FixpipeParamsV220& intriParams, FixpipeParamsL310<format>& dstParams)
{
    dstParams.nSize = intriParams.nSize;
    dstParams.mSize = intriParams.mSize;
    dstParams.srcStride = intriParams.srcStride;
    dstParams.dstStride = intriParams.dstStride;
    dstParams.deqScalar = intriParams.deqScalar;
    dstParams.quantPre = intriParams.quantPre;
    dstParams.reluEn = intriParams.reluEn;
    dstParams.unitFlag = intriParams.unitFlag;
    dstParams.isChannelSplit = intriParams.isChannelSplit;
    if constexpr (format == CO2Layout::ROW_MAJOR) {
        dstParams.params.ndNum = intriParams.ndNum;
        dstParams.params.dstNdStride = intriParams.dstNdStride;
        dstParams.params.srcNdStride = intriParams.srcNdStride;
    } else if constexpr (format == CO2Layout::COLUMN_MAJOR) {
        dstParams.params.ndNum = intriParams.ndNum;
        dstParams.params.dstNdStride = intriParams.srcNdStride;
        dstParams.params.srcNdStride = intriParams.srcNdStride;
    }
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL310<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL310(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL310<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL310(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(__gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL310<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL310(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL310<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL310(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL310<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL310(intriParams, params);
    FixpipeL0C2UBImpl<DstT, SrcT, config>(dst, src, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL310<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL310(intriParams, params);
    FixpipeL0C2UBImpl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}

} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_FIXPIPE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_IMPL_H__
#endif
```

### 3.3 3113（dav_l311/kernel_operator_fixpipe_impl.h，679 行）

函数修改状态：

| 函数/结构 | 行号（适配后） | 状态 |
|---|---|---|
| SetFixPipeClipReluImpl / SetFixPipeAddrImpl / SetFixPipeConfigImpl ×2 / SetFixpipeNz2ndFlagImpl / SetFixpipePreQuantFlagImpl | L29-78 | 维持 |
| TransformParamsL311 / FixpipeParamsL311 | L83-131 | 维持 |
| FixpipeTiling / GenFixpipeTiling / IsVectorQuantMode / IsScalarQuantMode | L133-176 | 维持 |
| CopyDeqTensorToFbuf（FB burst 64B） | L178-195 | 维持 |
| SetLoop3Para | L197-219 | 维持 |
| FixpipeL0cToL1（→ 24 参 copy_matrix_cc_to_cbuf，含 nz2dnEn） | L221-257 | 维持 |
| **FixpipeL0cToUB（→ 24 参 copy_matrix_cc_to_ub；dtype 矩阵含 bf16）** | **L259-296** | **新增** |
| GetGMLen | L298-316 | 维持 |
| FixpipeL0cToOut（→ 24 参 copy_matrix_cc_to_gm） | L318-371 | 维持 |
| FixpipeL0C2L1ImplN | L373-384 | 维持 |
| **FixpipeL0C2UBImplN** | **L386-397** | **新增** |
| FixpipeL0C2GMImplN | L399-410 | 维持 |
| FixpipeL0C2L1Impl 3 参 / 4 参 | L412-438 / L440-473 | 维持 |
| **FixpipeL0C2UBImpl 3 参** | **L475-501** | **新增** |
| **FixpipeL0C2UBImpl 4 参**（非向量量化新增断言） | **L503-539** | **新增** |
| FixpipeL0C2GMImpl 3 参 / 4 参 | L541-564 / L566-597 | 维持 |
| TransFixpipeParamsV220ToFixpipeParamsL311 | L599-621 | 维持 |
| V220 适配器：L1 3/4 参、GM 3/4 参 | L623-638 / L640-655 | 维持 |
| **V220 适配器：UB 3 参 / 4 参**（替换原 stub） | **L657-663 / L665-672** | **新增** |

完整代码：

```cpp
/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_operator_fixpipe_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_l311/kernel_operator_fixpipe_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_FIXPIPE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_FIXPIPE_IMPL_H

#include "kernel_operator_set_spr_impl.h"
#include "../../../include/basic_api/kernel_struct_fixpipe.h"

namespace AscendC {

__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}

template <typename T>
__aicore__ inline void SetFixPipeAddrImpl(const LocalTensor<T>& eleWiseTensor, uint16_t c0ChStride)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeAddr");
}

/* **************************************************************************************************
 * SPR                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void SetFixPipeConfigImpl(
    const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false)
{
    uint64_t config = 0;
    config = config | ((uint64_t)reluPre.GetPhyAddr() >> 6);         // align with 64bit, FPC[7:0], ReluPreAddr
    config = config | (((uint64_t)quantPre.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    config = config | (static_cast<uint64_t>(isUnitFlag) << 63);     // FPC[63], UnitFlag.
    set_fpc(config);
}

template <typename T, bool setRelu = false>
__aicore__ inline void SetFixPipeConfigImpl(const LocalTensor<T>& preTensor, bool isUnitFlag = false)
{
    uint64_t config = 0;
    if constexpr (setRelu) {
        config = config | ((uint64_t)preTensor.GetPhyAddr() >> 6); // align with 64bit, FPC[7:0], ReluPreAddr.
    } else {
        config = config | (((uint64_t)preTensor.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    }
    config = config | (static_cast<uint64_t>(isUnitFlag) << 63); // FPC[63], UnitFlag.
    set_fpc(config);
}

__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    // ND_PARA[63:32], dst nd stride in unit of element
    // ND_PARA[31:16], src nd stride in uint of C0_SIZE
    // ND_PARA[15:0], nd number.
    uint64_t config = (static_cast<uint64_t>(dstNdStride) << 32) | (static_cast<uint64_t>(srcNdStride) << 16) |
                      (static_cast<uint64_t>(ndNum));
    set_loop3_para(config);
}

__aicore__ inline void SetFixpipePreQuantFlagImpl(uint64_t config) { set_quant_pre(config); }

/* **************************************************************************************************
 * Fixpipe                                             *
 * ************************************************************************************************* */
template <CO2Layout format>
struct TransformParamsL311 {};

template <>
struct TransformParamsL311<CO2Layout::NZ> {
    __aicore__ inline TransformParamsL311(){};
    using PARAMS = uint8_t;
};

template <>
struct TransformParamsL311<CO2Layout::ROW_MAJOR> {
    __aicore__ inline TransformParamsL311(){};
    using PARAMS = Nz2NdParams;
};

template <>
struct TransformParamsL311<CO2Layout::COLUMN_MAJOR> {
    __aicore__ inline TransformParamsL311(){};
    using PARAMS = uint8_t;
};

template <CO2Layout format = CO2Layout::ROW_MAJOR>
struct FixpipeParamsL311 {
    __aicore__ FixpipeParamsL311() {}

    __aicore__ FixpipeParamsL311(
        const uint16_t nSizeIn_, const uint16_t mSizeIn_, const uint16_t srcStrideIn_, const uint32_t dstStrideIn_)
    {
        nSize = nSizeIn_;
        mSize = mSizeIn_;
        srcStride = srcStrideIn_;
        dstStride = dstStrideIn_;
    }

    uint16_t nSize = 0;
    uint16_t mSize = 0; // M-DirectionSize
    uint16_t srcStride = 0;
    uint32_t dstStride = 0;
    // Params: used for Quant
    uint64_t deqScalar;
    QuantMode_t quantPre = QuantMode_t::NoQuant;
    bool reluEn = false;
    uint8_t unitFlag = 0;
    // extend param
    bool subBlockId = false;
    uint8_t dualDstCtl = 0;
    typename TransformParamsL311<format>::PARAMS params;
    bool isChannelSplit = false;
};

// tiling params
struct FixpipeTiling {
    uint16_t nIterNum = 0;
    uint16_t nSize = 0;
    bool isDb = false;
    uint16_t tailNSize = 0;
};

// fixpipe tiling calculating
__aicore__ inline FixpipeTiling GenFixpipeTiling(uint16_t n)
{
    FixpipeTiling tiling;
    // deqTensor/reluTensor in FB valid num is 1024
    uint16_t maxDeqNums = 1024;
    if (n <= maxDeqNums) {
        tiling.nIterNum = 1;
        tiling.nSize = n;
        tiling.isDb = false;
        tiling.tailNSize = 0;
    } else {
        tiling.isDb = true;
        uint16_t dbMaxDeqNums = maxDeqNums / 2;
        tiling.nIterNum = n / dbMaxDeqNums;
        tiling.nSize = dbMaxDeqNums;
        tiling.tailNSize = n % dbMaxDeqNums;
    }
    return tiling;
}

__aicore__ inline bool IsVectorQuantMode(QuantMode_t quantPre)
{
    return (
        quantPre == QuantMode_t::VDEQF16 || quantPre == QuantMode_t::VQF162B8_PRE || quantPre == QuantMode_t::VREQ8 ||
        quantPre == QuantMode_t::VQF162S4_PRE || quantPre == QuantMode_t::VREQ4 || quantPre == QuantMode_t::VDEQS16 ||
        quantPre == QuantMode_t::VQF162S16_PRE);
}

__aicore__ inline bool IsScalarQuantMode(QuantMode_t quantPre)
{
    return (
        quantPre == QuantMode_t::DEQF16 || quantPre == QuantMode_t::QF162B8_PRE || quantPre == QuantMode_t::REQ8 ||
        quantPre == QuantMode_t::QF162S4_PRE || quantPre == QuantMode_t::REQ4 || quantPre == QuantMode_t::DEQS16 ||
        quantPre == QuantMode_t::QF162S16_PRE);
}

__aicore__ inline void CopyDeqTensorToFbuf(
    __cbuf__ uint64_t* cbufWorkspace, const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex)
{
    constexpr uint32_t deqTensorAddrAlignValue = 128;
    uint16_t deqDataSize = DivCeil(calNSize * sizeof(uint64_t), deqTensorAddrAlignValue) * deqTensorAddrAlignValue;
    __fbuf__ uint64_t* deqTensorTempBuf =
        AscendCUtils::GetTemporaryFbBufferAddr<uint64_t>(0, deqDataSize / sizeof(uint64_t));
    uint32_t deqValueOffset = nIterIndex * fixpipeTiling.nSize;

    // L1 -> FB
    constexpr uint16_t fbufBurstLenUnit = 64;
    uint16_t fbufBurstLen = deqDataSize / fbufBurstLenUnit; // copy from cbuf to fbuf, burst_len unit is 64Bytes
    copy_cbuf_to_fbuf(deqTensorTempBuf, cbufWorkspace + deqValueOffset, 1, fbufBurstLen, 0, 0);
    // FPC of fixpipe buffer for Quant_PRE is FPC[15:8], unit is 128Bytes
    uint64_t deqTensorAddr = ((uint64_t)deqTensorTempBuf >> static_cast<uint64_t>(7)) << 8;
    set_fpc(deqTensorAddr);
    AscendCUtils::FreeTemporaryFbBuffer<uint64_t>(deqTensorTempBuf);
}

template <const FixpipeConfig& config>
__aicore__ inline void SetLoop3Para(const FixpipeParamsL311<config.format>& intriParams)
{
    // the loop3DstStride range has increased.
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        ASCENDC_ASSERT((intriParams.params.ndNum > 0), { KERNEL_LOG(KERNEL_ERROR, "ndNum must be larger than 0"); });
        // Loop3_dst_stride in uint of element
        uint64_t loop3Para = static_cast<uint64_t>(intriParams.params.dstNdStride) << 32; // LOOP3_PARA[63:32]
        // original src_nd_stride in uint of fractal_size(1024B = 16 * 16 * sizeof(SrcT))
        // src_nd_stride in unit of C0_SIZE, Loop3_src_stride
        loop3Para |= static_cast<uint64_t>(intriParams.params.srcNdStride) << 16; // LOOP3_PARA[31:16]
        loop3Para |= static_cast<uint64_t>(intriParams.params.ndNum);             // LOOP3_PARA[15:0]
        set_loop3_para(loop3Para);
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        ASCENDC_ASSERT((intriParams.params.dnNum > 0), { KERNEL_LOG(KERNEL_ERROR, "dnNum must be larger than 0"); });
        // Loop3_dst_stride in uint of element
        uint64_t loop3Para = static_cast<uint64_t>(intriParams.params.dstDnMatrixStride) << 32; // LOOP3_PARA[63:32]
        // src_nd_stride in unit of C0_SIZE, Loop3_src_stride
        loop3Para |= static_cast<uint64_t>(intriParams.params.srcNzMatrixStride) << 16; // LOOP3_PARA[31:16]
        loop3Para |= static_cast<uint64_t>(intriParams.params.dnNum);                   // LOOP3_PARA[15:0]
        set_loop3_para(loop3Para);
    }
}

// contains loop info and cal n size for each loop
// move data L0C->L1
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToL1(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL311<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    ASCENDC_DEBUG_ASSERT(
        (!(intriParams.isChannelSplit)),
        KERNEL_LOG_INTERNAL(
            KERNEL_ERROR, "Failed to check isChannelSplit in Fixpipe, when src position is "
                          "L0C Buffer(CO1) and dst position is L1 Buffer(C1), isChannelSplit must be set as false \n"));

    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    // Loop1_src_stride in unit of C0_size
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    // If NZ2ND/NZ2DN is enabled, it is the dst_D value in unit of element. Loop2_dst_stride
    // If NZ2ND/NZ2DN is disabled, it is in unit of element, Loop1_dst_stride
    bool nz2ndEn = false;
    bool nz2dnEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize * intriParams.dstStride;
        nz2dnEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    // LOC -> L1 only n direction need fixpipeTiling, m no need fixpipeTiling
    return copy_matrix_cc_to_cbuf(
        (__cbuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag,
        static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn), false, nz2ndEn,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false, false, false, nz2dnEn);
}

// contains loop info and cal n size for each loop
// move data L0C->UB
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToUB(
    __ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL311<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    static_assert(
        SupportType<
            Tuple<SrcT, DstT>, Tuple<float, bfloat16_t>, Tuple<float, half>, Tuple<float, int8_t>,
            Tuple<float, uint8_t>, Tuple<float, float>, Tuple<int32_t, bfloat16_t>, Tuple<int32_t, half>,
            Tuple<int32_t, int8_t>, Tuple<int32_t, uint8_t>, Tuple<int32_t, int32_t>>(),
        "Failed to check dtype in Fixpipe");
    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    // Loop1_src_stride in unit of C0_size
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    // If NZ2ND/NZ2DN is enabled, it is the dst_D value in unit of element. Loop2_dst_stride
    // If NZ2ND/NZ2DN is disabled, it is in unit of element, Loop1_dst_stride
    bool nz2ndEn = false;
    bool nz2dnEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize * intriParams.dstStride;
        nz2dnEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    // LOC -> UB only n direction need fixpipeTiling, m no need fixpipeTiling
    return copy_matrix_cc_to_ub(
        (__ubuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag,
        static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn),
        intriParams.isChannelSplit, nz2ndEn, static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0,
        false, false, false, false, false, nz2dnEn);
}

template <const FixpipeConfig& config>
__aicore__ inline uint64_t GetGMLen(
    const FixpipeParamsL311<config.format>& intriParams, const uint16_t calNSize, const uint16_t dstEleSize)
{
    constexpr uint16_t fracNsize = 16;
    uint64_t cburstNum = calNSize / fracNsize;
    uint64_t gmLen = (cburstNum - 1) * intriParams.dstStride * dstEleSize + intriParams.mSize * fracNsize * dstEleSize;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        // dstStride is dst_D Loop2_dst_stride
        gmLen = (static_cast<uint64_t>(intriParams.params.ndNum) - 1) * dstEleSize * intriParams.params.dstNdStride +
                (intriParams.mSize - 1) * intriParams.dstStride * dstEleSize + cburstNum * fracNsize * dstEleSize;
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        gmLen =
            (static_cast<uint64_t>(intriParams.params.dnNum) - 1) * dstEleSize * intriParams.params.dstDnMatrixStride +
            (intriParams.nSize - 1) * intriParams.dstStride * dstEleSize +
            intriParams.mSize / fracNsize * fracNsize * dstEleSize;
    }
    return gmLen;
}

// contains loop info and cal n size for each loop
// move data L0C->GM
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToOut(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL311<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    if (intriParams.isChannelSplit) {
        ASCENDC_DEBUG_ASSERT(
            (IsSameType<SrcT, float>::value && IsSameType<DstT, float>::value),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR, "Failed to check "
                              "isChannelSplit value in Fixpipe, isChannelSplit can be set true only when src and dst "
                              "are both float \n"));
        ASCENDC_DEBUG_ASSERT(
            (config.format == CO2Layout::NZ),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR, "Failed to check format value in Fixpipe, "
                              "when isChannelSplit is set true, format must be set as NZ \n"));
    }
    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    bool nz2ndEn = false;
    bool nz2dnEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize * intriParams.dstStride;
        nz2dnEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    if constexpr (g_gm_overflow_check) {
        bool isSrc = false;
        uint16_t dstEleSize = sizeof(DstT);
        uint64_t gmLen = GetGMLen<config>(intriParams, calNSize, dstEleSize);
        AscendCUtils::CheckGmMemOverflow((__gm__ DstT*)(dst + dstOffset), isSrc, gmLen);
    }

    // LOC -> GM only n direction need fixpipeTiling, m no need fixpipeTiling
    if constexpr (!(IsSameType<SrcT, float>::value || IsSameType<DstT, float>::value)) {
        return copy_matrix_cc_to_gm(
            (__gm__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
            intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag,
            static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn),
            intriParams.isChannelSplit, nz2ndEn, static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0,
            false, false, false, false, false, nz2dnEn);
    }
    ASCENDC_DEBUG_ASSERT(
        !(IsSameType<SrcT, float>::value || IsSameType<DstT, float>::value),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "unsupported float L0c 2 GM "));
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1ImplN(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL311<config.format>& intriParams, const FixpipeTiling& fixpipeTiling, uint16_t calNSize,
    uint16_t nIterIndex)
{
    // mov deq tensor from L1 to FB
    CopyDeqTensorToFbuf(cbufWorkspace, fixpipeTiling, calNSize, nIterIndex);
    PipeBarrier<PIPE_FIX>();
    // L0C->L1
    FixpipeL0cToL1<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, calNSize, nIterIndex);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImplN(
    __ubuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL311<config.format>& intriParams, const FixpipeTiling& fixpipeTiling, uint16_t calNSize,
    uint16_t nIterIndex)
{
    // mov deq tensor from L1 to FB
    CopyDeqTensorToFbuf(cbufWorkspace, fixpipeTiling, calNSize, nIterIndex);
    PipeBarrier<PIPE_FIX>();
    // L0C->UB
    FixpipeL0cToUB<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, calNSize, nIterIndex);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImplN(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL311<config.format>& intriParams, const FixpipeTiling& fixpipeTiling, uint16_t calNSize,
    uint16_t nIterIndex)
{
    // mov deq tensor from L1 to FB
    CopyDeqTensorToFbuf(cbufWorkspace, fixpipeTiling, calNSize, nIterIndex);
    PipeBarrier<PIPE_FIX>();
    // L0C->GM
    FixpipeL0cToOut<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, calNSize, nIterIndex);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL311<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to l1
    */
    if (IsScalarQuantMode(intriParams.quantPre)) {
        // deq factor of uint64 bits describe: bits[31:13] is deq value of fp32,
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    FixpipeTiling fixpipeTiling;
    // LOC -> L1
    FixpipeL0cToL1<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL311<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for vector quant mode:
    1. generate fixpipeTiling
    2. copy deq tensor from l1 to fb1 (l1 -> fb1)
    3. code gen: move data from l0c to l1
    */
    if (IsVectorQuantMode(intriParams.quantPre)) {
        FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        // deal with the tail, it also need copy deq/relu tensor from L1 to fb1
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL311<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to ub
    */
    if (IsScalarQuantMode(intriParams.quantPre)) {
        // deq factor of uint64 bits describe: bits[31:13] is deq value of fp32,
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    FixpipeTiling fixpipeTiling;
    // LOC -> UB
    FixpipeL0cToUB<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL311<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);

    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        // Loop0_dst_stride in uint of CO_SIZE
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for vector quant mode:
    1. generate fixpipeTiling
    2. copy deq tensor from l1 to fb1 (l1 -> fb1)
    3. code gen: move data from l0c to ub
    */
    if (IsVectorQuantMode(intriParams.quantPre)) {
        FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2UBImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        // deal with the tail, it also need copy deq/relu tensor from L1 to fb1
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2UBImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(KERNEL_ERROR, "Fixpipe with cbufWorkspace only supports vector quant mode");
    });
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL311<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);
    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to gm
    */
    if (IsScalarQuantMode(intriParams.quantPre)) {
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    // LOC -> GM
    FixpipeTiling fixpipeTiling;
    FixpipeL0cToOut<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL311<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);
    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    /*
    make code for vector quant mode:
    1. generate fixpipeTiling
    2. copy deq tensor from l1 to fb1 (l1 -> fb1)
    3. code gen: move data from l0c to gm
    */
    FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
    if (IsVectorQuantMode(intriParams.quantPre)) {
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2GMImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        // deal with the tail, it also need copy deq/relu tensor from L1 to fb1
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2GMImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
}

template <CO2Layout format>
__aicore__ inline void TransFixpipeParamsV220ToFixpipeParamsL311(
    const FixpipeParamsV220& intriParams, FixpipeParamsL311<format>& dstParams)
{
    dstParams.nSize = intriParams.nSize;
    dstParams.mSize = intriParams.mSize;
    dstParams.srcStride = intriParams.srcStride;
    dstParams.dstStride = intriParams.dstStride;
    dstParams.deqScalar = intriParams.deqScalar;
    dstParams.quantPre = intriParams.quantPre;
    dstParams.reluEn = intriParams.reluEn;
    dstParams.unitFlag = intriParams.unitFlag;
    dstParams.isChannelSplit = intriParams.isChannelSplit;
    if constexpr (format == CO2Layout::ROW_MAJOR) {
        dstParams.params.ndNum = intriParams.ndNum;
        dstParams.params.dstNdStride = intriParams.dstNdStride;
        dstParams.params.srcNdStride = intriParams.srcNdStride;
    } else if constexpr (format == CO2Layout::COLUMN_MAJOR) {
        dstParams.params.ndNum = intriParams.ndNum;
        dstParams.params.dstNdStride = intriParams.srcNdStride;
        dstParams.params.srcNdStride = intriParams.srcNdStride;
    }
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(__gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2UBImpl<DstT, SrcT, config>(dst, src, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2UBImpl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}

} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_FIXPIPE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_IMPL_H__
#endif
```

### 3.4 5101（dav_5161/kernel_operator_fixpipe_impl.h，106 行）

函数修改状态：

| 函数 | 行号（适配后） | 状态 |
|---|---|---|
| SetFixPipeClipReluImpl（含 5101 条件分支） | L29-37 | 维持 |
| SetFixPipeAddrImpl | L39-45 | 维持 |
| SetFixPipeConfigImpl ×2 | L47-69 | 维持 |
| SetFixpipeNz2ndFlagImpl | L71-77 | 维持 |
| SetFixpipePreQuantFlagImpl | L79 | 维持 |
| **FixpipeL0C2UBImpl 3 参（显式桩）** | **L81-88** | **新增（用户）** |
| **FixpipeL0C2UBImpl 4 参（显式桩）** | **L90-99** | **新增（用户）** |

5101 无 L0C，不引入 L0C→L1/GM/UB 实现；`FixpipeL0C2L1Impl` / `FixpipeL0C2GMImpl` 维持"不存在"状态（V220 宏控不含 5101，声明不可见，包装不会实例化，新增桩为防御性兜底）。

完整代码：

```cpp
/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_operator_fixpipe_impl.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_5161/kernel_operator_fixpipe_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_FIXPIPE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_FIXPIPE_IMPL_H
#include "kernel_operator_set_spr_impl.h"
#include "../../../include/basic_api/kernel_struct_fixpipe.h"

namespace AscendC {
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101)
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config)
{
    (void)(config);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeClipRelu");
}
#else
__aicore__ inline void SetFixPipeClipReluImpl(uint64_t config) { set_m_clip_relu(config); }
#endif

template <typename T>
__aicore__ inline void SetFixPipeAddrImpl(const LocalTensor<T>& eleWiseTensor, uint16_t c0ChStride)
{
    (void)(eleWiseTensor);
    (void)(c0ChStride);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixPipeAddr");
}

template <typename T>
__aicore__ inline void SetFixPipeConfigImpl(
    const LocalTensor<T>& reluPre, const LocalTensor<T>& quantPre, bool isUnitFlag = false)
{
    ASCENDC_REPORT_NOT_SUPPORT(!isUnitFlag, "SetFixPipeConfig isUnitFlag");
    uint64_t config = 0;
    config = config | ((uint64_t)reluPre.GetPhyAddr() >> 6);         // align with 64bit, FPC[7:0], ReluPreAddr
    config = config | (((uint64_t)quantPre.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    set_m_fpc(config);
}

template <typename T, bool setRelu = false>
__aicore__ inline void SetFixPipeConfigImpl(const LocalTensor<T>& preTensor, bool isUnitFlag = false)
{
    ASCENDC_REPORT_NOT_SUPPORT(!isUnitFlag, "SetFixPipeConfig isUnitFlag");
    uint64_t config = 0;
    if constexpr (setRelu) {
        config = config | ((uint64_t)preTensor.GetPhyAddr() >> 6); // align with 64bit, FPC[7:0], ReluPreAddr.
    } else {
        config = config | (((uint64_t)preTensor.GetPhyAddr() >> 7) << 8); // align with 128bit, FPC[15:8], QuantPreAddr.
    }
    set_m_fpc(config);
}

__aicore__ inline void SetFixpipeNz2ndFlagImpl(uint16_t ndNum, uint16_t srcNdStride, uint32_t dstNdStride)
{
    (void)(ndNum);
    (void)(srcNdStride);
    (void)(dstNdStride);
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetFixpipeNz2ndFlag");
}

__aicore__ inline void SetFixpipePreQuantFlagImpl(uint64_t config) { set_m_quant_pre(config); }

template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ T* dst, __cc__ U* src, const FixpipeParamsV220& intriParams)
{
    (void)(dst);
    (void)(src);
    (void)(intriParams);
    ASCENDC_REPORT_NOT_SUPPORT(false, "Fixpipe L0C to UB");
}

template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    (void)(dst);
    (void)(src);
    (void)(cbufWorkspace);
    (void)(intriParams);
    ASCENDC_REPORT_NOT_SUPPORT(false, "Fixpipe L0C to UB");
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_FIXPIPE_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_FIXPIPE_IMPL_H__
#endif
```

### 3.5 维持版本（2201 / 3002 / 3102 / 3510 / 5102）

以下版本实现文件本次**未做任何修改**，适配后代码与适配前一致（各版本关键函数的完整代码已在分析报告 `Fixpipe(L0C-UB)-analysis.md` §4 中记录）：

| 版本 | 实现文件 | 说明 |
|---|---|---|
| 2201 | dav_c220（impl + v2_impl 双文件） | V220 宏控维持含 2201，未改动 |
| 3002 | dav_m300（impl + v2_impl 双文件） | M300 原生声明宿主，L1/UB/GM 完整，未改动 |
| 3102 | dav_m310（单文件） | M310 原生声明宿主，L1/UB/GM 完整，未改动 |
| 3510 | dav_3510（单文件，856 行） | 移植来源：`FixpipeL0cToUB`（L226-282，26 参，含双目的模式 dualDstCtl/subBlockId 检查）、`CheckFixpipeParams` dtype static_assert、V220 适配器（L800-832）均未改动；本次仅移植其 dtype 矩阵与函数结构 |
| 5102 | dav_m510（单文件） | V220 宏控维持含 5102，未改动 |

---

## 4. 适配实现说明

1. **调用链（适配后，3003/3103/3113 一致）**：
   `Fixpipe(LocalTensor dst, ...)`（V220 声明，intf.h L80-90）→ 共享包装（intf_impl.h L109-147，按 dst 物理位置分发）→ `FixpipeL0C2UBImpl<..., config>(V220参数)`（V220 适配器：Trans 转换 + 委托本地实现）→ 本地 `FixpipeL0C2UBImpl(FixpipeParamsLXXX)` → `FixpipeL0cToUB` → `copy_matrix_cc_to_ub`。
2. **`FixpipeL0cToUB` 仿各版本存量 `FixpipeL0cToL1`**：
   - 3003：22 参 `copy_matrix_cc_to_ub`（与 22 参 cbuf 同签名），双分支（ROW_MAJOR / else）；
   - 3103/3113：24 参 `copy_matrix_cc_to_ub`（与 24 参 cbuf 同签名，尾参 `nz2dnEn`），三分支（ROW_MAJOR / COLUMN_MAJOR / else）；
   - 与 L1 路径的差异仅两点：dst 指针为 `__ubuf__`；`isChannelSplit` 位传真实值 `intriParams.isChannelSplit`（三芯片 UB 支持该能力；L1 路径传 false 并有 DEBUG 断言，维持不变）。
3. **dtype 矩阵**（函数头部 static_assert，移植自 3510 `CheckFixpipeParams`）：
   - 3003/3103：`float→half/int8_t/uint8_t/float`、`int32_t→half/int8_t/uint8_t/int32_t`（**无 bf16**）；
   - 3113：额外含 `Tuple<float, bfloat16_t>`、`Tuple<int32_t, bfloat16_t>`（**bf16 仅 3113 支持**）；
   - 三者均**移除 fp8_e4m3fn_t / hifloat8_t**；错误信息 `"Failed to check dtype in Fixpipe"`。
4. **`FixpipeL0C2UBImpl` 3 参**（标量量化 + 普通模式）：`SetLoop3Para<config>` 无条件调用 + COLUMN_MAJOR 时 `set_channel_para(srcNzC0Stride << 48)` + 标量量化 `set_quant_pre(deqScalar)` + `PipeBarrier<PIPE_FIX>` + 默认 tiling 一次搬运 —— 与存量 L1 3 参完全同型。
5. **`FixpipeL0C2UBImpl` 4 参**（cbufWorkspace，向量量化）：`GenFixpipeTiling` 循环 + `FixpipeL0C2UBImplN`（CopyDeqTensorToFbuf + PipeBarrier + FixpipeL0cToUB）+ 尾块处理 + return；**非向量量化模式新增 `ASCENDC_ASSERT(false, "Fixpipe with cbufWorkspace only supports vector quant mode")`**（存量 L1 4 参为静默无操作，未改动；UB 侧按确认方案显式报错）。
6. **V220 适配器**：`FixpipeParamsLXXX<config.format> params; Trans...LXXX(intriParams, params); FixpipeL0C2UBImpl<...>(dst, src, [cbufWorkspace,] params);` —— 与存量 L1/GM 适配器同模式，替换原 UB stub。
7. **未移植项**（3510 专属，目标芯片不支持/不适用）：`isChannelSplit` 必须为 false 的 DEBUG 断言（三芯片 UB 支持真实值）；`dualDstCtl`/`subBlockId` 双目的模式及其三项断言（intrinsic 对应参数位固定传 `false`/`0`）。

---

## 5. 风险与遗留事项

1. **COLUMN_MAJOR（nz2dn）参数类型存量问题（未改动）**：三个 L 系列文件的 `TransformParamsL300/L310/L311<CO2Layout::COLUMN_MAJOR>::PARAMS = uint8_t`，而 SetLoop3Para/各 Impl/Trans 的 COLUMN_MAJOR 分支访问 `params.dnNum` / `params.srcNzC0Stride` 等成员 —— COLUMN_MAJOR 配置实例化时会编译失败（NZ / ROW_MAJOR 不受影响）。此为三文件一致的存量状态；本次新增的 UB 代码与存量 L1 代码行为完全一致。若需启用 nz2dn，需另行将 COLUMN_MAJOR 的 PARAMS 改回 `Nz2DnParams`（属独立改动，超出本次确认范围）。
2. **未做编译验证**：会话中 bash/glob/grep 工具损坏（命令不执行且无输出），本次仅完成逐文件、逐区域的读取校验（三个目标文件 + 声明层 + 共享层 + 5101 桩均已复核）。建议编译 3003/3103/3113 三个目标版本确认。
3. **Trans 函数 COLUMN_MAJOR 分支存量疑似 bug**（未改动）：`dstParams.params.dstNdStride = intriParams.srcNdStride;`（源为 srcNdStride，疑应为 dstNdStride），l300/l310/l311/3510 多处一致，详见分析报告 §5.3-6 / 待确认-7。
4. **行为差异提示**：UB 4 参非向量量化显式报错（新增断言），与存量 L1 4 参静默无操作不同 —— 经确认方案明确要求，属有意为之。
5. **待办**：旧分析文件 `M:\source\cann\Fixpipe(L0C-L1UB)-analysis.md` 因工具损坏无法删除，请手动删除（内容已由 `Fixpipe(L0C-UB)-analysis.md` 完整替代）。
