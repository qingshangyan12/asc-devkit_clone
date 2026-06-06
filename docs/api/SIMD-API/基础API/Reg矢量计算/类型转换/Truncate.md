# Truncate

## 产品支持情况

| 产品 | 是否支持  |
| :----------------------- | :------: |
| <cann-filter npu-type="950"> Ascend 950PR/Ascend 950DT | √ </cann-filter> |
| <cann-filter npu-type="A3"> Atlas A3 训练系列产品/Atlas A3 推理系列产品 | x </cann-filter> |
| <cann-filter npu-type="910b"> Atlas A2 训练系列产品/Atlas A2 推理系列产品 | x </cann-filter> |
| <cann-filter npu-type="310b"> Atlas 200I/500 A2 推理产品 | x </cann-filter> |
| <cann-filter npu-type="310p"> Atlas 推理系列产品AI Core | x </cann-filter> |
| <cann-filter npu-type="310p"> Atlas 推理系列产品Vector Core | x </cann-filter> |
| <cann-filter npu-type="910"> Atlas 训练系列产品 | x </cann-filter> |

## 功能说明

头文件路径：`"basic_api/reg\_compute/kernel\_reg\_compute\_vec\_vconv\_intf\_impl.h"`。

将源操作数的浮点数元素截断到整数位，同时源操作数的数据类型保持不变。

## 函数原型

```cpp
template <typename T = DefaultType, RoundMode roundMode = RoundMode::CAST_NONE, MaskMergeMode mode = MaskMergeMode::ZEROING, typename S>
__simd_callee__ inline void Truncate(S& dstReg, S& srcReg, MaskReg& mask)
```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。 dstReg/srcReg支持的数据类型为：half、float、bfloat16_t。 |
| roundMode | 舍入模式，取值如下：<br>&bull; RoundMode::CAST_RINT<br>&bull; RoundMode::CAST_ROUND<br>&bull; RoundMode::CAST_FLOOR<br>&bull; RoundMode::CAST_CEIL<br>&bull; RoundMode::CAST_TRUNC |
| mode | [MaskMergeMode](../数据类型/MaskMergeMode.md)枚举类型，选择MERGING模式或ZEROING模式。<br>&bull; ZEROING模式下，mask未筛选的元素在dstReg中置零。<br>&bull; MERGING模式当前不支持。 |
| S | srcReg/dstReg 类型，例如RegTensor&lt;float&gt;，由编译器自动推导，用户不需要填写。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dstReg | 输出 | 目的操作数。 <br>类型为[RegTensor](../寄存器数据类型/RegTensor.md)。 |
| srcReg | 输入 | 源操作数。 <br>类型为[RegTensor](../寄存器数据类型/RegTensor.md) |
| mask | 输入 | 源操作数元素操作的有效指示，详细说明请参考[MaskReg](../寄存器数据类型/MaskReg.md)。 |

## 返回值说明

无

## 数据类型

**表 3**  数据类型支持情况

| dstReg | srcReg |
| --- | --- |
| half | half |
| bfloat16_t | bfloat16_t |
| float | float |

## 约束说明

-   非饱和模式下，输入nan，输出nan; 输入值超出输入类型的最值范围，则输出+/-inf。
-   饱和模式下，输入nan, 输出为0，输入值超出输入类型的最值范围时，则其被饱和为最值。
-   float类型只支持不饱和模式。

## 理论性能说明

**表 4**  理论并行度

| srcReg数据类型 | dstReg数据类型 | 理论并行度（element / cycle） |
| --- | --- | --- |
| half | half | 64 |
| bfloat16_t | bfloat16_t | 64 |
| float | float | 32 |

## 调用示例

```cpp
template<typename T>
__simd_vf__ inline void TruncVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr, uint32_t count, uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask;
    for (uint16_t i = 0; i < repeatTimes; i++) {
        AscendC::Reg::LoadAlign(srcReg, srcAddr + i * oneRepeatSize);
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::Truncate<T, AscendC::RoundMode::CAST_FLOOR, AscendC::Reg::MaskMergeMode::ZEROING>(dstReg, srcReg, mask);
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}
```

