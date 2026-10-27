# MmadMxFix 接口适配报告

## 适配后支持矩阵

| 版本 | Arch | V1 (BitMode,无bias) | V2 (BitMode,带bias) | V3 (Params,无bias) | V4 (Params,带bias) | 变更说明 |
|------|------|---------------------|---------------------|---------------------|---------------------|---------|
| dav_l300 | 3003 | N/A | N/A | N/A | N/A | 维持 |
| dav_l310 | 3103 | N/A | N/A | N/A | N/A | 维持 |
| dav_l311 | 3113 | N/A | N/A | N/A | N/A | 维持 |
| dav_5161 | 5101 | N/A → **N/A** | N/A → **N/A** | N/A → **N/A** | N/A → **N/A** | 5101 不支持 matmul_mx_to_cbuf，排除声明 |
| dav_5161 | 5161 | 维持 完整 | 维持 完整 | 维持 完整 | 维持 完整 | 5161 保留 |
| dav_5161 | 5165 | 维持 完整 | 维持 完整 | 维持 完整 | 维持 完整 | 5165 保留 |
| dav_5161 | 5163 | 维持 完整 | 维持 完整 | 维持 完整 | 维持 完整 | 5163 保留 |
| dav_3510 | 3510 | N/A | N/A | N/A | N/A | 维持 |

> 5101 通过排除式 arch 守卫 `#if __NPU_ARCH__ == 5161 || 5165 || 5163` 不编译 MmadMxFix 相关声明和实现。5161/5165/5163 保持原有完整实现不变。

## 修改的文件清单

| 文件 | 修改内容 |
|------|---------|
| `include/basic_api/kernel_operator_macro_intf.h:231` | MmadMxFix 4 个声明用 `#if 5161\|\|5165\|\|5163` 包裹（排除 5101） |
| `impl/basic_api/kernel_operator_macro_intf_impl.h:181` | MmadMxFix 4 个包装用 `#if 5161\|\|5165\|\|5163` 包裹 |
| `impl/basic_api/kernel_operator_macro_base_impl.h:204` | MmadMxFixImpl 4 个实现用 `#if 5161\|\|5165\|\|5163` 包裹 |
| `impl/basic_api/dav_5161/kernel_operator_macro_impl.h:573` | MmadMxFixCal 4 个函数用 `#if 5161\|\|5165\|\|5163` 包裹 |

## 适配后各版本原始代码

### intf.h — MmadMxFix 声明（L231-261，修改：新增 `#if` 守卫）

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
/*
 * @ingroup MacroCube
 * @brief performs normal matmul
 * @param [out] dst
 * @param [in] left
 * @param [in] right
 * @param [in] bias
 * @param [in] mmadFixParams
 */
template <typename T, typename U, typename S>
__aicore__ inline void MmadMxFix(
    const LocalTensor<T>& dst, const LocalTensor<U>& left, const LocalTensor<S>& right,
    const MmadFixBitModeParams& mmadFixParams);

template <typename T, typename U, typename S, typename V>
__aicore__ inline void MmadMxFix(
    const LocalTensor<T>& dst, const LocalTensor<U>& left, const LocalTensor<S>& right, const LocalTensor<V>& bias,
    const MmadFixBitModeParams& mmadFixParams);

template <typename T, typename U, typename S>
__aicore__ inline void MmadMxFix(
    const LocalTensor<T>& dst, const LocalTensor<U>& left, const LocalTensor<S>& right,
    const MmadFixParams& mmadFixParams);

template <typename T, typename U, typename S, typename V>
__aicore__ inline void MmadMxFix(
    const LocalTensor<T>& dst, const LocalTensor<U>& left, const LocalTensor<S>& right, const LocalTensor<V>& bias,
    const MmadFixParams& mmadFixParams);
#endif
```

> **修改：** 新增 `#if 5161||5165||5163` 守卫（排除 5101）。

### intf_impl.h — MmadMxFix 包装（L181-213，修改：新增 `#if` 守卫）

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
template <typename T, typename U, typename S>
__aicore__ inline void MmadMxFix(
    const LocalTensor<T>& dst, const LocalTensor<U>& left, const LocalTensor<S>& right,
    const MmadFixBitModeParams& mmadFixParams)
{
    MmadMxFixImpl(dst, left, right, mmadFixParams);
}

template <typename T, typename U, typename S, typename V>
__aicore__ inline void MmadMxFix(
    const LocalTensor<T>& dst, const LocalTensor<U>& left, const LocalTensor<S>& right, const LocalTensor<V>& bias,
    const MmadFixBitModeParams& mmadFixParams)
{
    MmadMxFixImpl(dst, left, right, bias, mmadFixParams);
}

template <typename T, typename U, typename S>
__aicore__ inline void MmadMxFix(
    const LocalTensor<T>& dst, const LocalTensor<U>& left, const LocalTensor<S>& right,
    const MmadFixParams& mmadFixParams)
{
    MmadMxFixImpl(dst, left, right, mmadFixParams);
}

template <typename T, typename U, typename S, typename V>
__aicore__ inline void MmadMxFix(
    const LocalTensor<T>& dst, const LocalTensor<U>& left, const LocalTensor<S>& right, const LocalTensor<V>& bias,
    const MmadFixParams& mmadFixParams)
{
    MmadMxFixImpl(dst, left, right, bias, mmadFixParams);
}
#endif
```

> **修改：** 新增 `#if 5161||5165||5163` 守卫（排除 5101）。

