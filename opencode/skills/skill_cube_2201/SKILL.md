---
name: cube_c_api_2201
description: 通过AscendC C API编程语言编写Cube核（矩阵计算核心）2201架构的测试样例。样例仅考虑单核上的数据搬运、矩阵计算、搬出，且除了必要的搬运操作外，每次只测试单独的一个API接口。本技能仅考虑NPU架构为2201场景的AscendC Cube核C API测试样例编写。当用户提到2201 Cube核C API样例时使用。
---

# Ascend 2201 NPU Cube核 C API 技能文档

## 概述

2201 Cube核 C API（适用于 Atlas A3 Train/Infer, Atlas A2 Train/Infer）为 NPU 矩阵计算核心提供底层的、面向硬件的编程接口。

本文档涵盖 2201 架构 Cube核 C API（即 `npu_arch_2201`）。

## 与 3510 架构的详细差异

> 基于 `impl/c_api/instr_impl/npu_arch_2201/` vs `impl/c_api/instr_impl/npu_arch_3510/` 对比

### Impl 文件数量对照

| 类别 | 2201 独有 | 3510 独有 | 共有 | impl总数 |
|------|----------|----------|------|---------|
| `cube_compute_impl/` | **7** | **2** | 2 | 9+4=13 |
| `cube_datamove_impl/` | **16** | **12** | ~14 | 29+26=55 |

### 2201 独有 Compute API (7个)

| API | 说明 |
|-----|------|
| `asc_mmad_s4` / `_sync` | 4-bit (int4b_t) 矩阵乘加 |
| `asc_mmad_sparse` / `_sync` | 稀疏矩阵乘加(A稀疏，B稠密) |
| `asc_set_mmad_direction_m` | MMAD 优先 M 方向 |
| `asc_set_mmad_direction_n` | MMAD 优先 N 方向 |
| `asc_set_fp32_mode` | FP32 模式（不做舍入） |
| `asc_enable_hf32` | 使能 HF32 |
| `asc_enable_hf32_trans` | 使能 HF32 转置 |
| `asc_get_l0c2gm_config` | 获取 L0C→GM 配置信息 |

### 3510 独有 Compute API (2个)

| API | 说明 |
|-----|------|
| `asc_mmad_mx` / `_sync` | MX 格式矩阵乘加（fp4/fp8 类型 + 放缩） |
| `asc_set_l0c2gm_nz2nd` | 设置 NZ→ND 格式转换参数 |

### 2201 独有 Datamove API (16个)

| API | 说明 |
|-----|------|
| `asc_copy_gm2l0a` / `_sync` | GM→L0A 直连搬运 |
| `asc_copy_gm2l0b` / `_sync` | GM→L0B 直连搬运 |
| `asc_copy_l12l0c` / `_sync` | L1→L0C 线性搬运（**关键：2201 可用，3510 不可用**） |
| `asc_copy_l12gm` / `_sync` | L1→GM 线性搬运（**关键：2201 可用回环路，3510 不可用**） |
| `asc_copy_l12l0a_trans` / `_sync` | L1→L0A 转置搬运 |
| `asc_copy_l12l0b_trans` / `_sync` | L1→L0B 转置搬运 |
| `asc_copy_l12l0b_sparse` | L1→L0B 稀疏矩阵搬运 |
| `asc_copy_l12l0a_2d` / `_3d` | L1→L0A（独立 2D/3D 重载） |
| `asc_copy_l12l0b_2d` / `_3d` | L1→L0B（独立 2D/3D 重载） |
| `asc_load_image_to_cbuf` | 图像数据搬运到 L1 |
| `asc_copy_gm2l1_pad1` / `_pad2` | GM→L1（PAD_MODE1/2） |
| `asc_set_l0c_copy_params` | 设置 L0C 搬运参数 |
| `asc_set_l13d_padding` | 3D 搬运 Pad 填充值 |
| `asc_set_l13d_size` | L1 Buffer 边界值 |
| `asc_fill_l0a` / `asc_fill_l0b` | L0A/L0B 初始化（2201 独立 impl） |

### 3510 独有 Datamove API (12个)

