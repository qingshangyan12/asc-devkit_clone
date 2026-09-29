# 案例分析：Fixpipe(L0C→L1) 接口跨版本适配

## 接口概述

`Fixpipe` 用于 L0C→L1/UB/GM 的矩阵数据搬运（支持量化/ReLU/NZ2ND 等增强功能）。本次分析的 4 个接口均为 **L0C→L1（含运行期/编译期 UB 分流）** 变体，分布在 `kernel_operator_fixpipe_intf.h` 的 2 组 arch 宏控块中：

| 行号 | 变体 | 参数类型 | dst 类型 | arch 宏控 | 宏控条件 |
|------|------|---------|---------|-----------|---------|
| 79 | V1 V220 L0C→L1 | `FixpipeParamsV220` | `LocalTensor<T>` | L76 | `2201\|\|3003\|\|3113\|\|3510\|\|5102` |
| 84 | V2 V220 L0C→L1 deq | `FixpipeParamsV220` | `LocalTensor<T>` + `LocalTensor<S>` | L76 | 同上 |
| 159 | V7 Arch3510 L0C→L1/UB | `FixpipeParamsArch3510` | `LocalTensor<T>` | L157 | `!defined\|\|3510\|\|5102` |
| 164 | V8 Arch3510 L0C→L1/UB deq | `FixpipeParamsArch3510` | `LocalTensor<T>` + `LocalTensor<uint64_t>` | L157 | 同上 |

> **V1/V2 的 dst 位置分流（与 GM 变体的核心区别）：** V220 系列的 `LocalTensor` dst 在**运行期**由包装层按 `GetPhyType` 分发——L1 → `FixpipeL0C2L1Impl`，UB → `FixpipeL0C2UBImpl`。因此 V1/V2 实际覆盖 L0C→L1 与 L0C→UB 两条路径（UB 路径在 l300/l310/l311 上为桩，见下文）。

> **V7/V8 的 dst 位置分流：** Arch3510 系列在**编译期**由 `config.isToUB` 分发（`FixpipeConfig::isToUB` 字段仅 3510/5102 存在，kernel_struct_fixpipe.h L51-53）。

> **V2/V8 deq 变体：** 多一个 `LocalTensor<S> cbufWorkspace` 参数（deq tensor quant 工作区），SFINAE 约束 `S = uint64_t`。

### 各目标版本可见的声明

| 版本 | Arch | V1(79) V220 | V2(84) V220 deq | V7(159) Arch3510 | V8(164) Arch3510 deq |
|------|------|-------------|-----------------|------------------|----------------------|
| dav_l300 | 3003 | ✓ | ✓ | ✗ | ✗ |
| dav_l310 | 3103 | ✗ | ✗ | ✗ | ✗ |
| dav_l311 | 3113 | ✓ | ✓ | ✗ | ✗ |
| dav_5161 | 5101 | ✗ | ✗ | ✗ | ✗ |
| dav_3510 | 3510 | ✓ | ✓ | ✓ | ✓ |

> **关键发现：** 3103(3103) 不在任何 arch 宏控中——V220 宏控不含 3103，Arch3510 宏控仅 3510/5102。故 3103 **看不到任何 Fixpipe L0C→L1 声明**（与 L0C→GM 分析结论一致）。同理 5101 也看不到任何声明。

## 调用链

### V1/V2 (V220, L79/L84) 调用链

```
intf.h:79/84（声明，arch 宏控 L76: 2201||3003||3113||3510||5102）
  → intf_impl.h:110-125 / 128-147（公共包装，无 arch 宏控）
    → #if ASCENDC_DEBUG || CPU_DEBUG: CheckFixpipeTensor<T,U,config>(...)
    → dst∈L1: FixpipeL0C2L1Impl<PrimT<T>,PrimT<U>,config>((__cbuf__*)dst..., (__cc__*)src..., intriParams)      // V1: 3 参数
    → dst∈UB: FixpipeL0C2UBImpl<PrimT<T>,PrimT<U>,config>((__ubuf__*)dst..., (__cc__*)src..., intriParams)      // V1: 3 参数
    → （V2: 多传 (__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr() — 4 参数）
      → dav_XXXX/kernel_operator_fixpipe_impl.h（版本特定实现，通过 intf_impl.h:30-54 的 #elif #include 分派）
```

### V7/V8 (Arch3510, L159/L164) 调用链

```
intf.h:159/164（声明，arch 宏控 L157: !defined||3510||5102）
  → intf_impl.h:192-211 / 214-237（公共包装，arch 宏控 L190: 3510||5102）
    → CheckTensorPos(src, Hardware::L0C, "src", "CO1", "Fixpipe")
    → ASCENDC_CHECK_TPOSITION(dst ∈ {L1, UB}, ...)
    → （V8 另有 CheckTensorPos(cbufWorkspace, Hardware::L1, "cbufWorkspace", "C1", "Fixpipe")）
    → if constexpr (config.isToUB): FixpipeL0C2UBImpl(dst, src, intriParams)
    → else:                        FixpipeL0C2L1Impl(dst, src, intriParams)
      → dav_3510/kernel_operator_fixpipe_impl.h（仅 3510/5102 可达）
```

