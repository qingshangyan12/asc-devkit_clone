# 案例分析：DataCopy(GM→L1 Dn2Nz) 接口跨版本适配

## 接口概述

`DataCopy(LocalTensor<T>& dst, GlobalTensor<T>& src, Dn2NzParams& intriParams)` 用于 GlobalTensor(OUT) → LocalTensor(L1) 的 DN→NZ 格式转换数据搬运。

| 变体 | 行号 | 参数类型 | 模板参数 | Arch 守卫 | 守卫中的版本 | 未守卫版本 |
|------|------|---------|---------|-----------|-------------|-----------|
| Dn2Nz | L117 | `Dn2NzParams` | `<T, bool enableSmallC0>` | L115-116: `3510\|\|5102\|\|5101\|\|5161\|\|5165\|\|5163` | 3510, 5102, 5101, 5161, 5165, 5163 | **3003, 3103, 3113**（无 else 分支，完全无此接口） |

**关键发现**：
- 声明守卫**不含 3113**（Nd2Nz 守卫含 3113，Dn2Nz 不含），说明 Dn2Nz 在 3113 上不支持
- **无 else 分支** — 3003/3103/3113 编译时此接口完全不存在，不会导致编译错误
- `enableSmallC0` 作为**函数参数**传入（非模板参数），与 Nd2Nz 的模板参数模式不同

### 硬件特性支持范围

| 硬件特性 | 支持起始版本 | 3003 | 3103 | 3113 | 3510 | 5101 |
|---------|------------|------|------|------|------|------|
| `set_mte2_nz_para` | 自 311 起 | ❌ | ❌ | ✅ | ✅ | ✅ |
| `copy_gm_to_cbuf_multi_dn2nz` | 自 3510 起 | ❌ | ❌ | ❌ | ✅ | ✅ |
| `enableSmallC0` | 自 v510 起 | ❌ | ❌ | ❌ | ❌ | ✅ |
| `cacheMode` | 自 3510 起 | ❌ | ❌ | ❌ | ✅ | ✅ |

**影响分析**：
- **3003/3103**：不支持 `set_mte2_nz_para` 和 `copy_gm_to_cbuf_multi_dn2nz` → 声明层已排除，无影响
- **3113**：支持 `set_mte2_nz_para` 但不支持 `copy_gm_to_cbuf_multi_dn2nz` → 声明层已排除，无影响
- **3510**：支持 `set_mte2_nz_para` 和 `copy_gm_to_cbuf_multi_dn2nz`，`enableSmallC0` 作为运行时参数传入 → 完整实现
- **5101**：全部支持 → 完整实现

> **注意**：Dn2Nz 的 ISA 函数 `copy_gm_to_cbuf_multi_dn2nz` 自 3510 起支持，比 Nd2Nz 的 `copy_gm_to_cbuf_multi_nd2nz`（自 311 起）更晚。这解释了为什么 Dn2Nz 声明守卫不含 3113 — 3113 硬件有 `set_mte2_nz_para` 但无 `copy_gm_to_cbuf_multi_dn2nz` 指令。

## 调用链

```
kernel_operator_data_copy_intf.h:115-120（声明，有 arch 守卫 L115-116）
  → kernel_operator_data_copy_intf_impl.h:220-237（公共包装，有 arch 守卫 L203-205）
    → [L1 路径] DataCopyGM2L1DN2NZImpl(dst, src, intriParams, enableSmallC0, cacheMode)  [版本特定]
      → DataCopyGM2L1DN2NZImplBase(dst, src, intriParams, enableSmallC0, cacheMode)  [版本特定]
        → set_mte2_nz_para + copy_gm_to_cbuf_multi_dn2nz  [硬件指令]
```

### 公共包装层（intf_impl.h:220-237）

**编译于 5102/3510/5101/5161/5165/5163**（L203-205，与声明守卫一致）：

```cpp
template <typename T, bool enableSmallC0>
__aicore__ inline __inout_pipe__(MTE2) void DataCopy(
    const LocalTensor<T>& dst, const GlobalTensor<T>& src, const Dn2NzParams& intriParams)
{
#ifdef ASCENDC_TIME_STAMP_ON
    PrintTimeStamp(static_cast<uint32_t>(TimeStampId::TIME_STAMP_MTE2_DATACOPY));
#endif
    using PrimType = PrimT<T>;
    const Hardware dstHWPos = GetPhyType((TPosition)dst.GetPosition());
    if (dstHWPos == Hardware::L1) { // GM -> L1
        const uint8_t cacheMode = ExtractCacheMode(src);
        DataCopyGM2L1DN2NZImpl(
            (__cbuf__ PrimType*)dst.GetPhyAddr(), (__gm__ PrimType*)src.GetPhyAddr(), intriParams, enableSmallC0,
            cacheMode);
        return;
    }
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "DataCopy dn2nz only support position:GM to position: L1"); });
}
```

