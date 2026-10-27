# 案例分析：MmadMxFix 接口跨版本适配

## 接口概述

`MmadMxFix` 用于执行带量化/ReLU/eltwise 等后处理功能的 microscaling 矩阵乘法。有 4 个重载变体，按参数类型和是否有 bias 分组：

| 变体 | 参数签名 | 声明位置 | arch 守卫 | 说明 |
|------|---------|---------|-----------|------|
| V1 | `MmadMxFix(dst, left, right, MmadFixBitModeParams&)` | `intf.h:240-243` | `#if 5101\|\|5161\|\|5165\|\|5163` (L28-29) | BitMode 参数，无 bias |
| V2 | `MmadMxFix(dst, left, right, bias, MmadFixBitModeParams&)` | `intf.h:245-248` | 同上 | BitMode 参数，带 bias |
| V3 | `MmadMxFix(dst, left, right, MmadFixParams&)` | `intf.h:250-253` | 同上 | 普通 Params，无 bias |
| V4 | `MmadMxFix(dst, left, right, bias, MmadFixParams&)` | `intf.h:255-258` | 同上 | 普通 Params，带 bias |

> **整个 intf.h 文件**被 `#if __NPU_ARCH__ == 5101 || 5161 || 5165 || 5163` 守卫（L28-29），4 个变体仅在此架构下声明。与 `MmadFix` 共享同一参数结构体（`MmadFixParams` / `MmadFixBitModeParams`）。

## 调用链

```
intf.h:240/245/250/255（4 变体声明，#if 5101||5161||5165||5163 守卫 L28-29）
  → intf_impl.h:181-211（公共包装，无额外 arch 守卫，委托 MmadMxFixImpl）
    → base_impl.h:204-242（共享实现，无额外 arch 守卫，提取物理地址后委托 MmadMxFixCal）
      → dav_5161/kernel_operator_macro_impl.h:573-698（版本特定实现，通过 base_impl.h:25-35 的 #if 5101||5161||5165||5163 #include 分派）
        → matmul_mx_to_cbuf() 硬件指令
```

### 共享层分发逻辑

**版本隔离机制：** `kernel_operator_macro_base_impl.h:25-35` 通过 `#if __NPU_ARCH__` 分派 `#include`：

```cpp
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
#include "dav_5161/kernel_operator_macro_impl.h"
#endif
```

> **仅 dav_5161 被分派**，5101/5161/5165/5163 共用同一份实现。与 `MmadFix` 完全相同的分派机制。

**intf_impl.h 公共包装层（L181-211）：**

| 变体 | intf_impl.h 行号 | arch 守卫 | 调用的 Impl 函数 |
|------|-----------------|-----------|----------------|
| V1 (BitMode, 无bias) | L181-187 | 无（文件级守卫） | `MmadMxFixImpl(dst, left, right, mmadFixParams)` |
| V2 (BitMode, 带bias) | L189-195 | 无 | `MmadMxFixImpl(dst, left, right, bias, mmadFixParams)` |
| V3 (Params, 无bias) | L197-203 | 无 | `MmadMxFixImpl(dst, left, right, mmadFixParams)` |
| V4 (Params, 带bias) | L205-211 | 无 | `MmadMxFixImpl(dst, left, right, bias, mmadFixParams)` |

**base_impl.h 共享实现层（L204-242）：**

| 变体 | base_impl.h 行号 | arch 守卫 | 调用的 Cal 函数 |
|------|-----------------|-----------|----------------|
| V1 | L204-212 | 无 | `MmadMxFixCal(dst, left, right, mmadFixParams)` |
| V3 | L214-222 | 无 | `MmadMxFixCal(dst, left, right, mmadFixParams)` |
| V2 | L224-232 | 无 | `MmadMxFixCal(dst, left, right, bias, mmadFixParams)` |
| V4 | L234-242 | 无 | `MmadMxFixCal(dst, left, right, bias, mmadFixParams)` |

> base_impl.h 提取物理地址（`GetPhyAddr()`），将 `bias` 转为 `uint64_t`，然后委托 `MmadMxFixCal`。

## 支持矩阵

