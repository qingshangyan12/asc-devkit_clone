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
| `dst` | 输入 | 累加初值，同时提供结果mask。 |
| `src` | 输入 | 与标量相乘的输入寄存器。 |
| `scalar` | 输入 | 标量乘数，类型必须与`T`完全一致。 |

## 支持的数据类型

| `dst` | `src` | `scalar` | 返回值 |
| --- | --- | --- | --- |
| `reg_tensor<half>` | `reg_tensor<half>` | `half` | `reg_tensor<half>` |
| `reg_tensor<float>` | `reg_tensor<float>` | `float` | `reg_tensor<float>` |

## 返回值说明

返回`reg_tensor<T>`：

- `reg`：有效元素保存`dst.reg + scalar * src.reg`，无效元素置零。
- `mask`：等于`dst.mask`；`src.mask`不参与执行mask的选择。

## 约束说明

- mask初始化和传导规则请参见[mask使用与传导](../reg_vector_compute.md#mask使用与传导)。
- `T`不是`half`或`float`时，编译失败，并提示`axpy supports half and float`。
- `dst`与`src`的元素类型不一致时，找不到匹配的`axpy`重载，编译失败。
- `scalar`的类型与`T`不完全一致时，编译失败，并提示标量与寄存器元素类型必须匹配；例如`T`为`float`
  时应传入`0.25f`，不能传入`0.25`。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

同类Tensor API样例的Kernel、Host调用和数据验证流程请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/07_tensor_api/reg_vector_compute/composite_compute/)。

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
