# 案例分析：DataCopy(GM→L1 Nd2Nz) 接口跨版本适配

## 接口概述

两个接口均用于 GlobalTensor(OUT) → LocalTensor(L1) 的 ND→NZ 格式转换数据搬运，通过 `__NPU_ARCH__` 守卫区分变体：

| 变体 | 行号 | 参数类型 | 模板参数 | Arch 守卫 | 守卫中的版本 | Else 分支版本 |
|------|------|---------|---------|-----------|-------------|---------------|
| Nd2Nz V1 | L75 | `Nd2NzParams` | `<T, bool enableSmallC0>` | L73-74: `3510\|\|5102\|\|3113\|\|5101\|\|5161\|\|5165\|\|5163` | 3510, 5102, 3113, 5101, 5161, 5165, 5163 | — |
| Nd2Nz V2 | L79 | `Nd2NzParams` | `<T>` | 同上 #else | — | **3003, 3103** 及其他未列出版本 |

**关键发现**：
- V1 和 V2 是同一接口的两个编译分支，不会同时存在
- 3003/3103 走 V2 分支（无 enableSmallC0 模板参数）
- 3113/5101 走 V1 分支（有 enableSmallC0 模板参数）

### 硬件特性支持范围

| 硬件特性 | 支持起始版本 | 3003 | 3103 | 3113 | 3510 | 5101 |
|---------|------------|------|------|------|------|------|
| `set_mte2_nz_para` | 自 311 起 | ❌ | ❌ | ✅ | ✅ | ✅ |
| `copy_gm_to_cbuf_multi_nd2nz` | 自 311 起 | ❌ | ❌ | ✅ | ✅ | ✅ |
| `copy_gm_to_cbuf_multi_nd2nz_b16`/`_b8` | 自 300 起 | ✅ | ✅ | ✅ | ✅ | ✅ |
| `enableSmallC0` | 自 v510 起 | ❌ | ❌ | ❌ | ❌ | ✅ |
| `cacheMode` | 自 3510 起 | ❌ | ❌ | ❌ | ✅ | ✅ |

**影响分析**：
- **3003**：不支持 `set_mte2_nz_para` 和 `copy_gm_to_cbuf_multi_nd2nz` → 使用 `copy_gm_to_cbuf_multi_nd2nz_b16`/`_b8` 专用函数直接传所有 stride（当前实现正确），B32 无对应专用函数故为桩函数
- **3103**：不支持 `set_mte2_nz_para` 和 `copy_gm_to_cbuf_multi_nd2nz` → 与 3003 同代硬件，但当前代码从 l311 复制时引入了 `set_mte2_nz_para` + `copy_gm_to_cbuf_multi_nd2nz` 调用（**已修正为桩函数**）。理论上 3103 可参照 l300 模式用 `copy_gm_to_cbuf_multi_nd2nz_b16`/`_b8` 实现 B16/B8 路径，但 B32 仍无法支持
- **3113**：支持 `set_mte2_nz_para` 和 `copy_gm_to_cbuf_multi_nd2nz`，不支持 `enableSmallC0` → 当前实现用 `set_mte2_nz_para` + `copy_gm_to_cbuf_multi_nd2nz` 传 `false, false` 替代 enableSmallC0，正确
- **3510**：全部支持（参照版本）
- **5101**：全部支持 → 模板模式完整实现

> **注意**：3003 和 3103 属于同一代硬件（L300 系列），均不支持 `set_mte2_nz_para` 和 `copy_gm_to_cbuf_multi_nd2nz`，仅支持旧式 `copy_gm_to_cbuf_multi_nd2nz_b16`/`_b8` 专用函数。3113 起引入了 `set_mte2_nz_para` + `copy_gm_to_cbuf_multi_nd2nz` 通用模式。

## 调用链

```
kernel_operator_data_copy_intf.h:75-82（声明，有 arch 守卫 L73-74）
  → kernel_operator_data_copy_intf_impl.h:123-201（公共包装，有 arch 守卫）
    → [enableSmallC0 路径] DataCopyGM2L1ND2NZ<T, enableSmallC0>（base_impl.h:152-160）
      → DataCopyGM2L1ND2NZImpl<T, enableSmallC0>(dst, src, intriParams, cacheMode)  [版本特定]
    → [L1 路径] DataCopyGM2L1ND2NZImpl(dst, src, intriParams[, cacheMode])  [版本特定]
    → [UB 路径] DataCopyGM2UBND2NZImpl(dst, src, intriParams[, cacheMode])  [版本特定]
```

### 公共包装层 arch 守卫详情（intf_impl.h:123-201）

包装函数分 V1（L123-171）和 V2（L172-200）两个分支，由 L123-124 的 arch 守卫控制。

**V1 分支（L123-171）** — 编译于 3510/5102/3113/5101/5161/5165/5163：

