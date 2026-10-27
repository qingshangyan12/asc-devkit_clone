# 适配报告：DataCopy(L0C-L1/L0C-GM) 接口

## 接口概述

`DataCopy(..., DataCopyCO12DstParams)` 用于 L0C→L1 和 L0C→GM 的矩阵数据拷贝。有 2 个重载变体：

| 变体 | 参数签名 | 声明位置 | 说明 |
|------|---------|---------|------|
| V1 | `DataCopy(LocalTensor<T>& dst, LocalTensor<U>& src, DataCopyCO12DstParams)` | `intf.h:313-315` | L0C → L1 |
| V2 | `DataCopy(GlobalTensor<T>& dst, LocalTensor<U>& src, DataCopyCO12DstParams)` | `intf.h:317-319` | L0C → GM |

## 适配前支持矩阵

| 版本 | Arch | V1 (L0C→L1) | V2 (L0C→GM) | 问题 |
|------|------|-------------|-------------|------|
| dav_3510 | 3510 | **完整** (L1339-1372) | **完整** (L1374-1408) | 无（`#if` 路径，4 参数签名匹配） |
| dav_l300 | 3003 | **完整** (L940-973) | **完整** (L904-938) | V2 签名 4 参数 vs `#else` 调用 3 参数不匹配；ISA 调用 25 参数，硬件仅支持 22 参数 |
| dav_l310 | 3103 | **完整** (L849-882) | **完整** (L813-847) | 同上；ISA 调用 25 参数，硬件仅支持 24 参数 |
| dav_l311 | 3113 | **完整** (L900-933) | **完整** (L864-898) | 同上；ISA 调用 25 参数，硬件仅支持 24 参数 |
| dav_5161 | 5101 | **未定义** | **未定义** | 5101 不支持 L0C→L1/GM 矩阵拷贝，`#else` 路径调用不存在的函数 |

## 适配后支持矩阵

| 版本 | Arch | V1 (L0C→L1) | V2 (L0C→GM) | 适配内容 |
|------|------|-------------|-------------|---------|
| dav_3510 | 3510 | 维持（完整实现） | 维持（完整实现） | 无修改 |
| dav_l300 | 3003 | ISA 25→22 参数 | 签名 4→3 参数 + ISA 25→22 参数 | 去除 cacheMode，ISA 参数匹配 fixpipe L248-252 / L315-320 |
| dav_l310 | 3103 | ISA 25→24 参数 | 签名 4→3 参数 + ISA 25→24 参数 | 去除 cacheMode，ISA 参数匹配 fixpipe L252-256 / L322-327 |
| dav_l311 | 3113 | ISA 25→24 参数 | 签名 4→3 参数 + ISA 25→24 参数 | 同 dav_l310 |
| dav_5161 | 5101 | **新增桩函数** | **新增桩函数** | 3 参数桩函数，匹配 `#else` 路径 3 参数调用 |

### ISA 参数数量参照

| 版本 | Arch | `copy_matrix_cc_to_cbuf` 参数数 | `copy_matrix_cc_to_gm` 参数数 | 参照文件 |
|------|------|------|------|---------|
| dav_l300 | 3003 | 22 | 22 | `dav_l300/kernel_operator_fixpipe_impl.h:248-252 / :315-320` |
| dav_l310 | 3103 | 24 | 24 | `dav_l310/kernel_operator_fixpipe_impl.h:252-256 / :322-327` |
| dav_l311 | 3113 | 24 | 24 | 同 l310 结构 |
| dav_3510 | 3510 | 25 | 25 | 现有实现（含 `cacheMode`，无 `nz2dnEn`） |

### 共享层 intf_impl.h V2 的 arch 守卫

`intf_impl.h:1224-1229`（**未修改**）：

```cpp
#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 5102))
    const uint8_t cacheMode = ExtractCacheMode(dst);
    DataCopyL0C2GMImpl((__gm__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams, cacheMode);
#else
    DataCopyL0C2GMImpl((__gm__ PrimT<T>*)dst.GetPhyAddr(), (__cc__ PrimT<U>*)src.GetPhyAddr(), intriParams);
#endif
```

