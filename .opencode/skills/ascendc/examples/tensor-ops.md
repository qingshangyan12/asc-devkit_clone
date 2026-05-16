# C++ 代码到 AscendC 代码：Tensor 操作转换示例

本文档提供标准 C++ 代码到 AscendC 算子代码的完整转换示例，涵盖常见的 Tensor 操作场景。

---

## 示例 1：igamma中的helper函数

### 标准 C++ 参考实现

```cpp
// num和den为一个长为13的常量数组，m和n均为12
static float Igammac_lanczos_sum_expg_scaled_float(float x, const float* num, int64_t m, const float* den, int64_t n)
{
    int64_t i;
    int64_t dir;
    float y;
    float num_ans;
    float den_ans;
    float absx = fabsf(x);
    const float* p;

    if (absx > 1.0f) {
        dir = -1;
        p = num + m;
        y = 1.0f / x;
    } else {
        dir = 1;
        p = num;
        y = x;
    }

    num_ans = *p;
    p += dir;
    for (i = 1; i <= m; i++) {
        num_ans = num_ans * y + *p;
        p += dir;
    }

    p = (absx > 1.0f) ? (den + n) : den;

    den_ans = *p;
    p += dir;
    for (i = 1; i <= n; i++) {
        den_ans = den_ans * y + *p;
        p += dir;
    }

    if (absx > 1.0f) {
        i = n - m;
        return powf(x, float(i) * num_ans / den_ans);
    }
    return num_ans / den_ans;
}

```

### AscendC 使用RegTensor 实现

```cpp
#include "kernel_operator.h"

// Recursively unroll the loop for numerator computation
template <int32_t iterationNum>
__simd_callee__ inline void LanczosNumIter(
    Reg::RegTensor<float>& numAnsReg, Reg::RegTensor<float>& yReg,
    Reg::MaskReg& mask0, Reg::MaskReg& mask1, Reg::MaskReg& fullMask)
{
    Reg::RegTensor<float> tmpReg;

    // num_ans = num_ans * y + *p
    Reg::Mul(tmpReg, numAnsReg, yReg, fullMask);
    Reg::Adds(numAnsReg, tmpReg, kLanczosNum[13 - iterationNum], mask1);
    Reg::Adds(tmpReg, tmpReg, kLanczosNum[iterationNum-1], mask0);
    Reg::Copy<float, Reg::MaskMergeMode::MERGING>(numAnsReg, tmpReg, mask0);
    if constexpr (iterationNum > 1) {
        LanczosNumIter<iterationNum - 1>(numAnsReg, yReg, mask0, mask1, fullMask);
    }
}

// Recursively unroll the loop for denominator computation
template <int32_t iterationNum>
__simd_callee__ inline void LanczosDenIter(
    Reg::RegTensor<float>& denAnsReg, Reg::RegTensor<float>& yReg,
    Reg::MaskReg& mask0, Reg::MaskReg& mask1, Reg::MaskReg& fullMask)
{
    Reg::RegTensor<float> tmpReg;

    // den_ans = den_ans * y + *p
    Reg::Mul(tmpReg, denAnsReg, yReg, fullMask);
    Reg::Adds(denAnsReg, tmpReg, kLanczosDen[13 - iterationNum], mask1);
    Reg::Adds(tmpReg, tmpReg, kLanczosDen[iterationNum - 1], mask0);
    Reg::Copy<float, Reg::MaskMergeMode::MERGING>(denAnsReg, tmpReg, mask0);

    if constexpr (iterationNum > 1) {
        LanczosDenIter<iterationNum - 1>(denAnsReg, yReg, mask0, mask1, fullMask);
    }
}

__simd_callee__ inline void Igammac_lanczos_sum_expg_scaled_float_VFimpl(Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& src0Reg, Reg::MaskReg mask)
{
    Reg::MaskReg fullMask = Reg::CreateMask<float, Reg::MaskPattern::ALL>();
    Reg::RegTensor<float> tmpReg, tmpReg1;

    // if abs(a) > 1.0f
    Reg::RegTensor<float> absReg;
    Reg::RegTensor<float> yReg;
    Reg::MaskReg mask0;
    Reg::Abs(absReg, src0Reg, fullMask);
    Reg::Compares<float, CMPMODE::GT>(mask0, absReg, 1.0f, fullMask);
    // p is a pointer, dir should move a pointer
    Reg::Duplicate<float, Reg::MaskMergeMode::MERGING>(tmpReg, 1.0f, mask0);
    Reg::Div(yReg, tmpReg, src0Reg, mask0);

    // else abs(a) <= 1.0f
    Reg::MaskReg mask1;
    Reg::Not(mask1, mask0, fullMask);
    Reg::Copy<float, Reg::MaskMergeMode::MERGING>(yReg, src0Reg, mask1);

    // num_ans = *p
    // *p在不同的位置被dup进了numAnsReg，所以numAnsReg就是*p
    Reg::RegTensor<float> numAnsReg;
    Reg::Duplicate<float, Reg::MaskMergeMode::MERGING>(numAnsReg, kLanczosNum[12], mask0);
    Reg::Duplicate<float, Reg::MaskMergeMode::MERGING>(numAnsReg, kLanczosNum[0], mask1);

    // Recursively unroll the loop for numerator
    LanczosNumIter<12>(numAnsReg, yReg, mask0, mask1, fullMask);

    // p = (absx > 1.0f) ? (den + n) : den
    // den_ans = *p
    // reuse mask0, absReg
    Reg::RegTensor<float> denAnsReg;
    Reg::Duplicate<float, Reg::MaskMergeMode::MERGING>(denAnsReg, kLanczosDen[12], mask0);
    Reg::Duplicate<float, Reg::MaskMergeMode::MERGING>(denAnsReg, kLanczosDen[0], mask1);

    // Recursively unroll the loop for denominator
    LanczosDenIter<12>(denAnsReg, yReg, mask0, mask1, fullMask);

    // if (absx > 1.0f)
    // return (powf(x ,0.0f) * num_ans / den_ans)
    // replace it with return num_ans / den_ans, if nan input happens to be wrong,look here
    Reg::Div(dstReg, numAnsReg, denAnsReg, fullMask);
}

```

> **核心原则**：AscendC 中**所有操作都是向量化的**——一次 API 调用即完成整个 Tile 的计算。不需要编写逐元素的 for 循环（那是 C++ 宿主代码的模式）。这是从 C++ 到 AscendC 编程思维转换的关键。