> **注意：** V1/V2 包装按**运行期** `GetPhyType(dst.GetPosition())` 分流；V7/V8 包装按**编译期** `config.isToUB` 分流。两种机制互不兼容：`isToUB` 字段仅 3510/5102 的 `FixpipeConfig` 存在，V7/V8 因此无法平移到其他架构。

### 共享层分发逻辑

**版本隔离机制：** `intf_impl.h:30-54` 通过 `#elif __NPU_ARCH__` 分派 `#include`：

| Arch | #include 的文件 |
|------|----------------|
| 3510 | `dav_3510/kernel_operator_fixpipe_impl.h` |
| 3003 | `dav_l300/kernel_operator_fixpipe_impl.h` |
| 3103 | `dav_l310/kernel_operator_fixpipe_impl.h` |
| 3113 | `dav_l311/kernel_operator_fixpipe_impl.h` |
| 5101/5161/5165/5163 | `dav_5161/kernel_operator_fixpipe_impl.h` |

**intf_impl.h 公共包装层 arch 宏控汇总：**

| 包装区域 | 行号 | arch 宏控 | 调用签名 |
|---------|------|----------|---------|
| V220 L0C→L1 (V1) | L110-125 | **无** | 运行期分流 `FixpipeL0C2L1Impl` / `FixpipeL0C2UBImpl` — 3 参数 |
| V220 L0C→L1 deq (V2) | L128-147 | **无** | 同上 — 4 参数（含 cbufWorkspace） |
| Arch3510 L0C→L1/UB (V7) | L192-211 | `3510\|\|5102` | 编译期分流 `config.isToUB` — 3 参数 |
| Arch3510 L0C→L1/UB deq (V8) | L214-237 | `3510\|\|5102` | 同上 — 4 参数 |

> **V220 包装无 arch 宏控：** 即使 3103/5101 在 intf.h 中看不到 V220 声明，intf_impl.h 的 V220 包装模板仍会被编译。但由于模板是惰性实例化的，若用户无法调用 `Fixpipe`（因 intf.h 未声明），则模板不会被实例化，不会因缺少 `FixpipeL0C2L1Impl/L0C2UBImpl` 而报错。

> **ProcessLock 差异：** L1/UB 变体包装（L110-237）均无 `ProcessLock`（g_isAtomic 写锁仅 GM 变体需要）。

## 支持矩阵

| 行号 | 变体 | 3003 | 3103 | 3113 | 5101 | 3510 |
|------|------|------|------|------|------|------|
| 79 | V1 V220 L0C→L1 | **完整** | **未声明** | **完整** | **未声明** | **完整** |
| 84 | V2 V220 L0C→L1 deq | **完整** | **未声明** | **完整** | **未声明** | **完整** |
| 159 | V7 Arch3510 L0C→L1/UB | ✗ | ✗ | ✗ | ✗ | **完整** |
| 164 | V8 Arch3510 L0C→L1/UB deq | ✗ | ✗ | ✗ | ✗ | **完整** |

### V1/V2 (V220) 各版本实现详情

| 版本 | FixpipeL0C2L1Impl(V220) V1 | FixpipeL0C2L1Impl(V220) V2 deq | FixpipeL0C2UBImpl(V220) | 本地参数结构体 | V220 适配函数 |
|------|--------------------------|-------------------------------|------------------------|--------------|-------------|
| dav_l300 | **完整** (L498-504) | **完整** (L506-513) | **桩** (L532-543，运行时 assert) | `FixpipeParamsL300` (L104-131) | `TransFixpipeParamsV220ToFixpipeParamsL300` (L474-496) |
| dav_l310 | **完整** (L505-511) | **完整** (L513-520) | **桩** (L539-550，运行时 assert) | `FixpipeParamsL311` (L104-131) | `TransFixpipeParamsV220ToFixpipeParamsL311` (L481-503) |
| dav_l311 | **完整** (L505-511) | **完整** (L513-520) | **桩** (L539-550，运行时 assert) | `FixpipeParamsL311` (L104-131) | `TransFixpipeParamsV220ToFixpipeParamsL311` (L481-503) |
| dav_5161 | **未定义** | **未定义** | **未定义** | N/A | N/A |
| dav_3510 | **完整** (L800-806) | **完整** (L808-815) | **完整** (L817-832) | `FixpipeParamsArch3510` (kernel_struct_fixpipe.h:190-223) | `TransFixpipeParamsV220ToFixpipeParamsArch3510` (L776-798) |

