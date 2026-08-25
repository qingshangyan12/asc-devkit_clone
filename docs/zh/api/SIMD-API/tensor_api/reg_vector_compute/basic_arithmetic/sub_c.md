# sub/sub_c

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

`sub`对两个32位整数寄存器执行逐元素减法，返回低32位结果，并通过引用参数输出补码减法的
进位；`sub_c`使用前一段产生的进位继续计算，并输出新的进位。二者用于构成从低位到高位的
多字整数减法链：

$$
\begin{aligned}
sub:\quad dst_i &= src0_i - src1_i \\
sub\_c:\quad dst_i &= src0_i + \mathord{\sim}src1_i + carrySrc_i
\end{aligned}
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> sub(
    reg_tensor<bool>& carry,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1)

template <typename T>
__simd_callee__ inline reg_tensor<T> sub_c(
    reg_tensor<bool>& carry,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1,
    const reg_tensor<bool>& carry_src)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `carry` | 输出 | `sub`和`sub_c`的逐元素进位输出。`reg`保存进位，`mask`由接口写为`src0.mask`。 |
| `src0` | 输入 | 被减数，类型为`reg_tensor<T>`，同时提供执行mask。 |
| `src1` | 输入 | 减数，类型为与`src0`相同的`reg_tensor<T>`；`src1.mask`不参与执行mask的选择。 |
| `carry_src` | 输入 | 仅用于`sub_c`。逐元素进位输入，读取其`reg`；`carry_src.mask`不参与执行mask的选择。 |

## 支持的数据类型

| 参数 | 支持的数据类型 |
| --- | --- |
| `T` | `int32_t`、`uint32_t` |
| `carry`、`carry_src` | `reg_tensor<bool>` |

## 返回值说明

`sub`和`sub_c`均返回`reg_tensor<T>`，并通过`carry`返回借位状态：

- 返回值`reg`：`sub`保存`src0.reg - src1.reg`的低32位；`sub_c`保存
  `src0.reg + ~src1.reg + carry_src.reg`的低32位。
- 返回值`mask`：等于`src0.mask`。
- `carry.reg`：`1`表示未借位，`0`表示发生借位；`carry.mask`等于`src0.mask`。

## 约束说明

- mask初始化和传导规则请参见[mask使用与传导](../reg_vector_compute.md#mask使用与传导)。
- `sub`的`T`不是`int32_t`或`uint32_t`时，编译失败，并提示
  `sub with carry supports int32_t and uint32_t`；`sub_c`对应提示为
  `sub_c supports int32_t and uint32_t`。
- `carry_src`仅用于`sub_c`，且必须是`reg_tensor<bool>`；类型不匹配时找不到对应重载，编译失败。
- 用于多字整数分段减法时，未按照从低位到高位的顺序传递进位会得到错误的高位结果。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

同类Tensor API样例的Kernel、Host调用和数据验证流程请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/07_tensor_api/reg_vector_compute/basic_arithmetic/)。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void sub_with_carry_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    auto mask = asc::te::all_mask<uint32_t>();

    // 第0行保存低32位，第1行保存高32位。
    auto src0_low = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_low = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src0_high = input0.load(asc::te::make_coord(1, 0)).with_mask(mask);
    auto src1_high = input1.load(asc::te::make_coord(1, 0)).with_mask(mask);

    asc::te::reg_tensor<bool> carry0;
    auto low = asc::te::experimental::sub(carry0, src0_low, src1_low);

    asc::te::reg_tensor<bool> carry1;
    auto high = asc::te::experimental::sub_c(carry1, src0_high, src1_high, carry0);

    output.store(asc::te::make_coord(0, 0), low);
    output.store(asc::te::make_coord(1, 0), high);
}
```
