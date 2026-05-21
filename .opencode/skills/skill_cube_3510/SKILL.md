---
name: cube_c_api
description: 通过AscendC C API编程语言编写Cube核（矩阵计算核心）的测试样例。样例仅考虑单核上的数据搬运、矩阵计算、搬出，且除了必要的搬运操作外，每次只测试单独的一个API接口。本技能仅考虑NPU架构为3510场景的AscendC Cube核C API测试样例编写。当用户提到Cube核C API样例或矩阵计算C API时，应使用此技能。
---

# Ascend 3510 NPU Cube核 C API 技能文档

## 概述

3510 Cube核 C API（适用于 Ascend 950PR/950DT）为 NPU 矩阵计算核心提供底层的、面向硬件的编程接口。
这些 API 是对编译器 CCE（Cube Compute Engine）内部指令的薄封装，提供了 C 语言编程体验，支持数组分配内存，可直接控制
矩阵数据搬运、矩阵乘加计算。Cube核（AIC核）专门处理矩阵乘加运算，使用的缓冲区与Vector核不同。

本文档涵盖 3510 架构 Cube核 C API（即 `npu_arch_3510`），兼容 Ascend 950PR 和 Ascend 950DT 硬件。

---

## 重要限制

- Cube核的函数标识符为 `__cube__`，对应矩阵计算专用核心
- Cube核仅能访问 L0A Buffer (`__ca__`)、L0B Buffer (`__cb__`)、L0C Buffer (`__cc__`)、L1 Buffer (`__cbuf__`)，没有 UB
- Cube核只能使用 `__aicore__` 级别的函数，不支持 `__simd_vf__` 级别的寄存器编程
- Cube核使用 `cube_compute.h` 和 `cube_datamove.h` 头文件中的API
- 数据通路为: GM -> L1 -> L0A/L0B -> Cube MMAD -> L0C -> GM（或 L0C -> L1 -> GM）
- 矩阵数据在内存中以分形（fractal, NZ格式）存储，对每个维度需按分形大小对齐
- 3510架构中，`asc_mmad` 的950专用重载版本使用 `disable_gemv` 参数替代 `k_direction_align` 参数
- 编译时使用 `cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;` 在仿真模式下编译运行
- 执行精度对比时，应将输入、输出、真值数据打印并存储为csv文件

---

## 快速入门

### 头文件引入

```c
#include "c_api/asc_simd.h"
```

单个头文件即可包含所有 C API 接口。Cube核主要使用其中的 `cube_compute.h` 和 `cube_datamove.h` 相关接口。

### Kernel 函数结构

