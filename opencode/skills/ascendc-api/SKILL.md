# AscendC 3510 RegBase 计算类 API 开发技能

## 概述

本技能用于 AscendC 3510 架构下基于寄存器（Reg）操作的计算类 API 开发。核心模式为：输入 `LocalTensor`（UB 上的数据），通过 `GetPhyAddr()` 获取 UB 物理地址，使用 `Reg::LoadAlign` 将数据从 UB 加载到 `RegTensor` 寄存器，在寄存器上完成向量计算，再使用 `Reg::StoreAlign` 将结果从寄存器搬回 UB。

## 适用场景

当用户需要在 3510 架构下开发以下类型的内部 API 实现时应调用本技能：

1. 逐元素数学运算（如 Round、Ceil、Floor、Sign、Clamp、Tan、Log、Exp、Sinh、Cosh 等）
2. 逐元素位运算（如 BitwiseAnd、BitwiseOr、BitwiseXor、BitwiseNot）
3. 逐元素逻辑运算（如 LogicalAnd、LogicalOr、LogicalXor）
4. 寄存器级别的数值处理与类型转换

## 核心编程模式

### 整体数据流

```
LocalTensor<T> (UB数据) → GetPhyAddr() → __ubuf__ T* → Reg::LoadAlign → RegTensor (寄存器)
                                                                               ↓
                                                                        Reg::* 向量计算
                                                                               ↓
LocalTensor<T> (结果回UB) ← __ubuf__ T* ← Reg::StoreAlign ← RegTensor (结果)
```

### 标准实现框架

```cpp
// ==================== 第 1 层：__aicore__ 入口函数 ====================
// 职责：参数校验、UB 地址获取、循环参数计算、调用 __simd_vf__ 计算函数
template <typename T, bool isReuseSource = false>
__aicore__ inline void XxxImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const uint32_t calCount)
{
    // 架构门控：仅 3510 Vector Core 执行
    if ASCEND_IS_AIC { return; }

    // 类型校验
    static_assert(SupportType<T, half, float>(), "...");
    // 参数校验
    CheckTensorPosition(dstTensor, "dstTensor", "VECIN, VECOUT, VECCALC");
    CheckTensorPosition(srcTensor, "srcTensor", "VECIN, VECOUT, VECCALC");
    CheckCalCount(calCount, "calCount", srcTensor, "srcTensor", "Xxx");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Xxx");

    // 获取 UB 物理地址
    __ubuf__ T* srcUb = (__ubuf__ T*)srcTensor.GetPhyAddr();
    __ubuf__ T* dstUb = (__ubuf__ T*)dstTensor.GetPhyAddr();

    // 计算寄存器迭代参数
    constexpr uint32_t repeatElm = GetVecLen() / sizeof(T);
    uint16_t repeatTimes = static_cast<uint16_t>(CeilDivision(calCount, repeatElm));

    // 调用寄存器级计算函数
    XxxCompute<T, isReuseSource>(dstUb, srcUb, calCount, repeatTimes);
}

// ==================== 第 2 层：__simd_vf__ 寄存器计算函数 ====================
// 职责：声明 RegTensor/掩码，循环 LoadAlign → 计算 → StoreAlign
template <typename T, bool isReuseSource = false>
__simd_vf__ inline void XxxCompute(
    __ubuf__ T* dstUb, __ubuf__ T* srcUb, uint32_t calCount,
    const uint16_t repeatTimes)
{
    constexpr uint32_t repeatElm = GetVecLen() / sizeof(T);
    Reg::RegTensor<T> srcReg;
    Reg::RegTensor<T> dstReg;
    Reg::MaskReg maskReg;
    for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTimes); ++i) {
        maskReg = Reg::UpdateMask<T>(calCount);           // 更新尾块掩码
        Reg::LoadAlign<T>(srcReg, srcUb + i * repeatElm); // UB → Reg
        Reg::XXX(dstReg, srcReg, maskReg);                // 寄存器计算
        Reg::StoreAlign<T>(dstUb + i * repeatElm, dstReg, maskReg); // Reg → UB
    }
}
```

### 二元运算模板模式

对于二元位运算/逻辑运算，通用模板封装了双发射流水：

```cpp
template <auto func, typename T, typename RegT, const Reg::RegTrait& Trait>
__simd_vf__ inline void BitwiseTemplateImplVF(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1,
    uint16_t repeatTime, uint32_t count, uint32_t oneRepElm, uint32_t offset)
{
    RegT dstVreg, src0Vreg, src1Vreg;
    Reg::MaskReg mask;
    for (uint16_t i = 0; i < repeatTime; ++i) {
        mask = Reg::UpdateMask<T, Trait>(count);
        Reg::LoadAlign(src0Vreg, src0 + i * oneRepElm);
        Reg::LoadAlign(src1Vreg, src1 + i * oneRepElm);
        func(dstVreg, src0Vreg, src1Vreg, mask);
        Reg::StoreAlign(dst + i * oneRepElm, dstVreg, mask);
        // 双发射 (unroll by 2)
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
```

