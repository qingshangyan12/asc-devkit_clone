#include "kernel_operator.h"

using namespace AscendC;

// ============================================================================
// AscendC RegBase log1p (3510) : 标量算法→向量寄存器
//
// 对标实现:
//   float my_log1p(float x) {
//       float u = x + 1.0f;
//       if (u == 1.0f) return x;        // |x| so small
//       if (u == x)   return logf(x);   // |x| so large or +/-Inf
//       return logf(u) * x / (u - 1.0f);
//   }
//
// 分支消除: CompareScalar + Select 实现三路选择，零分支
// 特殊值收敛: log(NaN)=NaN, log(Inf)=Inf, 1/0=Inf 等 IEEE 754 自然传播
// ============================================================================

// ---- Layer 2: 向量寄存器计算 ----
template <typename T>
__simd_vf__ inline void Log1pCompute(
    __ubuf__ T* dstUb, __ubuf__ T* srcUb,
    uint32_t calCount, uint16_t repeatTimes)
{
    constexpr uint32_t vlSize = GetVecLen() / sizeof(T);

    Reg::RegTensor<T> xReg, uReg;
    Reg::RegTensor<T> logUReg, logXReg;
    Reg::RegTensor<T> denomReg, defaultReg, tempReg, resultReg;
    Reg::RegTensor<T> oneReg;
    Reg::MaskReg mask, maskSmall, maskLarge;

    Reg::Duplicate(oneReg, static_cast<T>(1.0f));

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = Reg::UpdateMask<T>(calCount);

        Reg::LoadAlign<T>(xReg, srcUb + i * vlSize);

        // u = x + 1.0f
        Reg::Add(uReg, xReg, oneReg, mask);

        // ---- 分支消除: 两路比较 ----
        Reg::CompareScalar<T, CMPMODE::EQ>(maskSmall, uReg, static_cast<T>(1.0f), mask);
        Reg::CompareScalar<T, CMPMODE::EQ>(maskLarge, uReg, xReg, mask);

        // ---- 三条路径全部计算（投机执行） ----
        Reg::Log(logUReg, uReg, mask);
        Reg::Log(logXReg, xReg, mask);

        // denom = u - 1.0f
        Reg::Sub(denomReg, uReg, oneReg, mask);

        // default = log(u) * x / (u - 1.0f)
        Reg::Mul(defaultReg, logUReg, xReg, mask);
        Reg::Div(defaultReg, defaultReg, denomReg, mask);

        // Select: maskSmall → x ; otherwise → default
        Reg::Select(tempReg, xReg, defaultReg, maskSmall);

        // Select: maskLarge → log(x) ; otherwise → temp
        Reg::Select(resultReg, logXReg, tempReg, maskLarge);

        Reg::StoreAlign<T>(dstUb + i * vlSize, resultReg, mask);
    }
}

// ---- Layer 1: 入口函数 ----
template <typename T>
__aicore__ inline void Log1pImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const uint32_t calCount)
{
    if ASCEND_IS_AIC { return; }

    static_assert(SupportType<T, float, half>(),
        "log1p only supports float/half data type on current device");

    __ubuf__ T* srcUb = (__ubuf__ T*)srcTensor.GetPhyAddr();
    __ubuf__ T* dstUb = (__ubuf__ T*)dstTensor.GetPhyAddr();
    constexpr uint32_t vlSize = GetVecLen() / sizeof(T);
    uint16_t repeatTimes = static_cast<uint16_t>(CeilDivision(calCount, vlSize));

    Log1pCompute<T>(dstUb, srcUb, calCount, repeatTimes);
}
