# Fixpipe（M300/M310 L0C→L1/UB 变体）跨版本分析报告

> 分析对象：`include/basic_api/kernel_operator_fixpipe_intf.h` 中 M300/M310 专属的 4 个 L0C→L1/UB `Fixpipe` 声明变体（用户原始行号 107/112/133/138，对应模板声明起始行；因本仓库早前对 L63-64 的宏控做过同步修改，L68 之后行号 +1，当前文件中对应注释行 L107/L112/L133/L138，声明体 L108-110/L113-118/L134-136/L139-144）。
>
> 目标版本：3003（dav_l300）、3103（dav_l310）、3113（dav_l311）、5101（dav_5161）；参考版本：3510（dav_3510）；原生版本（声明宿主）：3002（dav_m300）、3102（dav_m310）。
>
> L0C→GM 变体已在 `Fixpipe(L0C-GM)-analysis.md` 中分析，本报告不重复。

---

## 1. 接口概述

### 1.1 四个声明变体

| 编号 | 位置（当前行号） | 签名要点 | 宏控 |
|---|---|---|---|
| V3' M300 L0C→L1/UB | intf.h L107-110 | `Fixpipe(const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsM300& intriParams)`，`config = CFG_ROW_MAJOR` | L106 `#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3002` |
| V4' M300 L0C→L1/UB deq | intf.h L112-118 | 追加 `const LocalTensor<S>& cbufWorkspace`，SFINAE `Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true`，`typename S = uint64_t` | 同上 |
| V5' M310 L0C→L1/UB | intf.h L133-136 | 同 V3'，参数类型 `FixpipeParamsM310` | L132 `#elif defined(__NPU_ARCH__) && __NPU_ARCH__ == 3102` |
| V6' M310 L0C→L1/UB deq | intf.h L138-144 | 同 V4'，参数类型 `FixpipeParamsM310` | 同上 |

### 1.2 参数类型为 V220 别名（关键事实）

`include/basic_api/kernel_struct_fixpipe.h`：
- L235：`using FixpipeParamsM300 = FixpipeParamsV220;`
- L236：`using FixpipeParamsM310 = FixpipeParamsV220;`

即 M300/M310 的 4 个声明与 intf.h L77-104 的 V220 声明（宏控 `2201 || 3003 || 3113 || 3510 || 5102`）**签名完全相同**，是纯粹的按架构冗余声明。M300/M310 声明调用的并非各自专属实现入口，而是共享的无宏控 V220 包装（见第 2 章）。

### 1.3 声明可见性结论

4 个变体的宏控只放行 3002/3102。目标版本（3003/3103/3113/5101）与参考版本（3510）均**不可见**这些声明；其中 3003/3113/3510 可见签名等价的 V220 声明，3103/5101 连 V220 声明也不可见。

---

## 2. 调用链分析

### 2.1 完整链路（以 3002 上的 V3' 为例）

```
用户调用 Fixpipe(dstLocalTensor, srcLocalTensor, fixpipeParamsM300)
  │
  ├─ [声明] kernel_operator_fixpipe_intf.h L108-110（宏控 __NPU_ARCH__ == 3002）
  │    FixpipeParamsM300 = FixpipeParamsV220（别名）
  │
  ├─ [共享包装] impl/basic_api/kernel_operator_fixpipe_intf_impl.h L109-125
  │    Fixpipe(LocalTensor, LocalTensor, FixpipeParamsV220)   ← 无架构宏控
  │    · ASCENDC_DEBUG || ASCENDC_CPU_DEBUG 时 CheckFixpipeTensor
  │    · 按 dst 物理位置分发：L1 → FixpipeL0C2L1Impl；UB → FixpipeL0C2UBImpl
  │
  ├─ [版本实现] impl/basic_api/dav_m300/kernel_operator_fixpipe_v2_impl.h
  │    · FixpipeL0C2L1Impl(__cbuf__ T*, __cc__ U*, FixpipeParamsM300)  L496-517（3 参，完整）
  │    · FixpipeL0C2L1Impl(+ cbufWorkspace)                            L519-546（4 参，仅向量量化）
  │    · FixpipeL0C2UBImpl(__ubuf__ T*, __cc__ U*, FixpipeParamsM300)  L409-429（3 参，完整）
  │    · FixpipeL0C2UBImpl(+ cbufWorkspace)                            L431-458（4 参，仅向量量化）
  │    · 辅助：SetLoop3Para L67-74 / AdaptFixpipeParams L296-311 / CopyDeqTensorToFbuf L342-356
  │
  ├─ [ISA 封装] 同文件
  │    · FixpipeL0cToL1 L474-494 → copy_matrix_cc_to_cbuf（14 参数）
  │    · FixpipeL0cToUB L387-407 → copy_matrix_cc_to_ub（14 参数）
  │
  └─ [编译器内联函数] copy_matrix_cc_to_cbuf / copy_matrix_cc_to_ub
        附属 SPR：set_loop3_para / set_quant_pre / set_fpc / copy_cbuf_to_fbuf
```

deq 变体（V4'/V6'）走 intf_impl.h L127-147 的 4 参包装，多传 `(__cbuf__ uint64_t*)cbufWorkspace.GetPhyAddr()`，落入各版本 4 参 `FixpipeL0C2*Impl`。

### 2.2 共享层分发逻辑

1. **include 分发**（intf_impl.h L30-54，`#elif` 链，按 `__NPU_ARCH__` 选择版本实现文件）：

   | 行号 | 宏控条件 | 引入文件 |
   |---|---|---|
   | L37-39 | `3002` | dav_m300/kernel_operator_fixpipe_impl.h **+** dav_m300/kernel_operator_fixpipe_v2_impl.h（双文件） |
   | L40-41 | `3102` | dav_m310/kernel_operator_fixpipe_impl.h（单文件） |
   | L42-43 | `3510` | dav_3510/kernel_operator_fixpipe_impl.h |
   | L46-47 | `3003` | dav_l300/kernel_operator_fixpipe_impl.h |
   | L48-49 | `3113` | dav_l311/kernel_operator_fixpipe_impl.h |
   | L50-51 | `3103` | dav_l310/kernel_operator_fixpipe_impl.h |
   | L52-53 | `5101/5161/5165/5163` | dav_5161/kernel_operator_fixpipe_impl.h |

