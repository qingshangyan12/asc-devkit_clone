# operator*

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

逐元素执行寄存器与标量乘法，支持寄存器与标量、标量与寄存器两种操作数组合。

$$
dst_i = a_i \times b_i
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(
    const reg_tensor<T>& src, const T& scalar)

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(
    const T& scalar, const reg_tensor<T>& src)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 寄存器元素类型。具体支持范围请参见“支持的数据类型”。 |
| `src` | 输入 | 寄存器/标量或标量/寄存器形式中的寄存器操作数，类型为`reg_tensor<T>`。`src.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `scalar` | 输入 | 标量乘数，类型为`T`。 |

## 支持的数据类型

| 操作数组合 | 寄存器元素类型 |
| --- | --- |
| 寄存器/标量、标量/寄存器 | `int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float` |

## 返回值说明

返回`reg_tensor<T>`。`reg`保存逐元素乘法结果，计算所用的输入`mask`对应位置为`0`时，`reg`的对应位置置零。返回值的`mask`与`src.mask`相同。

## 约束说明

标量类型必须与寄存器元素类型一致。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void mul_example(InputTensor input, OutputTensor output)
{
    using asc::te::experimental::operator*;
    using asc::te::experimental::all_mask;
    using asc::te::experimental::load;
    using asc::te::experimental::store;

    auto mask = all_mask<float>();
    auto src = load(input, asc::te::make_coord(0, 0)).with_mask(mask);
    auto result = src * 2.0f;
    store(output, asc::te::make_coord(0, 0), result);
}
```