```c
__cube__ __global__ __aicore__ void cube_mmad_example(
    __gm__ half* gm_a, __gm__ half* gm_b, __gm__ float* gm_c)
{
    asc_init();

    // L1 Buffer 声明
    __cbuf__ half l1_a[256];
    __cbuf__ half l1_b[256];

    // L0 Buffer 声明
    __ca__ half l0a_a[256];
    __cb__ half l0b_b[256];
    __cc__ float l0c_c[256];

    // 步骤1: GM -> L1 (2D搬运, 自动分形)
    const uint8_t l2_cache_ctl = 0;
    const uint8_t decomp_mode = 0;
    asc_copy_gm2l1_sync(l1_a, gm_a, 0, 0,        // m_start, k_start
                        0, 16, 16,                // dst_stride, m_step, k_step
                        decomp_mode, l2_cache_ctl);
    asc_copy_gm2l1_sync(l1_b, gm_b, 0, 0,
                        0, 16, 16,
                        decomp_mode, l2_cache_ctl);

    // 步骤2: L1 -> L0A, L0B (分形搬运)
    asc_copy_l12l0a_sync(l0a_a, l1_a, 0, 0, 1, 1, 0, 0);
    asc_copy_l12l0b_sync(l0b_b, l1_b, 0, 0, 1, 1, 0, 0);

    // 步骤3: 矩阵乘加
    const uint16_t left_height = 16;
    const uint16_t n_dim = 16;
    const uint16_t right_width = 16;
    const uint8_t unit_flag = 0;
    const bool disable_gemv = false;
    const bool c_matrix_source = false;
    const bool c_matrix_init_val = true;
    asc_mmad_sync(l0c_c, l0a_a, l0b_b,
                  left_height, n_dim, right_width,
                  unit_flag, disable_gemv,
                  c_matrix_source, c_matrix_init_val);

    // 步骤4: L0C -> GM
    const uint16_t n_size = 1;
    const uint16_t m_size = 1;
    const uint32_t loop_dst_stride = 0;
    const uint16_t loop_src_stride = 0;
    const uint8_t clip_relu_pre = 0;
    const uint8_t unit_flag_ctl = 0;
    const uint64_t quant_pre = 0;   // NoQuant
    const uint8_t relu_pre = 0;
    const bool split_en = false;
    const bool NZ2ND_en = false;
    const uint64_t quant_post = 0;  // NoConv
    const uint8_t relu_post = 0;
    const bool clip_relu_post = false;
    const uint8_t eltwise_op = 0;
    const bool eltwise_antq_en = false;
    const bool C0_pad_en = false;
    const bool broadcast_en = false;
    const bool NZ2DN_en = false;

    asc_copy_l0c2gm_sync((__gm__ float*)gm_c, l0c_c,
                         n_size, m_size,
                         loop_dst_stride, loop_src_stride, l2_cache_ctl,
                         clip_relu_pre, unit_flag_ctl, quant_pre, relu_pre,
                         split_en, NZ2ND_en, quant_post, relu_post,
                         clip_relu_post, eltwise_op, eltwise_antq_en,
                         C0_pad_en, broadcast_en, NZ2DN_en);
}
```

---

## 核心概念

### 存储修饰符（Cube核专用）

| 修饰符     | 地址空间                   | 对齐要求 | 说明                        |
|-----------|---------------------------|----------|-----------------------------|
| `__gm__`  | Global Memory（DDR/HBM）   | 无       | 片外内存                    |
| `__cbuf__`| L1 Buffer（片上）          | 32B      | Cube核片上数据缓冲           |
| `__ca__`  | L0A Buffer（矩阵左操作数）  | 512B     | 左矩阵A的输入缓冲区          |
| `__cb__`  | L0B Buffer（矩阵右操作数）  | 512B     | 右矩阵B的输入缓冲区          |
| `__cc__`  | L0C Buffer（矩阵结果）      | 64B      | 结果矩阵C的输出缓冲区        |

**注意**: Cube核没有 UB（Unified Buffer），不能使用 `__ubuf__` 修饰符和 `vector_*` 寄存器类型。

### 内存分配

L1 和 L0 缓冲区必须使用编译期常量数组大小：

```c
constexpr uint32_t SIZE = 256;
__cbuf__ half buf[SIZE];   // 正确
__ca__ half l0a_buf[SIZE]; // 正确
```

### 函数标识符

| 标识符      | 含义                          | 适用核心    |
|------------|------------------------------|------------|
| `__cube__` | Cube核（矩阵计算）专用函数     | Cube核(AIC) |
| `__vector__` | Vector核（矢量计算）专用函数 | Vector核   |
| `__global__` | 核函数入口点                 | 任意       |
| `__aicore__` | AI Core属性（用于入口函数）   | 任意       |

**重要**: 对于 Cube核 kernel，使用 `__cube__ __global__ __aicore__` 组合。如果函数调用了 `asc_copy_l12ub`（Cube核与Vector核间的数据搬运），则不能使用 `__cube__` 或 `__vector__` 修饰符。

---

## API 分类概览

### Cube 计算 (cube_compute.h)

