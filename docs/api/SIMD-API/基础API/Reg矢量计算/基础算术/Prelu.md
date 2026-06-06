# Prelu

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

头文件路径：`"basic_api/reg_compute/kernel_reg_compute_vec_binary_intf.h"`。

该接口用于实现PReLU（Parametric ReLU）激活函数：当源操作数srcReg0中对应元素大于0时，直接将该元素写入目的操作数dstReg；否则将srcReg0与srcReg1对应元素相乘的结果写入dstReg，计算公式如下：

$$dstReg_i = \begin{cases} srcReg0_i & srcReg0_i > 0 \\ srcReg0_i \cdot srcReg1_i & srcReg0_i \leq 0 \end{cases}$$

## 函数原型

```cpp
template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void Prelu(U &dstReg, U &srcReg0, U &srcReg1, MaskReg &mask);
```

## 参数说明

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。 |
| mode | [MaskMergeMode](../数据类型/MaskMergeMode.md)枚举类型，选择MERGING模式或ZEROING模式。<br>&bull; ZEROING模式下，mask未筛选的元素在dstReg中置零。<br>&bull; MERGING模式当前不支持。 |
| U | 源操作数和目的操作数的RegTensor类型，例如RegTensor&lt;half&gt;，由编译器自动推导，用户不需要填写。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dstReg | 输出 | 目的操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md)。 |
| srcReg0 | 输入 | 源操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md)。<br>源操作数的数据类型需要与目的操作数保持一致。 |
| srcReg1 | 输入 | 源操作数。<br>类型为[RegTensor](../寄存器数据类型/RegTensor.md)。<br>源操作数的数据类型需要与目的操作数保持一致。 |
| mask | 输入 | 源操作数中元素有效性的指示，详细说明请参考[MaskReg](../寄存器数据类型/MaskReg.md)。 |

## 返回值说明

无

## 数据类型

**表 3**  数据类型组合

| dstReg | srcReg0 | srcReg1 |
| --- | --- | --- |
| half | half | half |
| float | float | float |

## 约束说明

寄存器的非饱和/饱和模式具体配置方式参考待补充。

## 理论性能说明

**表 4**  指令理论并行度

| dstReg | srcReg0 | srcReg1 | 理论并行度（element / cycle） |
| --- | --- | --- | --- |
| half | half | half | 128 |
| float | float | float | 64 |

## 调用示例


```cpp
template<typename T>
__simd_vf__ inline void PreluVF(__ubuf__ T* dstAddr, __ubuf__ T* src0Addr, __ubuf__ T* src1Addr, uint32_t count, uint32_t oneRepeatSize, uint16_t repeatTimes)
{
    AscendC::Reg::RegTensor<T> srcReg0;
    AscendC::Reg::RegTensor<T> srcReg1;
    AscendC::Reg::RegTensor<T> dstReg;
    AscendC::Reg::MaskReg mask;
    for (uint16_t i = 0; i < repeatTimes; i++) {
        mask = AscendC::Reg::UpdateMask<T>(count);
        AscendC::Reg::LoadAlign(srcReg0, src0Addr + i * oneRepeatSize);
        AscendC::Reg::LoadAlign(srcReg1, src1Addr + i * oneRepeatSize);
        AscendC::Reg::Prelu(dstReg, srcReg0, srcReg1, mask);
        AscendC::Reg::StoreAlign(dstAddr + i * oneRepeatSize, dstReg, mask);
    }
}
```

