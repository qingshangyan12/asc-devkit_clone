# 案例分析：DataCopy(L1→BT) 接口跨版本适配

## 接口概述

`DataCopy(LocalTensor<T>& dst, LocalTensor<U>& src, DataCopyParams& repeatParams)` 用于 L1 Buffer(C1) 到 BiasTable(C2) 的数据搬运，支持类型转换。双模板参数 `(T, U)` 设计目的正是支持搬运过程中的 dtype 转换（如 half→float、half→int32_t 等）。

| 变体 | 参数类型 | 声明位置 | 说明 |
|------|---------|---------|------|
| 双模板 `(T, U)` | `DataCopyParams` | `kernel_operator_data_copy_intf.h:160` | L1→BT，支持类型转换 |

声明**无 `__NPU_ARCH__` 守卫**。

公共包装函数通过 `convControl` 参数控制类型转换行为：
- `convControl = 0`：同类型拷贝（U == T）
- `convControl = 1`：half → float
- `convControl = 2`：特定 arch 的类型转换路径（int32_t dst + half/bfloat16/float src 等）

## 调用链

```
kernel_operator_data_copy_intf.h:160（声明，无 arch 守卫）
  → kernel_operator_data_copy_intf_impl.h:449-533（公共包装，函数体有大量 arch 守卫）
    → DataCopyL12BTImpl(dst, src, convControl, repeatParams)
      → dav_XXXX/kernel_operator_data_copy_impl.h（版本特定实现）
        → copy_cbuf_to_bt（硬件指令）
```

### 公共包装层 arch 守卫（intf_impl.h:449-533）

公共包装函数内按 arch 分三种路径：

| Arch 守卫 | 路径 | convControl | 说明 |
|----------|------|-------------|------|
| `3510 \|\| 5102` 且 `5102` | int32 dst + half/bfloat16/float/int16 src | 2 | 5102 专用类型转换 |
| `3510 \|\| 5102`（非 5102） | 同类型 / half→float | 0/1 | 3510/5102 通用路径，含 bfloat16 检查 |
| `5101 \|\| 5161 \|\| 5165 \|\| 5163` | int32 dst + half/float/int32 src → convControl=2；同类型 → convControl=0 | 2/0 | 5101 系列路径 |
| 其他（3003/3103/3113） | 同类型 → 0；half→float → 1 | 0/1 | 通用路径 |

### 版本隔离机制

`DataCopyL12BTImpl` 不在共享文件中定义，而是直接定义在各版本目录中。`kernel_operator_data_copy_base_impl.h:32-55` 按 arch 条件 include：

```cpp
#if __NPU_ARCH__ == 3510
#include "dav_3510/kernel_operator_data_copy_impl.h"
#elif (__NPU_ARCH__ == 3003)
#include "dav_l300/kernel_operator_data_copy_impl.h"
#elif (__NPU_ARCH__ == 3113)
#include "dav_l311/kernel_operator_data_copy_impl.h"
#elif (__NPU_ARCH__ == 3103)
#include "dav_l310/kernel_operator_data_copy_impl.h"
#elif (__NPU_ARCH__ == 5101 || ...)
#include "dav_5161/kernel_operator_data_copy_impl.h"
#endif
```

## 支持矩阵

| 版本 | Arch | DataCopyL12BTImpl | 硬件指令 | 支持类型 |
|------|------|-------------------|---------|---------|
| dav_l300 | 3003 | **完整** | `copy_cbuf_to_bt` | half, float, int32_t |
| dav_l310 | 3103 | **完整** | `copy_cbuf_to_bt` | half, float, int32_t |
| dav_l311 | 3113 | **完整** | `copy_cbuf_to_bt` | half, float, int32_t |
| dav_5161 | 5101 | **完整** | `copy_cbuf_to_bt`（多 fixVal 参数） | half, float, int32_t |
| dav_3510 | 3510 | **完整** | `copy_cbuf_to_bt`（经 CopyCbufToBt 间接调用） | half, float, int32_t, **bfloat16_t** |

## 各版本原始代码

### dav_3510（参照版本）

#### DataCopyL12BTImpl — 完整实现（L899-906）

```cpp
template <typename T>
__aicore__ inline __in_pipe__(MTE1) __out_pipe__(MTE1) void DataCopyL12BTImpl(
    const uint64_t dst, __cbuf__ T* src, const uint16_t isEnableConv, const DataCopyParams& intriParams)
{
    CopyCbufToBt(
        dst, src, isEnableConv, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride,
        intriParams.dstStride);
}
```

#### CopyCbufToBt — 硬件指令调用（L212-231）

