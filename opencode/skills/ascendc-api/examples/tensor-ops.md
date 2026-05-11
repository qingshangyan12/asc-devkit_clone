# 3510 RegBase API 开发示例

本文档提供 3510 架构下 RegBase 计算类 API 的开发示例，覆盖从简单一元运算到复杂组合计算的完整模式。

---

## 示例 1：简单一元运算 — Ceil

### 标准 C++ 参考

```cpp
void ceil_kernel(const float* x, float* y, int n) {
    for (int i = 0; i < n; i++) {
        y[i] = std::ceil(x[i]);
    }
}
```

### 3510 RegBase 实现

```cpp
#include "kernel_tensor.h"
#include "kernel_basic_intf.h"
#include "../../common/check.h"

namespace AscendC {

template <typename T, bool isReuseSource = false>
__simd_vf__ inline void CeilCompute(
    __ubuf__ T* dstUb, __ubuf__ T* srcUb, uint32_t calCount, const uint16_t repeatTimes)
{
    constexpr uint32_t repeatElm = GetVecLen() / sizeof(T);
    Reg::RegTensor<T> srcReg;
    Reg::RegTensor<T> dstReg;
    Reg::MaskReg maskReg;
    for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTimes); ++i) {
        maskReg = Reg::UpdateMask<T>(calCount);
        Reg::LoadAlign<T>(srcReg, srcUb + i * repeatElm);
        Reg::Truncate<T, RoundMode::CAST_CEIL>(dstReg, srcReg, maskReg);
        Reg::StoreAlign<T>(dstUb + i * repeatElm, dstReg, maskReg);
    }
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void CeilImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const uint32_t calCount)
{
    if ASCEND_IS_AIC { return; }

    static_assert(SupportType<T, float, half>(),
        "Ceil only support half/float data type on current device");
    CheckTensorPosition(dstTensor, "dstTensor", "VECIN, VECOUT, VECCALC");
    CheckTensorPosition(srcTensor, "srcTensor", "VECIN, VECOUT, VECCALC");
    CheckCalCount(calCount, "calCount", srcTensor, "srcTensor", "Round");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Round");

    __ubuf__ T* srcUb = (__ubuf__ T*)srcTensor.GetPhyAddr();
    __ubuf__ T* dstUb = (__ubuf__ T*)dstTensor.GetPhyAddr();
    constexpr uint32_t repeatElm = GetVecLen() / sizeof(T);
    uint16_t repeatTimes = static_cast<uint16_t>(CeilDivision(calCount, repeatElm));

    CeilCompute<T, isReuseSource>(dstUb, srcUb, calCount, repeatTimes);
}

} // namespace AscendC
```

---

## 示例 2：带标量的一元运算 — ClampMax / ClampMin

### 标准 C++ 参考

```cpp
void clamp_max(const float* x, float* y, float maxVal, int n) {
    for (int i = 0; i < n; i++) {
        y[i] = std::min(x[i], maxVal);
    }
}

void clamp_min(const float* x, float* y, float minVal, int n) {
    for (int i = 0; i < n; i++) {
        y[i] = std::max(x[i], minVal);
    }
}
```

### 3510 RegBase 实现

```cpp
namespace AscendC {

template <typename T, CLAMPMODE selMode, bool isReuseSource = false>
__simd_vf__ inline void ClampCompute(
    __ubuf__ T* dstUb, __ubuf__ T* srcUb, const T scalar,
    uint32_t calCount, const uint16_t repeatTimes)
{
    constexpr uint32_t repeatElm = GetVecLen() / sizeof(T);
    Reg::RegTensor<T> srcReg, dstReg;
    Reg::MaskReg maskReg;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        maskReg = Reg::UpdateMask<T>(calCount);
        Reg::LoadAlign<T>(srcReg, srcUb + i * repeatElm);
        if constexpr (selMode == CLAMPMODE::CLAMP_MAX) {
            Reg::Mins(dstReg, srcReg, scalar, maskReg);
        } else {
            Reg::Maxs(dstReg, srcReg, scalar, maskReg);
        }
        Reg::StoreAlign<T>(dstUb + i * repeatElm, dstReg, maskReg);
    }
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void ClampMaxImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const LocalTensor<uint8_t>& sharedTmpBuffer,
    const T scalar, const uint32_t calCount)
{
    if ASCEND_IS_AIC { return; }

    static_assert(SupportType<T, float, half>(),
        "ClampMax only support half/float data type on current device");
    CheckTensorPosition(dstTensor, "dstTensor", "VECIN, VECOUT, VECCALC");
    CheckTensorPosition(srcTensor, "srcTensor", "VECIN, VECOUT, VECCALC");
    CheckTensorPosition(sharedTmpBuffer, "sharedTmpBuffer", "VECIN, VECOUT, VECCALC");
    CheckCalCount(calCount, "calCount", srcTensor, "srcTensor", "ClampMax");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "ClampMax");

    __ubuf__ T* srcUb = (__ubuf__ T*)srcTensor.GetPhyAddr();
    __ubuf__ T* dstUb = (__ubuf__ T*)dstTensor.GetPhyAddr();
    constexpr uint32_t repeatElm = GetVecLen() / sizeof(T);
    uint16_t repeatTimes = static_cast<uint16_t>(CeilDivision(calCount, repeatElm));
    ClampCompute<T, CLAMPMODE::CLAMP_MAX, isReuseSource>(
        dstUb, srcUb, scalar, calCount, repeatTimes);
}

} // namespace AscendC
```

