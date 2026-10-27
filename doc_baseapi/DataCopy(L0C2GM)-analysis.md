# 案例分析：DataCopy(L0C→GM/L1, CO12Dst) 接口跨版本适配

## 接口概述

`DataCopy` 使用 `DataCopyCO12DstParams` 参数，用于 L0C(CO1)→GM 或 L0C(CO1)→L1 的 Cube 数据拷贝（支持量化、ReLU、通道拆分、NZ→ND 转换）。有两个变体：

| 变体 | 参数类型 | 声明位置 | arch 守卫 |
|------|---------|---------|----------|
| V1 (L0C→L1) | `(LocalTensor<T>&, LocalTensor<U>&, DataCopyCO12DstParams&)` | `intf.h:313-315` | 无 |
| V2 (L0C→GM) | `(GlobalTensor<T>&, LocalTensor<U>&, DataCopyCO12DstParams&)` | `intf.h:317-319` | 无 |

两个声明均无 `__NPU_ARCH__` 守卫，所有版本均编译。

### 辅助类型

**`DataCopyCO12DstParams`**（struct，`kernel_struct_data_copy.h:162-190`）：
- 12 个成员字段：`sid`, `nSize`, `mSize`, `dstStride`, `srcStride`, `unitFlag`, `clipReluPre`, `eltWiseOp`, `quantPre`(QuantMode_t), `reluPre`, `channelSplit`, `nz2ndEn`
- 支持量化模式（`QuantMode_t`：NoQuant/F322F16/F322BF16/DEQF16/VDEQF16/QF322B8_PRE/VQF322B8_PRE/REQ8/VREQ8）
- 支持 NZ→ND 转换（`nz2ndEn`）
- 支持通道拆分（`channelSplit`）

## 调用链

### V1 调用链（L0C→L1，无 arch 守卫）

```
intf.h:313-315（声明，无 arch 守卫）
  → intf_impl.h:1203-1214（公共包装，无 arch 守卫）
    → DataCopyL0C2L1Impl(dst, src, intriParams)  // 3参数，无cacheMode
      → dav_XXXX/kernel_operator_data_copy_impl.h（版本特定实现）
```

### V2 调用链（L0C→GM，无 arch 守卫，有条件 cacheMode）

```
intf.h:317-319（声明，无 arch 守卫）
  → intf_impl.h:1216-1230（公共包装，无 arch 守卫）
    → 3510/5102: DataCopyL0C2GMImpl(dst, src, intriParams, cacheMode)  // 4参数
    → 其他:      DataCopyL0C2GMImpl(dst, src, intriParams)             // 3参数
      → dav_XXXX/kernel_operator_data_copy_impl.h（版本特定实现）
```

### 共享层分发逻辑（intf_impl.h）

**V1 包装**（L1203-1214，无 arch 守卫）：
```cpp
template <typename T, typename U>
__aicore__ inline void DataCopy(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const DataCopyCO12DstParams& intriParams)
{
    CheckTensorPos<U>(src, Hardware::L0C, "src", "CO1", "...");
    CheckTensorPos<T>(dst, Hardware::L1, "dst", "A1", "...");
    ASCENDC_REPORT_OVERFLOW_MEM((CheckDataCopyTensorSizeOverflow(dst, src, intriParams)));
    // l0c -> l1
    DataCopyL0C2L1Impl((__cbuf__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams);
}
```

**V2 包装**（L1216-1230，无 arch 守卫）：
```cpp
template <typename T, typename U>
__aicore__ inline void DataCopy(
    const GlobalTensor<T>& dst, const LocalTensor<U>& src, const DataCopyCO12DstParams& intriParams)
{
    CheckTensorPos<U>(src, Hardware::L0C, "src", "CO1", "...");
    ASCENDC_REPORT_OVERFLOW_MEM((CheckDataCopyTensorSizeOverflow(dst, src, intriParams)));
    // l0c -> gm
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    const uint8_t cacheMode = ExtractCacheMode(dst);
    DataCopyL0C2GMImpl((__gm__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams, cacheMode);
#else
    DataCopyL0C2GMImpl((__gm__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams);
#endif
}
```

**关键问题**：V2 包装层在 `3510/5102` 分支调用 4 参数版本，在 `else` 分支调用 3 参数版本。但所有版本特定实现文件中的 `DataCopyL0C2GMImpl` 均为 **4 参数**签名（含 `uint8_t cacheMode`）。因此：

- **3510/5102**：4参数调用 → 4参数定义 ✓
- **3003/3103/3113**：3参数调用 → 4参数定义 → **编译错误**（参数数量不匹配）
- **5101**：3参数调用 → **未定义** → **链接错误**

## 支持矩阵

