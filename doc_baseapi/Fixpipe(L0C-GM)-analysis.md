# 案例分析：Fixpipe(L0C→GM) 接口跨版本适配

## 接口概述

`Fixpipe` 用于 L0C→L1/UB/GM 的矩阵数据搬运（支持量化/ReLU/NZ2ND 等增强功能）。本次分析的 8 个接口均为 **L0C→GM** 变体，分布在 `kernel_operator_fixpipe_intf.h` 的 4 组 arch 守卫块中：

| 行号 | 变体 | 参数类型 | dst 类型 | arch 守卫 | 守卫条件 |
|------|------|---------|---------|-----------|---------|
| 92 | V1 V220 L0C→GM | `FixpipeParamsV220` | `GlobalTensor<T>` | L76 | `2201\|\|3003\|\|3113\|\|3510\|\|5102` |
| 97 | V2 V220 L0C→GM deq | `FixpipeParamsV220` | `GlobalTensor<T>` + `LocalTensor<S>` | L76 | 同上 |
| 120 | V3 M300 L0C→GM | `FixpipeParamsM300` | `GlobalTensor<T>` | L105 | `3002` |
| 125 | V4 M300 L0C→GM deq | `FixpipeParamsM300` | `GlobalTensor<T>` + `LocalTensor<S>` | L105 | `3002` |
| 146 | V5 M310 L0C→GM | `FixpipeParamsM310` | `GlobalTensor<T>` | L131 | `3102` |
| 151 | V6 M310 L0C→GM deq | `FixpipeParamsM310` | `GlobalTensor<T>` + `LocalTensor<S>` | L131 | `3102` |
| 170 | V7 Arch3510 L0C→GM | `FixpipeParamsArch3510` | `GlobalTensor<T>` | L157 | `!defined\|\|3510\|\|5102` |
| 175 | V8 Arch3510 L0C→GM deq | `FixpipeParamsArch3510` | `GlobalTensor<T>` + `LocalTensor<uint64_t>` | L157 | 同上 |

> **参数类型别名：** `FixpipeParamsM300 = FixpipeParamsV220`（L235），`FixpipeParamsM310 = FixpipeParamsV220`（L236）。故 V3/V4 和 V5/V6 的参数类型与 V1/V2 相同。

> **V2/V4/V6/V8 deq 变体：** 多一个 `LocalTensor<S> cbufWorkspace` 参数（deq tensor quant 工作区），SFINAE 约束 `S = uint64_t`。

### 各目标版本可见的声明

| 版本 | Arch | V1(92) V220 | V2(97) V220 deq | V3(120) M300 | V4(125) M300 deq | V5(146) M310 | V6(151) M310 deq | V7(170) Arch3510 | V8(175) Arch3510 deq |
|------|------|-------------|-----------------|-------------|------------------|-------------|------------------|------------------|---------------------|
| dav_l300 | 3003 | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| dav_l310 | 3103 | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| dav_l311 | 3113 | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| dav_5161 | 5101 | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ | ✗ |
| dav_3510 | 3510 | ✓ | ✓ | ✗ | ✗ | ✗ | ✗ | ✓ | ✓ |

> **关键发现：** 3103(3103) 不在任何 arch 守卫中——V220 守卫不含 3103，M310 守卫是 3102(非 3103)，Arch3510 守卫也不含 3103。故 3103 **看不到任何 Fixpipe L0C→GM 声明**。同理 5101 也看不到任何声明。

## 调用链

### V1/V2 (V220, L92/L97) 调用链

```
intf.h:92/97（声明，arch 守卫 L76: 2201||3003||3113||3510||5102）
  → intf_impl.h:150-172 / 175-188（公共包装，无 arch 守卫）
    → FixpipeL0C2GMImpl<PrimT<T>, PrimT<U>, config>(dst, src, intriParams)         // V1: 3 参数
    → FixpipeL0C2GMImpl<PrimT<T>, PrimT<U>, config>(dst, src, cbufWorkspace, intriParams)  // V2: 4 参数
      → dav_XXXX/kernel_operator_fixpipe_impl.h（版本特定实现，通过 intf_impl.h:30-54 的 #elif #include 分派）
```

