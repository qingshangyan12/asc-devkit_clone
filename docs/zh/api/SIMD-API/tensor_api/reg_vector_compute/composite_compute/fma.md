# fma

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

头文件路径：`"tensor_api/experimental/vector_compute.h"`。

逐元素执行融合乘加：

$$
dst_i = src0_i \times src1_i + src2_i
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> fma(
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1,
    const reg_tensor<T>& src2)
```

## 参数说明

**表1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。支持的数据类型请参考[数据类型](#数据类型)。 |

**表2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src0` | 输入 | 第一个乘数，类型为`reg_tensor<T>`。`src0.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `src1` | 输入 | 第二个乘数，类型为`reg_tensor<T>`。`src1.mask`不用于本次计算。 |
| `src2` | 输入 | 加数，类型为`reg_tensor<T>`。`src2.mask`不用于本次计算。 |

## 数据类型

源操作数与返回值的数据类型保持一致。支持的数据类型为：half、bfloat16_t、float。

## 返回值说明

返回`reg_tensor<T>`。`reg`保存`src0 * src1 + src2`的逐元素融合乘加结果，返回值的`mask`与`src0.mask`相同。

## 约束说明

src0.mask需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。

`src0.mask`对应位置为`0`时，返回结果的对应元素置零。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void fma_example(
    InputTensor input0, InputTensor input1, InputTensor input2, OutputTensor output)
{
    auto mask = asc::te::experimental::all_mask<float>();
    auto src0_reg = asc::te::experimental::load(input0, asc::te::make_coord(0)).with_mask(mask);
    auto src1_reg = asc::te::experimental::load(input1, asc::te::make_coord(0)).with_mask(mask);
    auto src2_reg = asc::te::experimental::load(input2, asc::te::make_coord(0)).with_mask(mask);

    auto result = asc::te::experimental::fma(src0_reg, src1_reg, src2_reg);
    asc::te::experimental::store(output, asc::te::make_coord(0), result);
}
```
