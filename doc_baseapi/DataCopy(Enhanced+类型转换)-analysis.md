# DataCopy（Enhanced Local→Local + 类型转换变体）跨版本支持情况分析报告

> 分析对象：`asc-devkit\include\basic_api\kernel_operator_data_copy_intf.h` 中 8 个 DataCopy 变体声明（L308/L323/L333/L342/L351/L360/L369/L378 附近）
> 目标版本：3003（dav_l300）、3103（dav_l310）、3113（dav_l311）、5101/5161/5165/5163（dav_5161）
> 参照版本：3510（dav_3510），另覆盖 1001/2002/2201/3002/3102/5102 共 11 个架构版本

---

## 1. 接口概述

本次分析的 8 个变体分为两类：

- **V1（同类型 Enhanced Local→Local）**：`DataCopy(LocalTensor dst, LocalTensor src, DataCopyParams, DataCopyEnhancedParams)`，按 src/dst 位置运行时分发 6 条路径（UB→L1、UB→L0C、UB→UB、L1→UB、L1→L0C、L0C→UB）。
- **V2~V8（类型转换变体）**：src/dst dtype 不同，路径固定（L1→L0C 或 L0C→UB 或 UB→L0C）。

| 变体 | 声明位置（intf.h） | dtype（dst←src） | 硬件路径 | 管道属性 | 声明层宏控 |
|---|---|---|---|---|---|
| V1 | L308-311 | T←T | 运行时分发 6 条路径 | 无 | 无 |
| V2 | L321-330 | bfloat16_t←float | L1→L0C | 无 | `#if (__NPU_ARCH__ == 2201)`（仅 2201 可见） |
| V3 | L332-339 | half←float | L1→L0C 或 L0C→UB | 无 | 无 |
| V4 | L341-348 | half←int32_t | L0C→UB | `__inout_pipe__(V)` | 无 |
| V5 | L350-357 | int16_t←int32_t | L0C→UB | `__inout_pipe__(V)` | 无 |
| V6 | L359-366 | int8_t←int32_t | L0C→UB | `__inout_pipe__(V)` | 无 |
| V7 | L368-375 | uint8_t←int32_t | L0C→UB | `__inout_pipe__(V)` | 无 |
| V8 | L377-384 | float←half | UB→L0C | `__inout_pipe__(V)` | 无 |

注：L298-306 的 GM↔Local Enhanced 两个变体（MTE2/MTE3）及 L313-319 的 `DataCopyCO12DstParams`（CO12Dst）变体不在本次分析范围内。

---

## 2. 调用链追踪

### 2.1 分层结构（自上而下 4 层）

```
声明层  include/basic_api/kernel_operator_data_copy_intf.h                    （8 个声明）
  ↓
包装层  impl/basic_api/kernel_operator_data_copy_intf_impl.h                  （位置/dtype 检查 + 分发）
  ↓
共享层  impl/basic_api/kernel_operator_data_copy_base_impl.h  L32-55          （按 __NPU_ARCH__ 宏控包含版本文件）
  ↓
版本层  impl/basic_api/dav_XXXX/kernel_operator_data_copy_impl.h              （11 个版本目录）
```

### 2.2 共享层版本分发宏控（base_impl.h L32-55 原文）

```cpp
#if __NPU_ARCH__ == 1001
#include "dav_c100/kernel_operator_data_copy_impl.h"
#elif __NPU_ARCH__ == 2002
#include "dav_m200/kernel_operator_data_copy_impl.h"
#elif __NPU_ARCH__ == 2201
#include "dav_c220/kernel_operator_data_copy_impl.h"
#include "dav_c220/kernel_operator_set_atomic_impl.h"
#elif __NPU_ARCH__ == 3002
#include "dav_m300/kernel_operator_data_copy_impl.h"
#elif __NPU_ARCH__ == 3102
#include "dav_m310/kernel_operator_data_copy_impl.h"
#elif __NPU_ARCH__ == 3510
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

### 2.3 V1 包装层分发逻辑（intf_impl.h L1145-1211）

```cpp
template <typename T>
__aicore__ inline void DataCopy(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
#ifdef __MSTX_DFX_REPORT__
    MstxTensor::GetMstxDataCopyInfo(dst, src, intriParams, "DataCopy");
#endif
    using PrimType = PrimT<T>;
    const Hardware dstHWPos = GetPhyType((TPosition)dst.GetPosition());
    const Hardware srcHWPos = GetPhyType((TPosition)src.GetPosition());
    ASCENDC_REPORT_OVERFLOW_MEM((CheckDataCopyTensorSizeOverflow(dst, src, intriParams, enhancedParams)));

    if (srcHWPos == Hardware::UB) {
        if (dstHWPos == Hardware::L1) {
            DataCopyUB2L1Impl((__cbuf__ PrimType*)dst.GetPhyAddr(), (__ubuf__ PrimType*)src.GetPhyAddr(), intriParams);      // 直调 Impl
        } else if (dstHWPos == Hardware::L0C) {
            DataCopyUB2L0CIntf(dst, src, intriParams, enhancedParams);                                                          // 经 Intf
        } else if (dstHWPos == Hardware::UB) {
            DataCopyUB2UBIntf(dst, src, intriParams);                                                                           // 经 Intf
        } else { /* ASCENDC_CHECK_TPOSITION 报错 */ }
    } else if (srcHWPos == Hardware::L1) {
        if (dstHWPos == Hardware::UB) {
            DataCopyL12UBIntf(dst, src, intriParams);                                                                           // 经 Intf
        } else if (dstHWPos == Hardware::L0C) {
            DataCopyL12L0CImpl((__cc__ PrimType*)dst.GetPhyAddr(), (__cbuf__ PrimType*)src.GetPhyAddr(), intriParams, enhancedParams); // 直调 Impl
        } else { /* 报错 */ }
    } else if (srcHWPos == Hardware::L0C) {
        if (dstHWPos == Hardware::UB) {
            DataCopyL0C2UBImpl((__ubuf__ PrimType*)dst.GetPhyAddr(), (__cc__ PrimType*)src.GetPhyAddr(), intriParams, enhancedParams); // 直调 Impl
        } else { /* 报错 */ }
    } else { /* 报错（2002 与其余架构错误文案不同） */ }
}
```

要点：
- UB→L0C、UB→UB、L1→UB 三条路径经 **Intf 包装函数**（`DataCopyUB2L0CIntf` / `DataCopyUB2UBIntf` / `DataCopyL12UBIntf`）转调 Impl；
- UB→L1、L1→L0C、L0C→UB 三条路径**直调 Impl**；
- 该包装层本身无版本宏控（仅 L1200 `#if __NPU_ARCH__ == 2002` 影响错误文案）。