### base_impl.h — MmadMxFixImpl 实现（L204-244，修改：新增 `#if` 守卫）

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
template <typename T, typename U, typename S>
__aicore__ inline void MmadMxFixImpl(
    const LocalTensor<T>& dst, const LocalTensor<U>& left, const LocalTensor<S>& right,
    const MmadFixBitModeParams& mmadFixParams)
{
    MmadMxFixCal(
        (__cbuf__ PrimT<T>*)dst.GetPhyAddr(), (__cbuf__ PrimT<U>*)left.GetPhyAddr(),
        (__cb__ PrimT<S>*)right.GetPhyAddr(), mmadFixParams);
}

template <typename T, typename U, typename S>
__aicore__ inline void MmadMxFixImpl(
    const LocalTensor<T>& dst, const LocalTensor<U>& left, const LocalTensor<S>& right,
    const MmadFixParams& mmadFixParams)
{
    MmadMxFixCal(
        (__cbuf__ PrimT<T>*)dst.GetPhyAddr(), (__cbuf__ PrimT<U>*)left.GetPhyAddr(),
        (__cb__ PrimT<S>*)right.GetPhyAddr(), mmadFixParams);
}

template <typename T, typename U, typename S, typename V>
__aicore__ inline void MmadMxFixImpl(
    const LocalTensor<T>& dst, const LocalTensor<U>& left, const LocalTensor<S>& right, const LocalTensor<V>& bias,
    const MmadFixBitModeParams& mmadFixParams)
{
    MmadMxFixCal(
        (__cbuf__ PrimT<T>*)dst.GetPhyAddr(), (__cbuf__ PrimT<U>*)left.GetPhyAddr(),
        (__cb__ PrimT<S>*)right.GetPhyAddr(), (uint64_t)bias.GetPhyAddr(), mmadFixParams);
}

template <typename T, typename U, typename S, typename V>
__aicore__ inline void MmadMxFixImpl(
    const LocalTensor<T>& dst, const LocalTensor<U>& left, const LocalTensor<S>& right, const LocalTensor<V>& bias,
    const MmadFixParams& mmadFixParams)
{
    MmadMxFixCal(
        (__cbuf__ PrimT<T>*)dst.GetPhyAddr(), (__cbuf__ PrimT<U>*)left.GetPhyAddr(),
        (__cb__ PrimT<S>*)right.GetPhyAddr(), (uint64_t)bias.GetPhyAddr(), mmadFixParams);
}
#endif
```

> **修改：** 新增 `#if 5161||5165||5163` 守卫（排除 5101）。

### dav_5161/kernel_operator_macro_impl.h — MmadMxFixCal 实现（L573-700，修改：新增 `#if` 守卫）

```cpp
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadMxFixCal(
    __cbuf__ DstT* dst, __cbuf__ Src0T* left, __cb__ Src1T* right, const MmadFixBitModeParams& mmadFixParams)
{
    matmul_mx_to_cbuf(dst, left, right, (__cbuf__ int32_t*)get_imm(0), (uint64_t)0, mmadFixParams.GetConfig0());
}

template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadMxFixCal(
    __cbuf__ DstT* dst, __cbuf__ Src0T* left, __cb__ Src1T* right, const MmadFixParams& mmadFixParams)
{
    // ... 21 个 POS_ 常量定义 + config0 位拼接 ...
    matmul_mx_to_cbuf(dst, left, right, (__cbuf__ int32_t*)get_imm(0), (uint64_t)0, config0);
}

template <typename DstT, typename Src0T, typename Src1T, typename BiasT>
__aicore__ inline void MmadMxFixCal(
    __cbuf__ DstT* dst, __cbuf__ Src0T* left, __cb__ Src1T* right, uint64_t bias,
    const MmadFixBitModeParams& mmadFixParams)
{
    if (mmadFixParams.GetBiasMatrixBroadcast()) {
        matmul_mx_to_cbuf(dst, left, right, (__cbuf__ int32_t*)get_imm(0), bias, mmadFixParams.GetConfig0());
    } else {
        matmul_mx_to_cbuf(dst, left, right, (__cbuf__ int32_t*)bias, (uint64_t)0, mmadFixParams.GetConfig0());
    }
}

template <typename DstT, typename Src0T, typename Src1T, typename BiasT>
__aicore__ inline void MmadMxFixCal(
    __cbuf__ DstT* dst, __cbuf__ Src0T* left, __cb__ Src1T* right, uint64_t bias, const MmadFixParams& mmadFixParams)
{
    // ... 21 个 POS_ 常量定义 + config0 位拼接 ...
    if (mmadFixParams.biasMatrixBroadcast) {
        matmul_mx_to_cbuf(dst, left, right, (__cbuf__ int32_t*)get_imm(0), bias, config0);
    } else {
        matmul_mx_to_cbuf(dst, left, right, (__cbuf__ int32_t*)bias, (uint64_t)0, config0);
    }
}
#endif
```

> **修改：** 新增 `#if 5161||5165||5163` 守卫（排除 5101）。函数体内容不变。