| API                        | 流水线   | 说明                                            |
|----------------------------|---------|-------------------------------------------------|
| `asc_mmad`                 | PIPE_M  | 矩阵乘加: C = A * B + C                         |
| `asc_mmad_sync`            | PIPE_M  | 矩阵乘加（同步版本）                              |
| `asc_mmad_s4`              | PIPE_M  | 矩阵乘加（4-bit数据类型）                         |
| `asc_mmad_s4_sync`         | PIPE_M  | 矩阵乘加（4-bit，同步版本）                       |
| `asc_mmad_mx`              | PIPE_M  | 矩阵乘加（MX格式，带放缩）                        |
| `asc_mmad_mx_sync`         | PIPE_M  | 矩阵乘加（MX格式，同步版本）                       |
| `asc_mmad_sparse`          | PIPE_M  | 稀疏矩阵乘加（A为稀疏，B为稠密）                   |
| `asc_mmad_sparse_sync`     | PIPE_M  | 稀疏矩阵乘加（同步版本）                           |
| `asc_set_l0c2gm_config`    | PIPE_S  | 设置L0C->GM随路量化参数                           |
| `asc_set_l0c2gm_nz2nd`     | PIPE_S  | 设置NZ->ND格式转换参数                            |
| `asc_set_mmad_direction_m` | PIPE_S  | 设置MMAD优先M方向计算                             |
| `asc_set_mmad_direction_n` | PIPE_S  | 设置MMAD优先N方向计算                             |
| `asc_set_fp32_mode`        | PIPE_S  | 设置MMAD的FP32模式（不做舍入）                     |
| `asc_get_l0c2gm_unitflag`  | PIPE_S  | 获取unit_flag设置                                |

### Cube 数据搬运 (cube_datamove.h)

| API                          | 流水线      | 说明                                              |
|------------------------------|------------|---------------------------------------------------|
| `asc_copy_gm2l1`             | PIPE_MTE2  | GM -> L1（2D搬运，支持多种数据类型）                 |
| `asc_copy_gm2l1_align`       | PIPE_MTE2  | GM -> L1（带padding的劈裂搬运）                     |
| `asc_copy_gm2l0a`            | PIPE_MTE2  | GM -> L0A（分形搬运）                               |
| `asc_copy_gm2l0b`            | PIPE_MTE2  | GM -> L0B（分形搬运）                               |
| `asc_copy_l12l0a`            | PIPE_MTE1  | L1 -> L0A（2D/3D分形搬运，支持转置）                |
| `asc_copy_l12l0b`            | PIPE_MTE1  | L1 -> L0B（2D/3D分形搬运，支持转置）                |
| `asc_copy_l12l0c`            | PIPE_MTE1  | L1 -> L0C（线性搬运，支持类型转换）                  |
| `asc_copy_l0c2gm`            | PIPE_FIX   | L0C -> GM（支持量化、relu、格式转换）               |
| `asc_copy_l0c2ub`            | PIPE_FIX   | L0C -> UB（支持量化、relu、格式转换）               |
| `asc_copy_l0c2l1`            | PIPE_FIX   | L0C -> L1（支持量化、relu、格式转换）               |
| `asc_copy_l12fb`             | PIPE_FIX   | L1 -> Fixpipe Buffer（量化参数搬运）               |
| `asc_copy_l12fb_v2`          | PIPE_MTE1  | L1 -> Fixpipe Buffer（v2版本）                    |
| `asc_copy_l12bt`             | PIPE_MTE1  | L1 -> BiasTable Buffer（Bias数据搬运）            |
| `asc_copy_l12gm`             | PIPE_MTE1  | L1 -> GM（线性搬运）                               |
| `asc_copy_l12ub`             | PIPE_MTE1  | L1 -> UB（跨核搬运，不能使用__cube__）              |
| `asc_fill_l0a`               | PIPE_MTE1  | L0A 初始化为指定值                                  |
| `asc_fill_l0b`               | PIPE_MTE1  | L0B 初始化为指定值                                  |
| `asc_fill_l1`                | PIPE_MTE1  | L1 初始化为指定值                                  |
| `asc_set_l13d_fmatrix`       | PIPE_S     | 设置左矩阵3D搬运的Feature map属性                   |
| `asc_set_l13d_fmatrix_b`     | PIPE_S     | 设置右矩阵3D搬运的Feature map属性                   |
| `asc_set_l13d_rpt`           | PIPE_S     | 设置Load3Dv2的repeat参数                           |
| `asc_set_l13d_size`          | PIPE_S     | 设置3D搬运的L1 Buffer边界值                        |
| `asc_set_l13d_padding`       | PIPE_S     | 设置3D搬运的Pad填充值                               |
| `asc_set_gm2l1_loop_size`    | PIPE_S     | 设置GM->L1搬运的循环体配置                          |
| `asc_set_gm2l1_loop1_stride` | PIPE_S     | 设置GM->L1搬运的循环1步长                           |
| `asc_set_gm2l1_loop2_stride` | PIPE_S     | 设置GM->L1搬运的循环2步长                           |
| `asc_set_gm2l1_pad`          | PIPE_S     | 设置GM->L1搬运的Pad填充模式                        |

