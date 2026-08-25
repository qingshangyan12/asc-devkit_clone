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

头文件路径：`"tensor_api/tensor.h"`。

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
| `src_reg` | 输入 | 待计算的`reg_tensor<T>`。 |

## 返回值说明

返回`reg_tensor<T>`：

- `reg`：有效元素为`ln(src_reg.reg)`，无效元素置零。
- `mask`：等于`src_reg.mask`。

## 约束说明

- 有效输入元素小于`0`时返回`nan`，等于`0`时返回`-inf`。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void log_example(InputTensor input, OutputTensor output)
{
    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<float>());

    auto dst_reg = asc::te::experimental::log(src_reg);
    output.store(asc::te::make_coord(0, 0), dst_reg);
}
```