2. **dst 位置分发**（包装内，运行期/编译期混合）：`GetPhyType(dst.GetPosition()) == Hardware::L1` → `FixpipeL0C2L1Impl`；`== Hardware::UB` → `FixpipeL0C2UBImpl`。两个分支都不是该硬件能力时静默无操作。

3. **重载解析**：包装以 `FixpipeL0C2*Impl<PrimT<T>, PrimT<U>, config>(...)` 显式三模板实参调用。3002 上 `FixpipeParamsM300` 与包装形参 `FixpipeParamsV220` 为同一类型，直接命中 dav_m300 v2_impl 的实现；3102 上同理命中 dav_m310 的 `FixpipeParamsM310` 版本。

### 2.3 各层宏控记录

| 层 | 文件:行号 | 宏控 | 说明 |
|---|---|---|---|
| 声明 V3'/V4' | intf.h L106 | `__NPU_ARCH__ == 3002` | 仅 3002 可见 |
| 声明 V5'/V6' | intf.h L132 | `__NPU_ARCH__ == 3102` | 仅 3102 可见 |
| 等效 V220 声明 | intf.h L77-78 | `2201 \|\| 3003 \|\| 3113 \|\| 3510 \|\| 5102` | 3002/3102/3103/5101 不可见 |
| Arch3510 专属声明 | intf.h L158 | `!defined(__NPU_ARCH__) \|\| 3510 \|\| 5102` | 参数为 `FixpipeParamsArch3510<config.format>`，不在本报告范围 |
| 包装（2 个） | intf_impl.h L109-147 | 无 | 所有架构编译 |
| include 分发 | intf_impl.h L30-54 | `#elif __NPU_ARCH__` 链 | 选择版本实现文件 |
| 版本实现 | 各 dav_*/kernel_operator_fixpipe_impl.h | 无函数级宏控 | 由 include 链保证单架构编译 |
| intf.h 尾部 | intf.h L183-185 | `#if defined(__NPU_ARCH__)` 才 include intf_impl.h | 非硬件编译（如接口提取）不含实现 |

---

## 3. 支持矩阵

### 3.1 声明可见性（4 个目标变体 + 等效 V220）

| 版本 | V3' M300 L1/UB | V4' M300 deq | V5' M310 L1/UB | V6' M310 deq | 等效 V220 L1/UB 声明 | Arch3510 专属声明 |
|---|---|---|---|---|---|---|
| 3002 | ✔ | ✔ | ✘ | ✘ | ✘ | ✘ |
| 3102 | ✘ | ✘ | ✔ | ✔ | ✘ | ✘ |
| 3003 | ✘ | ✘ | ✘ | ✘ | ✔ | ✘ |
| 3103 | ✘ | ✘ | ✘ | ✘ | **✘** | ✘ |
| 3113 | ✘ | ✘ | ✘ | ✘ | ✔ | ✘ |
| 5101 | ✘ | ✘ | ✘ | ✘ | **✘** | ✘ |
| 3510（参考） | ✘ | ✘ | ✘ | ✘ | ✔ | ✔ |

### 3.2 底层 L0C→L1/UB 能力（经等效 V220 路径或原生路径）

| 版本 | L0C→L1 | L0C→UB | 依据（文件:行号） |
|---|---|---|---|
| 3002 | **完整** | **完整** | dav_m300 v2_impl L496-546（L1）、L409-458（UB）；另有老 API 完整双路径（impl.h L593-662） |
| 3102 | **完整** | **完整** | dav_m310 L458-520（L1）、L370-431（UB） |
| 3003 | 完整（V220 适配器） | **Stub** | dav_l300 L498-513（L1 适配器→本地 L300 路径 L353-414）；L532-543 UB stub（ASCENDC_ASSERT） |
| 3103 | 实现存在但为**死代码** | 同左 | dav_l310 L505-520/L539-550 实现齐全，但 3103 上无任何 Fixpipe 声明可见 |
| 3113 | 完整（V220 适配器） | **Stub** | dav_l311 L505-520（适配器）、L539-550（UB stub）；文件与 dav_l310 逐行一致 |
| 5101 | **未定义** | **未定义** | dav_5161 全文 86 行仅含 Set* SPR stub，无任何 Fixpipe 实现 |
| 3510（参考） | **完整** | **完整** | dav_3510 L572-640（L1）、L642-708（UB）原生 Arch3510 实现 + L800-832 V220 适配器 |

### 3.3 编译器 API（最低层）汇总

| 版本 | L1 路径 | UB 路径 | 附属 SPR/intrinsic |
|---|---|---|---|
| 3002 | `copy_matrix_cc_to_cbuf`（14 参） | `copy_matrix_cc_to_ub`（14 参） | `set_loop3_para`、`set_quant_pre`（经 SetQuantPreImpl）、`set_fpc`、`copy_cbuf_to_fbuf`、老路径另有 `set_nd_para`（SetNdParaImpl） |
| 3102 | `copy_matrix_cc_to_cbuf`（14 参） | `copy_matrix_cc_to_ub`（14 参） | `set_loop3_para`、`set_quant_pre`、`set_fpc`、`copy_cbuf_to_fbuf` |
| 3003 | `copy_matrix_cc_to_cbuf`（22 参，含 `QuantMode_post::NoConv` 等扩展参数） | 无（UB stub） | `set_loop3_para`、`set_channel_para`（COLUMN_MAJOR）、`set_quant_pre`、`set_fpc`、`copy_cbuf_to_fbuf` |
| 3103/3113 | `copy_matrix_cc_to_cbuf`（24 参，尾部多 `nz2dnEn`） | 无（UB stub；死代码） | 同 3003（`fbufBurstLenUnit=64`） |
| 5101 | 无 | 无 | 仅 `set_m_fpc`、`set_m_quant_pre`、`set_m_clip_relu`（M 系列 SPR） |
| 3510（参考） | `copy_matrix_cc_to_cbuf`（25 参） | `copy_matrix_cc_to_ub`（26 参，含 `dualDstCtl`、`subBlockId` 双目的模式） | `set_loop3_para`、`set_channel_para`、`set_quant_pre`、`set_fpc`、`copy_cbuf_to_fbuf` |

