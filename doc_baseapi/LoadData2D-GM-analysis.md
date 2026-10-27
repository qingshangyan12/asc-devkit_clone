# 案例分析：LoadData 2D/2DV2/2DV2+Nd2Nz GM→L1 接口跨版本适配

## 接口概述

三个接口均用于 GlobalTensor(OUT) → LocalTensor 的 Cube 数据加载，通过参数类型区分变体：

| 变体 | 行号 | 参数类型 | 模板参数 | Arch 守卫 | pipe 标注 |
|------|------|---------|---------|-----------|-----------|
| 2D GM→L1 | L71 | `LoadData2DParams` | `<T>` | 无 | `__inout_pipe__(MTE2)` |
| 2DV2 GM→L1 | L96 | `LoadData2DParamsV2` | `<T>` | 无 | `__inout_pipe__(MTE2)` |
| 2DV2+Nd2Nz GM→L1 | L111 | `LoadData2DParamsV2` + `Nd2NzParamsV2` | `<T, U>` | L108: `5102\|\|3113\|\|5101\|\|5161\|\|5165\|\|5163` | `__inout_pipe__(MTE2)` |

**关键发现**：
- 两个声明**均无 `__NPU_ARCH__` 守卫**，所有版本均编译
- 版本差异完全下沉到 Impl 层和 Cal 层

## 调用链

```
kernel_operator_mm_intf.h:71/96（声明，无 arch 守卫）
  → kernel_operator_mm_intf_impl.h:61-74 / 124-136（公共包装，无 arch 守卫）
    → LoadDataImpl(dst, src, loadDataParams)
      → kernel_operator_mm_load2d_impl.h:58-81 / 231-266（共享实现，有 arch 守卫）
        → LoadData2DGM2L0ACal / LoadData2DGM2L0BCal / LoadData2DGM2L1Cal  [版本特定]
```

### 公共包装层（intf_impl.h）

| 变体 | 行号 | Arch 守卫 | 调用 |
|------|------|-----------|------|
| 2D GM→L1 | L61-74 | 无 | `LoadDataImpl(dst, src, loadDataParams)` (L73) |
| 2DV2 GM→L1 | L124-136 | 无 | `LoadDataImpl(dst, src, loadDataParams)` (L135) |

### 共享实现层（load2d_impl.h）

#### 2D GM→L1（LoadDataImpl，L58-81）

| 路径 | 行号 | Arch 守卫 | 调用 |
|------|------|-----------|------|
| L0A | L68-69 | 无 | `LoadData2DGM2L0ACal(dst, src, loadDataParams)` |
| L0B | L70-71 | 无 | `LoadData2DGM2L0BCal(dst, src, loadDataParams)` |
| L1 | L72-79 | **L73: `3510`** | 3510: `LoadData2DGM2L1Cal(dst, src, loadDataParams, cacheMode)`；其他: `LoadData2DGM2L1Cal(dst, src, loadDataParams)` |

#### 2DV2 GM→L1（LoadDataImpl，L231-266）

| 路径 | 行号 | Arch 守卫 | 调用 |
|------|------|-----------|------|
| L0A | L243-245 / L258-259 | **L241: `3510\|\|5102`** | 3510/5102: `LoadData2DGM2L0ACal(dst, src, loadDataParams, cacheMode)`；其他: `LoadData2DGM2L0ACal(dst, src, loadDataParams)` |
| L0B | L246-248 / L260-261 | 同上 | 同上模式 |
| L1 | L249-251 / L262-263 | 同上 | 同上模式 |

### 版本隔离机制

`kernel_operator_mm_base_impl.h` 按 arch 条件 include 各版本目录的 `kernel_operator_mm_impl.h`。

## 支持矩阵

### 2D GM→L1（LoadData2DParams）