---

## 示例 3：二元运算 — 按位异或

### 标准 C++ 参考

```cpp
void bitwise_xor(const int16_t* a, const int16_t* b, int16_t* c, int n) {
    for (int i = 0; i < n; i++) {
        c[i] = a[i] ^ b[i];
    }
}
```

### 3510 RegBase 实现

```cpp
#include "kernel_basic_intf.h"
#include "../../api_check/kernel_api_check.h"

namespace AscendC {
namespace XorAPI {

template <typename T>
__simd_vf__ inline void XorCompute(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1,
    uint32_t calCount, uint16_t repeatTimes)
{
    Reg::MaskReg mask;
    Reg::RegTensor<T> dstVreg, src0Vreg, src1Vreg;
    constexpr uint32_t oneRepSize = GetVecLen() / sizeof(T);
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = Reg::UpdateMask<T>(calCount);
        Reg::LoadAlign(src0Vreg, src0 + i * oneRepSize);
        Reg::LoadAlign(src1Vreg, src1 + i * oneRepSize);
        Reg::Xor(dstVreg, src0Vreg, src1Vreg, mask);
        Reg::StoreAlign(dst + i * oneRepSize, dstVreg, mask);
    }
}

} // namespace XorAPI

template <typename T, bool isReuseSource = false>
__aicore__ inline void XorImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor,
    const LocalTensor<T>& src1Tensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
    if ASCEND_IS_AIC { return; }

    CHECK_FUNC_HIGHLEVEL_API(Xor, (T, isReuseSource),
        (dstTensor, src0Tensor, src1Tensor, sharedTmpBuffer, calCount));

    static_assert(SupportType<T, uint16_t, int16_t>(),
        "current data type is not supported on current device!");

    __ubuf__ T* dst = (__ubuf__ T*)dstTensor.GetPhyAddr();
    __ubuf__ T* src0 = (__ubuf__ T*)src0Tensor.GetPhyAddr();
    __ubuf__ T* src1 = (__ubuf__ T*)src1Tensor.GetPhyAddr();
    constexpr uint32_t oneRepSize = GetVecLen() / sizeof(T);
    uint16_t repeatTimes = CeilDivision(calCount, oneRepSize);
    XorAPI::XorCompute<T>(dst, src0, src1, calCount, repeatTimes);
}

} // namespace AscendC
```

---

## 示例 4：比较+选择组合 — Sign

### 标准 C++ 参考

```cpp
void sign(const float* x, float* y, int n) {
    for (int i = 0; i < n; i++) {
        if (x[i] > 0)      y[i] = 1;
        else if (x[i] < 0) y[i] = -1;
        else               y[i] = 0;
    }
}
```

### 3510 RegBase 实现

