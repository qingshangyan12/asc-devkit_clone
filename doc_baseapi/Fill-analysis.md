# 案例分析：Fill 接口跨版本适配

## 接口概述

`Fill` 用于 L0A/L0B/L1 buffer 的常量值初始化。有两个 API 变体（功能等价，`InitConstValue` 已废弃）：

| 变体 | 参数类型 | 声明位置 | arch 守卫 | 说明 |
|------|---------|---------|-----------|------|
| Fill | `InitConstValueParams<U>` | `intf.h:384-386` | 无 | 主接口 |
| InitConstValue | `InitConstValueParams<U>` | `intf.h:389-391` | 无 | 已废弃，提示用 Fill 替代 |

**参数结构体** `InitConstValueParams<T>`（定义于 `include/basic_api/kernel_struct_mm.h:444-461`）：

| 字段 | 类型 | 说明 |
|------|------|------|
| `repeatTimes` | `uint16_t` | 重复次数 |
| `blockNum` | `uint16_t` | 块数量 |
| `dstGap` | `uint16_t` | 前一尾部与下一块头之间的间隔 |
| `initValue` | `T` | 初始化值 |

**模板签名：**
```cpp
template <typename T, typename U = PrimT<T>,
          typename Std::enable_if<Std::is_same<PrimT<T>, U>::value, bool>::type = true>
__aicore__ inline void Fill(const LocalTensor<T>& dst, const InitConstValueParams<U>& initConstValueParams);
```

## 调用链

```
intf.h:384/389（声明，无 arch 守卫）
  → intf_impl.h:538-543 / 546-551（公共包装，无 arch 守卫）
    → CheckInitParams（intf_impl.h:514-531，含 #if __NPU_ARCH__ == 2201 守卫，仅影响参数检查逻辑）
    → FillImpl / InitConstValueImpl（base_impl.h:739-757 / 771-790，共享实现，无 arch 守卫）
      → 按 dstScope 分发（L0A / L0B / L1）
        → InitL0ANzMatrixCal / InitL0BNzMatrixCal / InitL1BufferCal
          → dav_XXXX/kernel_operator_mm_impl.h（版本特定实现，通过 base_impl.h:27-49 的 #elif #include 分派）
```

### 共享层分发逻辑（base_impl.h）

**版本隔离机制：** `kernel_operator_mm_base_impl.h:27-49` 通过 `#elif __NPU_ARCH__` 分派 `#include`：

```cpp
#if __NPU_ARCH__ == 1001
  #include "dav_c100/kernel_operator_mm_impl.h"
...
#elif (__NPU_ARCH__ == 3003)
  #include "dav_l300/kernel_operator_mm_impl.h"
#elif (__NPU_ARCH__ == 3113)
  #include "dav_l311/kernel_operator_mm_impl.h"
#elif (__NPU_ARCH__ == 3103)
  #include "dav_l310/kernel_operator_mm_impl.h"
#elif (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
  #include "dav_5161/kernel_operator_mm_impl.h"
#endif
```

**`FillImpl`（base_impl.h:739-757）— 位置分发，无 arch 守卫：**

| dstScope | Cal 函数 | 地址空间 |
|----------|---------|---------|
| `Hardware::L0A` | `InitL0ANzMatrixCal` | `__ca__` |
| `Hardware::L0B` | `InitL0BNzMatrixCal` | `__cb__` |
| `Hardware::L1` | `InitL1BufferCal` | `__cbuf__` |
| 其他 | `ASCENDC_CHECK_TPOSITION` 报错 | — |

> `FillImpl` 与 `InitConstValueImpl` 函数体逻辑完全相同，仅函数名和错误消息字符串不同。

### CheckInitParams 中的 arch 守卫（intf_impl.h:514-531）

```cpp
#if ASCENDC_CPU_DEBUG
    uint16_t repeatTime = initConstValueParams.repeatTimes;
#if __NPU_ARCH__ == 2201
    uint16_t blockNum = initConstValueParams.blockNum;
    uint16_t dstGap = initConstValueParams.dstGap;
#else
    uint16_t blockNum = 1;       // 非 2201 架构硬编码为 1
    uint16_t dstGap = 0;         // 非 2201 架构硬编码为 0
#endif
```