```cpp
template <typename T>
__aicore__ inline void CopyCbufToBt(
    uint64_t dst, __cbuf__ T* src, const uint16_t convControl, const uint16_t blockCount, const uint16_t blockLen,
    const uint16_t srcStride, const uint16_t dstStride)
{
    if ASCEND_IS_AIC {
        if constexpr (
            std::is_same<T, bfloat16_t>::value || std::is_same<T, float>::value || std::is_same<T, int32_t>::value ||
            std::is_same<T, half>::value) {
            if constexpr (sizeof(T) == 4) {
                ASCENDC_DEBUG_ASSERT(
                    (blockLen % 2 == 0),
                    KERNEL_LOG_INTERNAL(
                        KERNEL_ERROR,
                        "For float or int32_t types, blockLen must be even when DataCopy from L1 to BIAS."));
            }
            copy_cbuf_to_bt(dst, src, static_cast<bool>(convControl), blockCount, blockLen, srcStride, dstStride);
        }
    }
}
```

---

### dav_l300（目标版本，3003）

#### DataCopyL12BTImpl — 完整实现（L159-174）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12BTImpl(
    const uint64_t dst, __cbuf__ T* src, const uint16_t isenableConv, const DataCopyParams& intriParams)
{
    // ISA: unit of 64B    API: unit of 32B
    uint16_t burstLength = DivCeil(intriParams.blockLen, 2);
    uint16_t dstStride = DivCeil(intriParams.dstStride, 2);
    if constexpr (std::is_same<T, float>::value || std::is_same<T, int32_t>::value || std::is_same<T, half>::value) {
        copy_cbuf_to_bt(
            dst, src, (bool)isenableConv, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride,
            intriParams.dstStride);
    } else {
        ASCENDC_ASSERT(
            false, { KERNEL_LOG(KERNEL_ERROR, "unsupported data type of copy from cbuf to bt on this version"); });
    }
}
```

---

### dav_l310（目标版本，3103）

#### DataCopyL12BTImpl — 完整实现（L190-205）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12BTImpl(
    const uint64_t dst, __cbuf__ T* src, const uint16_t isenableConv, const DataCopyParams& intriParams)
{
    if constexpr (std::is_same<T, float>::value || std::is_same<T, int32_t>::value || std::is_same<T, half>::value) {
        // the burst length, destination gap size must be even.
        uint16_t blockLenAlign = AlignUp(intriParams.blockLen, 2);
        uint16_t dstStrideAlign = AlignUp(intriParams.dstStride, 2);
        copy_cbuf_to_bt(
            dst, src, (bool)isenableConv, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride,
            intriParams.dstStride);
    } else {
        ASCENDC_ASSERT(
            false, { KERNEL_LOG(KERNEL_ERROR, "unsupported data type of copy from cbuf to bt on this version"); });
    }
}
```

---

### dav_l311（目标版本，3113）

