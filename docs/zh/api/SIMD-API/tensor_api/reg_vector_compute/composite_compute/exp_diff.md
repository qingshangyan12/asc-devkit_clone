# exp_diff

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径：`"tensor_api/experimental/vector_compute.h"`。

逐元素计算两个输入之差的自然指数：

$$
dst_i = e^{(src0_i - src1_i)}
$$

float输入按连续元素计算。half输入可通过`src_pos`选择偶数位置或奇数位置；未指定`src_pos`时默认选择偶数位置。

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<float> exp_diff(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)

template <typename position_type>
__simd_callee__ inline reg_tensor<float> exp_diff(
    const reg_tensor<half>& src0,
    const reg_tensor<half>& src1,
    position_type src_pos)
```

## 参数说明

**表1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 源操作数的元素类型。支持的数据类型请参考[数据类型](#数据类型)。 |

**表2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src0` | 输入 | 被减数，类型为`reg_tensor<float>`或`reg_tensor<half>`。`src0.mask`按输出元素编号，对应位置为`1`时该输出元素参与计算，为`0`时不参与计算。 |
| `src1` | 输入 | 减数，类型与`src0`相同。`src1.mask`不用于本次计算。 |
| `src_pos` | 输入 | 仅用于half输入，支持`ASC_POSITION_EVEN`和`ASC_POSITION_ODD`，分别表示选择偶数位置和奇数位置。 |

## 数据类型

| 输入类型 | 返回类型 |
| --- | --- |
| `reg_tensor<float>` | `reg_tensor<float>` |
| `reg_tensor<half>` | `reg_tensor<float>` |

## 返回值说明

返回`reg_tensor<float>`。`reg`保存逐元素计算结果，`src0.mask`对应位置为`0`时，`reg`的对应位置置零。返回值的`mask`与`src0.mask`相同。输入为`half`时，mask第`i`位控制输出的第`i`个`float`元素；`ASC_POSITION_EVEN`和`ASC_POSITION_ODD`分别使该输出元素读取源操作数的第`2 * i`和第`2 * i + 1`个元素。

## 约束说明

- `src0.mask`需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。
- 输入为`half`时，mask按照输出`float`元素解释，应使用`all_mask<float>`、`make_mask<..., float>`或`update_mask<float>`生成。不要将mask设置成交替的奇偶位；源元素位置由`src_pos`选择。
- 输入为`half`且未指定`src_pos`时，默认选择偶数位置参与计算。

## 调用示例

以下示例展示half输入的调用方式。

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void exp_diff_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    auto mask = asc::te::experimental::all_mask<float>();
    auto src0 = asc::te::experimental::load(input0, asc::te::make_coord(0)).with_mask(mask);
    auto src1 = asc::te::experimental::load(input1, asc::te::make_coord(0)).with_mask(mask);
    auto dst = asc::te::experimental::exp_diff(src0, src1, ASC_POSITION_ODD);
    asc::te::experimental::store(output, asc::te::make_coord(0), dst);
}
```