| API | 说明 |
|-----|------|
| `asc_copy_gm2l1_align` / `_sync` | GM→L1 带 padding 的劈裂（burst）搬运 |
| `asc_copy_gm2l1_dn2nz` | GM→L1（DN→NZ 格式转换） |
| `asc_copy_l0c2ub` / `_sync` | L0C→UB 跨核搬运（Cube→Vector） |
| `asc_copy_l12ub` / `_sync` | L1→UB 跨核搬运 |
| `asc_copy_l12fb_v2` / `_sync` | L1→Fixpipe Buffer（v2 版本） |
| `asc_copy_l12l0a_mx` / `_sync` | L1→L0A MX 放缩数据搬运 |
| `asc_copy_l12l0b_mx` / `_sync` | L1→L0B MX 放缩数据搬运 |
| `asc_set_gm2l1_loop_size` | GM→L1 循环体次数 |
| `asc_set_gm2l1_loop1_stride` | GM→L1 内循环步长 |
| `asc_set_gm2l1_loop2_stride` | GM→L1 外循环步长 |
| `asc_set_gm2l1_pad` | GM→L1 Pad 填充模式 |
| `asc_copy_l12bt` (3510 impl) | L1→BiasTable（3510 独立 impl） |

### 关键签名差异

#### `asc_copy_gm2l1` typed 重载

| | 2201 | 3510 |
|--|------|------|
| 参数 | `(base_idx, repeat, src_stride, dst_stride)` | `(m_start, k_start, dst_stride, m_step, k_step, decomp, cache)` |
| 语义 | 分形索引搬运 (分形ID × 重复次数) | 2D 分形坐标搬运 (M/K 起止 + 步长) |

#### `asc_mmad` / `asc_mmad_sync`

| | 2201 | 3510 |
|--|------|------|
| 第7参数 | `k_direction_align` (bool) | `disable_gemv` (bool) |
| 含义 | true=右矩阵N方向对齐 | false=启用GEMV模式(left_height=1) |
| fp8/fp4 重载 | ❌ | ✅ `fp8_e4m3fn_t`, `fp4x2_*` |

#### `asc_copy_l0c2gm`

| | 2201 (9 参数) | 3510 (21 参数) |
|--|---------------|---------------|
| 参数 | n_size, m_size, dst_stride, src_stride, **unit_flag_mode**, **quant_pre**, **relu_pre**, channel_split, nz2nd_en | n_size, m_size, dst_stride, src_stride, **l2_cache_ctl**, clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre, split_en, NZ2ND_en, **quant_post**, **relu_post**, **clip_relu_post**, **eltwise_op**, **eltwise_antq_en**, **C0_pad_en**, broadcast_en, NZ2DN_en |
| 后量化 | ❌ | ✅ quant_post 链 |

### 数据通路差异

| 搬运路径 | 2201 | 3510 |
|----------|------|------|
| L1→GM（直接回环） | ✅ `asc_copy_l12gm` | ❌ 不可用 |
| L1→L0C（线性中转） | ✅ `asc_copy_l12l0c` | ❌ 不可用 |
| GM→L0A/L0B（直连） | ✅ `asc_copy_gm2l0a/b` | ❌ 不可用 |
| L0C→UB（跨核） | ❌ | ✅ `asc_copy_l0c2ub` |
| L1→UB（跨核） | ❌ | ✅ `asc_copy_l12ub` |
| Burst 对齐搬运 | ❌ | ✅ `asc_copy_gm2l1_align` |
| Loop/Stride/Pad 控制 | ❌ | ✅ `asc_set_gm2l1_loop_*` |

### 数据类型支持差异

| 类型/特性 | 2201 | 3510 |
|-----------|------|------|
| half/bf16/float/int8 MMAD | ✅ | ✅ |
| 4-bit (int4b_t) MMAD | ✅ `asc_mmad_s4` | ❌ |
| fp8/fp4 MX 格式 MMAD | ❌ | ✅ `asc_mmad_mx` |
| 稀疏矩阵 A | ✅ `asc_mmad_sparse` | ❌ |

## 重要限制

