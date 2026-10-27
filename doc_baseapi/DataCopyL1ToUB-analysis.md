# 案例分析：DataCopyL1ToUB 接口跨版本适配

## 接口概述

`DataCopyL1ToUB` 用于 L1 Buffer(A1/B1) → UB(VECIN/VECOUT) 的数据搬运，有两个重载变体：

| 变体 | 参数类型 | 声明位置 | arch 守卫 |
|------|---------|---------|----------|
| V1 (count) | `uint32_t count` | `intf.h:453-454` | 无 |
| V2 (repeatParams) | `DataCopyParams& repeatParams` | `intf.h:456-458` | 无 |

两个声明均无单独的 `__NPU_ARCH__` 守卫，位于 `namespace AscendC` 内、文件末尾（L471 `}` 之前）。`intf.h` 文件本身也无文件级 arch 守卫（L24-25 仅有 include guard）。

### 模板参数

```cpp
template <typename T, uint8_t subBlockId = 0>
```

- `T`：数据类型
- `subBlockId`：子块 ID（默认 0），仅 3510 的底层实现使用此参数

### 关键参数结构体（`kernel_struct_data_copy.h:63-94`）

```cpp
struct DataCopyParams {
    uint16_t blockCount = DEFAULT_DATA_COPY_NBURST;
    uint16_t blockLen = 0;
    union { uint16_t srcGap = 0; uint16_t srcStride; };
    union { uint16_t dstGap = 0; uint16_t dstStride; };
    // 条件编译字段（5101/5161/5165/5163 才有）：
    //   uint8_t fixShiftVal;  (5102/5101/5161/5165/5163)
    //   uint8_t postProcBufBlock;  (5101/5161/5165/5163)
    //   uint8_t sid;  (5101/5161/5165/5163)
};
```

## 调用链

### V1 (count) 调用链

```
intf.h:453-454（声明，无 arch 守卫）
  → intf_impl.h:922-940（实现，函数体受 #if __NPU_ARCH__ == 3510 保护，无 #else）
    → 将 count 转换为 DataCopyParams.blockLen
    → 调用 DataCopyL1ToUB<T, subBlockId>(dst, src, repeatParams)  [委托 V2]
```

### V2 (repeatParams) 调用链

```
intf.h:456-458（声明，无 arch 守卫）
  → intf_impl.h:535-547（实现，函数体受 #if __NPU_ARCH__ == 3510 保护，无 #else）
    → DataCopyL12UBImpl<PrimType, subBlockId>(dst_ptr, src_ptr, repeatParams)
      → base_impl.h L32-55（#elif 分发，按 arch #include 对应 dav 目录）
        → dav_3510/kernel_operator_data_copy_impl.h:876-890（完整实现）
```

### 共享层分发逻辑（base_impl.h L32-55）

```cpp
#if __NPU_ARCH__ == 3510
#include "dav_3510/kernel_operator_data_copy_impl.h"
#elif (__NPU_ARCH__ == 3003)
#include "dav_l300/kernel_operator_data_copy_impl.h"
#elif (__NPU_ARCH__ == 3103)
#include "dav_l310/kernel_operator_data_copy_impl.h"
#elif (__NPU_ARCH__ == 3113)
#include "dav_l311/kernel_operator_data_copy_impl.h"
#elif (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
#include "dav_5161/kernel_operator_data_copy_impl.h"
#endif
```

> 每个 arch 包含各自的 `kernel_operator_data_copy_impl.h`，其中定义了 `DataCopyL12UBImpl`。

### 注意：DataCopyL1ToUB 与通用 DataCopy 的 L1→UB 路径是两条独立链路

| 路径 | 入口 | L1→UB 分发 | 5101/5161/5165/5163 可用？ |
|------|------|-----------|--------------------------|
| DataCopyL1ToUB | `DataCopyL1ToUB(dst, src, ...)` | 直接调 `DataCopyL12UBImpl` | **否**（函数体空，3510 专用） |
| 通用 DataCopy | `DataCopy(dst, src, repeatParams)` | 调 `DataCopyL12UBIntf` → `DataCopyL12UBImpl` | **否**（DataCopyL12UBIntf 未定义） |

## 支持矩阵