---

## 4. 各版本原始代码

### 4.0 声明与共享包装（所有链路共用）

**intf.h L106-118（M300 段，宏控 L106）**：
```cpp
#if defined(__NPU_ARCH__) && __NPU_ARCH__ == 3002
// L0C->L1/UB
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsM300& intriParams);

// L0C->L1/UB deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsM300& intriParams);
```

**intf.h L132-144（M310 段，宏控 L132）**：
```cpp
#elif defined(__NPU_ARCH__) && __NPU_ARCH__ == 3102
// L0C->L1/UB
template <typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const FixpipeParamsM310& intriParams);

// L0C->L1/UB deq tensor quant
template <
    typename T, typename U, const FixpipeConfig& config = CFG_ROW_MAJOR, typename S = uint64_t,
    typename Std::enable_if<Std::is_same<PrimT<S>, uint64_t>::value, bool>::type = true>
__aicore__ inline void Fixpipe(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const LocalTensor<S>& cbufWorkspace,
    const FixpipeParamsM310& intriParams);
```

**intf_impl.h L109-147（无架构宏控的 V220 包装，M300/M310 声明实际绑定于此）**：
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

---

### 4.1 3002（dav_m300，原生 M300）

V220/M300 路径位于 `dav_m300/kernel_operator_fixpipe_v2_impl.h`（672 行）。M300 声明（FixpipeParamsM300）实际落到此文件。

**SetLoop3Para（L67-74）**：
```cpp
__aicore__ inline void SetLoop3Para(const FixpipeParamsM300& intriParams)
{
    ASCENDC_ASSERT((intriParams.ndNum > 0), { KERNEL_LOG(KERNEL_ERROR, "ndNum must be larger than 0"); });
    uint64_t loop3Para = static_cast<uint64_t>(intriParams.dstNdStride) << 32; // LOOP3_PARA[63:32]
    loop3Para |= static_cast<uint64_t>(intriParams.srcNdStride) << 16;         // LOOP3_PARA[31:16]
    loop3Para |= static_cast<uint64_t>(intriParams.ndNum);                     // LOOP3_PARA[15:0]
    set_loop3_para(loop3Para);
}
```

**AdaptFixpipeParams（L296-311，单位换算）**：
```cpp
// adaptation due to unit differences;
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline FixpipeParamsM300 AdaptFixpipeParams(const FixpipeParamsM300& intriParams)
{
    FixpipeParamsM300 params(intriParams);
    if constexpr (config.format == CO2Layout::NZ) {
        params.dstStride = intriParams.dstStride * ONE_BLK_SIZE / sizeof(T);
    }

    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        constexpr uint16_t oriUnit = 1024;
        constexpr uint16_t multiples = oriUnit / (BLOCK_CUBE * sizeof(U));
        params.srcNdStride = params.srcNdStride * multiples;
    }
    return params;
}
```

**CopyDeqTensorToFbuf（L342-356，FB burst 单位 128B）**：
```cpp
__aicore__ inline void CopyDeqTensorToFbuf(
    __cbuf__ uint64_t* cbufWorkspace, const FixpipeTilingV220& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex)
{
    uint16_t deqDataSize = DivCeil(calNSize * sizeof(uint64_t), 128) * 128;
    __fbuf__ uint64_t* deqTensorTempBuf =
        AscendCUtils::GetTemporaryFbBufferAddr<uint64_t>(0, deqDataSize / sizeof(uint64_t));
    uint32_t deqValueOffset = nIterIndex * fixpipeTiling.nSize;
    // L1 -> FB
    uint16_t fbufBurstLen = deqDataSize / 128; // copy from cbuf to fbuf, burst_len unit is 128Bytes
    copy_cbuf_to_fbuf(deqTensorTempBuf, cbufWorkspace + deqValueOffset, 1, fbufBurstLen, 0, 0);
    // FPC of fixpipe buffer for Quant_PRE is FPC[15:8], unit is 128Bytes
    uint64_t deqTensorAddr = ((uint64_t)deqTensorTempBuf >> static_cast<uint64_t>(7)) << 8;
    set_fpc(deqTensorAddr);
    AscendCUtils::FreeTemporaryFbBuffer<uint64_t>(deqTensorTempBuf);
}
```

**FixpipeL0cToUB（L387-407，→ copy_matrix_cc_to_ub，14 参）**：
```cpp
// contains loop info and cal n size for each loop
// move data L0C->UB
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToUB(
    __ubuf__ T* dst, __cc__ U* src, const FixpipeParamsM300& intriParams, const FixpipeTilingV220& fixpipeTiling,
    uint16_t calNSize, uint16_t nIterIndex = 0)
{
    uint16_t cburstNum = fixpipeTiling.nSize / 16;
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    bool nz2ndEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * 32 / sizeof(T);
    }
    // LOC -> UB only n direction need tiling, m no need tiling
    return copy_matrix_cc_to_ub(
        (__ubuf__ T*)(dst + dstOffset), (__cc__ U*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag, intriParams.quantPre,
        static_cast<uint8_t>(intriParams.reluEn), intriParams.isChannelSplit, nz2ndEn, false);
}
```

**FixpipeL0C2UBImpl 3 参（L409-429，完整：标量量化 + 普通模式）**：
```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ T* dst, __cc__ U* src, const FixpipeParamsM300& intriParams)
{
    const FixpipeParamsM300& params = AdaptFixpipeParams<T, U, config>(intriParams);
    CheckFixpipeL0C2UBParam<T, U, config>(dst, src, params);
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        SetLoop3Para(params);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to ub
    */
    if (IsScalarQuantMode(params.quantPre)) {
        SetQuantPreImpl(params.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    // LOC -> UB
    FixpipeTilingV220 fixpipeTiling;
    FixpipeL0cToUB<T, U, config>(dst, src, params, fixpipeTiling, params.nSize, 0);
}
```

