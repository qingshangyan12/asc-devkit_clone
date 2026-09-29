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

## V7/V8（Arch3510 变体）目标版本支持可行性分析

> 本节分析 3003/3103/3113 能否支持 V7 (L170) / V8 (L175)（`FixpipeParamsArch3510` 参数的 L0C→GM 变体）。5101 经分析**不可行**（架构性缺失，详见下方「5101（dav_5161）可能性分析」小节）。
>
> **注：** 本节基于 V1/V2 适配后的代码状态（dav_l310 已完成 `ParamsL311`→`ParamsL310` 重命名）；前文各节为适配前快照（dav_l310 沿用 L311 命名）。本节行号均为当前实际行号。

### 结论总览

| 版本 | 可行性 | 限制 |
|------|--------|------|
| 3003 (dav_l300) | **部分可行** | 仅 NZ / ROW_MAJOR；COLUMN_MAJOR (NZ2DN) 不可行 |
| 3103 (dav_l310) | **基本可行** | COLUMN_MAJOR 需将 `TransformParamsL310<COLUMN_MAJOR>::PARAMS` 从 `uint8_t` 改为 `Nz2DnParams` |
| 3113 (dav_l311) | **基本可行** | 同 3103（`TransformParamsL311`，行号与 l310 一致） |
| 5101 (dav_5161) | **不可行** | 架构性缺失：无 Cube/L0C 通路、无 `copy_matrix_cc_to_gm` 内建（详见下方专节） |

**3003 COLUMN_MAJOR 三重阻断**（改 PARAMS 也无法解决）：

1. 22 参数 `copy_matrix_cc_to_gm` 无 `nz2dnEn` 槽位（l310/l311 为第 24 参数，3510 为第 25 参数）；
2. l300 `FixpipeL0cToOut`（L278-325）dstOffset 分支仅有 ROW_MAJOR（L300-302）/ else-NZ（L303-305），无 COLUMN_MAJOR 分支，函数内无 `nz2dnEn` 变量；
3. `TransformParamsL300<COLUMN_MAJOR>::PARAMS = uint8_t`（L98-102）。

**3103/3113 COLUMN_MAJOR 条件可行**——所需代码全部存在，仅 PARAMS 类型阻断：

1. 24 参数 ISA 有 `nz2dnEn` 槽位（l310 `FixpipeL0cToOut` L322-327 的 `copy_matrix_cc_to_gm`，`copy_matrix_cc_to_cbuf` 同）；
2. COLUMN_MAJOR 分支已存在（当前死代码，`if constexpr` 弃置分支不编译）：`SetLoop3Para`（l310 L210-218）、`GetGMLen`（L270-274）、`FixpipeL0cToOut`（L307-309）、`FixpipeL0C2L1Impl`（L368-372）、`FixpipeL0C2GMImpl`（L430-433）；
3. 将 `TransformParamsL310/L311<COLUMN_MAJOR>::PARAMS` 改为 `Nz2DnParams` 即激活上述全部路径（`Nz2DnParams` 无条件定义于 `kernel_utils_struct_dma_params.h`，字段 `dnNum`/`srcNzMatrixStride`/`dstDnMatrixStride`/`srcNzC0Stride` 与各分支引用完全匹配）。

### 有利因素（代码已预留扩展点）

1. **intf_impl.h 内层 `#else` 已预接线** — V7/V8 包装（L240-291）内层 `#if 3510||5102`（cacheMode 4/5 参数）/ `#else`（无 cacheMode 3/4 参数，L255/L281）已按目标版本 ISA 形态写好调用；外层守卫扩展后包装层立即正确工作。
2. **`FixpipeParamsArch3510` 可在所有架构实例化** — 公共模板（`kernel_struct_fixpipe.h:190-223`），无 arch 守卫；其 `params` 字段在目标版本上：NZ=`uint8_t`、ROW_MAJOR=`Nz2NdParams`（变体B，与本地结构体同型）、COLUMN_MAJOR=`Nz2DnParams`。
3. **V7/V8 包装不使用 `config.isToUB`** — `isToUB` 仅被 L0C→L1/UB 包装（intf_impl.h L192-237）使用，GM 变体不受该字段缺失影响。
4. **deq 机制齐备** — V8 所需 `GenFixpipeTiling`/`FixpipeL0C2GMImplN`/`CopyDeqTensorToFbuf` 在 l300/l310/l311 均已定义。
5. **无重载冲突** — `FixpipeParamsArch3510` 与 `FixpipeParamsV220`/`FixpipeParamsL310` 类型互异，V7/V8 适配器重载可与现有 V220 适配器、本地实现共存（3510 即 V1-V8 八重载并存）。