---

## 矩阵分形（Fractal）核心知识

### 分形基本概念

Cube核的矩阵运算以"分形"（fractal）为基本处理单元。分形是 Cube 单元在一次运算中能处理的固定大小的矩阵子块。

### 3510架构分形尺寸

| 数据类型位宽 | L0A分形大小  | L0B分形大小  | L0C分形大小  | 分形字节数(C0_SIZE) |
|:-----------:|:----------:|:----------:|:----------:|:------------------:|
| 4-bit (int4/fp4) | 16 x 64 | 64 x 16 | 16 x 16 | 16 * sizeof(T) |
| 8-bit (int8/fp8/hifloat8) | 16 x 32 | 32 x 16 | 16 x 16 | 16 * sizeof(T) |
| 16-bit (half/bf16/int16) | 16 x 16 | 16 x 16 | 16 x 16 | 16 * sizeof(T) |
| 32-bit (float/int32) | 16 x 16 | 16 x 16 | 16 x 16 | 16 * sizeof(T) |

- C0 = 16（对于8-bit及以下类型，C0 = 32 用于L0A/L0B，C0 = 16 用于L0C）
- 每个分形在L0A/L0B中占据 **512B**
- k_step 的单位为32B，m_step 的单位为16个元素

### NZ矩阵存储格式

矩阵在 L0C 中按照 **NZ (N-major, Z-major)** 格式存储：
- N 方向（列方向）为第一优先级
- Z 方向（C0 个元素组成的"骨"）为第二优先级
- 以 half (16-bit) 为例：C0=16，将矩阵按16行一组划分，每个16x16的分形按列主序存储

理解分形存储对于正确计算 `loop_src_stride` 至关重要：
- `loop_src_stride` 的单位为 C0_Size = 16 * sizeof(T)
- 对于 float 类型: C0_Size = 16 * 4 = 64B
- 对于 half 类型: C0_Size = 16 * 2 = 32B

### m_start_position / k_start_position 参数

在 `asc_copy_l12l0a` 和 `asc_copy_l12l0b` 的2D格式中：

```
以 M x K 矩阵为例:
  m_start_position: M轴方向的起始位置，单位 = 16个元素
  k_start_position: K轴方向的起始位置，单位 = 32B

  m_step: M轴方向搬运长度，单位 = 16个元素
  k_step: K轴方向搬运长度，单位 = 32B

  src_stride: K方向相邻分形起始地址间隔，单位 = 512B (分形块大小)
  dst_stride: K方向相邻分形起始地址间隔，单位 = 512B (分形块大小)
```

**举例**: 对于 half 类型的 32x32 矩阵，M方向有2个分形，K方向有2个分形：
- 第0个分形: m_start=0, k_start=0
- 第1个分形: m_start=0, k_start=1 (K方向平移)
- 第2个分形: m_start=1, k_start=0 (M方向平移)
- 第3个分形: m_start=1, k_start=1

### GM->L1 2D搬运参数

`asc_copy_gm2l1` 在2D模式下：

```
asc_copy_gm2l1(__cbuf__ half *dst, __gm__ half *src,
    m_start_position, k_start_position,
    dst_stride, m_step, k_step,
    decomp_mode, l2_cache_ctl)

参数说明:
  m_start_position: M方向起始位置，单位 = 16个元素
  k_start_position: K方向起始位置，单位 = 32B
  dst_stride: L1中相邻分形的K方向步长，单位 = 32B (每个Block)
  m_step: M方向搬运长度，单位 = 16个元素
  k_step: K方向搬运长度，单位 = 32B
```

