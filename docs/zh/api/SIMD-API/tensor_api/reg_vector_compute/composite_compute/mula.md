# mula

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

该接口根据`dst.mask`将`src0`与`src1`按元素相乘，再与`dst`原值相加，返回计算结果。计算公式如下：

$$
dst_i = src0_i \times src1_i + dst_i
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> mula(
    reg_tensor<T>& dst,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1)
```

## 参数说明

**表1**模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。支持的数据类型请参考[数据类型](#数据类型)。 |

**表2**参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输入/输出 | 累加初值和结果。接口直接更新`dst.reg`，`dst.mask`控制参与计算的元素。 |
| src0 | 输入 | 第一个乘数。`src0.mask`不参与本次计算。 |
| src1 | 输入 | 第二个乘数。`src1.mask`不参与本次计算。 |

## 返回值说明

返回`reg_tensor<T>`，保存`src0 * src1 + dst`的结果，返回值的`mask`与`dst.mask`相同。

## 数据类型

**表3**数据类型组合

| src0 | src1 | dst |
| --- | --- | --- |
| int16_t | int16_t | int16_t |
| uint16_t | uint16_t | uint16_t |
| half | half | half |
| bfloat16_t | bfloat16_t | bfloat16_t |
| int32_t | int32_t | int32_t |
| uint32_t | uint32_t | uint32_t |
| float | float | float |

## 约束说明

- `dst.mask`需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。
- `dst.mask`对应位置为`0`时，返回值`reg`的对应位置置零。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式，`count`表示待处理的元素个数。

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void mula_example(
    InputTensor initial_input, InputTensor input0, InputTensor input1, OutputTensor output, uint32_t count)
{
    constexpr uint32_t vector_length = asc_get_vf_len() / sizeof(int32_t);
    for (uint32_t offset = 0; count > 0; offset += vector_length) {
        auto coord = asc::te::make_coord(offset);
        auto mask = asc::te::experimental::update_mask<int32_t>(count);
        auto dst = asc::te::experimental::load(initial_input, coord).with_mask(mask);
        auto src0 = asc::te::experimental::load(input0, coord).with_mask(mask);
        auto src1 = asc::te::experimental::load(input1, coord).with_mask(mask);
        dst = asc::te::experimental::mula(dst, src0, src1);
        asc::te::experimental::store(output, coord, dst);
    }
}
```