> **注意：** 仅 2201 架构在参数检查中使用真实的 `blockNum`/`dstGap` 值，其余所有版本（含全部目标版本和参照版本）在检查阶段将这两个参数硬编码为 1 和 0。但实际 Cal 实现中仍使用参数的真实值。

## 支持矩阵

注：在宏指令场景下没有l0a，所以是部分支持，自NCH版本硬件变更裁剪了l0b，所以NCH只支持L1
| 版本 | Arch | L1 (InitL1BufferCal) | L0A (InitL0ANzMatrixCal) | L0B (InitL0BNzMatrixCal) |
|------|------|----------------------|--------------------------|--------------------------|
| dav_l300 | 3003 | **完整** (L639-655) | **完整** (L660-676) | **完整** (L681-697) |
| dav_l310 | 3103 | **完整** (L618-634) | **完整** (L639-655) | **完整** (L660-676) |
| dav_l311 | 3113 | **完整** (L636-652) | **完整** (L657-673) | **完整** (L678-694) |
| dav_5161 | 5101 | **完整** (L478-493) | **桩** (L498-502) | **桩** (L507-511) |
| dav_3510 | 3510 | **完整** (L722-744) | **桩** (L749-754) | **桩** (L759-764) |

> Fill 和 InitConstValue 的支持矩阵完全一致（两者调用相同的 Cal 函数）。

## 各版本原始代码

### dav_3510（参照版本）

#### InitL1BufferCal — 完整实现（L722-744）

```cpp
template <typename T>
__aicore__ inline void InitL1BufferCal(__cbuf__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    if ASCEND_IS_AIC {
        int64_t repeatBit = (static_cast<uint64_t>(initConstValueParams.blockNum) << 16) |
                            (static_cast<uint64_t>(initConstValueParams.dstGap) << 32) |
                            initConstValueParams.repeatTimes;
        if constexpr (IsSameType<T, bfloat16_t>::value) {
            create_cbuf_matrix_bf16(dst, repeatBit, initConstValueParams.initValue);
        } else if constexpr (IsSameType<T, uint32_t>::value || IsSameType<T, half>::value) {
            create_cbuf_matrix(dst, repeatBit, static_cast<T>(initConstValueParams.initValue));
        } else if constexpr (IsSameType<T, int16_t>::value || IsSameType<T, uint16_t>::value) {
            create_cbuf_matrix(dst, repeatBit, GetScalarBitcodeToHalf(initConstValueParams.initValue));
        } else if constexpr (IsSameType<T, float>::value || IsSameType<T, int32_t>::value) {
            create_cbuf_matrix(
                dst, repeatBit, static_cast<uint32_t>(GetScalarBitcodeValue(initConstValueParams.initValue)));
        } else {
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(KERNEL_ERROR, "InitConstValue doesn't support current data type on current device");
            });
        }
    }
}
```

#### InitL0ANzMatrixCal — 桩函数（L749-754）

```cpp
template <typename T>
__aicore__ inline void InitL0ANzMatrixCal(__ca__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    ASCENDC_ASSERT(
        false, { KERNEL_LOG(KERNEL_ERROR, "InitConstValue in L0A Buffer(A2) is not supported on current device"); });
}
```

#### InitL0BNzMatrixCal — 桩函数（L759-764）

```cpp
template <typename T>
__aicore__ inline void InitL0BNzMatrixCal(__cb__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    ASCENDC_ASSERT(
        false, { KERNEL_LOG(KERNEL_ERROR, "InitConstValue in L0B Buffer(B2) is not supported on current device"); });
}
```

---

### dav_l300（目标版本，3003）

#### InitL1BufferCal — 完整实现（L639-655）

