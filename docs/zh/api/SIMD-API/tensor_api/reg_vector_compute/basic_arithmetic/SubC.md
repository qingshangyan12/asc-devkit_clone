# sub / sub_c

> **命名空间** `asc::te::experimental` · **头文件** `tensor_api/tensor.h`

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

头文件路径：`"tensor_api/tensor.h"`。

`sub` 对两个 32 位整数寄存器执行逐元素减法，返回低 32 位结果，并通过引用参数输出补码减法的
进位；`sub_c` 使用前一段产生的进位继续计算，并输出新的进位。二者用于构成从低位到高位的
多字整数减法链：

$$
\begin{aligned}
sub:\quad dst_i &= src0_i - src1_i \\
sub\_c:\quad dst_i &= src0_i + \mathord{\sim}src1_i + carrySrc_i
\end{aligned}
$$

## 3. 函数原型

```cpp
namespace asc::te::experimental {

template <typename T>
__simd_callee__ inline decltype(auto) sub(
    reg_tensor<bool>& carry,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1);

template <typename T>
__simd_callee__ inline decltype(auto) sub_c(
    reg_tensor<bool>& carry,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1,
    const reg_tensor<bool>& carry_src);

} // namespace asc::te::experimental
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `carry` | 输出 | `sub` 和 `sub_c` 的逐元素进位输出。`reg` 保存进位，`mask` 由接口写为 `src0.mask`。 |
| `src0` | 输入 | 被减数，类型为 `reg_tensor<T>`，同时提供执行 mask。 |
| `src1` | 输入 | 减数，类型为与 `src0` 相同的 `reg_tensor<T>`；`src1.mask` 不参与执行 mask 的选择。 |
| `carry_src` | 输入 | 仅用于 `sub_c`。逐元素进位输入，读取其 `reg`；`carry_src.mask` 不参与执行 mask 的选择。 |

**支持的数据类型**

`sub` 和 `sub_c` 的 `T` 均支持 `int32_t`、`uint32_t`；进位输入和输出为 `reg_tensor<bool>`。

## 5. 返回值说明

`sub` 和 `sub_c` 均返回 `reg_tensor<T>`，并通过 `carry` 返回借位状态：

- 返回值 `reg`：`sub` 保存 `src0.reg - src1.reg` 的低 32 位；`sub_c` 保存
  `src0.reg + ~src1.reg + carry_src.reg` 的低 32 位。
- 返回值 `mask`：等于 `src0.mask`。
- `carry.reg`：`1` 表示未借位，`0` 表示发生借位；`carry.mask` 等于 `src0.mask`。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，须在 AIV 的 SIMD 调用链中使用，通常由
  `__simd_vf__` 函数直接或间接调用；
  `sub` 底层调用带进位输出的 `asc_sub`，`sub_c` 底层调用 `asc_subc`。
- 作为执行 mask 的第一个寄存器操作数 `.mask` 必须在调用前初始化，通常使用
  `load(...).with_mask(m)`。`m` 可由 `update_mask<T>(remain)`、`make_mask<pattern, T>()` 或 `all_mask<T>()` 生成；
  未初始化的 mask 内容不确定。
- 单个矢量寄存器宽度为 `asc_get_vf_len()` 字节；`T` 类型对应的最大 lane 数为
  `asc_get_vf_len() / sizeof(T)`。
- 两个底层 C API 均将执行 mask 无效位置的低 32 位结果和进位输出置零。返回值与进位对象
  仍携带 `src0.mask`，后续计算和存储应继续遵循该 mask。
- `sub` 的 `T` 不是 `int32_t` 或 `uint32_t` 时，编译失败，并提示
  `sub with carry supports int32_t and uint32_t`；`sub_c` 对应提示为
  `sub_c supports int32_t and uint32_t`。
- `carry_src` 仅用于 `sub_c`，且必须是 `reg_tensor<bool>`；类型不匹配时找不到对应重载，编译失败。
- 用于多字整数分段减法时，未按照从低位到高位的顺序传递进位会得到错误的高位结果。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，
再执行计算并将结果写回 UB。

同类 Tensor API 样例的 Kernel、Host 调用和数据验证流程请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/07_tensor_api/reg_vector_compute/basic_arithmetic/)。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void sub_with_carry_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    auto mask = asc::te::all_mask<uint32_t>();

    // 第 0 行保存低 32 位，第 1 行保存高 32 位。
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