**FixpipeL0C2UBImpl 4 参（L431-458，仅向量量化，非向量模式静默无操作）**：
```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsM300& intriParams)
{
    const FixpipeParamsM300& params = AdaptFixpipeParams<T, U, config>(intriParams);
    CheckFixpipeL0C2UBParam<T, U, config>(dst, src, params);
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        SetLoop3Para(params);
    }
    /*
    make code for vector quant mode:
    1. generate tiling
    2. copy deq tensor from l1 to fb0
    3. code gen: move data from l0c to ub
    */
    FixpipeTilingV220 fixpipeTiling = GenFixpipeTilingV220(params.nSize);
    if (IsVectorQuantMode(params.quantPre)) {
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2UBImplN<T, U, config>(dst, src, cbufWorkspace, params, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        // deal with the tail, it also need copy deq/relu tensor from L1 to fb0
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2UBImplN<T, U, config>(
                dst, src, cbufWorkspace, params, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
}
```

（FixpipeL0C2UBImplN L460-470：CopyDeqTensorToFbuf + PipeBarrier + FixpipeL0cToUB。）

**FixpipeL0cToL1（L474-494，→ copy_matrix_cc_to_cbuf，14 参）**：
```cpp
// contains loop info and cal n size for each loop
// move data L0C->L1
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToL1(
    __cbuf__ T* dst, __cc__ U* src, const FixpipeParamsM300& intriParams, const FixpipeTilingV220& fixpipeTiling,
    uint16_t calNSize, uint16_t nIterIndex = 0)
{
    uint16_t cburstNum = fixpipeTiling.nSize / 16;
    uint32_t srcOffset = cburstNum * nIterIndex * intriParams.srcStride * BLOCK_CUBE;
    uint32_t dstOffset = 0;
    bool nz2ndEn = false;
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        dstOffset = nIterIndex * fixpipeTiling.nSize;
        nz2ndEn = true;
    } else {
        dstOffset = cburstNum * nIterIndex * intriParams.dstStride * 32 / sizeof(T);
    }
    // LOC -> L1 only n direction need tiling, m no need tiling
    return copy_matrix_cc_to_cbuf(
        (__cbuf__ T*)(dst + dstOffset), (__cc__ U*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, 0, intriParams.unitFlag, intriParams.quantPre,
        static_cast<uint8_t>(intriParams.reluEn), false, nz2ndEn, false);
}
```

**FixpipeL0C2L1Impl 3 参（L496-517，完整；注意 L500 检查传入的是未适配的 intriParams）**：
```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ T* dst, __cc__ U* src, const FixpipeParamsM300& intriParams)
{
    const FixpipeParamsM300& params = AdaptFixpipeParams<T, U, config>(intriParams);
    CheckFixpipeL0C2L1Param<T, U, config>(dst, src, intriParams);
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        SetLoop3Para(params);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to l1
    */
    if (IsScalarQuantMode(params.quantPre)) {
        // deq factor of uint64 bits describe: bits[31:13] is deq value of fp32,
        SetQuantPreImpl(params.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    FixpipeTilingV220 fixpipeTiling;
    // LOC -> L1
    FixpipeL0cToL1<T, U, config>(dst, src, params, fixpipeTiling, params.nSize);
}
```

**FixpipeL0C2L1Impl 4 参（L519-546，仅向量量化）**：
```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(
    __cbuf__ T* dst, __cc__ U* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsM300& intriParams)
{
    const FixpipeParamsM300& params = AdaptFixpipeParams<T, U, config>(intriParams);
    CheckFixpipeL0C2L1Param<T, U, config>(dst, src, params);
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        SetLoop3Para(params);
    }
    /*
    make code for vector quant mode:
    1. generate tiling
    2. copy deq tensor from l1 to fb0
    3. code gen: move data from l0c to l1
    */
    FixpipeTilingV220 fixpipeTiling = GenFixpipeTilingV220(params.nSize);
    if (IsVectorQuantMode(params.quantPre)) {
        for (uint16_t i = 0; i < fixpipeTiling.nIterNum; ++i) {
            FixpipeL0C2L1ImplN<T, U, config>(dst, src, cbufWorkspace, params, fixpipeTiling, fixpipeTiling.nSize, i);
        }
        // deal with the tail, it also need copy deq/relu tensor from L1 to fb0
        if (fixpipeTiling.tailNSize > 0) {
            FixpipeL0C2L1ImplN<T, U, config>(
                dst, src, cbufWorkspace, params, fixpipeTiling, fixpipeTiling.tailNSize, fixpipeTiling.nIterNum);
        }
        return;
    }
}
```

**异常代码：单模板参数 T 的 stub（L358-383，用途不明，不可从包装到达）**：
```cpp
template <typename T, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ T* dst, __cc__ T* src, const FixpipeParamsM300& intriParams)
{
    ASCENDC_DEBUG_ASSERT(
        false,
        KERNEL_LOG_INTERNAL(
            KERNEL_ERROR,
            "Failed to check dtype in Fixpipe, when src position is L0C Buffer(CO1) and dst position is "
            "L1 Buffer(A1), "
            "support dtype combinations are src: float, dst: int8_t / uint8_t / half / bfloat16_t; src: int32_t, dst: "
            "int8_t / uint8_t / half\n"));
}
// （L371-383 为带 cbufWorkspace 的同型 4 参版本，同样恒为 assert false）
```
包装以显式 `<PrimT<T>, PrimT<U>, config>` 三模板实参调用，恒命中 `<T, U, config>` 版本，此单 T 版本无法经正常链路触达。