| 内部 arch 守卫 | 行号 | 条件 | 编译版本 | 调用 |
|--------------|------|------|---------|------|
| enableSmallC0 路径 | L138-139 | `3510\|\|5102\|\|5101\|\|5161\|\|5165\|\|5163` | 3510, 5102, 5101, 5161, 5165, 5163 | `DataCopyGM2L1ND2NZ<T, enableSmallC0>` |
| cacheMode 提取 | L145-146 | `3510\|\|5102\|\|3113\|\|5101\|\|5161\|\|5165\|\|5163` | 全部 V1 版本 | `ExtractCacheMode(src)` |
| L1 路径（带 cacheMode） | L151-152 | `3510\|\|5102\|\|5101\|\|5161\|\|5165\|\|5163` | 3510, 5102, 5101, 5161, 5165, 5163 | `DataCopyGM2L1ND2NZImpl(..., cacheMode)` |
| L1 路径（无 cacheMode） | L155-156 | #else（3113） | **3113** | `DataCopyGM2L1ND2NZImpl(...)` |
| UB 路径（带 cacheMode） | L159-160 | `3510\|\|5102\|\|3113\|\|5101\|\|5161\|\|5165\|\|5163` | 全部 V1 版本 | `DataCopyGM2UBND2NZImpl(..., cacheMode)` |
| UB 路径（无 cacheMode） | L163-164 | #else | 无（3113 已在上分支） | `DataCopyGM2UBND2NZImpl(...)` |

**V2 分支（L172-200）** — 编译于 3003/3103 及其他未列出版本：

| 路径 | 行号 | 调用 |
|------|------|------|
| L1 路径 | L192 | `DataCopyGM2L1ND2NZImpl(dst, src, intriParams)` 无 cacheMode |
| UB 路径 | L194 | `DataCopyGM2UBND2NZImpl(dst, src, intriParams)` 无 cacheMode |

### 共享层（base_impl.h）

| 函数 | 行号 | Arch 守卫 | 说明 |
|------|------|-----------|------|
| `DataCopyGM2L1ND2NZ<T, enableSmallC0>` | L152-160 | `3510\|\|5102\|\|5101\|\|5161\|\|5165\|\|5163` | 提取 cacheMode，调用 `DataCopyGM2L1ND2NZImpl<PrimType, enableSmallC0>` |
| `DataCopyGM2UBND2NZ<T>` | L162-170 | 同上 | 提取 cacheMode，调用 `DataCopyGM2UBND2NZImpl` |

**注意**：3113 不在共享层 arch 守卫中，但共享层函数未被 3113 调用（3113 的 enableSmallC0 路径在包装层 L138-139 被排除，直接走 L155-156 的无 cacheMode L1 路径）。

### 版本隔离机制

`kernel_operator_data_copy_base_impl.h:32-55` 按 arch 条件 include 各版本目录的 `kernel_operator_data_copy_impl.h`：

```cpp
#if __NPU_ARCH__ == 3510
#include "dav_3510/kernel_operator_data_copy_impl.h"
#elif (__NPU_ARCH__ == 5102)
#include "dav_m510/kernel_operator_data_copy_impl.h"
#elif (__NPU_ARCH__ == 3003)
#include "dav_l300/kernel_operator_data_copy_impl.h"
#elif (__NPU_ARCH__ == 3113)
#include "dav_l311/kernel_operator_data_copy_impl.h"
#elif (__NPU_ARCH__ == 3103)
#include "dav_l310/kernel_operator_data_copy_impl.h"
#elif (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
#include "dav_5161/kernel_operator_data_copy_impl.h"
#endif
```

## 支持矩阵

### Nd2Nz — GM→L1 路径

| 版本 | Arch | 声明分支 | DataCopyGM2L1ND2NZImpl | enableSmallC0 | cacheMode | 状态 |
|------|------|---------|----------------------|---------------|-----------|------|
| dav_3510 | 3510 | V1 | **完整**（模板，L364-379） | ✅ | ✅ | 完整（参照） |
| dav_l300 | 3003 | V2 | **B16完整 / B8完整 / B32桩**（重载，L214-282） | ❌ | ❌ | 部分完整 |
| dav_l310 | 3103 | V2 | **桩函数**（B16/B32/B8全桩，重载，L240-307） | ❌ | ❌ | 桩函数 |
| dav_l311 | 3113 | V1 | **完整**（B16/B32/B8，重载，L240-358） | ❌ | ❌ | 完整 |
| dav_5161 | 5101 | V1 | **完整**（模板，L325-334） | ✅ | ✅ | 完整 |

### Nd2Nz — GM→UB 路径