**注意**: `dst_stride` = k_step 时表示分形连续存放（无间隔）。

### 分形数量计算

对于 M x K 的矩阵（half类型，分形大小 16x16）：
- M方向分形数: `num_fractal_m = (M + 15) / 16`
- K方向分形数: `num_fractal_k = (K + 15) / 16`  (注意: 单位为32B = 16个half元素)
- 总元素数（分形对齐后）: `num_fractal_m * 16 * num_fractal_k * 16`

---

## Cube核常用使用模式

### 模式 1：单核单分形矩阵乘法 (half -> float)

数据流: GM -> L1 -> L0A/L0B -> MMAD -> L0C -> GM

```c
__cube__ __global__ __aicore__ void simple_matmul(
    __gm__ half* a, __gm__ half* b, __gm__ float* c)
{
    asc_init();

    constexpr uint32_t M = 16;
    constexpr uint32_t K = 16;
    constexpr uint32_t N = 16;

    __cbuf__ half l1_a[M * K / 16 * 16];  // 256
    __cbuf__ half l1_b[K * N / 16 * 16];  // 256
    __ca__ half l0a_a[256];
    __cb__ half l0b_b[256];
    __cc__ float l0c_c[256];

    // GM -> L1
    asc_copy_gm2l1_sync(l1_a, a, 0, 0, 0, 1, 1, 0, 0);
    asc_copy_gm2l1_sync(l1_b, b, 0, 0, 0, 1, 1, 0, 0);

    // L1 -> L0A, L0B
    asc_copy_l12l0a_sync(l0a_a, l1_a, 0, 0, 1, 1, 0, 0);
    asc_copy_l12l0b_sync(l0b_b, l1_b, 0, 0, 1, 1, 0, 0);

    // MMAD
    asc_mmad_sync(l0c_c, l0a_a, l0b_b,
                  M, K, N, 0, false, false, true);

    // L0C -> GM (无量化)
    asc_copy_l0c2gm_sync((__gm__ float*)c, l0c_c,
                         1, 1, 0, 0, 0,
                         0, 0, 0/*NoQuant*/, 0,
                         false, false, 0/*NoConv*/, 0,
                         false, 0, false, false, false, false);
}
```

### 模式 2：大矩阵分形遍历

对于 MxKxN 的大矩阵，需要以分形为单位双重循环遍历：

```c
__cube__ __global__ __aicore__ void large_matmul(
    __gm__ half* a, __gm__ half* b, __gm__ float* c,
    uint32_t num_fractal_m, uint32_t num_fractal_k, uint32_t num_fractal_n)
{
    asc_init();

    __cbuf__ half l1_a[256];    // 1个分形
    __cbuf__ half l1_b[256];    // 1个分形
    __ca__ half l0a_a[256];
    __cb__ half l0b_b[256];
    __cc__ float l0c_c[256];

    for (uint32_t m = 0; m < num_fractal_m; ++m) {
        for (uint32_t n = 0; n < num_fractal_n; ++n) {
            // 初始化 C 分形为0
            asc_fill_l0a_sync(l0a_a, half(0),
                asc_fill_value_config());  // 此处用 fill_l0a 做示范，实际需搬入数据

            for (uint32_t k = 0; k < num_fractal_k; ++k) {
                uint32_t a_k_start = k * 1;          // K方向分形索引
                uint32_t a_m_start = m;              // M方向分形索引
                uint32_t b_k_start = k;              // K方向分形索引（B的M维度）
                uint32_t b_n_start = n;              // N方向分形索引

                // GM -> L1
                asc_copy_gm2l1(l1_a, a, a_m_start, a_k_start,
                               0, 1, 1, 0, 0);
                asc_copy_gm2l1(l1_b, b, b_k_start, b_n_start,
                               0, 1, 1, 0, 0);

                asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
                asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);

                // L1 -> L0
                asc_copy_l12l0a(l0a_a, l1_a, 0, 0, 1, 1, 0, 0);
                asc_copy_l12l0b(l0b_b, l1_b, 0, 0, 1, 1, 0, 0);

                asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
                asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);

                // MMAD (k>0时累加到L0C)
                bool is_first_k = (k == 0);
                asc_mmad(l0c_c, l0a_a, l0b_b,
                         16, 16, 16,
                         0, false, !is_first_k, is_first_k);
            }

            asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
            asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);

            // L0C -> GM
            uint32_t c_gm_offset = m * num_fractal_n * 256 + n * 256;
            asc_copy_l0c2gm((__gm__ float*)(c + c_gm_offset), l0c_c,
                            1, 1, 0, 0, 0,
                            0, 0, 0, 0,
                            false, false, 0, 0,
                            false, 0, false, false, false, false);
        }
    }
}
```