### V3/V4 (M300, L120/L125) 调用链

```
intf.h:120/125（声明，arch 守卫 L105: 3002）
  → 无 intf_impl.h 公共包装（M300 变体在 intf_impl.h 中无对应包装函数）
    → dav_m300/kernel_operator_fixpipe_impl.h（版本特定实现）
```

> **注意：** intf_impl.h 中无 M300 变体的包装函数。M300 的 Fixpipe 可能直接在 dav_m300 的 impl 文件中实现完整逻辑，或通过其他机制。但 3002 不是目标版本，此处不深入。

### V5/V6 (M310, L146/L151) 调用链

```
intf.h:146/151（声明，arch 守卫 L131: 3102）
  → 无 intf_impl.h 公共包装（同 M300）
    → dav_m310/kernel_operator_fixpipe_impl.h（版本特定实现）
```

### V7/V8 (Arch3510, L170/L175) 调用链

```
intf.h:170/175（声明，arch 守卫 L157: !defined||3510||5102）
  → intf_impl.h:240-269 / 272-291（公共包装，arch 守卫 L190: 3510||5102）
    → 内层 arch 守卫 L255/L281:
      #if 3510||5102: FixpipeL0C2GMImpl(dst, src, intriParams, cacheMode)       // 4 参数（含 cacheMode）
      #else:           FixpipeL0C2GMImpl(dst, src, intriParams)                // 3 参数（不含 cacheMode）
      → dav_XXXX/kernel_operator_fixpipe_impl.h（版本特定实现）
```

> **注意：** V7/V8 的 intf_impl.h 包装有**双层 arch 守卫**：外层 `#if 3510||5102`（L190）限制整段代码仅 3510/5102 可见；内层 `#if 3510||5102`（L255/L281）的 `#else` 分支在当前守卫下永远不会执行——这是为未来扩展预留的代码模式。

### 共享层分发逻辑

**版本隔离机制：** `intf_impl.h:30-54` 通过 `#elif __NPU_ARCH__` 分派 `#include`：

| Arch | #include 的文件 |
|------|----------------|
| 3510 | `dav_3510/kernel_operator_fixpipe_impl.h` |
| 3003 | `dav_l300/kernel_operator_fixpipe_impl.h` |
| 3103 | `dav_l310/kernel_operator_fixpipe_impl.h` |
| 3113 | `dav_l311/kernel_operator_fixpipe_impl.h` |
| 5101/5161/5165/5163 | `dav_5161/kernel_operator_fixpipe_impl.h` |

**intf_impl.h 公共包装层 arch 守卫汇总：**

| 包装区域 | 行号 | arch 守卫 | 调用签名 |
|---------|------|----------|---------|
| V220 L0C→GM (V1) | L150-172 | **无** | `FixpipeL0C2GMImpl(dst, src, intriParams)` — 3 参数 |
| V220 L0C→GM deq (V2) | L175-188 | **无** | `FixpipeL0C2GMImpl(dst, src, cbufWorkspace, intriParams)` — 4 参数 |
| Arch3510 L0C→GM (V7) | L240-269 | `3510\|\|5102` | 内层 `#if`: 4 参数(含cacheMode) / `#else`: 3 参数 |
| Arch3510 L0C→GM deq (V8) | L272-291 | `3510\|\|5102` | 内层 `#if`: 5 参数(含cacheMode) / `#else`: 4 参数 |

> **V220 包装无 arch 守卫：** 即使 3103/5101 在 intf.h 中看不到 V220 声明，intf_impl.h 的 V220 包装模板仍会被编译。但由于模板是惰性实例化的，若用户无法调用 `Fixpipe`（因 intf.h 未声明），则模板不会被实例化，不会因缺少 `FixpipeL0C2GMImpl` 而报错。

## 支持矩阵

