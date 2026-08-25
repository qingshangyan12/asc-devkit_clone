# operator!

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

对输入寄存器中的有效元素逐位取反。`reg_tensor<bool>`对predicate位取反；其他类型对元素的二进制编码取反。

$$
dst_i = \mathord{\sim}src_i
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> operator!(const reg_tensor<T>& src)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，支持`bool`、`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float`。 |
| `src` | 输入 | 待取反的寄存器，同时提供执行mask。 |

## 返回值说明

返回`reg_tensor<T>`，结果mask等于`src.mask`。

## 约束说明

- 本接口不是数值零值判断。对于非`bool`类型，结果是输入二进制编码逐位取反后的同类型数据。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void logical_not_example(InputTensor input, OutputTensor output)
{
    using asc::te::experimental::operator!;

    auto src = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<int32_t>());
    auto result = !src;
    output.store(asc::te::make_coord(0, 0), result);
}
```
