# 案例分析：MmadFix 接口跨版本适配

## 接口概述

`MmadFix` 用于执行带量化/ReLU/eltwise 等后处理功能的矩阵乘法。有 4 个重载变体，按参数类型和是否有 bias 分组：

| 变体 | 参数签名 | 声明位置 | arch 守卫 | 说明 |
|------|---------|---------|-----------|------|
| V1 | `MmadFix(dst, left, right, MmadFixBitModeParams&)` | `intf.h:211-214` | `#if 5101\|\|5161\|\|5165\|\|5163` (L28-29) | BitMode 参数，无 bias |
| V2 | `MmadFix(dst, left, right, bias, MmadFixBitModeParams&)` | `intf.h:216-219` | 同上 | BitMode 参数，带 bias |
| V3 | `MmadFix(dst, left, right, MmadFixParams&)` | `intf.h:221-224` | 同上 | 普通 Params，无 bias |
| V4 | `MmadFix(dst, left, right, bias, MmadFixParams&)` | `intf.h:226-229` | 同上 | 普通 Params，带 bias |

> **整个 intf.h 文件**被 `#if __NPU_ARCH__ == 5101 || 5161 || 5165 || 5163` 守卫（L28-29），4 个变体仅在此架构下声明。

**参数结构体：**

- `MmadFixParams`（`include/basic_api/kernel_struct_macro.h:291-346`）— 21 个字段的普通结构体（biasMatrixInitVal, biasMatrixBroadcast, preQuantMode, preReluMode, postQuantMode, actPostMode, preClipReluMode, eltwiseOp, eltwiseAntiqEnable, eltwiseBroadcastEnable, itaEnable, biasS32Enable, hmaxEnable, lsbMask, gemvCtrl, groupMode, dependEnable, instrId, postProcHalfEnable, layoutCtrl, n）
- `MmadFixBitModeParams`（`include/basic_api/kernel_operator_macro_bitmode_intf.h:408-464`）— 类，内部使用 `union { uint64_t config0; struct MmadFixBitModeConfig0 config0BitMode; }` 位域打包，提供 Get/Set 方法 + `GetConfig0()` 返回打包的 64 位值

## 调用链

```
intf.h:211/216/221/226（4 变体声明，#if 5101||5161||5165||5163 守卫 L28-29）
  → intf_impl.h:149-179（公共包装，无额外 arch 守卫，委托 MmadFixImpl）
    → base_impl.h:164-202（共享实现，无额外 arch 守卫，提取物理地址后委托 MmadFixCal）
      → dav_5161/kernel_operator_macro_impl.h:446-571（版本特定实现，通过 base_impl.h:25-35 的 #if 5101||5161||5165||5163 #include 分派）
        → matmul() (L425-444) → matmul_to_cbuf*() 硬件指令
```

### 共享层分发逻辑

**版本隔离机制：** `kernel_operator_macro_base_impl.h:25-35` 通过 `#if __NPU_ARCH__` 分派 `#include`：

```cpp
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
#include "dav_5161/kernel_operator_macro_impl.h"
#endif
```

> **仅 dav_5161 被分派**，5101/5161/5165/5163 共用同一份实现。

**intf_impl.h 公共包装层（L149-179）：**

| 变体 | intf_impl.h 行号 | arch 守卫 | 调用的 Impl 函数 |
|------|-----------------|-----------|----------------|
| V1 (BitMode, 无bias) | L149-155 | 无（文件级守卫） | `MmadFixImpl(dst, left, right, mmadFixParams)` |
| V2 (BitMode, 带bias) | L157-163 | 无 | `MmadFixImpl(dst, left, right, bias, mmadFixParams)` |
| V3 (Params, 无bias) | L165-171 | 无 | `MmadFixImpl(dst, left, right, mmadFixParams)` |
| V4 (Params, 带bias) | L173-179 | 无 | `MmadFixImpl(dst, left, right, bias, mmadFixParams)` |

**base_impl.h 共享实现层（L164-202）：**

| 变体 | base_impl.h 行号 | arch 守卫 | 调用的 Cal 函数 |
|------|-----------------|-----------|----------------|
| V1 | L164-172 | 无 | `MmadFixCal(dst, left, right, mmadFixParams)` |
| V3 | L174-182 | 无 | `MmadFixCal(dst, left, right, mmadFixParams)` |
| V2 | L184-192 | 无 | `MmadFixCal(dst, left, right, bias, mmadFixParams)` |
| V4 | L194-202 | 无 | `MmadFixCal(dst, left, right, bias, mmadFixParams)` |

> base_impl.h 提取物理地址（`GetPhyAddr()`），将 `bias` 转为 `uint64_t`，然后委托 `MmadFixCal`。

## 支持矩阵