### 模式 3：同步指令使用

Cube核中的同步指令用于协调不同流水线间的依赖：

```c
// PIPE_MTE2: GM->L1 搬运流水线
// PIPE_MTE1: L1->L0A/L0B 搬运流水线
// PIPE_M:    Cube矩阵计算流水线（MMAD）
// PIPE_FIX:  L0C->GM 搬运流水线
// PIPE_S:    标量/控制流水线

// GM->L1 完成后通知 PIPE_MTE1 可以读取
asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);

// L1->L0 完成后通知 PIPE_M 可以计算
asc_sync_notify(PIPE_MTE1, PIPE_M, EVENT_ID0);
asc_sync_wait(PIPE_MTE1, PIPE_M, EVENT_ID0);

// MMAD 完成后通知 PIPE_FIX 可以搬运结果
asc_sync_notify(PIPE_M, PIPE_FIX, EVENT_ID0);
asc_sync_wait(PIPE_M, PIPE_FIX, EVENT_ID0);
```

**注意**: 使用 `_sync` 后缀的API（如 `asc_copy_gm2l1_sync`、`asc_mmad_sync`）会自动插入同步，简化编程但可能降低性能。

---

## asc_mmad 参数详解

```
asc_mmad(__cc__ float* c_matrix,
         __ca__ half* a_matrix,
         __cb__ half* b_matrix,
         uint16_t left_height,      // A矩阵的行数 (M)
         uint16_t n_dim,            // A矩阵的列数/B矩阵的行数 (K)
         uint16_t right_width,      // B矩阵的列数 (N)
         uint8_t unit_flag,         // 细粒度并行控制
         bool disable_gemv,         // 3510专用: 关闭GEMV模式
         bool c_matrix_source,      // C矩阵来源
         bool c_matrix_init_val)    // C矩阵是否初始化为0
```

### unit_flag 说明

| 取值 | 含义                                                         |
|:---:|-------------------------------------------------------------|
| 0   | 关闭unit_flag                                                |
| 1   | 保留值                                                       |
| 2   | 使能unit_flag，硬件执行完后不关闭（用于中间分形）               |
| 3   | 使能unit_flag，硬件执行完后关闭（用于最后一个分形）             |

unit_flag 使能后，硬件每计算完一个分形就将结果搬出，实现矩阵计算与数据搬运的细粒度并行。最后一个分形使用3，其余使用2。

### c_matrix_source 和 c_matrix_init_val

| c_matrix_init_val | c_matrix_source | 效果                                  |
|:-----------------:|:--------------:|---------------------------------------|
| true              | 任意            | C矩阵初始化为全0                       |
| false             | true           | C矩阵初始值来源于 BiasTable (C2)       |
| false             | false          | C矩阵初始值来源于 L0C 中已有的数据      |

### disable_gemv (3510专用)

| 取值  | 含义                                                         |
|:----:|-------------------------------------------------------------|
| true | 关闭GEMV模式（标准矩阵乘法）                                   |
| false| 开启GEMV模式（矩阵-向量乘法，要求 left_height=1）              |

---

## asc_copy_l0c2gm 参数详解

