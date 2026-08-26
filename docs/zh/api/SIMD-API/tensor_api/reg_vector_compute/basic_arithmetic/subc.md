# subc

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3训练系列产品/Atlas A3推理系列产品：不支持
- Atlas A2训练系列产品/Atlas A2推理系列产品：不支持
- Atlas 200I/500 A2推理产品：不支持
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
- Atlas训练系列产品：不支持

## 功能说明

头文件路径：`"tensor_api/tensor.h"`。

命名空间：`asc::te::experimental`。

`subc`提供带借位输出减法，以及带借位输入和输出减法。减法在硬件中通过补码加法实现，输出的进位标志表示减法是否借位，`1`表示未借位，`0`表示发生借位。

### 带借位输出减法

对`src0`和`src1`按元素求差，返回计算结果的低32位，并通过`carry`输出每个元素的进位标志。

$$
\{carry_i, dst_i\} = \{1'b0, src0_i\} + \{1'b0, \mathord{\sim}src1_i\} + 33'b1
$$

### 带借位输入和输出减法

使用`carry_src`中的输入进位继续进行按元素减法，返回计算结果的低32位，并通过`carry`输出每个元素的新进位标志。

$$
\{carry_i, dst_i\} = \{1'b0, src0_i\} + \{1'b0, \mathord{\sim}src1_i\} + \{32'b0, carry\_src_i\}
$$

## 函数原型

### 带借位输出减法

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> subc(
    reg_tensor<bool>& carry,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1)
```

### 带借位输入和输出减法

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> subc(
    reg_tensor<bool>& carry,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1,
    const reg_tensor<bool>& carry_src)
```

## 参数说明

### 带借位输出减法

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，仅支持`uint32_t`。 |
| `carry` | 输出 | 借位输出，类型为`reg_tensor<bool>`。`carry.reg`保存每个元素的进位标志，`1`表示未借位，`0`表示发生借位。返回时`carry.mask`与`src0.mask`相同，用于标记哪些进位标志有效。 |
| `src0` | 输入 | 被减数，类型为`reg_tensor<T>`。`src0.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `src1` | 输入 | 减数，类型为`reg_tensor<T>`。`src1.mask`不用于本次计算。 |

### 带借位输入和输出减法

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，仅支持`uint32_t`。 |
| `carry` | 输出 | 借位输出，类型为`reg_tensor<bool>`。`carry.reg`保存每个元素的新进位标志，`1`表示未借位，`0`表示发生借位。返回时`carry.mask`与`src0.mask`相同，用于标记哪些进位标志有效。 |
| `src0` | 输入 | 被减数，类型为`reg_tensor<T>`。`src0.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `src1` | 输入 | 减数，类型为`reg_tensor<T>`。`src1.mask`不用于本次计算。 |
| `carry_src` | 输入 | 输入进位标志，类型为`reg_tensor<bool>`。`carry_src.reg`保存上一部分减法产生的进位标志，`carry_src.mask`不用于本次计算。 |

## 返回值说明

### 带借位输出减法

返回`reg_tensor<T>`。`reg`保存`src0`与`src1`逐元素相减后的低32位，`src0.mask`对应位置为`0`时，`reg`的对应位置置零。返回值的`mask`与`src0.mask`相同。

### 带借位输入和输出减法

返回`reg_tensor<T>`。`reg`保存加入输入进位后逐元素计算得到的低32位，`src0.mask`对应位置为`0`时，`reg`的对应位置置零。返回值的`mask`与`src0.mask`相同。

## 约束说明

用于多字整数分段减法时，最低32位使用带借位输出减法，其余部分按照从低位到高位的顺序使用带借位输入和输出减法传递进位标志。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。第0行保存低32位，第1行保存高32位。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void subc_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    auto mask = asc::te::all_mask<uint32_t>();
    auto src0_low = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_low = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src0_high = input0.load(asc::te::make_coord(1, 0)).with_mask(mask);
    auto src1_high = input1.load(asc::te::make_coord(1, 0)).with_mask(mask);

    asc::te::reg_tensor<bool> carry_low;
    auto low = asc::te::experimental::subc(carry_low, src0_low, src1_low);
    asc::te::reg_tensor<bool> carry_high;
    auto high = asc::te::experimental::subc(carry_high, src0_high, src1_high, carry_low);

    output.store(asc::te::make_coord(0, 0), low);
    output.store(asc::te::make_coord(1, 0), high);
}
```