### 2.4 V2~V8 包装层（intf_impl.h L1242-1423）

| 变体 | 位置 | 前置检查 | 落点 |
|---|---|---|---|
| V2 bf16←f32 | L1242-1276（整体在 `#if (__NPU_ARCH__ == 2201)` 内） | src=L1、dst=L0C | `DataCopyL12L0CImpl((__cc__ PrimT<T>*)dst, (__cbuf__ PrimT<U>*)src, ...)` |
| V3 f16←f32 | L1278-1320 | src=L1→dst=L0C；src=L0C→dst=UB | `DataCopyL12L0CImpl((__cc__ half*, __cbuf__ float*)` / `DataCopyL0C2UBImpl((__ubuf__ half*, (__cc__ float*)` |
| V4 f16←s32 | L1333-1348 | `CheckTensorL0C2UB`（src=CO1、dst=CO2） | `DataCopyL0C2UBImpl((__ubuf__ half*)dst, (__cc__ int32_t*)src, ...)` |
| V5 s16←s32 | L1350-1366 | 同上 | `DataCopyL0C2UBImpl((__ubuf__ int16_t*, (__cc__ int32_t*)` |
| V6 s8←s32 | L1368-1384 | 同上 | `DataCopyL0C2UBImpl((__ubuf__ int8_t*, (__cc__ int32_t*)` |
| V7 u8←s32 | L1386-1402 | 同上 | `DataCopyL0C2UBImpl((__ubuf__ uint8_t*, (__cc__ int32_t*)` |
| V8 f32←f16 | L1404-1423 | `CheckTensorPos`（src=UB/CO2、dst=L0C/CO1） | `DataCopyUB2L0CImpl((__cc__ float*)dst, (__ubuf__ half*)src, ...)` |

V4~V8 均带 `__inout_pipe__(V)` 属性，公共前置含 `__MSTX_DFX_REPORT__` 埋点与 `ASCENDC_REPORT_OVERFLOW_MEM` 溢出检查。

### 2.5 版本层函数命名约定

| 函数 | 管道路径 | 被哪些变体使用 |
|---|---|---|
| DataCopyUB2L1Impl | UB→L1 | V1 |
| DataCopyUB2L0CImpl / DataCopyUB2L0CIntf | UB→L0C | V1（经 Intf）、V8（直调 Impl） |
| DataCopyUB2UBImpl / DataCopyUB2UBIntf | UB→UB | V1（经 Intf） |
| DataCopyL12UBImpl / DataCopyL12UBIntf | L1→UB | V1（经 Intf） |
| DataCopyL12L0CImpl | L1→L0C | V1（直调）、V2、V3 |
| DataCopyL0C2UBImpl | L0C→UB | V1（直调）、V3、V4~V7 |

---

## 3. 支持矩阵

### 3.1 底层函数 × 版本

| 底层函数 | 1001 c100 | 2002 m200 | 2201 c220 | 3002 m300 | 3003 l300 | 3102 m310 | 3103 l310 | 3113 l311 | 5102 m510 | 5101 | 5161/5165/5163 | 3510（参照） |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| DataCopyUB2L1Impl | ✓ | AIC✓ / VEC桩 | ✓（软件模拟） | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓（条件宏控） |
| DataCopyUB2UBImpl | ✓ | AIC✓ / VEC桩 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| DataCopyUB2UBIntf | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | **缺失** | **缺失** | ✓ |
| DataCopyL12UBImpl | ✓ | AIC✓ / VEC桩 | 桩 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓（FIX 指令族） | ✓（FIX 指令族） | 条件：MIX≠1✓ / 1:1桩 |
| DataCopyL12UBIntf | ✓ | ✓ | ✓（→桩） | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | **缺失** | **桩** |
| DataCopyUB2L0CImpl | ✓ | AIC✓ / VEC桩；int8 特化桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 |
| DataCopyUB2L0CIntf | ✓ | ✓ | ✓（→桩） | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | **缺失** | **缺失** | ✓（→桩） |
| DataCopyL0C2UBImpl | ✓ | AIC✓ / VEC桩；int8 特化桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩（提示用 FixPipe） |
| DataCopyL12L0CImpl | 桩 | 桩 | **✓** | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 |

说明：
- "AIC✓ / VEC桩"：m200 上 Cube 相关函数整体被 `#if defined(__DAV_M200_VEC__)` 核型宏控分为两份，VEC 核编译时为 `ASCENDC_REPORT_NOT_SUPPORT` 桩。
- "缺失"：dav_5161 中该 Intf 函数**未定义**。由于包装层模板是惰性实例化的，未调用不报错；一旦用户在对应架构上走到该路径（V1 的 UB→UB / UB→L0C / L1→UB），**编译直接失败**（undeclared identifier）。
- 2201 的 `DataCopyL12UBImpl` 为桩，且桩体被 `#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)` 包裹——设备构建下函数体为空（静默无操作）。

### 3.2 八个接口变体 × 版本可用性

V1（同类型 Enhanced，6 条路径）：

| V1 路径 | 1001 | 2002 | 2201 | 3002 | 3003 | 3102 | 3103 | 3113 | 5102 | 5101 | 5161/5165/5163 | 3510 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| UB→L1 | ✓ | AIC✓ | ✓软件模拟 | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| UB→UB | ✓ | AIC✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | **编译失败** | **编译失败** | ✓ |
| UB→L0C | ✓ | AIC✓ | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | **编译失败** | **编译失败** | 桩 |
| L1→UB | ✓ | AIC✓ | 桩（设备静默） | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | **编译失败** | 桩（Intf 桩） |
| L1→L0C | 桩 | 桩 | **✓** | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 |
| L0C→UB | ✓ | AIC✓ | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 |

V2~V8（类型转换变体）：

| 变体 | 路径 | 1001 | 2002 | 2201 | 3002 | 3003 | 3102 | 3103 | 3113 | 5102 | 5101/5161/5165/5163 | 3510 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| V2 bf16←f32 | L1→L0C | 声明不可见 | 声明不可见 | **✓** | 声明不可见 | 声明不可见 | 声明不可见 | 声明不可见 | 声明不可见 | 声明不可见 | 声明不可见 | 声明不可见 |
| V3 f16←f32 | L1→L0C | 桩 | 桩 | **✓** | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 |
| V3 f16←f32 | L0C→UB | **✓** | AIC✓ | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 |
| V4 f16←s32 | L0C→UB | **✓** | AIC✓ | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 |
| V5 s16←s32 | L0C→UB | **✓** | AIC✓ | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 |
| V6 s8←s32 | L0C→UB | **✓** | AIC✓ | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 |
| V7 u8←s32 | L0C→UB | **✓** | AIC✓ | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 |
| V8 f32←f16 | UB→L0C | **✓** | AIC✓ | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 | 桩 |