**老式 API 并存说明**：`dav_m300/kernel_operator_fixpipe_impl.h`（668 行）另含一套完整老式路径：`FixpipeInfoParams<T>`（L100-189，由老式 `FixpipeParams<S>` 换单位）、`FixpipeL0C2L1Impl/FixpipeL0C2UBImpl(__cc__ U*, FixpipeInfoParams<U>&)`（L191-232 / L276-317）、`FixpipeL0cToL1/FixpipeL0cToUb`（L354-420 / L510-575，按 9 种 quantPre switch 分发到 14 参 intrinsic），以及直接定义的老式公共函数 `Fixpipe(LocalTensor, LocalTensor, FixpipeParams<S>)` 等（L593-662，按 dst 位置分发 UB/L1，GM 版本带 CPU_DEBUG ProcessLock）。intf.h 当前**没有**为 3002 声明老式 `FixpipeParams<S>` 变体，该套定义无对应接口声明（定义本身可见，属历史并存路径）。

---

### 4.2 3102（dav_m310，原生 M310）

单文件 `dav_m310/kernel_operator_fixpipe_impl.h`（634 行），V220 风格、M310 命名类型，无老式 API、无 v2 文件。

**SetLoop3Para（L93-100）**：与 dav_m300 v2 L67-74 逐字相同（参数类型换为 `FixpipeParamsM310`）。
**AdaptFixpipeParams（L283-298）**：与 dav_m300 v2 L296-311 相同（类型换为 `FixpipeParamsM310`）。

**CopyDeqTensorToFbuf（L329-344，FB burst 单位 64B —— 与 m300 v2 的 128B 不同）**：
```cpp
__aicore__ inline void CopyDeqTensorToFbuf(
    __cbuf__ uint64_t* cbufWorkspace, const FixpipeTilingM310& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex)
{
    uint16_t deqDataSize = DivCeil(calNSize * sizeof(uint64_t), 128) * 128;
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
```

**FixpipeL0cToUB（L348-368）**：与 dav_m300 v2 L387-407 逻辑相同（tiling 类型 `FixpipeTilingM310`，14 参 `copy_matrix_cc_to_ub`）。

**FixpipeL0C2UBImpl 3 参（L370-390，完整；标量量化直接 `set_quant_pre` 而非 SetQuantPreImpl）**：
```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ T* dst, __cc__ U* src, const FixpipeParamsM310& intriParams)
{
    const FixpipeParamsM310& params = AdaptFixpipeParams<T, U, config>(intriParams);
    CheckFixpipeL0C2UBParam<T, U, config>(dst, src, params);
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        SetLoop3Para(params);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to ub
    */
    if (IsScalarQuantMode(params.quantPre)) {
        set_quant_pre(params.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    // LOC -> UB
    FixpipeTilingM310 fixpipeTiling;
    FixpipeL0cToUB<T, U, config>(dst, src, params, fixpipeTiling, params.nSize, 0);
}
```

**FixpipeL0C2UBImpl 4 参（L404-431）**：结构与 dav_m300 v2 L431-458 相同（仅向量量化 + return）。
**FixpipeL0cToL1（L435-456）**：同 dav_m300 v2 L474-494（14 参 `copy_matrix_cc_to_cbuf`）。

**FixpipeL0C2L1Impl 3 参（L458-479，完整；L462 检查同样传 intriParams）**：
```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ T* dst, __cc__ U* src, const FixpipeParamsM310& intriParams)
{
    const FixpipeParamsM310& params = AdaptFixpipeParams<T, U, config>(intriParams);
    CheckFixpipeL0C2L1Param<T, U, config>(dst, src, intriParams);
    if constexpr (config.format == CO2Layout::ROW_MAJOR) {
        SetLoop3Para(params);
    }
    /*
    make code for scalar quant mode:
    1. copy deq scalar u64 immediate
    2. code gen: move data from l0c to l1
    */
    if (IsScalarQuantMode(params.quantPre)) {
        // deq factor of uint64 bits describe: bits[31:13] is deq value of fp32,
        set_quant_pre(params.deqScalar);
    }
    PipeBarrier<PIPE_FIX>();
    FixpipeTilingM310 fixpipeTiling;
    // LOC -> L1
    FixpipeL0cToL1<T, U, config>(dst, src, params, fixpipeTiling, params.nSize);
}
```

**FixpipeL0C2L1Impl 4 参（L493-520）**：结构与 dav_m300 v2 L519-546 相同（仅向量量化）。

**参数检查风格（与 m300 v2 的差异）**：`CheckCommonFixpipeParam`（L102-171）src 对齐用 `ASCENDC_CHECK_TENSOR_PTR_ALIGN`（m300 v2 手工算地址），范围检查用 `ASCENDC_CHECK_VALUE_RANGE` / `UINT12_MAX` 宏（m300 v2 手写 ASCENDC_ASSERT）；`CheckFixpipeL0C2UBParam`（L173-213）dtype 列表**不含 bfloat16_t**（m300 v2 含）；L1 dst 对齐检查在 CPU_DEBUG 下手工计算 A1 基址（L219-227）。

---

### 4.3 3003（dav_l300，目标版本）

`dav_l300/kernel_operator_fixpipe_impl.h`（550 行）。V220 声明在 3003 可见，走"V220 适配器 → 本地 FixpipeParamsL300 路径"。

**V220 适配器（L498-513，完整；纯参数转换后委托本地实现）**：
```cpp
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
```

**UB stub（L532-543，两参变体均断言不支持）**：
```cpp
template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ T* dst, __cc__ U* src, const FixpipeParamsV220& intriParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Fixpipe doesn't support L0C to UB on current device\n"); });
}

template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, __cbuf__ uint64_t* cbufWorkspace, const FixpipeParamsV220& intriParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Fixpipe doesn't support L0C to UB on current device\n"); });
}
```