> **注意 1：** l310 虽然定义了完整的 `FixpipeL0C2L1Impl(V220)`，但 intf.h 未为 3103 声明 V220 `Fixpipe` 函数，故该实现为**死代码**。

> **注意 2（与 GM 变体的核心差异）：** V1/V2 的 dst=UB 路径——l300/l310/l311 为**桩函数**（编译通过、运行时 `ASCENDC_ASSERT` 报错 "Fixpipe doesn't support L0C to UB on current device"）；仅 3510 通过 `copy_matrix_cc_to_ub` 完整支持。

## 各版本原始代码

### dav_3510（参照版本，3510）

#### V7 FixpipeL0C2L1Impl(Arch3510) — 完整实现（L572-601）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    if ASCEND_IS_AIC {
        CheckFixpipeL0C2L1Param<DstT, SrcT, config>(dst, src, intriParams);
        // nz2nd/nz2dn need set LOOP3_PARA
        SetLoop3Para<config>(intriParams);
        // nz2dn mode need set CHANNEL_PARA extra
        if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
            // Loop0_dst_stride in uint of CO_SIZE
            uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
            set_channel_para(channelPara);
        }
        if (IsScalarQuantMode(intriParams.quantPre)) {
            // deq factor of uint64 bits describe: bits[31:13] is deq value of fp32,
            set_quant_pre(intriParams.deqScalar);
        }
        PipeBarrier<PIPE_FIX>();
        FixpipeTiling fixpipeTiling;
        // LOC -> L1
        FixpipeL0cToL1<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
    }
}
```

#### V8 FixpipeL0C2L1Impl(Arch3510, deq) — 完整实现（L603-640）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams)
{
    if ASCEND_IS_AIC {
        CheckFixpipeL0C2L1Param<DstT, SrcT, config>(dst, src, intriParams);
        SetLoop3Para<config>(intriParams);
        if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
            uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48;
            set_channel_para(channelPara);
        }
        FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
        if (IsVectorQuantMode(intriParams.quantPre)) {
            for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
                FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                    dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
            }
            if (fixpipeTiling.tailNSize > 0) {
                FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                    dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize,
                    fixpipeTiling.nIterNum);
            }
            return;
        }
    }
}
```

#### V7/V8 的 UB 分流目标：FixpipeL0C2UBImpl — 完整实现（L642-670 / L672-708）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams)
{
    if ASCEND_IS_AIC {
        CheckFixpipeL0C2UBParam<DstT, SrcT, config>(dst, src, intriParams);
        SetLoop3Para<config>(intriParams);
        if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
            uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48;
            set_channel_para(channelPara);
        }
        if (IsScalarQuantMode(intriParams.quantPre)) {
            set_quant_pre(intriParams.deqScalar); // float32->uint64_t
        }
        PipeBarrier<PIPE_FIX>();
        // LOC->UB
        FixpipeTiling fixpipeTiling;
        FixpipeL0cToUB<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
    }
}
//（L672-708：带 cbufWorkspace 的 vector quant 版本，结构同 L1 版）
```

#### V1 FixpipeL0C2L1Impl(V220) — 适配包装（L800-806）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsArch3510<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsArch3510(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, params);
}
```

#### V2 FixpipeL0C2L1Impl(V220, deq) — 适配包装（L808-815）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsArch3510<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsArch3510(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}
```

#### V1/V2 的 UB 分流目标：FixpipeL0C2UBImpl(V220) — 适配包装（L817-832）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsArch3510<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsArch3510(intriParams, params);
    FixpipeL0C2UBImpl<DstT, SrcT, config>(dst, src, params);
}
//（L825-832：带 cbufWorkspace 版本，同构）
```

#### V220→Arch3510 参数转换（L776-798）

```cpp
template <CO2Layout format>
__aicore__ inline void TransFixpipeParamsV220ToFixpipeParamsArch3510(
    const FixpipeParamsV220& intriParams, FixpipeParamsArch3510<format>& dstParams)
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
        dstParams.params.ndNum = intriParams.ndNum;           // ← Nz2DnParams 无此字段（见待确认问题 5）
        dstParams.params.srcNdStride = intriParams.srcNdStride;
        dstParams.params.dstNdStride = intriParams.srcNdStride;
    }
}
```

