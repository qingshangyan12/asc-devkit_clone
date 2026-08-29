# log

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

逐元素计算自然对数：

$$
dst_i = \ln(src_i)
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> log(const reg_tensor<T>& src_reg)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，支持`half`、`float`。 |
| `src_reg` | 输入 | 源操作数，类型为`reg_tensor<T>`。`src_reg.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |

## 返回值说明

返回`reg_tensor<T>`。`reg`保存逐元素自然对数计算结果，返回值的`mask`与`src_reg.mask`相同。

## 约束说明

`src_reg.mask`对应位置为`0`的元素不参与计算，返回值的`reg`对应位置置零。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void log_example(InputTensor input, OutputTensor output)
{
    using asc::te::experimental::all_mask;
    using asc::te::experimental::load;
    using asc::te::experimental::store;

    auto src_reg = load(input, asc::te::make_coord(0, 0)).with_mask(all_mask<float>());

    auto dst_reg = asc::te::experimental::log(src_reg);
    store(output, asc::te::make_coord(0, 0), dst_reg);
}
```