```cpp
template <typename T>
__aicore__ inline void InitL1BufferCal(__cbuf__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    if constexpr (B16_BYTE_SIZE == sizeof(T)) {
        T tmpValue = initConstValueParams.initValue;
        // 由于set_l0_set_value_h只支持half类型，故需要将其它类型的数据的二进制用half
        half initValue = *(half*)(&tmpValue);
        set_l0_set_value_h(initValue);
        int64_t config = 0;
        config |= (uint64_t)initConstValueParams.repeatTimes;
        config |= (uint64_t)initConstValueParams.blockNum << 16;
        config |= (uint64_t)initConstValueParams.dstGap << 32;
        set_l1_2d((__cbuf__ half*)dst, config);
    } else {
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported the data type!"); });
    }
}
```

#### InitL0ANzMatrixCal — 完整实现（L660-676）

```cpp
template <typename T>
__aicore__ inline void InitL0ANzMatrixCal(__ca__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    if constexpr (B16_BYTE_SIZE == sizeof(T)) {
        T tmpValue = initConstValueParams.initValue;
        // 由于set_l0_set_value_h只支持half类型，故需要将其它类型的数据的二进制用half
        half initValue = *(half*)(&tmpValue);
        set_l0_set_value_h(initValue);
        int64_t config = 0;
        config |= static_cast<uint64_t>(initConstValueParams.repeatTimes);
        config |= static_cast<uint64_t>(initConstValueParams.blockNum) << 16;
        config |= static_cast<uint64_t>(initConstValueParams.dstGap) << 32;
        set_l0a_2d((__ca__ half*)dst, config);
    } else {
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported the data type!"); });
    }
}
```

#### InitL0BNzMatrixCal — 完整实现（L681-697）

```cpp
template <typename T>
__aicore__ inline void InitL0BNzMatrixCal(__cb__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    if constexpr (B16_BYTE_SIZE == sizeof(T)) {
        T tmpValue = initConstValueParams.initValue;
        // 由于set_l0_set_value_h只支持half类型，故需要将其它类型的数据的二进制用half
        half initValue = *(half*)(&tmpValue);
        set_l0_set_value_h(initValue);
        int64_t config = 0;
        config |= static_cast<uint64_t>(initConstValueParams.repeatTimes);
        config |= static_cast<uint64_t>(initConstValueParams.blockNum) << 16;
        config |= static_cast<uint64_t>(initConstValueParams.dstGap) << 32;
        set_l0b_2d((__cb__ half*)dst, config);
    } else {
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported the data type!"); });
    }
}
```

---

### dav_l310（目标版本，3103）

#### InitL1BufferCal — 完整实现（L618-634）

```cpp
template <typename T>
__aicore__ inline void InitL1BufferCal(__cbuf__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    if constexpr (2 == sizeof(T)) {
        T tmpValue = initConstValueParams.initValue;
        // 由于set_l0_set_value_h只支持half类型，故需要将其它类型的数据的二进制用half
        half initValue = *(half*)(&tmpValue);
        set_l0_set_value_h(initValue);
        int64_t config = 0;
        config |= (uint64_t)initConstValueParams.repeatTimes;
        config |= (uint64_t)initConstValueParams.blockNum << 16;
        config |= (uint64_t)initConstValueParams.dstGap << 32;
        set_l1_2d((__cbuf__ half*)dst, config);
    } else {
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported the data type!"); });
    }
}
```

#### InitL0ANzMatrixCal — 完整实现（L639-655）

```cpp
template <typename T>
__aicore__ inline void InitL0ANzMatrixCal(__ca__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    if constexpr (2 == sizeof(T)) {
        T tmpValue = initConstValueParams.initValue;
        // 由于set_l0_set_value_h只支持half类型，故需要将其它类型的数据的二进制用half
        half initValue = *(half*)(&tmpValue);
        set_l0_set_value_h(initValue);
        int64_t config = 0;
        config |= static_cast<uint64_t>(initConstValueParams.repeatTimes);
        config |= static_cast<uint64_t>(initConstValueParams.blockNum) << 16;
        config |= static_cast<uint64_t>(initConstValueParams.dstGap) << 32;
        set_l0a_2d((__ca__ half*)dst, config);
    } else {
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported the data type!"); });
    }
}
```

#### InitL0BNzMatrixCal — 完整实现（L660-676）