| 版本 | Arch | DataCopyGM2UBND2NZImpl | 状态 |
|------|------|----------------------|------|
| dav_3510 | 3510 | **完整**（L473-496，循环实现） | ✅ 完整 |
| dav_l300 | 3003 | **未定义** | ❌ 链接错误风险 |
| dav_l310 | 3103 | **未定义** | ❌ 链接错误风险 |
| dav_l311 | 3113 | **未定义** | ❌ 链接错误风险（V1 分支会调用） |
| dav_5161 | 5101 | **桩函数**（L336-340，ASSERT false） | ❌ 签名不匹配 |

### 支持的数据类型矩阵（GM→L1）

| 版本 | half | bfloat16_t | int16_t | uint16_t | float | int32_t | uint32_t | int8_t | uint8_t | fp4x2 |
|------|------|-----------|---------|----------|-------|---------|----------|--------|---------|-------|
| dav_3510 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| dav_l300 | ✅ | ✅ | ✅ | ✅ | ❌桩 | ❌桩 | ❌桩 | ✅ | ✅ | ❌ |
| dav_l310 | ✅ | ❌缺失 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ |
| dav_l311 | ✅ | ❌缺失 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ |
| dav_5161 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ❌ |

## 各版本原始代码

---

### dav_3510（参照版本）

#### DataCopyGM2L1ND2NZImplBase — 完整实现（L310-362）

```cpp
template <typename T, bool enableSmallC0 = false>
__aicore__ inline void DataCopyGM2L1ND2NZImplBase(
    __cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams, const uint8_t cacheMode = 0)
{
    if ASCEND_IS_AIC {
        SmallC0Checker(enableSmallC0, intriParams.dValue);

        if constexpr (g_gm_overflow_check) {
            __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
            AscendCUtils::CheckGmMemOverflowNd2Nz(src, workSpace, true, intriParams);
        }

        uint16_t loop2DstStride = intriParams.dstNzNStride;
        uint16_t loop3DstStride = intriParams.dstNzC0Stride;
        uint16_t loop4DstStride = static_cast<uint16_t>(intriParams.dstNzMatrixStride * sizeof(T) / ONE_BLK_SIZE);

        uint64_t mte2NzPara = static_cast<uint64_t>(loop4DstStride) << 48;
        mte2NzPara |= static_cast<uint64_t>(loop3DstStride) << 32;
        mte2NzPara |= static_cast<uint64_t>(loop2DstStride) << 16;
        mte2NzPara |= static_cast<uint64_t>(intriParams.ndNum);
        set_mte2_nz_para(mte2NzPara);

        uint64_t loop1SrcStride = intriParams.srcDValue * sizeof(T);
        uint64_t loop4SrcStride = intriParams.srcNdMatrixStride * sizeof(T);
        if constexpr (sizeof(T) == B8_BYTE_SIZE) {
            copy_gm_to_cbuf_multi_nd2nz(
                (__cbuf__ int8_t*)dst, (__gm__ int8_t*)src, 0, loop1SrcStride, cacheMode, intriParams.nValue,
                intriParams.dValue, loop4SrcStride, enableSmallC0);
        }
        if constexpr (sizeof(T) == B16_BYTE_SIZE) {
            copy_gm_to_cbuf_multi_nd2nz(
                (__cbuf__ half*)dst, (__gm__ half*)src, 0, loop1SrcStride, cacheMode, intriParams.nValue,
                intriParams.dValue, loop4SrcStride, enableSmallC0);
        }
        if constexpr (sizeof(T) == B32_BYTE_SIZE) {
            copy_gm_to_cbuf_multi_nd2nz(
                (__cbuf__ float*)dst, (__gm__ float*)src, 0, loop1SrcStride, cacheMode, intriParams.nValue,
                intriParams.dValue, loop4SrcStride, enableSmallC0);
        }
    } else if ASCEND_IS_AIV {
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
        uint8_t* tscmCpuBaseAddr = GetTPipePtr()->GetBaseAddr(int8_t(TPosition::TSCM));
        uint64_t l1Addr = (uint8_t*)dst - tscmCpuBaseAddr;
        ScmDataCopyND2NZMsg((__cbuf__ void*)l1Addr, (__gm__ void*)src, sizeof(T), intriParams, -1);
#else
        ScmDataCopyND2NZMsg(dst, src, sizeof(T), intriParams, -1);
#endif
    }
}
```

#### DataCopyGM2L1ND2NZImpl — 完整实现（L364-379）

