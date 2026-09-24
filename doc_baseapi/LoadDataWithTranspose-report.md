# LoadDataWithTranspose 适配报告

## 适配后支持矩阵

| 版本 | Arch | L0A V1 | L0A V2 | L0B V1 | L0B V2 |
|------|------|--------|--------|--------|--------|
| dav_l300 | 3003 | 完整（维持） | 未定义 → 桩（**新增**） | 完整（维持） | 桩（维持） |
| dav_l310 | 3103 | 桩（维持） | 未定义 → 桩（**新增**） | 桩（维持） | 桩 → **完整**（**替换**，从 dav_3510 移植） |
| dav_l311 | 3113 | 桩（维持） | 未定义 → 桩（**新增**） | 桩（维持） | 桩 → 桩（维持，硬件不支持） |
| dav_5161 | 5101 | 桩（维持） | 未定义 → 桩（**新增**） | 桩（维持，**修正错误消息** "A1 To A2" → "B1 To B2"） | 未定义 → 桩（**新增**，硬件不支持） |
| dav_3510 | 3510 | 桩（参照版本，不动） | 未定义（不动） | 完整·委托V2（不动） | 完整（不动） |

## 修改的文件

| 文件 | 修改内容 |
|------|---------|
| `dav_l300/kernel_operator_mm_impl.h` | 新增 L0A V2 桩函数（L51-60） |
| `dav_l310/kernel_operator_mm_impl.h` | 新增 L0A V2 桩函数（L42-51）+ L0B V2 替换为完整实现（L82-95） |
| `dav_l311/kernel_operator_mm_impl.h` | 新增 L0A V2 桩函数（L42-51）+ L0B V2 维持桩函数 |
| `dav_5161/kernel_operator_mm_impl.h` | 新增 L0A V2 桩函数（L268-273）+ 修正 L0B V1 错误消息（L280）+ 新增 L0B V2 桩函数（L283-288） |

## 适配后各版本原始代码

### dav_3510（参照版本，未修改）

#### L0A V1 — 桩函数（L460-469）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ATransposeCal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParam)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(
            KERNEL_ERROR,
            "LoadDataWithTranspose from L1 Buffer(A1) to L0A Buffer(A2) is not supported on current device");
    });
}
```

#### L0B V1 — 完整实现，委托 V2（L273-291）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0BTransposeCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParam)
{
    static_assert(
        SupportType<T, uint8_t, int8_t, half, bfloat16_t, float, int32_t, uint32_t>(),
        "LoadDataWithTranspose only support uint8_t, int8_t, half, bfloat16_t, float, int32_t, uint32_t \
         on current device!");
    uint32_t unitMultiples = 1;
    if (IsSameType<T, int8_t>::value || IsSameType<T, uint8_t>::value || IsSameType<T, int32_t>::value ||
        IsSameType<T, uint32_t>::value || IsSameType<T, float>::value) {
        unitMultiples = 2;
    }
    // Value multiplication due to unit mismatch
    LoadData2dTransposeParamsV2 loadDataParamsV2(
        loadDataParam.startIndex * unitMultiples, loadDataParam.repeatTimes, loadDataParam.srcStride * unitMultiples,
        loadDataParam.dstGap, loadDataParam.dstFracGap, 0, loadDataParam.addrMode);
    LoadData2DL12L0BTransposeCal(dst, src, loadDataParamsV2);
}
```

#### L0B V2 — 完整实现（L293-306）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0BTransposeCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParamsV2& loadDataParam)
{
    static_assert(
        SupportType<T, uint8_t, int8_t, half, bfloat16_t, float, int32_t, uint32_t>(),
        "LoadDataWithTranspose only support uint8_t, int8_t, half, bfloat16_t, float, int32_t, uint32_t \
         on current device!");
    if ASCEND_IS_AIC {
        load_cbuf_to_cb_transpose(
            dst, src, loadDataParam.startIndex, loadDataParam.repeatTimes, loadDataParam.srcStride,
            loadDataParam.dstGap, inc, loadDataParam.dstFracGap, loadDataParam.srcFracGap);
    }
}
```

---

### dav_l300（目标版本，3003）

#### L0A V1 — 完整实现（L30-49，未修改）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ATransposeCal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParam)
{
    if constexpr (B8_BYTE_SIZE == sizeof(T)) {
        load_cbuf_to_ca_transpose(
            (__ca__ uint8_t*)dst, (__cbuf__ uint8_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, inc, loadDataParam.dstFracGap);
    } else if constexpr (B16_BYTE_SIZE == sizeof(T)) {
        load_cbuf_to_ca_transpose(
            (__ca__ half*)dst, (__cbuf__ half*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, inc, loadDataParam.dstFracGap);
    } else if constexpr (B32_BYTE_SIZE == sizeof(T)) {
        load_cbuf_to_ca_transpose(
            (__ca__ uint32_t*)dst, (__cbuf__ uint32_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, inc, loadDataParam.dstFracGap);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
    }
}
```