#### FixpipeL0cToL1 — ISA 调用（L188-222，25 参数 copy_matrix_cc_to_cbuf）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToL1(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    ASCENDC_DEBUG_ASSERT((!(intriParams.isChannelSplit)),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "Failed to check isChannelSplit in Fixpipe ..."));
    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    bool nz2ndEn = false;
    bool nz2dnEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize * intriParams.dstStride;   // ← 与 l300 的 NZ 回退式不同
        nz2dnEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    return copy_matrix_cc_to_cbuf(
        (__cbuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, 0, intriParams.unitFlag,
        static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn), false, nz2ndEn,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false, false, false, nz2dnEn);
}
```

#### FixpipeL0cToUB — ISA 调用（L226-282，26 参数 copy_matrix_cc_to_ub，l300 系无此函数）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToUB(
    __ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    // ...isChannelSplit DEBUG_ASSERT + dualDstCtl 三重限制检查：
    //    dualDstCtl!=0 仅允许普通 DMA/NZ2ND；0b01 要求 mSize%2==0；0b10 要求 calNSize%32==0 ...
    return copy_matrix_cc_to_ub(
        (__ubuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, intriParams.dualDstCtl, intriParams.subBlockId, 0,
        intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn),
        false, nz2ndEn, static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false, false,
        false, nz2dnEn);
}
```

#### 参数检查（L535-570 摘要）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void CheckFixpipeL0C2L1Param(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& params)
{
    CheckFixpipeParams<DstT, SrcT, config>(src, params);   // static_assert 13 种类型组合 + 量化矩阵
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
    uint64_t dstAbsAddr = dst - (DstT*)(GetBaseAddrCpu(int8_t(TPosition::C1)));   // ← L1 目的地 32B 对齐检查
    ASCENDC_ASSERT((dstAbsAddr * sizeof(DstT) % ONE_BLK_SIZE == 0), { ... });
#endif
}
//（CheckFixpipeL0C2UBParam L535-548 同构，基址取 TPosition::VECCALC）
```

---

### dav_l300（目标版本，3003）

#### V1 FixpipeL0C2L1Impl(V220) — 适配包装（L498-504）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, params);
}
```

#### V2 FixpipeL0C2L1Impl(V220, deq) — 适配包装（L506-513）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}
```

#### V1/V2 的 UB 分流目标：FixpipeL0C2UBImpl(V220) — **桩函数**（L532-543）

```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ T* dst, __cc__ U* src, const FixpipeParamsV220& intriParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Fixpipe doesn't support L0C to UB on current device\n"); });
}
//（L538-543：带 cbufWorkspace 版本，同样为桩）
```

#### V1 本地 FixpipeL0C2L1Impl(L300) — 完整实现（L353-379）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL300<config.format>& intriParams)
{
    // nz2nd/nz2dn need set LOOP3_PARA
    SetLoop3Para<config>(intriParams);
    // nz2dn mode need set CHANNEL_PARA extra
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48; // CHANNEL_PARA[63:48]
        set_channel_para(channelPara);
    }
    if (IsScalarQuantMode(intriParams.quantPre)) {
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    FixpipeTiling fixpipeTiling;
    // LOC -> L1
    FixpipeL0cToL1<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}
```

#### V2 本地 FixpipeL0C2L1Impl(L300, deq) — 完整实现（L381-414）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL300<config.format>& intriParams)
{
    SetLoop3Para<config>(intriParams);
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48;
        set_channel_para(channelPara);
    }
    if (IsVectorQuantMode(intriParams.quantPre)) {
        FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2L1ImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
}
```

#### V220→L300 参数转换（L474-496）

```cpp
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
        dstParams.params.ndNum = intriParams.ndNum;           // ← uint8_t 占位，无此字段（见待确认问题 5）
        dstParams.params.srcNdStride = intriParams.srcNdStride;
        dstParams.params.dstNdStride = intriParams.srcNdStride;
    }
}
```

#### FixpipeL0cToL1 — ISA 调用（L223-253，**22 参数** copy_matrix_cc_to_cbuf）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToL1(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL300<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    ASCENDC_DEBUG_ASSERT((!(intriParams.isChannelSplit)), ...);
    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    bool nz2ndEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else {
        // ← 无 COLUMN_MAJOR 专分支：dstOffset 回退到 NZ 模式计算
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    return copy_matrix_cc_to_cbuf(
        (__cbuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag,
        static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn), false, nz2ndEn,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false, false);
}
```

#### CopyDeqTensorToFbuf — deq tensor L1→FB（L178-195，**burstLen=128B**）

```cpp
__aicore__ inline void CopyDeqTensorToFbuf(
    __cbuf__ uint64_t* cbufWorkspace, const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex)
{
    constexpr uint32_t deqTensorAddrAlignValue = 128;
    uint16_t deqDataSize = DivCeil(calNSize * sizeof(uint64_t), deqTensorAddrAlignValue) * deqTensorAddrAlignValue;
    __fbuf__ uint64_t* deqTensorTempBuf =
        AscendCUtils::GetTemporaryFbBufferAddr<uint64_t>(0, deqDataSize / sizeof(uint64_t));
    uint32_t deqValueOffset = nIterIndex * fixpipeTiling.nSize;
    constexpr uint16_t fbufBurstLenUnit = 128;               // ← l300 独有 128B
    uint16_t fbufBurstLen = deqDataSize / fbufBurstLenUnit;
    copy_cbuf_to_fbuf(deqTensorTempBuf, cbufWorkspace + deqValueOffset, 1, fbufBurstLen, 0, 0);
    uint64_t deqTensorAddr = ((uint64_t)deqTensorTempBuf >> static_cast<uint64_t>(7)) << 8;
    set_fpc(deqTensorAddr);
    AscendCUtils::FreeTemporaryFbBuffer<uint64_t>(deqTensorTempBuf);
}
```