```cpp
template <typename T, bool enableSmallC0 = false>
__aicore__ inline void DataCopyGM2L1ND2NZImpl(
    __cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams, const uint8_t cacheMode = 0)
{
    if constexpr (g_gm_overflow_check) {
        __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
        AscendCUtils::CheckGmMemOverflowNd2Nz(src, workSpace, true, intriParams);
    }
    if constexpr (SupportType<T, fp4x2_e2m1_t, fp4x2_e1m2_t>()) {
        DataCopyGM2L1ND2NZImplBase<uint8_t, enableSmallC0>(
            (__cbuf__ uint8_t*)dst, (__gm__ uint8_t*)src, intriParams, cacheMode);
    } else {
        DataCopyGM2L1ND2NZImplBase<T, enableSmallC0>((__cbuf__ T*)dst, (__gm__ T*)src, intriParams, cacheMode);
    }
}
```

#### DataCopyGM2UBND2NZImpl — 完整实现（L473-496）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2UBND2NZImpl(
    __ubuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams, const uint8_t cacheMode = 0)
{
    if ASCEND_IS_AIV {
        if constexpr (g_gm_overflow_check) {
            __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
            AscendCUtils::CheckGmMemOverflowNd2Nz(src, workSpace, true, intriParams);
        }
        const uint16_t& ndNum = intriParams.ndNum;
        const uint16_t& dValue = intriParams.dValue;
        const uint16_t& srcNdMatrixStride = intriParams.srcNdMatrixStride;
        const uint16_t& dstNzMatrixStride = static_cast<uint16_t>(intriParams.dstNzMatrixStride);
        const uint16_t& c0Count = DEFAULT_C0_SIZE / sizeof(T);
        for (int index = 0; index < ndNum; ++index) {
            int16_t copyNum = (dValue + c0Count - 1) / c0Count;
            for (int copyTime = 0; copyTime < copyNum; ++copyTime) {
                int computeCount = (dValue >= (copyTime + 1) * c0Count) ? c0Count : (dValue % c0Count);
                DataCopyGM2UBSingleImpl(
                    dst + dstNzMatrixStride, src + srcNdMatrixStride, intriParams, copyTime, computeCount, cacheMode);
            }
        }
    }
}
```

---

### dav_l300（目标版本，3003）

#### DataCopyGM2L1ND2NZImplB16 — 完整实现（L201-212）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1ND2NZImplB16(__cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    if constexpr (g_gm_overflow_check) {
        __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
        AscendCUtils::CheckGmMemOverflowNd2Nz(src, workSpace, true, intriParams);
    }
    copy_gm_to_cbuf_multi_nd2nz_b16(
        (__cbuf__ T*)dst, (__gm__ T*)src, 0, intriParams.ndNum, intriParams.nValue, intriParams.dValue,
        intriParams.srcNdMatrixStride, intriParams.srcDValue, intriParams.dstNzC0Stride, intriParams.dstNzNStride,
        intriParams.dstNzMatrixStride);
}
```

#### DataCopyGM2L1ND2NZImplB32 — 桩函数（L237-241）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1ND2NZImplB32(__cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "unsupported b32 data copy from gm to l1 nz2nd"); });
}
```

#### DataCopyGM2L1ND2NZImplB8 — 完整实现（L260-271）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1ND2NZImplB8(__cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    if constexpr (g_gm_overflow_check) {
        __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
        AscendCUtils::CheckGmMemOverflowNd2Nz(src, workSpace, true, intriParams);
    }
    copy_gm_to_cbuf_multi_nd2nz_b8(
        (__cbuf__ T*)dst, (__gm__ T*)src, 0, intriParams.ndNum, intriParams.nValue, intriParams.dValue,
        intriParams.srcNdMatrixStride, intriParams.srcDValue, intriParams.dstNzC0Stride, intriParams.dstNzNStride,
        intriParams.dstNzMatrixStride);
}
```

#### 类型重载（L214-282）

```cpp
__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ half* dst, __gm__ half* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB16(dst, src, intriParams); }

__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB16(dst, src, intriParams); }

__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ int16_t* dst, __gm__ int16_t* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB16(dst, src, intriParams); }

__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ uint16_t* dst, __gm__ uint16_t* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB16(dst, src, intriParams); }

__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ float* dst, __gm__ float* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB32(dst, src, intriParams); }  // → 桩函数

__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ int32_t* dst, __gm__ int32_t* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB32(dst, src, intriParams); }  // → 桩函数

__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB32(dst, src, intriParams); }  // → 桩函数

__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ int8_t* dst, __gm__ int8_t* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB8(dst, src, intriParams); }

__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB8(dst, src, intriParams); }
```

#### DataCopyGM2UBND2NZImpl — 未定义

> 文件中不存在此函数。若 V2 包装层 L194 的 UB 路径被触发，将导致链接错误。

---

### dav_l310（目标版本，3103）