| 版本 | Arch | V1 (L0C→L1) `DataCopyL0C2L1Impl` | V2 (L0C→GM) `DataCopyL0C2GMImpl` | V2 调用路径 | V2 签名匹配 |
|------|------|----------------------------------|----------------------------------|------------|-------------|
| dav_3510 | 3510 | **完整** (L1339-1372) | **完整** (L1375-1408, 4参) | 4参数调用 | ✓ 匹配 |
| dav_l300 | 3003 | **完整** (L940-973) | **完整** (L905-938, 4参) | 3参数调用 | ✗ **不匹配** |
| dav_l310 | 3103 | **完整** (L849-882) | **完整** (L814-847, 4参) | 3参数调用 | ✗ **不匹配** |
| dav_l311 | 3113 | **完整** (L900-933) | **完整** (L865-898, 4参) | 3参数调用 | ✗ **不匹配** |
| dav_5161 | 5101 | **未定义** | **未定义** | 3参数调用 | ✗ **链接错误** |

## 各版本原始代码

### dav_3510（参照版本，3510）

#### `DataCopyL0C2L1Impl` — 完整实现（L1339-1372）

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

#### `DataCopyL0C2GMImpl` — 完整实现（L1375-1408，4参数）

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

#### `DataCopyL0C2L1Impl` — 完整实现（L940-973）

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

#### `DataCopyL0C2GMImpl` — 完整实现（L905-938，4参数）

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

> **注意**：dav_l300 的 `DataCopyL0C2GMImpl` 签名为 4 参数（含 `cacheMode`），但共享层 `intf_impl.h:1228` 的 else 分支以 3 参数调用 → **签名不匹配，编译错误**。

---

### dav_l310（目标版本，3103）

#### `DataCopyL0C2L1Impl` — 完整实现（L849-882）

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

#### `DataCopyL0C2GMImpl` — 完整实现（L814-847，4参数）

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

> **注意**：dav_l310 的 `DataCopyL0C2GMImpl` 签名为 4 参数（含 `cacheMode`），但共享层以 3 参数调用 → **签名不匹配，编译错误**。代码与 dav_l300 完全一致（仅行号不同）。

---

### dav_l311（目标版本，3113）

#### `DataCopyL0C2L1Impl` — 完整实现（L900-933）

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

#### `DataCopyL0C2GMImpl` — 完整实现（L865-898，4参数）

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

> **注意**：dav_l311 的 `DataCopyL0C2GMImpl` 签名为 4 参数（含 `cacheMode`），但共享层以 3 参数调用 → **签名不匹配，编译错误**。代码与 dav_l300/l310 完全一致（仅行号不同）。

---

### dav_5161（目标版本，5101）

#### `DataCopyL0C2L1Impl` — 未定义

> 文件中不存在此函数。

#### `DataCopyL0C2GMImpl` — 未定义

> 文件中不存在此函数。

> **注意**：dav_5161 中的 L0C 相关函数（`DataCopyL0C2UBImpl`/`DataCopyL12L0CImpl`/`DataCopyUB2L0CImpl`）均为桩函数（`ASSERT(false && ...)`），但未定义 `DataCopyL0C2GMImpl` 和 `DataCopyL0C2L1Impl`。共享层 V2 以 3 参数调用 `DataCopyL0C2GMImpl` → **链接错误**。

## 代码级实现差异分析

### 风格对比表

| 特征 | dav_3510 (3510) | dav_l300 (3003) | dav_l310 (3103) | dav_l311 (3113) | dav_5161 (5101) |
|------|-----------------|----------------|----------------|----------------|----------------|
| V1 `DataCopyL0C2L1Impl` | **完整** | **完整** | **完整** | **完整** | **未定义** |
| V2 `DataCopyL0C2GMImpl` | **完整**(4参) | **完整**(4参) | **完整**(4参) | **完整**(4参) | **未定义** |
| `if ASCEND_IS_AIC` 守卫 | ✓ 有 | ✓ 有 | ✓ 有 | ✓ 有 | N/A |
| `static_assert` 类型检查 | ✓ `SupportType` | ✓ `SupportType` | ✓ `SupportType` | ✓ `SupportType` | N/A |
| 硬件指令 (L1) | `copy_matrix_cc_to_cbuf` | `copy_matrix_cc_to_cbuf` | `copy_matrix_cc_to_cbuf` | `copy_matrix_cc_to_cbuf` | N/A |
| 硬件指令 (GM) | `copy_matrix_cc_to_gm` | `copy_matrix_cc_to_gm` | `copy_matrix_cc_to_gm` | `copy_matrix_cc_to_gm` | N/A |
| cacheMode 参数 | ✓ (第4参数) | ✓ (第4参数) | ✓ (第4参数) | ✓ (第4参数) | N/A |
| 错误消息文本 | `"L0C Buffer(CO1)"` | `"CO1"` | `"CO1"` | `"CO1"` | N/A |
| 支持的量化模式 | NoQuant/F322F16/.../VREQ8 | 同 3510 | 同 3510 | 同 3510 | N/A |
| V2 调用路径签名匹配 | ✓ (4参→4参) | ✗ (3参→4参) | ✗ (3参→4参) | ✗ (3参→4参) | ✗ (3参→未定义) |