| 版本 | Arch | V1 (count) | V2 (repeatParams) | DataCopyL12UBImpl | DataCopyL12UBIntf |
|------|------|------------|-------------------|-------------------|-------------------|
| dav_l300 | 3003 | **空函数体** | **空函数体** | **完整** (`copy_cbuf_to_ubuf`) | **完整** |
| dav_l310 | 3103 | **空函数体** | **空函数体** | **完整** (`copy_cbuf_to_ubuf`) | **完整** |
| dav_l311 | 3113 | **空函数体** | **空函数体** | **完整** (`copy_cbuf_to_ubuf`) | **完整** |
| dav_5161 | 5101 | **空函数体** | **空函数体** | **完整** (`fix_cbuf_to_ubuf`) | **未定义** |
| dav_5161 | 5161 | **空函数体** | **空函数体** | **完整** (`fix_cbuf_to_ubuf`) | **未定义** |
| dav_5161 | 5165 | **空函数体** | **空函数体** | **完整** (`fix_cbuf_to_ubuf`) | **未定义** |
| dav_5161 | 5163 | **空函数体** | **空函数体** | **完整** (`fix_cbuf_to_ubuf`) | **未定义** |
| dav_3510 | 3510 | **完整** | **完整** | **完整** (`CopyCbufToUbuf`) | **桩** (`ASCENDC_ASSERT false`) |

> **空函数体** = `DataCopyL1ToUB` 声明可见，但 `#if __NPU_ARCH__ == 3510` 守卫使函数体为空，调用后无任何效果（静默无操作）。  
> **未定义** = 函数不存在于该 dav 目录的任何文件中。  
> **桩** = 函数存在但直接 `ASCENDC_ASSERT(false, ...)`。

## 各版本原始代码

### intf_impl.h — DataCopyL1ToUB V1 (count)（L922-940）

```cpp
template <typename T, uint8_t subBlockId>
__aicore__ inline void DataCopyL1ToUB(const LocalTensor<T>& dst, const LocalTensor<T>& src, const uint32_t count)
{
#if (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510))
    using PrimType = PrimT<T>;
    ASCENDC_ASSERT((count % AscendCUtils::GetC0Count(sizeof(PrimType)) == 0), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to "
            "check count value in DataCopy from LocalTensor to LocalTensor, count * sizeof(T) must be 32B align, "
            "current count value is %u. In NPU mode, no error is reported. The value is rounded down by 32B.",
            count);
    });
    ASCENDC_REPORT_OVERFLOW_MEM((CheckDataCopyTensorSizeOverflow(dst, src, count)));
    struct DataCopyParams repeatParams;
    repeatParams.blockLen = count / AscendCUtils::GetC0Count(sizeof(PrimType));
    DataCopyL1ToUB<T, subBlockId>(dst, src, repeatParams);
#endif
}
```

> **3510 守卫**：`#if __NPU_ARCH__ == 3510`，无 `#else`。非 3510 架构函数体为空。

### intf_impl.h — DataCopyL1ToUB V2 (repeatParams)（L535-547）

```cpp
template <typename T, uint8_t subBlockId>
__aicore__ inline void DataCopyL1ToUB(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& repeatParams)
{
#if (defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510))
    using PrimType = PrimT<T>;
    ASCENDC_REPORT_OVERFLOW_MEM(CheckDataCopyTensorSizeOverflow(dst, src, repeatParams));
    CheckTensorAlign<T>(dst, ONE_BLK_SIZE, "dst", "DataCopy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT)"); // 32B align
    CheckTensorAlign<T>(src, ONE_BLK_SIZE, "src", "DataCopy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT)"); // 32B align
    DataCopyL12UBImpl<PrimType, subBlockId>(
        (__ubuf__ PrimType*)dst.GetPhyAddr(), (__cbuf__ PrimType*)src.GetPhyAddr(), repeatParams);
#endif
}
```

> **3510 守卫**：`#if __NPU_ARCH__ == 3510`，无 `#else`。非 3510 架构函数体为空。

### dav_3510 — DataCopyL12UBImpl（L876-890）

```cpp
// SRC: L1
template <typename T, uint8_t subBlockId = 0>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
#if __MIX_CORE_AIC_RATION__ != 1
    static_assert(subBlockId < 2, "subBlockId must be less than 2 when AIC:AIV ratio is 1:2");
    CopyCbufToUbuf<T, subBlockId>(
        dst, src, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
#else
    ASCENDC_ASSERT((false), {
        KERNEL_LOG(
            KERNEL_ERROR, "unsupported data copy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT) on current device when "
                          "AIC:AIV ratio is 1:1");
    });
#endif
}
```

> 有 `subBlockId` 模板参数。AIC:AIV=1:1 时不支持，AIC:AIV=1:2 时通过 `CopyCbufToUbuf` 实现。

