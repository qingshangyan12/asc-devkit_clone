# 案例分析：Macro 寄存器设置接口跨版本适配

## 接口概述

本文分析 `kernel_operator_macro_intf.h` 中 5 组寄存器设置接口（共 8 个声明），这些接口用于配置 Cube/Fixpipe 硬件寄存器：

| 接口 | 行号 | 变体 | 参数类型 | 说明 |
|------|------|------|---------|------|
| SetFixPipeReluAlpha | L108 | V1 (BitMode) | `FixPipeReluAlphaBitModeParams` | 设置 ReLU Alpha 寄存器（LUT 模式 + 预设值） |
| SetFixPipeReluAlpha | L109 | V2 (Params) | `FixPipeReluAlphaParams` | 同上，字段式参数 |
| SetFixpipePostQuantFlag | L116 | V1 | `uint64_t config` | 设置 Post Quant 标志寄存器 |
| SetFixpipeEltAntiq | L123 | V1 | `uint64_t config` | 设置_eltwise 抗量化参数寄存器 |
| SetMatrixPara | L146 | V1 (BitMode) | `MatrixParaBitModeParams` | 设置矩阵参数（M/K/groupK） |
| SetMatrixPara | L147 | V2 (Params) | `MatrixParaParams` | 同上，字段式参数 |
| SetMmadStridePara | L154 | V1 (BitMode) | `MmadStrideParaBitModeParams` | 设置 Mmad 步进参数（loop4 src/dst stride） |
| SetMmadStridePara | L155 | V2 (Params) | `MmadStrideParaParams` | 同上，字段式参数 |

### 文件级 arch 守卫

所有声明均位于 `kernel_operator_macro_intf.h` 文件级守卫内：

```cpp
// intf.h L28-29
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
```

> **无单独的 per-API 或 per-变体 arch 守卫。** 所有 8 个声明仅受文件级守卫保护，对 5101/5161/5165/5163 全部可见。

### 参数结构体定义（`kernel_struct_macro.h`）

| 结构体 | 行号 | 字段 | arch 守卫 |
|--------|------|------|----------|
| `FixPipeReluAlphaParams` | L157-166 | `lut_mode_t lut` (enum: Gelu/Silu/Sigmoid/Tanh), `uint32_t preValue` | 文件级 L27-28（5101\|\|5161\|\|5165\|\|5163） |
| `MatrixParaParams` | L193-203 | `uint16_t m`, `uint16_t k`, `uint16_t groupK` | 同上 |
| `MmadStrideParaParams` | L205-214 | `uint16_t loop4SrcStride`, `uint16_t loop4DstStride` | 同上 |

> BitMode 变体（`FixPipeReluAlphaBitModeParams` 等）定义在 `kernel_operator_macro_bitmode_intf.h` 中，内部 union 含 `uint64_t config0`，通过 `GetConfig0()` 返回预打包的 64 位寄存器值。该文件同样受文件级 `5101||5161||5165||5163` 守卫保护。

## 调用链

### 统一调用链结构

所有 5 组接口共享相同的四层调用链模式：

```
intf.h（声明，文件级守卫 5101||5161||5165||5163）
  → intf_impl.h（公共包装，文件级守卫 同上）
    → base_impl.h（共享实现 Impl，文件级守卫 同上，#include dav_5161/macro_impl.h L30-33）
      → dav_5161/macro_impl.h（版本特定实现 Cal，文件级守卫 同上）
        → 硬件指令（set_m_relu_alpha / set_m_quant_post / set_m_elt_antiq_para / set_matrix_para / set_cube_stride_para）
```

### 各接口调用链详情

