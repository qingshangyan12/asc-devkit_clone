# Cube C API 踩坑记录

本文档记录在编写 Cube 核 C API 测试用例过程中发现的常见错误、根因分析及修复方案。

## 1. int8 类型矩阵乘 (asc_mmad) 2D 搬运精度失败

### 现象
```
[FAIL] asc_mmad_int8 max_diff=17
A[0:2]=-3 -2 -1
B[0:2]=-2 -1 0
output[0:2]=-1 -1 -11    // 大量错误
golden[0:2]=-1 9 -1
```
输出矩阵后 8 列全为 0，前 8 列仅第一列正确。

### 根因
`asc_copy_gm2l1_sync` 进行 2D 分形搬运时，`k_step` 参数单位为 **32B**。

| 类型 | 分形 | k_step=1 覆盖元素数 | 所需 K 宽度 | 是否对齐 |
|------|------|--------------------|-------------|---------|
| half | 16×16 / 16×16 | 16 个 half = 32B | 16 | ✅ |
| bf16 | 16×16 / 16×16 | 16 个 bf16 = 32B | 16 | ✅ |
| **int8** | 16×**32** / **32**×16 | **32** 个 int8 = 32B | B 只有 **16** 列 | ❌ |

int8 的 `k_step=1` 一次搬运 32 个 int8（32B），但 B 矩阵的 N 方向只有 16 列 = 16B = 0.5×32B。`k_step` 不能为 0.5，导致 B 数据读溢出或被压缩，MMAD 读取到错误的数据。

### 修复方案
将 B 矩阵补齐到 32 列（虚列填 0），使 `k_step=1` 覆盖完整宽度：

```c
// 修复前: B 只有 16 列，k_step=1 无法对齐
constexpr uint32_t TOTAL_B = FRACTAL_K * N_SIZE;       // 32×16=512

// 修复后: B 补齐到 32 列
constexpr uint32_t FRACTAL_B_COLS = 32;                 // k_step=1 匹配的列数
constexpr uint32_t TOTAL_B = FRACTAL_K * FRACTAL_B_COLS; // 32×32=1024

// 数据初始化: 使用 32 列步长
b[k * FRACTAL_B_COLS + j] = value;  // 列 0-15 有效，16-31 为零

// GM→L1→L0B: m_step=2(32K行), k_step=1(32int8=32B) 
asc_copy_gm2l1_sync(l1_b, gm_b, 0, 0, 0, 2, 1, 0, 0);
asc_copy_l12l0b_sync(l0b, l1_b, 0, 0, 2, 1, 0, 0);
```

### 泛化规则
**任何非 16-bit 数据类型的 2D 分形搬运，必须确保 GM 源数据的 K 方向宽度是 `k_step * 32B / sizeof(T)` 的整数倍。** 例如：
- half/bf16 (16-bit): k_step=1 → 16 列对齐
- int8 (8-bit): k_step=1 → 32 列对齐，k_step=0.5 不可用
- float/int32 (32-bit): k_step=1 → 8 列对齐


## 2. CANN 9.0.0-beta.2 缺少 FP4/FP8 类型定义

### 现象
```
error: unknown type name 'fp4x2_e1m2_t'
error: unknown type name 'fp8_e4m3fn_t'
error: unknown type name 'int4b_t'
```

### 根因
CANN 9.0.0-beta.2 的 `asc/impl/utils/common_types.h` 缺少以下类型别名：
```c
using fp4x2_e1m2_t = uint8_t;
using fp4x2_e2m1_t = uint8_t;
using fp8_e4m3fn_t = uint8_t;
using fp8_e5m2_t = uint8_t;
using fp8_e8m0_t = uint8_t;
using int4b_t = uint8_t;
```

CANN 9.1.0-beta.1 已修复。

### 修复方案（规避）
在 `#include "c_api/asc_simd.h"` 之前插入互异 struct 定义（不能用 `using`/`#define`，会导致函数重载冲突）：
```c
struct fp4x2_e1m2_t { uint8_t v; };
struct fp4x2_e2m1_t { uint8_t v; };
struct fp8_e4m3fn_t { uint8_t v; };
struct fp8_e5m2_t   { uint8_t v; };
struct fp8_e8m0_t   { uint8_t v; };
struct int4b_t      { uint8_t v; };
```
必须使用**互不相同的 struct** 来定义，因为 impl 头文件里有针对不同 fp8 类型的函数重载，若用 `uint8_t` 会让重载坍塌导致 `redefinition` 错误。


## 3. L0C→GM 搬运参数 NZ2ND_en 应默认为 false

### 现象
输出全部为 0 或完全错误的数值。

### 根因
参考示例 `cube_mmad_example.asc:92` 明确指出：
```c
constexpr bool NZ2ND_en = false;  // 输出NZ格式 (避免格式转换错误)
```

设置 `NZ2ND_en = true` 会触发 NZ→ND 格式转换，但转换过程中的 stride/alignment 参数极易出错。

### 修复
所有测试的 `asc_copy_l0c2gm_sync` 中，`NZ2ND_en` 统一设为 `false`。仅 `asc_set_l0c2gm_nz2nd` 测试保留 `true`（该测试的目的就是验证 NZ2ND 转换）。

### 参考参数（3510 21 参数版本）
```c
asc_copy_l0c2gm_sync((__gm__ float*)gm_c, l0c_c,
    n_size, m_size,                              // N/M 方向元素数 (如 16)
    loop_dst_stride, loop_src_stride, l2_cache_ctl,  // dst=N_SIZE, src=1(C0_Size), cache=0
    0/*clip_relu*/, 0/*unit_flag*/, 0/*quant_pre*/, 0/*relu_pre*/,
    false/*split*/, false/*NZ2ND*/, 0/*quant_post*/, 0/*relu_post*/,
    false/*clip_relu_post*/, 0/*eltwise*/, false/*antq*/, false/*C0_pad*/,
    false/*broadcast*/, false/*NZ2DN*/);
```


## 4. 测试用例基本规则

### 计算类 API
- 数据流: GM→L1→L0A/L0B → MMAD → L0C→GM
- 验证: 与 CPU 矩阵乘结果比较

### 搬运类 API
- 最优验证: 搬入搬出回环比对（GM→L1→GM, L0C→L1→L0C→GM 等）
- 备选验证: 通过 MMAD 管道间接验证（3510 缺少 asc_copy_l12gm 回环路径时）
- 注意: 线性 burst 搬运（如 asc_copy_gm2l1_align）产生的数据布局可能与分形 2D 搬运不同，通过 MMAD 管道间接验证时可能因格式不兼容而失败

### CSV 输出规范
- 每个测试输出**单一 CSV**，包含:
  - 输入矩阵 A/B（标注行列和值）
  - 输出矩阵 C（标注输出 vs 期望）
  - 最终 PASS/FAIL 结论及误差
