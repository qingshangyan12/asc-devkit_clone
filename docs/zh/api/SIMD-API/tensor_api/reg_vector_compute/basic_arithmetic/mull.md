# mull<a name="ZH-CN_TOPIC_TENSOR_API_MULL"></a>

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

## 功能说明<a name="section618mcpsimp"></a>

头文件路径：`"tensor_api/experimental/vector_compute.h"`。

该接口根据src0携带的mask，对两个32位整数源操作数按元素相乘，返回64位乘积的低32位，并通过high输出高32位。计算公式如下：

$$
\{high_i, low_i\} = src0_i \times src1_i
$$

## 函数原型<a name="section620mcpsimp"></a>

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> mull(
    reg_tensor<T>& high,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1)
```

## 参数说明<a name="section622mcpsimp"></a>

**表 1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。支持的数据类型请参考[数据类型](#数据类型)。 |

**表 2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| high | 输出 | 乘积的高32位，类型为reg_tensor&lt;T&gt;。返回时high.mask与src0.mask相同。 |
| src0 | 输入 | 源操作数，类型为reg_tensor&lt;T&gt;。src0.reg保存矢量数据，src0.mask控制各元素是否参与计算。 |
| src1 | 输入 | 源操作数，类型为reg_tensor&lt;T&gt;。src1.mask不参与本次计算。 |

## 数据类型

源操作数、high与返回值的数据类型保持一致。支持的数据类型为：int32_t、uint32_t。

## 返回值说明<a name="section640mcpsimp"></a>

返回乘积的低32位，类型为reg_tensor&lt;T&gt;。返回值mask与src0.mask相同；src0.mask对应位置为0时，返回值和high的对应元素均置零。

## 约束说明<a name="section633mcpsimp"></a>

src0.mask需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。

## 调用示例<a name="section642mcpsimp"></a>

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void mull_example(InputTensor input0, InputTensor input1, OutputTensor output)
{
    auto mask = asc::te::experimental::all_mask<uint32_t>();
    auto src0 = asc::te::experimental::load(input0, asc::te::make_coord(0)).with_mask(mask);
    auto src1 = asc::te::experimental::load(input1, asc::te::make_coord(0));
    asc::te::reg_tensor<uint32_t> high;
    auto low = asc::te::experimental::mull(high, src0, src1);
    asc::te::experimental::store(output, asc::te::make_coord(0), low);
    asc::te::experimental::store(output, asc::te::make_coord(1), high);
}
```