#### DataCopyL12BTImpl — 完整实现（L190-205）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12BTImpl(
    const uint64_t dst, __cbuf__ T* src, const uint16_t isenableConv, const DataCopyParams& intriParams)
{
    if constexpr (std::is_same<T, float>::value || std::is_same<T, int32_t>::value || std::is_same<T, half>::value) {
        // the burst length, destination gap size must be even.
        uint16_t blockLenAlign = AlignUp(intriParams.blockLen, 2);
        uint16_t dstStrideAlign = AlignUp(intriParams.dstStride, 2);
        copy_cbuf_to_bt(
            dst, src, (bool)isenableConv, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride,
            intriParams.dstStride);
    } else {
        ASCENDC_ASSERT(
            false, { KERNEL_LOG(KERNEL_ERROR, "unsupported data type of copy from cbuf to bt on this version"); });
    }
}
```

> **dav_l310 与 dav_l311 的 DataCopyL12BTImpl 代码完全相同。**

---

### dav_5161（目标版本，5101）

#### DataCopyL12BTImpl — 完整实现（L229-246）

```cpp
template <typename T>
__aicore__ inline void DataCopyL12BTImpl(
    const uint64_t dst, __cbuf__ T* src, const uint16_t isenableConv, const DataCopyParams& intriParams)
{
    uint8_t fixVal = 0;
    if constexpr (std::is_same<T, half>::value) {
        fixVal = intriParams.fixShiftVal == 0 ? FIX_VAL : intriParams.fixShiftVal;
    }
    if constexpr (std::is_same<T, float>::value || std::is_same<T, int32_t>::value || std::is_same<T, half>::value) {
        // the destination gap size must be even.
        uint16_t dstStrideAlign = AlignUp(intriParams.dstStride, 2);
        copy_cbuf_to_bt(
            dst, src, isenableConv, intriParams.blockCount, intriParams.blockLen, intriParams.srcStride, dstStrideAlign,
            fixVal);
    } else {
        ASSERT(false && "unsupported data type of copy from cbuf to bt on current device");
    }
}
```

---

## 代码级实现差异分析

### 实现风格对比

| 特征 | dav_3510 | dav_l300 | dav_l310 | dav_l311 | dav_5161 |
|------|----------|----------|----------|----------|----------|
| 调用方式 | 间接（经 CopyCbufToBt） | 直接 | 直接 | 直接 | 直接 |
| 硬件指令签名 | 7 参数 | 7 参数 | 7 参数 | 7 参数 | **8 参数**（多 fixVal） |
| 支持类型 | half/float/int32_t/**bfloat16_t** | half/float/int32_t | half/float/int32_t | half/float/int32_t | half/float/int32_t |
| pipe 标注 | `__in_pipe__(MTE1) __out_pipe__(MTE1)` | 无 | 无 | 无 | 无 |
| AIC 守卫 | `if ASCEND_IS_AIC` | 无 | 无 | 无 | 无 |
| convControl 传参 | `static_cast<bool>(convControl)` | `(bool)isenableConv` | `(bool)isenableConv` | `(bool)isenableConv` | `isenableConv`（未转换） |
| dstStride 传参 | 原始值 | 原始值 | 原始值 | 原始值 | **dstStrideAlign**（对齐后） |
| 错误报告 | `ASCENDC_DEBUG_ASSERT` | `ASCENDC_ASSERT` | `ASCENDC_ASSERT` | `ASCENDC_ASSERT` | `ASSERT` |
| 无用变量 | 无 | burstLength/dstStride（计算未用） | blockLenAlign/dstStrideAlign（计算未用） | 同 l310 | 无（dstStrideAlign 实际使用） |
| half 特殊逻辑 | 无 | 无 | 无 | 无 | **fixShiftVal/FIX_VAL** |

### 实现模式

**模式1：dav_3510 间接调用模式**
- 通过中间函数 `CopyCbufToBt` 调用 `copy_cbuf_to_bt`
- 有 `ASCEND_IS_AIC` 守卫和 pipe 标注
- 唯一支持 bfloat16_t 的版本
- 对 float/int32_t 检查 blockLen 偶数

**模式2：dav_l300/l310/l311 直接模式**
- 直接调用 `copy_cbuf_to_bt`，7 参数
- 无 pipe 标注，无 AIC 守卫
- 支持 half/float/int32_t，不支持 bfloat16_t
- l300 计算 burstLength/dstStride 但未使用（疑似遗留代码）
- l310/l311 计算 blockLenAlign/dstStrideAlign 但未使用（疑似遗留代码）

**模式3：dav_5161 fixVal 模式**
- `copy_cbuf_to_bt` 多第 8 参数 `fixVal`
- half 类型有 fixShiftVal 逻辑（默认 FIX_VAL）
- dstStride 使用对齐后的值
- 使用 `ASSERT` 而非 `ASCENDC_ASSERT`

### 关键差异点

1. **dav_5161 硬件指令签名不同** — `copy_cbuf_to_bt` 多 `fixVal` 参数，不能直接移植其他版本的实现
2. **dav_3510 唯一支持 bfloat16_t** — 通过间接调用和 AIC 守卫实现
3. **dav_l300/l310/l311 有无用变量** — 计算了 align/ceil 值但未传入硬件指令，疑似遗留代码或 bug
4. **dav_3510 有 pipe 标注** — MTE1 pipe 标注仅 3510 有
5. **dav_5161 使用 ASSERT 而非 ASCENDC_ASSERT** — 错误报告方式不一致

## 待确认问题

1. **dav_l300/l310/l311 的无用变量** — `burstLength`/`blockLenAlign`/`dstStrideAlign` 计算后未传入 `copy_cbuf_to_bt`，是遗留代码还是 bug？是否应该使用对齐后的值？
2. **dav_5161 的 fixVal** — `copy_cbuf_to_bt` 的第 8 参数 `fixVal` 是硬件 ISA 差异还是软件扩展？其他版本是否也需要类似逻辑？
3. **bfloat16_t 支持** — 仅 dav_3510 支持 bfloat16_t，其他版本是硬件不支持还是未实现？
4. **公共包装层的 arch 守卫** — 包装函数（intf_impl.h:449-533）内大量 arch 守卫控制 convControl 路径，各版本的 convControl=2 路径支持类型不同，是否需要统一？
5. **dav_3510 的 pipe 标注** — MTE1 标注仅 3510 有，其他版本是否应该添加？
6. **dav_5161 错误报告不一致** — 使用 `ASSERT` 而非 `ASCENDC_ASSERT`，是否需要统一？
7. **dav_l300 注释 "ISA: unit of 64B API: unit of 32B"** — 单位差异说明但计算值未使用，是否需要补全单位换算逻辑？