---

### dav_l310（目标版本，3103）

> **注意：** 3103 不在 intf.h 任何 arch 宏控中，用户无法调用 Fixpipe L0C→L1。以下实现虽存在但为**死代码**（与 L0C→GM 分析结论一致）。

#### V1 FixpipeL0C2L1Impl(V220) — 适配包装（L505-511）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, params);
}
```

#### V2 FixpipeL0C2L1Impl(V220, deq) — 适配包装（L513-520）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}
```

#### V1/V2 的 UB 分流目标：FixpipeL0C2UBImpl(V220) — **桩函数**（L539-550）

```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ T* dst, __cc__ U* src, const FixpipeParamsV220& intriParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Fixpipe doesn't support L0C to UB on current device\n"); });
}
//（L545-550：带 cbufWorkspace 版本，同样为桩）
```

#### V1 本地 FixpipeL0C2L1Impl(L311) — 完整实现（L360-386）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL311<config.format>& intriParams)
{
    SetLoop3Para<config>(intriParams);
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48;
        set_channel_para(channelPara);
    }
    if (IsScalarQuantMode(intriParams.quantPre)) {
        set_quant_pre(intriParams.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    FixpipeTiling fixpipeTiling;
    FixpipeL0cToL1<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}
```

#### V2 本地 FixpipeL0C2L1Impl(L311, deq) — 完整实现（L388-414）

> 与 dav_l300 的 V2 本地版本（L381-414）结构相同，仅参数结构体为 `FixpipeParamsL311`。

#### V220→L311 参数转换（L481-503）

```cpp
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
        dstParams.params.ndNum = intriParams.ndNum;           // ← uint8_t 占位，无此字段（见待确认问题 5）
        dstParams.params.dstNdStride = intriParams.srcNdStride;
        dstParams.params.srcNdStride = intriParams.srcNdStride;
    }
}
```

#### FixpipeL0cToL1 — ISA 调用（L223-257，**24 参数** copy_matrix_cc_to_cbuf，含 nz2dnEn）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToL1(
    __cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL311<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    ASCENDC_DEBUG_ASSERT((!(intriParams.isChannelSplit)), ...);
    uint16_t cburstNum = fixpipeTiling.nSize / BLOCK_CUBE;
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    bool nz2ndEn = false;
    bool nz2dnEn = false;                                     // ← 有 COLUMN_MAJOR 专分支（与 l300 不同）
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize * intriParams.dstStride;
        nz2dnEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * DEFAULT_C0_SIZE / sizeof(DstT);
    }
    return copy_matrix_cc_to_cbuf(
        (__cbuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag,
        static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn), false, nz2ndEn,
        static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false, false, false, nz2dnEn);
}
```

> **ISA 参数数量：** l310 `copy_matrix_cc_to_cbuf` = 24 参数（无 `cacheMode`，有 `nz2dnEn`）。CopyDeqTensorToFbuf burstLen=64B。

---

### dav_l311（目标版本，3113）

#### V1 FixpipeL0C2L1Impl(V220) — 适配包装（L505-511）