### dav_5161 — DataCopyL12UBImpl（L200-219）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    constexpr uint32_t POS_LOOP3_SIZE = 40;
    constexpr uint32_t POS_MODE = 61;
    constexpr uint32_t POS_LOOP4_SRC_STRIDE = 48;

    uint64_t xm = 0;
    xm |= ((uint64_t)intriParams.blockLen * 32 / sizeof(T)) << POS_LOOP3_SIZE;
    xm |= (uint64_t)fixp_trans_mode_t::NORMAL_DMA << POS_MODE;

    uint64_t xt = ((uint64_t)intriParams.blockLen + (uint64_t)intriParams.srcStride);

    uint64_t config = 0;
    config |= (uint64_t)intriParams.blockCount;
    config |= ((uint64_t)intriParams.blockLen + (uint64_t)intriParams.dstStride) << POS_LOOP4_SRC_STRIDE;

    set_fixp_nz_para(config);
    fix_cbuf_to_ubuf((__ubuf__ int8_t*)dst, (__cbuf__ int8_t*)src, xm, xt);
}
```

> **无** `subBlockId` 模板参数。使用 `fix_cbuf_to_ubuf` 硬件指令 + `set_fixp_nz_para` 寄存器配置。

### dav_l300 — DataCopyL12UBImpl（L151-157）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    copy_cbuf_to_ubuf(
        (__ubuf__ void*)dst, (__cbuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}
```

> **无** `subBlockId` 模板参数。使用 `copy_cbuf_to_ubuf` 硬件指令。

### dav_l310 — DataCopyL12UBImpl（L182-188）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    copy_cbuf_to_ubuf(
        (__ubuf__ void*)dst, (__cbuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}
```

> 与 dav_l300 完全相同。

### dav_l311 — DataCopyL12UBImpl（L182-188）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    copy_cbuf_to_ubuf(
        (__ubuf__ void*)dst, (__cbuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}
```

> 与 dav_l300/dav_l310 完全相同。

### DataCopyL12UBIntf（通用 DataCopy L1→UB 路径的中间层）

| 版本 | 行号 | 状态 | 代码 |
|------|------|------|------|
| dav_3510 | L1410-1417 | **桩** | `ASCENDC_ASSERT(false, {"unsupported data copy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT)"});` |
| dav_l300 | L871-876 | **完整** | `DataCopyL12UBImpl(dst, src, intriParams);` |
| dav_l310 | L916-921 | **完整** | `DataCopyL12UBImpl(dst, src, intriParams);` |
| dav_l311 | L967-972 | **完整** | `DataCopyL12UBImpl(dst, src, intriParams);` |
| dav_5161 | — | **未定义** | 函数不存在于 dav_5161 的任何文件中 |

## 代码级实现差异分析

### 风格对比表

| 维度 | dav_3510 | dav_5161 | dav_l300 | dav_l310 | dav_l311 |
|------|---------|---------|---------|---------|---------|
| 硬件指令 | `CopyCbufToUbuf` | `fix_cbuf_to_ubuf` | `copy_cbuf_to_ubuf` | `copy_cbuf_to_ubuf` | `copy_cbuf_to_ubuf` |
| subBlockId 模板参数 | ✅ 有 (`<T, subBlockId>`) | ❌ 无 (`<T>`) | ❌ 无 (`<T>`) | ❌ 无 (`<T>`) | ❌ 无 (`<T>`) |
| 寄存器配置 | 无 | `set_fixp_nz_para(config)` | 无 | 无 | 无 |
| 位拼接 | 无 | `POS_LOOP3_SIZE`/`POS_MODE`/`POS_LOOP4_SRC_STRIDE` | 无 | 无 | 无 |
| 类型转换 | 无 | `(__ubuf__ int8_t*)` / `(__cbuf__ int8_t*)` 强转 | `(__ubuf__ void*)` / `(__cbuf__ void*)` | 同 l300 | 同 l300 |
| AIC:AIV 检查 | `__MIX_CORE_AIC_RATION__` | 无 | 无 | 无 | 无 |
| 类型检查 | `static_assert(subBlockId < 2)` | 无 | 无 | 无 | 无 |

### 实现模式总结