| 版本 | Arch | V1 (BitMode, 无bias) | V2 (BitMode, 带bias) | V3 (Params, 无bias) | V4 (Params, 带bias) |
|------|------|---------------------|---------------------|---------------------|---------------------|
| dav_l300 | 3003 | **N/A** | **N/A** | **N/A** | **N/A** |
| dav_l310 | 3103 | **N/A** | **N/A** | **N/A** | **N/A** |
| dav_l311 | 3113 | **N/A** | **N/A** | **N/A** | **N/A** |
| dav_5161 | 5101 | **完整** (L573-578) | **完整** (L632-642) | **完整** (L580-630) | **完整** (L644-698) |
| dav_3510 | 3510 | **N/A** | **N/A** | **N/A** | **N/A** |

> N/A = 接口未声明（intf.h 文件级 `#if 5101||5161||5165||5163` 守卫排除）。dav_5161 是唯一实现版本。

## 各版本原始代码

### dav_5161（5101/5161/5165/5163，唯一实现版本）

#### V1 MmadMxFixCal — 完整实现（L573-578）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadMxFixCal(
    __cbuf__ DstT* dst, __cbuf__ Src0T* left, __cb__ Src1T* right, const MmadFixBitModeParams& mmadFixParams)
{
    matmul_mx_to_cbuf(dst, left, right, (__cbuf__ int32_t*)get_imm(0), (uint64_t)0, mmadFixParams.GetConfig0());
}
```

#### V3 MmadMxFixCal — 完整实现（L580-630）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadMxFixCal(
    __cbuf__ DstT* dst, __cbuf__ Src0T* left, __cb__ Src1T* right, const MmadFixParams& mmadFixParams)
{
    constexpr uint8_t POS_BIAS_MATRIX_INITVAL = 0;
    constexpr uint8_t POS_BIAS_MATRIX_BROADCAST = 1;
    constexpr uint8_t POS_PRE_QUANT_MODE = 2;
    constexpr uint8_t POS_PRE_RELU_MODE = 7;
    constexpr uint8_t POS_POST_QUANT_MODE = 10;
    constexpr uint8_t POS_ACT_POST_ENABLE = 15;
    constexpr uint8_t POS_PRE_CLIP_RELU_MODE = 18;
    constexpr uint8_t POS_ELTWISE_OP = 20;
    constexpr uint8_t POS_ELTWISE_ANTIQ_ENABLE = 23;
    constexpr uint8_t POS_ELTWISE_BROADCAST_ENABLE = 24;
    constexpr uint8_t POS_ITA_ENABLE = 25;
    constexpr uint8_t POS_BIAS_S32_ENABLE = 26;
    constexpr uint8_t POS_HMAX_ENABLE = 27;
    constexpr uint8_t POS_LSB_MASK = 28;
    constexpr uint8_t POS_GEMV_CTRL = 31;
    constexpr uint8_t POS_GROUP_MODE = 32;
    constexpr uint8_t POS_DEPEND_ENABLE = 36;
    constexpr uint8_t POS_INSTRI_D = 37;
    constexpr uint8_t POS_POST_PROC_HALF_ENABLE = 39;
    constexpr uint8_t POS_LAYOUT_CTRL = 40;
    constexpr uint8_t POS_N = 48;

    uint64_t config0 = 0;
    config0 |= (uint64_t)(mmadFixParams.biasMatrixInitVal ? 1 : 0) << POS_BIAS_MATRIX_INITVAL;
    config0 |= (uint64_t)(mmadFixParams.biasMatrixBroadcast ? 1 : 0) << POS_BIAS_MATRIX_BROADCAST;
    config0 |= (uint64_t)(static_cast<uint8_t>(mmadFixParams.preQuantMode)) << POS_PRE_QUANT_MODE;
    config0 |= (uint64_t)(static_cast<uint8_t>(mmadFixParams.preReluMode)) << POS_PRE_RELU_MODE;
    config0 |= (uint64_t)(static_cast<uint8_t>(mmadFixParams.postQuantMode)) << POS_POST_QUANT_MODE;
    config0 |= (uint64_t)(mmadFixParams.actPostMode) << POS_ACT_POST_ENABLE;
    config0 |= (uint64_t)(static_cast<uint8_t>(mmadFixParams.preClipReluMode)) << POS_PRE_CLIP_RELU_MODE;
    config0 |= (uint64_t)(static_cast<uint8_t>(mmadFixParams.eltwiseOp)) << POS_ELTWISE_OP;
    config0 |= (uint64_t)(mmadFixParams.eltwiseAntiqEnable ? 1 : 0) << POS_ELTWISE_ANTIQ_ENABLE;
    config0 |= (uint64_t)(mmadFixParams.eltwiseBroadcastEnable ? 1 : 0) << POS_ELTWISE_BROADCAST_ENABLE;
    config0 |= (uint64_t)(mmadFixParams.itaEnable ? 1 : 0) << POS_ITA_ENABLE;
    config0 |= (uint64_t)(mmadFixParams.biasS32Enable ? 1 : 0) << POS_BIAS_S32_ENABLE;
    config0 |= (uint64_t)(mmadFixParams.hmaxEnable ? 1 : 0) << POS_HMAX_ENABLE;
    config0 |= (uint64_t)(static_cast<uint8_t>(mmadFixParams.lsbMask)) << POS_LSB_MASK;
    config0 |= (uint64_t)(mmadFixParams.gemvCtrl ? 1 : 0) << POS_GEMV_CTRL;
    config0 |= (uint64_t)(mmadFixParams.groupMode) << POS_GROUP_MODE;
    config0 |= (uint64_t)(mmadFixParams.dependEnable ? 1 : 0) << POS_DEPEND_ENABLE;
    config0 |= (uint64_t)(static_cast<uint8_t>(mmadFixParams.instrId)) << POS_INSTRI_D;
    config0 |= (uint64_t)(mmadFixParams.postProcHalfEnable ? 1 : 0) << POS_POST_PROC_HALF_ENABLE;
    config0 |= (uint64_t)(mmadFixParams.layoutCtrl ? 1 : 0) << POS_LAYOUT_CTRL;
    config0 |= (uint64_t)(mmadFixParams.n) << POS_N;

    matmul_mx_to_cbuf(dst, left, right, (__cbuf__ int32_t*)get_imm(0), (uint64_t)0, config0);
}
```