#### DataCopyGM2L1ND2NZImplB16 — 完整实现（L240-263）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1ND2NZImplB16(__cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    if constexpr (g_gm_overflow_check) {
        __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
        AscendCUtils::CheckGmMemOverflowNd2Nz(src, workSpace, true, intriParams);
    }

    uint16_t loop2DstStride = intriParams.dstNzNStride;
    uint16_t loop3DstStride = intriParams.dstNzC0Stride;
    uint16_t loop4DstStride = static_cast<uint16_t>(intriParams.dstNzMatrixStride * sizeof(T) / ONE_BLOCK_SIZE);

    uint64_t mte2NzPara = static_cast<uint64_t>(loop4DstStride) << 48;
    mte2NzPara |= static_cast<uint64_t>(loop3DstStride) << 32;
    mte2NzPara |= static_cast<uint64_t>(loop2DstStride) << 16;
    mte2NzPara |= static_cast<uint64_t>(intriParams.ndNum);
    set_mte2_nz_para(mte2NzPara);

    uint64_t loop1SrcStride = intriParams.srcDValue * sizeof(T);
    uint64_t loop4SrcStride = intriParams.srcNdMatrixStride * sizeof(T);
    copy_gm_to_cbuf_multi_nd2nz(
        (__cbuf__ half*)dst, (__gm__ half*)src, 0, loop1SrcStride, intriParams.nValue, intriParams.dValue,
        loop4SrcStride, false, false);
}
```

#### DataCopyGM2L1ND2NZImplB32 — 完整实现（L282-305）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1ND2NZImplB32(__cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    if constexpr (g_gm_overflow_check) {
        __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
        AscendCUtils::CheckGmMemOverflowNd2Nz(src, workSpace, true, intriParams);
    }

    uint16_t loop2DstStride = intriParams.dstNzNStride;
    uint16_t loop3DstStride = intriParams.dstNzC0Stride;
    uint16_t loop4DstStride = static_cast<uint16_t>(intriParams.dstNzMatrixStride * sizeof(T) / ONE_BLOCK_SIZE);

    uint64_t mte2NzPara = static_cast<uint64_t>(loop4DstStride) << 48;
    mte2NzPara |= static_cast<uint64_t>(loop3DstStride) << 32;
    mte2NzPara |= static_cast<uint64_t>(loop2DstStride) << 16;
    mte2NzPara |= static_cast<uint64_t>(intriParams.ndNum);
    set_mte2_nz_para(mte2NzPara);

    uint64_t loop1SrcStride = intriParams.srcDValue * sizeof(T);
    uint64_t loop4SrcStride = intriParams.srcNdMatrixStride * sizeof(T);
    copy_gm_to_cbuf_multi_nd2nz(
        (__cbuf__ float*)dst, (__gm__ float*)src, 0, loop1SrcStride, intriParams.nValue, intriParams.dValue,
        loop4SrcStride, false, false);
}
```

#### DataCopyGM2L1ND2NZImplB8 — 完整实现（L324-347）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1ND2NZImplB8(__cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    if constexpr (g_gm_overflow_check) {
        __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
        AscendCUtils::CheckGmMemOverflowNd2Nz(src, workSpace, true, intriParams);
    }

    uint16_t loop2DstStride = intriParams.dstNzNStride;
    uint16_t loop3DstStride = intriParams.dstNzC0Stride;
    uint16_t loop4DstStride = static_cast<uint16_t>(intriParams.dstNzMatrixStride * sizeof(T) / ONE_BLOCK_SIZE);

    uint64_t mte2NzPara = static_cast<uint64_t>(loop4DstStride) << 48;
    mte2NzPara |= static_cast<uint64_t>(loop3DstStride) << 32;
    mte2NzPara |= static_cast<uint64_t>(loop2DstStride) << 16;
    mte2NzPara |= static_cast<uint64_t>(intriParams.ndNum);
    set_mte2_nz_para(mte2NzPara);

    uint64_t loop1SrcStride = intriParams.srcDValue * sizeof(T);
    uint64_t loop4SrcStride = intriParams.srcNdMatrixStride * sizeof(T);
    copy_gm_to_cbuf_multi_nd2nz(
        (__cbuf__ int8_t*)dst, (__gm__ int8_t*)src, 0, loop1SrcStride, intriParams.nValue, intriParams.dValue,
        loop4SrcStride, false, false);
}
```

#### 类型重载（L265-358）

```cpp
// half, int16_t, uint16_t → B16
__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ half* dst, __gm__ half* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB16(dst, src, intriParams); }
__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ int16_t* dst, __gm__ int16_t* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB16(dst, src, intriParams); }
__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ uint16_t* dst, __gm__ uint16_t* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB16(dst, src, intriParams); }
// 注：无 bfloat16_t 重载

// float, int32_t, uint32_t → B32
__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ float* dst, __gm__ float* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB32(dst, src, intriParams); }
__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ int32_t* dst, __gm__ int32_t* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB32(dst, src, intriParams); }
__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB32(dst, src, intriParams); }