| 版本 | Arch | V1 (BitMode, 无bias) | V2 (BitMode, 带bias) | V3 (Params, 无bias) | V4 (Params, 带bias) |
|------|------|---------------------|---------------------|---------------------|---------------------|
| dav_l300 | 3003 | **N/A** | **N/A** | **N/A** | **N/A** |
| dav_l310 | 3103 | **N/A** | **N/A** | **N/A** | **N/A** |
| dav_l311 | 3113 | **N/A** | **N/A** | **N/A** | **N/A** |
| dav_5161 | 5101 | **完整** (L446-451) | **完整** (L505-515) | **完整** (L453-503) | **完整** (L517-571) |
| dav_3510 | 3510 | **N/A** | **N/A** | **N/A** | **N/A** |

> N/A = 接口未声明（intf.h 文件级 `#if 5101||5161||5165||5163` 守卫排除）。dav_5161 是唯一实现版本，5101/5161/5165/5163 共用此实现。

## 各版本原始代码

### dav_5161（5101/5161/5165/5163，唯一实现版本）

#### matmul() 类型分派器（L425-444）

```cpp
template <typename Src0T, typename Src1T>
__aicore__ inline void matmul(
    __cbuf__ void* dst, __cbuf__ Src0T* left, __cb__ Src1T* right, __cbuf__ int32_t* bias, uint64_t btBias,
    uint64_t config)
{
    if constexpr (
        (IsSameType<Src0T, int8_t>::value && IsSameType<Src1T, int8_t>::value) ||
        (IsSameType<Src0T, int16_t>::value && IsSameType<Src1T, int8_t>::value) ||
        (IsSameType<Src0T, half>::value && IsSameType<Src1T, half>::value) ||
        (IsSameType<Src0T, float8_e4m3_t>::value && IsSameType<Src1T, float8_e4m3_t>::value) ||
        (IsSameType<Src0T, float8_e4m3_t>::value && IsSameType<Src1T, int8_t>::value)) {
        matmul_to_cbuf(dst, left, right, bias, btBias, config);
    } else if constexpr (IsSameType<Src0T, float8_e4m3_t>::value) {
        matmul_to_cbuf_e4m3s4(dst, left, (__cb__ void*)right, bias, btBias, config);
    } else if constexpr (IsSameType<Src0T, int8_t>::value) {
        matmul_to_cbuf_s8s4(dst, left, (__cb__ void*)right, bias, btBias, config);
    } else {
        matmul_to_cbuf_s4s4(dst, (__cbuf__ void*)left, (__cb__ void*)right, bias, btBias, config);
    }
}
```

| Src0T × Src1T 类型组合 | 硬件指令 |
|------------------------|---------|
| int8×int8, int16×int8, half×half, f8e4m3×f8e4m3, f8e4m3×int8 | `matmul_to_cbuf` |
| float8_e4m3_t × (其他) | `matmul_to_cbuf_e4m3s4` |
| int8_t × (其他) | `matmul_to_cbuf_s8s4` |
| (其他) × (其他) | `matmul_to_cbuf_s4s4` |

#### V1 MmadFixCal — 完整实现（L446-451）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadFixCal(
    __cbuf__ DstT* dst, __cbuf__ Src0T* left, __cb__ Src1T* right, const MmadFixBitModeParams& mmadFixParams)
{
    matmul(dst, left, right, (__cbuf__ int32_t*)get_imm(0), (uint64_t)0, mmadFixParams.GetConfig0());
}
```

#### V3 MmadFixCal — 完整实现（L453-503）

```cpp
template <typename DstT, typename Src0T, typename Src1T>
__aicore__ inline void MmadFixCal(
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

    matmul(dst, left, right, (__cbuf__ int32_t*)get_imm(0), (uint64_t)0, config0);
}
```

#### V2 MmadFixCal — 完整实现（L505-515）

```cpp
template <typename DstT, typename Src0T, typename Src1T, typename BiasT>
__aicore__ inline void MmadFixCal(
    __cbuf__ DstT* dst, __cbuf__ Src0T* left, __cb__ Src1T* right, uint64_t bias,
    const MmadFixBitModeParams& mmadFixParams)
{
    if (mmadFixParams.GetBiasMatrixBroadcast()) {
        matmul(dst, left, right, (__cbuf__ int32_t*)get_imm(0), bias, mmadFixParams.GetConfig0());
    } else {
        matmul(dst, left, right, (__cbuf__ int32_t*)bias, (uint64_t)0, mmadFixParams.GetConfig0());
    }
}
```

#### V4 MmadFixCal — 完整实现（L517-571）

```cpp
template <typename DstT, typename Src0T, typename Src1T, typename BiasT>
__aicore__ inline void MmadFixCal(
    __cbuf__ DstT* dst, __cbuf__ Src0T* left, __cb__ Src1T* right, uint64_t bias, const MmadFixParams& mmadFixParams)
{
    constexpr uint8_t POS_BIAS_MATRIX_INITVAL = 0;
    // ... (同 V3 的 21 个 POS_ 常量定义, L521-541)

    uint64_t config0 = 0;
    // ... (同 V3 的 21 行 config0 位拼接, L543-564)

    if (mmadFixParams.biasMatrixBroadcast) {
        matmul(dst, left, right, (__cbuf__ int32_t*)get_imm(0), bias, config0);
    } else {
        matmul(dst, left, right, (__cbuf__ int32_t*)bias, (uint64_t)0, config0);
    }
}
```

> V4 与 V3 的 config0 构建逻辑完全相同（21 个位域拼接），区别仅在 bias 处理：V4 根据 `biasMatrixBroadcast` 选择 bias 传入方式。

### dav_l300 / dav_l310 / dav_l311 / dav_3510 — 未定义

> 这 4 个版本的目录中**不存在** `kernel_operator_macro_impl.h` 或 `kernel_operator_macro_bitmode_impl.h` 文件。`MmadFix` 接口在 `intf.h` 文件级 `#if 5101||5161||5165||5163` 守卫下，这些架构编译时不可见。

