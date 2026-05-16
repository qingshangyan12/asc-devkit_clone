# SIMD 高性能算法设计 C++ 示例

## 1. 零分支 — 掩码替代 if/else

```cpp
#include <immintrin.h>

// 差: 分支无法向量化
void abs_bad(float* dst, const float* src, size_t n) {
    for (size_t i = 0; i < n; ++i)
        dst[i] = (src[i] < 0) ? -src[i] : src[i];
}

// 好: 掩码 + blend, 完全向量化, +0/-0/NaN/Inf 自然收敛
void abs_simd(float* dst, const float* src, size_t n) {
    const __m256 sign_mask = _mm256_set1_ps(-0.0f);  // 0x80000000
    size_t i = 0;
    for (; i + 8 <= n; i += 8) {
        __m256 v = _mm256_loadu_ps(src + i);
        __m256 absv = _mm256_andnot_ps(sign_mask, v);
        _mm256_storeu_ps(dst + i, absv);
    }
    for (; i < n; ++i)
        dst[i] = fabsf(src[i]);
}
```

```cpp
// clamp: 用 min/max 替代分支
// 差: 分支
float clamp_bad(float x, float lo, float hi) {
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

// 好: SIMD min/max 无分支, Inf 自然收敛
__m256 clamp_simd(__m256 x, __m256 lo, __m256 hi) {
    return _mm256_max_ps(lo, _mm256_min_ps(x, hi));
    // x=NaN → max(NaN, lo) = NaN  (IEEE 自然传播)
    // x=Inf → min(Inf, hi) = hi → max(lo, hi) = hi  (正Inf截断)
    // lo>hi → _mm_min_ps 返回第二个操作数当 NaN 或不可比较时, 需保证 lo≤hi
}
```

## 2. 特殊值自然收敛 — copysign / signbit 消除符号分支

```cpp
// 差: 分支判断符号
__m256 copysign_bad(__m256 x, __m256 sign_src) {
    // 无法用分支实现 per-lane copysign
    return _mm256_blendv_ps(
        _mm256_andnot_ps(_mm256_set1_ps(-0.0f), x),  // abs(x)
        x,
        _mm256_cmp_ps(sign_src, _mm256_setzero_ps(), _CMP_GE_OQ)
    );
}

// 好: 位运算 copysign, 单条 SIMD 逻辑指令, +0/-0/NaN 自然传播
__m256 copysign_simd(__m256 x, __m256 sign_src) {
    const __m256 sign_mask = _mm256_set1_ps(-0.0f);
    __m256 abs_x  = _mm256_andnot_ps(sign_mask, x);
    __m256 sign_y = _mm256_and_ps(sign_mask, sign_src);
    return _mm256_or_ps(abs_x, sign_y);
}
```

```cpp
// 无分支 signbit: 比较归零向量 → 所有 lane 并行
__m256i signbit_simd(__m256 x) {
    return _mm256_srai_epi32(_mm256_castps_si256(x), 31);
    // x=-0 → bit31=1 → -1; x=+0 → bit31=0 → 0
    // x=NaN → bit31未定义但结果可预测; x=Inf → bit31=0
}
```

## 3. 避免跨 lane 依赖 — pairwise 树形归约替代 Kahan

```cpp
// 差: Kahan 求和 — 每个元素依赖前一元素的 comp, SIMD 完全失效
float kahan_sum(const float* data, size_t n) {
    float sum = 0.0f, comp = 0.0f;
    for (size_t i = 0; i < n; ++i) {
        float y = data[i] - comp;
        float t = sum + y;
        comp = (t - sum) - y;
        sum = t;
    }
    return sum;
}

// 好: 向量化累加 + pairwise 树形归约
float simd_sum(const float* data, size_t n) {
    __m256 sum = _mm256_setzero_ps();
    size_t i = 0;
    // Phase 1: SIMD 垂直累加 (8 lanes 并行)
    for (; i + 8 <= n; i += 8)
        sum = _mm256_add_ps(sum, _mm256_loadu_ps(data + i));
    // Phase 2: 树形归约 (3 步并行 shuffle)
    __m128 hi = _mm256_extractf128_ps(sum, 1);
    __m128 lo = _mm256_castps256_ps128(sum);
    __m128 r  = _mm_add_ps(lo, hi);          // lo+hi
    r = _mm_hadd_ps(r, r);                    // a+b, c+d → a+b, c+d
    r = _mm_hadd_ps(r, r);                    // (a+b)+(c+d)
    float result = _mm_cvtss_f32(r);
    // Phase 3: 尾部标量
    for (; i < n; ++i)
        result += data[i];
    return result;
}
```