注：V2 声明被 `#if (__NPU_ARCH__ == 2201)` 宏控排除，其余架构上该重载不存在，用户代码写 bf16←f32 组合将直接编译失败（无其他可匹配重载）。V6 在 m200 上调用的是 `<int8_t, int32_t>` 模板版（完整），不受 int8_t 同类型特化桩影响。

### 3.3 目标版本结论

| 目标架构 | V1 可用路径 | V1 不可用路径 | V2 | V3~V8 |
|---|---|---|---|---|
| 3003（dav_l300） | UB→L1、UB→UB、L1→UB | UB→L0C、L1→L0C、L0C→UB（桩，设备静默） | 声明不可见 | 全部桩（设备静默） |
| 3103（dav_l310） | UB→L1、UB→UB、L1→UB | 同上 | 声明不可见 | 全部桩 |
| 3113（dav_l311） | UB→L1、UB→UB、L1→UB | 同上 | 声明不可见 | 全部桩 |
| 5101（dav_5161） | UB→L1、L1→UB（FIX 指令族） | UB→UB、UB→L0C（**Intf 缺失→编译失败**）；L1→L0C、L0C→UB（桩） | 声明不可见 | 全部桩 |
| 5161/5165/5163（dav_5161） | UB→L1 | UB→UB、UB→L0C、L1→UB（**Intf 缺失→编译失败**）；L1→L0C、L0C→UB（桩） | 声明不可见 | 全部桩 |
| 3510（参照） | UB→L1、UB→UB | L1→UB（Intf 桩）、UB→L0C、L1→L0C、L0C→UB（桩） | 声明不可见 | 全部桩 |

桩在 CPU 调试构建下会报错终止（便于提前暴露），**设备构建下全部为空宏/空函数体，静默无操作**（详见 5.2）。

---

## 4. 各版本原始代码

以下代码均摘自 `asc-devkit\impl\basic_api\dav_XXXX\kernel_operator_data_copy_impl.h`，行号为当前文件实际行号。

### 4.1 dav_l300（3003，999 行）

```cpp
// L135-141 完整
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    copy_ubuf_to_ubuf(
        (__ubuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// L143-149 完整
template <typename T>
__aicore__ inline void DataCopyUB2L1Impl(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    copy_ubuf_to_cbuf(
        (__cbuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// L151-157 完整
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    copy_cbuf_to_ubuf(
        (__ubuf__ void*)dst, (__cbuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
        intriParams.srcStride, intriParams.dstStride);
}

// L590-595 桩
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from cbuf to l0c"); });
}

// L602-607 桩
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from l0c to ubuf"); });
}

// L609-614 桩
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from ubuf to l0c"); });
}

// L853-868 Intf（UB2L0CIntf 直转桩；UB2UBIntf 带 #pragma begin_pipe(V)）
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyUB2L0CImpl(
        (__cc__ T*)dstLocal.GetPhyAddr(), (__ubuf__ T*)srcLocal.GetPhyAddr(), intriParams, enhancedParams);
}

#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyUB2UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__ubuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}
#pragma end_pipe

// L871-876 Intf
template <typename T>
__aicore__ inline void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__cbuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}

// L878-885 Intf（带 MTE1 管道属性；注意 V1 包装层直调 Impl，不走此 Intf）
template <typename T>
__aicore__ inline void __in_pipe__(MTE1) __out_pipe__(MTE1) DataCopyL12L0CIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyL12L0CImpl(
        (__cc__ T*)dstLocal.GetPhyAddr(), (__cbuf__ T*)srcLocal.GetPhyAddr(), intriParams, enhancedParams);
}

// L887-894 Intf
template <typename T>
__aicore__ inline void DataCopyL0C2UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    DataCopyL0C2UBImpl(
        (__ubuf__ T*)dstLocal.GetPhyAddr(), (__cc__ T*)srcLocal.GetPhyAddr(), intriParams, enhancedParams);
}
```

### 4.2 dav_l310（3103，967 行）

与 dav_l300 的差异：UB2UB 改为手动拼 1017 config；桩消息后缀为 "on this version"；另有 L1→PT（预取）实路径。

```cpp
// L165-172 完整（手动拼 config）
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    // 当前#1018实现有问题，需要手动拼1017的config值
    uint64_t config = ((uint64_t)intriParams.dstStride << 48) + (((uint64_t)intriParams.srcStride) << 32) +
                      (((uint64_t)intriParams.blockLen) << 16) + (((uint64_t)intriParams.blockCount) << 4);
    copy_ubuf_to_ubuf((__ubuf__ void*)dst, (__ubuf__ void*)src, config);
}

// L174-188 完整（UB2L1 / L12UB 与 l300 相同，直接调 copy_ubuf_to_cbuf / copy_cbuf_to_ubuf）

// L521-526 桩
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from cbuf to l0c on this version"); });
}

// L533-538 桩："unsupported data copy from l0c to ubuf on this version"
// L540-545 桩："unsupported data copy from ubuf to l0c on this version"

// L891-939 Intf 组（结构同 l300：UB2L0CIntf / #pragma begin_pipe(V) UB2UBIntf / L12UBIntf / MTE1 L12L0CIntf / L0C2UBIntf）

// L227-232 附加实路径（不在本次 8 变体内）：DataCopyL12PTImpl → copy_cbuf_to_pt
```

### 4.3 dav_l311（3113，1018 行）

与 dav_l310 基本为同源副本：

```cpp
// L165-172 完整（手动拼 config，与 l310 相同，blockCount << 4）
// L174-188 完整（UB2L1 / L12UB 直接指令）
// L572-577 桩："unsupported data copy from cbuf to l0c on this version"
// L584-589 桩："unsupported data copy from l0c to ubuf on this version"
// L591-596 桩："unsupported data copy from ubuf to l0c on this version"
// L943-990 Intf 组（结构同 l310）
```

### 4.4 dav_5161（5101/5161/5165/5163 共用，1030 行）

关键特征：L1→UB 使用 FIX 管道指令族；三个 Intf 包装函数缺失或仅 5101 可见。