| 行号 | 变体 | 3003 | 3103 | 3113 | 5101 | 3510 |
|------|------|------|------|------|------|------|
| 92 | V1 V220 L0C→GM | **完整** | **未声明** | **完整** | **未声明** | **完整** |
| 97 | V2 V220 L0C→GM deq | **完整** | **未声明** | **完整** | **未声明** | **完整** |
| 120 | V3 M300 L0C→GM | ✗ | ✗ | ✗ | ✗ | ✗ |
| 125 | V4 M300 L0C→GM deq | ✗ | ✗ | ✗ | ✗ | ✗ |
| 146 | V5 M310 L0C→GM | ✗ | ✗ | ✗ | ✗ | ✗ |
| 151 | V6 M310 L0C→GM deq | ✗ | ✗ | ✗ | ✗ | ✗ |
| 170 | V7 Arch3510 L0C→GM | ✗ | ✗ | ✗ | ✗ | **完整** |
| 175 | V8 Arch3510 L0C→GM deq | ✗ | ✗ | ✗ | ✗ | **完整** |

> V3/V4 (M300) 和 V5/V6 (M310) 对所有目标版本均不可见（arch 守卫分别为 3002 和 3102）。

### V1/V2 (V220) 各版本实现详情

| 版本 | FixpipeL0C2GMImpl(V220) V1 | FixpipeL0C2GMImpl(V220) V2 deq | 本地参数结构体 | V220 适配函数 |
|------|--------------------------|-------------------------------|--------------|-------------|
| dav_l300 | **完整** (L515-521) | **完整** (L523-530) | `FixpipeParamsL300` (L104-131) | `TransFixpipeParamsV220ToFixpipeParamsL300` (L474-496) |
| dav_l310 | **完整** (L522-528) | **完整** (L530-537) | `FixpipeParamsL311` (L104-131) | `TransFixpipeParamsV220ToFixpipeParamsL311` (L481-503) |
| dav_l311 | **完整** (L522-528) | **完整** (L530-537) | `FixpipeParamsL311` (L104-131) | `TransFixpipeParamsV220ToFixpipeParamsL311` (L481-503) |
| dav_5161 | **未定义** | **未定义** | N/A | N/A |
| dav_3510 | **完整** (L834-840) | **完整** (L842-849) | `FixpipeParamsArch3510` (kernel_struct_fixpipe.h:190-223) | `TransFixpipeParamsV220ToFixpipeParamsArch3510` (L776-798) |

> **注意：** l310 虽然定义了 `FixpipeL0C2GMImpl(V220)`，但 intf.h 未为 3103 声明 V220 `Fixpipe` 函数，故该实现为**死代码**。

## 各版本原始代码

### dav_3510（参照版本，3510）

#### V7 FixpipeL0C2GMImpl(Arch3510, cacheMode) — 完整实现（L710-737）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams,
    const uint8_t cacheMode = 0)
{
    if ASCEND_IS_AIC {
        CheckFixpipeL0C2GMParam<DstT, SrcT, config>(dst, src, intriParams);
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
        FixpipeL0cToOut<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize, cacheMode);
    }
}
```

#### V8 FixpipeL0C2GMImpl(Arch3510, deq, cacheMode) — 完整实现（L739-774）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsArch3510<config.format>& intriParams, const uint8_t cacheMode = 0)
{
    if ASCEND_IS_AIC {
        CheckFixpipeL0C2GMParam<DstT, SrcT, config>(dst, src, intriParams);
        SetLoop3Para<config>(intriParams);
        if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
            uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48;
            set_channel_para(channelPara);
        }
        FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
        if (IsVectorQuantMode(intriParams.quantPre)) {
            for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
                FixpipeL0C2GMImplN<DstT, SrcT, config>(
                    dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i, cacheMode);
            }
            if (fixpipeTiling.tailNSize > 0) {
                FixpipeL0C2GMImplN<DstT, SrcT, config>(
                    dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize,
                    fixpipeTiling.nIterNum, cacheMode);
            }
            return;
        }
    }
}
```

#### V1 FixpipeL0C2GMImpl(V220) — 适配包装（L834-840）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(__gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsArch3510<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsArch3510(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, params);
}
```

#### V2 FixpipeL0C2GMImpl(V220, deq) — 适配包装（L842-849）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsArch3510<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsArch3510(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}
```