> `#else` 分支（3003/3103/3113/5101）调用 **3 参数版** `DataCopyL0C2GMImpl`。适配后 l300/l310/l311/5161 的签名均为 3 参数，与 `#else` 路径匹配。

## 修改的文件清单

| 文件 | 修改内容 |
|------|---------|
| `impl/basic_api/dav_5161/kernel_operator_data_copy_impl.h` | L729-740: **新增** `DataCopyL0C2L1Impl` (3 参数桩) + `DataCopyL0C2GMImpl` (3 参数桩) |
| `impl/basic_api/dav_l300/kernel_operator_data_copy_impl.h` | L904-937: `DataCopyL0C2GMImpl` 签名 4→3 参数 + ISA 25→22 参数；L940-971: `DataCopyL0C2L1Impl` ISA 25→22 参数 |
| `impl/basic_api/dav_l310/kernel_operator_data_copy_impl.h` | L813-847: `DataCopyL0C2GMImpl` 签名 4→3 参数 + ISA 25→24 参数；L849-882: `DataCopyL0C2L1Impl` ISA 25→24 参数 |
| `impl/basic_api/dav_l311/kernel_operator_data_copy_impl.h` | L864-898: `DataCopyL0C2GMImpl` 签名 4→3 参数 + ISA 25→24 参数；L900-933: `DataCopyL0C2L1Impl` ISA 25→24 参数 |

> dav_3510 的 impl 文件未修改。intf_impl.h 共享层未修改。

## 适配后各版本代码

### dav_3510（参照版本，3510，未修改）

#### V2 DataCopyL0C2GMImpl — 完整实现（L1374-1408，4 参数签名，25 参数 ISA）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2GMImpl(
    __gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams, uint8_t cacheMode)
{
    if ASCEND_IS_AIC {
        // ... static_assert + IsSupportQuantMode check ...
            return copy_matrix_cc_to_gm(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride,
                cacheMode, 0, intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0, false, false, false, false, false,
                false);
        // ...
    }
}
```

#### V1 DataCopyL0C2L1Impl — 完整实现（L1339-1372，3 参数签名，25 参数 ISA）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2L1Impl(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    if ASCEND_IS_AIC {
        // ... static_assert + IsSupportQuantMode check ...
            return copy_matrix_cc_to_cbuf(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride, 0, 0,
                intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0, false, false, false, false, false,
                false);
        // ...
    }
}
```

---

### dav_l300（目标版本，3003）

#### V2 DataCopyL0C2GMImpl — 完整实现（L904-937，**签名 4→3 参数**，**ISA 25→22 参数**）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2GMImpl(
    __gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    if ASCEND_IS_AIC {
        // ... static_assert + IsSupportQuantMode check ...
            return copy_matrix_cc_to_gm(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride,
                0, intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0, false, false, false, false);
        // ...
    }
}
```

#### V1 DataCopyL0C2L1Impl — 完整实现（L940-971，3 参数签名，**ISA 25→22 参数**）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2L1Impl(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    if ASCEND_IS_AIC {
        // ... static_assert + IsSupportQuantMode check ...
            return copy_matrix_cc_to_cbuf(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride,
                0, intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0, false, false, false, false);
        // ...
    }
}
```

---

### dav_l310（目标版本，3103）

#### V2 DataCopyL0C2GMImpl — 完整实现（L813-847，**签名 4→3 参数**，**ISA 25→24 参数**）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2GMImpl(
    __gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    if ASCEND_IS_AIC {
        // ... static_assert + IsSupportQuantMode check ...
            return copy_matrix_cc_to_gm(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride,
                0, intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0,
                false, false, false, false, false, false);
        // ...
    }
}
```

#### V1 DataCopyL0C2L1Impl — 完整实现（L849-882，3 参数签名，**ISA 25→24 参数**）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2L1Impl(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    if ASCEND_IS_AIC {
        // ... static_assert + IsSupportQuantMode check ...
            return copy_matrix_cc_to_cbuf(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride,
                0, intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0,
                false, false, false, false, false, false);
        // ...
    }
}
```

---

### dav_l311（目标版本，3113）