```cpp
#include "kernel_basic_intf.h"
#include "kernel_tensor.h"
#include "../../common/check.h"

namespace AscendC {
namespace SignInternal {

template <typename T, typename RegT, const Reg::RegTrait& trait = Reg::RegTraitNumOne>
__simd_vf__ inline void SignCoreCompute(
    __ubuf__ T* dstUb, __ubuf__ T* srcUb,
    uint32_t calCount, uint16_t repeatTime, uint32_t vlSize)
{
    Reg::MaskReg signMask, cmpMask0, cmpMask1;
    RegT brcZeroReg, brcOneReg, brcNegOneReg, srcReg, selReg0, selReg1;

    // 广播常数寄存器
    Reg::Duplicate(brcZeroReg, 0);
    Reg::Duplicate(brcOneReg, 1);
    Reg::Duplicate(brcNegOneReg, -1);

    for (uint16_t i = 0; i < repeatTime; ++i) {
        signMask = Reg::UpdateMask<T, trait>(calCount);
        Reg::LoadAlign(srcReg, srcUb + i * vlSize);

        // src < 0 → cmpMask0
        Reg::CompareScalar<T, CMPMODE::LT>(cmpMask0, srcReg, 0, signMask);
        // src > 0 → cmpMask1
        Reg::CompareScalar<T, CMPMODE::GT>(cmpMask1, srcReg, 0, signMask);

        // sel0 = cmpMask0 ? -1 : 0
        Reg::Select(selReg0, brcNegOneReg, brcZeroReg, cmpMask0);
        // sel1 = cmpMask1 ? 1 : sel0
        Reg::Select(selReg1, brcOneReg, selReg0, cmpMask1);

        Reg::StoreAlign(dstUb + i * vlSize, selReg1, signMask);
    }
}

} // namespace SignInternal

template <typename T, bool isReuseSource = false>
__aicore__ inline void SignCompute(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const uint32_t calCount)
{
    static_assert(SupportType<T, half, float, int64_t>(),
        "Sign only support half/float/int64_t data type on current device!");
    CheckTensorPosition(dstTensor, "dstTensor", "VECIN, VECOUT, VECCALC");
    CheckTensorPosition(srcTensor, "srcTensor", "VECIN, VECOUT, VECCALC");
    CheckCalCount(calCount, "calCount", srcTensor, "srcTensor", "Sign");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Sign");

    __ubuf__ T* dstUb = (__ubuf__ T*)dstTensor.GetPhyAddr();
    __ubuf__ T* srcUb = (__ubuf__ T*)srcTensor.GetPhyAddr();

    if constexpr (sizeof(T) == 8) {
        using RegT = Reg::RegTensor<T, Reg::RegTraitNumTwo>;
        constexpr int32_t vlSize = GetVecLen() / sizeof(T) * 2;
        uint16_t repeatTime = CeilDivision(calCount, vlSize);
        SignInternal::SignCoreCompute<T, RegT, Reg::RegTraitNumTwo>(
            dstUb, srcUb, calCount, repeatTime, vlSize);
    } else {
        using RegT = Reg::RegTensor<T>;
        constexpr int32_t vlSize = GetVecLen() / sizeof(T);
        uint16_t repeatTime = CeilDivision(calCount, vlSize);
        SignInternal::SignCoreCompute<T, RegT>(
            dstUb, srcUb, calCount, repeatTime, vlSize);
    }
}

} // namespace AscendC
```

本示例展示了：
- `Duplicate` 创建常数寄存器
- `CompareScalar` 产生条件掩码
- `Select` 基于掩码的条件赋值
- `sizeof(T) == 8` 的 64 位类型分支处理

---

## 示例 5：复杂组合运算 — Tan

### 标准 C++ 参考

```cpp
void tan_kernel(const float* x, float* y, int n) {
    for (int i = 0; i < n; i++) {
        y[i] = std::tan(x[i]);
    }
}
```

### 3510 RegBase 实现（简化示意）

Tan 的实现展示了 3510 下复杂的组合运算模式：先将输入归一化到 `(-pi/2, pi/2)`，再用多项式逼近计算 tan 值。核心包含：

1. **除法性质等数值常量**：使用 Reg::Muls/Sub/Adds 组合实现 `x = x - round(x/PI) * PI`
2. **多项式逼近**：`tan(x) ≈ x·P(x²) / ((pi/2-x)(pi/2+x)·Q(x²))`，其中 P、Q 为多项式
3. **half→float 转换**：使用 `Reg::LoadDist::DIST_UNPACK_B16` 加载，`Reg::Cast` 转 float，计算完成后用 `Reg::Cast` 转回 half，`Reg::StoreDist::DIST_PACK_B32` 存回