1. **dav_3510 模式**：`CopyCbufToUbuf` + `subBlockId` 模板参数 + AIC:AIV 比例检查。是唯一支持 `subBlockId` 的版本。
2. **dav_5161 模式**：`fix_cbuf_to_ubuf` + `set_fixp_nz_para` 寄存器配置。通过位拼接构建 `xm`/`xt`/`config` 参数，调用 fixpipe 专用 DMA 指令。
3. **dav_l300/l310/l311 模式**：`copy_cbuf_to_ubuf`，直接透传参数，最简单的实现。三个版本代码完全相同。

### 关键差异点

1. **subBlockId 不兼容**：`DataCopyL1ToUB` 的 V2 调用 `DataCopyL12UBImpl<PrimType, subBlockId>(...)`（两个模板参数），但仅 dav_3510 的 `DataCopyL12UBImpl` 接受 `subBlockId`。其他版本的签名是 `template <typename T>`，若取消 3510 守卫将导致编译错误。

2. **DataCopyL1ToUB 仅 3510 可用**：两个重载的函数体均被 `#if __NPU_ARCH__ == 3510` 包裹，无 `#else` 分支。对 3003/3103/3113/5101/5161/5165/5163，调用 `DataCopyL1ToUB` 编译通过但**静默无操作**（空函数体）。

3. **通用 DataCopy L1→UB 路径在 5161 缺失**：`DataCopyL12UBIntf` 在 dav_5161 中未定义，而 `intf_impl.h:402` 的通用 `DataCopy(L1→UB)` 路径无 arch 守卫地调用它。若在 5101/5161/5165/5163 上调用 `DataCopy(L1_tensor, UB_tensor, repeatParams)`，将导致**链接错误**（未定义符号）。

4. **dav_3510 的 L1→UB 矛盾**：`DataCopyL1ToUB` 仅在 3510 有实现，但 3510 的 `DataCopyL12UBIntf`（通用 DataCopy 路径）却是桩函数（assert false）。说明 3510 上 L1→UB 只能通过专用 `DataCopyL1ToUB` API 调用，不能通过通用 `DataCopy` 调用。

5. **dav_l300/l310/l311 的 L1→UB 路径**：`DataCopyL1ToUB` 不可用（空函数体），但通用 `DataCopy(L1→UB)` 通过 `DataCopyL12UBIntf` → `DataCopyL12UBImpl` 可用。

## 待确认问题

1. **DataCopyL1ToUB 为何仅 3510 可用？底层 DataCopyL12UBImpl 在所有版本都存在。**
   - 3003/3103/3113 的 `DataCopyL12UBImpl` 已完整实现，但 `DataCopyL1ToUB` 的 `#if 3510` 守卫阻止了调用
   - 是有意设计（这些版本通过通用 `DataCopy` 走 L1→UB），还是遗漏了守卫扩展？
   - **需确认**：3003/3103/3113 是否应该开放 `DataCopyL1ToUB`（去掉 3510 限制，或扩展守卫条件）？

2. **subBlockId 模板参数如何适配非 3510 版本？**
   - `DataCopyL1ToUB` 声明了 `subBlockId` 模板参数，但非 3510 版本的 `DataCopyL12UBImpl` 不接受此参数
   - 若开放 3510 守卫，需要：要么给非 3510 的 `DataCopyL12UBImpl` 添加 `subBlockId` 参数（忽略或传递给硬件），要么在 `DataCopyL1ToUB` 中根据 arch 分发不同调用方式
   - **需确认**：3003/3103/3113/5101/5161/5165/5163 的硬件是否支持 subBlockId 概念？

3. **dav_5161 的 DataCopyL12UBIntf 缺失是遗漏还是有意？**
   - 通用 `DataCopy(L1→UB)` 在 `intf_impl.h:402` 无 arch 守卫地调用 `DataCopyL12UBIntf`
   - dav_5161 未定义此函数，编译时若实例化该路径将报错
   - **需确认**：5101/5161/5165/5163 是否需要支持 L1→UB 数据搬运？若需要，应补全 `DataCopyL12UBIntf`；若不需要，应在 `intf_impl.h:402` 添加 arch 守卫排除 5161 系列

4. **dav_3510 的 DataCopyL12UBIntf 为何是桩函数？**
   - 3510 有 `DataCopyL12UBImpl` 的完整实现（通过 `CopyCbufToUbuf`），但 `DataCopyL12UBIntf` 却直接 assert false
   - 3510 上 L1→UB 只能通过 `DataCopyL1ToUB` 专用 API 调用，不能通过通用 `DataCopy`
   - **需确认**：这是有意的设计约束（通用 DataCopy 不支持 L1→UB），还是 `DataCopyL12UBIntf` 应该委托 `DataCopyL12UBImpl`？
