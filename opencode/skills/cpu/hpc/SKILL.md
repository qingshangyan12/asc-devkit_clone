---
name: hpc-algo-design
description: SIMD场景下的高性能数值计算算法设计准则。在CPU上以SIMD（SSE/AVX/AVX-512）方式实现高性能数学函数、数值计算内核时使用。所有准则均以SIMD并行执行模型为前提：无分支、无逐元素串行依赖、掩码驱动控制流。当用户需要设计或优化SIMD向量化数值计算API时，应使用此技能。
---

# SIMD 高性能数值计算算法设计准则

## 1. 零分支强制

SIMD 所有 lane 同步执行同一指令，分支导致所有 lane 串行化。必须完全消除运行时分支。

- **条件选择**: 用 `_mm_blendv_ps` / `_mm_mask_xxx` 替代 `if/else`
- **比较 + 逻辑**: 用 `_mm_cmp_ps` + `_mm_and_ps` / `_mm_or_ps` 构建无分支条件
- **符号操作**: 用 `_mm_and_ps(x, signmask)` 取绝对值，`_mm_xor_ps(x, sign_src)` 做 copysign
- **查表无分支**: `_mm_permutevar_ps` / `_mm_shuffle_epi8` 实现 per-lane LUT
- 循环展开边界用 mask 处理尾部元素，不拆分标量尾循环

```cpp
// 差: 分支（SIMD 不可用）
for (int i = 0; i < n; ++i)
    dst[i] = (src[i] > 0) ? src[i] : -src[i];

// 好: 掩码 + blend (完全向量化)
__m256 sign = _mm256_set1_ps(-0.0f);
__m256 absv = _mm256_andnot_ps(sign, _mm256_loadu_ps(src));
// 或更简: _mm256_max_ps(src, _mm256_sub_ps(_mm256_setzero_ps(), src)) 但用 andnot 更优
```

## 2. 特殊值自然收敛（Inf / NaN / ±0）

对 Inf、NaN、±0 输入，利用 IEEE 754 运算的自然行为，零分支收敛到正确结果。

- **NaN 传播**: `NaN * x = NaN`, `NaN + x = NaN`，任何含 NaN 的运算自然产生 NaN；无需 `isnan` 检查
- **Inf 运算**: `1/Inf = 0`, `Inf + x = Inf`, `0 * Inf = NaN`；利用规则自然处理
- **±0 处理**: `copysign(x, y)` 正确传播 -0；`signbit` 无分支提取符号；`x + (-0) = x`
- **除法保护**: 接受 IEEE 754 行为（除零 → Inf），由调用方处理；或 `x / (|y| + FLT_MIN)` 避免除零
- **次正规数**: 默认 FTZ + DAZ 模式自动刷新为零，无需 per-lane 检测

## 3. 避免跨 lane 依赖

SIMD 的威力在于 lane 间并行；任何 lane 间数据依赖都强制串行化。

- **禁止 sequantial reduction**: Kahan 求和每个元素依赖前一元素的 comp 变量，在 SIMD 中完全失效
- **用树形归约**: pairwise summation，log₂(N) 步完成，每步完全并行
- **避免 hadd/hsub**: `_mm_hadd_ps` 内部有 lane 间依赖，延迟高；用垂直操作 + shuffle 替代
- **拆分跨 lane 与 lane 内**: 先做 lane 内并行计算，最后一步再做跨 lane 归约

```cpp
// 差: Kahan 求和 (逐元素串行依赖，SIMD 不可用)
float sum = 0, comp = 0;
for (...) { float y = data[i] - comp; float t = sum + y; comp = (t-sum)-y; sum = t; }

// 好: pairwise 树形归约 (SIMD 友好)
for (int stride = 1; stride < n; stride *= 2)
    for (int i = 0; i < n; i += 2*stride)
        data[i] += data[i + stride];
```

## 4. 向量寄存器压力最小化

x86-64 SIMD 寄存器：SSE 16×128bit、AVX 16×256bit、AVX-512 32×512bit。寄存器溢出（spill）代价高于标量。

- 复用可覆盖的向量变量
- 内联多项式求值用 Horner/FMA 链，减少中间寄存器占用
- 展开因子与向量寄存器数匹配：AVX 展开 4-8 次即可，过度展开反而增加 spill
- 常量广播用 `_mm_set1_ps`，避免从内存重复加载
- 用 `__m128`/`__m256` 别名复用同一寄存器组

## 5. 数据对齐与连续访问