## 4. 快速收敛 — Newton-Raphson 固定迭代（无分支收敛条件）

```cpp
// rsqrt: 内建指令 12bit → 一次 Newton → ~24bit → 二次 Newton → ~48bit
// 固定迭代次数，所有 lane 同步，无 while 循环
__m256 rsqrt_simd(__m256 x) {
    // x=0 → mm_rsqrt_ps 返回 Inf → Newton: Inf*(..) → Inf (自然行为)
    // x<0 → mm_rsqrt_ps 返回 NaN → Newton: NaN*(..) → NaN (自然传播)
    // x=Inf → mm_rsqrt_ps 返回 0 → Newton: 0*(..) → 0 (自然行为)
    __m256 y = _mm256_rsqrt_ps(x);
    __m256 half = _mm256_set1_ps(0.5f);
    __m256 three = _mm256_set1_ps(1.5f);
    // 一次 Newton 迭代: y = y * (3 - x*y*y) * 0.5
    // 用 FMA 重写: y = y * fma(-half*x, y*y, 1.5)
    y = _mm256_mul_ps(y, _mm256_fnmadd_ps(
        _mm256_mul_ps(_mm256_mul_ps(half, x), y), y, three));
    y = _mm256_mul_ps(y, _mm256_fnmadd_ps(
        _mm256_mul_ps(_mm256_mul_ps(half, x), y), y, three));
    return y;
}

// sqrt 通过 rsqrt 推导 (比 sqrtps 快): sqrt(x) = x * rsqrt(x)
__m256 sqrt_via_rsqrt(__m256 x) {
    return _mm256_mul_ps(x, rsqrt_simd(x));
}
```

## 5. 多项式求值 — Estrin Scheme（SIMD ILP 最大化）

```cpp
// 7阶 minimax 多项式, Estrin Scheme: 两条独立链并行计算
// p(x) = c0 + c1*x + c2*x^2 + c3*x^3 + c4*x^4 + c5*x^5 + c6*x^6 + c7*x^7
__m256 poly_estrin_simd(__m256 x,
                        __m256 c0, __m256 c1, __m256 c2, __m256 c3,
                        __m256 c4, __m256 c5, __m256 c6, __m256 c7) {
    __m256 x2 = _mm256_mul_ps(x, x);
    __m256 x4 = _mm256_mul_ps(x2, x2);
    // 两条链完全并行, 无依赖
    __m256 lo = _mm256_fmadd_ps(c1, x, c0);   // c1*x + c0
    __m256 hi = _mm256_fmadd_ps(c3, x, c2);   // c3*x + c2
    __m256 pe = _mm256_fmadd_ps(c5, x, c4);   // c5*x + c4
    __m256 po = _mm256_fmadd_ps(c7, x, c6);   // c7*x + c6
    // 合并: 向量宽度内 8 lane 同时计算
    lo = _mm256_fmadd_ps(hi, x2, lo);
    pe = _mm256_fmadd_ps(po, x2, pe);
    return _mm256_fmadd_ps(pe, x4, lo);
}
```

## 6. SoA 布局 — gather/scatter 零开销

