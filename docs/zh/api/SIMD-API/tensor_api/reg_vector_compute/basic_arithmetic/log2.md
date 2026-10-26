# log2

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径：`"tensor_api/experimental/arch/vector/basic_arithmetic.h"`。

该接口根据输入reg_tensor携带的mask，对源操作数按元素计算以2为底的对数，并返回计算结果。计算公式如下：

$$
dst_i = \log_2(src_i)
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> log2(const reg_tensor<T>& src)
```

## 参数说明

**表1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。支持的数据类型请参考[数据类型](#数据类型)。 |

**表2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| src | 输入 | 源操作数，类型为reg_tensor&lt;T&gt;。其中，src.reg保存矢量数据，src.mask用于控制各元素是否参与计算。src.mask中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。half类型的mask使用限制请参考[约束说明](#约束说明)。 |

## 数据类型

源操作数与返回值的数据类型保持一致。支持的数据类型为：half、float。

## 返回值说明

返回按元素计算以2为底对数的结果，类型为reg_tensor&lt;T&gt;。返回值mask与src.mask相同。float类型下，src.mask对应位置为0时，返回值的对应元素置零；half类型当前仅应使用全有效mask，详见[约束说明](#约束说明)。

## 约束说明

- src.mask需通过[with_mask](../reg_tensor/reg_tensor.md)接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。
- 当前实现中，half类型需使用[all_mask&lt;half&gt;](../mask_reg_compute/all_mask.md)设置全有效mask。使用部分有效mask时，部分有效元素的计算结果可能错误，且无效元素不保证置零。该限制不适用于float类型。

## 关键特性说明

### 精度提升

当源操作数数据类型为half时，将half提升为float后计算自然对数，再乘以常量`1.4426950408889634`（即`1 / ln(2)`，以float类型参与计算），最后将结果转换为half后输出。源操作数类型与返回值类型保持一致。

## 调用示例

以下示例的输入和输出均为UB中的half类型Tensor，每次处理128个元素，使用全有效mask。

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/experimental/arch/vector/basic_arithmetic.h"
#include "tensor_api/experimental/arch/vector/mask_reg_compute.h"
#include "tensor_api/experimental/arch/vector/reg_data_load.h"
#include "tensor_api/experimental/arch/vector/reg_data_store.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void log2_example(InputTensor input, OutputTensor output)
{
    auto mask = asc::te::experimental::all_mask<half>();
    auto src = asc::te::experimental::load(input, asc::te::make_coord(0)).with_mask(mask);
    auto dst = asc::te::experimental::log2(src);
    asc::te::experimental::store(output, asc::te::make_coord(0), dst);
}
```

完整的half对比样例及验证结果请参考[基础API与Tensor API的Log2对比样例](../../../../../../../examples/01_simd_cpp_api/07_tensor_api/experimental/reg_vector_compute/log2/README.md)。该样例包含全有效mask和部分有效mask的对比测试，其中部分有效mask用于复现上述已知限制，不作为推荐用法。