### 阻碍因素（必须处理）

1. **守卫必须拆分而非整体扩展**：
   - **intf.h**：Arch3510 块（L157 起，守卫 `!defined||3510||5102`）同时含 V7/V8 声明（L170-175）与 L0C→L1/UB 声明；后者对应包装使用 `config.isToUB`（仅 3510/5102 存在，`kernel_struct_fixpipe.h:51-53`），整体扩展将导致目标版本编译错误。须将 V7/V8 声明拆为独立 `#if`（守卫 `3510||5102||3003||3103||3113`），L1/UB 声明保持原守卫。
   - **intf_impl.h**：外层守卫（L190）同理——L240-291（V7/V8 包装）可扩展，L192-237（L1/UB 包装）保持 `3510||5102`。
2. **Impl 缺 Arch3510 参数重载** — l300/l310/l311 仅有 `FixpipeParamsV220`（适配器）与 `FixpipeParamsL300/L310`（本地）两族 `FixpipeL0C2GMImpl`，无 `FixpipeParamsArch3510` 版本。需新增 `TransFixpipeParamsArch3510ToFixpipeParamsL300/L310` + 3/4 参数适配器（参照 dav_3510 的 V220 适配器 L834-849，方向相反）。字段映射：基础字段 + `subBlockId`/`dualDstCtl`/`params`（目标版本上类型相同，直接拷贝）逐一复制；裁剪 `preReluMode`/`preClipReluMode`/`reluScalar`/`vectorRelu`/`fixShiftVal`——GM 路径的 `SetLoop3Para` 与 `copy_matrix_cc_to_gm` ISA 参数均不消费这些字段（3510 `FixpipeL0cToOut` L307-352 的 ISA 参数列表可证），裁剪安全。
3. **QuantMode 体系不兼容** — Arch3510 用户可传 QF322 系列模式；目标版本 `IsScalarQuantMode`/`IsVectorQuantMode`（l310 L162-176）仅识别 QF162 系列，QF322 模式静默无效（不报错）。

### 语义差异（移植后行为变化）

| 项 | 3510 | 目标版本 (3003/3103/3113) | 影响 |
|----|------|--------------------------|------|
| `dstNdStride` | `uint32_t`（Nz2NdParams 变体A） | `uint16_t`（变体B） | 大值截断风险 |
| QuantMode | F322/QF322 系列 | 仅 QF162 系列有效 | QF322 静默无效 |
| float L0C→GM | 支持 | `if constexpr` 阻断（l310 L321/329-331） | float 调用编译期报错 |
| AIC 守卫/参数检查 | 有 | 无 | 仅防御性差异 |
| `preReluMode`/`reluScalar`/`vectorRelu` | 结构体含 | Trans 裁剪丢弃 | 扩展 ReLU 功能不可用 |

### 工作量估算

| # | 修改项 | 文件 | 说明 |
|---|--------|------|------|
| 1 | intf.h 守卫拆分 | `kernel_operator_fixpipe_intf.h` | V7/V8 声明拆独立 `#if`（`3510\|\|5102\|\|3003\|\|3103\|\|3113`）；L1/UB 声明保持原守卫 |
| 2 | intf_impl.h 守卫拆分 | `kernel_operator_fixpipe_intf_impl.h` | L240-291 外层守卫扩展；L192-237 保持 `3510\|\|5102` |
| 3 | Trans 适配器 + Arch3510 重载 | `dav_l300/l310/l311 kernel_operator_fixpipe_impl.h` | `TransFixpipeParamsArch3510ToFixpipeParamsL300/L310` + 3/4 参数 `FixpipeL0C2GMImpl(FixpipeParamsArch3510)` |
| 4 | COLUMN_MAJOR PARAMS 修正 | `dav_l310/l311` | `TransformParamsL310/L311<COLUMN_MAJOR>::PARAMS`: `uint8_t` → `Nz2DnParams`（激活死代码） |
| 5 | 3003 COLUMN_MAJOR 阻断 | `dav_l300` | static_assert / 编译期报错（22 参数 ISA 无 nz2dnEn） |

