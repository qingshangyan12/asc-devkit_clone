# muls_cast

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

将`float`输入与标量逐元素相乘，并将结果转换为`half`。转换结果写入返回寄存器的偶数`half`位置。

$$
dst_{2i} = \operatorname{RNA}_{half}(src_i \times scalar)
$$

## 函数原型

```cpp
__simd_callee__ inline decltype(auto) muls_cast(
    const reg_tensor<float>& src, const float& scalar)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src` | 输入 | 源操作数，类型为`reg_tensor<float>`。`src.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `scalar` | 输入 | `float`标量乘数。 |

## 返回值说明

返回`reg_tensor<half>`。`reg`保存乘法及类型转换结果，每个参与计算的`float`元素对应一个位于偶数位置的`half`结果。`src.mask`对应位置为`0`时，`reg`的对应位置置零。返回值的`mask`与`src.mask`相同。

## 约束说明

返回值采用偶数位置布局，不能按连续`half`数据解释。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void muls_cast_example(InputTensor input, OutputTensor output)
{
    auto src = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<float>());
    auto result = asc::te::experimental::muls_cast(src, 1.5f);
    output.store(asc::te::make_coord(0, 0), result);
}
```