- Cube核的函数标识符为 `__cube__`，对应矩阵计算专用核心
- Cube核仅能访问 L0A (`__ca__`)、L0B (`__cb__`)、L0C (`__cc__`)、L1 (`__cbuf__`)，没有 UB
- 头文件: `#include "c_api/asc_simd.h"`
- 2201 架构: `cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201`
- **2201 的 `asc_mmad` 使用 `k_direction_align` 参数**（true=K方向对齐，false=不使用）
- **2201 的 `asc_copy_l0c2gm` 仅有 9 参数版本**（无 quant_post、broadcast_en、NZ2DN_en 等）

---

## API 分类概览

### Cube 计算 (cube_compute.h, 2201 段: lines 32-128)

| API | 流水线 | 说明 |
|-----|--------|------|
| `asc_mmad` | PIPE_M | 矩阵乘加 (half/bf16/int8/float→float/int32) |
| `asc_mmad_sync` | PIPE_M | 矩阵乘加（同步版本） |
| `asc_mmad_s4` | PIPE_M | 矩阵乘加（4-bit int4b_t 数据类型） |
| `asc_mmad_s4_sync` | PIPE_M | 矩阵乘加（4-bit，同步版本） |
| `asc_mmad_sparse` | PIPE_M | 稀疏矩阵乘加（A 稀疏，B 稠密） |
| `asc_mmad_sparse_sync` | PIPE_M | 稀疏矩阵乘加（同步版本） |
| `asc_set_mmad_direction_m` | PIPE_S | 设置 MMAD 优先 M 方向计算 |
| `asc_set_mmad_direction_n` | PIPE_S | 设置 MMAD 优先 N 方向计算 |
| `asc_enable_hf32_trans` | PIPE_S | 使能 HF32 转置 |
| `asc_enable_hf32` | PIPE_S | 使能 HF32 |
| `asc_set_fp32_mode` | PIPE_S | 设置 MMAD 的 FP32 模式（不做舍入） |
| `asc_get_l0c2gm_relu` | PIPE_S | 获取 relu 设置 |
| `asc_get_l0c2gm_unitflag` | PIPE_S | 获取 unit_flag 设置 |
| `asc_get_l0c2gm_prequant` | PIPE_S | 获取 prequant 设置 |
| `asc_set_l0c2gm_config` | PIPE_S | 设置 L0C→GM 随路量化参数（common 段，2201 也适用） |

### Cube 数据搬运 (cube_datamove.h, 2201 段: lines 224-841)

| API | 流水线 | 说明 |
|-----|--------|------|
| `asc_copy_gm2l1` | PIPE_MTE2 | GM→L1（线性/2D 搬运，void* 重载） |
| `asc_copy_gm2l1_nd2nz` | PIPE_MTE2 | GM→L1（ND→NZ 格式转换搬运） |
| `asc_copy_gm2l1_pad1` | PIPE_MTE2 | GM→L1（PAD_MODE1） |
| `asc_copy_gm2l1_pad2` | PIPE_MTE2 | GM→L1（PAD_MODE2） |
| `asc_copy_l12l0a` (2D/3D) | PIPE_MTE1 | L1→L0A（分形搬运，支持转置） |
| `asc_copy_l12l0b` (2D/3D) | PIPE_MTE1 | L1→L0B（分形搬运，支持转置） |
| `asc_copy_l12l0a_trans` | PIPE_MTE1 | L1→L0A（转置搬运） |
| `asc_copy_l12l0b_trans` | PIPE_MTE1 | L1→L0B（转置搬运） |
| `asc_copy_l12l0c` | PIPE_MTE1 | L1→L0C（线性搬运，支持类型转换） |
| `asc_copy_l0c2gm` | PIPE_FIX | L0C→GM（9 参数: 支持量化、relu、格式转换） |
| `asc_copy_l0c2l1` | PIPE_FIX | L0C→L1（支持量化、relu、格式转换） |
| `asc_copy_l12gm` | PIPE_MTE1 | L1→GM（线性搬运） |
| `asc_copy_gm2l0a` | PIPE_MTE2 | GM→L0A（分形搬运） |
| `asc_copy_gm2l0b` | PIPE_MTE2 | GM→L0B（分形搬运） |
| `asc_load_image_to_cbuf` | PIPE_MTE2 | 图像数据搬运到 L1 |
| `asc_fill_l0a` / `asc_fill_l0b` / `asc_fill_l1` | PIPE_MTE1 | 缓冲区初始化（common 段） |
| `asc_set_l0c2gm_lrelu_alpha` | PIPE_S | 设置 LReLU alpha（common 段） |
| `asc_set_l13d_fmatrix` / `_b` | PIPE_S | 3D 搬运 Feature map 属性（common 段） |
| `asc_set_l13d_rpt` | PIPE_S | Load3Dv2 repeat 参数（common 段） |
| `asc_set_l13d_size` | PIPE_S | L1 Buffer 边界值（2201 段） |
| `asc_set_l13d_padding` | PIPE_S | 3D 搬运 Pad 填充值（2201 段） |

