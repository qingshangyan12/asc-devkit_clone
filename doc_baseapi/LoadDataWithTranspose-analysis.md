# 案例分析：LoadDataWithTranspose 接口跨版本适配

## 接口概述

`LoadDataWithTranspose` 用于 L1→L0A/L0B 带转置的数据加载，有两个变体：

| 变体 | 参数类型 | 声明位置 | 说明 |
|------|---------|---------|------|
| V1 | `LoadData2dTransposeParams` | `intf.h:246` | 基础转置参数 |
| V2 | `LoadData2dTransposeParamsV2` | `intf.h:263` | 增加 `srcFracGap`、`addrMode` 字段 |

两个声明均无 `__NPU_ARCH__` 守卫。

## 调用链

```
intf.h:246/263（声明，无 arch 守卫）
  → intf_impl.h:337/363（公共包装，无 arch 守卫）
    → load2d_impl.h:122/154（共享实现，无 arch 守卫）
      → dav_XXXX/kernel_operator_mm_impl.h（版本特定实现）
```

**注意：** 整条调用链无任何 `__NPU_ARCH__` 守卫，版本隔离仅靠 `kernel_operator_mm_base_impl.h` 的 `#elif` 分派 `#include` 实现。

### 共享层实现（load2d_impl.h）

| 变体 | 行号 | L0A 路径 | L0B 路径 |
|------|------|---------|---------|
| V1 | L122-139 | 调用 `LoadData2DL12L0ATransposeCal` | 调用 `LoadData2DL12L0BTransposeCal` |
| V2 | L154-168 | **不调用**（仅 L0B） | 调用 `LoadData2DL12L0BTransposeCal` |

V2 变体在共享层仅走 L0B 路径，因此所有版本都不需要 `LoadData2DL12L0ATransposeCal` 的 V2 重载。

## 支持矩阵

| 版本 | Arch | L0A V1 | L0A V2 | L0B V1 | L0B V2 |
|------|------|--------|--------|--------|--------|
| dav_l300 | 3003 | **完整** | 未定义 | **完整** | 桩 |
| dav_l310 | 3103 | 桩 | 未定义 | 桩 | 桩 |
| dav_l311 | 3113 | 桩 | 未定义 | 桩 | 桩 |
| dav_5161 | 5101 | 桩 | 未定义 | 桩 | **未定义** |
| dav_3510 | 3510 | 桩 | 未定义 | **完整**(委托V2) | **完整** |

## 各版本原始代码

### dav_3510（参照版本）

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

#### L0A V1 — 完整实现（L30-49）

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

#### L0B V1 — 完整实现（L83-102）

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

#### L0B V2 — 桩函数（L104-110）

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

#### L0A V1 — 桩函数（L31-40）

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

#### L0B V1 — 桩函数（L70-80）

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

#### L0B V2 — 完整实现（L82-95，适配后从桩函数改为完整实现）

> **注意**：适配后 L0B V2 已从桩函数改为完整实现。此为 analysis 文档记录的适配前状态。

---

### dav_l311（目标版本，3113）

#### L0A V1 — 桩函数（L31-40）

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

#### L0B V1 — 桩函数（L70-80）

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

#### L0B V2 — 桩函数（L82-92）

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

> **dav_l310 与 dav_l311 的 TransposeCal 函数定义完全相同。**

---

### dav_5161（目标版本，5101）

#### L0A V1 — 桩函数（L261-266）

```cpp
template <typename T>
__aicore__ inline void LoadData2DL12L0ATransposeCal(
    __ca__ T* dst, __cbuf__ T* src, const LoadData2dTransposeParams& loadDataParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "LoadData A1 To A2 is not supported!"); });
}
```

#### L0B V1 — 桩函数（L275-281，适配后错误消息已修正）

> **注意**：适配后错误消息已从 "LoadData A1 To A2" 修正为 "LoadData B1 To B2"。此为 analysis 文档记录的适配前状态。

#### L0B V2 — 未定义

> dav_5161 未定义 `LoadData2dTransposeParamsV2` 重载。

---

## 代码级实现差异分析

### 实现风格对比

| 特征 | dav_3510 | dav_l300 | dav_l310 | dav_l311 | dav_5161 |
|------|----------|----------|----------|----------|----------|
| 类型检查方式 | `static_assert` + `SupportType` | `if constexpr (sizeof(T))` | 无 | 无 | 无 |
| L0B V1 实现方式 | 委托 V2 | 直接调用硬件指令 | 桩 | 桩 | 桩 |
| 硬件指令调用 | `load_cbuf_to_cb_transpose` | `load_cbuf_to_cb_transpose` / `load_cbuf_to_ca_transpose` | 无 | 无 | 无 |
| AIC 守卫 | `if ASCEND_IS_AIC` | 无 | 无 | 无 | 无 |
| 支持的数据类型 | uint8/int8/half/bfloat16/float/int32/uint32 | uint8/half/uint32 (按 sizeof 分支) | 无 | 无 | 无 |
| unitMultiples 转换 | 有（int8/uint8/int32/uint32/float 乘2） | 无 | 无 | 无 | 无 |

### 两种实现模式

**模式1：dav_3510 委托模式**
- V1 参数转换为 V2（`unitMultiples` 乘以 startIndex/srcStride）
- V2 调用 `load_cbuf_to_cb_transpose` 硬件指令
- 优点：V1/V2 统一入口，减少重复代码
- 缺点：需要正确计算 unitMultiples

**模式2：dav_l300 直接模式**
- V1 直接调用硬件指令，按 `sizeof(T)` 分支选择类型
- V2 为桩函数
- 优点：V1 实现完整，同时支持 L0A 和 L0B
- 缺点：L0B V2 未实现

### 关键差异点

1. **dav_l300 是唯一实现 L0A 转置的版本** — 调用 `load_cbuf_to_ca_transpose`
2. **dav_3510 的 L0B V1 委托 V2** — 通过 `unitMultiples` 处理单位换算
3. **dav_l310/dav_l311 全桩** — 所有 TransposeCal 均断言不支持
4. **dav_5161 最不完整** — 连 L0B V2 重载都未定义

## 待确认问题

1. **dav_l310/dav_l311 的 L0B V2 桩函数** — 硬件是否支持 `load_cbuf_to_cb_transpose` 指令？如支持，是否需要从 dav_3510 移植完整实现？
2. **dav_l310/dav_l311 的 L0B V1 桩函数** — 是否需要像 dav_3510 那样实现 V1→V2 委托？还是硬件不支持 V1 转置？
3. **dav_l310/dav_l311 的 L0A V1 桩函数** — dav_l300 实现了 L0A 转置，l310/l311 硬件是否有相同的 L0A 转置能力？
4. **dav_5161 的 L0B V2 缺失** — 是硬件不支持，还是遗漏未实现？需要补全桩函数还是完整实现？
5. **dav_l300 的 L0B V2 桩函数** — 硬件是否支持 V2 参数（`srcFracGap`）？如支持，是否需要从 dav_3510 移植？
6. **实现风格选择** — 若需补全，采用 dav_3510 的 V1→V2 委托模式，还是 dav_l300 的 `sizeof(T)` 直接分支模式？
7. **dav_5161 错误消息错误** — L0B V1 桩函数错误消息为 "LoadData A1 To A2"，应为 "B1 To B2"，是否需要修正？