| 版本 | Arch | L0A | L0B | L1 | cacheMode | 支持类型 |
|------|------|-----|-----|----|-----------|---------|
| dav_3510 | 3510 | ❌桩 | ❌桩 | ✅完整 | ✅ | uint8/int8/uint16/int16/half/bfloat16/float/int32/uint32 |
| dav_l300 | 3003 | ✅完整 | ✅完整 | ✅完整 | ❌ | B8/B16/B32（int8/uint8/half/int16/uint16/float/int32/uint32） |
| dav_l310 | 3103 | ❌桩 | ❌桩 | ❌桩 | ❌ | 无（全桩） |
| dav_l311 | 3113 | ❌桩 | ❌桩 | ❌桩 | ❌ | 无（全桩） |
| dav_5161 | 5101 | ❌桩 | ❌桩 | ❌桩 | ❌ | 无（全桩） |

### 2DV2 GM→L1（LoadData2DParamsV2）

| 版本 | Arch | L0A | L0B | L1 | cacheMode | 支持类型 |
|------|------|-----|-----|----|-----------|---------|
| dav_3510 | 3510 | ❌桩 | ❌桩 | ✅完整 | ✅ | fp4x2/uint8/int8/hifloat8/fp8_e5m2/fp8_e4m3fn/half/bfloat16/float/int32/uint32 |
| dav_l300 | 3003 | ❌桩 | ❌桩 | ❌桩 | ❌ | 无（全桩） |
| dav_l310 | 3103 | ✅完整 | ✅完整 | ✅完整 | ❌ | B8/B16/B32 |
| dav_l311 | 3113 | ✅完整 | ✅完整 | ✅完整 | ❌ | B8/B16/B32 |
| dav_5161 | 5101 | ❌桩 | ✅完整 | ✅完整 | ❌ | B8/B16/B32 |

## 各版本原始代码

---

### dav_3510（参照版本）

#### 2D — LoadData2DGM2L0ACal — 桩函数（L30-36）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L0ACal(
    __ca__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParam, const uint8_t cacheMode = 0)
{
    ASCENDC_ASSERT(
        (false), { KERNEL_LOG(KERNEL_ERROR, "unsupported loaddata_2d from GM to L0A Buffer(A2) on current device"); });
}
```

#### 2D — LoadData2DGM2L0BCal — 桩函数（L38-44）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L0BCal(
    __cb__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParam, const uint8_t cacheMode = 0)
{
    ASCENDC_ASSERT(
        (false), { KERNEL_LOG(KERNEL_ERROR, "unsupported loaddata_2d from GM to L0B Buffer(B2) on current device"); });
}
```

#### 2D — LoadData2DGM2L1Cal — 完整实现（L46-65）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L1Cal(
    __cbuf__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParam, const uint8_t cacheMode = 0)
{
    static_assert(
        SupportType<T, uint8_t, int8_t, uint16_t, int16_t, half, bfloat16_t, float, int32_t, uint32_t>(),
        "LoadData 2dv2 only support uint8_t, int8_t, uint16_t, int16_t, half, bfloat16_t, float, int32_t, uint32_t on "
        "current device!");
    if ASCEND_IS_AIC {
        uint16_t mStartPosition = 0;
        uint16_t kStartPosition = loadDataParam.startIndex;
        uint8_t mStep = loadDataParam.srcStride;
        uint8_t kStep = loadDataParam.repeatTimes;
        int16_t srcStride = static_cast<int16_t>(loadDataParam.srcStride);
        uint16_t dstStride = loadDataParam.dstGap + 1;
        set_mte2_src_para(uint64_t(srcStride));
        load_gm_to_cbuf_2dv2(
            dst, src, mStartPosition, kStartPosition, dstStride, mStep, kStep, loadDataParam.sid, 0, cacheMode);
    }
}
```

#### 2DV2 — LoadData2DGM2L0ACal — 桩函数（L409-416）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L0ACal(
    __ca__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParam, const uint8_t cacheMode = 0)
{
    ASCENDC_ASSERT((false), {
        KERNEL_LOG(KERNEL_ERROR, "unsupported loaddata_2d_v2 from GM to L0A Buffer(A2) on current device");
    });
}
```

