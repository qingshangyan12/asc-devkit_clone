# exp_diff

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

逐元素计算两个输入之差的自然指数：

$$
dst_i = e^{(src0_i - src1_i)}
$$

`float`输入按连续元素计算。`half`输入可以通过`ASC_POSITION_EVEN`或`ASC_POSITION_ODD`选择参与计算的偶数位置或奇数位置；未指定位置时默认选择偶数位置。

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<float> exp_diff(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)

template <typename position_type>
__simd_callee__ inline reg_tensor<float> exp_diff(
    const reg_tensor<half>& src0, const reg_tensor<half>& src1, position_type src_pos)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src0` | 输入 | 被减数，同时提供执行mask。 |
| `src1` | 输入 | 减数。 |
| `src_pos` | 输入 | `half`输入的位置选择，支持`ASC_POSITION_EVEN`和`ASC_POSITION_ODD`。 |

## 支持的数据类型

| 输入类型 | `src_pos` | 返回类型 |
| --- | --- | --- |
| `reg_tensor<float>` | 不适用 | `reg_tensor<float>` |
| `reg_tensor<half>` | 省略、`ASC_POSITION_EVEN`或`ASC_POSITION_ODD` | `reg_tensor<float>` |

## 返回值说明

返回`reg_tensor<float>`，结果mask等于`src0.mask`。输入为`half`时，返回值中的每个元素对应所选位置上的一个输入元素。

## 约束说明

- 两个输入寄存器的元素类型必须一致。
- 三参数形式仅支持`half`输入，`src_pos`仅支持`ASC_POSITION_EVEN`和`ASC_POSITION_ODD`。

## 调用示例

以下示例展示`half`输入分别选择偶数位置和奇数位置的调用方式。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void exp_diff_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    auto mask = asc::te::all_mask<half>();
    auto src0 = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1 = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto even = asc::te::experimental::exp_diff(src0, src1, ASC_POSITION_EVEN);
    auto odd = asc::te::experimental::exp_diff(src0, src1, ASC_POSITION_ODD);

    output.store(asc::te::make_coord(0, 0), even);
    output.store(asc::te::make_coord(1, 0), odd);
}
```