> 与 dav_l310 完全相同。

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, params);
}
```

#### V2 FixpipeL0C2L1Impl(V220, deq) — 适配包装（L513-520）

> 与 dav_l310 完全相同。

#### V1/V2 的 UB 分流目标：FixpipeL0C2UBImpl(V220) — 桩函数（L539-550）

> 与 dav_l310 完全相同（ASCENDC_ASSERT 桩）。

#### V1 本地 FixpipeL0C2L1Impl(L311) — 完整实现（L360-386）

> 与 dav_l310 完全相同。

#### V2 本地 FixpipeL0C2L1Impl(L311, deq) — 完整实现（L388-414）

> 与 dav_l310 完全相同。

#### FixpipeL0cToL1 — ISA 调用（L223-257，24 参数 copy_matrix_cc_to_cbuf）

> 与 dav_l310 完全相同（24 参数，有 `nz2dnEn`，CopyDeqTensorToFbuf burstLen=64B）。

---

### dav_5161（目标版本，5101）

#### V1/V2 FixpipeL0C2L1Impl / FixpipeL0C2UBImpl — **未定义**

> dav_5161 的 `kernel_operator_fixpipe_impl.h` 仅 78 行，仅定义了 `SetFixPipeClipReluImpl`、`SetFixPipeAddrImpl`、`SetFixPipeConfigImpl`、`SetFixpipeNz2ndFlagImpl`、`SetFixpipePreQuantFlagImpl` 等配置函数。**完全没有定义** `FixpipeL0C2L1Impl`、`FixpipeL0C2UBImpl` 或 `FixpipeL0cToL1` 等数据搬运函数。
>
> 5101 架构在 intf.h 中看不到任何 Fixpipe L0C→L1 声明（不在任何 arch 宏控中），因此即使缺少实现也不会导致编译错误。

## 代码级实现差异分析

### 实现风格对比

| 特征 | dav_3510 | dav_l300 | dav_l310 | dav_l311 | dav_5161 |
|------|----------|----------|----------|----------|----------|
| V1 V220 可见 | ✓ | ✓ | ✗（未声明） | ✓ | ✗（未声明） |
| V2 V220 deq 可见 | ✓ | ✓ | ✗（未声明） | ✓ | ✗（未声明） |
| V7 Arch3510 可见 | ✓ | ✗ | ✗ | ✗ | ✗ |
| V8 Arch3510 deq 可见 | ✓ | ✗ | ✗ | ✗ | ✗ |
| 本地参数结构体 | `FixpipeParamsArch3510` | `FixpipeParamsL300` | `FixpipeParamsL311` | `FixpipeParamsL311` | N/A |
| AIC 宏控 | `if ASCEND_IS_AIC` | 无 | 无 | 无 | N/A |
| 参数检查 | `CheckFixpipeL0C2L1Param`（static_assert 类型矩阵 + L1/UB 目的端对齐） | 无 | 无 | 无 | N/A |
| ISA `copy_matrix_cc_to_cbuf` 参数数 | 25（含 cacheMode 位 + nz2dnEn） | 22（无 cacheMode，无 nz2dnEn） | 24（无 cacheMode，有 nz2dnEn） | 24（同 l310） | N/A |
| ISA `copy_matrix_cc_to_ub` | ✓（`FixpipeL0cToUB`，dualDstCtl/subBlockId） | ✗（桩函数） | ✗（桩函数） | ✗（桩函数） | ✗（未定义） |
| COLUMN_MAJOR (nz2dnEn) | ✓ | ✗（无专分支，回退 NZ 计算） | ✓ | ✓ | N/A |
| dst 位置分流 | V220: 运行期 GetPhyType；Arch3510: 编译期 `config.isToUB` | V220: 运行期（UB→桩） | 同 l300 | 同 l300 | N/A |
| QuantMode 集合 | F322 系列 | QF162 系列 | QF162 系列 | QF162 系列 | N/A |
| CopyDeqTensorToFbuf burstLen | 64B | 128B | 64B | 64B | N/A |
| `FixpipeConfig::isToUB` | ✓（3510/5102 专属） | ✗ | ✗ | ✗ | ✗ |
| ProcessLock（CPU debug 原子锁） | 无（仅 GM 变体有） | 无 | 无 | 无 | N/A |

### QuantMode 对比

| 版本 | IsVectorQuantMode 包含的模式 | IsScalarQuantMode 包含的模式 |
|------|---------------------------|---------------------------|
| l300/l310/l311 | VDEQF16, VQF162B8_PRE, VREQ8, VQF162S4_PRE, VREQ4, VDEQS16, VQF162S16_PRE | DEQF16, QF162B8_PRE, REQ8, QF162S4_PRE, REQ4, DEQS16, QF162S16_PRE |
| 3510 | VDEQF16, VQF322B8_PRE, VREQ8, VQS322BF16_PRE, VQF322F16_PRE, VQF322BF16_PRE, VQF322FP8_PRE, VQF322HIF8_PRE, VQF322HIF8_PRE_HYBRID, VQF322F32_PRE | DEQF16, QF322B8_PRE, REQ8, QS322BF16_PRE, QF322F16_PRE, QF322BF16_PRE, QF322FP8_PRE, QF322HIF8_PRE, QF322HIF8_PRE_HYBRID, QF322F32_PRE |

### 参数结构体对比

四个结构体定义位置（详细字段对比见 `Fixpipe(L0C-GM)-analysis.md` 参数结构体全量对比，结论一致）：

| 结构体 | 定义位置 | 使用版本 | 是否模板 |
|--------|---------|---------|---------|
| `FixpipeParamsV220` | `kernel_struct_fixpipe.h:107-166` | 2201/3003/3113/3510/5102（V1/V2 变体） | ✗（非模板，NZ2ND 字段平铺） |
| `FixpipeParamsArch3510` | `kernel_struct_fixpipe.h:190-223` | 3510/5102（V7/V8 变体） | ✓ `template<CO2Layout format>` |
| `FixpipeParamsL300` | `dav_l300/kernel_operator_fixpipe_impl.h:104-131` | 3003（本地实现） | ✓ |
| `FixpipeParamsL311` | `dav_l310/l311/kernel_operator_fixpipe_impl.h:104-131` | 3103/3113（本地实现） | ✓ |

> **COLUMN_MAJOR 的 PARAMS 类型差异（本路径特有重点）：** 共享层 `TransformParams<COLUMN_MAJOR>::PARAMS = Nz2DnParams`（kernel_struct_fixpipe.h:184-188，仅 Arch3510 系列可达）；dav 内部 `TransformParamsL300/L311<COLUMN_MAJOR>::PARAMS = uint8_t`（占位，不支持）。`Nz2DnParams`（kernel_utils_struct_dma_params.h:80-97）仅有 `dnNum/srcNzMatrixStride/dstDnMatrixStride/srcNzC0Stride` 四字段。

### ISA 参数映射对比（copy_matrix_cc_to_cbuf）

| 参数位置 | 3510 (25 参数) | l300 (22 参数) | l310/l311 (24 参数) |
|---------|---------------|---------------|-------------------|
| 1-7 | dst, src, 0, calNSize, mSize, dstStride, srcStride | 同 | 同 |
| 8 | **0**（cacheMode 位，L1 无 cache，填 0） | 0 | 0 |
| 9 | 0 | unitFlag | unitFlag |
| 10 | unitFlag | quantPre | quantPre |
| 11 | quantPre | reluEn | reluEn |
| 12 | reluEn | false | false |
| 13 | false | nz2ndEn | nz2ndEn |
| 14 | nz2ndEn | NoConv | NoConv |
| 15 | NoConv | 0 | 0 |
| 16 | 0 | false | false |
| 17 | false | false | false |
| 18 | false | 0 | 0 |
| 19 | 0 | false | false |
| 20 | false | false | false |
| 21 | false | false | false |
| 22 | false | false | false |
| 23 | false | — | false |
| 24 | false | — | **nz2dnEn** |
| 25 | **nz2dnEn** | — | — |

> 与 `copy_matrix_cc_to_gm` 的映射模式一致（GM 版位置 8 为真实 `cacheMode` 变量，cbuf 版为字面 0）。

### 实现模式总结

**dav_3510 — 最完整实现**
- 支持 V1/V2 (V220) 和 V7/V8 (Arch3510) 双路径；V220 经 `TransFixpipeParamsV220ToFixpipeParamsArch3510` 适配后委托
- L0C→L1 与 L0C→UB 双目的地完整（`copy_matrix_cc_to_cbuf` 25 参 / `copy_matrix_cc_to_ub` 26 参，UB 支持 dualDstCtl 双目的模式）
- 有 AIC 宏控、static_assert 类型矩阵（13 种 src×dst 组合）、CPU debug 的 L1/UB 目的端 32B 对齐检查
- V220 的 UB 分流同样完整（V220→Arch3510 转换后走原生 UB 实现）

**dav_l300 — 精简实现（V220 only，L1 only）**
- 仅支持 V1/V2 (V220) 的 L0C→L1；UB 为桩（运行时 assert）
- 本地 `FixpipeParamsL300`；无 AIC 宏控、无参数检查
- ISA 22 参数（无 cacheMode、无 nz2dnEn）；COLUMN_MAJOR 无专分支（dstOffset 回退 NZ 计算）
- CopyDeqTensorToFbuf burstLen=128B（独有）

**dav_l310 / dav_l311 — 中间实现（V220 only，L1 only，代码完全相同）**
- 仅支持 V1/V2 (V220) 的 L0C→L1；UB 为桩（运行时 assert）
- 本地 `FixpipeParamsL311`；无 AIC 宏控、无参数检查
- ISA 24 参数（无 cacheMode，有 nz2dnEn）；COLUMN_MAJOR 有专分支
- CopyDeqTensorToFbuf burstLen=64B
- **l310 的实现对 3103 为死代码**（intf.h 未声明）

**dav_5161 — 无实现**
- 完全未定义 Fixpipe 数据搬运函数；5101 不在 intf.h 任何 arch 宏控中

### 关键差异点

1. **3103 完全不可见** — 3103 不在 V220 宏控（`2201||3003||3113||3510||5102`）中，也不在 Arch3510 宏控中。intf.h 中无任何 Fixpipe L0C→L1 声明对 3103 可见。但 l310 impl 文件中存在完整实现（死代码）。**与 L0C→GM 分析的待确认问题 1 同源，需一并对齐**。

2. **5101 完全不可见** — 同理 5101 不在任何宏控中，且 impl 文件中无任何 Fixpipe 数据搬运实现。

3. **L0C→UB 路径仅 3510 可用（本路径最核心差异）** — V1/V2 的运行期 UB 分流在 l300/l310/l311 上落到桩函数（编译通过、运行时 `ASCENDC_ASSERT` 报错），在 3510 上完整（`copy_matrix_cc_to_ub`，含 dualDstCtl/subBlockId）。V7/V8 的编译期 `isToUB` 分流仅 3510/5102 可达。

4. **两种 dst 分流机制并存** — V220 系列运行期 `GetPhyType`（错误延迟到运行期）；Arch3510 系列编译期 `config.isToUB`（错误暴露在编译期）。`FixpipeConfig::isToUB` 字段仅 3510/5102 存在，是 V7/V8 无法直接移植到其他架构的结构性障碍。

5. **ISA 参数数量差异** — `copy_matrix_cc_to_cbuf`：l300=22、l310/l311=24、3510=25。差异来源：`cacheMode` 位（仅 3510 有）和 `nz2dnEn`（l310/l311/3510 有，l300 无）。与 GM 路径的参数数量模式完全一致。

6. **COLUMN_MAJOR 支持** — l300 无 nz2dnEn 路径（dstOffset 回退 NZ 模式），l310/l311/3510 支持。但 V220 系列的 `TransV220` COLUMN_MAJOR 分支在**所有版本**上都是坏代码（见待确认问题 5）。

7. **检查力度** — 3510 有 L1/UB 目的端对齐检查（`CheckFixpipeL0C2L1Param`/`CheckFixpipeL0C2UBParam`，基址分别取 C1/VECCALC）；l300 系完全无目的端检查。

## 待确认问题

1. **3103 是否应该支持 Fixpipe L0C→L1？** — l310 impl 文件中有完整的 `FixpipeL0C2L1Impl` 实现（V220 适配 + L311 本地），但 intf.h 的 V220 arch 宏控不含 3103。这是遗漏（应在宏控中添加 3103）还是有意排除（3103 硬件不支持）？与 L0C→GM 分析待确认问题 1 同源，两处宏控（L76 与 L92 区域）需一并对齐。

2. **5101 是否应该支持 Fixpipe L0C→L1？** — 5161 impl 文件中完全没有 Fixpipe 数据搬运函数。5101 硬件是否支持 `copy_matrix_cc_to_cbuf` 指令？如不支持，当前状态（intf.h 不声明 + impl 不定义）是正确的。如支持，需要补充实现。

3. **intf_impl.h V220 包装无 arch 宏控** — intf_impl.h L110-147 的 V220 包装模板无 arch 宏控，在 3103/5101 上也会被编译。由于 intf.h 未声明对应 `Fixpipe` 函数，模板不会被实例化，因此不会报错。但这种"impl 有代码但 intf 不暴露"的模式是否是设计意图？

4. **l300/l310/l311 的 L0C→UB 桩是硬件不支持还是待补全？** — 桩文案 "Fixpipe doesn't support L0C to UB on current device" 表明按硬件能力声明；但 3510 有 `copy_matrix_cc_to_ub`。若 3003/3103/3113 硬件支持 L0C→UB，需参照 3510 补 `FixpipeL0cToUB` + `FixpipeL0C2UBImpl`；若不支持，建议将运行期 assert 提前为编译期报错（当前编译通过、运行期才报错）。

5. **`TransV220` 的 COLUMN_MAJOR 分支在所有版本上均为坏代码。** — 3510 侧：`Nz2DnParams` 无 `ndNum/srcNdStride/dstNdStride` 字段（仅 `dnNum/srcNzMatrixStride/dstDnMatrixStride/srcNzC0Stride`）；l300/l310/l311 侧：COLUMN_MAJOR 的 PARAMS 为 `uint8_t` 占位。任何版本以 `CFG_COLUMN_MAJOR` 实例化 V220 `Fixpipe` 都会编译错误。且该分支的 `dstNdStride = srcNdStride` 映射本身可疑（V220 平铺参数无 DN 专用字段）。是占位符表达"不支持"（建议删除死分支并加 static_assert 明示）还是遗漏（应补全 Nz2DnParams 映射）？

6. **V7/V8 依赖 `FixpipeConfig::isToUB` 字段** — 该字段仅 3510/5102 存在（kernel_struct_fixpipe.h L51-53）。其他架构若需 Arch3510 系列接口，需先扩展 `FixpipeConfig`——确认这是有意分层，避免误判为宏控遗漏。

7. **CopyDeqTensorToFbuf burstLen 差异** — l300 使用 128B burstLen，l310/l311/3510 使用 64B。这是硬件差异还是代码遗留？（与 L0C→GM 分析待确认问题 7 相同，属同一函数。）

8. **l310 与 l311 完全相同** — l310 和 l311 的 `kernel_operator_fixpipe_impl.h` 文件内容完全相同（仅文件路径不同）。是否应该合并或通过宏区分？（与 L0C→GM 分析待确认问题 8 相同。）