---

## 快速入门

### Kernel 函数结构 (2201)

```c
__cube__ __global__ __aicore__ void cube_mmad_2201(
    __gm__ half* gm_a, __gm__ half* gm_b, __gm__ float* gm_c)
{
    asc_init();

    __cbuf__ half l1_a[256];
    __cbuf__ half l1_b[256];
    __ca__ half l0a_a[256];
    __cb__ half l0b_b[256];
    __cc__ float l0c_c[256];

    // 步骤1: GM -> L1 (2D搬运)
    asc_copy_gm2l1_sync(l1_a, gm_a, 0, 0, 0, 1, 1, 0, 0);
    asc_copy_gm2l1_sync(l1_b, gm_b, 0, 0, 0, 1, 1, 0, 0);

    // 步骤2: L1 -> L0A, L0B
    asc_copy_l12l0a_sync(l0a_a, l1_a, 0, 0, 1, 1, 0, 0);
    asc_copy_l12l0b_sync(l0b_b, l1_b, 0, 0, 1, 1, 0, 0);

    // 步骤3: MMAD (2201 使用 k_direction_align)
    constexpr uint16_t left_height = 16;
    constexpr uint16_t n_dim = 16;
    constexpr uint16_t right_width = 16;
    constexpr uint8_t unit_flag = 0;
    constexpr bool k_direction_align = true;   // 2201 特有参数
    constexpr bool c_matrix_source = false;
    constexpr bool c_matrix_init_val = true;

    asc_mmad_sync(l0c_c, l0a_a, l0b_b,
                  left_height, n_dim, right_width,
                  unit_flag, k_direction_align,
                  c_matrix_source, c_matrix_init_val);

    // 步骤4: L0C -> GM (2201 9参数版本)
    asc_copy_l0c2gm_sync(
        (__gm__ float*)gm_c, l0c_c,
        1/*n_size*/, 1/*m_size*/,
        0/*dst_stride*/, 0/*src_stride*/,
        0/*unit_flag*/, 0/*NoQuant*/, 0/*no relu*/,
        false/*split*/, false/*NZ2ND*/);
}
```

### CMakeLists.txt (2201)

```cmake
cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(cube_example cube_example.asc)
target_compile_options(cube_example PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-2201>
)
```

---

## asc_mmad 参数详解 (2201)

```
asc_mmad(__cc__ float* c_matrix,
         __ca__ half* a_matrix,
         __cb__ half* b_matrix,
         uint16_t left_height,      // A 行数 (M)
         uint16_t n_dim,            // A 列数/B 行数 (K)
         uint16_t right_width,      // B 列数 (N)
         uint8_t unit_flag,         // 细粒度并行控制
         bool k_direction_align,    // 2201: K 方向对齐
         bool c_matrix_source,      // C 矩阵来源
         bool c_matrix_init_val)    // C 矩阵清零
```

### k_direction_align 说明 (2201 替代 3510 的 disable_gemv)

| 取值 | 含义 |
|------|------|
| true | 右矩阵宽度（N方向）按规则对齐 |
| false | 不进行 N 方向对齐 |

### unit_flag

| 取值 | 含义 |
|------|------|
| 0 | 关闭 unit_flag |
| 2 | 使能，硬件不自动关闭（中间分形） |
| 3 | 使能，硬件自动关闭（最后分形） |

