# 案例分析：DataCopy(L0C-L1/L0C-GM) 接口跨版本适配

## 接口概述

`DataCopy(..., DataCopyCO12DstParams)` 用于 L0C→L1 和 L0C→GM 的矩阵数据拷贝（支持量化/ReLU 等增强功能）。有 2 个重载变体：

| 变体 | 参数签名 | 声明位置 | arch 守卫 | 说明 |
|------|---------|---------|-----------|------|
| V1 | `DataCopy(LocalTensor<T>& dst, LocalTensor<U>& src, DataCopyCO12DstParams)` | `intf.h:313-315` | 无 | L0C → L1 |
| V2 | `DataCopy(GlobalTensor<T>& dst, LocalTensor<U>& src, DataCopyCO12DstParams)` | `intf.h:317-319` | 无 | L0C → GM |

**参数结构体** `DataCopyCO12DstParams`（定义于 `include/basic_api/kernel_struct_data_copy.h:162-190`）：

| 字段 | 类型 | 说明 |
|------|------|------|
| `sid` | `uint8_t` | SID |
| `nSize` | `uint16_t` | N 维度大小 |
| `mSize` | `uint16_t` | M 维度大小 |
| `dstStride` | `uint32_t` | 目标步长 |
| `srcStride` | `uint16_t` | 源步长 |
| `unitFlag` | `uint8_t` | unit flag |
| `quantPre` | `QuantMode_t` | 量化模式 |
| `reluPre` | `uint8_t` | ReLU 模式 |
| `channelSplit` | `bool` | 通道拆分 |
| `nz2ndEn` | `bool` | NZ 格式使能 |

> 结构体无 `__NPU_ARCH__` 守卫，所有版本可见。

## 调用链

```
intf.h:313/317（V1/V2 声明，无 arch 守卫）
  → intf_impl.h:1203-1214 / 1216-1230（公共包装，V1 无 arch 守卫；V2 有 #if 3510/5102 守卫影响 cacheMode 传递）
    → DataCopyL0C2L1Impl / DataCopyL0C2GMImpl
      → dav_XXXX/kernel_operator_data_copy_impl.h（版本特定实现，通过 base_impl.h:32-55 的 #elif #include 分派）
```

### 共享层分发逻辑

**版本隔离机制：** `kernel_operator_data_copy_base_impl.h:32-55` 通过 `#elif __NPU_ARCH__` 分派 `#include`。

**intf_impl.h 公共包装层：**

| 变体 | intf_impl.h 行号 | arch 守卫 | 调用的 Impl 函数 |
|------|-----------------|-----------|----------------|
| V1 (L0C→L1) | L1203-1214 | 无 | `DataCopyL0C2L1Impl(dst, src, intriParams)` |
| V2 (L0C→GM) | L1216-1230 | `#if 3510\|\|5102` 分支传 `cacheMode`，`#else` 不传 | `DataCopyL0C2GMImpl(dst, src, intriParams, cacheMode)` 或 `DataCopyL0C2GMImpl(dst, src, intriParams)` |

**V2 的 arch 守卫细节（intf_impl.h:1224-1229）：**

```cpp
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    const uint8_t cacheMode = ExtractCacheMode(dst);
    DataCopyL0C2GMImpl((__gm__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams, cacheMode);
#else
    DataCopyL0C2GMImpl((__gm__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams);
#endif
```

> **关键问题：** `#else` 分支（3003/3103/3113/5101 等）调用 3 参数版 `DataCopyL0C2GMImpl`，但 l300/l310/l311 的实际实现签名为 4 参数版（含 `cacheMode`）。这意味着 `#else` 路径在 l300/l310/l311 上会编译失败（无匹配重载）。详见待确认问题。

## 支持矩阵

| 版本 | Arch | V1 (L0C→L1) | V2 (L0C→GM) | 说明 |
|------|------|-------------|-------------|------|
| dav_l300 | 3003 | **完整** (L940-973) | **完整** (L904-938) | `#else` 路径调用签名不匹配（4参数 vs 3参数），潜在编译问题 |
| dav_l310 | 3103 | **完整** (L849-882) | **完整** (L813-847) | 同上 |
| dav_l311 | 3113 | **完整** (L900-933) | **完整** (L864-898) | 同上 |
| dav_5161 | 5101 | **未定义** | **未定义** | 未定义 Impl 函数，`#else` 路径调用不存在的函数 |
| dav_3510 | 3510 | **完整** (L1339-1372) | **完整** (L1374-1408) | `#if 3510` 路径传 cacheMode，签名匹配 |