- **对齐加载**: 64 字节对齐（AVX-512）、32 字节对齐（AVX）、16 字节对齐（SSE）；`alignas(64)` 声明
- **连续访问**: gather/scatter 在 AVX2 之前不支持，AVX2 的 gather 延迟也远高于连续 load
- **避免跨 cache line 的向量加载**: 32/64 字节对齐确保单次向量加载不触两行 cache line
- **SoA 布局**: 将同质数据连续排列（`x[], y[], z[]` 而非 `struct {x,y,z}[]`）
- **无别名保证**: `__restrict` 让编译器生成流式存储（`_mm_stream_ps`），避免 read-for-ownership

## 6. 快速收敛

多项式/迭代逼近在 SIMD 下同样适用，但需确保所有 lane 执行相同次数的迭代。

- **固定迭代次数**: Newton-Raphson 用固定次数（2-3次），不用 `while(error > tol)` 变循环
- **minimax 多项式**: Remez 算法求解，项数最少 → 依赖链最短
- **Estrin Scheme**: 提高 ILP，降低依赖链深度；SIMD 下收益更显著
- **查表 + 插值**: 用 `_mm_permutevar_ps` 做并行查表，低阶多项式插值
- **rsqrt 逼近**: `_mm_rsqrt_ps`（12bit 精度）+ 一次 Newton 迭代 → 24bit 精度

## 7. 廉价操作优先（SIMD 强化版）

SIMD 除法/jump/水平操作代价更高。

- **除法 → 乘倒数**: `divps` 延迟 11-13 周期 vs `mulps` 4 周期；用 `_mm_rcp_ps` + Newton 迭代
- **sqrt → rsqrt**: `sqrtps` 延迟 12 周期 vs `rsqrtps` 4 周期；`x * rsqrt(x)` 替代 `sqrt(x)`
- **整数除法 → 乘法 + 移位**: SIMD 无整数除法指令（仅 AVX-512 有），必须用 Magic Number 乘法替代
- **避免 gather/scatter**: 延迟 ~20 周期，用连续 load + permute 替代
- **避免水平 add/min/max**: 1 周期垂直 add vs 3-5 周期水平 add

## 8. 掩码驱动尾部处理

当数组长度不是向量宽度的整数倍时，用掩码处理尾部元素，不拆分标量尾循环。

```cpp
// SSE: 最后 <4 元素用标量循环 → 差
// AVX-512: 用 __mmask16 掩码 → 好
_mm512_mask_store_ps(dst, mask, result);  // 仅写入 mask=1 的 lane
```

- AVX/SSE 无原生掩码时，用 blend 写入：`_mm256_maskstore_ps` 或 `_mm256_blendv_ps` + `_mm256_storeu_ps`
- AVX-512 提供完整零掩码操作，无需额外指令

## 9. 数值稳定性（SIMD 适配）

某些标量稳定技术在 SIMD 下代价过高或不可用，需要 SIMD 友好的替代方案。

- ~~Kahan 求和~~ → **pairwise summation**: O(log N) 归约，无 lane 间串行依赖
- **FMA 优先**: `_mm_fmadd_ps` 单次舍入，减少舍入误差累积
- **`hypot` 可分解为**: `sqrt(fma(x,x, y*y))`；SIMD 下直接用 FMA，无需标量 `hypot` 调用
- **`log1p`/`expm1`**: 用 minimax 多项式在 SIMD 内实现，避免调用标量库函数
- 避免两个接近的大数相减 → 重排表达式，消除灾难性抵消

## 10. 编译时计算与常量广播

- 系数表 `alignas(64) static constexpr float coeff[]`，按向量宽度广播
- 掩码常量 `static const __m256 sign_mask = _mm256_set1_ps(-0.0f)` 编译期构造
- `constexpr` 函数编译期求值多项式系数
- 用 `_mm_set1_ps(constexpr_value)` 而非 `_mm_set_ps(a,b,c,d)` 减少指令数

## 快速对照：标量优化 → SIMD 替代

| 标量方案 | SIMD 问题 | SIMD 替代 |
|----------|-----------|------------|
| `if/else` | 分支发散 | `blendv_ps` / mask |
| `while` 收敛 | 迭代次数不定 | 固定次数 Newton |
| Kahan 求和 | 逐元素串行依赖 | pairwise 树形归约 |
| `x / y` | divps 延迟高 | `x * rcp(y)` 或 `rcpps+Newton` |
| `sqrt(x)` | sqrtps 延迟高 | `x * rsqrtps(x)` + Newton |
| 标量尾循环 | 额外分支 | 掩码写入 / blend |
| AoS 布局 | gather/scatter | SoA 布局 |
| `hadd_ps` | 跨 lane 延迟 | 垂直 + shuffle |
| `std::atan` | 非 SIMD 函数 | 自实现 minimax 多项式 |