// int8_t, uint8_t → B8
__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ int8_t* dst, __gm__ int8_t* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB8(dst, src, intriParams); }
__aicore__ inline void DataCopyGM2L1ND2NZImpl(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, const Nd2NzParams& intriParams)
{ DataCopyGM2L1ND2NZImplB8(dst, src, intriParams); }
```

#### DataCopyGM2UBND2NZImpl — 未定义

> 文件中不存在此函数。若 V2 包装层 L194 的 UB 路径被触发，将导致链接错误。

---

### dav_l311（目标版本，3113）

> **注意**：dav_l311 仍保留完整实现（B16/B32/B8 辅助函数和类型重载在 L240-358）。dav_l310 已改为桩函数，两者不再一致。

#### DataCopyGM2UBND2NZImpl — 未定义

> 文件中不存在此函数。但 3113 走 V1 分支，包装层 L159-160 会调用 `DataCopyGM2UBND2NZImpl(..., cacheMode)`。若 UB 路径被触发，将导致链接错误。

---

### dav_5161（目标版本，5101）

#### DataCopyGM2L1ND2NZImplBase — 完整实现（L279-323）

```cpp
template <typename T, bool enableSmallC0 = false>
__aicore__ inline void DataCopyGM2L1ND2NZImplBase(
    __cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams, const uint8_t cacheMode = 0)
{
    if ASCEND_IS_AIC {
        SmallC0Checker(enableSmallC0, intriParams.dValue);

        if constexpr (g_gm_overflow_check) {
            __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
            AscendCUtils::CheckGmMemOverflowNd2Nz(src, workSpace, true, intriParams);
        }

        uint16_t loop2DstStride = intriParams.dstNzNStride;
        uint16_t loop3DstStride = intriParams.dstNzC0Stride;
        uint16_t loop4DstStride = static_cast<uint16_t>(intriParams.dstNzMatrixStride * sizeof(T) / ONE_BLK_SIZE);

        uint64_t mte2NzPara = static_cast<uint64_t>(loop4DstStride) << 48;
        mte2NzPara |= static_cast<uint64_t>(loop3DstStride) << 32;
        mte2NzPara |= static_cast<uint64_t>(loop2DstStride) << 16;
        mte2NzPara |= static_cast<uint64_t>(intriParams.ndNum);
        set_mte2_nz_para(mte2NzPara);

        uint64_t loop1SrcStride = intriParams.srcDValue * sizeof(T);
        uint64_t loop4SrcStride = intriParams.srcNdMatrixStride * sizeof(T);
        if constexpr (sizeof(T) == B8_BYTE_SIZE) {
            copy_gm_to_cbuf_multi_nd2nz(
                (__cbuf__ int8_t*)dst, (__gm__ int8_t*)src, intriParams.sid, loop1SrcStride, cacheMode,
                intriParams.nValue, intriParams.dValue, loop4SrcStride, enableSmallC0);
        }
        if constexpr (sizeof(T) == B16_BYTE_SIZE) {
            copy_gm_to_cbuf_multi_nd2nz(
                (__cbuf__ half*)dst, (__gm__ half*)src, intriParams.sid, loop1SrcStride, cacheMode, intriParams.nValue,
                intriParams.dValue, loop4SrcStride, enableSmallC0);
        }
        if constexpr (sizeof(T) == B32_BYTE_SIZE) {
            copy_gm_to_cbuf_multi_nd2nz(
                (__cbuf__ float*)dst, (__gm__ float*)src, intriParams.sid, loop1SrcStride, cacheMode,
                intriParams.nValue, intriParams.dValue, loop4SrcStride, enableSmallC0);
        }
    }
}
```

#### DataCopyGM2L1ND2NZImpl — 完整实现（L325-334）

```cpp
template <typename T, bool enableSmallC0 = false>
__aicore__ inline void DataCopyGM2L1ND2NZImpl(
    __cbuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams, const uint8_t cacheMode = 0)
{
    if constexpr (g_gm_overflow_check) {
        __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
        AscendCUtils::CheckGmMemOverflowNd2Nz(src, workSpace, true, intriParams);
    }
    DataCopyGM2L1ND2NZImplBase<T, enableSmallC0>((__cbuf__ T*)dst, (__gm__ T*)src, intriParams, cacheMode);
}
```

#### DataCopyGM2UBND2NZImpl — 桩函数（L336-340）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2UBND2NZImpl(__ubuf__ T* dst, __gm__ T* src, const Nd2NzParams& intriParams)
{
    ASSERT(false && "unsupported data copy GM to UB ND2NZ on current device");
}
```

> **注意**：此桩函数签名无 cacheMode 参数，但 5101 走 V1 分支，包装层 L159-162 调用时传入 cacheMode → **签名不匹配，可能导致编译错误**。

## 代码级实现差异分析

### 实现风格对比