## 各版本原始代码

### dav_3510（参照版本）

#### V1 DataCopyL0C2L1Impl — 完整实现（L1339-1372）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2L1Impl(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    if ASCEND_IS_AIC {
        static_assert(
            (SupportType<
                Tuple<U, T>, Tuple<float, float>, Tuple<float, half>, Tuple<float, bfloat16_t>, Tuple<float, int8_t>,
                Tuple<float, uint8_t>, Tuple<int32_t, int32_t>, Tuple<int32_t, int16_t>, Tuple<int32_t, int8_t>,
                Tuple<int32_t, uint8_t>, Tuple<int32_t, half>>()),
            "Failed to check dtype in "
            "DataCopy from L0C Buffer(CO1) to L1 Buffer(A1/B1), current api support dtype combination is "
            "src: float, dst: half / bfloat16_t / float / int8_t / uint8_t; "
            "src: int32_t, dst: half / int32_t / int16_t / int8_t / uint8_t.");
        if (IsSupportQuantMode(intriParams.quantPre)) {
            uint32_t dstStride = intriParams.dstStride;
            if (!intriParams.nz2ndEn) {
                dstStride = dstStride * ONE_BLK_SIZE / sizeof(T);
            }
            return copy_matrix_cc_to_cbuf(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride, 0, 0,
                intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0, false, false, false, false, false,
                false);
        } else {
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR, "Failed to check quantPre value in DataCopy from L0C Buffer(CO1) "
                                  "to L1 Buffer(A1/B1), supported values are NoQuant / F322F16 / F322BF16 / DEQF16 / "
                                  "VDEQF16 / QF322B8_PRE / "
                                  "VQF322B8_PRE / REQ8 / VREQ8.");
            });
        }
    }
}
```

#### V2 DataCopyL0C2GMImpl — 完整实现（L1374-1408）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2GMImpl(
    __gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams, uint8_t cacheMode)
{
    if ASCEND_IS_AIC {
        static_assert(
            (SupportType<
                Tuple<U, T>, Tuple<float, float>, Tuple<float, half>, Tuple<float, bfloat16_t>, Tuple<float, int8_t>,
                Tuple<float, uint8_t>, Tuple<int32_t, int32_t>, Tuple<int32_t, int16_t>, Tuple<int32_t, int8_t>,
                Tuple<int32_t, uint8_t>, Tuple<int32_t, half>>()),
            "Failed to check dtype in "
            "DataCopy from L0C Buffer(CO1) to GM, current api support dtype combination is "
            "src: float, dst: half / bfloat16_t / float / int8_t / uint8_t; "
            "src: int32_t, dst: half / int32_t / int16_t / int8_t / uint8_t.");
        if (IsSupportQuantMode(intriParams.quantPre)) {
            uint32_t dstStride = intriParams.dstStride;
            if (!intriParams.nz2ndEn) {
                dstStride = dstStride * ONE_BLK_SIZE / sizeof(T);
            }
            return copy_matrix_cc_to_gm(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride,
                cacheMode, 0, intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0, false, false, false, false, false,
                false);
        } else {
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "Failed to check quantPre value in DataCopy from L0C Buffer(CO1) "
                    "to GM, supported values are NoQuant / F322F16 / F322BF16 / DEQF16 / VDEQF16 / QF322B8_PRE / "
                    "VQF322B8_PRE / REQ8 / VREQ8.");
            });
        }
    }
}
```

---

### dav_l300（目标版本，3003）

