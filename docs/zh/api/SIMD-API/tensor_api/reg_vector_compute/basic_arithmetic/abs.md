# abs

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

对`src_reg`中mask有效的元素逐个取绝对值，并返回计算结果：

$$
dst_i = |src_i|
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> abs(const reg_tensor<T>& src_reg)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型。 |
| `src_reg` | 输入 | 待计算的`reg_tensor<T>`，同时携带有效元素mask。 |

## 支持的数据类型

| 模板参数 | 支持的数据类型 |
| --- | --- |
| `T` | `int8_t`、`int16_t`、`half`、`int32_t`、`float` |

## 返回值说明

返回`reg_tensor<T>`：

- `reg`：有效元素为`abs(src_reg.reg)`，无效元素置零。
- `mask`：等于`src_reg.mask`。

## 约束说明

- mask初始化和传导规则请参见[mask使用与传导](../reg_vector_compute.md#mask使用与传导)。
- 整型计算采用非饱和行为。对于有符号整数的最小值，其绝对值无法由原类型表示，结果按原位宽保留。
  例如，`int8_t(-128)`的计算结果仍为`int8_t(-128)`。
- 当`T`可实例化为`reg_tensor<T>`、但不是上述支持类型时，编译期校验失败，诊断信息包含
  `abs does not support this element type`。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void abs_example(InputTensor input, OutputTensor output)
{
    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<float>());
    auto dst_reg = asc::te::experimental::abs(src_reg);
    output.store(asc::te::make_coord(0, 0), dst_reg);
}
```