| 特征 | dav_3510 | dav_l300 | dav_l310 | dav_l311 | dav_5161 |
|------|----------|----------|----------|----------|----------|
| 实现方式 | 模板 `<T, enableSmallC0>` + ImplBase | 非模板重载 × 9 + B16/B32/B8 | 非模板重载 × 8 + B16/B32/B8 桩 | 非模板重载 × 8 + B16/B32/B8 | 模板 `<T, enableSmallC0>` + ImplBase |
| 类型分派 | `sizeof(T)` if constexpr | 具体类型重载 | 具体类型重载(→桩) | 具体类型重载 | `sizeof(T)` if constexpr |
| ISA 函数 | `copy_gm_to_cbuf_multi_nd2nz` | `copy_gm_to_cbuf_multi_nd2nz_b16`/`_b8` | 无(桩函数) | `copy_gm_to_cbuf_multi_nd2nz` | `copy_gm_to_cbuf_multi_nd2nz` |
| ISA 第3参数 | `0` | `0` | N/A(桩函数) | `0` | `intriParams.sid` |
| ISA 参数列表 | `(dst, src, 0, loop1SrcStride, cacheMode, nValue, dValue, loop4SrcStride, enableSmallC0)` | `(dst, src, 0, ndNum, nValue, dValue, srcNdMatrixStride, srcDValue, dstNzC0Stride, dstNzNStride, dstNzMatrixStride)` | N/A(桩函数) | `(dst, src, 0, loop1SrcStride, nValue, dValue, loop4SrcStride, false, false)` | `(dst, src, sid, loop1SrcStride, cacheMode, nValue, dValue, loop4SrcStride, enableSmallC0)` |
| stride 传递 | `set_mte2_nz_para` + ISA 传 loop1/loop4 | ISA 直接传所有 stride | N/A(桩函数) | `set_mte2_nz_para` + ISA 传 loop1/loop4 | `set_mte2_nz_para` + ISA 传 loop1/loop4 |
| `set_mte2_nz_para` 硬件支持 | ✅(3510) | ❌(不支持，未使用) | ❌(不支持，已改桩) | ✅(3113起支持) | ✅ |
| AIC 守卫 | `if ASCEND_IS_AIC` + AIV 路径 | 无 | 无 | 无 | 仅 `if ASCEND_IS_AIC` |
| AIV TSCM 路径 | 有（`ScmDataCopyND2NZMsg`） | 无 | 无 | 无 | 无 |
| fp4x2 支持 | ✅ | ❌ | ❌ | ❌ | ❌ |
| cacheMode | ✅ | ❌ | ❌ | ❌ | ✅ |
| enableSmallC0 | ✅ | ❌ | ❌ | ❌ | ✅ |
| bfloat16_t | ✅ | ✅ | ❌缺失 | ❌缺失 | ✅ |
| B32 (float等) | ✅完整 | ❌桩函数 | ❌桩函数 | ✅完整 | ✅完整 |
| UB Nd2Nz | ✅完整 | ❌未定义 | ❌未定义 | ❌未定义 | ❌桩函数(签名不匹配) |

### 实现模式总结

**模式1：dav_3510/dav_5161 模板模式**
- 模板 `<T, bool enableSmallC0>` 统一处理所有类型
- `sizeof(T)` if constexpr 分派 B8/B16/B32
- `set_mte2_nz_para` 寄存器设置 + `copy_gm_to_cbuf_multi_nd2nz` 硬件指令
- 支持 cacheMode/enableSmallC0
- 有 AIC 守卫
- 3510 独有：AIV TSCM 路径（`ScmDataCopyND2NZMsg`）、fp4x2 支持
- 5161 独有：使用 `intriParams.sid` 替代 `0`

**模式2：dav_l300 旧式重载模式**
- 非模板具体类型重载（9 个重载）
- 专用 ISA 函数 `copy_gm_to_cbuf_multi_nd2nz_b16`/`_b8`（不同类型不同函数名）
- 直接传所有 stride 给 ISA（不用 `set_mte2_nz_para`）
- 不支持 cacheMode/enableSmallC0
- B32 为桩函数
- 支持 bfloat16_t

**模式3：dav_l310 桩函数模式**（已修正）
- 3103 不支持 `set_mte2_nz_para` 和 `copy_gm_to_cbuf_multi_nd2nz`，从 l311 复制的实现无法工作
- B16/B32/B8 三个辅助函数已全部改为 ASSERT false 桩函数
- 8 个类型重载仍保留（委托到桩函数），保持接口完整性

**模式4：dav_l311 过渡模式**
- 非模板具体类型重载（8 个重载，无 bfloat16_t）
- 通用 ISA 函数 `copy_gm_to_cbuf_multi_nd2nz` 但传 `false, false` 替代 cacheMode/enableSmallC0
- 使用 `set_mte2_nz_para` 寄存器设置（3113 起支持）
- B32 完整实现（与 l300 不同）
- 不支持 bfloat16_t

