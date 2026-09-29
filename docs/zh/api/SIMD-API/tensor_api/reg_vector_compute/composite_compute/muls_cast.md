# muls_cast

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

头文件路径：`"tensor_api/experimental/arch/vector/composite_compute.h"`。

该接口根据`src.mask`将`src`与标量`scalar`按元素相乘，再按照`CAST_ROUND`模式将结果转换为`half`类型并返回。计算公式如下：

$$
dst_i = cast\_round\_to\_f16(src_i \times scalar)
$$

默认将结果写入返回寄存器的偶数`half`位置。

## 函数原型

```cpp
template <cast_layout Layout = cast_layout::zero, typename T, typename U>
__simd_callee__ inline reg_tensor<T> muls_cast(
    const reg_tensor<U>& src, const U& scalar)
```

## 参数说明

**表1**模板参数说明

| 参数名 | 描述 |
| --- | --- |
| Layout | 结果布局。支持`cast_layout::zero`和`cast_layout::one`，默认值为`cast_layout::zero`。 |
| T | 返回值元素类型，当前仅支持`half`。 |
| U | 源操作数和标量类型，从输入自动推导，当前仅支持`float`。 |

**表2**参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| src | 输入 | 源操作数，类型为`reg_tensor<float>`。`src.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| scalar | 输入 | `float`标量乘数。 |

## 返回值说明

返回`reg_tensor<half>`。`reg`保存乘法及类型转换结果。`Layout`为`cast_layout::zero`时结果写入偶数位置，为`cast_layout::one`时结果写入奇数位置，另一组位置置零。返回值的`mask`与`src.mask`相同。

## 数据类型

**表3**数据类型组合

| src | scalar | dst |
| --- | --- | --- |
| float | float | half |

## 约束说明

- `src.mask`需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。
- `src.mask`对应位置为`0`时，返回值`reg`的对应位置置零。
- 类型转换按照`CAST_ROUND`模式舍入。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式，`count`表示待处理的float输入元素个数。

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void muls_cast_example(InputTensor input, OutputTensor output, uint32_t count)
{
    constexpr uint32_t input_vector_length = asc_get_vf_len() / sizeof(float);
    constexpr uint32_t output_vector_length = asc_get_vf_len() / sizeof(half);
    for (uint32_t input_offset = 0, output_offset = 0; count > 0;
         input_offset += input_vector_length, output_offset += output_vector_length) {
        auto input_coord = asc::te::make_coord(input_offset);
        auto output_coord = asc::te::make_coord(output_offset);
        auto mask = asc::te::experimental::update_mask<float>(count);
        auto src = asc::te::experimental::load(input, input_coord).with_mask(mask);
        auto dst = asc::te::experimental::muls_cast<asc::te::experimental::cast_layout::one, half>(src, 1.5f);
        asc::te::experimental::store(output, output_coord, dst);
    }
}
```
