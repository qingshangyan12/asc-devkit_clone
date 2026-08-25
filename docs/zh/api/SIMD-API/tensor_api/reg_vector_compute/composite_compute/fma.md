# fma

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

逐元素执行融合乘加：

$$
dst_i = src0_i \times src1_i + src2_i
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline decltype(auto) fma(
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1,
    const reg_tensor<T>& src2)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，支持`half`、`float`。 |
| `src0` | 输入 | 第一个乘数，类型为`reg_tensor<T>`。`src0.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `src1` | 输入 | 第二个乘数，类型为`reg_tensor<T>`。`src1.mask`不用于本次计算。 |
| `src2` | 输入 | 加数，类型为`reg_tensor<T>`。`src2.mask`不用于本次计算。 |

## 返回值说明

返回`reg_tensor<T>`。`reg`保存`src0 * src1 + src2`的逐元素融合乘加结果，`src0.mask`对应位置为`0`时，`reg`的对应位置置零。返回值的`mask`与`src0.mask`相同。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void fma_example(
    InputTensor input0, InputTensor input1, InputTensor input2, OutputTensor output)
{
    auto mask = asc::te::all_mask<float>();
    auto src0_reg = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_reg = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src2_reg = input2.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto result = asc::te::experimental::fma(src0_reg, src1_reg, src2_reg);
    output.store(asc::te::make_coord(0, 0), result);
}
```