| 接口 | intf.h | intf_impl.h (包装) | base_impl.h (Impl) | dav_5161/macro_impl.h (Cal) | 硬件指令 |
|------|--------|-------------------|-------------------|---------------------------|---------|
| SetFixPipeReluAlpha V1 | L108 | L82-85 | L86-89 | L180-183 | `set_m_relu_alpha` |
| SetFixPipeReluAlpha V2 | L109 | L87 | L91 | L185-195 | `set_m_relu_alpha` |
| SetFixpipePostQuantFlag | L116 | L89 | L93 | L197 | `set_m_quant_post` |
| SetFixpipeEltAntiq | L123 | L91 | L95 | L199 | `set_m_elt_antiq_para` |
| SetMatrixPara V1 | L146 | L101 | L105 | L239 | `set_matrix_para` |
| SetMatrixPara V2 | L147 | L103 | L107 | L241-253 | `set_matrix_para` |
| SetMmadStridePara V1 | L154 | L105 | L109-112 | L255-258 | `set_cube_stride_para` |
| SetMmadStridePara V2 | L155 | L107 | L114 | L260-270 | `set_cube_stride_para` |

### 共享层分发逻辑（base_impl.h）

base_impl.h 仅 `#include "dav_5161/kernel_operator_macro_impl.h"`（L30-33），无 `#elif` 分支：

```cpp
// base_impl.h L30-33
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 5101 || __NPU_ARCH__ == 5161 || __NPU_ARCH__ == 5165 || __NPU_ARCH__ == 5163)
#include "dav_5161/kernel_operator_macro_impl.h"
#endif
```

> **无版本分发逻辑。** 所有 4 个架构（5101/5161/5165/5163）共享 dav_5161 的同一套实现。dav_3510/dav_l300/dav_l310/dav_l311 目录中无 `kernel_operator_macro_impl.h` 文件，macro API 对这些架构完全不可见。

## 支持矩阵

| 接口 | 变体 | 3003 (dav_l300) | 3103 (dav_l310) | 3113 (dav_l311) | 5101 (dav_5161) | 5161 (dav_5161) | 5165 (dav_5161) | 5163 (dav_5161) | 3510 (dav_3510) |
|------|------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|-----------------|
| SetFixPipeReluAlpha | V1 (BitMode) | N/A | N/A | N/A | **完整** | **完整** | **完整** | **完整** | N/A |
| SetFixPipeReluAlpha | V2 (Params) | N/A | N/A | N/A | **完整** | **完整** | **完整** | **完整** | N/A |
| SetFixpipePostQuantFlag | V1 | N/A | N/A | N/A | **完整** | **完整** | **完整** | **完整** | N/A |
| SetFixpipeEltAntiq | V1 | N/A | N/A | N/A | **完整** | **完整** | **完整** | **完整** | N/A |
| SetMatrixPara | V1 (BitMode) | N/A | N/A | N/A | **完整** | **完整** | **完整** | **完整** | N/A |
| SetMatrixPara | V2 (Params) | N/A | N/A | N/A | **完整** | **完整** | **完整** | **完整** | N/A |
| SetMmadStridePara | V1 (BitMode) | N/A | N/A | N/A | **完整** | **完整** | **完整** | **完整** | N/A |
| SetMmadStridePara | V2 (Params) | N/A | N/A | N/A | **完整** | **完整** | **完整** | **完整** | N/A |

> **N/A** = 该架构不受文件级 arch 守卫保护，接口声明不可见，无实现文件。  
> **完整** = 有完整实现，直接调用硬件寄存器设置指令。  
> 5101/5161/5165/5163 四个架构共享 dav_5161 目录的同一套实现，无 per-arch 差异化。

## 各版本原始代码

### dav_5161（5101/5161/5165/5163 共享实现）

> 以下所有函数均位于 `impl/basic_api/dav_5161/kernel_operator_macro_impl.h`，受文件级守卫 L25-26 保护。以下为完整函数体。

#### SetFixPipeReluAlphaCal — V1 (BitMode)（L180-183）

```cpp
__aicore__ inline void SetFixPipeReluAlphaCal(const FixPipeReluAlphaBitModeParams& params)
{
    set_m_relu_alpha(params.GetConfig0());
}
```

#### SetFixPipeReluAlphaCal — V2 (Params)（L185-195）