`asc_copy_l0c2gm` 将 L0C 的结果搬运到 GM，支持随路量化、ReLU、格式转换。

关键参数：

| 参数             | 说明                                              |
|-----------------|---------------------------------------------------|
| n_size          | 源NZ矩阵N方向大小（分形数），范围[1,4095]           |
| m_size          | 源NZ矩阵M方向大小（分形数），范围[1,65535]          |
| loop_dst_stride | 目的相邻ND矩阵起始地址偏移                          |
| loop_src_stride | 源NZ矩阵中相邻Z排布起始地址偏移，单位=C0_Size         |
| quant_pre       | 预处理量化模式（如NoQuant=0, DEQF16等）            |
| quant_post      | 后处理量化模式（如NoConv=0, VQS162B8_POST等）      |
| NZ2ND_en        | 使能NZ->ND格式转换（true: 转成连续ND格式）          |

### 常见量化模式 (quant_pre)

| 枚举值                  | 含义                            |
|------------------------|--------------------------------|
| NoQuant (0)            | 不使能量化                      |
| F322BF16               | float -> bfloat16              |
| F322F16                | float -> half                  |
| DEQF16                 | int32 -> half                  |
| VDEQF16                | int32 -> half（矢量量化）       |
| VREQ8                  | int32 -> uint8/int8（矢量量化）  |

### 常见量化模式 (quant_post)

| 枚举值              | 含义                      |
|--------------------|--------------------------|
| NoConv (0)         | 不使能量化                |
| VQS162B8_POST      | int16 -> bfloat8（矢量量化）|
| VQS162S4_POST      | int16 -> int4（矢量量化）  |
| VQS162S16_POST     | int16 -> int16（矢量量化） |

---

## 支持的数据类型（Cube核常用）

| C/C++ 类型    | L0A修饰符 | L0B修饰符 | L0C修饰符 | 说明              |
|--------------|----------|----------|----------|-------------------|
| `half`       | `__ca__` | `__cb__` | `__cc__` | 半精度浮点         |
| `float`      | `__ca__` | `__cb__` | `__cc__` | 单精度浮点         |
| `bfloat16_t` | `__ca__` | `__cb__` | `__cc__` | Brain浮点          |
| `int8_t`     | `__ca__` | `__cb__` | `__cc__` | 8位有符号整数      |
| `int32_t`    | `__ca__` | `__cb__` | `__cc__` | 32位有符号整数     |
| `fp8_e4m3fn_t` | `__ca__` | `__cb__` | -        | FP8 E4M3格式       |
| `fp8_e5m2_t`   | `__ca__` | `__cb__` | -        | FP8 E5M2格式       |
| `hifloat8_t`   | `__ca__` | `__cb__` | -        | HiFloat8格式       |
| `fp4x2_e2m1_t` | `__ca__` | `__cb__` | -        | FP4 E2M1格式（MX） |
| `fp4x2_e1m2_t` | `__ca__` | `__cb__` | -        | FP4 E1M2格式（MX） |
| `int4b_t`      | `__ca__` | `__cb__` | -        | 4位整数（S4）       |

**注意**: `asc_mmad` 的结果类型由输入类型决定。常见组合：
- A=half, B=half => C=float
- A=int8, B=int8 => C=int32
- A=bf16, B=bf16 => C=float
- A=fp8, B=fp8 => C=float

---

## CMakeLists.txt 模板

```cmake
cmake_minimum_required(VERSION 3.16)
find_package(ASC REQUIRED)
project(kernel_samples LANGUAGES ASC CXX)
add_executable(cube_example cube_example.asc)
target_compile_options(cube_example PRIVATE
    $<$<COMPILE_LANGUAGE:ASC>:--npu-arch=dav-3510>
)
```

---

完整的 API 参考请参见 [../../../docs/api/c_api/cube_compute/](../../../docs/api/c_api/cube_compute/) 和 [../../../docs/api/c_api/cube_datamove/](../../../docs/api/c_api/cube_datamove/)。
完整的可运行示例请参见 [examples/cube_example.md](examples/cube_example.md)。


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