**TransFixpipeParamsV220ToFixpipeParamsL300（L474-496；注意 COLUMN_MAJOR 分支 L494 `dstNdStride = srcNdStride`）**：
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
        dstParams.params.ndNum = intriParams.ndNum;
        dstParams.params.srcNdStride = intriParams.srcNdStride;
        dstParams.params.dstNdStride = intriParams.srcNdStride;
    }
}
```

**本地路径 SetLoop3Para（L197-219，ROW_MAJOR 与 COLUMN_MAJOR 双分支）**：
```cpp
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
```

**本地路径 FixpipeL0cToL1（L223-253，→ 22 参 copy_matrix_cc_to_cbuf）**：
```cpp
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
```

**本地路径 FixpipeL0C2L1Impl 3 参（L353-379）**：
```cpp
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
```

4 参版本（L381-414）仅向量量化模式循环 + return，与 m300/m310 同型。`CopyDeqTensorToFbuf`（L178-195）：`fbufBurstLenUnit = 128`；`GenFixpipeTiling`（L142-160）：`maxDeqNums = 1024`（m300 v2/m310 为 256）。

---

### 4.4 3103（dav_l310，目标版本）与 4.5 3113（dav_l311，目标版本）

两文件逐行一致（dav_l310 由 dav_l311 复制而来，类型名均为 `FixpipeParamsL311`）。与 dav_l300 的结构差异：

- `FixpipeL0cToL1`（L223-257）尾参多 `nz2dnEn`（COLUMN_MAJOR 分支 `dstOffset = nIterIndex * nSize * dstStride; nz2dnEn = true;`），intrinsic 为 **24 参** `copy_matrix_cc_to_cbuf`；
- `CopyDeqTensorToFbuf`（L178-195）：`fbufBurstLenUnit = 64`；
- V220 适配器 `FixpipeL0C2L1Impl`（L505-520）、UB stub（L539-550）与 dav_l300 相同模式：

```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsL311<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsL311(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, params);
}

template <typename T, typename U, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ T* dst, __cc__ U* src, const FixpipeParamsV220& intriParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Fixpipe doesn't support L0C to UB on current device\n"); });
}
```

本地 `FixpipeL0C2L1Impl` 3 参（L360-386）/4 参（L388-421）与 dav_l300 同型（SetLoop3Para 无条件调用 + COLUMN_MAJOR 额外 `set_channel_para`）。

**3103 关键结论**：intf.h L77-78 的 V220 宏控不含 3103，M300/M310 声明也不可见 → 3103 上**没有任何 Fixpipe 声明**，dav_l310 中完整的 V220 适配器与本地实现均为**死代码**（编译不报错，因为模板未被实例化）。3113 上 V220 声明可见，L1 路径可用、UB 为 stub。

---

### 4.6 3510（dav_3510，参考版本）

`dav_3510/kernel_operator_fixpipe_impl.h`（856 行）。原生 Arch3510 实现 + V220 适配器双层齐全，L1 与 UB 均完整。

**FixpipeL0C2L1Impl 3 参（L572-601，AIC 保护 + 显式参数检查）**：
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
}
```

4 参（L603-640）仅向量量化；UB 同构：3 参 L642-670、4 参 L672-708（均 `if ASCEND_IS_AIC` + `CheckFixpipeL0C2UBParam`）。

**FixpipeL0cToL1（L188-222，→ 25 参 copy_matrix_cc_to_cbuf）与 FixpipeL0cToUB（L226-282，→ 26 参 copy_matrix_cc_to_ub，含双目的模式检查）**：
```cpp
// contains loop info and cal n size for each loop
// move data L0C->UB
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0cToUB(
    __ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsArch3510<config.format>& intriParams,
    const FixpipeTiling& fixpipeTiling, uint16_t calNSize, uint16_t nIterIndex = 0)
{
    ASCENDC_DEBUG_ASSERT(
        (!(intriParams.isChannelSplit)),
        KERNEL_LOG_INTERNAL(
            KERNEL_ERROR, "Failed to check isChannelSplit in Fixpipe, when src position is "
                          "L0C Buffer(CO1) and dst position is UB(VECIN/VECCALC/VECOUT), isChannelSplit must be set "
                          "as false \n"));

    // dual destination mode limit
    ASCENDC_ASSERT(
        !(intriParams.dualDstCtl != 0b00 && (config.format == CO2Layout::COLUMN_MAJOR ||
                                             intriParams.quantPre != QuantMode_t::NoQuant || intriParams.reluEn)),
        {
            KERNEL_LOG(
                KERNEL_ERROR,
                "Dual destination mode can be enabled only when normal DMA or NZ2ND is enabled with any other "
                "Fixpipe functions bypassed.");
        });

    ASCENDC_ASSERT(!(intriParams.dualDstCtl == 0b01 && intriParams.mSize % 2 != 0), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "dual destination mode, split in M dimension, M/2 * N is written to each UB, M must be number of 2.");
    });

    ASCENDC_ASSERT(!(intriParams.dualDstCtl == 0b10 && calNSize % 32 != 0), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "dual destination mode, split in N dimension, M * N/2 is written to each UB, N must be number of 32.");
    });

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
    // LOC -> UB only n direction need fixpipeTiling, m no need fixpipeTiling
    return copy_matrix_cc_to_ub(
        (__ubuf__ DstT*)(dst + dstOffset), (__cc__ SrcT*)(src + srcOffset), 0, calNSize, intriParams.mSize,
        intriParams.dstStride, intriParams.srcStride, intriParams.dualDstCtl, intriParams.subBlockId, 0,
        intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), static_cast<uint8_t>(intriParams.reluEn),
        false, nz2ndEn, static_cast<uint64_t>(QuantMode_post::NoConv), 0, false, false, 0, false, false, false, false,
        false, nz2dnEn);
}
```

**V220 适配器（L800-832，L1 与 UB 均完整；与 l300/l311 同模式）**：
```cpp
template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2L1Impl(__cbuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsArch3510<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsArch3510(intriParams, params);
    FixpipeL0C2L1Impl<DstT, SrcT, config>(dst, src, params);
}

template <typename DstT, typename SrcT, const FixpipeConfig& config>
__aicore__ inline void FixpipeL0C2UBImpl(__ubuf__ DstT* dst, __cc__ SrcT* src, const FixpipeParamsV220& intriParams)
{
    FixpipeParamsArch3510<config.format> params;
    TransFixpipeParamsV220ToFixpipeParamsArch3510(intriParams, params);
    FixpipeL0C2UBImpl<DstT, SrcT, config>(dst, src, params);
}
// （4 参 cbufWorkspace 版本及 GM 版本同模式，L808-849）
```

