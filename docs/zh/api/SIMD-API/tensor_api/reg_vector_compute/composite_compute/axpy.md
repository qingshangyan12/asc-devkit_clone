# axpy

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

逐元素将`src`与标量相乘，再与`dst`原值相加：

$$
result_i = scalar \times src_i + dst_i
$$

## 函数原型

```cpp
template <typename T, typename scalar_type>
__simd_callee__ inline decltype(auto) axpy(
    const reg_tensor<T>& dst,
    const reg_tensor<T>& src,
    const scalar_type& scalar)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，支持`half`、`float`。 |
| `scalar_type` | 模板参数 | 标量类型，必须与`T`相同。 |
| `dst` | 输入 | 累加初值，同时提供结果mask。 |
| `src` | 输入 | 与标量相乘的输入寄存器。 |
| `scalar` | 输入 | 标量乘数，类型必须与`T`完全一致。 |

## 返回值说明

返回`reg_tensor<T>`：

- `reg`：有效元素保存`dst.reg + scalar * src.reg`，无效元素置零。
- `mask`：等于`dst.mask`；`src.mask`不参与执行mask的选择。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void axpy_example(
    InputTensor initial_input, InputTensor src_input, OutputTensor output)
{
    auto mask = asc::te::all_mask<float>();
    auto initial_reg = initial_input.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src_reg = src_input.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto result = asc::te::experimental::axpy(initial_reg, src_reg, 0.25f);
    output.store(asc::te::make_coord(0, 0), result);
}
```