## 代码级实现差异分析

### 实现风格对比

| 特征 | dav_5161 |
|------|----------|
| 接口可见性 | 仅 5101/5161/5165/5163 |
| V1/V2 (BitMode) 实现 | `mmadFixParams.GetConfig0()` 直接获取打包值 |
| V3/V4 (Params) 实现 | 手动构建 64 位 config0（21 个位域拼接） |
| 硬件指令 | `matmul_to_cbuf` / `matmul_to_cbuf_e4m3s4` / `matmul_to_cbuf_s8s4` / `matmul_to_cbuf_s4s4` |
| 类型分派 | `IsSameType<Src0T, ...>` 编译期 `if constexpr` |
| 支持 Src0T 类型 | int8_t, int16_t, half, float8_e4m3_t, 其他(s4s4) |
| 支持 Src1T 类型 | int8_t, half, float8_e4m3_t, 其他(s4s4) |
| AIC 守卫 | 无 |
| bias 处理 | `biasMatrixBroadcast` 决定 bias 传参方式（broadcast 用 btBias，否则用 bias 指针） |
| 无 bias 时 | `get_imm(0)` 作为零 bias 指针，btBias=0 |

### 实现模式总结

**单一实现模式：dav_5161 独占**
- `MmadFix` 是 5101/5161/5165/5163 架构专属接口，从 intf.h 文件级守卫到 base_impl.h 分派均仅限此架构
- 4 个变体共用 `matmul()` 类型分派器，根据 `Src0T × Src1T` 类型组合选择不同的 `matmul_to_cbuf*` 硬件指令
- BitMode 变体（V1/V2）通过 `GetConfig0()` 直接获取预打包的 64 位配置值
- Params 变体（V3/V4）在函数内手动构建 64 位 config0（21 个位域拼接），与 BitMode 的 `MmadFixBitModeConfig0` 位域布局对应
- bias 处理逻辑：`biasMatrixBroadcast=true` 时 bias 地址通过 `btBias` 参数传递，bias 指针用 `get_imm(0)`；`false` 时 bias 地址作为指针传递，btBias=0

### 关键差异点

1. **5101/5161/5165/5163 独占接口** — 与 `Mmad`（使用 `mad()` 指令，3510/l300/l310/l311 支持）不同，`MmadFix` 使用 `matmul_to_cbuf*` 系列指令，仅 5101 架构支持
2. **无跨版本差异** — 4 个架构变体（5101/5161/5165/5163）共用 dav_5161 的单一实现，无版本间差异
3. **BitMode vs Params 双路径** — BitMode 变体通过 `MmadFixBitModeConfig0` 位域结构体预打包，Params 变体在函数内手动拼接，两者位域布局必须一致
4. **matmul 类型分派** — `matmul()` 函数是 5101 独有的类型分派器，支持 int8/int16/half/float8_e4m3 及其组合
5. **bias 广播机制** — 独特的 `biasMatrixBroadcast` 机制：广播模式下 bias 作为标量通过 btBias 传递，非广播模式下 bias 作为指针传递

## 待确认问题

1. **dav_l300/l310/l311/3510 是否需要 MmadFix 支持？** — 当前这些架构完全不可见此接口（intf.h 文件级守卫）。如需支持，需创建 `kernel_operator_macro_impl.h` 和 `kernel_operator_macro_bitmode_impl.h` 并扩展守卫。但需确认这些架构的硬件是否支持 `matmul_to_cbuf*` 系列指令。
2. **V3/V4 config0 手动拼接与 BitMode 位域布局一致性** — V3/V4 在函数内手动拼接 21 个位域，V1/V2 通过 `MmadFixBitModeConfig0` 位域结构体。两者位域布局是否完全一致？是否有机制保证同步？
3. **matmul() 的 s4s4 fallback 路径** — 当 Src0T 和 Src1T 不匹配已知类型组合时，fallback 到 `matmul_to_cbuf_s4s4`，将 left 和 right 都 cast 为 `void*`。这是否意味着支持任意 4-bit 类型的矩阵乘法？还是仅作为兜底路径？
4. **get_imm(0) 的语义** — 无 bias 变体使用 `get_imm(0)` 作为 bias 指针。这是否返回一个硬件零值指针？在所有 5101 子架构上行为是否一致？