```cpp
// L183-190 完整（手动拼 config，blockCount 不移位，与 l310/l311 不同！）
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    // 当前#1018实现有问题，需要手动拼1017的config值
    uint64_t config = ((uint64_t)intriParams.dstStride << 48) + (((uint64_t)intriParams.srcStride) << 32) +
                      (((uint64_t)intriParams.blockLen) << 16) + ((uint64_t)intriParams.blockCount);
    copy_ubuf_to_ubuf((__ubuf__ void*)dst, (__ubuf__ void*)src, config);
}

// L192-198 完整（UB2L1 直接指令，同 l300）
// L200-219 完整（L1→UB 使用 FIX 指令族，与其他所有版本不同）
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

// L221-228 Intf 仅 5101 可见
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101)
template <typename T>
__aicore__ inline void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    DataCopyL12UBImpl((__ubuf__ T*)dstLocal.GetPhyAddr(), (__cbuf__ T*)srcLocal.GetPhyAddr(), intriParams);
}
#endif

// L724-729 桩（ASSERT 风格，与其他版本的 ASCENDC_ASSERT 风格不同）
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASSERT(false && "unsupported data copy L1 to L0C on current device");
}

// L731-736 桩：ASSERT(false && "unsupported data copy L0C to UB on current device")
// L751-756 桩：ASSERT(false && "unsupported data copy UB to L0C on current device")

// L738-749 CO12Dst 变体桩（不在 8 变体内，ASCENDC_ASSERT 风格）：
//   DataCopyL0C2L1Impl："unsupported data copy L0C to L1 on current device"
//   DataCopyL0C2GMImpl："unsupported data copy L0C to GM on current device"
```

**重要**：全文件不存在 `DataCopyUB2UBIntf` 与 `DataCopyUB2L0CIntf` 定义（文件尾部 L931-1030 已确认，中间区域经全文检查亦无）。V1 包装层在 UB→UB / UB→L0C 路径引用这两个符号，四个架构（5101/5161/5165/5163）上实例化即编译失败；`DataCopyL12UBIntf` 仅 5101 存在，5161/5165/5163 的 L1→UB 路径同样编译失败。

### 4.5 dav_c220（2201，1225 行）—— L1→L0C 唯一实路径版本

```cpp
// L119-133 完整（AIV 核型宏控 + 32B 对齐调试断言）
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    if ASCEND_IS_AIV {
        ASCENDC_DEBUG_ASSERT(/* src/dst 32B 对齐检查 */);
        copy_ubuf_to_ubuf(
            (__ubuf__ void*)dst, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
            intriParams.srcStride, intriParams.dstStride);
    }
}

// L135-189 完整（TSCM 软件模拟：UB→GM 中转 + KFC 消息通知 AIC 执行 GM→L1）
template <typename T>
__aicore__ inline void DataCopyUB2L1Impl(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    // Add for TSCM AIV: Copy buffer into workspace, and send message to AIC
    ASCENDC_ASSERT((GetTPipePtr() != nullptr), { KERNEL_LOG(KERNEL_ERROR, "tpipe ptr can not be nullptr"); });
    ...
    if ASCEND_IS_AIV {
        // 1.获取GM的地址（设备侧 GetKfcClient()->AllocUB；CPU 调试侧直接算 UB 偏移）
        GM_ADDR gmAddr = (GetKfcClient()->AllocUB(tensorSize, ubAddr));
        event_t eventID = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_MTE3));
        SetFlag<HardEvent::V_MTE3>(eventID);
        WaitFlag<HardEvent::V_MTE3>(eventID);
        // 2.进行对等拷贝ub->GM
        copy_ubuf_to_gm(
            (__gm__ void*)gmAddr, (__ubuf__ void*)src, 0, intriParams.blockCount, intriParams.blockLen,
            intriParams.srcStride, intriParams.srcStride);
        // 3.发送消息,从GM拷贝至L1（设备侧被 #ifndef __NO_KFC_SERVER__ 包裹）
        ScmDataCopyMsg((__cbuf__ void*)dst, (__gm__ void*)gmAddr, intriParams, ubAddr);
    }
}

// L245-251 桩（桩体被 debug 宏控包裹：设备构建下函数体为空，静默无操作）
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    ReportNotSupport(false, "DataCopy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT)");
#endif
}

// L400-420 完整（全版本中唯一的 L1→L0C 实路径）
template <typename T, typename U>
__aicore__ inline __inout_pipe__(MTE1) void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    if ASCEND_IS_AIC {
        static_assert(
            (SupportType<
                Tuple<U, T>, Tuple<half, half>, Tuple<float, half>, Tuple<float, bfloat16_t>, Tuple<float, float>,
                Tuple<bfloat16_t, bfloat16_t>, Tuple<int32_t, int32_t>, Tuple<uint32_t, uint32_t>>()),
            "Failed to check dtype in DataCopy from L1 Buffer(A1/B1) to L0C Buffer(CO1), ...");
        ASCENDC_CHECK_TENSOR_PTR_ALIGN(src, TPosition::A1, ONE_BLK_SIZE, ...);   // src 32B 对齐
        ASCENDC_CHECK_TENSOR_PTR_ALIGN(dst, TPosition::CO1, 1024, ...);          // dst 1024B 对齐
        copy_matrix_cbuf_to_cc(
            (__cc__ T*)dst, (__cbuf__ U*)src, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride,
            intriParams.dstStride);
    }
}

// L425-430 桩（ReportNotSupport，无条件调用）
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ReportNotSupport(false, "DataCopy from L0C Buffer(CO1) to GM(CO2)");
}

// L432-437 桩
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ReportNotSupport(false, "DataCopy from GM(CO2) to L0C Buffer(CO1)");
}

// L1170-1193 Intf 组（均为 thin wrapper）
//   DataCopyL12UBIntf  L1170-1175（→桩）
//   DataCopyUB2L0CIntf L1177-1184（→桩）
//   DataCopyUB2UBIntf  L1186-1193（#pragma begin_pipe(V)，→完整）
```

### 4.6 dav_c100（1001，1113 行）—— L0C↔UB 直达搬运实路径版本之一

