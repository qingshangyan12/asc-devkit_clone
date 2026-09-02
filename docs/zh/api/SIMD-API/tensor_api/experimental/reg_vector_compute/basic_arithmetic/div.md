# operator/

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

该接口根据src0携带的mask，对两个源操作数按元素相除，并返回计算结果。计算公式如下：

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

**表1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。支持的数据类型请参考[数据类型](#数据类型)。 |

**表2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| src0 | 输入 | 被除数，类型为reg_tensor&lt;T&gt;。src0.reg保存矢量数据，src0.mask控制各元素是否参与计算。 |
| src1 | 输入 | 除数，类型为reg_tensor&lt;T&gt;。src1.mask不参与本次计算。 |

## 数据类型

源操作数与返回值的数据类型保持一致。支持的数据类型为：int16_t、uint16_t、half、int32_t、uint32_t、float。

## 返回值说明

返回按元素相除的结果，类型为reg_tensor&lt;T&gt;。返回值mask与src0.mask相同；src0.mask对应位置为0时，返回值的对应元素置零。

## 约束说明

- src0.mask需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。
- 使用整数类型时，除数不能为0。
- 使用half或float类型时，非零有限值除以0得到带符号的inf，0除以0、inf除以inf或任一操作数为nan时得到nan，有限值除以inf得到0，inf除以有限值或0得到带符号的inf。
- half在饱和模式下，结果为inf时输出带符号的最大有限值65504.0或-65504.0，结果为nan时输出0。float在饱和模式下的结果与INF/NAN模式相同。

## 调用示例

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void div_example(InputTensor input0, InputTensor input1, OutputTensor output)
{
    using asc::te::experimental::operator/;
    auto mask = asc::te::experimental::all_mask<float>();
    auto src0 = asc::te::experimental::load(input0, asc::te::make_coord(0)).with_mask(mask);
    auto src1 = asc::te::experimental::load(input1, asc::te::make_coord(0));
    auto dst = src0 / src1;
    asc::te::experimental::store(output, asc::te::make_coord(0), dst);
}
```
