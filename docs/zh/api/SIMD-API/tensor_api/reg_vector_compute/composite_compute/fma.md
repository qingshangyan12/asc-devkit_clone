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
| `src0` | 输入 | 第一个乘数，同时提供结果mask。 |
| `src1` | 输入 | 第二个乘数。 |
| `src2` | 输入 | 加数。 |

## 支持的数据类型

| 模板参数 | 支持的数据类型 |
| --- | --- |
| `T` | `half`、`float` |

## 返回值说明

返回`reg_tensor<T>`：

- `reg`：有效元素保存`src0.reg * src1.reg + src2.reg`，无效元素置零。
- `mask`：等于`src0.mask`；`src1.mask`和`src2.mask`不参与执行mask的选择。

## 约束说明

- mask初始化和传导规则请参见[mask使用与传导](../reg_vector_compute.md#mask使用与传导)。
- `T`不是`half`或`float`时，编译失败，并提示`fma supports half and float`。
- 三个输入寄存器的元素类型不一致时，找不到匹配的`fma`重载，编译失败。

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