```cpp
// L61-84 完整（UB2UB / UB2L1 / L12UB 均为直接指令，UB2L1 带 MTE3 管道属性）

// L849-854 桩
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "DataCopy from L1 Buffer(A1/B1) to L0C Buffer(CO1)");
}

// L861-907 完整（deq 量化 SPR 预处理 + blockMode 分发）
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    if ((enhancedParams.deqScale == DeqScale::DEQ8) || (enhancedParams.deqScale == DeqScale::DEQ16)) {
        uint64_t deqScaleSpr = enhancedParams.deqValue;
        if (enhancedParams.isRelu) {
            deqScaleSpr |= 1ULL << 47;
        }
        if (IsSameType<T, int8_t>::value) {
            deqScaleSpr |= 1ULL << 46;
        }
        set_deqscale(deqScaleSpr);
        event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
        SetFlag<HardEvent::S_V>(eventIdSToV);
        WaitFlag<HardEvent::S_V>(eventIdSToV);
    } else if ((enhancedParams.deqScale == DeqScale::VDEQ8) || (enhancedParams.deqScale == DeqScale::VDEQ16)) {
        uint64_t deqScaleSpr = enhancedParams.deqTensorAddr >> 5;
        if (enhancedParams.isRelu) {
            deqScaleSpr |= 1ULL << 15;
        }
        set_deqscale(deqScaleSpr);
#if ASCENDC_CPU_DEBUG
        // CPU 调试侧 SetModelDeqTensor 额外处理
#endif
        event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
        SetFlag<HardEvent::S_V>(eventIdSToV);
        WaitFlag<HardEvent::S_V>(eventIdSToV);
    }
    if (enhancedParams.blockMode == BlockMode::BLOCK_MODE_MATRIX) {
        DataCopyMatrixL0C2UBImpl(dst, src, intriParams, enhancedParams);   // L370-555：copy_matrix_cc_to_ubuf + CRMODE 分发
    } else if (enhancedParams.blockMode == BlockMode::BLOCK_MODE_VECTOR) {
        DataCopyVectorL0C2UBImpl(dst, src, intriParams, enhancedParams);   // L559-744：copy_vector_cc_to_ubuf + CRMODE 分发
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "... blockMode should be BLOCK_MODE_MATRIX / BLOCK_MODE_VECTOR"); });
    }
}

// L909-924 完整（blockMode 分发）
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    if (enhancedParams.blockMode == BlockMode::BLOCK_MODE_MATRIX) {
        DataCopyMatrixUB2L0CImpl(dst, src, intriParams, enhancedParams);   // L749-796：copy_matrix_ubuf_to_cc
    } else if (enhancedParams.blockMode == BlockMode::BLOCK_MODE_VECTOR) {
        DataCopyVectorUB2L0CImpl(dst, src, intriParams, enhancedParams);   // L800-847：copy_vector_ubuf_to_cc
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "... blockMode should be BLOCK_MODE_MATRIX / BLOCK_MODE_VECTOR"); });
    }
}

// L1058-1081 Intf 组（UB2L0CIntf 与 UB2UBIntf 同处一个 #pragma begin_pipe(V) 块）
#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(...)
{
    DataCopyUB2L0CImpl((__cc__ PrimT<T>*)dst.GetPhyAddr(), (__ubuf__ PrimT<T>*)src.GetPhyAddr(), intriParams, enhancedParams);
}

template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(...)
{
    DataCopyUB2UBImpl((__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__ubuf__ PrimT<T>*)src.GetPhyAddr(), intriParams);
}
#pragma end_pipe

template <typename T>
__aicore__ inline __in_pipe__(MTE1) __out_pipe__(MTE1) void DataCopyL12UBIntf(...)
{
    DataCopyL12UBImpl((__ubuf__ PrimT<T>*)dst.GetPhyAddr(), (__cbuf__ PrimT<T>*)src.GetPhyAddr(), intriParams);
}
```

L0C2UB 方向 dtype/CRMODE 组合（Matrix/Vector 两组分发，合计）：
- half←half：CRMODE_NONE / _RELU / _DEQSCALE_SPR(_RELU)（DEQ8/DEQ16）
- half←float：CRMODE_F32toF16_NONE / _RELU / _DEQSCALE(_RELU)
- half←int32_t：CRMODE_S32toF16_NONE / _RELU / _DEQSCALE_SPR(_RELU) / _DEQSCALE_VDEQ16 / _DEQSCALE_DEQ16
- float←float、int32_t←int32_t、uint32_t←uint32_t：CRMODE_NONE / _RELU
- int8_t←int32_t、uint8_t←int32_t：DEQ8 / VDEQ8 系列（含 RELU）
- int16_t←int32_t：VDEQ16 / DEQ16 系列（含 RELU）

UB2L0C 方向 dtype 组合：half←half、float←float、float←half（CRMODE_F16toF32_NONE）、int32_t←int32_t、uint32_t←uint32_t。

### 4.7 dav_m200（2002，1469 行）

关键特征：Cube 相关函数全部按 `#if defined(__DAV_M200_VEC__)` 核型宏控二分为桩/完整两份；L0C↔UB 与 c100 同源但增加 `VDEQ`/`DEQ` 标量 deq 模式与 int8_t 同类型特化桩。

```cpp
// L94-144：UB2UB / UB2L1 / L12UB 均为
//   #if defined(__DAV_M200_VEC__)  → ASCENDC_REPORT_NOT_SUPPORT 桩
//   #else                          → 完整（copy_ubuf_to_ubuf / copy_ubuf_to_cbuf / copy_cbuf_to_ubuf）
// （UB2L1 完整版带 __inout_pipe__(MTE3)）

// L1156-1161 桩（无条件）
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "DataCopy from L1 Buffer(A1/B1) to L0C Buffer(CO1)");
}

// L1167-1172 int8_t 同类型特化桩（V1 的 L0C→UB int8 路径命中此特化）
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ int8_t* dst, __cc__ int8_t* src, const DataCopyParams& intriParams,
    const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "DataCopy from L0C Buffer(CO1) to UB(CO2) with type int8_t");
}

// L1175-1232 模板版完整（VEC 桩 / AIC 完整；相比 c100 新增两个分支）
    } else if (
        (enhancedParams.deqScale == DeqScale::VDEQ8) || (enhancedParams.deqScale == DeqScale::VDEQ16) ||
        (enhancedParams.deqScale == DeqScale::VDEQ)) {          // 新增 VDEQ
        ...
    } else if (enhancedParams.deqScale == DeqScale::DEQ) {      // 新增 DEQ：half -> half / int32_t -> half
        uint64_t deqScaleSpr = enhancedParams.deqValue;
        set_deqscale(deqScaleSpr);
        ...S_V 事件同步...
    }
    // blockMode 分发同 c100（DataCopyMatrixL0C2UBImpl / DataCopyVectorL0C2UBImpl，VEC 桩 / AIC 完整）

// L1234-1239 int8_t 同类型特化桩（UB2L0C 方向）
// L1241-1259 模板版完整（VEC 桩 / AIC 完整，blockMode 分发）

// L1414-1437 Intf 组
template <typename T>
__aicore__ inline __inout_pipe__(MTE1) void DataCopyL12UBIntf(...)      // 注意：inout 形式，其他版本为 in/out 两个属性
{ ... }
#pragma begin_pipe(V)
template <typename T>
__aicore__ inline void DataCopyUB2L0CIntf(...) { ... }
template <typename T>
__aicore__ inline void DataCopyUB2UBIntf(...) { ... }
#pragma end_pipe
```