#### L0A V2 — 桩函数（L51-60，**新增**）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ATransposeCal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParamsV2& loadDataParam)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(
            KERNEL_ERROR,
            "LoadDataWithTranspose from L1 Buffer(A1) to L0A Buffer(A2) is not supported on current device");
    });
}
```

#### L0B V1 — 完整实现（L83-102，未修改）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0BTransposeCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParam)
{
    if constexpr (B8_BYTE_SIZE == sizeof(T)) {
        load_cbuf_to_cb_transpose(
            (__cb__ uint8_t*)dst, (__cbuf__ uint8_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, inc, loadDataParam.dstFracGap);
    } else if constexpr (B16_BYTE_SIZE == sizeof(T)) {
        load_cbuf_to_cb_transpose(
            (__cb__ half*)dst, (__cbuf__ half*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, inc, loadDataParam.dstFracGap);
    } else if constexpr (B32_BYTE_SIZE == sizeof(T)) {
        load_cbuf_to_cb_transpose(
            (__cb__ uint32_t*)dst, (__cbuf__ uint32_t*)src, loadDataParam.startIndex, loadDataParam.repeatTimes,
            loadDataParam.srcStride, loadDataParam.dstGap, inc, loadDataParam.dstFracGap);
    } else {
        ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
    }
}
```

#### L0B V2 — 桩函数（L104-110，未修改）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0BTransposeCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParamsV2& loadDataParam)
{
    ASCENDC_REPORT_NOT_SUPPORT(
        false, "LoadDataWithTranspose with LoadData2dTransposeParamsV2 is not supported on current device");
}
```

---

### dav_l310（目标版本，3103）

#### L0A V1 — 桩函数（L31-40，未修改）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ATransposeCal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParam)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(
            KERNEL_ERROR,
            "LoadDataWithTranspose from L1 Buffer(A1) to L0A Buffer(A2) is not supported on current device");
    });
}
```

#### L0A V2 — 桩函数（L42-51，**新增**）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ATransposeCal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParamsV2& loadDataParam)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(
            KERNEL_ERROR,
            "LoadDataWithTranspose from L1 Buffer(A1) to L0A Buffer(A2) is not supported on current device");
    });
}
```

#### L0B V1 — 桩函数（L70-80，未修改）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0BTransposeCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParam)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(
            KERNEL_ERROR,
            "LoadDataWithTranspose with LoadData2dTransposeParams from L1 Buffer(B1) to L0B Buffer(B2) is not "
            "supported on current device");
    });
}
```

#### L0B V2 — 完整实现（L82-95，**替换**，从 dav_3510 移植）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0BTransposeCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParamsV2& loadDataParam)
{
    static_assert(
        SupportType<T, uint8_t, int8_t, half, bfloat16_t, float, int32_t, uint32_t>(),
        "LoadDataWithTranspose only support uint8_t, int8_t, half, bfloat16_t, float, int32_t, uint32_t \
         on current device!");
    if ASCEND_IS_AIC {
        load_cbuf_to_cb_transpose(
            dst, src, loadDataParam.startIndex, loadDataParam.repeatTimes, loadDataParam.srcStride,
            loadDataParam.dstGap, inc, loadDataParam.dstFracGap, loadDataParam.srcFracGap);
    }
}
```

---

### dav_l311（目标版本，3113）

#### L0A V1 — 桩函数（L31-40，未修改）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ATransposeCal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParam)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(
            KERNEL_ERROR,
            "LoadDataWithTranspose from L1 Buffer(A1) to L0A Buffer(A2) is not supported on current device");
    });
}
```

#### L0A V2 — 桩函数（L42-51，**新增**）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ATransposeCal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParamsV2& loadDataParam)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(
            KERNEL_ERROR,
            "LoadDataWithTranspose from L1 Buffer(A1) to L0A Buffer(A2) is not supported on current device");
    });
}
```

#### L0B V1 — 桩函数（L70-80，未修改）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0BTransposeCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParam)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(
            KERNEL_ERROR,
            "LoadDataWithTranspose with LoadData2dTransposeParams from L1 Buffer(B1) to L0B Buffer(B2) is not "
            "supported on current device");
    });
}
```

#### L0B V2 — 桩函数（L82-92，**维持桩函数**）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0BTransposeCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParamsV2& loadDataParam)
{
    ASCENDC_ASSERT(false, {
        KERNEL_LOG(
            KERNEL_ERROR,
            "LoadDataWithTranspose with LoadData2dTransposeParamsV2 from L1 Buffer(B1) to L0B Buffer(B2) is not "
            "supported on current device");
    });
}
```

---

### dav_5161（目标版本，5101）

#### L0A V1 — 桩函数（L261-266，未修改）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ATransposeCal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData A1 To A2 is not supported!"); });
}
```

#### L0A V2 — 桩函数（L268-273，**新增**）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ATransposeCal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParamsV2& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData A1 To A2 is not supported!"); });
}
```

#### L0B V1 — 桩函数（L275-281，**修正错误消息**）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0BTransposeCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParams)
{
    // LoadData2dTransposeParams do not match hardware capabilities, not currently supported, will be added later
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData B1 To B2 is not supported!"); });
}
```

> 修正：错误消息从 "LoadData A1 To A2 is not supported!" 改为 "LoadData B1 To B2 is not supported!"

#### L0B V2 — 桩函数（L283-288，**新增桩函数**）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0BTransposeCal(
    __cb__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParamsV2& loadDataParam)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData B1 To B2 is not supported!"); });
}
```