### 关键差异点

1. **ISA 函数签名三族**
   - l300：`copy_gm_to_cbuf_multi_nd2nz_b16`/`_b8`（专用函数，参数列表完全不同——直接传所有 stride，不用 `set_mte2_nz_para`）
   - l310/l311：`copy_gm_to_cbuf_multi_nd2nz(dst, src, 0, loop1SrcStride, nValue, dValue, loop4SrcStride, false, false)`（9 参数，最后两值为 `false` 替代 cacheMode/enableSmallC0）
   - 3510/5161：`copy_gm_to_cbuf_multi_nd2nz(dst, src, 0/sid, loop1SrcStride, cacheMode, nValue, dValue, loop4SrcStride, enableSmallC0)`（9 参数但第 3、5、9 位含义不同）

2. **dav_l300 B32 桩函数** — float/int32_t/uint32_t 的 GM→L1 Nd2Nz 在 3003 上不支持

3. **dav_l310/l311 缺少 bfloat16_t 重载** — l300 有 bfloat16_t 重载但 l310/l311 没有

4. **UB Nd2Nz 路径普遍缺失**
   - l300/l310/l311：完全未定义（若 UB 路径触发将链接错误）
   - 5161：桩函数但签名不匹配（无 cacheMode 参数，但包装层传入 cacheMode）
   - 仅 3510 有完整 UB Nd2Nz 实现

5. **3113 在 V1 声明分支但实现走旧式重载** — 3113 在声明 arch 守卫中（使用 enableSmallC0 模板），但版本实现是非模板重载（不支持 enableSmallC0）。包装层通过内部 arch 守卫排除了 3113 的 enableSmallC0 路径（L138-139），且 L1 路径走无 cacheMode 分支（L155-156），逻辑正确但风格不一致

6. **5161 使用 `intriParams.sid`** — Nd2Nz ISA 调用用 `sid` 替代 3510 的硬编码 `0`

7. **3510 独有 AIV TSCM 路径** — 通过 `ScmDataCopyND2NZMsg` 支持 AIV 调用，其他版本无此路径

## 待确认问题

1. **dav_l300 B32 桩函数** — `DataCopyGM2L1ND2NZImplB32` 是 ASSERT false 桩函数，float/int32_t/uint32_t 的 GM→L1 Nd2Nz 在 3003 上是硬件不支持还是待补全？（L300 系列无 B32 专用 ISA 函数 `copy_gm_to_cbuf_multi_nd2nz_b32`，疑似硬件不支持）

2. **dav_l310/l311 缺少 bfloat16_t 重载** — l300 有 bfloat16_t 重载但 l310/l311 没有，是硬件不支持还是遗漏？

3. **UB Nd2Nz 路径缺失** — l300/l310/l311 的 `DataCopyGM2UBND2NZImpl` 完全未定义。若用户调用 `DataCopy(UB_tensor, GM_tensor, Nd2NzParams)` 将导致链接错误。是硬件不支持（应加桩函数）还是待补全？

4. **dav_5161 UB 桩函数签名不匹配** — 包装层传入 4 参数（含 cacheMode），但 5161 定义的是 3 参数版本。这是编译错误还是有意为之（可能另有 4 参数重载未发现）？

5. **dav_l310 Nd2Nz 全桩函数**（已确认）— 3103 不支持 `set_mte2_nz_para`、`copy_gm_to_cbuf_multi_nd2nz`、`copy_gm_to_cbuf_multi_nd2nz_b16`/`_b8`，Nd2Nz GM→L1 路径全部为桩函数，维持现状。

6. **3113 在 V1 声明分支但不支持 enableSmallC0** — 3113 使用 V1 声明（含 enableSmallC0 模板参数），但实现层不支持 enableSmallC0。包装层通过内部 arch 守卫排除了 3113 的 enableSmallC0 路径（L138-139），逻辑正确但风格不一致。是否应该将 3113 从声明 arch 守卫中移除，或为 3113 补全 enableSmallC0 支持？

7. **dav_l300 ISA 函数差异** — l300 使用 `copy_gm_to_cbuf_multi_nd2nz_b16`/`_b8`（带后缀的专用函数），而 3113+ 使用 `copy_gm_to_cbuf_multi_nd2nz`（通用函数）。这是 ISA 版本差异（L300 系列仅有旧式专用函数，3113 起引入通用函数）还是软件抽象差异？

8. **dav_l310 已修正为桩函数**（已解决）— 3103 不支持 Nd2Nz 相关的所有 ISA 函数，B16/B32/B8 辅助函数已全部改为 ASSERT false 桩函数。