```cpp
// 差: AoS 布局 — 需要 gather 或多次 load
struct Vec3 { float x, y, z; };
void normalize_aos(Vec3* v, size_t n) {
    for (size_t i = 0; i < n; ++i) {
        float len = 1.0f / sqrtf(v[i].x*v[i].x + v[i].y*v[i].y + v[i].z*v[i].z);
        v[i].x *= len; v[i].y *= len; v[i].z *= len;
    }
}

// 好: SoA 布局 — 一次 load 8 个 x, 完全向量化
void normalize_soa(float* x, float* y, float* z, size_t n) {
    for (size_t i = 0; i < n; i += 8) {
        __m256 vx = _mm256_loadu_ps(x + i);
        __m256 vy = _mm256_loadu_ps(y + i);
        __m256 vz = _mm256_loadu_ps(z + i);
        __m256 len2 = _mm256_fmadd_ps(vx, vx,
                       _mm256_fmadd_ps(vy, vy,
                       _mm256_mul_ps(vz, vz)));
        __m256 inv_len = _mm256_rsqrt_ps(len2);
        _mm256_storeu_ps(x + i, _mm256_mul_ps(vx, inv_len));
        _mm256_storeu_ps(y + i, _mm256_mul_ps(vy, inv_len));
        _mm256_storeu_ps(z + i, _mm256_mul_ps(vz, inv_len));
    }
}
```

## 7. 廉价操作 — 用乘倒数消除除法

```cpp
// 差: 向量除法 divps 延迟 11-13 周期
__m256 div_bad(__m256 x, __m256 y) {
    return _mm256_div_ps(x, y);
}

// 好: rcp 逼近 + Newton (4+4+4=12 周期, 但吞吐更高且流水线友好)
__m256 div_rcp_simd(__m256 x, __m256 y) {
    __m256 r = _mm256_rcp_ps(y);             // 4 cycle, ~12bit 精度
    // 一次 Newton: r = r * (2 - y * r)
    r = _mm256_mul_ps(r, _mm256_fnmadd_ps(y, r, _mm256_set1_ps(2.0f)));
    return _mm256_mul_ps(x, r);              // 乘法替代除法
}
```

## 8. 掩码尾部处理（AVX-512 零掩码）

```cpp
// AVX-512: 零掩码操作，无分支尾部处理
void copy_masked_512(float* dst, const float* src, size_t n) {
    size_t i = 0;
    for (; i + 16 <= n; i += 16) {
        _mm512_storeu_ps(dst + i, _mm512_loadu_ps(src + i));
    }
    if (i < n) {
        __mmask16 mask = (1u << (n - i)) - 1;
        _mm512_mask_store_ps(dst + i, mask, _mm512_maskz_loadu_ps(mask, src + i));
    }
}

// AVX/SSE: 用 blend 实现掩码写入（无 AVX-512 掩码）
void copy_blend_256(float* dst, const float* src, size_t n) {
    size_t i = 0;
    for (; i + 8 <= n; i += 8) {
        _mm256_storeu_ps(dst + i, _mm256_loadu_ps(src + i));
    }
    if (i < n) {
        alignas(32) float buf[8] = {};
        // 避免写入 dst 越界的元素: load→blend→store
        for (size_t j = 0; j < n - i; ++j) buf[j] = src[i + j];
        __m256 val = _mm256_load_ps(buf);
        __m256 old = _mm256_loadu_ps(dst + i);
        __m256i mask = _mm256_set_epi32(
            (7 < (int)(n-i-1)) ? -1 : 0,
            (6 < (int)(n-i-1)) ? -1 : 0,
            (5 < (int)(n-i-1)) ? -1 : 0,
            (4 < (int)(n-i-1)) ? -1 : 0,
            (3 < (int)(n-i-1)) ? -1 : 0,
            (2 < (int)(n-i-1)) ? -1 : 0,
            (1 < (int)(n-i-1)) ? -1 : 0,
            (0 < (int)(n-i-1)) ? -1 : 0
        );
        _mm256_storeu_ps(dst + i, _mm256_blendv_ps(old, val, _mm256_castsi256_ps(mask)));
    }
}
```

## 9. exp 向量化实现 — 综合示例