#### FixpipeL0cToOut — ISA 调用（L307-352，25 参数 copy_matrix_cc_to_gm）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToOut(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, const uint8_t cacheMode, uint16_t nIterIndex = 0)
{
    // ... offset calculation ...
    return copy_matrix_cc_to_gm(
        (__gm__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, cacheMode, 0, intriParams.unitFlag,
        static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn),
        intriParams.isChannelSplit, nz2ndEn, static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false,
        false, false, false, false, nz2dnEn);
}
```

---

### dav_l300（目标版本，3003）

#### V1 FixpipeL0C2GMImpl(V220) — 适配包装（L515-521）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(__gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, params);
}
```

#### V2 FixpipeL0C2GMImpl(V220, deq) — 适配包装（L523-530）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL300<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL300(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}
```

#### V1 本地 FixpipeL0C2GMImpl(L300) — 完整实现（L416-439）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL300<config.format>& intriParams)
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
    FixpipeL0cToOut<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}
```

#### V2 本地 FixpipeL0C2GMImpl(L300, deq) — 完整实现（L441-472）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL300<config.format>& intriParams)
{
    SetLoop3Para<config>(intriParams);
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48;
        set_channel_para(channelPara);
    }
    FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
    if (IsVectorQuantMode(intriParams.quantPre)) {
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2GMImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2GMImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
}
```

#### FixpipeL0cToOut — ISA 调用（L278-325，22 参数 copy_matrix_cc_to_gm）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToOut(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL300<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    // ... offset calculation ...
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
```

> **ISA 参数数量：** l300 `copy_matrix_cc_to_gm` = 22 参数（无 `cacheMode`，无 `nz2dnEn`）。

---

### dav_l310（目标版本，3103）

> **注意：** 3103 不在 intf.h 任何 arch 守卫中，用户无法调用 Fixpipe L0C→GM。以下实现虽存在但为**死代码**。

#### V1 FixpipeL0C2GMImpl(V220) — 适配包装（L522-528）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(__gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, params);
}
```

#### V2 FixpipeL0C2GMImpl(V220, deq) — 适配包装（L530-537）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, cbufWorkspace, params);
}
```

#### V1 本地 FixpipeL0C2GMImpl(L311) — 完整实现（L423-446）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL311<config.format>& intriParams)
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
    FixpipeL0cToOut<DstT, SrcT, config>(dst, src, intriParams, fixpipeTiling, intriParams.nSize);
}
```

#### V2 本地 FixpipeL0C2GMImpl(L311, deq) — 完整实现（L448-479）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(
    __gm__ DstT* dst, __cc__ SrcT* src, __cbuf__ uint64_t* cbufWorkspace,
    const FixpipeParamsL311<config.format>& intriParams)
{
    SetLoop3Para<config>(intriParams);
    if constexpr (config.format == CO2Layout::COLUMN_MAJOR) {
        uint64_t channelPara = static_cast<uint64_t>(intriParams.params.srcNzC0Stride) << 48;
        set_channel_para(channelPara);
    }
    FixpipeTiling fixpipeTiling = GenFixpipeTiling(intriParams.nSize);
    if (IsVectorQuantMode(intriParams.quantPre)) {
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2GMImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2GMImplN<DstT, SrcT, config>(
                dst, src, cbufWorkspace, intriParams, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
}
```