```cpp
template <typename T>
__aicore__ inline void InitL0BNzMatrixCal(__cb__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    if constexpr (2 == sizeof(T)) {
        T tmpValue = initConstValueParams.initValue;
        // 由于set_l0_set_value_h只支持half类型，故需要将其它类型的数据的二进制用half
        half initValue = *(half*)(&tmpValue);
        set_l0_set_value_h(initValue);
        int64_t config = 0;
        config |= static_cast<uint64_t>(initConstValueParams.repeatTimes);
        config |= static_cast<uint64_t>(initConstValueParams.blockNum) << 16;
        config |= static_cast<uint64_t>(initConstValueParams.dstGap) << 32;
        set_l0b_2d((__cb__ half*)dst, config);
    } else {
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported the data type!"); });
    }
}
```

---

### dav_l311（目标版本，3113）

#### InitL1BufferCal — 完整实现（L636-652）

```cpp
template <typename T>
__aicore__ inline void InitL1BufferCal(__cbuf__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    if constexpr (2 == sizeof(T)) {
        T tmpValue = initConstValueParams.initValue;
        // 由于set_l0_set_value_h只支持half类型，故需要将其它类型的数据的二进制用half
        half initValue = *(half*)(&tmpValue);
        set_l0_set_value_h(initValue);
        int64_t config = 0;
        config |= (uint64_t)initConstValueParams.repeatTimes;
        config |= (uint64_t)initConstValueParams.blockNum << 16;
        config |= (uint64_t)initConstValueParams.dstGap << 32;
        set_l1_2d((__cbuf__ half*)dst, config);
    } else {
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported the data type!"); });
    }
}
```

#### InitL0ANzMatrixCal — 完整实现（L657-673）

```cpp
template <typename T>
__aicore__ inline void InitL0ANzMatrixCal(__ca__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    if constexpr (2 == sizeof(T)) {
        T tmpValue = initConstValueParams.initValue;
        // 由于set_l0_set_value_h只支持half类型，故需要将其它类型的数据的二进制用half
        half initValue = *(half*)(&tmpValue);
        set_l0_set_value_h(initValue);
        int64_t config = 0;
        config |= static_cast<uint64_t>(initConstValueParams.repeatTimes);
        config |= static_cast<uint64_t>(initConstValueParams.blockNum) << 16;
        config |= static_cast<uint64_t>(initConstValueParams.dstGap) << 32;
        set_l0a_2d((__ca__ half*)dst, config);
    } else {
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported the data type!"); });
    }
}
```

#### InitL0BNzMatrixCal — 完整实现（L678-694）

```cpp
template <typename T>
__aicore__ inline void InitL0BNzMatrixCal(__cb__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    if constexpr (2 == sizeof(T)) {
        T tmpValue = initConstValueParams.initValue;
        // 由于set_l0_set_value_h只支持half类型，故需要将其它类型的数据的二进制用half
        half initValue = *(half*)(&tmpValue);
        set_l0_set_value_h(initValue);
        int64_t config = 0;
        config |= static_cast<uint64_t>(initConstValueParams.repeatTimes);
        config |= static_cast<uint64_t>(initConstValueParams.blockNum) << 16;
        config |= static_cast<uint64_t>(initConstValueParams.dstGap) << 32;
        set_l0b_2d((__cb__ half*)dst, config);
    } else {
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported the data type!"); });
    }
}
```

> **dav_l310 与 dav_l311 的三个 Cal 函数定义完全相同**（仅行号不同）。

---

### dav_5161（目标版本，5101）

#### InitL1BufferCal — 完整实现（L478-493）

```cpp
template <typename T>
__aicore__ inline void InitL1BufferCal(__cbuf__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    if constexpr (2 == sizeof(T)) {
        T tmpValue = initConstValueParams.initValue;
        half initValue = *(half*)(&tmpValue);
        set_l0_set_value_h(initValue);
        int64_t config = 0;
        config |= (uint64_t)initConstValueParams.repeatTimes;
        config |= (uint64_t)initConstValueParams.blockNum << 16;
        config |= (uint64_t)initConstValueParams.dstGap << 32;
        set_l1_2d((__cbuf__ half*)dst, config);
    } else {
        ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "unsupported the data type!"); });
    }
}
```

