# sub

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

对两个32位整数寄存器执行逐元素减法，返回减法结果的低32位，并通过引用参数`carry`输出补码减法的进位：

$$
dst_i = src0_i - src1_i
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> sub(
    reg_tensor<bool>& carry,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，支持`int32_t`、`uint32_t`。 |
| `carry` | 输出 | 逐元素进位输出，类型为`reg_tensor<bool>`。`reg`保存进位，`mask`由接口写为`src0.mask`。 |
| `src0` | 输入 | 被减数，类型为`reg_tensor<T>`，同时提供执行mask。 |
| `src1` | 输入 | 减数，类型为`reg_tensor<T>`；`src1.mask`不参与执行mask的选择。 |

## 返回值说明

返回`reg_tensor<T>`：

- `reg`：保存`src0.reg - src1.reg`的低32位。
- `mask`：等于`src0.mask`。

`carry.reg`保存补码减法的逐元素进位，`1`表示未借位，`0`表示发生借位。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void sub_with_carry_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    auto mask = asc::te::all_mask<uint32_t>();
    auto src0 = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1 = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);

    asc::te::reg_tensor<bool> carry;
    auto result = asc::te::experimental::sub(carry, src0, src1);
    output.store(asc::te::make_coord(0, 0), result);
}
```