#### 2DV2 — LoadData2DGM2L0BCal — 桩函数（L418-425）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L0BCal(
    __cb__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParam, const uint8_t cacheMode = 0)
{
    ASCENDC_ASSERT((false), {
        KERNEL_LOG(KERNEL_ERROR, "unsupported loaddata_2d_v2 from GM to L0B Buffer(B2) on current device");
    });
}
```

#### 2DV2 — LoadData2DGM2L1Cal — 完整实现（L427-449）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L1Cal(
    __cbuf__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParam, const uint8_t cacheMode = 0)
{
    static_assert(
        SupportType<
            T, fp4x2_e2m1_t, fp4x2_e1m2_t, uint8_t, int8_t, hifloat8_t, fp8_e5m2_t, fp8_e4m3fn_t, half, bfloat16_t,
            float, int32_t, uint32_t>(),
        "LoadData 2dv2 only support fp4x2_e2m1_t, fp4x2_e1m2_t, uint8_t, int8_t, hifloat8_t, fp8_e5m2_t, fp8_e4m3fn_t, \
         half, bfloat16_t, float, int32_t, uint32_t on current device!");
    if ASCEND_IS_AIC {
        set_mte2_src_para(uint64_t(loadDataParam.srcStride));
        if constexpr (SupportType<T, fp4x2_e2m1_t, fp4x2_e1m2_t>()) {
            load_gm_to_cbuf_2dv2_s4(
                dst, src, loadDataParam.mStartPosition, loadDataParam.kStartPosition, loadDataParam.dstStride,
                loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid, 0, cacheMode);
        } else {
            load_gm_to_cbuf_2dv2(
                dst, src, loadDataParam.mStartPosition, loadDataParam.kStartPosition, loadDataParam.dstStride,
                loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid, 0, cacheMode);
        }
    }
}
```

---

### dav_l300（目标版本，3003）