```cpp
__aicore__ inline void SetFixPipeReluAlphaCal(const FixPipeReluAlphaParams& params)
{
    constexpr uint8_t POS_LUT = 0;
    constexpr uint8_t POS_PRE_VALUE = 13;

    uint64_t reluAlpha = 0;
    reluAlpha |= (uint64_t)(static_cast<uint8_t>(params.lut)) << POS_LUT;
    reluAlpha |= (uint64_t)(params.preValue) << POS_PRE_VALUE;

    set_m_relu_alpha(reluAlpha);
}
```

#### SetFixpipePostQuantFlagCal（L197）

```cpp
__aicore__ inline void SetFixpipePostQuantFlagCal(uint64_t config) { set_m_quant_post(config); }
```

#### SetFixpipeEltAntiqCal（L199）

```cpp
__aicore__ inline void SetFixpipeEltAntiqCal(uint64_t config) { set_m_elt_antiq_para(config); }
```

#### SetMatrixParaCal — V1 (BitMode)（L239）

```cpp
__aicore__ inline void SetMatrixParaCal(const MatrixParaBitModeParams& params) { set_matrix_para(params.GetConfig0()); }
```

#### SetMatrixParaCal — V2 (Params)（L241-253）

```cpp
__aicore__ inline void SetMatrixParaCal(const MatrixParaParams& params)
{
    constexpr uint8_t POS_M = 0;
    constexpr uint8_t POS_K = 16;
    constexpr uint8_t POS_GROUP_K = 32;

    uint64_t matrixPara = 0;
    matrixPara |= (uint64_t)(params.m) << POS_M;
    matrixPara |= (uint64_t)(params.k) << POS_K;
    matrixPara |= (uint64_t)(params.groupK) << POS_GROUP_K;

    set_matrix_para(matrixPara);
}
```

#### SetMmadStrideParaCal — V1 (BitMode)（L255-258）

```cpp
__aicore__ inline void SetMmadStrideParaCal(const MmadStrideParaBitModeParams& params)
{
    set_cube_stride_para(params.GetConfig0());
}
```

#### SetMmadStrideParaCal — V2 (Params)（L260-270）

```cpp
__aicore__ inline void SetMmadStrideParaCal(const MmadStrideParaParams& params)
{
    constexpr uint8_t POS_LOOP4_SRC_STRIDE = 0;
    constexpr uint8_t POS_LOOP4_DST_STRIDE = 16;

    uint64_t stridePara = 0;
    stridePara |= (uint64_t)(params.loop4SrcStride) << POS_LOOP4_SRC_STRIDE;
    stridePara |= (uint64_t)(params.loop4DstStride) << POS_LOOP4_DST_STRIDE;

    set_cube_stride_para(stridePara);
}
```

### dav_3510 / dav_l300 / dav_l310 / dav_l311

> 这些目录中**不存在** `kernel_operator_macro_impl.h` 文件。macro API 的声明、包装、共享实现均受 `5101||5161||5165||5163` 文件级守卫保护，对这些架构完全不可见。无任何相关代码。

## 代码级实现差异分析

### 实现模式总结

所有 5 组接口遵循统一的实现模式：

| 模式 | BitMode 变体 (V1) | Params 变体 (V2) |
|------|-------------------|-----------------|
| 函数体 | `set_xxx(params.GetConfig0())` — 单行，直接透传 | 定义 `POS_*` 位偏移常量 → 按位拼接 `uint64_t` → `set_xxx(value)` |
| 参数来源 | 用户预打包的 64 位值 | 结构体字段 |
| 灵活性 | 用户自行控制全部 bit | 框架按固定 bit 布局打包 |

### 风格对比表