#### V2 MmadMxFixCal — 完整实现（L632-642）

```cpp
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
```

#### V4 MmadMxFixCal — 完整实现（L644-698）

```cpp
template <typename DstT, typename Src0T, typename Src1T, typename BiasT>
__aicore__ inline void MmadMxFixCal(
    __cbuf__ DstT* dst, __cbuf__ Src0T* left, __cb__ Src1T* right, uint64_t bias, const MmadFixParams& mmadFixParams)
{
    constexpr uint8_t POS_BIAS_MATRIX_INITVAL = 0;
    // ... (同 V3 的 21 个 POS_ 常量定义, L648-668)

    uint64_t config0 = 0;
    // ... (同 V3 的 21 行 config0 位拼接, L670-691)

    if (mmadFixParams.biasMatrixBroadcast) {
        matmul_mx_to_cbuf(dst, left, right, (__cbuf__ int32_t*)get_imm(0), bias, config0);
    } else {
        matmul_mx_to_cbuf(dst, left, right, (__cbuf__ int32_t*)bias, (uint64_t)0, config0);
    }
}
```

> V4 与 V3 的 config0 构建逻辑完全相同（21 个位域拼接），区别仅在 bias 处理。

### dav_l300 / dav_l310 / dav_l311 / dav_3510 — 未定义

> 这 4 个版本的目录中**不存在** `kernel_operator_macro_impl.h` 文件。`MmadMxFix` 接口在 `intf.h` 文件级 `#if 5101||5161||5165||5163` 守卫下，这些架构编译时不可见。

## 代码级实现差异分析

### MmadMxFix 与 MmadFix 的对比