#### V1 DataCopyL0C2L1Impl — 完整实现（L940-973）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2L1Impl(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    if ASCEND_IS_AIC {
        static_assert(
            (SupportType<
                Tuple<U, T>, Tuple<float, float>, Tuple<float, half>, Tuple<float, bfloat16_t>, Tuple<float, int8_t>,
                Tuple<float, uint8_t>, Tuple<int32_t, int32_t>, Tuple<int32_t, int16_t>, Tuple<int32_t, int8_t>,
                Tuple<int32_t, uint8_t>, Tuple<int32_t, half>>()),
            "Failed to check dtype in "
            "DataCopy from CO1 to A1 / B1, current api support dtype combination is "
            "src: float, dst: half / bfloat16_t / float / int8_t / uint8_t; "
            "src: int32_t, dst: half / int32_t / int16_t / int8_t / uint8_t.");
        if (IsSupportQuantMode(intriParams.quantPre)) {
            uint32_t dstStride = intriParams.dstStride;
            if (!intriParams.nz2ndEn) {
                dstStride = dstStride * ONE_BLK_SIZE / sizeof(T);
            }
            return copy_matrix_cc_to_cbuf(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride, 0, 0,
                intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0, false, false, false, false, false,
                false);
        } else {
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "Failed to check quantPre value in DataCopy from CO1 "
                    "to A1 / B1, supported values are NoQuant / F322F16 / F322BF16 / DEQF16 / VDEQF16 / QF322B8_PRE / "
                    "VQF322B8_PRE / REQ8 / VREQ8.");
            });
        }
    }
}
```

#### V2 DataCopyL0C2GMImpl — 完整实现（L904-938）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2GMImpl(
    __gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams, uint8_t cacheMode)
{
    if ASCEND_IS_AIC {
        static_assert(
            (SupportType<
                Tuple<U, T>, Tuple<float, float>, Tuple<float, half>, Tuple<float, bfloat16_t>, Tuple<float, int8_t>,
                Tuple<float, uint8_t>, Tuple<int32_t, int32_t>, Tuple<int32_t, int16_t>, Tuple<int32_t, int8_t>,
                Tuple<int32_t, uint8_t>, Tuple<int32_t, half>>()),
            "Failed to check dtype in "
            "DataCopy from CO1 to GM, current api support dtype combination is "
            "src: float, dst: half / bfloat16_t / float / int8_t / uint8_t; "
            "src: int32_t, dst: half / int32_t / int16_t / int8_t / uint8_t.");
        if (IsSupportQuantMode(intriParams.quantPre)) {
            uint32_t dstStride = intriParams.dstStride;
            if (!intriParams.nz2ndEn) {
                dstStride = dstStride * ONE_BLK_SIZE / sizeof(T);
            }
            return copy_matrix_cc_to_gm(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride,
                cacheMode, 0, intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0, false, false, false, false, false,
                false);
        } else {
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "Failed to check quantPre value in DataCopy from CO1 "
                    "to GM, supported values are NoQuant / F322F16 / F322BF16 / DEQF16 / VDEQF16 / QF322B8_PRE / "
                    "VQF322B8_PRE / REQ8 / VREQ8.");
            });
        }
    }
}
```

---

### dav_l310（目标版本，3103）

#### V1 DataCopyL0C2L1Impl — 完整实现（L849-882）

