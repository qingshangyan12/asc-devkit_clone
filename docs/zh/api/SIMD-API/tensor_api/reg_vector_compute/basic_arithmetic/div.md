# operator/

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

根据`src0.mask`，对`src0`和`src1`按元素求商。

$$
dst_i = \frac{src0_i}{src1_i}
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> operator/(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，支持`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float`。 |
| `src0` | 输入 | 被除数，类型为`reg_tensor<T>`。`src0.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `src1` | 输入 | 除数，类型为`reg_tensor<T>`。`src1.mask`不用于本次计算。 |

## 返回值说明

返回`reg_tensor<T>`。`reg`保存逐元素求商结果，`src0.mask`对应位置为`0`时，`reg`的对应位置置零。返回值的`mask`与`src0.mask`相同。

## 约束说明

- 使用整数类型时，除数不能为`0`。
- 使用浮点类型时，非零有限值除以`0`得到带符号的`inf`，`0`除以`0`、`inf`除以`inf`或任一操作数为`nan`时得到`nan`，有限值除以`inf`得到`0`，`inf`除以有限值或`0`得到带符号的`inf`。
- `half`在INF/NAN模式下遵循上述浮点规则。
- `half`在饱和模式下，结果为`inf`的计算将饱和为带符号的最大有限值`65504.0`或`-65504.0`，结果为`nan`的计算将得到`0`。`float`在饱和模式下的结果与INF/NAN模式相同。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void operator_slash_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    using asc::te::experimental::operator/;

    auto mask = asc::te::all_mask<float>();
    auto src0_reg = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_reg = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto result = src0_reg / src1_reg;
    output.store(asc::te::make_coord(0, 0), result);
}
```
