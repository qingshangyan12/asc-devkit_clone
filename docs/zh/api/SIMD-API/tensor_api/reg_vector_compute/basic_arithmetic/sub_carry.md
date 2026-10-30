# sub_carry

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/basic_arithmetic.h"`。

该接口根据`src0.mask`，对输入元素执行逐元素减法，并返回减法结果和无借位标志。返回值`first`保存减法结果的低32位，`second`保存无借位标志：1表示本次减法未发生借位，0表示本次减法发生借位。

硬件将源操作数的位模式按32位无符号数处理，通过补码加法实现减法。输入、输出标志均按无符号借位规则解释，不表示有符号运算是否溢出。该接口提供以下两种形式，两者均输出无借位标志。

### [不带输入进位标志的减法](#sub-carry-without-carry-src)

`sub_carry(src0, src1)`直接计算`src0 - src1`，不考虑上一步计算是否发生借位，适用于独立减法或多字长减法的最低位部分。计算公式如下：

$$
\{second_i, first_i\} = \{1'b0, src0_i\} + \{1'b0, \sim src1_i\} + 33'b1
$$

其中，`~src1_i`表示对src1的32位位模式按位取反，`first`保存补码加法结果的低32位，`second`保存最高位的进位标志：1表示无借位，0表示有借位。

### [带输入进位标志的减法](#sub-carry-with-carry-src)

`sub_carry(src0, src1, carry_src)`通过`carry_src`接收上一步计算的借位状态：1表示上一步无借位，计算`src0 - src1`；0表示上一步有借位，计算`src0 - src1 - 1`。该形式适用于多字长减法的高位部分，用于扣除低位计算向高位借走的1。计算公式如下：

$$
\{second_i, first_i\} = \{1'b0, src0_i\} + \{1'b0, \sim src1_i\} + \{32'b0, carry\_src_i\}
$$

其中，`first`保存补码加法结果的低32位，`second`保存本次计算产生的进位标志：1表示无借位，0表示有借位。多字长减法中，可将低位计算返回的`second`直接作为高位计算的`carry_src`传入。

## 函数原型

<a id="sub-carry-without-carry-src"></a>

### 不带输入进位标志的减法

```cpp
template <typename T>
__simd_callee__ inline reg_pair<T, bool> sub_carry(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)
```

<a id="sub-carry-with-carry-src"></a>

### 带输入进位标志的减法

```cpp
template <typename T>
__simd_callee__ inline reg_pair<T, bool> sub_carry(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1, const reg_tensor<bool>& carry_src)
```

## 参数说明

**表1** 模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。支持的数据类型请参考[数据类型](#数据类型)。 |

### 不带输入进位标志的减法

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| src0 | 输入 | 被减数，类型为reg_tensor&lt;T&gt;。其中，src0.reg保存矢量数据，src0.mask用于控制各元素是否参与计算。<br>src0.mask与元素对应的比特位为1时，该元素参与计算。<br>src0.mask与元素对应的比特位为0时，该元素不参与计算。 |
| src1 | 输入 | 减数，类型为reg_tensor&lt;T&gt;。src1.mask不参与本次计算。 |

### 带输入进位标志的减法

**表3** 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| src0 | 输入 | 被减数，类型为reg_tensor&lt;T&gt;。其中，src0.reg保存矢量数据，src0.mask用于控制各元素是否参与计算。<br>src0.mask与元素对应的比特位为1时，该元素参与计算。<br>src0.mask与元素对应的比特位为0时，该元素不参与计算。 |
| src1 | 输入 | 减数，类型为reg_tensor&lt;T&gt;。src1.mask不参与本次计算。 |
| carry_src | 输入 | 输入进位标志（对于减法，表示无借位），类型为reg_tensor&lt;bool&gt;。对应元素为1时不额外减1，为0时额外减1。carry_src.mask不参与本次计算。 |

## 返回值说明

返回类型为`reg_pair<T, bool>`：`first`保存减法结果的低32位，`second`保存无借位标志，对应元素为1表示无借位，为0表示有借位。两个成员的`mask`均设置为`src0.mask`。

## 数据类型

### 不带输入进位标志的减法

**表4** 数据类型组合

| src0 | src1 | first | second |
| --- | --- | --- | --- |
| int32_t | int32_t | int32_t | bool |
| uint32_t | uint32_t | uint32_t | bool |

### 带输入进位标志的减法

**表5** 数据类型组合

| src0 | src1 | carry_src | first | second |
| --- | --- | --- | --- | --- |
| int32_t | int32_t | bool | int32_t | bool |
| uint32_t | uint32_t | bool | uint32_t | bool |

## 约束说明

- src0.mask需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。

## 调用示例

### 不带输入进位标志的减法

```cpp
#include "tensor_api/experimental/vector_ops.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void sub_carry_vf(
    const InputTensor input0, const InputTensor input1, OutputTensor output, uint16_t repeat_times,
    uint32_t total, uint32_t one_repeat_size)
{
    using data_type = typename InputTensor::data_type;
    uint32_t remain = total;
    for (uint16_t i = 0; i < repeat_times; ++i) {
        const uint32_t offset = i * one_repeat_size;
        const auto coord = asc::te::make_coord(offset);
        auto mask = asc::te::experimental::update_mask<data_type>(remain);
        auto src0_reg = asc::te::experimental::load(input0, coord).with_mask(mask);
        auto src1_reg = asc::te::experimental::load(input1, coord);
        auto result = asc::te::experimental::sub_carry(src0_reg, src1_reg);
        asc::te::experimental::store(output, coord, result.first);
    }
}
```

### 带输入进位标志的减法

本示例使用`none_mask<data_type>()`将输入标志置为0，因此参与计算的元素执行`src0 - src1 - 1`。若不需要额外减1，可使用`all_mask<data_type>()`将输入标志置为1。

```cpp
#include "tensor_api/experimental/vector_ops.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void sub_carry_with_carry_vf(
    const InputTensor input0, const InputTensor input1, OutputTensor output, uint16_t repeat_times,
    uint32_t total, uint32_t one_repeat_size)
{
    using data_type = typename InputTensor::data_type;
    uint32_t remain = total;
    for (uint16_t i = 0; i < repeat_times; ++i) {
        const uint32_t offset = i * one_repeat_size;
        const auto coord = asc::te::make_coord(offset);
        auto mask = asc::te::experimental::update_mask<data_type>(remain);
        auto src0_reg = asc::te::experimental::load(input0, coord).with_mask(mask);
        auto src1_reg = asc::te::experimental::load(input1, coord);
        auto carry_src = asc::te::experimental::none_mask<data_type>();
        auto result = asc::te::experimental::sub_carry(src0_reg, src1_reg, carry_src);
        asc::te::experimental::store(output, coord, result.first);
    }
}
```