### 4.8 dav_3510（3510，1729 行，参照版本）

```cpp
// L607-612 完整
template <typename T>
__aicore__ inline void DataCopyUB2UBImpl(__ubuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
    CopyUbufToUbuf(dst, src, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
}

// L617-674 完整（条件宏控：直连或 TSCM 软件模拟）
template <typename T>
__aicore__ inline __in_pipe__(MTE3)
    __out_pipe__(MTE3) void DataCopyUB2L1Impl(__cbuf__ T* dst, __ubuf__ T* src, const DataCopyParams& intriParams)
{
#if KFC_C310_SSBUF == 1 || __MIX_CORE_AIC_RATION__ != 1
    CopyUbufToCbuf(dst, src, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
#else
    // Add for TSCM AIV: Copy buffer into workspace, and send message to AIC
    // （与 c220 同构：AllocUB → V_MTE3 同步 → CopyUbufToGmAlignV2 → ScmDataCopyMsg；含 g_gm_overflow_check 溢出检查）
#endif
}

// L868-873 桩
template <typename T, typename U>
__aicore__ inline void DataCopyUB2L0CImpl(
    __cc__ T* dst, __ubuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from ubuf to L0C on current device"); });
}

// L876-890 条件完整（Impl 层面条件宏控；支持 subBlockId 模板参数）
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

// L908-913 桩："unsupported data copy from cbuf to L0C on current device"

// L916-925 桩（带迁移指引）
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "unsupported data copy from l0c to ubuf on current device, please use FixPipe function instead");
    });
}

// L1410-1417 Intf 桩（注意：Impl 条件完整，但 Intf 无条件桩 → V1 的 L1→UB 路径在 3510 上不可用）
template <typename T>
__aicore__ inline __in_pipe__(MTE1) __out_pipe__(MTE1) void DataCopyL12UBIntf(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const DataCopyParams& intriParams)
{
    ASCENDC_ASSERT((false), {
        KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from L1 Buffer(A1/B1) to UB(VECIN/VECOUT) on current device");
    });
}

// L1419-1427 UB2L0CIntf thin wrapper（→桩）
// L1429-1437 UB2UBIntf thin wrapper（#pragma begin_pipe(V)，→完整）

// L1439-1449 附加实路径（不在 8 变体内）：L1→FIXBUF
template <typename T>
__aicore__ inline __in_pipe__(FIX)
    __out_pipe__(FIX) void DataCopyL12FBImpl(__fbuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    constexpr uint16_t unitCoeff = 2;
    if ASCEND_IS_AIC {
        copy_cbuf_to_fbuf(
            (__fbuf__ void*)dst, (__cbuf__ void*)src, intriParams.blockCount, intriParams.blockLen * unitCoeff,
            intriParams.srcStride, intriParams.dstStride * unitCoeff);
    }
}

// 另：L0C2L1Impl（L1339-1372，copy_matrix_cc_to_cbuf）/ L0C2GMImpl（L1374-1408，copy_matrix_cc_to_gm）
// 为 CO12Dst 变体实路径（不在 8 变体内）
```

### 4.9 dav_m300（3002，1048 行）

```cpp
// L60-83 完整（UB2UB / UB2L1(MTE3) / L12UB 直接指令，同 l300 风格）

// L766-771 桩（ASSERT 风格）
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASSERT(false && "unsupported data copy from cbuf to l0c on current device");
}

// L778-784 桩（注意 "deviceB1" 疑似复制残留）
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT(
        (false), { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from l0c to ubuf on current deviceB1"); });
}

// L786-792 桩："unsupported data copy from ubuf to l0c on current deviceB1"（同样带 B1 残留）

// L993-1016 Intf 组（L12UBIntf 带 MTE1 属性 / UB2L0CIntf / #pragma begin_pipe(V) UB2UBIntf）
```

### 4.10 dav_m310（3102，930 行）

```cpp
// L40-63 完整（UB2UB / UB2L1 / L12UB 直接指令）

// L602-607 桩（ASSERT 风格）
template <typename T, typename U>
__aicore__ inline void DataCopyL12L0CImpl(
    __cc__ T* dst, __cbuf__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASSERT(false && "unsupported data copy from cbuf to l0c on current device");
}

// L614-619 桩：ASCENDC_ASSERT "unsupported data copy from l0c to ubuf on current device"
// L621-626 桩：ASCENDC_ASSERT "unsupported data copy from ubuf to l0c on current device"

// L901-924 Intf 组（UB2L0CIntf / #pragma begin_pipe(V) UB2UBIntf / MTE1 L12UBIntf）
```

### 4.11 dav_m510（5102，1391 行）

```cpp
// L543-556 完整（UB2UB 经 CopyUbufToUbuf；UB2L1 带 MTE3 属性经 CopyUbufToCbuf，无条件直连，无 TSCM 分支）

// L704-709 桩："unsupported data copy from ubuf to L0C on current device"

// L712-717 完整（无条件，无核型/混核宏控）
// SRC: L1
template <typename T>
__aicore__ inline void DataCopyL12UBImpl(__ubuf__ T* dst, __cbuf__ T* src, const DataCopyParams& intriParams)
{
    CopyCbufToUbuf(dst, src, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, intriParams.dstStride);
}

// L735-740 桩："unsupported data copy from cbuf to L0C on current device"

// L743-752 桩（带迁移指引，同 3510）
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2UBImpl(
    __ubuf__ T* dst, __cc__ U* src, const DataCopyParams& intriParams, const DataCopyEnhancedParams& enhancedParams)
{
    ASCENDC_ASSERT((false), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "unsupported data copy from l0c to ubuf on current device, please use FixPipe function instead");
    });
}

// L1169-1204 Intf 组：L12UBIntf（MTE1 in/out 属性，→完整）/ UB2L0CIntf（→桩）/ #pragma begin_pipe(V) UB2UBIntf（→完整）
//   另有 L12FBImpl（L1196-1204，copy_cbuf_to_fbuf，不在 8 变体内）
```

