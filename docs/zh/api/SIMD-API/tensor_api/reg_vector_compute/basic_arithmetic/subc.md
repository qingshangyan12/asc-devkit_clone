# subc

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

减法运算在硬件底层通过补码加法实现，carry中的进位标志为1表示减法未借位，为0表示减法发生借位。该接口支持以下两种按元素减法：

- 带进位输出减法：对src0和src1求差，返回计算结果的低32位，并通过carry输出进位标志。
- 带进位输入和输出减法：对src0、src1和carry_src中的进位求差，返回计算结果的低32位，并通过carry输出新的进位标志。

计算公式如下：

$$
\{carry_i, dst_i\} = \{1'b0, src0_i\} + \{1'b0, \sim src1_i\} + 33'b1
$$

$$
\{carry_i, dst_i\} = \{1'b0, src0_i\} + \{1'b0, \sim src1_i\} + \{32'b0, carry\_src_i\}
$$

## 函数原型

- 带进位输出减法

    ```cpp
    template <typename T>
    __simd_callee__ inline reg_tensor<T> subc(
        reg_tensor<bool>& carry,
        const reg_tensor<T>& src0,
        const reg_tensor<T>& src1)
    ```

- 带进位输入和输出减法

    ```cpp
    template <typename T>
    __simd_callee__ inline reg_tensor<T> subc(
        reg_tensor<bool>& carry,
        const reg_tensor<T>& src0,
        const reg_tensor<T>& src1,
        const reg_tensor<bool>& carry_src)
    ```

## 参数说明

**表1**  模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 操作数数据类型。支持int32_t、uint32_t。 |

### 带进位输出减法

**表2**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| carry | 输出 | 进位标志，类型为reg_tensor&lt;bool&gt;。carry.reg中，未发生借位的元素为1，发生借位的元素为0；返回时carry.mask与src0.mask相同。 |
| src0 | 输入 | 被减数，类型为reg_tensor&lt;T&gt;。src0.reg保存矢量数据，src0.mask控制各元素是否参与计算。 |
| src1 | 输入 | 减数，类型为reg_tensor&lt;T&gt;。src1.mask不参与本次计算。 |

### 带进位输入和输出减法

**表3**  参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| carry | 输出 | 新的进位标志，类型为reg_tensor&lt;bool&gt;。carry.reg中，未发生借位的元素为1，发生借位的元素为0；返回时carry.mask与src0.mask相同。 |
| src0 | 输入 | 被减数，类型为reg_tensor&lt;T&gt;。src0.reg保存矢量数据，src0.mask控制各元素是否参与计算。 |
| src1 | 输入 | 减数，类型为reg_tensor&lt;T&gt;。src1.mask不参与本次计算。 |
| carry_src | 输入 | 输入进位，类型为reg_tensor&lt;bool&gt;。carry_src.reg中的进位标志参与计算，carry_src.mask不参与本次计算。 |

## 数据类型

src0、src1与返回值的数据类型保持一致，支持int32_t、uint32_t。carry和carry_src的元素类型为bool。

## 返回值说明

返回减法结果的低32位，类型为reg_tensor&lt;T&gt;。返回值mask与src0.mask相同；src0.mask对应位置为0时，返回值和carry的对应元素均置零。

## 约束说明

src0.mask需通过`with_mask`接口预先设置。未设置时，mask的内容不确定，会导致参与计算的元素位置错误。

## 调用示例

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void subc_example(InputTensor input0, InputTensor input1, OutputTensor output)
{
    auto mask = asc::te::experimental::all_mask<uint32_t>();
    auto src0_low = asc::te::experimental::load(input0, asc::te::make_coord(0)).with_mask(mask);
    auto src1_low = asc::te::experimental::load(input1, asc::te::make_coord(0));
    auto src0_high = asc::te::experimental::load(input0, asc::te::make_coord(1)).with_mask(mask);
    auto src1_high = asc::te::experimental::load(input1, asc::te::make_coord(1));

    asc::te::experimental::reg_tensor<bool> carry_low;
    auto low = asc::te::experimental::subc(carry_low, src0_low, src1_low);
    asc::te::experimental::reg_tensor<bool> carry_high;
    auto high = asc::te::experimental::subc(carry_high, src0_high, src1_high, carry_low);

    asc::te::experimental::store(output, asc::te::make_coord(0), low);
    asc::te::experimental::store(output, asc::te::make_coord(1), high);
}
```