#### V2 DataCopyL0C2GMImpl — 完整实现（L864-898，**签名 4→3 参数**，**ISA 25→24 参数**）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2GMImpl(
    __gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    if ASCEND_IS_AIC {
        // ... static_assert + IsSupportQuantMode check ...
            return copy_matrix_cc_to_gm(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride,
                0, intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0,
                false, false, false, false, false, false);
        // ...
    }
}
```

#### V1 DataCopyL0C2L1Impl — 完整实现（L900-933，3 参数签名，**ISA 25→24 参数**）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2L1Impl(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    if ASCEND_IS_AIC {
        // ... static_assert + IsSupportQuantMode check ...
            return copy_matrix_cc_to_cbuf(
                dst, src, intriParams.sid, intriParams.nSize, intriParams.mSize, dstStride, intriParams.srcStride,
                0, intriParams.unitFlag, static_cast<uint64_t>(intriParams.quantPre), intriParams.reluPre,
                intriParams.channelSplit, intriParams.nz2ndEn, 0, 0, false, false, 0,
                false, false, false, false, false, false);
        // ...
    }
}
```

---

### dav_5161（目标版本，5101）

#### V1 DataCopyL0C2L1Impl — 桩函数（L729-733，**新增**）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2L1Impl(__cbuf__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy L0C to L1 on current device"); });
}
```

#### V2 DataCopyL0C2GMImpl — 桩函数（L735-740，**新增**）

```cpp
template <typename T, typename U>
__aicore__ inline void DataCopyL0C2GMImpl(
    __gm__ T* dst, __cc__ U* src, const DataCopyCO12DstParams& intriParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "unsupported data copy L0C to GM on current device"); });
}
```

## ISA 参数映射说明

### 3510 → l300 (25→22 参数)

去除 `cacheMode`（第 8 参数）及其后的 `0`（第 9 参数），尾部去除 2 个 `false`。参照 fixpipe `dav_l300/kernel_operator_fixpipe_impl.h:248-252`（cbuf）和 `:315-320`（gm）。

| 参数位置 | 3510 (25 参数) | l300 (22 参数) | fixpipe 参照 |
|---------|---------------|---------------|-------------|
| 8 | `cacheMode` / `0` | `0` | `0` |
| 9 | `0` | `unitFlag` | `unitFlag` |
| 10 | `unitFlag` | `quantPre` | `quantPre` |
| 11 | `quantPre` | `reluPre` | `reluEn` |
| 12 | `reluPre` | `channelSplit` | `false` / `isChannelSplit` |
| 13 | `channelSplit` | `nz2ndEn` | `nz2ndEn` |
| 14 | `nz2ndEn` | `0` | `NoConv` |
| 15 | `0` | `0` | `0` |
| ... | ... | ... | ... |
| 尾部 | `..., false, false, false, false, false, false` | `..., false, false, false, false` | `..., false, false, false, false` |

### 3510 → l310/l311 (25→24 参数)

去除 `cacheMode`（第 8 参数）及其后的 `0`（第 9 参数），尾部保持 6 个 `false`。参照 fixpipe `dav_l310/kernel_operator_fixpipe_impl.h:252-256`（cbuf）和 `:322-327`（gm）。

| 参数位置 | 3510 (25 参数) | l310/l311 (24 参数) | fixpipe 参照 |
|---------|---------------|-------------------|-------------|
| 8 | `cacheMode` / `0` | `0` | `0` |
| 9 | `0` | `unitFlag` | `unitFlag` |
| 10 | `unitFlag` | `quantPre` | `quantPre` |
| 11 | `quantPre` | `reluPre` | `reluEn` |
| 12 | `reluPre` | `channelSplit` | `false` / `isChannelSplit` |
| 13 | `channelSplit` | `nz2ndEn` | `nz2ndEn` |
| 14 | `nz2ndEn` | `0` | `NoConv` |
| 15 | `0` | `0` | `0` |
| ... | ... | ... | ... |
| 尾部 | `..., false, false, false, false, false, false` | `..., false, false, false, false, false, false` | `..., false, false, false, false, false, nz2dnEn` |

> fixpipe 末尾参数为 `nz2dnEn`（NZ→DN 使能），DataCopy 不支持此功能，传 `false`。