| 特征 | MmadFix | MmadMxFix |
|------|---------|-----------|
| 硬件指令 | `matmul()` 类型分派器 → `matmul_to_cbuf*` 系列 | **直接** `matmul_mx_to_cbuf()`（无类型分派） |
| 类型分派 | `IsSameType<Src0T, ...>` 编译期 `if constexpr` | **无**（直接调用，由硬件处理类型） |
| 参数结构体 | `MmadFixParams` / `MmadFixBitModeParams` | **共用**（相同） |
| config0 构建 | 21 个位域拼接 | **相同**（21 个位域拼接） |
| bias 处理 | `biasMatrixBroadcast` 决定 bias 传入方式 | **相同** |
| 无 bias 时 | `get_imm(0)` + btBias=0 | **相同** |
| 架构守卫 | `#if 5101\|\|5161\|\|5165\|\|5163` | **相同** |

### 实现风格对比

| 特征 | dav_5161 MmadMxFixCal |
|------|----------------------|
| 接口可见性 | 仅 5101/5161/5165/5163 |
| V1/V2 (BitMode) 实现 | `mmadFixParams.GetConfig0()` 直接获取打包值 |
| V3/V4 (Params) 实现 | 手动构建 64 位 config0（21 个位域拼接） |
| 硬件指令 | `matmul_mx_to_cbuf`（单一指令，无类型分派） |
| AIC 守卫 | 无 |
| bias 处理 | `biasMatrixBroadcast` 决定 bias 传参方式 |
| 无 bias 时 | `get_imm(0)` 作为零 bias 指针，btBias=0 |

### 实现模式总结

**单一实现模式：dav_5161 独占**
- `MmadMxFix` 与 `MmadFix` 一样是 5101/5161/5165/5163 架构专属接口
- 4 个变体共用 `matmul_mx_to_cbuf()` 硬件指令（与 `MmadFix` 的 `matmul()` 类型分派器不同）
- BitMode 变体（V1/V2）通过 `GetConfig0()` 直接获取预打包的 64 位配置值
- Params 变体（V3/V4）在函数内手动构建 64 位 config0（21 个位域拼接）
- bias 处理逻辑与 `MmadFix` 完全一致

### 关键差异点

1. **MmadMxFix vs MmadFix 唯一功能差异** — `MmadFix` 调用 `matmul()` 类型分派器（按 `Src0T × Src1T` 分派到 `matmul_to_cbuf` / `matmul_to_cbuf_e4m3s4` / `matmul_to_cbuf_s8s4` / `matmul_to_cbuf_s4s4`），`MmadMxFix` 直接调用 `matmul_mx_to_cbuf()`（无类型分派，由硬件自行处理 microscaling 类型）
2. **5101/5161/5165/5163 独占接口** — 与 `MmadFix` 相同，从 intf.h 文件级守卫到 base_impl.h 分派均仅限此架构
3. **无跨版本差异** — 4 个架构变体共用 dav_5161 的单一实现
4. **config0 构建逻辑与 MmadFix 完全相同** — 21 个位域拼接代码逐字符一致，仅底层指令从 `matmul_to_cbuf*` 换为 `matmul_mx_to_cbuf`

## 待确认问题

1. **`matmul_mx_to_cbuf` 与 `matmul_to_cbuf` 的硬件差异** — `MmadMxFix` 直接调用 `matmul_mx_to_cbuf`（无类型分派），`MmadFix` 通过 `matmul()` 分派到 `matmul_to_cbuf*` 系列。microscaling 指令是否在硬件层面自动处理类型转换？是否需要额外的 scale matrix 参数？
2. **dav_l300/l310/l311/3510 是否需要 MmadMxFix 支持？** — 当前这些架构完全不可见此接口。如需支持，需确认硬件是否支持 `matmul_mx_to_cbuf` 指令。
3. **V3/V4 config0 手动拼接与 V1/V2 BitMode 位域布局一致性** — 与 `MmadFix` 相同的待确认问题，两者共用 `MmadFixBitModeConfig0` 位域结构体，但 V3/V4 手动拼接的代码是否与位域布局严格一致？
4. **MmadMxFix 是否支持所有 MmadFix 支持的数据类型？** — `MmadFix` 通过 `matmul()` 支持int8/int16/half/float8_e4m3 等，`MmadMxFix` 无类型分派直接调用 `matmul_mx_to_cbuf`，是否支持相同类型集？还是仅支持 microscaling 专用类型（如 mx_fp8_e4m3_t）？
