# Neg

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

头文件路径：`"basic_api/reg_compute/kernel_reg_compute_vec_unary_intf.h"`。

该接口用于根据mask对输入数据srcReg进行取相反数操作，将结果写入dstReg。计算公式如下：

$$dstReg_i = -srcReg_i$$

## 函数原型

```cpp
template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void Neg(U& dstReg, U& srcReg, MaskReg& mask);
```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。 |
| mode | [MaskMergeMode](../数据类型/MaskMergeMode.md)枚举类型。选择MERGING模式或ZEROING模式。<br>&bull; ZEROING，mask未筛选的元素在dstReg中置零。<br>&bull; MERGING，暂不支持。 |
| U | 源操作数和目的操作数的RegTensor类型，例如RegTensor&lt;half&gt;，由编译器自动推导，用户不需要填写。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dstReg | 输出 | 目的操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md)。 |
| srcReg | 输入 | 源操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md)。 |
| mask | 输入 | 源操作数中元素有效性的指示，详细说明请参考[MaskReg](../寄存器数据类型/MaskReg.md)。 |

## 返回值说明

无

## 数据类型

**表 3**  数据类型组合

| srcReg | dstReg |
| --- | --- |
| int8_t | int8_t |
| int16_t | int16_t |
| half | half |
| int32_t | int32_t |
| float | float |
| int64_t | int64_t |

**int64_t为软仿数据类型。**

## 约束说明

- 支持操作数重叠。
- int64_t为软仿数据类型。

## 理论性能说明

**表 4**  理论性能

| srcReg | dstReg | 理论并行度(element/cycle) |
| --- | --- | --- |
| int8_t | int8_t | 128 |
| int16_t | int16_t | 64 |
| half | half | 128 |
| int32_t | int32_t | 32 |
| float | float | 64 |

**int64_t为软仿数据类型。**

## 调用示例


```cpp
template<typename T>
__simd_vf__ inline void NegVF(__ubuf__ T* dstAddr, __ubuf__ T* srcAddr, uint32_t count,
 uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask;
    for (uint16_t i = 0; i < repeatTimes; i++) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg, srcAddr + i * oneRepeatSize);
        AscendC::Reg::Neg(dstReg, srcReg, mask);
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}
```