## 关键概念

### 头文件依赖

```cpp
#include "kernel_basic_intf.h"   // Reg::* API, Mask 定义
#include "kernel_tensor.h"       // LocalTensor<T>, GetPhyAddr()
#include "../../common/check.h"  // CheckTensorPosition(), CheckCalCount(), CeilDivision()
```

### 函数属性

| 属性 | 含义 | 使用位置 |
|------|------|---------|
| `__aicore__` | 设备端入口函数 | 对外 `XxxImpl()` 接口 |
| `__simd_vf__` | Vector 单元 SIMD 函数 | 内部 `XxxCompute()` 寄存器操作 |

### 核心类型

| 类型 | 说明 |
|------|------|
| `Reg::RegTensor<T>` | 寄存器向量，单次操作最多 `GetVecLen()/sizeof(T)` 个元素 |
| `Reg::RegTensor<T, Reg::RegTraitNumTwo>` | 64 位数据的寄存器向量（占两个向量寄存器槽位） |
| `Reg::MaskReg` | 掩码寄存器，位宽等于向量 lane 数，bit=1 表示该 lane 参与计算 |
| `LocalTensor<T>` | UB 上的张量抽象，通过 `GetPhyAddr()` 获取 `__ubuf__ T*` 指针 |

### 核心操作

| 操作 | 方向 | 说明 |
|------|------|------|
| `Reg::LoadAlign<T>(reg, ubuf_ptr)` | UB → Reg | 从 UB 对齐加载到寄存器 |
| `Reg::StoreAlign<T>(ubuf_ptr, reg, mask)` | Reg → UB | 从寄存器对齐存回 UB |
| `Reg::UpdateMask<T>(count)` | — | 创建前 count 个元素有效的掩码 |
| `Reg::Duplicate(reg, scalar)` | — | 将标量广播到寄存器所有 lane |

### 向量长度与迭代

```cpp
constexpr uint32_t repeatElm = GetVecLen() / sizeof(T);  // 每次迭代处理的元素数
uint16_t repeatTimes = CeilDivision(calCount, repeatElm); // 迭代次数（向上取整）
```

- `GetVecLen()`: 返回 Vector 单元一次指令处理的最大字节数（典型值 64 或 128）
- `repeatTimes` 包含尾块：最后一次迭代由 `UpdateMask(calCount)` 仅处理剩余元素

### 64 位类型处理

当 `sizeof(T) == 8` 时，每个元素占 8 字节，需要使用双倍向量寄存器空间：

```cpp
using RegT = Reg::RegTensor<T, Reg::RegTraitNumTwo>;           // 寄存器类型
constexpr uint32_t vlSize = GetVecLen() / sizeof(T) * 2;       // 双倍元素数
uint16_t repeatTime = CeilDivision(calCount, vlSize);
```

### 半精度浮点处理

half（float16）加载/存储需要特殊的分布模式实现解包/打包：

```cpp
// 加载 half → 解包为 float 寄存器计算
Reg::LoadAlign<half, Reg::LoadDist::DIST_UNPACK_B16>(srcReg, srcUb + offset);
Reg::Cast<float, half, castTrait>(floatReg, srcReg, mask);
// ... 使用 float 寄存器计算 ...
Reg::Cast<half, float, castTrait>(srcReg, floatReg, mask);
// 存回 half，打包
Reg::StoreAlign<half, Reg::StoreDist::DIST_PACK_B32>(dstUb + offset, srcReg, mask);
```

### 类型转换 (CastTrait)

```cpp
constexpr Reg::CastTrait myCastTrait = {
    Reg::RegLayout::ZERO,          // 寄存器布局
    Reg::SatMode::UNKNOWN,         // 饱和模式
    Reg::MaskMergeMode::ZEROING,   // 掩码合并模式
    RoundMode::UNKNOWN             // 舍入模式
};
```

### 架构门控

```cpp
if ASCEND_IS_AIC {
    return;  // 仅在 AI Core 上执行，非 AIC 架构直接返回
}
```

## 知识库文件

| 文件 | 内容 |
|------|------|
| [reference/api-guide.md](reference/api-guide.md) | Reg API 完整操作参考 |
| [examples/tensor-ops.md](examples/tensor-ops.md) | 3510 RegBase API 开发示例 |