> 与 l300 代码逻辑完全相同，仅错误消息措辞略有差异（"CO1 to A1 / B1" vs "CO1 to A1 / B1"）。

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2L1Impl(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    if ASCEND_IS_AIC {
        static_assert(
            (SupportType<
                Tuple<U, T>, Tuple<float, float>, Tuple<float, half>, Tuple<float, bfloat16_t>, Tuple<float, int8_t>,
                Tuple<float, uint8_t>, Tuple<int32_t, int32_t>, Tuple<int32_t, int16_t>, Tuple<int32_t, int8_t>,
                Tuple<int32_t, uint8_t>, Tuple<int32_t, half>>()),
            "Failed to check dtype in "
            "DataCopy from CO1 to A1 / B1, current api support dtype combination is "
            "src: float, dst: half / bfloat16_t / float / int8_t / uint8_t; "
            "src: int32_t, dst: half / int32_t / int16_t / int8_t / uint8_t.");
        if (IsSupportQuantMode(intriParams.quantPre)) {
            uint32_t dstStride = intriParams.dstStride;
            if (!intriParams.nz2ndEn) {
                dstStride = dstStride * ONE_BLK_SIZE / sizeof(T);
            }
            return copy_matrix_cc_to_cbuf(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride, 0, 0,
                intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0, false, false, false, false, false,
                false);
        } else {
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "Failed to check quantPre value in DataCopy from CO1 "
                    "to A1 / B1, supported values are NoQuant / F322F16 / F322BF16 / DEQF16 / VDEQF16 / QF322B8_PRE / "
                    "VQF322B8_PRE / REQ8 / VREQ8.");
            });
        }
    }
}
```

#### V2 DataCopyL0C2GMImpl — 完整实现（L813-847）

> 与 l300 代码逻辑完全相同。

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2GMImpl(
    __gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams, uint8_t cacheMode)
{
    if ASCEND_IS_AIC {
        static_assert(
            (SupportType<
                Tuple<U, T>, Tuple<float, float>, Tuple<float, half>, Tuple<float, bfloat16_t>, Tuple<float, int8_t>,
                Tuple<float, uint8_t>, Tuple<int32_t, int32_t>, Tuple<int32_t, int16_t>, Tuple<int32_t, int8_t>,
                Tuple<int32_t, uint32_t>, Tuple<int32_t, half>>()),
            "Failed to check dtype in "
            "DataCopy from CO1 to GM, current api support dtype combination is "
            "src: float, dst: half / bfloat16_t / float / int8_t / uint8_t; "
            "src: int32_t, dst: half / int32_t / int16_t / int8_t / uint8_t.");
        if (IsSupportQuantMode(intriParams.quantPre)) {
            uint32_t dstStride = intriParams.dstStride;
            if (!intriParams.nz2ndEn) {
                dstStride = dstStride * ONE_BLK_SIZE / sizeof(T);
            }
            return copy_matrix_cc_to_gm(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride,
                cacheMode, 0, intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0, false, false, false, false, false,
                false);
        } else {
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "Failed to check quantPre value in DataCopy from CO1 "
                    "to GM, supported values are NoQuant / F322F16 / F322BF16 / DEQF16 / VDEQF16 / QF322B8_PRE / "
                    "VQF322B8_PRE / REQ8 / VREQ8.");
            });
        }
    }
}
```

---

### dav_l311（目标版本，3113）

#### V1 DataCopyL0C2L1Impl — 完整实现（L900-933）

> 与 l300/l310 代码逻辑完全相同。

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2L1Impl(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    if ASCEND_IS_AIC {
        static_assert(
            (SupportType<
                Tuple<U, T>, Tuple<float, float>, Tuple<float, half>, Tuple<float, bfloat16_t>, Tuple<float, int8_t>,
                Tuple<float, uint8_t>, Tuple<int32_t, int32_t>, Tuple<int32_t, int16_t>, Tuple<int32_t, int8_t>,
                Tuple<int32_t, uint8_t>, Tuple<int32_t, half>>()),
            "Failed to check dtype in "
            "DataCopy from CO1 to A1 / B1, current api support dtype combination is "
            "src: float, dst: half / bfloat16_t / float / int8_t / uint8_t; "
            "src: int32_t, dst: half / int32_t / int16_t / int8_t / uint8_t.");
        if (IsSupportQuantMode(intriParams.quantPre)) {
            uint32_t dstStride = intriParams.dstStride;
            if (!intriParams.nz2ndEn) {
                dstStride = dstStride * ONE_BLK_SIZE / sizeof(T);
            }
            return copy_matrix_cc_to_cbuf(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride, 0, 0,
                intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0, false, false, false, false, false,
                false);
        } else {
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "Failed to check quantPre value in DataCopy from CO1 "
                    "to A1 / B1, supported values are NoQuant / F322F16 / F322BF16 / DEQF16 / VDEQF16 / QF322B8_PRE / "
                    "VQF322B8_PRE / REQ8 / VREQ8.");
            });
        }
    }
}
```

#### V2 DataCopyL0C2GMImpl — 完整实现（L864-898）

> 与 l300/l310 代码逻辑完全相同。

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2GMImpl(
    __gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams, uint8_t cacheMode)
{
    if ASCEND_IS_AIC {
        static_assert(
            (SupportType<
                Tuple<U, T>, Tuple<float, float>, Tuple<float, half>, Tuple<float, bfloat16_t>, Tuple<float, int8_t>,
                Tuple<float, uint8_t>, Tuple<int32_t, int32_t>, Tuple<int32_t, int16_t>, Tuple<int32_t, int8_t>,
                Tuple<int32_t, uint8_t>, Tuple<int32_t, half>>()),
            "Failed to check dtype in "
            "DataCopy from CO1 to GM, current api support dtype combination is "
            "src: float, dst: half / bfloat16_t / float / int8_t / uint8_t; "
            "src: int32_t, dst: half / int32_t / int16_t / int8_t / uint8_t.");
        if (IsSupportQuantMode(intriParams.quantPre)) {
            uint32_t dstStride = intriParams.dstStride;
            if (!intriParams.nz2ndEn) {
                dstStride = dstStride * ONE_BLK_SIZE / sizeof(T);
            }
            return copy_matrix_cc_to_gm(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride,
                cacheMode, 0, intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0, false, false, false, false, false,
                false);
        } else {
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "Failed to check quantPre value in DataCopy from CO1 "
                    "to GM, supported values are NoQuant / F322F16 / F322BF16 / DEQF16 / VDEQF16 / QF322B8_PRE / "
                    "VQF322B8_PRE / REQ8 / VREQ8.");
            });
        }
    }
}
```