```cpp
namespace AscendC {
namespace TanInternal {

constexpr Reg::CastTrait TAN_F162F32 = {
    Reg::RegLayout::ZERO, Reg::SatMode::UNKNOWN,
    Reg::MaskMergeMode::ZEROING, RoundMode::UNKNOWN};
constexpr Reg::CastTrait TAN_F322F16 = {
    Reg::RegLayout::ZERO, Reg::SatMode::SAT,
    Reg::MaskMergeMode::ZEROING, RoundMode::CAST_RINT};

// 归一化输入到 (-pi/2, pi/2)
__simd_callee__ inline void TanRound(
    Reg::RegTensor<float>& srcReg, Reg::RegTensor<float>& tmpReg,
    Reg::RegTensor<float>& roundReg, Reg::RegTensor<float>& resReg,
    Reg::RegTensor<float>& downReg1, Reg::RegTensor<float>& downReg2,
    Reg::MaskReg mask)
{
    // k = round(x * invpi)
    Reg::Muls(roundReg, srcReg, PI_INV, mask);
    Reg::Truncate<float, RoundMode::CAST_RINT, Reg::MaskMergeMode::ZEROING>(
        roundReg, roundReg, mask);

    // x -= k * pi_0
    Reg::Muls(tmpReg, roundReg, PI_V2, mask);
    Reg::Sub(resReg, srcReg, tmpReg, mask);
    // ... 后续 pi_1 ~ pi_4 的逐步精度补偿 ...
}

// 多项式逼近
__simd_callee__ inline void TanPolyApprox(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& tmpReg,
    Reg::RegTensor<float>& roundReg, Reg::RegTensor<float>& resReg,
    Reg::RegTensor<float>& downReg1, Reg::RegTensor<float>& downReg2,
    Reg::MaskReg mask)
{
    Reg::Mul(roundReg, resReg, resReg, mask);       // x²
    Reg::Muls(tmpReg, roundReg, R0, mask);           // R0 * x²
    Reg::Adds(tmpReg, tmpReg, R1, mask);             // + R1
    Reg::Mul(tmpReg, tmpReg, roundReg, mask);        // * x²
    Reg::Adds(tmpReg, tmpReg, R2, mask);             // + R2
    Reg::Mul(tmpReg, tmpReg, resReg, mask);          // * x → P(x)
    // ... Q(x) 多项式 ...
    Reg::Div(dstReg, tmpReg, roundReg, mask);        // dst = P/Q
}

template <typename T>
__simd_vf__ inline void TanCompute(
    __ubuf__ T* dstUb, __ubuf__ T* srcUb,
    uint32_t calCount, uint16_t repeatTimes)
{
    constexpr uint32_t stride = GetVecLen() / sizeof(float);
    Reg::MaskReg mask;
    Reg::RegTensor<T> srcReg;
    Reg::RegTensor<float> castReg, roundReg, tmpReg;
    Reg::RegTensor<float> downReg1, downReg2, resReg, dstReg;

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = Reg::UpdateMask<float>(calCount);
        if constexpr (sizeof(T) == sizeof(half)) {
            Reg::LoadAlign<T, Reg::LoadDist::DIST_UNPACK_B16>(
                srcReg, srcUb + i * stride);
            Reg::Cast<float, T, TAN_F162F32>(castReg, srcReg, mask);
        } else {
            Reg::LoadAlign(castReg, srcUb + i * stride);
        }
        TanRound(castReg, tmpReg, roundReg, resReg,
                 downReg1, downReg2, mask);
        TanPolyApprox(dstReg, tmpReg, roundReg, resReg,
                      downReg1, downReg2, mask);
        if constexpr (sizeof(T) == sizeof(half)) {
            Reg::Cast<T, float, TAN_F322F16>(srcReg, dstReg, mask);
            Reg::StoreAlign<T, Reg::StoreDist::DIST_PACK_B32>(
                dstUb + i * stride, srcReg, mask);
        } else {
            Reg::StoreAlign(dstUb + i * stride, dstReg, mask);
        }
    }
}

} // namespace TanInternal

template <typename T, bool isReuseSource = false>
__aicore__ inline void TanImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const uint32_t calCount)
{
    if ASCEND_IS_AIC { return; }

    CheckTensorPosition(dstTensor, "dstTensor", "VECIN, VECOUT, VECCALC");
    CheckTensorPosition(srcTensor, "srcTensor", "VECIN, VECOUT, VECCALC");
    CheckCalCount(calCount, "calCount", srcTensor, "srcTensor", "Tan");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Tan");
    static_assert(SupportType<T, half, float>(),
        "current data type is not supported on current device!");

    __ubuf__ T* dstUb = (__ubuf__ T*)dstTensor.GetPhyAddr();
    __ubuf__ T* srcUb = (__ubuf__ T*)srcTensor.GetPhyAddr();
    constexpr uint32_t stride = GetVecLen() / sizeof(float);
    uint16_t repeatTimes = CeilDivision(calCount, stride);

    TanInternal::TanCompute<T>(dstUb, srcUb, calCount, repeatTimes);
}

} // namespace AscendC
```