### c_matrix_source 和 c_matrix_init_val

| c_matrix_init_val | c_matrix_source | 效果 |
|:---:|:---:|------|
| true | 任意 | C 矩阵初始化为全 0 |
| false | true | C 初始值来源于 BiasTable |
| false | false | C 初始值来源于 L0C 已有数据 |

### 支持的数据类型重载

| L0A 类型 | L0B 类型 | L0C 类型 | 说明 |
|----------|----------|----------|------|
| half | half | float | 半精度浮点 |
| bfloat16_t | bfloat16_t | float | Brain 浮点 |
| float | float | float | 单精度浮点 |
| int8_t | int8_t | int32_t | 8-bit 整数 |
| int4b_t | int4b_t | int32_t | 4-bit 整数 (asc_mmad_s4) |

---

## asc_copy_l0c2gm 参数详解 (2201 9参数版)

```
asc_copy_l0c2gm(__gm__ float* dst, __cc__ float* src,
    uint16_t n_size,           // NZ 矩阵 N 方向分形数 [1,4095]
    uint16_t m_size,           // NZ 矩阵 M 方向分形数 [1,65535]
    uint32_t dst_stride_dst_d, // 目的 ND 矩阵间距
    uint16_t src_stride,       // 源 NZ 矩阵 Z 方向步长，单位 C0_Size
    uint8_t unit_flag_mode,    // unit_flag 模式
    uint64_t quant_pre,        // 预处理量化 (NoQuant=0, DEQF16, F322BF16 等)
    uint8_t relu_pre,          // ReLU 使能
    bool channel_split,        // Channel split
    bool nz2nd_en)             // NZ→ND 格式转换
```

### 常见量化模式 (quant_pre)

| 枚举值 | 含义 |
|--------|------|
| NoQuant (0) | 不量化 |
| F322BF16 | float→bfloat16 |
| F322F16 | float→half |
| DEQF16 | int32→half |

---

## 矩阵分形 (2201)

与 3510 一致:

| 数据类型 | L0A 分形 | L0B 分形 | L0C 分形 |
|----------|---------|---------|---------|
| 4-bit | 16×64 | 64×16 | 16×16 |
| 8-bit | 16×32 | 32×16 | 16×16 |
| 16-bit | 16×16 | 16×16 | 16×16 |
| 32-bit | 16×16 | 16×16 | 16×16 |

---

## 测试方法

| API 类型 | 验证方式 |
|----------|---------|
| 计算类 | MMAD → L0C→GM，与 CPU 对比 |
| 搬运类 | 搬入→搬出回环比对 (如 GM→L1→L0C→GM) |
| 2201 特有能力 | L1→GM 可用，可直接做 GM→L1→GM 回环 |

---

## 2201 特有测试路径

受益于 `asc_copy_l12gm`（L1→GM）和 `asc_copy_l12l0c`（L1→L0C）在 2201 可用:

```
搬运类回环路径:
  GM→L1 (API under test) → L1→GM (asc_copy_l12gm) → 比对
  GM→L1 → L1→L0C (asc_copy_l12l0c) → L0C→GM (asc_copy_l0c2gm) → 比对
```

详见 [lessons-learned.md](lessons-learned.md) 和 3510 技能文档的踩坑记录。

# 测试方法
1. 不同类型api测试方法

   | api类型 | 对应头文件           | 测试方法                              |
       |-------|-----------------|-----------------------------------|
   | 搬运类   | cube_datamove.h | 测试搬入搬出后的数据是否一致，无需计算，只要搬入和搬出然后比较数据 |
   | 计算类   | cube_compute.h  | 测试和cpu计算真值是否一致                    |
2. 测试完备性  
   对于接口所覆盖的数据类型和不同的重载版本均要覆盖
3. 对于当前CANN版本不支持的数据类型，总结并保存到测试报告中，并把对应的测试用例标记出来
4. 用例可读性：测试用例代码要清晰明了，关键时候把每一步对应的含义注释清楚，调用接口处，可以把对应的接口签名注释出来。