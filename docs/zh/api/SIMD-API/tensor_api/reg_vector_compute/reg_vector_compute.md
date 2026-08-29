# Reg矢量计算

Tensor API Reg矢量计算接口位于`asc::te::experimental`命名空间，以`reg_tensor<T>`为操作对象，支持函数调用和C++运算符表达式。

## 头文件

```cpp
#include "tensor_api/experimental/vector_compute.h"
```

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3训练系列产品/Atlas A3推理系列产品：不支持
- Atlas A2训练系列产品/Atlas A2推理系列产品：不支持
- Atlas 200I/500 A2推理产品：不支持
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
- Atlas训练系列产品：不支持

接口仅支持`__NPU_ARCH__ == 3510`，须在AIV的SIMD调用链中使用，通常由`__simd_vf__`函数直接或间接调用。

## 接口分类

| 分类 | 说明 |
| --- | --- |
| [基础算术](basic_arithmetic/basic_arithmetic.md) | 自然对数、加减、带进位加减、标量乘和最大值。 |
| [operator\|](logical_compute/bit_or.md) | 按位或。 |

## 调用示例

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void reg_vector_compute_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    using asc::te::experimental::operator+;
    using asc::te::experimental::all_mask;
    using asc::te::experimental::load;
    using asc::te::experimental::store;

    auto mask = all_mask<float>();
    auto src0 = load(input0, asc::te::make_coord(0)).with_mask(mask);
    auto src1 = load(input1, asc::te::make_coord(0)).with_mask(mask);
    auto result = asc::te::experimental::log(src0) + src1;
    store(output, asc::te::make_coord(0), result);
}
```
