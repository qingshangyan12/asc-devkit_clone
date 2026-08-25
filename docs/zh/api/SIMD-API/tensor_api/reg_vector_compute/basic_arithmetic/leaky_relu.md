# leaky_relu

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

使用标量斜率执行Leaky ReLU：

$$
dst_i = \begin{cases}
src_i, & src_i \ge 0 \\
src_i \times slope, & src_i < 0
\end{cases}
$$

## 函数原型

```cpp
template <typename T, typename scalar_type>
__simd_callee__ inline reg_tensor<T> leaky_relu(
    const reg_tensor<T>& src_reg, const scalar_type& slope)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，支持`half`、`float`。 |
| `scalar_type` | 模板参数 | 标量斜率类型，必须可转换为`T`。 |
| `src_reg` | 输入 | 源操作数，类型为`reg_tensor<T>`。`src_reg.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `slope` | 输入 | 负数区间使用的标量斜率。 |

## 返回值说明

返回`reg_tensor<T>`。`src_reg`中的元素大于等于`0`时，`reg`保存原值，否则保存该元素与`slope`的乘积。`src_reg.mask`对应位置为`0`时，`reg`的对应位置置零。返回值的`mask`与`src_reg.mask`相同。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void leaky_relu_example(InputTensor input, OutputTensor output)
{
    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<float>());

    auto dst_reg = asc::te::experimental::leaky_relu(src_reg, 0.1f);
    output.store(asc::te::make_coord(0, 0), dst_reg);
}
```