#### 2D — LoadData2DGM2L0ACal — 完整实现（L133-151）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L0ACal(__ca__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParam)
{
    if constexpr (B8_BYTE_SIZE == sizeof(T)) {
        load_gm_to_ca(
            (__ca__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, loadDataParam.sid, inc);
    } else if constexpr (B16_BYTE_SIZE == sizeof(T)) {
        load_gm_to_ca(
            (__ca__ half*)dst, (__gm__ half*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, loadDataParam.sid, inc);
    } else if constexpr (B32_BYTE_SIZE == sizeof(T)) {
        load_gm_to_ca(
            (__ca__ int32_t*)dst, (__gm__ int32_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, loadDataParam.sid, inc);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
    }
}
```

#### 2D — LoadData2DGM2L0BCal — 完整实现（L154-172）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L0BCal(__cb__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParam)
{
    if constexpr (B8_BYTE_SIZE == sizeof(T)) {
        load_gm_to_cb(
            (__cb__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, loadDataParam.sid, inc);
    } else if constexpr (B16_BYTE_SIZE == sizeof(T)) {
        load_gm_to_cb(
            (__cb__ half*)dst, (__gm__ half*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, loadDataParam.sid, inc);
    } else if constexpr (B32_BYTE_SIZE == sizeof(T)) {
        load_gm_to_cb(
            (__cb__ int32_t*)dst, (__gm__ int32_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, loadDataParam.sid, inc);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
    }
}
```

#### 2D — LoadData2DGM2L1Cal — 完整实现（L175-193）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L1Cal(__cbuf__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParam)
{
    if constexpr (B8_BYTE_SIZE == sizeof(T)) {
        load_gm_to_cbuf(
            (__cbuf__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, loadDataParam.sid, inc);
    } else if constexpr (B16_BYTE_SIZE == sizeof(T)) {
        load_gm_to_cbuf(
            (__cbuf__ half*)dst, (__gm__ half*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, loadDataParam.sid, inc);
    } else if constexpr (B32_BYTE_SIZE == sizeof(T)) {
        load_gm_to_cbuf(
            (__cbuf__ int32_t*)dst, (__gm__ int32_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, loadDataParam.sid, inc);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
    }
}
```

#### 2DV2 — LoadData2DGM2L0ACal/L0BCal/L1Cal — 桩函数（L294-310）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L0ACal(__ca__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv2 is not supported!"); });
}

template <typename T>
__aicore__ inline void LoadData2DGM2L0BCal(__cb__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv2 is not supported!"); });
}

template <typename T>
__aicore__ inline void LoadData2DGM2L1Cal(__cbuf__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv2 is not supported!"); });
}
```

---

### dav_l310（目标版本，3103）

#### 2DV2 — LoadData2DGM2L0ACal — 完整实现（L129-148）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L0ACal(__ca__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParam)
{
    set_mte2_src_para(uint64_t(loadDataParam.srcStride));
    if constexpr (1 == sizeof(T)) {
        load_gm_to_ca_2dv2(
            (__ca__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
            loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid);
    } else if constexpr (2 == sizeof(T)) {
        load_gm_to_ca_2dv2(
            (__ca__ half*)dst, (__gm__ half*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
            loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid);
    } else if constexpr (4 == sizeof(T)) {
        load_gm_to_ca_2dv2(
            (__ca__ float*)dst, (__gm__ float*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
            loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
    }
}
```

#### 2DV2 — LoadData2DGM2L0BCal — 完整实现（L151-172）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L0BCal(__cb__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParam)
{
    if (loadDataParam.kStep != 1) {
        set_mte2_src_para(uint64_t(loadDataParam.srcStride));
    }
    if constexpr (1 == sizeof(T)) {
        load_gm_to_cb_2dv2(
            (__cb__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
            loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid, 0);
    } else if constexpr (2 == sizeof(T)) {
        load_gm_to_cb_2dv2(
            (__cb__ half*)dst, (__gm__ half*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
            loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid, 0);
    } else if constexpr (4 == sizeof(T)) {
        load_gm_to_cb_2dv2(
            (__cb__ float*)dst, (__gm__ float*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
            loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid, 0);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
    }
}
```

#### 2DV2 — LoadData2DGM2L1Cal — 完整实现（L175-196）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L1Cal(__cbuf__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParam)
{
    if (loadDataParam.kStep != 1) {
        set_mte2_src_para(uint64_t(loadDataParam.srcStride));
    }
    if constexpr (1 == sizeof(T)) {
        load_gm_to_cbuf_2dv2(
            (__cbuf__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
            loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid, 0);
    } else if constexpr (2 == sizeof(T)) {
        load_gm_to_cbuf_2dv2(
            (__cbuf__ half*)dst, (__gm__ half*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
            loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid, 0);
    } else if constexpr (4 == sizeof(T)) {
        load_gm_to_cbuf_2dv2(
            (__cbuf__ float*)dst, (__gm__ float*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
            loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, loadDataParam.sid, 0);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
    }
}
```

#### 2D — LoadData2DGM2L0ACal/L0BCal/L1Cal — 桩函数（L297-313）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L0ACal(__ca__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}

template <typename T>
__aicore__ inline void LoadData2DGM2L0BCal(__cb__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}

template <typename T>
__aicore__ inline void LoadData2DGM2L1Cal(__cbuf__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}
```

---

### dav_l311（目标版本，3113）

> **2DV2 代码与 dav_l310 完全一致**（L126-193），**2D 全桩与 dav_l310 一致**（L322-338，适配后因 Nd2NzParamsV2 函数插入下移）。

---

### dav_5161（目标版本，5101）

#### 2D — LoadData2DGM2L0ACal/L0BCal/L1Cal — 桩函数（L127-143）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L0ACal(__ca__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}

template <typename T>
__aicore__ inline void LoadData2DGM2L0BCal(__cb__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}

template <typename T>
__aicore__ inline void LoadData2DGM2L1Cal(__cbuf__ T* dst, __gm__ T* src, const LoadData2DParams& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData 2dv1 is not supported!"); });
}
```

#### 2DV2 — LoadData2DGM2L0ACal — 桩函数（L177-181）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L0ACal(__ca__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData GM To A2 is not supported!"); });
}
```

#### 2DV2 — LoadData2DGM2L0BCal — 完整实现（L183-204）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L0BCal(__cb__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParams)
{
    if (loadDataParams.kStep != 1) {
        set_mte2_src_para(uint64_t(loadDataParams.srcStride));
    }
    if constexpr (1 == sizeof(T)) {
        load_gm_to_cb_2dv2(
            (__cb__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParams.mStartPosition, loadDataParams.kStartPosition,
            loadDataParams.dstStride, loadDataParams.mStep, loadDataParams.kStep, loadDataParams.sid);
    } else if constexpr (2 == sizeof(T)) {
        load_gm_to_cb_2dv2(
            (__cb__ half*)dst, (__gm__ half*)src, loadDataParams.mStartPosition, loadDataParams.kStartPosition,
            loadDataParams.dstStride, loadDataParams.mStep, loadDataParams.kStep, loadDataParams.sid);
    } else if constexpr (4 == sizeof(T)) {
        load_gm_to_cb_2dv2(
            (__cb__ float*)dst, (__gm__ float*)src, loadDataParams.mStartPosition, loadDataParams.kStartPosition,
            loadDataParams.dstStride, loadDataParams.mStep, loadDataParams.kStep, loadDataParams.sid);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
    }
}
```

#### 2DV2 — LoadData2DGM2L1Cal — 完整实现（L206-227）

```cpp
template <typename T>
__aicore__ inline void LoadData2DGM2L1Cal(__cbuf__ T* dst, __gm__ T* src, const LoadData2DParamsV2& loadDataParams)
{
    if (loadDataParams.kStep != 1) {
        set_mte2_src_para(uint64_t(loadDataParams.srcStride));
    }
    if constexpr (1 == sizeof(T)) {
        load_gm_to_cbuf_2dv2(
            (__cbuf__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParams.mStartPosition, loadDataParams.kStartPosition,
            loadDataParams.dstStride, loadDataParams.mStep, loadDataParams.kStep, loadDataParams.sid);
    } else if constexpr (2 == sizeof(T)) {
        load_gm_to_cbuf_2dv2(
            (__cbuf__ half*)dst, (__gm__ half*)src, loadDataParams.mStartPosition, loadDataParams.kStartPosition,
            loadDataParams.dstStride, loadDataParams.mStep, loadDataParams.kStep, loadDataParams.sid);
    } else if constexpr (4 == sizeof(T)) {
        load_gm_to_cbuf_2dv2(
            (__cbuf__ float*)dst, (__gm__ float*)src, loadDataParams.mStartPosition, loadDataParams.kStartPosition,
            loadDataParams.dstStride, loadDataParams.mStep, loadDataParams.kStep, loadDataParams.sid);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
    }
}
```

## 代码级实现差异分析

### 实现风格对比

| 特征 | dav_3510 | dav_l300 | dav_l310 | dav_l311 | dav_5161 |
|------|----------|----------|----------|----------|----------|
| 2D L0A | ❌桩 | ✅完整 `load_gm_to_ca` | ❌桩 | ❌桩 | ❌桩 |
| 2D L0B | ❌桩 | ✅完整 `load_gm_to_cb` | ❌桩 | ❌桩 | ❌桩 |
| 2D L1 | ✅完整 `load_gm_to_cbuf_2dv2` | ✅完整 `load_gm_to_cbuf` | ❌桩 | ❌桩 | ❌桩 |
| 2DV2 L0A | ❌桩 | ❌桩 | ✅完整 `load_gm_to_ca_2dv2` | ✅完整 | ❌桩 |
| 2DV2 L0B | ❌桩 | ❌桩 | ✅完整 `load_gm_to_cb_2dv2` | ✅完整 | ✅完整 `load_gm_to_cb_2dv2` |
| 2DV2 L1 | ✅完整 `load_gm_to_cbuf_2dv2` | ❌桩 | ✅完整 `load_gm_to_cbuf_2dv2` | ✅完整 | ✅完整 `load_gm_to_cbuf_2dv2` |
| cacheMode | ✅（默认=0） | ❌ | ❌ | ❌ | ❌ |
| AIC 守卫 | ✅（L1路径） | ❌ | ❌ | ❌ | ❌ |
| 类型分派 | `static_assert` + `SupportType` | `sizeof(T)` if constexpr | `sizeof(T)` if constexpr | 同 l310 | `sizeof(T)` if constexpr |
| 参数名 | `loadDataParam` | `loadDataParam` | `loadDataParam` | `loadDataParam` | `loadDataParams`(复数) |
| ISA 尾参 `0` | 有（L1路径） | 有 `inc` | 有（L0B/L1） | 同 l310 | 无 |

### 实现模式总结

**模式1：dav_3510 — 2D L1 + 2DV2 L1 完整，L0A/L0B 桩**
- 2D L1 用 `load_gm_to_cbuf_2dv2`（将 2D 参数映射为 2DV2 参数）
- 2DV2 L1 用 `load_gm_to_cbuf_2dv2` + fp4x2 分支用 `load_gm_to_cbuf_2dv2_s4`
- 支持 cacheMode，有 AIC 守卫
- 类型用 `static_assert` + `SupportType` 检查

**模式2：dav_l300 — 2D 全完整，2DV2 全桩**
- 2D 用旧式 ISA `load_gm_to_ca`/`load_gm_to_cb`/`load_gm_to_cbuf`（直接传 startIndex/repeatTimes 等）
- 2DV2 不支持（硬件无 2DV2 ISA）
- 类型用 `sizeof(T)` if constexpr，传 `inc` 尾参

**模式3：dav_l310/dav_l311 — 2DV2 全完整，2D 全桩**
- 2DV2 用新式 ISA `load_gm_to_ca_2dv2`/`load_gm_to_cb_2dv2`/`load_gm_to_cbuf_2dv2`
- 2D 不支持（硬件无旧式 2D ISA）
- 类型用 `sizeof(T)` if constexpr，L0B/L1 有 `kStep != 1` 条件设置 `set_mte2_src_para`

**模式4：dav_5161 — 2D 全桩，2DV2 L0B+L1 完整，L0A 桩**
- 2DV2 L0A 不支持（GM→L0A 不支持）
- 2DV2 L0B/L1 与 l310/l311 类似但无尾参 `0`，参数名用复数
- 2D 全桩

### 关键差异点

1. **2D vs 2DV2 互补支持**
   - 3510：2D L1 + 2DV2 L1（仅 L1 路径）
   - l300：2D 全路径（L0A+L0B+L1）
   - l310/l311：2DV2 全路径（L0A+L0B+L1）
   - 5161：2DV2 L0B+L1（无 L0A）

2. **ISA 函数族差异**
   - 3510：`load_gm_to_cbuf_2dv2` / `load_gm_to_cbuf_2dv2_s4`（统一用 2DV2 ISA）
   - l300：`load_gm_to_ca` / `load_gm_to_cb` / `load_gm_to_cbuf`（旧式 2D ISA）
   - l310/l311：`load_gm_to_ca_2dv2` / `load_gm_to_cb_2dv2` / `load_gm_to_cbuf_2dv2`（新式 2DV2 ISA）
   - 5161：同 l310/l311 但 ISA 参数列表不同（无尾参 `0`）

3. **cacheMode 仅 3510 支持** — 共享层 L73/L241 的 arch 守卫控制

4. **参数名差异** — 5161 用 `loadDataParams`（复数），其他用 `loadDataParam`

5. **3510 2D L1 用 2DV2 ISA** — 将 2D 参数映射为 2DV2 参数后调用 `load_gm_to_cbuf_2dv2`

6. **l310/l311 L0B/L1 有 `kStep != 1` 条件** — 仅当 kStep≠1 时设置 `set_mte2_src_para`，L0A 无条件设置

7. **5161 2DV2 L0A 桩但 l310/l311 完整** — 5101 硬件不支持 GM→L0A 的 2DV2

## L111（2DV2+Nd2Nz GM→L1）接口分析

### 接口说明

`LoadData(LocalTensor<T>& dst, GlobalTensor<U>& src, LoadData2DParamsV2& loadDataParams, Nd2NzParamsV2& nd2nzParams)` 用于 GM→L1 的 2DV2 数据加载 + ND→NZ 格式转换 + 类型转换（低精度解压）。双模板参数 `<T, U>` 支持 dst 和 src 不同类型。

**与 L96 的区别**：
- L96：`<T>` 单模板，src 为 `GlobalTensor<T>`，无 Nd2NzParamsV2
- L111：`<T, U>` 双模板，src 为 `GlobalTensor<U>`，多 `Nd2NzParamsV2` 参数，支持低精度解压（int4b→int8、int2b→int4b/int8、uint1b→int4b）

### 调用链

```
kernel_operator_mm_intf.h:108-115（声明，arch 守卫 L108: 5102||3113||5101||5161||5165||5163，适配后扩展）
  → kernel_operator_mm_intf_impl.h:138-151（公共包装，arch 守卫 L138: 同上，适配后扩展）
    → LoadDataImpl(dst, src, loadDataParams, nd2nzParams)
      → dav_m510/kernel_operator_mm_impl.h: LoadData2DGM2L1Cal(dst, src, loadDataParam, nd2nzParams)  [仅 L1 路径]
        → set_mte2_qtable0/qtable1 + load_gm_to_cbuf_2dv2  [硬件指令]
```

**注意**：`LoadDataImpl` 带 `Nd2NzParamsV2` 的重载不在共享文件 `load2d_impl.h` 中，直接在包装层调用后由版本特定文件提供。仅 5102（dav_m510）定义了此函数。

### 支持矩阵

| 版本 | Arch | 声明守卫 | 包装函数 | LoadDataImpl(Nd2NzParamsV2) | LoadData2DGM2L1Cal(Nd2NzParamsV2) | L0A/L0B | 状态 |
|------|------|---------|---------|---------------------------|----------------------------------|---------|------|
| dav_3510 | 3510 | ❌不包含 | ❌不编译 | 未定义 | 桩函数 | 未定义 | ❌ 接口存在但桩 |
| dav_l300 | 3003 | ❌不包含 | ❌不编译 | 未定义 | 桩函数 | 未定义 | ❌ 接口存在但桩 |
| dav_l310 | 3103 | ❌不包含 | ❌不编译 | 未定义 | 桩函数 | 未定义 | ❌ 接口存在但桩 |
| dav_l311 | 3113 | ✅包含 | ✅编译 | 完整 | **完整**（L195-221，从dav_m510移植） | 未定义(仅L1路径) | ✅ 完整 |
| dav_5161 | 5101 | ✅包含 | ✅编译 | 完整 | **完整**（L229-255，从dav_m510移植） | 未定义(仅L1路径) | ✅ 完整 |
| dav_m510 | 5102 | ✅包含 | ✅编译 | 完整 | **完整**（L211-237） | 未定义(仅L1路径) | ✅ 完整（参照） |

### dav_m510 原始代码（参照版本）

#### LoadData2DGM2L1Cal(Nd2NzParamsV2) — 完整实现（L211-237）

```cpp
template <typename T, typename U>
__aicore__ inline void LoadData2DGM2L1Cal(
    __cbuf__ T* dst, __gm__ U* src, const LoadData2DParamsV2& loadDataParam, const Nd2NzParamsV2& nd2nzParams)
{
    static_assert(
        ((IsSameType<T, int8_t>::value && IsSameType<U, int4b_t>::value) ||
         (IsSameType<T, int4b_t>::value && IsSameType<U, int2b_t>::value) ||
         (IsSameType<T, int8_t>::value && IsSameType<U, int2b_t>::value) ||
         (IsSameType<T, int4b_t>::value && IsSameType<U, uint1b_t>::value)),
        "LoadData dst type or src type is not supported on current device!");
    set_mte2_qtable0(nd2nzParams.lookupTable0);
    set_mte2_src_para(uint64_t(loadDataParam.srcStride));
    uint8_t decompressMode = 0;
    if constexpr (IsSameType<T, int4b_t>::value && IsSameType<U, int2b_t>::value) {
        decompressMode = 0b0111;
    } else if constexpr (IsSameType<T, int8_t>::value && IsSameType<U, int2b_t>::value) {
        decompressMode = 0b0001;
    } else if constexpr (IsSameType<T, int4b_t>::value && IsSameType<U, uint1b_t>::value) {
        decompressMode = 0b0110;
    } else if constexpr (IsSameType<T, int8_t>::value && IsSameType<U, int4b_t>::value) {
        set_mte2_qtable1(nd2nzParams.lookupTable1);
        decompressMode = 0b0011;
    }
    load_gm_to_cbuf_2dv2(
        (__cbuf__ int8_t*)dst, (__gm__ int8_t*)src, loadDataParam.mStartPosition, loadDataParam.kStartPosition,
        loadDataParam.dstStride, loadDataParam.mStep, loadDataParam.kStep, 0, decompressMode, 0);
}
```

**支持的类型组合**：

| dst(T) | src(U) | decompressMode | 额外操作 |
|--------|--------|---------------|---------|
| int8_t | int4b_t | 0b0011 | `set_mte2_qtable1` |
| int4b_t | int2b_t | 0b0111 | — |
| int8_t | int2b_t | 0b0001 | — |
| int4b_t | uint1b_t | 0b0110 | — |

**硬件指令**：`set_mte2_qtable0` + `set_mte2_qtable1`（仅 int8←int4）+ `set_mte2_src_para` + `load_gm_to_cbuf_2dv2`

### 实现差异分析

**关键特征**：
1. **仅 L1 路径** — 无 L0A/L0B 路径（与 L71/L96 不同，后者有 L0A/L0B/L1 三路径）
2. **双模板 `<T, U>`** — 唯一支持 dst/src 不同类型的 LoadData 2D 接口
3. **低精度解压** — 通过 `decompressMode` 和 `lookupTable` 实现 int4b/int2b/uint1b → int8/int4b 的硬件解压
4. **5102 专用 ISA** — 使用 `set_mte2_qtable0`/`set_mte2_qtable1` 寄存器，其他版本可能不支持

## 待确认问题

1. **dav_l300 2DV2 全桩** — 3003 不支持 2DV2，是硬件不支持还是待补全？（疑似硬件不支持，3003 仅有旧式 2D ISA）

2. **dav_l310/l311 2D 全桩** — 3103/3113 不支持旧式 2D，是硬件不支持还是待补全？（疑似硬件不支持，3113+ 仅有新式 2DV2 ISA）

3. **dav_5161 2D 全桩 + 2DV2 L0A 桩** — 5101 不支持 2D 和 GM→L0A 2DV2，是硬件不支持还是待补全？

4. **dav_3510 2D L0A/L0B 桩** — 3510 不支持 GM→L0A/L0B 的 2D，是硬件不支持还是待补全？

5. **dav_3510 2D L1 用 2DV2 ISA** — 3510 将 2D 参数映射为 2DV2 后调用 `load_gm_to_cbuf_2dv2`，其他版本的 2D L1 是否也应该这样？还是 l300 的旧式 `load_gm_to_cbuf` 才是正确的 2D L1 实现？

6. **dav_5161 参数名差异** — `loadDataParams` vs `loadDataParam`，是否需要统一？

7. **dav_5161 2DV2 ISA 无尾参 `0`** — l310/l311 的 `load_gm_to_cb_2dv2` 有尾参 `0`，5161 没有。是 ISA 签名差异还是遗漏？

8. **dav_l300 2D ISA 尾参 `inc`** — l300 传 `inc` 尾参，其他版本无此参数。`inc` 是什么含义？

9. **L111 接口适配后状态** — 适配后 3113(dav_l311) 和 5101(dav_5161) 已包含声明守卫并从 dav_m510 移植完整实现；3510(dav_3510)/3003(dav_l300)/3103(dav_l310) 仍排除。详见 LoadData2D-GM-report.md。

10. **L109 的 LoadDataImpl 位置** — `LoadDataImpl` 带 `Nd2NzParamsV2` 的重载不在共享文件 `load2d_impl.h` 中，直接在包装层调用后由版本特定文件提供。这是设计如此还是应在共享层添加分发逻辑？

11. **L109 仅 L1 路径** — dav_m510 仅定义了 L1 路径的 `LoadData2DGM2L1Cal`，无 L0A/L0B 路径。是硬件不支持 GM→L0A/L0B 的低精度解压，还是待补全？

12. **L109 的 `set_mte2_qtable0`/`set_mte2_qtable1` 硬件支持** — 这两个寄存器设置指令是否为 5102 专用？其他版本硬件是否支持？
