#include "kernel_operator.h"

using namespace AscendC;

// ============================================================================
// AscendC RegBase erfcx (3510) : 标量算法→向量寄存器
//
// 对标实现:
//   float my_erfcx(float x) 两大区域:
//     |x| < 10.055  → 有理逼近  p(t)/(2|x|+1),  t=(|x|-4)/(|x|+4)
//     |x| ≥ 10.055  → 渐近展开  1/(√π·x)·(1−1/(2x²)+3/(4x⁴)−15/(8x⁶)+105/(16x⁸))
//     负x → 恒等式  erfcx(x)=2·exp(x²)−erfcx(−x)
//
// 分支消除: CompareScalar + Select 实现区域选择和负x处理
// 特殊值收敛: Inf/NaN/±0 依赖 IEEE 754 自然传播，无单独分支
// ============================================================================

// ---- Layer 2: 向量寄存器计算 ----
template <typename T>
__simd_vf__ inline void ErfcxCompute(
    __ubuf__ T* dstUb, __ubuf__ T* srcUb,
    uint32_t calCount, uint16_t repeatTimes)
{
    constexpr uint32_t vlSize = GetVecLen() / sizeof(T);

    // ---- 常量 ----
    constexpr T kThresh    = static_cast<T>(10.055f);
    constexpr T kInvSqrtPi = static_cast<T>(0.5641895835477563f);

    // Degree-10 多项式系数 (c10→c0), Horner 求值用, fitted on [0, 10.055]
    constexpr T kPolyC10 = static_cast<T>(0.000803186f);
    constexpr T kPolyC9  = static_cast<T>(0.006800647f);
    constexpr T kPolyC8  = static_cast<T>(-0.016062400f);
    constexpr T kPolyC7  = static_cast<T>(0.036449701f);
    constexpr T kPolyC6  = static_cast<T>(-0.066562777f);
    constexpr T kPolyC5  = static_cast<T>(0.093829234f);
    constexpr T kPolyC4  = static_cast<T>(-0.101000630f);
    constexpr T kPolyC3  = static_cast<T>(0.068091795f);
    constexpr T kPolyC2  = static_cast<T>(0.015377827f);
    constexpr T kPolyC1  = static_cast<T>(-0.139621016f);
    constexpr T kPolyC0  = static_cast<T>(1.232995131f);

    // 渐近展开系数 (c4→c0 = z⁴→z⁰)
    constexpr T kAsymC4 = static_cast<T>(6.5625f);       // 105/16
    constexpr T kAsymC3 = static_cast<T>(-1.875f);       // -15/8
    constexpr T kAsymC2 = static_cast<T>(0.75f);         // 3/4
    constexpr T kAsymC1 = static_cast<T>(-0.5f);         // -1/2
    constexpr T kAsymC0 = static_cast<T>(1.0f);

    // ---- 常数寄存器（一次性初始化，跨迭代复用） ----
    Reg::RegTensor<T> oneReg, twoReg, fourReg, negTwoReg;
    Reg::RegTensor<T> kThreshReg, kInvSqrtPiReg;

    Reg::Duplicate(oneReg,       static_cast<T>(1.0f));
    Reg::Duplicate(twoReg,       static_cast<T>(2.0f));
    Reg::Duplicate(fourReg,      static_cast<T>(4.0f));
    Reg::Duplicate(negTwoReg,    static_cast<T>(-2.0f));
    Reg::Duplicate(kThreshReg,   kThresh);
    Reg::Duplicate(kInvSqrtPiReg, kInvSqrtPi);

    // ---- 计算寄存器 ----
    Reg::RegTensor<T> xReg, axReg;
    Reg::RegTensor<T> tReg, pReg, qReg, resultReg;
    Reg::RegTensor<T> tmpReg, coeffReg;
    Reg::RegTensor<T> invXReg, zReg, asymReg;
    Reg::RegTensor<T> xxReg, expReg, negResultReg, posResultReg;
    Reg::MaskReg mask, maskRational, maskNeg;

    // 残差校正用临时寄存器
    Reg::RegTensor<T> invAp4Reg, residualReg;
    Reg::RegTensor<T> invDenomReg;

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = Reg::UpdateMask<T>(calCount);

        Reg::LoadAlign<T>(xReg, srcUb + i * vlSize);

        // ───── ax = |x| ─────
        Reg::Abs(axReg, xReg, mask);

        // ───── 区域掩码: rational (|x| < 10.055) ─────
        Reg::CompareScalar<T, CMPMODE::LT>(maskRational, axReg, kThreshReg, mask);

        // ====================================================================
        //  路径 A: 有理逼近  (全部 lane 投机计算)
        // ====================================================================

        // t = (ax - 4) / (ax + 4)  基本映射
        Reg::Add(tmpReg, axReg, fourReg, mask);            // ax+4
        Reg::Div(tReg, oneReg, tmpReg, mask);              // tReg = 1/(ax+4) = inv_ap4
        // 暂存 inv_ap4 供残差校正用
        // Reg::DataCopy not needed — just use tReg as inv_ap4 for now
        // Reg::RegTensor: re-derive inv_ap4 later if needed

        Reg::Sub(tmpReg, axReg, fourReg, mask);            // ax-4
        Reg::Mul(tReg, tmpReg, tReg, mask);                // t = (ax-4)/(ax+4)

        // ---- 残差校正: residual_t = ax*(1−t) − 4*(t+1) ----
        // 使用 invAp4Reg 暂存
        Reg::Sub(invAp4Reg, oneReg, tReg, mask);           // 1 - t
        Reg::Mul(invAp4Reg, axReg, invAp4Reg, mask);       // ax*(1-t)
        Reg::Add(tmpReg, tReg, oneReg, mask);              // t+1
        Reg::Mul(tmpReg, fourReg, tmpReg, mask);           // 4*(t+1)
        Reg::Sub(residualReg, invAp4Reg, tmpReg, mask);    // residual_t = ax*(1-t) − 4*(t+1)

        // 重新计算 inv_ap4
        Reg::Add(tmpReg, axReg, fourReg, mask);            // ax+4
        Reg::Div(invAp4Reg, oneReg, tmpReg, mask);         // 1/(ax+4)

        // t_new = t + residual_t / (ax+4)
        Reg::Mul(residualReg, residualReg, invAp4Reg, mask); // residual*inv_ap4
        Reg::Add(tReg, tReg, residualReg, mask);            // t = t + correction

        // ---- Horner 多项式求值 p(t), deg=10 ----
        Reg::Duplicate(pReg, kPolyC10);
        Reg::Duplicate(coeffReg, kPolyC9);  Reg::MulDstAdd(pReg, tReg, coeffReg, mask);
        Reg::Duplicate(coeffReg, kPolyC8);  Reg::MulDstAdd(pReg, tReg, coeffReg, mask);
        Reg::Duplicate(coeffReg, kPolyC7);  Reg::MulDstAdd(pReg, tReg, coeffReg, mask);
        Reg::Duplicate(coeffReg, kPolyC6);  Reg::MulDstAdd(pReg, tReg, coeffReg, mask);
        Reg::Duplicate(coeffReg, kPolyC5);  Reg::MulDstAdd(pReg, tReg, coeffReg, mask);
        Reg::Duplicate(coeffReg, kPolyC4);  Reg::MulDstAdd(pReg, tReg, coeffReg, mask);
        Reg::Duplicate(coeffReg, kPolyC3);  Reg::MulDstAdd(pReg, tReg, coeffReg, mask);
        Reg::Duplicate(coeffReg, kPolyC2);  Reg::MulDstAdd(pReg, tReg, coeffReg, mask);
        Reg::Duplicate(coeffReg, kPolyC1);  Reg::MulDstAdd(pReg, tReg, coeffReg, mask);
        Reg::Duplicate(coeffReg, kPolyC0);  Reg::MulDstAdd(pReg, tReg, coeffReg, mask);

        // ---- 补偿除法 q = p / (2·ax + 1) ----
        // denom = 2 · ax + 1
        Reg::Fma(tmpReg, twoReg, axReg, oneReg, mask);
        // inv_denom = 1 / denom
        Reg::Div(invDenomReg, oneReg, tmpReg, mask);
        // q = p * inv_denom
        Reg::Mul(qReg, pReg, invDenomReg, mask);
        // residual_q = fma(ax, -2*q, p) - q
        //           = p - 2*q*ax - q
        Reg::Mul(residualReg, qReg, axReg, mask);           // q * ax
        Reg::Fma(residualReg, negTwoReg, residualReg, pReg, mask);
        Reg::Sub(residualReg, residualReg, qReg, mask);     // residual_q

        // rational_result = q + residual_q * inv_denom
        Reg::Fma(resultReg, residualReg, invDenomReg, qReg, mask);

        // resultReg 暂存 rational 路径结果，后续与 asymptotic 做 Select
        // 直接用 pReg 保存 rational 结果以释放 resultReg
        Reg::RegTensor<T> rationalReg;
        Reg::Select(rationalReg, resultReg, resultReg, mask); // copy

        // ====================================================================
        //  路径 B: 渐近展开  (全部 lane 投机计算)
        // ====================================================================

        // inv_x = 1 / ax
        Reg::Div(invXReg, oneReg, axReg, mask);
        // z = inv_x^2
        Reg::Mul(zReg, invXReg, invXReg, mask);

        // Horner 多项式求值 (in z)
        Reg::Duplicate(asymReg, kAsymC4);
        Reg::Duplicate(coeffReg, kAsymC3);  Reg::MulDstAdd(asymReg, zReg, coeffReg, mask);
        Reg::Duplicate(coeffReg, kAsymC2);  Reg::MulDstAdd(asymReg, zReg, coeffReg, mask);
        Reg::Duplicate(coeffReg, kAsymC1);  Reg::MulDstAdd(asymReg, zReg, coeffReg, mask);
        Reg::Duplicate(coeffReg, kAsymC0);  Reg::MulDstAdd(asymReg, zReg, coeffReg, mask);

        // result = kInvSqrtPi * inv_x * poly
        Reg::Mul(asymReg, kInvSqrtPiReg, asymReg, mask);
        Reg::Mul(asymReg, invXReg, asymReg, mask);

        // ───── 区域选择: pos_result = rational (|x|<10.055) ? rational : asymptotic ─────
        Reg::Select(posResultReg, rationalReg, asymReg, maskRational);

        // ====================================================================
        //  负 x 处理: erfcx(x) = 2·exp(x²) − erfcx(−x)
        // ====================================================================

        // maskNeg = (x < 0)
        Reg::CompareScalar<T, CMPMODE::LT>(maskNeg, xReg, static_cast<T>(0.0f), mask);

        // x² (所有 lane)
        Reg::Mul(xxReg, xReg, xReg, mask);
        // exp(x²) — 正x路径可能溢出至Inf，但Select会丢弃该lane的值
        Reg::Exp(expReg, xxReg, mask);

        // negResult = 2·exp(x²) − posResult
        Reg::Mul(negResultReg, twoReg, expReg, mask);
        Reg::Sub(negResultReg, negResultReg, posResultReg, mask);

        // ───── 最终选择: x<0 → negResult ; 否则 → posResult ─────
        Reg::Select(resultReg, negResultReg, posResultReg, maskNeg);

        // ───── 写回 UB ─────
        Reg::StoreAlign<T>(dstUb + i * vlSize, resultReg, mask);
    }
}

// ---- Layer 1: 入口函数 ----
template <typename T>
__aicore__ inline void ErfcxImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const uint32_t calCount)
{
    if ASCEND_IS_AIC { return; }

    static_assert(SupportType<T, float, half>(),
        "erfcx only supports float/half data type on current device");

    __ubuf__ T* srcUb = (__ubuf__ T*)srcTensor.GetPhyAddr();
    __ubuf__ T* dstUb = (__ubuf__ T*)dstTensor.GetPhyAddr();
    constexpr uint32_t vlSize = GetVecLen() / sizeof(T);
    uint16_t repeatTimes = static_cast<uint16_t>(CeilDivision(calCount, vlSize));

    ErfcxCompute<T>(dstUb, srcUb, calCount, repeatTimes);
}