**修改项 4 的溢出效应：**

- `TransformParamsL310/L311` 同时被本地 `FixpipeParamsL310/L311` 使用——改 PARAMS 后本地 COLUMN_MAJOR 实例化亦被激活（L1 与 GM 路径的 COLUMN_MAJOR 分支均存在，见上）。属预期效果。
- **遗留问题（不引入回归）：** `TransFixpipeParamsV220ToFixpipeParamsL310` 的 COLUMN_MAJOR 分支（l310 L498-502）写 `params.ndNum`/`dstNdStride`/`srcNdStride`，与 `Nz2DnParams` 字段（`dnNum`/`srcNzMatrixStride`/`dstDnMatrixStride`）不匹配——该分支在当前 `uint8_t` PARAMS 下同样不可实例化（`uint8_t` 无成员），即 V1/V2 + COLUMN_MAJOR 路径今日已不可用；改动不使其恶化。V7/V8 使用新写的 Trans（正确映射 `dnNum` 等），不受影响。
- l300 同类死代码（`SetLoop3Para` L210-218、`GetGMLen` L267-272、`FixpipeL0C2L1Impl` L361-365）即使改 PARAMS，GM 路径仍缺 dstOffset 分支与 ISA 槽位，3003 的 GM COLUMN_MAJOR 仍不可行（故修改项 5 必要）。

### 5101（dav_5161）可能性分析：不可行（架构性缺失）

> 结论：**5101 无法支持 V7/V8**（连带 V1/V2 及一切 L0C→GM/L1 的 Fixpipe）。与 3003/3103/3113 的"守卫/适配器缺失"（代码已存在约 90%，属工程问题）性质完全不同，5101 是**架构性缺失**——整个芯片无 Cube/L0C 数据通路。

**证据链（四层）：**

1. **无 L0C 数据通路（impl 层全为桩）：**
   - Mmad：dav_5161 全部 4 个 `MmadCal` 重载均为桩（`"Mmad is not supported!"`，`kernel_operator_mm_impl.h:346-370`）——Cube 矩阵单元不可用，L0C 累加器无数据生产者；
   - DataCopy：所有 L0C 出入口均为 not-support 桩（`dav_5161/kernel_operator_data_copy_impl.h:725-756`）：`DataCopyL12L0C` / `DataCopyL0C2UB` / `DataCopyUB2L0C` / `DataCopyL0C2L1` / `DataCopyL0C2GM`；
   - Fixpipe：impl 仅 106 行，无 `FixpipeL0C2GMImpl`/`FixpipeL0C2L1Impl`/`FixpipeL0cToOut`，无任何 `copy_matrix_*` 调用，仅剩 SPR 配置函数（`set_m_fpc`/`set_m_quant_pre`/`set_m_clip_relu`）。