#### InitL0ANzMatrixCal — 桩函数（L498-502）

```cpp
template <typename T>
__aicore__ inline void InitL0ANzMatrixCal(__ca__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "InitConstValue unsupported the position A2!"); });
}
```

#### InitL0BNzMatrixCal — 桩函数（L507-511）

```cpp
template <typename T>
__aicore__ inline void InitL0BNzMatrixCal(__cb__ T* dst, const InitConstValueParams<T>& initConstValueParams)
{
    ASCENDC_ASSERT((false), { KERNEL_LOG(KERNEL_ERROR, "InitConstValue unsupported the position B2!"); });
}
```

> **注意：** dav_5161 的 `InitL1BufferCal` 与 dav_l310/l311 逻辑相同，但缺少中文注释行 `// 由于set_l0_set_value_h只支持half类型...`。

---

## 代码级实现差异分析

### 实现风格对比

| 特征 | dav_3510 | dav_l300 | dav_l310 | dav_l311 | dav_5161 |
|------|----------|----------|----------|----------|----------|
| 类型检查方式 (L1) | `IsSameType<T,...>` 多分支 | `B16_BYTE_SIZE == sizeof(T)` | `2 == sizeof(T)` | `2 == sizeof(T)` | `2 == sizeof(T)` |
| AIC 守卫 | `if ASCEND_IS_AIC` | 无 | 无 | 无 | 无 |
| L1 硬件指令 | `create_cbuf_matrix` / `create_cbuf_matrix_bf16` | `set_l0_set_value_h` + `set_l1_2d` | `set_l0_set_value_h` + `set_l1_2d` | `set_l0_set_value_h` + `set_l1_2d` | `set_l0_set_value_h` + `set_l1_2d` |
| L0A 硬件指令 | 无（桩） | `set_l0a_2d` | `set_l0a_2d` | `set_l0a_2d` | 无（桩） |
| L0B 硬件指令 | 无（桩） | `set_l0b_2d` | `set_l0b_2d` | `set_l0b_2d` | 无（桩） |
| 支持数据类型 (L1) | bfloat16_t, half, uint32_t, int16_t, uint16_t, float, int32_t | half, bfloat16_t (仅2字节) | half, bfloat16_t (仅2字节) | half, bfloat16_t (仅2字节) | half, bfloat16_t (仅2字节) |
| config 构建方式 | `(blockNum<<16) \| (dstGap<<32) \| repeatTimes` 单行 | `config |= ...;` 三行逐字段 | 同 l300 | 同 l300 | 同 l300 |
| 类型转换辅助 | `GetScalarBitcodeToHalf` / `GetScalarBitcodeValue` | `*(half*)(&tmpValue)` 指针强转 | `*(half*)(&tmpValue)` 指针强转 | `*(half*)(&tmpValue)` 指针强转 | `*(half*)(&tmpValue)` 指针强转 |
| 中文注释 | 无 | 有 | 有 | 有 | 无 |
| static_cast 风格 | `static_cast<uint64_t>` | 混用（L1 用 C 风格转换，L0A/L0B 用 `static_cast`） | 混用（同 l300） | 混用（同 l300） | C 风格转换 |

### 两种实现模式

**模式1：dav_3510 — 多类型分支模式**
- 使用 `IsSameType<T, ...>` 进行编译期类型分派
- 通过 `create_cbuf_matrix` / `create_cbuf_matrix_bf16` 硬件指令直接初始化 L1
- 对每种数据类型采用不同的值转换策略（`GetScalarBitcodeToHalf`、`GetScalarBitcodeValue`、`static_cast`）
- 有 `if ASCEND_IS_AIC` 守卫，仅在 AIC 核上执行
- 支持最丰富的数据类型（7种）
- L0A/L0B 为桩函数

