# addc

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

`addc`提供带进位输出加法，以及带进位输入和输出加法。

### 带进位输出加法

对`src0`和`src1`按元素求和，返回加法和的低32位，并通过`carry`输出每个元素产生的进位。

$$
\{carry_i, dst_i\} = \{1'b0, src0_i\} + \{1'b0, src1_i\}
$$

### 带进位输入和输出加法

对`src0`、`src1`和`carry_src`中的输入进位按元素求和，返回加法和的低32位，并通过`carry`输出每个元素产生的新进位。

$$
\{carry_i, dst_i\} = \{1'b0, src0_i\} + \{1'b0, src1_i\} + \{32'b0, carry\_src_i\}
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> addc(
    reg_tensor<bool>& carry,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1)

template <typename T>
__simd_callee__ inline reg_tensor<T> addc(
    reg_tensor<bool>& carry,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1,
    const reg_tensor<bool>& carry_src)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，仅支持`uint32_t`。 |
| `carry` | 输出 | 进位输出，类型为`reg_tensor<bool>`。产生进位时对应元素为`1`，否则为`0`；返回时`carry.mask`与`src0.mask`相同。 |
| `src0` | 输入 | 源操作数，类型为`reg_tensor<T>`。`src0.mask`标记参与计算的元素。 |
| `src1` | 输入 | 源操作数，类型为`reg_tensor<T>`。`src1.mask`不用于本次计算。 |
| `carry_src` | 可选输入 | 输入进位，类型为`reg_tensor<bool>`。`carry_src.mask`不用于本次计算。 |

## 返回值说明

返回`reg_tensor<T>`。`reg`保存逐元素相加后的低32位；`src0.mask`对应位置为`0`时，结果的对应位置置零。返回值的`mask`与`src0.mask`相同。

## 约束说明

当一个大于32位的整数拆成多个32位部分进行加法时，最低32位使用不带`carry_src`的重载，其余部分从低位到高位依次使用带`carry_src`的重载。

## 调用示例

以下示例展示64位无符号整数拆分为高低两个32位部分时的加法。

```cpp
#include "tensor_api/experimental/vector_compute.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void addc_example(InputTensor input0, InputTensor input1, OutputTensor output)
{
    auto mask = asc::te::experimental::all_mask<uint32_t>();
    auto src0_low = asc::te::experimental::load(input0, asc::te::make_coord(0)).with_mask(mask);
    auto src1_low = asc::te::experimental::load(input1, asc::te::make_coord(0)).with_mask(mask);
    auto src0_high = asc::te::experimental::load(input0, asc::te::make_coord(1)).with_mask(mask);
    auto src1_high = asc::te::experimental::load(input1, asc::te::make_coord(1)).with_mask(mask);

    asc::te::experimental::reg_tensor<bool> carry_low;
    auto low = asc::te::experimental::addc(carry_low, src0_low, src1_low);
    asc::te::experimental::reg_tensor<bool> carry_high;
    auto high = asc::te::experimental::addc(carry_high, src0_high, src1_high, carry_low);

    asc::te::experimental::store(output, asc::te::make_coord(0), low);
    asc::te::experimental::store(output, asc::te::make_coord(1), high);
}
```