2. **ISA 缺失：** `copy_matrix_cc_to_gm`/`copy_matrix_cc_to_cbuf` 为编译器内建（仓库内无任何声明；现代 C API 封装 `asc_copy_l0c2gm` 仅实现 2201/3510）。全仓库调用方仅 3003（22 参数）/3103/3113（24）/3510（25）/5102（25）；5161 家族 SDK **从不调用任何 `__cc__`（L0C）源 DMA 内建**，无证据表明 5101 编译器提供该指令。
3. **5101 的 fixpipe 等价路径在 L1 而非 L0C：** 5161 家族的后处理搬运为 `fix_cbuf_to_gm`（L1→GM，5101 为 8 参数，5161/5165/5163 为 9 参数）+ `fix_cbuf_to_ubuf`（L1→UB，经 `DataCopyL1ToUB` 暴露，3510/3003/3103/3113/5101 可见）+ SPR `set_fixp_nz_para`/`set_m_fpc`/`set_m_quant_pre`，经 `DataCopyPadL12GM` 等路径提供 NZ2ND/NZ2DN/NORMAL_DMA 模式。其语义是 **L1 源**后处理，与 V7/V8 的 L0C 源语义不可互换——即便包装成同名 API 也是语义造假。
4. **家族对照：** 同为 510x 家族的 **5102（dav_m510）完整支持 V7/V8**——957 行 fixpipe impl，直接使用 `FixpipeParamsArch3510`（无本地参数结构体），`FixpipeL0C2GMImpl` 含 cacheMode 重载（L871-874/L918-921），25 参数 `copy_matrix_cc_to_gm`（L435-448）。家族内 Cube 能力分界清晰：5102 有、5101 无（向量核取向）。另：`set_m_clip_relu` 仅 5161/5165/5163 可用，5101 连 ClipRelu 配置都是 NOT_SUPPORT 桩（fixpipe impl L29-37 的 `#if 5101` 分支）——组内 5101 还是最低配。

**因此：** 当前"intf.h 不声明 + impl 不定义/打桩"对 5101 是**正确设计**，无需（也无法）适配。若需 510x 家族的 Arch3510 参数风格 Fixpipe，应使用 5102。5161/5165/5163 与 5101 共用 dav_5161，结论相同。

## 待确认问题

1. **3103 是否应该支持 Fixpipe L0C→GM？** — l310 impl 文件中有完整的 `FixpipeL0C2GMImpl` 实现（V220 适配 + L311 本地），但 intf.h 的 V220 arch 守卫不含 3103。这是遗漏（应在守卫中添加 3103）还是有意排除（3103 硬件不支持）？

2. **5101 是否应该支持 Fixpipe L0C→GM？** — 5161 impl 文件中完全没有 Fixpipe 数据搬运函数。5101 硬件是否支持 `copy_matrix_cc_to_gm` 指令？如不支持，当前状态（intf.h 不声明 + impl 不定义）是正确的。如支持，需要补充实现。**→ 已分析：不可行，当前状态正确**（详见"V7/V8 可行性分析"节末小节：无 Cube/L0C 通路、无 `copy_matrix_cc_to_gm` 内建，5101 的 fixpipe 等价路径为 L1 源 `fix_cbuf_to_gm`/`fix_cbuf_to_ubuf`）。

3. **intf_impl.h V220 包装无 arch 守卫** — intf_impl.h L110-188 的 V220 包装模板无 arch 守卫，在 3103/5101 上也会被编译。由于 intf.h 未声明对应 `Fixpipe` 函数，模板不会被实例化，因此不会报错。但这种"impl 有代码但 intf 不暴露"的模式是否是设计意图？

4. **intf_impl.h Arch3510 包装内层 `#else` 分支** — L255-262/L281-290 的内层 `#if 3510||5102` / `#else` 中，`#else` 调用 3 参数版 `FixpipeL0C2GMImpl`（不带 cacheMode）。但外层守卫已限制为 `3510||5102`，`#else` 永远不会执行。这是为未来扩展预留的代码模式，还是有其他用途？

5. **l300 不支持 COLUMN_MAJOR** — l300 的 `FixpipeParamsL300` 的 COLUMN_MAJOR 特化使用 `uint8_t` 作为 PARAMS 类型（而非 `Nz2DnParams`），且 `FixpipeL0cToOut` 中无 COLUMN_MAJOR 的 nz2dnEn 路径。这是硬件限制还是待补全？

6. **l300 不支持 float L0C→GM** — l300/l310/l311 的 `FixpipeL0cToOut` 中通过 `if constexpr` 阻断 float 类型，并在 `#else` 分支 assert 报错。这是硬件限制还是待补全？

7. **CopyDeqTensorToFbuf burstLen 差异** — l300 使用 128B burstLen，l310/l311/3510 使用 64B。这是硬件差异还是代码遗留？

8. **l310 与 l311 完全相同** — l310 和 l311 的 `kernel_operator_fixpipe_impl.h` 文件内容完全相同（仅文件路径和 copyright 年份不同）。是否应该合并或通过宏区分？