**特点**：
- 仅支持 GM→L1 路径（无 UB 路径，与 Nd2Nz 不同）
- `enableSmallC0` 作为 `bool` 函数参数传入 `DataCopyGM2L1DN2NZImpl`（非模板参数）
- `cacheMode` 提取后传入

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

| 版本 | Arch | 声明守卫 | DataCopyGM2L1DN2NZImpl | DataCopyGM2L1DN2NZImplBase | 状态 |
|------|------|---------|----------------------|---------------------------|------|
| dav_3510 | 3510 | ✅ 包含 | **完整**（L416-433） | **完整**（L381-414） | ✅ 完整（参照） |
| dav_l300 | 3003 | ❌ 不包含 | **未定义** | **未定义** | ❌ 接口不存在 |
| dav_l310 | 3103 | ❌ 不包含 | **未定义** | **未定义** | ❌ 接口不存在 |
| dav_l311 | 3113 | ❌ 不包含 | **未定义** | **未定义** | ❌ 接口不存在 |
| dav_5161 | 5101 | ✅ 包含 | **完整**（L379-396） | **完整**（L346-377） | ✅ 完整 |

### 支持的数据类型矩阵

| 版本 | half | bfloat16_t | int16_t | uint16_t | float | int32_t | uint32_t | int8_t | uint8_t | fp4x2 |
|------|------|-----------|---------|----------|-------|---------|----------|--------|---------|-------|
| dav_3510 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |
| dav_l300 | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ |
| dav_l310 | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ |
| dav_l311 | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ | ❌ |
| dav_5161 | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ | ✅ |

> 3003/3103/3113 因声明层已排除，所有类型均不可用。

## 各版本原始代码

---

### dav_3510（参照版本）

#### DataCopyGM2L1DN2NZImplBase — 完整实现（L381-414）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1DN2NZImplBase(
    __cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams, bool enableSmallC0, const uint8_t cacheMode = 0)
{
    if ASCEND_IS_AIC {
        SmallC0Checker(enableSmallC0, intriParams.dValue);

        if constexpr (g_gm_overflow_check) {
            __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
            AscendCUtils::CheckGmMemOverflowDn2Nz(src, workSpace, true, intriParams);
        }

        uint16_t loop2DstStride = intriParams.dstNzNStride;  // loop2_dst_stride = dst_nz_n_stride
        uint16_t loop3DstStride = intriParams.dstNzC0Stride; // loop3_dst_stride = dst_nz_c0_Stride
        // loop4_dst_stride: dst_nz_matrix_stride * size_of_dst_type / C0_size
        uint16_t loop4DstStride = static_cast<uint16_t>(intriParams.dstNzMatrixStride * sizeof(T) / ONE_BLK_SIZE);

        uint64_t mte2NzPara = static_cast<uint64_t>(loop4DstStride) << 48; // MTE2_NZ_PARA[63:48]
        mte2NzPara |= static_cast<uint64_t>(loop3DstStride) << 32;         // MTE2_NZ_PARA[47:32]
        mte2NzPara |= static_cast<uint64_t>(loop2DstStride) << 16;         // MTE2_NZ_PARA[31:16]
        mte2NzPara |= static_cast<uint64_t>(intriParams.dnNum);            // MTE2_NZ_PARA[15:0]
        set_mte2_nz_para(mte2NzPara); // CCE: store parameters for DN2NZ DMA instructions

        // input params: srcDValue, srcDnMatrixStride                        unit of element nums
        // expected params for ISA: loop1_src_stride, loop4_src_stride       uint of bytes
        // loop1SrcStride = srcD * sizeof(srcType)     loop4SrcStride = srcDnMatrixStride * sizeof(srcType)
        uint64_t loop1SrcStride = intriParams.srcDValue * sizeof(T);
        uint64_t loop4SrcStride = intriParams.srcDnMatrixStride * sizeof(T);

        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ T*)dst, (__gm__ T*)src, 0, loop1SrcStride, cacheMode, intriParams.nValue, intriParams.dValue,
            loop4SrcStride, enableSmallC0);
    }
}
```

#### DataCopyGM2L1DN2NZImpl — 完整实现（L416-433）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1DN2NZImpl(
    __cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams, bool enableSmallC0, const uint8_t cacheMode = 0)
{
    if constexpr (g_gm_overflow_check) {
        __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
        AscendCUtils::CheckGmMemOverflowDn2Nz(src, workSpace, true, intriParams);
    }
    if constexpr (SupportType<T, fp4x2_e2m1_t, fp4x2_e1m2_t>()) {
        ASCENDC_ASSERT((intriParams.nValue % 2 == 0), {
            KERNEL_LOG(KERNEL_ERROR, "if src datatype is 4bit, the inner axis must be an even number.");
        });
        // data is transferred based on the b8 type, and parameters are set based on the b8 type
        DataCopyGM2L1DN2NZImplBase((__cbuf__ uint8_t*)dst, (__gm__ uint8_t*)src, intriParams, enableSmallC0, cacheMode);
    } else {
        DataCopyGM2L1DN2NZImplBase((__cbuf__ T*)dst, (__gm__ T*)src, intriParams, enableSmallC0, cacheMode);
    }
}
```

