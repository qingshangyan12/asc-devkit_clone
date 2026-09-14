# muls_cast

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径：`"tensor_api/experimental/vector_compute.h"`。

将`float`输入与标量逐元素相乘，并将结果转换为`half`，写入返回寄存器的偶数`half`位置。

$$
dst_{2i} = \operatorname{RNA}_{half}(src_i \times scalar)
$$

## 函数原型

```cpp
template <typename T, typename U, cast_layout Layout = cast_layout::zero>
__simd_callee__ inline reg_tensor<T> muls_cast(
    const reg_tensor<U>& src, const U& scalar)
```

## 参数说明

**表1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| `T` | 返回值元素类型，当前仅支持`half`。 |
| `U` | 源操作数和标量类型，当前仅支持`float`。 |
| `Layout` | 结果布局。仅支持`cast_layout::zero`，默认值为`cast_layout::zero`。 |

**表2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src` | 输入 | 源操作数，类型为`reg_tensor<float>`。`src.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `scalar` | 输入 | `float`标量乘数。 |

## 数据类型

src和scalar的数据类型为float，返回值的元素类型为half。

## 返回值说明

返回`reg_tensor<half>`。`reg`保存乘法及类型转换结果，结果写入偶数位置，奇数位置置零。`src.mask`对应位置为`0`时，`reg`的对应位置置零。返回值的`mask`与`src.mask`相同。

## 约束说明

- src.mask需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。
- `T`和`U`当前仅支持`half`和`float`的组合。
- `Layout`仅支持`cast_layout::zero`。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void muls_cast_example(InputTensor input, OutputTensor output)
{
    auto src = asc::te::experimental::load(input, asc::te::make_coord(0)).with_mask(asc::te::experimental::all_mask<float>());
    auto result = asc::te::experimental::muls_cast<half>(src, 1.5f);
    asc::te::experimental::store(output, asc::te::make_coord(0), result);
}
```