**模式2：dav_l300/l310/l311/5161 — half 中转模式**
- 使用 `sizeof(T) == 2`（或 `B16_BYTE_SIZE`）判断，仅支持 2 字节类型
- 将 initValue 通过 `*(half*)(&tmpValue)` 指针强转 reinterpret 为 half
- 调用 `set_l0_set_value_h` 设置值，再调用 `set_l1_2d` / `set_l0a_2d` / `set_l0b_2d` 完成初始化
- config 参数通过位拼接（`repeatTimes | blockNum<<16 | dstGap<<32`）
- 无 AIC 守卫
- 仅支持 half / bfloat16_t（2字节类型）
- dav_l300/l310/l311 三个路径（L0A/L0B/L1）全部完整实现
- dav_5161 仅 L1 完整，L0A/L0B 为桩函数

### 关键差异点

1. **dav_l300 是唯一使用 `B16_BYTE_SIZE` 常量判断的版本** — 其余版本（l310/l311/5161）使用硬编码 `2 == sizeof(T)`，功能等价但风格不同
2. **dav_3510 使用完全不同的硬件指令** — `create_cbuf_matrix` 系列 vs `set_l0_set_value_h` + `set_lX_2d` 组合
3. **dav_3510 支持最广泛的数据类型** — 7 种（含 int16/uint16/float/int32），而 l300/l310/l311/5161 仅支持 2 字节类型（half/bfloat16_t）
4. **dav_3510 有 AIC 守卫** — 仅在 AIV 核上跳过执行；其余版本无此守卫，所有核均执行
5. **dav_3510 和 dav_5161 的 L0A/L0B 均为桩函数** — 但错误消息不同：3510 明确指出 "not supported on current device"，5161 说 "unsupported the position A2/B2"
6. **dav_l310 与 dav_l311 代码完全相同** — 三个 Cal 函数体逐字符一致
7. **dav_5161 缺少中文注释** — 与 l310/l311 相比缺少 `// 由于set_l0_set_value_h只支持half类型...` 注释
8. **config 构建方式差异** — dav_3510 使用单行位运算表达式，其余版本使用三行 `config |=` 逐字段累加（功能等价）
9. **dav_l300 的 L1 用 C 风格转换，L0A/L0B 用 `static_cast`** — 同一文件内风格不一致；dav_l310/l311 相同；dav_5161 全部用 C 风格

## 待确认问题

1. **dav_3510/dav_5161 的 L0A/L0B 桩函数** — 硬件是否支持 `set_l0a_2d` / `set_l0b_2d` 指令？如支持，是否需要从 dav_l300 移植完整实现？还是这两个架构的 L0A/L0B 硬件不支持常量初始化？
2. **dav_5161 缺少中文注释** — 是有意删除还是遗漏？该注释解释了为何用 half 指针强转，对维护有参考价值。
3. **数据类型支持范围** — dav_l300/l310/l311/5161 仅支持 2 字节类型（half/bfloat16_t），若用户需要 int32/float 等类型的 Fill，是否需要参照 dav_3510 的多类型分支模式扩展？还是硬件指令 `set_l0_set_value_h` 本身仅支持 half？
4. **CheckInitParams 中 blockNum/dstGap 硬编码** — 非 2201 架构在参数检查阶段将 blockNum=1、dstGap=0，但实际实现使用真实值。这是否意味着这些架构的 blockNum/dstGap 参数实际无效（硬件忽略），还是检查逻辑需要更新？
5. **dav_l300 使用 `B16_BYTE_SIZE` vs dav_l310/l311/5161 使用 `2`** — 是否应统一为常量 `B16_BYTE_SIZE` 以提高可读性？
6. **dav_l300 同文件内 cast 风格不一致** — L1 的 `InitL1BufferCal` 用 C 风强转 `(uint64_t)`，而 L0A/L0B 的 Cal 函数用 `static_cast<uint64_t>`。是否需要统一？
7. **`InitConstValue` 废弃接口的兼容性** — 已标注 "has been updated, please use Fill instead"，但两者实现完全相同。是否在所有目标版本中均保留 InitConstValue 的完整支持？
8. **dav_3510 的 `if ASCEND_IS_AIC` 守卫** — 其余版本均无此守卫。在 AIV 核上调用 Fill 时，3510 静默跳过，其余版本会执行硬件指令。这是否会导致跨版本行为差异？