---

## 5. 代码级实现差异分析

### 5.1 宏控差异

| 宏控 | 位置 | 作用 |
|---|---|---|
| `#if (__NPU_ARCH__ == 2201)` | intf.h L321 / intf_impl.h L1242 | V2（bf16←f32）声明与实现仅 2201 开放 |
| `#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5101)` | dav_5161 L221 | `DataCopyL12UBIntf` 仅 5101 定义；5161/5165/5163 无此函数 |
| `#if defined(__DAV_M200_VEC__)` | dav_m200 全文件 | Cube 类函数按核型二分：VEC 核编译为桩，AIC 核完整 |
| `#if KFC_C310_SSBUF == 1 \|\| __MIX_CORE_AIC_RATION__ != 1` | dav_3510 L621 | UB→L1 直连（`CopyUbufToCbuf`）或 TSCM AIV 软件模拟（UB→GM+KFC 消息） |
| `#if __MIX_CORE_AIC_RATION__ != 1` | dav_3510 L879 | L1→UB Impl 完整或 1:1 场景桩 |
| `#if defined(ASCENDC_DEBUG) \|\| defined(ASCENDC_CPU_DEBUG)` | dav_c220 L248 | L12UB 桩体仅调试构建生效，设备构建为空函数体 |
| `#ifndef __NO_KFC_SERVER__` | dav_c220 L184 | 设备侧 KFC 消息发送开关 |
| `#if ASCENDC_CPU_DEBUG` | c220/3510/c100/m200 多处 | CPU 调试侧地址换算 / SetModelDeqTensor 等额外处理 |
| `#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) \|\| (__NPU_ARCH__ == 5102))` | intf_impl.h L1234 | L0C→GM cacheMode 参数（CO12Dst 变体，不在 8 变体内） |

### 5.2 桩（stub）风格差异与设备构建语义

| 风格 | 使用版本（本 8 变体相关） | CPU 调试构建 | 设备构建 |
|---|---|---|---|
| `ASCENDC_ASSERT(false, { KERNEL_LOG(...) })` | l300/l310/l311、m300(L0C2UB/UB2L0C)、m310(L0C2UB/UB2L0C)、m510、3510、5161(L0C2L1/L0C2GM) | 日志 + `raise(SIGABRT)` 终止 | 空宏，**静默无操作** |
| `ASSERT(false && "...")`（裸断言） | 5161(L12L0C/L0C2UB/UB2L0C)、m300(L12L0C)、m310(L12L0C) | `assert()` 终止 | 空宏，**静默无操作** |
| `ASCENDC_REPORT_NOT_SUPPORT(false, "...")` | c100、m200 | 日志 + `raise(SIGABRT)` 终止 | 空宏，**静默无操作** |
| `ReportNotSupport(false, "...")`（c220 专用） | c220 | 报错（推测） | L12UB 路径被 debug 宏控包裹→空函数体；L0C2UB/UB2L0C 无条件调用（该函数定义未在已追踪包含链中定位，见待确认问题 2） |

**共同风险**：所有桩在设备构建下均不产生任何运行时防护——调用即静默无操作，无数据搬移、无报错。仅 CPU 调试构建能提前暴露误用。

### 5.3 指令选型差异（最底层编译器内建 API）

| 搬运路径 | 指令/API | 使用版本 |
|---|---|---|
| UB→UB | `copy_ubuf_to_ubuf(dst, src, 0, ...)` 7 参直接形式 | l300、m300、m310、c220、c100、3510(经 CopyUbufToUbuf)、m510(经 CopyUbufToUbuf) |
| UB→UB | `copy_ubuf_to_ubuf(dst, src, config)` 手动拼 1017 config | l310、l311（blockCount `<<4`）；**5161（blockCount 不移位）** |
| UB→L1 | `copy_ubuf_to_cbuf` | l300/l310/l311/m300/m310/5161 直接；c100/m200/m510 经包装；3510 条件宏控 |
| UB→L1（软件模拟） | `copy_ubuf_to_gm` + `ScmDataCopyMsg`（KFC 消息，AIC 侧执行 GM→L1） | c220；3510 TSCM 分支（`CopyUbufToGmAlignV2`） |
| L1→UB | `copy_cbuf_to_ubuf` | l300/l310/l311/m300/m310/c100 直接；m510/3510 经 CopyCbufToUbuf |
| L1→UB（FIX 指令族） | `set_fixp_nz_para` + `fix_cbuf_to_ubuf`（NZ 格式） | **仅 5161 家族**（xm/xt/config 位域手工拼装） |
| L1→L0C | `copy_matrix_cbuf_to_cc` | **仅 c220**（dtype 白名单 static_assert + 对齐检查） |
| L0C→UB | `copy_matrix_cc_to_ubuf` / `copy_vector_cc_to_ubuf`（+ `set_deqscale` SPR 与 S_V 事件同步） | **仅 c100、m200(AIC)** |
| UB→L0C | `copy_matrix_ubuf_to_cc` / `copy_vector_ubuf_to_cc` | **仅 c100、m200(AIC)** |
| L1→FB（参考） | `copy_cbuf_to_fbuf` | 3510、m510（不在 8 变体内） |

### 5.4 管道属性差异

| 层级 | 属性 | 说明 |
|---|---|---|
| 声明/包装层 V4~V8 | `__inout_pipe__(V)` | 统一 |
| 声明/包装层 V1/V2/V3 | 无 | — |
| `DataCopyUB2UBIntf` | `#pragma begin_pipe(V)` | 所有版本统一 |
| `DataCopyL12UBIntf` | `__in_pipe__(MTE1) __out_pipe__(MTE1)` | c100/c220/m300/m310/l300/l310/l311/m510/3510；m200 为 `__inout_pipe__(MTE1)`；5161(5101) 无属性 |
| `DataCopyL12L0CImpl` | c220 `__inout_pipe__(MTE1)`；其余无 | — |
| `DataCopyUB2L1Impl` | 3510/m510 `__in_pipe__(MTE3) __out_pipe__(MTE3)`；m200/c100 为 MTE3 属性；l300/l310/l311/m300/m310/5161 无 | — |

### 5.5 其他差异