---

### dav_5161（目标版本，5101）

#### V1 DataCopyL0C2L1Impl — 未定义

> dav_5161 的 `kernel_operator_data_copy_impl.h` 中**未定义** `DataCopyL0C2L1Impl`（带 `DataCopyCO12DstParams` 参数的版本）。
>
> 文件中仅有 L0C 相关的桩函数（使用 `DataCopyParams` + `DataCopyEnhancedParams`）：
> - `DataCopyL12L0CImpl` (L715-720): `ASSERT(false && "unsupported data copy L1 to L0C on current device")`
> - `DataCopyL0C2UBImpl` (L722-727): `ASSERT(false && "unsupported data copy L0C to UB on current device")`
> - `DataCopyUB2L0CImpl` (L729-734): `ASSERT(false && "unsupported data copy UB to L0C on current device")`

#### V2 DataCopyL0C2GMImpl — 未定义

> 同样未定义。dav_5161 中不存在 `DataCopyL0C2GMImpl`（带 `DataCopyCO12DstParams` 参数的版本）。

---

## 代码级实现差异分析

### 实现风格对比

| 特征 | dav_3510 | dav_l300 | dav_l310 | dav_l311 | dav_5161 |
|------|----------|----------|----------|----------|----------|
| V1 (L0C→L1) | 完整 | 完整 | 完整 | 完整 | 未定义 |
| V2 (L0C→GM) | 完整 | 完整 | 完整 | 完整 | 未定义 |
| V2 函数签名 | 4参数 (含 cacheMode) | 4参数 (含 cacheMode) | 4参数 (含 cacheMode) | 4参数 (含 cacheMode) | N/A |
| AIC 守卫 | `if ASCEND_IS_AIC` | `if ASCEND_IS_AIC` | `if ASCEND_IS_AIC` | `if ASCEND_IS_AIC` | N/A |
| 类型检查 | `SupportType<Tuple<...>>` | 同 3510 | 同 3510 | 同 3510 | N/A |
| 硬件指令 (V1) | `copy_matrix_cc_to_cbuf` | `copy_matrix_cc_to_cbuf` | `copy_matrix_cc_to_cbuf` | `copy_matrix_cc_to_cbuf` | N/A |
| 硬件指令 (V2) | `copy_matrix_cc_to_gm` | `copy_matrix_cc_to_gm` | `copy_matrix_cc_to_gm` | `copy_matrix_cc_to_gm` | N/A |
| 量化模式检查 | `IsSupportQuantMode` | `IsSupportQuantMode` | `IsSupportQuantMode` | `IsSupportQuantMode` | N/A |
| 错误消息措辞 | "L0C Buffer(CO1) to L1 Buffer(A1/B1)" | "CO1 to A1 / B1" | "CO1 to A1 / B1" | "CO1 to A1 / B1" | N/A |
| 支持数据类型 | float→{float,half,bf16,int8,uint8}, int32→{int32,int16,int8,uint8,half} | 同 3510 | 同 3510 | 同 3510 | N/A |

### intf_impl.h V2 的 cacheMode 传递问题