| 接口 | 硬件指令 | BitMode 实现 | Params 实现 | 位偏移常量数 | 类型转换 |
|------|---------|-------------|------------|------------|---------|
| SetFixPipeReluAlpha | `set_m_relu_alpha` | `GetConfig0()` 透传 | POS_LUT(0), POS_PRE_VALUE(13) | 2 | `static_cast<uint8_t>(params.lut)` |
| SetFixpipePostQuantFlag | `set_m_quant_post` | N/A（无 BitMode 变体） | N/A（无 Params 变体，直接 uint64_t） | 0 | 无 |
| SetFixpipeEltAntiq | `set_m_elt_antiq_para` | N/A | N/A | 0 | 无 |
| SetMatrixPara | `set_matrix_para` | `GetConfig0()` 透传 | POS_M(0), POS_K(16), POS_GROUP_K(32) | 3 | 无 |
| SetMmadStridePara | `set_cube_stride_para` | `GetConfig0()` 透传 | POS_LOOP4_SRC_STRIDE(0), POS_LOOP4_DST_STRIDE(16) | 2 | 无 |

### 关键差异点

1. **变体数量差异**：
   - SetFixPipeReluAlpha / SetMatrixPara / SetMmadStridePara 有 BitMode + Params 两个变体
   - SetFixpipePostQuantFlag / SetFixpipeEltAntiq 仅有一个 `uint64_t config` 变体（等价于 BitMode 路径，但参数直接由用户传入而非通过 BitModeParams 封装）

2. **位拼接模式统一**：所有 Params 变体使用相同的 `constexpr POS_*` + `|=` 位拼接模式，无版本差异

3. **无类型检查**：与 LoadDataWithTranspose 等接口不同，这些寄存器设置接口不做 `SupportType<>` / `static_assert` 类型检查，因为参数均为整数类型而非 tensor

4. **无 per-arch 差异**：5101/5161/5165/5163 共享完全相同的实现，不像 Mmad/MmadFix/MmadMxFix 那样需要 per-arch 区分

5. **与 fixpipe 层接口的区别**：`SetFixpipePostQuantFlag`（macro 层，L116）与 `SetFixpipePreQuantFlag`（fixpipe 层，`kernel_operator_fixpipe_intf.h:69`）是不同接口：
   - macro 层 `SetFixpipePostQuantFlag` → `set_m_quant_post()` （dav_5161/macro_impl.h L197）
   - fixpipe 层 `SetFixpipePreQuantFlag` → `set_quant_pre()` / `set_m_quant_pre()`（各 dav fixpipe_impl.h）
   - macro 层仅 5101/5161/5165/5163 可见；fixpipe 层全版本可见

## 待确认问题

1. **3003/3103/3113/3510 是否需要这些 macro 寄存器设置接口？**
   - 这些接口用于设置 Cube/Fixpipe 硬件寄存器（`set_m_relu_alpha`、`set_m_quant_post`、`set_matrix_para` 等）
   - 如果这些架构的硬件有对应的寄存器，可能需要适配（创建 macro_impl.h 并实现 Cal 函数）
   - 如果硬件不支持这些寄存器，则 N/A 是合理的
   - **需确认**：3003/3103/3113/3510 的硬件是否包含 `set_m_relu_alpha` / `set_m_quant_post` / `set_m_elt_antiq_para` / `set_matrix_para` / `set_cube_stride_para` 等指令

2. **5101 是否真正支持所有 5 组接口的硬件指令？**
   - 当前 5101 与 5161/5165/5163 共享同一实现，无 per-arch 桩函数
   - 已知 5101 不支持 `matmul_mx_to_cbuf` 类接口（MmadMxFix），但 `set_m_relu_alpha` 等寄存器设置指令是否支持？
   - 如果 5101 硬件不支持某些 `set_*` 指令，需要对 5101 添加 `ASCENDC_ASSERT(false, ...)` 桩函数
   - **需确认**：5101 硬件是否支持 `set_m_relu_alpha`、`set_m_quant_post`、`set_m_elt_antiq_para`、`set_matrix_para`、`set_cube_stride_para` 指令

3. **SetFixpipePostQuantFlag / SetFixpipeEltAntiq 是否需要 BitMode 变体？**
   - 这两个接口仅接受 `uint64_t config`，用户需自行打包 bit 布局
   - 其他接口都有 BitMode 变体（通过 `BitModeParams` 类封装），提供更友好的 API
   - 是否是有意省略 BitMode 变体（因为这两个寄存器的 bit 布局尚未稳定/不对外公开）？