### 实现模式总结

**所有版本实现完全一致**（dav_3510/l300/l310/l311），唯一差异为错误消息文本：
- **dav_3510**：错误消息使用 `"L0C Buffer(CO1) to L1 Buffer(A1/B1)"` / `"L0C Buffer(CO1) to GM"` — 完整的 buffer 名称
- **dav_l300/l310/l311**：错误消息使用 `"CO1 to A1 / B1"` / `"CO1 to GM"` — 简短的 buffer 名称

**dav_5161 完全缺失**：未定义 `DataCopyL0C2GMImpl` 和 `DataCopyL0C2L1Impl`。

### 关键差异点

1. **V2 签名不匹配问题（严重）**：共享层 `intf_impl.h:1224-1229` 在 `else` 分支（非 3510/5102）以 3 参数调用 `DataCopyL0C2GMImpl`，但 l300/l310/l311 的实际定义为 4 参数（含 `cacheMode`）。这将导致**编译错误**。

2. **dav_5161 完全缺失**：5101 既未定义 `DataCopyL0C2GMImpl` 也未定义 `DataCopyL0C2L1Impl`。V1 和 V2 的公共包装函数均无 arch 守卫，5101 编译时将触发**链接错误**。

3. **V1 (L0C→L1) 无签名问题**：V1 的 `DataCopyL0C2L1Impl` 在所有版本中均为 3 参数，共享层也以 3 参数调用，签名匹配。但 5101 未定义 → 链接错误。

4. **cacheMode 参数未使用**：l300/l310/l311 的 `DataCopyL0C2GMImpl` 虽然定义了 `cacheMode` 参数，但因共享层 `else` 分支不传该参数，实际上该参数从未被传入（编译错误已阻止调用）。

5. **copy_matrix_cc_to_gm ISA 签名**：3510 和 l300/l310/l311 的 `copy_matrix_cc_to_gm` 调用参数列表完全一致（25 个参数），`cacheMode` 作为第 8 个参数传入。

## 待确认问题

### 1. [严重] V2 (L0C→GM) 签名不匹配：3参数调用 vs 4参数定义

共享层 `intf_impl.h:1228` 的 `else` 分支（3003/3103/3113/5101）以 3 参数调用 `DataCopyL0C2GMImpl`，但 l300/l310/l311 的定义为 4 参数（含 `cacheMode`）。

**解决方案选项**：
- **方案 A**：将 `else` 分支的 arch 守卫扩展到 l300/l310/l311（使他们也走 4 参数路径，`ExtractCacheMode` + 4参数调用）
- **方案 B**：为 l300/l310/l311 新增 3 参数重载（`cacheMode` 默认为 0）

**推荐方案 B**：在 l300/l310/l311 中新增 3 参数重载，内部委托到 4 参数版本（`cacheMode = 0`）。这与共享层的 `else` 分支逻辑一致（不使用 cacheMode）。

### 2. [严重] dav_5161 缺失 `DataCopyL0C2L1Impl` 和 `DataCopyL0C2GMImpl`

5101 未定义这两个函数，但 V1/V2 的公共包装函数无 arch 守卫，5101 编译时将触发链接错误。

**解决方案**：为 dav_5161 新增桩函数（`DataCopyL0C2L1Impl` 3参数 + `DataCopyL0C2GMImpl` 3参数），使用 `ASCENDC_ASSERT(false, ...)` 格式。

**需确认**：5101 硬件是否支持 `copy_matrix_cc_to_gm` / `copy_matrix_cc_to_cbuf` 指令？若支持，应移植完整实现；若不支持，应加桩函数。

### 3. dav_5161 的 L0C 相关函数风格不一致

dav_5161 中 `DataCopyL0C2UBImpl`/`DataCopyL12L0CImpl`/`DataCopyUB2L0CImpl` 均为桩函数（`ASSERT(false && ...)` 格式），但 `DataCopyL0C2L1Impl`/`DataCopyL0C2GMImpl` 完全缺失。是否应该统一为桩函数？

### 4. cacheMode 在 l300/l310/l311/5101 上的硬件支持

`cacheMode` 参数传入 `copy_matrix_cc_to_gm` 的第 8 个参数。3510/5102 通过 `ExtractCacheMode` 提取 cacheMode，其他版本不提取。l300/l310/l311 的实现定义了 `cacheMode` 参数（4参数签名），但共享层 `else` 分支不传该参数。cacheMode 在这些版本上是否有意义？

**推测**：l300/l310/l311 可能不支持 cacheMode（硬件无 cache 机制），因此共享层 `else` 分支不传。但版本特定实现仍定义了 4 参数签名（从 3510 复制时遗留），导致签名不匹配。
