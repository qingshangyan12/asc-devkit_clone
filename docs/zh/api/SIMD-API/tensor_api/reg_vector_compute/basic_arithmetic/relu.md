# relu

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

逐元素执行ReLU：

$$
dst_i = \max(src_i, 0)
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> relu(const reg_tensor<T>& src_reg)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型。 |
| `src_reg` | 输入 | 待计算的`reg_tensor<T>`。 |

## 支持的数据类型

| 模板参数 | 支持的数据类型 |
| --- | --- |
| `T` | `half`、`int32_t`、`float` |

## 返回值说明

返回`reg_tensor<T>`：

- `reg`：有效元素为`max(src_reg.reg, 0)`，无效元素置零。
- `mask`：等于`src_reg.mask`。

## 约束说明

- mask初始化和传导规则请参见[mask使用与传导](../reg_vector_compute.md#mask使用与传导)。
- `T`不是`half`、`int32_t`或`float`时，编译失败，并提示
  `relu supports half, int32_t, and float`。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

同类Tensor API样例的Kernel、Host调用和数据验证流程请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/07_tensor_api/reg_vector_compute/basic_arithmetic/)。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void relu_example(InputTensor input, OutputTensor output)
{
    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<float>());

    auto dst_reg = asc::te::experimental::relu(src_reg);
    output.store(asc::te::make_coord(0, 0), dst_reg);
}
```
