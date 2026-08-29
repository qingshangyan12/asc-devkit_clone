# max

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3训练系列产品/Atlas A3推理系列产品：不支持
- Atlas A2训练系列产品/Atlas A2推理系列产品：不支持
- Atlas 200I/500 A2推理产品：不支持
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
- Atlas训练系列产品：不支持

## 功能说明

头文件路径：`"tensor_api/experimental/vector_compute.h"`。

命名空间：`asc::te::experimental`。

逐元素返回两个操作数中的较大值，支持寄存器与寄存器、寄存器与标量、标量与寄存器三种组合。

$$
dst_i = \max(a_i, b_i)
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> max(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)

template <typename T>
__simd_callee__ inline reg_tensor<T> max(
    const reg_tensor<T>& src, const T& scalar)

template <typename T>
__simd_callee__ inline reg_tensor<T> max(
    const T& scalar, const reg_tensor<T>& src)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，支持`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。 |
| `src0` | 输入 | 寄存器/寄存器形式中的源操作数，类型为`reg_tensor<T>`。`src0.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `src1` | 输入 | 寄存器/寄存器形式中的源操作数，类型为`reg_tensor<T>`。`src1.mask`不用于本次计算。 |
| `src` | 输入 | 寄存器/标量或标量/寄存器形式中的源操作数，类型为`reg_tensor<T>`。`src.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `scalar` | 输入 | 标量操作数，类型为`T`。 |

## 返回值说明

返回`reg_tensor<T>`。`reg`保存两个操作数逐元素比较后的较大值，计算所用的输入`mask`对应位置为`0`时，`reg`的对应位置置零。寄存器/寄存器形式的返回值`mask`与`src0.mask`相同，包含标量的形式与`src.mask`相同。

## 约束说明

- 标量类型必须与寄存器元素类型一致。
- 浮点输入分别为`-0`和`+0`时，返回`+0`。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void max_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    using asc::te::experimental::all_mask;
    using asc::te::experimental::load;
    using asc::te::experimental::store;

    auto mask = all_mask<float>();
    auto src0_reg = load(input0, asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_reg = load(input1, asc::te::make_coord(0, 0)).with_mask(mask);

    auto result = asc::te::experimental::max(src0_reg, src1_reg);
    store(output, asc::te::make_coord(0, 0), result);
}
```