---

### dav_5161（目标版本，5101）

#### DataCopyGM2L1DN2NZImplBase — 完整实现（L346-377）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1DN2NZImplBase(
    __cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams, bool enableSmallC0, const uint8_t cacheMode = 0)
{
    SmallC0Checker(enableSmallC0, intriParams.dValue);

    if constexpr (g_gm_overflow_check) {
        __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
        AscendCUtils::CheckGmMemOverflowDn2Nz(src, workSpace, true, intriParams);
    }

    uint16_t loop2DstStride = intriParams.dstNzNStride;  // loop2DstStride = dst_nz_n_stride
    uint16_t loop3DstStride = intriParams.dstNzC0Stride; // loop3_dst_stride = dst_nz_c0_Stride
    // loop4DstStride: dst_nz_matrix_stride * size_of_dst_type / C0_size
    uint16_t loop4DstStride = static_cast<uint16_t>(intriParams.dstNzMatrixStride * sizeof(T) / ONE_BLK_SIZE);

    uint64_t mte2NzPara = static_cast<uint64_t>(loop4DstStride) << 48; // MTE2_NZ_PARA[63:48]
    mte2NzPara |= static_cast<uint64_t>(loop3DstStride) << 32;         // MTE2_NZ_PARA[47:32]
    mte2NzPara |= static_cast<uint64_t>(loop2DstStride) << 16;         // MTE2_NZ_PARA[31:16]
    mte2NzPara |= static_cast<uint64_t>(intriParams.dnNum);            // MTE2_NZ_PARA[15:0]
    set_mte2_nz_para(mte2NzPara); // CCE: store parameters for DN2NZ DMA instructions

    // input params: srcDValue, srcDnMatrixStride                        unit of element nums
    // expected params for ISA: loop1_src_stride, loop4SrcStride       uint of bytes
    // loop1SrcStride = srcD * sizeof(srcType)     loop4SrcStride = srcDnMatrixStride * sizeof(srcType)
    uint64_t loop1SrcStride = intriParams.srcDValue * sizeof(T);
    uint64_t loop4SrcStride = intriParams.srcDnMatrixStride * sizeof(T);

    copy_gm_to_cbuf_multi_dn2nz(
        (__cbuf__ T*)dst, (__gm__ T*)src, intriParams.sid, loop1SrcStride, cacheMode, intriParams.nValue,
        intriParams.dValue, loop4SrcStride, enableSmallC0);
}
```

#### DataCopyGM2L1DN2NZImpl — 完整实现（L379-396）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1DN2NZImpl(
    __cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams, bool enableSmallC0, const uint8_t cacheMode = 0)
{
    if constexpr (g_gm_overflow_check) {
        __gm__ uint8_t* workSpace = GetSysWorkSpacePtr();
        AscendCUtils::CheckGmMemOverflowDn2Nz(src, workSpace, true, intriParams);
    }
    if constexpr (SupportType<T, fp4x2_e2m1_t, fp4x2_e1m2_t>()) {
        ASCENDC_ASSERT((intriParams.nValue % 2 == 0), {
            KERNEL_LOG(KERNEL_ERROR, "if src datatype is 4bit, the inner axis must be an even number.");
        });
        // data is transferred based on the b8 type, and parameters are set based on the b8 type
        DataCopyGM2L1DN2NZImplBase((__cbuf__ uint8_t*)dst, (__gm__ uint8_t*)src, intriParams, enableSmallC0, cacheMode);
    } else {
        DataCopyGM2L1DN2NZImplBase((__cbuf__ T*)dst, (__gm__ T*)src, intriParams, enableSmallC0, cacheMode);
    }
}
```

---

### dav_l300 / dav_l310 / dav_l311

> **三个版本均未定义** `DataCopyGM2L1DN2NZImpl` 和 `DataCopyGM2L1DN2NZImplBase`。由于 Dn2Nz 声明守卫（L115-116）不包含 3003/3103/3113，且包装函数守卫（L203-205）同样不包含这三个版本，Dn2Nz 接口在这些版本上**完全不可用**（编译时被排除），不会导致编译或链接错误。

## 代码级实现差异分析

### 实现风格对比