| 版本 | intf_impl.h 分支 | 调用签名 | 实际 Impl 签名 | 是否匹配 |
|------|-----------------|---------|--------------|---------|
| 3510/5102 | `#if 3510\|\|5102` | `DataCopyL0C2GMImpl(..., intriParams, cacheMode)` — 4参数 | `DataCopyL0C2GMImpl(..., intriParams, cacheMode)` — 4参数 | ✓ |
| l300/l310/l311 | `#else` | `DataCopyL0C2GMImpl(..., intriParams)` — 3参数 | `DataCopyL0C2GMImpl(..., intriParams, cacheMode)` — 4参数 | **✗ 不匹配** |
| 5161 | `#else` | `DataCopyL0C2GMImpl(..., intriParams)` — 3参数 | 未定义 | **✗ 未定义** |

> **关键发现：** intf_impl.h 的 `#else` 分支调用 3 参数版 `DataCopyL0C2GMImpl`，但 l300/l310/l311 只定义了 4 参数版（含 cacheMode）。由于 C++ 模板仅在实例化时检查，如果用户在 l300/l310/l311 上调用 V2（L0C→GM），会导致编译错误。

### 实现模式总结

**dav_3510 / dav_l300 / dav_l310 / dav_l311 — 统一模式**
- 所有 4 个版本的 `DataCopyL0C2L1Impl` 和 `DataCopyL0C2GMImpl` 函数体逻辑完全相同
- 调用相同的硬件指令 `copy_matrix_cc_to_cbuf` / `copy_matrix_cc_to_gm`
- 使用相同的 `SupportType` 类型检查和 `IsSupportQuantMode` 量化模式检查
- 唯一差异：错误消息措辞（3510 用 "L0C Buffer(CO1) to L1 Buffer(A1/B1)"，l300/l310/l311 用 "CO1 to A1 / B1"）

**dav_5161 — 缺失**
- 完全未定义 `DataCopyL0C2L1Impl` / `DataCopyL0C2GMImpl`（带 `DataCopyCO12DstParams`）
- 5101 架构使用宏指令，不支持 `copy_matrix_cc_to_cbuf` / `copy_matrix_cc_to_gm` 硬件指令

### 关键差异点

1. **dav_5161 完全缺失** — V1/V2 均未定义，5101 不支持 L0C→L1/GM 的矩阵拷贝
2. **intf_impl.h V2 的 #else 分支签名不匹配** — `#else` 调用 3 参数版，但 l300/l310/l311 只有 4 参数版（含 cacheMode），潜在编译问题
3. **dav_3510 错误消息更详细** — 使用 "L0C Buffer(CO1) to L1 Buffer(A1/B1)"，l300/l310/l311 简写为 "CO1 to A1 / B1"
4. **4 个完整版本的实现逻辑完全相同** — 除错误消息措辞外无功能差异

## 待确认问题

1. **dav_5161 的 V1/V2 未定义** — 5101 架构是否支持 `copy_matrix_cc_to_cbuf` / `copy_matrix_cc_to_gm` 指令？如不支持，是否需要在 dav_5161 中补充桩函数？还是该接口在 5101 上不应被使用？
2. **intf_impl.h V2 的 #else 分支签名不匹配** — `#else` 调用 3 参数版 `DataCopyL0C2GMImpl(..., intriParams)`，但 l300/l310/l311 的实际定义为 4 参数版（含 cacheMode）。这是否是一个 bug？应修改 `#else` 分支传 cacheMode，还是为 l300/l310/l311 补充 3 参数重载？
3. **dav_l300/l310/l311 的 V2 是否真的在 #else 路径？** — 如果实际编译时走 `#else`，则 V2 接口在 l300/l310/l311 上完全不可用（编译失败）。还是说这些版本实际有其他机制绕过此问题（如通过 `#if` 在编译时排除）？
4. **错误消息措辞不统一** — 3510 用 "L0C Buffer(CO1) to L1 Buffer(A1/B1)"，l300/l310/l311 用 "CO1 to A1 / B1"。是否需要统一？
5. **dav_l310/l311 与 l300 的 V2 代码完全相同** — 是否确认 l310/l311 硬件支持 `copy_matrix_cc_to_gm` 指令？