1. **UB2UB config 拼接位域不一致**：l310/l311 与 5161 同为"手动拼 1017 config"方案，但 l310/l311 的 blockCount 占位为 `<<4`，5161 不移位（dav_5161 L188）。跨版本移植该段代码时不可直接照抄。
2. **桩消息文案不统一**：l300 无后缀；l310/l311 为 "on this version"；m300/m310/3510/m510 为 "on current device"；m300 两条消息带 "deviceB1" 疑似复制残留；3510/m510 的 L0C→UB 桩带 "please use FixPipe function instead" 迁移指引（其余版本无）。
3. **5161 桩风格独立**：L0C 相关三个桩用裸 `ASSERT(false && "...")`，与同文件 CO12Dst 桩（`ASCENDC_ASSERT`）及兄弟版本的 `ASCENDC_ASSERT` 风格不一致。
4. **3510 的 Impl/Intf 矛盾**：`DataCopyL12UBImpl` 在 `__MIX_CORE_AIC_RATION__ != 1` 时完整（还支持 `subBlockId` 模板参数），但 `DataCopyL12UBIntf` 为无条件桩，V1 路径实际不可用。
5. **m200 附加 deq 模式与特化桩**：相比 c100 新增 `VDEQ`、`DEQ` 标量模式；`int8_t` 同类型 `DataCopyL0C2UBImpl`/`DataCopyUB2L0CImpl` 特化为桩（V1 同类型 int8 路径受影响，V6 不受影响）。
6. **c220 软件模拟依赖**：UB→L1 依赖 KFC（需 `REGISTER_MATMUL_OBJ` 启用 Matmul API），CPU 调试与设备侧地址获取逻辑分叉（`AllocUB` vs 直接偏移）。
7. **对齐检查差异**：c220 的 UB2UB、m200 的 L0C2UB/UB2L0C 有 32B 对齐调试断言；c220 的 L12L0C 有 src 32B / dst 1024B 对齐检查；其余版本多数无对齐检查。
8. **m200 废弃标注**：`CopyImpl`（L460-475）与 `DataCopyPad*` 系列带 `[[deprecated]]`，其余版本无。

---

## 6. 待确认问题

1. **dav_5161 Intf 缺失是否有意**：`DataCopyUB2UBIntf` / `DataCopyUB2L0CIntf` 在 5101/5161/5165/5163 全部缺失，`DataCopyL12UBIntf` 仅 5101 存在。对应的 `DataCopyUB2UBImpl` 本身是完整的，仅缺 Intf 包装。若是无意遗漏，V1 的 UB→UB / UB→L0C / L1→UB 路径在对应架构上实例化即编译失败，属于 API 可用性缺口；若是有意裁剪，建议补充编译期显式报错（而非链接/实例化期失败）。
2. **`ReportNotSupport` 定义位置**（仅 dav_c220 使用）：未在已追踪的包含链（kernel_common.h / kernel_log.h / kernel_macros.h / kernel_utils.h / kernel_utils_base.h / kernel_utils_mode*.h / kernel_struct_data_copy.h / kernel_operator_scm_data_copy_impl.h / helpers_impl.h 等）中定位到定义。其设备构建语义（推测为"调试报错+设备空操作"，与 `ASCENDC_REPORT_NOT_SUPPORT` 同模式）待确认。
3. **3510 `DataCopyL12UBImpl` 完整分支的调用方**：Impl 条件完整但 Intf 无条件桩，Impl 的完整分支（含 `subBlockId`）供哪些内部路径（如 Matmul 内部）使用待确认。
4. **m300 "deviceB1" 文案残留**：`DataCopyL0C2UBImpl` / `DataCopyUB2L0CImpl` 桩消息尾部的 "B1" 疑似从 B1 位置相关消息复制而来，建议清理。
5. **V2（bf16←f32）仅 2201 开放的原因**：c220 的 `copy_matrix_cbuf_to_cc` dtype 白名单含 `float→bfloat16_t`，其余架构均无 L1→L0C 实路径，该限制与 L12L0C 支持面一致；是否有硬件约束待确认。
6. **新架构 L0C↔UB 直达搬运的迁移指引**：3002/3003/3102/3103/3113/5101 族/5102/3510 的 L0C 相关路径全部为桩，仅 3510/m510 的桩给出 "please use FixPipe function instead" 提示。V3~V8 类型转换语义在这些架构上的官方替代方案（FixPipe / Cast 向量算子组合）是否有文档化迁移路径待确认。
7. **手动拼 config 的位域定义**：l310/l311（`blockCount<<4`）与 5161（不移位）的 1017 config 位域差异需对照硬件指令手册确认，跨版本复用代码时存在隐患。

---

## 附：涉及的文件清单

| 层 | 文件 |
|---|---|
| 声明层 | `asc-devkit\include\basic_api\kernel_operator_data_copy_intf.h` |
| 包装层 | `asc-devkit\impl\basic_api\kernel_operator_data_copy_intf_impl.h` |
| 共享层 | `asc-devkit\impl\basic_api\kernel_operator_data_copy_base_impl.h`（L32-55 版本分发宏控） |
| 版本层 | `asc-devkit\impl\basic_api\dav_l300\kernel_operator_data_copy_impl.h`（999 行） |
| | `asc-devkit\impl\basic_api\dav_l310\kernel_operator_data_copy_impl.h`（967 行） |
| | `asc-devkit\impl\basic_api\dav_l311\kernel_operator_data_copy_impl.h`（1018 行） |
| | `asc-devkit\impl\basic_api\dav_5161\kernel_operator_data_copy_impl.h`（1030 行） |
| | `asc-devkit\impl\basic_api\dav_3510\kernel_operator_data_copy_impl.h`（1729 行） |
| | `asc-devkit\impl\basic_api\dav_c220\kernel_operator_data_copy_impl.h`（1225 行） |
| | `asc-devkit\impl\basic_api\dav_c100\kernel_operator_data_copy_impl.h`（1113 行） |
| | `asc-devkit\impl\basic_api\dav_m200\kernel_operator_data_copy_impl.h`（1469 行） |
| | `asc-devkit\impl\basic_api\dav_m300\kernel_operator_data_copy_impl.h`（1048 行） |
| | `asc-devkit\impl\basic_api\dav_m310\kernel_operator_data_copy_impl.h`（930 行） |
| | `asc-devkit\impl\basic_api\dav_m510\kernel_operator_data_copy_impl.h`（1391 行） |
| 辅助 | `asc-devkit\impl\basic_api\dav_c220\kernel_operator_scm_data_copy_impl.h`（KFC 软件模拟消息） |
| | `asc-devkit\impl\basic_api\kernel_log.h` / `kernel_macros.h`（桩宏语义） |
