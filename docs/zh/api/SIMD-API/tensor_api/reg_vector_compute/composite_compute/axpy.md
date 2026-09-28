# axpy

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

逐元素将`src`与标量相乘，再与`dst`原值相加：

$$
result_i = scalar \times src_i + dst_i
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> axpy(
    reg_tensor<T>& dst,
    const reg_tensor<T>& src,
    const T& scalar)
```

## 参数说明

**表1**模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | reg_tensor元素类型。支持的数据类型请参考[数据类型](#数据类型)。 |

**表2**参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| dst | 输入/输出 | 加法操作数和结果，类型为`reg_tensor<T>`。接口直接更新`dst.reg`，`dst.mask`用于标记参与计算的元素。 |
| src | 输入 | 与标量相乘的源操作数，类型为`reg_tensor<T>`。`src.mask`不用于本次计算。 |
| scalar | 输入 | 标量乘数，类型必须与`T`完全一致。 |

## 数据类型

dst、src、scalar与返回值的数据类型保持一致。支持的数据类型为：half、float。

## 返回值说明

返回`reg_tensor<T>`。`reg`保存`dst + scalar * src`的逐元素计算结果，返回值的`mask`与`dst.mask`相同。

## 约束说明

- `dst.mask`需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。
- `dst.mask`对应位置为`0`时，返回值`reg`的对应位置置零。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，再执行计算并将结果写回UB。

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void axpy_example(
    InputTensor initial_input, InputTensor src_input, OutputTensor output)
{
    auto mask = asc::te::experimental::update_mask<float>(48);
    auto dst = asc::te::experimental::load(initial_input, asc::te::make_coord(0)).with_mask(mask);
    auto src_reg = asc::te::experimental::load(src_input, asc::te::make_coord(0)).with_mask(mask);

    dst = asc::te::experimental::axpy(dst, src_reg, 0.25f);
    asc::te::experimental::store(output, asc::te::make_coord(0), dst);
}
```