#### FixpipeL0cToOut — ISA 调用（L281-332，24 参数 copy_matrix_cc_to_gm）

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToOut(
    __gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsL311<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    // ... offset calculation, supports nz2dnEn for COLUMN_MAJOR ...
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
```

> **ISA 参数数量：** l310 `copy_matrix_cc_to_gm` = 24 参数（无 `cacheMode`，有 `nz2dnEn`）。

---

### dav_l311（目标版本，3113）

#### V1 FixpipeL0C2GMImpl(V220) — 适配包装（L522-528）

> 与 dav_l310 完全相同。

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2GMImpl(__gm__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2GMImpl<DstT, SrcT, config>(dst, src, params);
}
```

#### V2 FixpipeL0C2GMImpl(V220, deq) — 适配包装（L530-537）

> 与 dav_l310 完全相同。

#### V1 本地 FixpipeL0C2GMImpl(L311) — 完整实现（L423-446）

> 与 dav_l310 完全相同。

#### V2 本地 FixpipeL0C2GMImpl(L311, deq) — 完整实现（L448-479）

> 与 dav_l310 完全相同。

#### FixpipeL0cToOut — ISA 调用（L281-332，24 参数 copy_matrix_cc_to_gm）

> 与 dav_l310 完全相同（24 参数，有 `nz2dnEn`）。

---

### dav_5161（目标版本，5101）

#### V1/V2 FixpipeL0C2GMImpl — **未定义**

> dav_5161 的 `kernel_operator_fixpipe_impl.h` 仅 78 行，仅定义了 `SetFixPipeClipReluImpl`、`SetFixPipeAddrImpl`、`SetFixPipeConfigImpl`、`SetFixpipeNz2ndFlagImpl`、`SetFixpipePreQuantFlagImpl` 等配置函数。**完全没有定义** `FixpipeL0C2GMImpl`、`FixpipeL0C2L1Impl` 或 `FixpipeL0cToOut` 等数据搬运函数。
>
> 5101 架构在 intf.h 中看不到任何 Fixpipe L0C→GM 声明（不在任何 arch 守卫中），因此即使缺少实现也不会导致编译错误。

## 代码级实现差异分析

### 实现风格对比

| 特征 | dav_3510 | dav_l300 | dav_l310 | dav_l311 | dav_5161 |
|------|----------|----------|----------|----------|----------|
| V1 V220 可见 | ✓ | ✓ | ✗（未声明） | ✓ | ✗（未声明） |
| V2 V220 deq 可见 | ✓ | ✓ | ✗（未声明） | ✓ | ✗（未声明） |
| V7 Arch3510 可见 | ✓ | ✗ | ✗ | ✗ | ✗ |
| V8 Arch3510 deq 可见 | ✓ | ✗ | ✗ | ✗ | ✗ |
| 本地参数结构体 | `FixpipeParamsArch3510` | `FixpipeParamsL300` | `FixpipeParamsL311` | `FixpipeParamsL311` | N/A |
| AIC 守卫 | `if ASCEND_IS_AIC` | 无 | 无 | 无 | N/A |
| 参数检查 | `CheckFixpipeL0C2GMParam` | 无 | 无 | 无 | N/A |
| ISA `copy_matrix_cc_to_gm` 参数数 | 25（含 cacheMode + nz2dnEn） | 22（无 cacheMode，无 nz2dnEn） | 24（无 cacheMode，有 nz2dnEn） | 24（同 l310） | N/A |
| float 类型支持 | ✓（无 `if constexpr` 阻断） | ✗（`if constexpr` 阻断 float） | ✗（同 l300） | ✗（同 l300） | N/A |
| COLUMN_MAJOR (nz2dnEn) | ✓ | ✗（无 nz2dnEn 路径） | ✓ | ✓ | N/A |
| cacheMode 参数 | ✓（4/5 参数签名） | ✗（3/4 参数签名） | ✗（3/4 参数签名） | ✗（3/4 参数签名） | N/A |
| QuantMode 集合 | F322 系列 | QF162 系列 | QF162 系列 | QF162 系列 | N/A |
| CopyDeqTensorToFbuf burstLen | 64B | 128B | 64B | 64B | N/A |
| `FixpipeConfig::isToUB` | ✓（3510/5102 专属） | ✗ | ✗ | ✗ | ✗ |
| `FixpipeConfig::enableFixVal` | ✗（仅 5102） | ✗ | ✗ | ✗ | ✗ |
| L0C→UB 支持 | ✓（`FixpipeL0cToUB`） | ✗（桩函数） | ✗（桩函数） | ✗（桩函数） | ✗（未定义） |

### QuantMode 对比

| 版本 | IsVectorQuantMode 包含的模式 | IsScalarQuantMode 包含的模式 |
|------|---------------------------|---------------------------|
| l300/l310/l311 | VDEQF16, VQF162B8_PRE, VREQ8, VQF162S4_PRE, VREQ4, VDEQS16, VQF162S16_PRE | DEQF16, QF162B8_PRE, REQ8, QF162S4_PRE, REQ4, DEQS16, QF162S16_PRE |
| 3510 | VDEQF16, VQF322B8_PRE, VREQ8, VQS322BF16_PRE, VQF322F16_PRE, VQF322BF16_PRE, VQF322FP8_PRE, VQF322HIF8_PRE, VQF322HIF8_PRE_HYBRID, VQF322F32_PRE | DEQF16, QF322B8_PRE, REQ8, QS322BF16_PRE, QF322F16_PRE, QF322BF16_PRE, QF322FP8_PRE, QF322HIF8_PRE, QF322HIF8_PRE_HYBRID, QF322F32_PRE |

### 参数结构体全量对比

`FixpipeParamsM300` 和 `FixpipeParamsM310` 均为 `FixpipeParamsV220` 的 `using` 别名（`kernel_struct_fixpipe.h:235-236`），三者完全相同。四个结构体定义位置：

| 结构体 | 定义位置 | 使用版本 | 是否模板 |
|--------|---------|---------|---------|
| `FixpipeParamsV220`（= M300 = M310） | `kernel_struct_fixpipe.h:107-166` | 2201/3003/3113/3510/5102（V1/V2 变体） | ✗（非模板，NZ2ND 字段平铺） |
| `FixpipeParamsArch3510` | `kernel_struct_fixpipe.h:190-223` | 3510/5102（V7/V8 变体） | ✓ `template<CO2Layout format>` |
| `FixpipeParamsL300` | `dav_l300/kernel_operator_fixpipe_impl.h:104-131` | 3003（本地实现） | ✓ `template<CO2Layout format>` |
| `FixpipeParamsL311` | `dav_l310/l311/kernel_operator_fixpipe_impl.h:104-131` | 3103/3113（本地实现） | ✓ `template<CO2Layout format>` |

> `FixpipeParamsV220` 是公共 API 参数，用户直接构造。`FixpipeParamsArch3510` / `FixpipeParamsL300` / `FixpipeParamsL311` 是各版本内部使用的本地参数结构体，通过 `TransFixpipeParamsV220ToXXX` 从 V220 转换而来。

| 字段 | 类型 | V220 (=M300=M310) | Arch3510 | L300 | L311 |
|------|------|:---:|:---:|:---:|:---:|
| **基础搬运参数** | | | | | |
| `nSize` | `uint16_t` | ✓ | ✓ | ✓ | ✓ |
| `mSize` | `uint16_t` | ✓ | ✓ | ✓ | ✓ |
| `srcStride` | `uint16_t` | ✓ | ✓ | ✓ | ✓ |
| `dstStride` | `uint32_t` | ✓ | ✓ | ✓ | ✓ |
| **量化参数** | | | | | |
| `quantPre` | `QuantMode_t` | ✓ | ✓ | ✓ | ✓ |
| `deqScalar` | `uint64_t` | ✓ | ✓ | ✓ | ✓ |
| **ReLU 参数** | | | | | |
| `reluEn` | `bool` | ✓ | ✓ | ✓ | ✓ |
| `preReluMode` | `ReluMode` | ✗ | ✓ | ✗ | ✗ |
| `preClipReluMode` | `ClipReluMode` | ✗ | ✓ | ✗ | ✗ |
| `reluScalar` | `uint64_t` | ✗ | ✓ | ✗ | ✗ |
| `vectorRelu` | `uint64_t` | ✗ | ✓ | ✗ | ✗ |
| **控制标志** | | | | | |
| `unitFlag` | `uint8_t` | ✓ | ✓ | ✓ | ✓ |
| `isChannelSplit` | `bool` | ✓ | ✓ | ✓ | ✓ |
| `dualDstCtl` | `uint8_t` | ✗ | ✓ | ✓ | ✓ |
| `subBlockId` | `bool` | ✗ | ✓ | ✓ | ✓ |
| `fixShiftVal` | `uint8_t` | ✗ | ✓（`#ifdef CANN_ASC_ENABLE_FIX_SHIFT`） | ✗ | ✗ |
| **NZ2ND 参数（ROW_MAJOR）** | | | | | |
| 组织方式 | — | **平铺顶层** | **params 子结构** | **params 子结构** | **params 子结构** |
| `ndNum` | `uint16_t` | ✓ 顶层 | params.`ndNum` | params.`ndNum` | params.`ndNum` |
| `srcNdStride` | `uint16_t` | ✓ 顶层 | params.`srcNdStride` | params.`srcNdStride` | params.`srcNdStride` |
| `dstNdStride` | — | ✓ `uint16_t` 顶层 | ✓ `uint32_t`（Nz2NdParams 变体A） | ✓ `uint16_t`（Nz2NdParams 变体B） | ✓ `uint16_t`（Nz2NdParams 变体B） |
| `nz2ndEn` | `bool` | ✗ | ✗（变体A 无此字段） | ✓（变体B `params.nz2ndEn`） | ✓（变体B `params.nz2ndEn`） |
| `originalNSize` | `uint16_t` | ✗ | ✗（变体A 无此字段） | ✓（变体B `params.originalNSize`） | ✓（变体B `params.originalNSize`） |
| **NZ2DN 参数（COLUMN_MAJOR）** | | | | | |
| params 类型 | — | ✗（无模板，不支持） | `Nz2DnParams` | `uint8_t`（不支持） | `uint8_t`（不支持） |
| `dnNum` | `uint16_t` | ✗ | ✓（params.`dnNum`） | ✗ | ✗ |
| `srcNzMatrixStride` | `uint16_t` | ✗ | ✓（params.`srcNzMatrixStride`） | ✗ | ✗ |
| `dstDnMatrixStride` | `uint32_t` | ✗ | ✓（params.`dstDnMatrixStride`） | ✗ | ✗ |
| `srcNzC0Stride` | `uint16_t` | ✗ | ✓（params.`srcNzC0Stride`） | ✗ | ✗ |

> **`Nz2NdParams` 变体差异**：该结构体有 arch 守卫——3510/5102 用变体A（3 字段：`ndNum`/`srcNdStride`/`dstNdStride`，`dstNdStride` 为 `uint32_t`）；其他版本用变体B（5 字段：增加 `nz2ndEn`/`originalNSize`，`dstNdStride` 为 `uint16_t`）。定义于 `kernel_utils_struct_dma_params.h:37-77`。

### ISA 参数映射对比（copy_matrix_cc_to_gm）

| 参数位置 | 3510 (25 参数) | l300 (22 参数) | l310/l311 (24 参数) |
|---------|---------------|---------------|-------------------|
| 1-7 | dst, src, 0, calNSize, mSize, dstStride, srcStride | 同 | 同 |
| 8 | **cacheMode** | 0 | 0 |
| 9 | 0 | unitFlag | unitFlag |
| 10 | unitFlag | quantPre | quantPre |
| 11 | quantPre | reluEn | reluEn |
| 12 | reluEn | isChannelSplit | isChannelSplit |
| 13 | isChannelSplit | nz2ndEn | nz2ndEn |
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

### 实现模式总结

**dav_3510 — 最完整实现**
- 支持 V1/V2 (V220) 和 V7/V8 (Arch3510) 双路径
- V220 通过 `TransFixpipeParamsV220ToFixpipeParamsArch3510` 适配为 Arch3510 本地参数后委托
- 有 AIC 守卫、参数检查、cacheMode 支持、float 类型支持、COLUMN_MAJOR (nz2dnEn) 支持
- ISA 25 参数（最全）

**dav_l300 — 精简实现（V220 only）**
- 仅支持 V1/V2 (V220)，无 Arch3510 路径
- 使用本地 `FixpipeParamsL300` 结构体（无 preReluMode 等扩展字段）
- 无 AIC 守卫、无参数检查
- ISA 22 参数（无 cacheMode，无 nz2dnEn），不支持 COLUMN_MAJOR
- float 类型通过 `if constexpr` 阻断
- CopyDeqTensorToFbuf burstLen=128B（独有）

**dav_l310 / dav_l311 — 中间实现（V220 only，代码完全相同）**
- 仅支持 V1/V2 (V220)，无 Arch3510 路径
- 使用本地 `FixpipeParamsL311` 结构体（字段顺序与 l300 略有差异）
- 无 AIC 守卫、无参数检查
- ISA 24 参数（无 cacheMode，有 nz2dnEn），支持 COLUMN_MAJOR
- float 类型通过 `if constexpr` 阻断
- CopyDeqTensorToFbuf burstLen=64B
- **l310 的实现对 3103 为死代码**（intf.h 未声明）

**dav_5161 — 无实现**
- 完全未定义 Fixpipe 数据搬运函数
- 5101 不在 intf.h 任何 arch 守卫中

### 关键差异点

1. **3103 完全不可见** — 3103 不在 V220 守卫（`2201||3003||3113||3510||5102`）中，也不是 M310（3102），也不在 Arch3510 守卫中。intf.h 中无任何 Fixpipe L0C→GM 声明对 3103 可见。但 l310 impl 文件中存在完整实现（死代码）。

2. **5101 完全不可见** — 同理 5101 不在任何守卫中，且 impl 文件中无任何 Fixpipe 数据搬运实现。

3. **3510 双路径** — 3510 同时可见 V220 和 Arch3510 变体，两套重载通过不同参数类型（`FixpipeParamsV220` vs `FixpipeParamsArch3510`）区分。

4. **ISA 参数数量差异** — l300=22 参数，l310/l311=24 参数，3510=25 参数。差异来源：`cacheMode`（仅 3510 有）和 `nz2dnEn`（l310/l311/3510 有，l300 无）。

5. **float 类型支持** — l300/l310/l311 通过 `if constexpr` 阻断 float 类型的 L0C→GM 拷贝（assert 报错），3510 无此限制。

6. **COLUMN_MAJOR 支持** — l300 不支持（无 nz2dnEn 路径，COLUMN_MAJOR 的 dstOffset 回退到 NZ 模式），l310/l311/3510 支持。

7. **QuantMode 体系不同** — l300/l310/l311 使用 QF162 系列量化模式（如 `QF162B8_PRE`），3510 使用 F322/QF322 系列量化模式（如 `QF322B8_PRE`）。

## 待确认问题

1. **3103 是否应该支持 Fixpipe L0C→GM？** — l310 impl 文件中有完整的 `FixpipeL0C2GMImpl` 实现（V220 适配 + L311 本地），但 intf.h 的 V220 arch 守卫不含 3103。这是遗漏（应在守卫中添加 3103）还是有意排除（3103 硬件不支持）？

2. **5101 是否应该支持 Fixpipe L0C→GM？** — 5161 impl 文件中完全没有 Fixpipe 数据搬运函数。5101 硬件是否支持 `copy_matrix_cc_to_gm` 指令？如不支持，当前状态（intf.h 不声明 + impl 不定义）是正确的。如支持，需要补充实现。

3. **intf_impl.h V220 包装无 arch 守卫** — intf_impl.h L110-188 的 V220 包装模板无 arch 守卫，在 3103/5101 上也会被编译。由于 intf.h 未声明对应 `Fixpipe` 函数，模板不会被实例化，因此不会报错。但这种"impl 有代码但 intf 不暴露"的模式是否是设计意图？

4. **intf_impl.h Arch3510 包装内层 `#else` 分支** — L255-262/L281-290 的内层 `#if 3510||5102` / `#else` 中，`#else` 调用 3 参数版 `FixpipeL0C2GMImpl`（不带 cacheMode）。但外层守卫已限制为 `3510||5102`，`#else` 永远不会执行。这是为未来扩展预留的代码模式，还是有其他用途？

5. **l300 不支持 COLUMN_MAJOR** — l300 的 `FixpipeParamsL300` 的 COLUMN_MAJOR 特化使用 `uint8_t` 作为 PARAMS 类型（而非 `Nz2DnParams`），且 `FixpipeL0cToOut` 中无 COLUMN_MAJOR 的 nz2dnEn 路径。这是硬件限制还是待补全？

6. **l300 不支持 float L0C→GM** — l300/l310/l311 的 `FixpipeL0cToOut` 中通过 `if constexpr` 阻断 float 类型，并在 `#else` 分支 assert 报错。这是硬件限制还是待补全？

7. **CopyDeqTensorToFbuf burstLen 差异** — l300 使用 128B burstLen，l310/l311/3510 使用 64B。这是硬件差异还是代码遗留？

8. **l310 与 l311 完全相同** — l310 和 l311 的 `kernel_operator_fixpipe_impl.h` 文件内容完全相同（仅文件路径和 copyright 年份不同）。是否应该合并或通过宏区分？