```cpp
// 向量化 expf: 范围归约 + minimax 多项式 + 指数重构
// Inf/NaN/±0 零分支收敛
__m256 exp_simd(__m256 x) {
    const __m256 log2e  = _mm256_set1_ps(1.442695041f);
    const __m256 c0     = _mm256_set1_ps(0.5f);
    const __m256 c1     = _mm256_set1_ps(1.0f / 6.0f);
    const __m256 c2     = _mm256_set1_ps(1.0f / 24.0f);
    const __m256 c3     = _mm256_set1_ps(1.0f / 120.0f);
    const __m256 c4     = _mm256_set1_ps(1.0f / 720.0f);
    const __m256 min_in = _mm256_set1_ps(-87.3365f);   // exp 下溢边界
    const __m256 max_in = _mm256_set1_ps(88.7228f);    // exp 上溢边界

    // clamp 输入, 无分支: x = max(min_in, min(max_in, x))
    // x=NaN → min(NaN, max_in)=NaN → max(min_in, NaN)=NaN (自然传播)
    // x=Inf → min(Inf, max_in)=max_in → max(min_in, max_in)=max_in → exp(max_in)=Inf ✓
    // x=-Inf → min(-Inf, max_in)=-Inf → max(min_in, -Inf)=min_in → exp(min_in)=0 ✓
    x = _mm256_max_ps(min_in, _mm256_min_ps(max_in, x));

    // 整数指数: k = round(x * log2(e))
    __m256 t = _mm256_mul_ps(x, log2e);
    __m256i k = _mm256_cvtps_epi32(_mm256_round_ps(t, _MM_FROUND_TO_NEAREST_INT));
    __m256 f = _mm256_fnmadd_ps(_mm256_cvtepi32_ps(k), _mm256_set1_ps(0.69314718f), x);

    // minimax 多项式: exp(f), f∈[-0.34657, 0.34657]
    __m256 f2 = _mm256_mul_ps(f, f);
    __m256 poly = _mm256_fmadd_ps(c4, f, c3);
    poly = _mm256_fmadd_ps(poly, f, c2);
    poly = _mm256_fmadd_ps(poly, f, c1);
    poly = _mm256_fmadd_ps(poly, f, c0);
    poly = _mm256_fmadd_ps(poly, f2, f);     // f + poly*f2
    poly = _mm256_add_ps(poly, _mm256_set1_ps(1.0f));

    // 指数重构: result = poly * 2^k
    k = _mm256_add_epi32(k, _mm256_set1_epi32(127));
    k = _mm256_slli_epi32(k, 23);
    __m256 result = _mm256_mul_ps(poly, _mm256_castsi256_ps(k));
    return result;
}
```

## 10. pairwise 归约 — 完全并行版 vs Kahan

```cpp
// 向量化 pairwise 求和: O(log₂N) 步, 每步完全并行
// N=256 → 8 步 (vs Kahan 的 256 步串行)
float pairwise_simd(const float* data, size_t n) {
    // 将数据按 8 lane 分组, 垂直累加
    __m256 sum0 = _mm256_setzero_ps(), sum1 = _mm256_setzero_ps();
    __m256 sum2 = _mm256_setzero_ps(), sum3 = _mm256_setzero_ps();
    size_t i = 0;
    for (; i + 32 <= n; i += 32) {
        sum0 = _mm256_add_ps(sum0, _mm256_loadu_ps(data + i));
        sum1 = _mm256_add_ps(sum1, _mm256_loadu_ps(data + i + 8));
        sum2 = _mm256_add_ps(sum2, _mm256_loadu_ps(data + i + 16));
        sum3 = _mm256_add_ps(sum3, _mm256_loadu_ps(data + i + 24));
    }
    sum0 = _mm256_add_ps(sum0, sum1);
    sum2 = _mm256_add_ps(sum2, sum3);
    sum0 = _mm256_add_ps(sum0, sum2);

    // 树形归约 8→1
    __m128 hi = _mm256_extractf128_ps(sum0, 1);
    __m128 lo = _mm256_castps256_ps128(sum0);
    __m128 r  = _mm_add_ps(lo, hi);
    r = _mm_hadd_ps(r, r);
    r = _mm_hadd_ps(r, r);
    float result = _mm_cvtss_f32(r);
    for (; i < n; ++i) result += data[i];
    return result;
}
```
