# 适配报告：DataCopy(GM→L1 Dn2Nz) 接口

## 适配后支持矩阵

| 版本 | Arch | 声明守卫 | DataCopyGM2L1DN2NZImpl (L1路径) | 适配前 → 适配后 |
|------|------|---------|-------------------------------|----------------|
| dav_3510 | 3510 | ✅ 包含 | 完整实现（L416-433） | 维持 |
| dav_l300 | 3003 | ❌ 不包含 | **桩函数**（L284-300） | 未定义 → **桩函数** |
| dav_l310 | 3103 | ❌ 不包含 | **桩函数**（L309-325） | 未定义 → **桩函数** |
| dav_l311 | 3113 | ❌ 不包含 | **桩函数**（L360-376） | 未定义 → **桩函数** |
| dav_5161 | 5101 | ✅ 包含 | 完整实现（L379-396） | 维持 |

## 修改的文件清单

| 文件 | 修改内容 |
|------|---------|
| `dav_l300/kernel_operator_data_copy_impl.h` | 新增 `DataCopyGM2L1DN2NZImplBase` + `DataCopyGM2L1DN2NZImpl` 桩函数（L284-300） |
| `dav_l310/kernel_operator_data_copy_impl.h` | 新增 `DataCopyGM2L1DN2NZImplBase` + `DataCopyGM2L1DN2NZImpl` 桩函数（L309-325） |
| `dav_l311/kernel_operator_data_copy_impl.h` | 新增 `DataCopyGM2L1DN2NZImplBase` + `DataCopyGM2L1DN2NZImpl` 桩函数（L360-376） |

## 适配后各版本原始代码

### dav_3510（参照版本，维持不变）

#### DataCopyGM2L1DN2NZImplBase — 完整实现（L381-414，未修改）

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

        uint16_t loop2DstStride = intriParams.dstNzNStride;
        uint16_t loop3DstStride = intriParams.dstNzC0Stride;
        uint16_t loop4DstStride = static_cast<uint16_t>(intriParams.dstNzMatrixStride * sizeof(T) / ONE_BLK_SIZE);

        uint64_t mte2NzPara = static_cast<uint64_t>(loop4DstStride) << 48;
        mte2NzPara |= static_cast<uint64_t>(loop3DstStride) << 32;
        mte2NzPara |= static_cast<uint64_t>(loop2DstStride) << 16;
        mte2NzPara |= static_cast<uint64_t>(intriParams.dnNum);
        set_mte2_nz_para(mte2NzPara);

        uint64_t loop1SrcStride = intriParams.srcDValue * sizeof(T);
        uint64_t loop4SrcStride = intriParams.srcDnMatrixStride * sizeof(T);

        copy_gm_to_cbuf_multi_dn2nz(
            (__cbuf__ T*)dst, (__gm__ T*)src, 0, loop1SrcStride, cacheMode, intriParams.nValue, intriParams.dValue,
            loop4SrcStride, enableSmallC0);
    }
}
```

#### DataCopyGM2L1DN2NZImpl — 完整实现（L416-433，未修改）

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
        DataCopyGM2L1DN2NZImplBase((__cbuf__ uint8_t*)dst, (__gm__ uint8_t*)src, intriParams, enableSmallC0, cacheMode);
    } else {
        DataCopyGM2L1DN2NZImplBase((__cbuf__ T*)dst, (__gm__ T*)src, intriParams, enableSmallC0, cacheMode);
    }
}
```

---

### dav_l300（目标版本，3003，**已修改**）

#### DataCopyGM2L1DN2NZImplBase — 桩函数（L284-291，**新增**）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1DN2NZImplBase(
    __cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams, bool enableSmallC0, const uint8_t cacheMode = 0)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from gm to l1 dn2nz on this version");
    });
}
```

#### DataCopyGM2L1DN2NZImpl — 桩函数（L293-300，**新增**）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1DN2NZImpl(
    __cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams, bool enableSmallC0, const uint8_t cacheMode = 0)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from gm to l1 dn2nz on this version");
    });
}
```

---

### dav_l310（目标版本，3103，**已修改**）

#### DataCopyGM2L1DN2NZImplBase — 桩函数（L309-316，**新增**）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1DN2NZImplBase(
    __cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams, bool enableSmallC0, const uint8_t cacheMode = 0)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from gm to l1 dn2nz on this version");
    });
}
```

#### DataCopyGM2L1DN2NZImpl — 桩函数（L318-325，**新增**）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1DN2NZImpl(
    __cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams, bool enableSmallC0, const uint8_t cacheMode = 0)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from gm to l1 dn2nz on this version");
    });
}
```

---

### dav_l311（目标版本，3113，**已修改**）

#### DataCopyGM2L1DN2NZImplBase — 桩函数（L360-367，**新增**）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1DN2NZImplBase(
    __cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams, bool enableSmallC0, const uint8_t cacheMode = 0)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from gm to l1 dn2nz on this version");
    });
}
```

#### DataCopyGM2L1DN2NZImpl — 桩函数（L369-376，**新增**）

```cpp
template <typename T>
__aicore__ inline void DataCopyGM2L1DN2NZImpl(
    __cbuf__ T* dst, __gm__ T* src, const Dn2NzParams& intriParams, bool enableSmallC0, const uint8_t cacheMode = 0)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(KERNEL_ERROR, "unsupported data copy from gm to l1 dn2nz on this version");
    });
}
```

---

### dav_5161（目标版本，5101，维持不变）

#### DataCopyGM2L1DN2NZImplBase — 完整实现（L346-377，未修改）

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

    uint16_t loop2DstStride = intriParams.dstNzNStride;
    uint16_t loop3DstStride = intriParams.dstNzC0Stride;
    uint16_t loop4DstStride = static_cast<uint16_t>(intriParams.dstNzMatrixStride * sizeof(T) / ONE_BLK_SIZE);

    uint64_t mte2NzPara = static_cast<uint64_t>(loop4DstStride) << 48;
    mte2NzPara |= static_cast<uint64_t>(loop3DstStride) << 32;
    mte2NzPara |= static_cast<uint64_t>(loop2DstStride) << 16;
    mte2NzPara |= static_cast<uint64_t>(intriParams.dnNum);
    set_mte2_nz_para(mte2NzPara);

    uint64_t loop1SrcStride = intriParams.srcDValue * sizeof(T);
    uint64_t loop4SrcStride = intriParams.srcDnMatrixStride * sizeof(T);

    copy_gm_to_cbuf_multi_dn2nz(
        (__cbuf__ T*)dst, (__gm__ T*)src, intriParams.sid, loop1SrcStride, cacheMode, intriParams.nValue,
        intriParams.dValue, loop4SrcStride, enableSmallC0);
}
```

#### DataCopyGM2L1DN2NZImpl — 完整实现（L379-396，未修改）

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
        DataCopyGM2L1DN2NZImplBase((__cbuf__ uint8_t*)dst, (__gm__ uint8_t*)src, intriParams, enableSmallC0, cacheMode);
    } else {
        DataCopyGM2L1DN2NZImplBase((__cbuf__ T*)dst, (__gm__ T*)src, intriParams, enableSmallC0, cacheMode);
    }
}
```