| 特征 | dav_3510 | dav_5161 |
|------|----------|----------|
| 模板签名 | `template <typename T>` + `bool enableSmallC0` 函数参数 | 同 3510 |
| ISA 函数 | `copy_gm_to_cbuf_multi_dn2nz` | 同 |
| ISA 第3参数 | `0` | `intriParams.sid` |
| AIC 守卫 | `if ASCEND_IS_AIC` | 无（直接执行） |
| SmallC0Checker 位置 | 在 AIC 守卫内 | 在 AIC 守卫外（无守卫） |
| fp4x2 支持 | ✅ | ✅ |
| overflow 检查 | Impl 层 + ImplBase 层 | Impl 层 + ImplBase 层 |
| cacheMode | ✅ | ✅ |
| enableSmallC0 | ✅（函数参数） | ✅（函数参数） |
| 所有基本类型 | ✅ | ✅ |

### 实现模式总结

**dav_3510 和 dav_5161 是仅有的两个实现版本，模式高度相似**：

**共同点**：
- 模板 `<typename T>` + `bool enableSmallC0` 函数参数（非模板参数）
- `set_mte2_nz_para` 寄存器设置 + `copy_gm_to_cbuf_multi_dn2nz` 硬件指令
- `sizeof(T)` if constexpr 不需要（Dn2Nz 直接用 `T` 类型传给 ISA，不做 B8/B16/B32 分派）
- fp4x2 支持：转 uint8_t 后调用 ImplBase
- overflow 检查在两层都做

**差异点**：
1. **ISA 第 3 参数** — 3510 传 `0`（硬编码），5161 传 `intriParams.sid`
2. **AIC 守卫** — 3510 有 `if ASCEND_IS_AIC` 守卫（仅 CubeCore 执行），5161 无守卫（直接执行）
3. **SmallC0Checker 位置** — 3510 在 AIC 守卫内，5161 在守卫外

### 关键差异点

1. **Dn2Nz 仅 3510/5161 可用** — 3003/3103/3113 在声明层被排除。原因是：
   - 3003/3103：不支持 `set_mte2_nz_para` 和 `copy_gm_to_cbuf_multi_dn2nz`
   - 3113：支持 `set_mte2_nz_para` 但不支持 `copy_gm_to_cbuf_multi_dn2nz`（Dn2Nz ISA 自 3510 起支持，比 Nd2Nz 的 311 起更晚）

2. **5161 无 AIC 守卫** — 3510 的 ImplBase 有 `if ASCEND_IS_AIC` 守卫，5161 没有。5161 上 AIV 调用 Dn2Nz 会直接执行 `copy_gm_to_cbuf_multi_dn2nz`，可能有问题

3. **enableSmallC0 作为函数参数而非模板参数** — 与 Nd2Nz 的 `<T, bool enableSmallC0>` 模板模式不同，Dn2Nz 使用 `template <typename T>` + `bool enableSmallC0` 函数参数。这是设计差异还是历史遗留？

4. **5161 使用 `intriParams.sid`** — Dn2Nz ISA 调用用 `sid` 替代 3510 的硬编码 `0`（与 Nd2Nz 相同模式）

5. **Dn2Nz 无类型重载** — 与 Nd2Nz 的 l300/l310/l311 非模板类型重载模式不同，Dn2Nz 仅用模板 `<typename T>` 统一处理所有类型

## 待确认问题

1. **Dn2Nz 在 3113 上不支持**（已确认）— 3113 支持 `set_mte2_nz_para` 但不支持 `copy_gm_to_cbuf_multi_dn2nz`，声明守卫正确排除了 3113。维持现状。

2. **Dn2Nz 在 3003/3103 上不支持**（已确认）— 3003/3103 不支持 `set_mte2_nz_para` 和 `copy_gm_to_cbuf_multi_dn2nz`，声明守卫正确排除了这两个版本。维持现状。

3. **dav_5161 无 AIC 守卫** — 3510 的 ImplBase 有 `if ASCEND_IS_AIC` 守卫（仅 CubeCore 执行），5161 没有。5161 上 AIV 调用 Dn2Nz 会直接执行 `copy_gm_to_cbuf_multi_dn2nz`，是否会导致问题？是遗漏还是 5161 硬件允许 AIV 执行此指令？

4. **enableSmallC0 作为函数参数而非模板参数** — Dn2Nz 使用 `template <typename T>` + `bool enableSmallC0` 函数参数，而 Nd2Nz 使用 `template <typename T, bool enableSmallC0>` 模板参数。这是设计差异还是历史遗留？是否需要统一？

5. **dav_5161 SmallC0Checker 在 AIC 守卫外** — 3510 的 SmallC0Checker 在 `if ASCEND_IS_AIC` 内，5161 在外。如果 5161 上 AIV 调用，SmallC0Checker 仍会执行（可能无意义），是否需要加 AIC 守卫？
