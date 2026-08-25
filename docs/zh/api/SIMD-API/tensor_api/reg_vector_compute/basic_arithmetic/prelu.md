# prelu

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

使用逐元素斜率执行PReLU：

$$
dst_i = \begin{cases}
src_i, & src_i \ge 0 \\
src_i \times slope_i, & src_i < 0
\end{cases}
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> prelu(
    const reg_tensor<T>& src_reg, const reg_tensor<T>& slope)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入、斜率和返回值的元素类型，支持`half`、`float`。 |
| `src_reg` | 输入 | 待计算的输入寄存器，同时提供执行mask。 |
| `slope` | 输入 | 负数区间使用的逐元素斜率寄存器；`slope.mask`不参与执行mask的选择。 |

## 返回值说明

返回`reg_tensor<T>`：

- `reg`：有效元素在`src_reg.reg >= 0`时取原值，否则取`src_reg.reg * slope.reg`。
- `mask`：等于`src_reg.mask`；`slope.mask`不参与本次执行mask的选择。

## 约束说明

- 仅`src_reg.mask`中的有效元素参与计算；`slope.mask`不改变结果mask。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void prelu_example(
    InputTensor input, InputTensor slope_input, OutputTensor output)
{
    auto mask = asc::te::all_mask<float>();
    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto slope_reg = slope_input.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto dst_reg = asc::te::experimental::prelu(src_reg, slope_reg);
    output.store(asc::te::make_coord(0, 0), dst_reg);
}
```