本示例展示了：
- 多寄存器协作完成复杂算法
- `__simd_callee__` 属性用于被 `__simd_vf__` 调用的辅助函数
- `Reg::Cast` + `LoadDist::DIST_UNPACK_B16` / `StoreDist::DIST_PACK_B32` 处理 half 类型
- 多项式逼近使用 `Reg::Mul` + `Reg::Adds` + `Reg::Muls` 组合
- `Reg::Truncate<..., CAST_RINT>` 实现向偶数舍入

---

## 示例 6：带双发射流水优化的二元运算模板

### 3510 RegBase 实现（通过模板实现 BitwiseAnd/Or/Xor 通用基础）

```cpp
template <auto func, typename T, typename RegT,
          const Reg::RegTrait& Trait = Reg::RegTraitNumOne>
__simd_vf__ inline void BitwiseTemplateImplVF(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1,
    uint16_t repeatTime, uint32_t count,
    uint32_t oneRepElm, uint32_t offset)
{
    RegT dstVreg, src0Vreg, src1Vreg;
    Reg::MaskReg mask;
    for (uint16_t i = 0; i < repeatTime; ++i) {
        // 第一半
        mask = Reg::UpdateMask<T, Trait>(count);
        Reg::LoadAlign(src0Vreg, src0 + i * oneRepElm);
        Reg::LoadAlign(src1Vreg, src1 + i * oneRepElm);
        func(dstVreg, src0Vreg, src1Vreg, mask);
        Reg::StoreAlign(dst + i * oneRepElm, dstVreg, mask);

        // 第二半（双发射）
        mask = Reg::UpdateMask<T, Trait>(count);
        Reg::LoadAlign(src0Vreg, src0 + i * oneRepElm + offset);
        Reg::LoadAlign(src1Vreg, src1 + i * oneRepElm + offset);
        func(dstVreg, src0Vreg, src1Vreg, mask);
        Reg::StoreAlign(dst + i * oneRepElm + offset, dstVreg, mask);
    }
    // 尾块
    mask = Reg::UpdateMask<T, Trait>(count);
    Reg::LoadAlign(src0Vreg, src0 + repeatTime * 2 * oneRepElm);
    Reg::LoadAlign(src1Vreg, src1 + repeatTime * 2 * oneRepElm);
    func(dstVreg, src0Vreg, src1Vreg, mask);
    Reg::StoreAlign(dst + repeatTime * 2 * oneRepElm, dstVreg, mask);
}

// 调用方通过传函数指针实现不同运算
// BitwiseAnd 调用:
//   BitwiseTemplateImplVF<Reg::And, T, RegT>(...);
// BitwiseOr 调用:
//   BitwiseTemplateImplVF<Reg::Or, T, RegT>(...);
// BitwiseXor 调用:
//   BitwiseTemplateImplVF<Reg::Xor, T, RegT>(...);
```

本示例展示了：
- `template<auto func>` 将 Reg API 作为模板参数传递
- 双发射（unroll by 2）流水优化：每次循环处理两组数据
- 尾块单独处理，不使用双发射

---

## 完整模板总结

| 模式 | 核心运算 | 示例 API |
|------|---------|---------|
| 一轮 Load-Compute-Store | `Truncate` | Ceil, Floor, Round |
| 向量-标量运算 | `Mins / Maxs` | ClampMax, ClampMin |
| 二元运算 | `Xor / And / Or` | BitwiseXor, BitwiseAnd, BitwiseOr |
| Compare + Select 组合 | `CompareScalar` + `Select` | Sign |
| 多寄存器多项式 | Mul/Adds/Muls/Sub/Div 组合 | Tan, Sin, Cos, Exp, Log |
| Cast + 计算 + Uncast | `LoadAlign(DIST_UNPACK)` + `Cast` + 计算 + `Cast` + `StoreAlign(DIST_PACK)` | 所有需要 half→float→half 的 API |
| 双发射模板 | 将 func 作为模板参数 + 2x unroll | Bitwise 系列 |