`TransFixpipeParamsV220ToFixpipeParamsArch3510`（L776-798）同样存在 COLUMN_MAJOR 分支 `dstParams.params.dstNdStride = intriParams.srcNdStride;`（L796）。`SetLoop3Para`（L162-184）、`CopyDeqTensorToFbuf`（L143-160，`fbufBurstLenUnit = 64`）、`GenFixpipeTiling`（maxDeqNums = 1024）。

---

### 4.7 5101（dav_5161，目标版本）

`dav_5161/kernel_operator_fixpipe_impl.h` 全文 86 行，**无任何 Fixpipe 实现**（FixpipeL0C2L1Impl/FixpipeL0C2UBImpl 均不存在）。仅含 SPR 类 stub/实现：

```cpp
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
// ……（SetFixPipeConfigImpl 单 Tensor 版、SetFixpipeNz2ndFlagImpl stub、SetFixpipePreQuantFlagImpl → set_m_quant_pre）
```

由于 5101 上 M300/M310 声明与 V220 声明均不可见，包装模板不会实例化，缺失实现不导致编译错误；但若未来向 5101 放行任何 Fixpipe 声明，将直接产生链接/编译失败。

---

## 5. 代码级实现差异分析

### 5.1 风格对比表

| 维度 | 3002 (m300 v2) | 3102 (m310) | 3003 (l300) | 3103/3113 (l310/l311) | 3510（参考） | 5101 |
|---|---|---|---|---|---|---|
| 文件组织 | 双文件（老 API impl.h + V220 v2_impl.h） | 单文件 V220 风格 | 单文件：本地路径 + V220 适配器 | 同 l300 | 单文件：原生 + V220 适配器 | 仅 SPR |
| Impl 参数类型 | `FixpipeParamsM300`（=V220 别名，非模板） | `FixpipeParamsM310`（同左） | `FixpipeParamsL300<format>` 模板 | `FixpipeParamsL311<format>` | `FixpipeParamsArch3510<format>` | — |
| 参数适配 | `AdaptFixpipeParams` 单位换算（原地改 dstStride/srcNdStride） | 同左 | `Trans*` 字段拷贝到本地模板类 | 同左 | 同左 | — |
| L1 能力 | 完整 | 完整 | 完整 | 完整（3103 死代码） | 完整 | 无 |
| UB 能力 | 完整（dtype 含 bfloat16_t） | 完整（dtype 无 bfloat16_t） | stub | stub（3103 死代码） | 完整（双目的模式 dualDstCtl/subBlockId） | 无 |
| AIC 保护 | 无 | 无 | 无 | 无 | `if ASCEND_IS_AIC` 包裹全部逻辑 | — |
| 参数检查 | `CheckFixpipeL0C2*Param`（CPU_DEBUG 手写断言 + static_assert dtype，dst/src 对齐手工计算） | 同结构，改用 `ASCENDC_CHECK_VALUE_RANGE`/`UINT12_MAX`/`ASCENDC_CHECK_TENSOR_PTR_ALIGN` 宏 | 适配器无检查；检查分散在 `FixpipeL0cToL1`（ASCENDC_DEBUG_ASSERT） | 同 l300 | Impl 内显式调用 `CheckFixpipeL0C2*Param` | — |
| 标量量化 SPR | `SetQuantPreImpl(deqScalar)` | `set_quant_pre(deqScalar)` 直调 | `set_quant_pre` 直调 | `set_quant_pre` 直调 | `set_quant_pre` 直调 | — |
| FB burst 单位 | 128B | **64B** | 128B | **64B** | **64B** | — |
| tiling maxDeqNums | 256 | 256 | **1024** | **1024** | **1024** | — |
| COLUMN_MAJOR(nz2dn) | 不支持（仅 ROW_MAJOR 设 LOOP3_PARA） | 不支持 | 支持（`set_channel_para`） | 支持（`set_channel_para` + intrinsic 尾参 nz2dnEn） | 支持 | — |
| quantPre 传参 | `QuantMode_t` 直传（14 参 intrinsic） | 同左 | `static_cast<uint64_t>` + `QuantMode_post::NoConv`（22 参） | 同左 + nz2dnEn（24 参） | 同左（L1 25 参 / UB 26 参） | — |
| 量化模式集合 | 6 种（V/标量 DEQF16、QF322B8_PRE、REQ8） | 6 种 | **13 种**（增加 QF162S4_PRE/REQ4/DEQS16/QF162S16_PRE 及向量版） | 13 种 | 13 种 | — |
| GM 溢出检查 | `g_gm_overflow_check` + `GetGMLength` | 同左 | 同左 | 同左 | 同左 | — |

### 5.2 实现模式总结

1. **两代实现谱系**：
   - "直连"谱系（3002/3102）：V220 别名参数直接驱动 `FixpipeL0C2*Impl`，`AdaptFixpipeParams` 做少量单位换算，14 参精简 intrinsic，`QuantMode_t` 强类型传参。
   - "转接"谱系（3003/3113/3510）：V220 参数先 `Trans*` 到本地模板参数类（`FixpipeParamsL300/L311/Arch3510<format>`，含 `params` 子结构与 nz2dn 字段），再进本地 `FixpipeL0C2*Impl`；intrinsic 为 22-26 参扩展签名（uint64 量化模式 + 后处理模式）。
2. **3 参/4 参 Impl 语义分工**（所有版本一致）：3 参 = 标量量化（deqScalar → set_quant_pre/SetQuantPreImpl）+ 普通模式一次拷贝；4 参（cbufWorkspace）= 向量量化（GenTiling 循环 + CopyDeqTensorToFbuf 分批搬运 deqTensor，尾部处理）后 `return`，**非向量量化模式静默无操作**（无 fallback、无断言）。
3. **LOOP3_PARA 布局一致**（dstNdStride[63:32] / srcNdStride[31:16] / ndNum[15:0]），但触发条件不同：m300 v2/m310 仅 ROW_MAJOR 且在 Impl 内 if constexpr；l300/l311/3510 在本地 Impl 无条件调用（SetLoop3Para 内部按 format 分支），且 COLUMN_MAJOR 走 dnNum/dstDnMatrixStride/srcNzMatrixStride 并额外 `set_channel_para`。
4. **UB 支持梯度**：3510（双目的模式）> 3002/3102（基础 UB）> 3003/3113（stub）> 3103（死代码）/5101（未定义）。

### 5.3 关键差异点/风险点

1. **签名级冗余**：M300/M310 4 个声明与 V220 声明因别名而完全同签名，仅宏控不同。3002/3102 未纳入 V220 宏控（L77-78），因此各版本各取一套声明，但底层共享同一无宏控包装 —— 声明层的架构拆分并未带来实现层的隔离。
2. **3103 死代码**：dav_l310 实现完整（含 V220 适配器与本地路径），但 3103 无任何 Fixpipe 声明可见（V220 宏控不含 3103，M300/M310 宏控不含 3103），全部不可达。
3. **5101 空实现**：无 Fixpipe 任何痕迹；当前不可编译出错误仅因声明不可见。
4. **4 参变体静默无操作**：所有版本的 4 参 Impl 在非向量量化模式下只做检查与 SetLoop3Para 后直接结束（m300 v2 L431-458、m310 L404-431、l300 L381-414、l311 L388-421、3510 L603-640/L672-708），用户传 NoQuant + deqTensor 形态时数据不会被搬运，也无告警。
5. **检查参数不一致**：m300 v2 L500 与 m310 L462 的 3 参 L1 Impl 将**未适配**的 `intriParams` 传给 `CheckFixpipeL0C2L1Param`（UB 变体传适配后的 `params`），NZ 格式下 dstStride 换算后的检查与实际使用值不一致。
6. **Trans 的 COLUMN_MAJOR 疑似 bug**：l300 L494、l311 L500、3510 L796 均写 `dstParams.params.dstNdStride = intriParams.srcNdStride;`（源为 srcNdStride，疑应为 dstNdStride），三处一致，或为有意映射，需确认。
7. **m300 v2 单 T stub**（L358-383）：`template <typename T, const FixpipeConfig& config>` 的 `FixpipeL0C2L1Impl` 恒断言失败，且正常链路（显式三模板实参）无法触达，用途不明。
8. **FB burst 单位硬件差异**：m300 v2/l300 为 128B，m310/l310/l311/3510 为 64B；maxDeqNums 3002/3102 为 256，其余为 1024 —— 均为芯片差异，适配时不可混用。
9. **老式 API 残留**：dav_m300 impl.h 中 `Fixpipe(FixpipeParams<S>)` 定义（L593-662）无 intf.h 声明对应，与 V220 路径并存（不同参数类型重载不冲突），属历史遗留。
10. **dtype 能力差异**：m300 v2 的 UB dtype 列表含 bfloat16_t、L1 列表含 int4b_t/bfloat16_t；m310 均无；l 系列与 3510 的检查在本地路径中相对宽松（多为 ASCENDC_DEBUG_ASSERT）。

---

## 6. 待确认问题

1. M300/M310 声明与 V220 声明签名完全相同（别名），是否应将 3002/3102 纳入 V220 宏控并删除冗余的 M300/M310 声明段（intf.h L106-157）？或保留现状作为架构语义标记？
2. 3103 是否应支持 Fixpipe？dav_l310 实现完整但为死代码 —— 是宏控遗漏（L77-78 未含 3103）还是有意不支持？
3. 5101 是否计划支持 Fixpipe？若支持，dav_5161 需从零补齐 `FixpipeL0C2L1Impl/FixpipeL0C2UBImpl`（当前连 stub 都没有）。
4. 3003/3113 的 L0C→UB stub（"doesn't support L0C to UB on current device"）是硬件不支持还是未实现？3510/3002/3102 均有 UB 实现可参考。
5. 4 参（deqTensor）变体在非向量量化模式下静默无操作 —— 是否应补充断言或 fallback 到普通拷贝？
6. 3 参 L1 Impl 检查传未适配参数（m300 v2 L500 / m310 L462）—— 是否需要改为适配后的 `params`？
7. Trans 函数 COLUMN_MAJOR 分支 `dstNdStride = srcNdStride`（l300 L494 / l311 L500 / 3510 L796）是否有意为之？
8. m300 v2 单模板参数 T 的 `FixpipeL0C2L1Impl` stub（L358-383）的调用方是谁？是否可删除？
9. m300 v2 老 API（`FixpipeParams<S>` 路径，impl.h L593-662）是否仍有外部使用方？若无，是否应清理？
10. FB burst 单位（128B vs 64B）与 maxDeqNums（256 vs 1024）差异请硬件侧确认，作为跨版本移植的约束输入。

---

## 附：涉及文件清单

| 文件 | 角色 |
|---|---|
| include/basic_api/kernel_operator_fixpipe_intf.h（190 行） | 声明层（本报告行号基于当前已修改状态） |
| include/basic_api/kernel_struct_fixpipe.h | FixpipeParamsM300/M310 别名（L235-236）、FixpipeParamsV220/Arch3510/L300/L311 定义 |
| impl/basic_api/kernel_operator_fixpipe_intf_impl.h（298 行） | 共享包装 + include 分发（L30-54） |
| impl/basic_api/dav_m300/kernel_operator_fixpipe_impl.h（668 行） | 3002 老 API 路径 |
| impl/basic_api/dav_m300/kernel_operator_fixpipe_v2_impl.h（672 行） | 3002 V220/M300 路径（M300 声明实际落点） |
| impl/basic_api/dav_m310/kernel_operator_fixpipe_impl.h（634 行） | 3102 V220/M310 路径 |
| impl/basic_api/dav_l300/kernel_operator_fixpipe_impl.h（550 行） | 3003 |
| impl/basic_api/dav_l310/kernel_operator_fixpipe_impl.h（557 行） | 3103（死代码） |
| impl/basic_api/dav_l311/kernel_operator_fixpipe_impl.h（557 行） | 3113（与 l310 逐行一致） |
| impl/basic_api/dav_3510/kernel_operator_fixpipe_impl.h（856 行） | 3510（参考） |
| impl/basic_api/dav_5161/kernel_operator_fixpipe_impl.h（86 行） | 5101（仅 SPR） |
