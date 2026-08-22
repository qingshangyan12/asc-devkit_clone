# exp_sub

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

逐元素计算两个输入之差的自然指数。输入为 `float` 时：

$$
dst_i = e^{(src0_i - src1_i)}
$$

输入为 `half` 时，底层按 EVEN 位置读取输入，并将计算结果提升为 `float`：

$$
dst_i = e^{(src0_{2i} - src1_{2i})}
$$

## 3. 函数原型

```cpp
namespace asc::te::experimental {

template <typename T>
__simd_callee__ inline decltype(auto) exp_sub(
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1);

} // namespace asc::te::experimental
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src0` | 输入 | 被减数，同时提供结果 mask。 |
| `src1` | 输入 | 减数。 |

**支持的数据类型**

| `src0` | `src1` | 返回值 |
| --- | --- | --- |
| `reg_tensor<half>` | `reg_tensor<half>` | `reg_tensor<float>` |
| `reg_tensor<float>` | `reg_tensor<float>` | `reg_tensor<float>` |

## 5. 返回值说明

返回 `reg_tensor<float>`：

- `reg`：输入为 `float` 时，有效 lane 保存 `exp(src0.reg - src1.reg)`；输入为 `half` 时，
  第 `i` 个 `float` 结果读取两个输入寄存器第 `2 * i` 个 `half` 元素，即偶数位置。
- `mask`：等于 `src0.mask`；`src1.mask` 不参与执行 mask 的选择。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，须在 AIV 的 SIMD 调用链中使用，通常由
  `__simd_vf__` 函数直接或间接调用。`float` 输入底层调用 `asc_exp_sub`；`half` 输入底层调用
  `asc_exp_sub_half2float(..., ASC_POSITION_EVEN)`。
- 作为执行 mask 的第一个寄存器操作数 `.mask` 必须在调用前初始化，通常使用
  `load(...).with_mask(m)`。`m` 可由 `update_mask<T>(remain)`、`make_mask<pattern, T>()` 或
  `all_mask<T>()` 生成；
  未初始化的 mask 内容不确定。
- 3510 架构的单个矢量寄存器宽度为 `asc_get_vf_len()` 字节，即 256 字节。输入为 `float` 时，
  最多处理 64 个连续元素；输入为 `half` 时，最多读取偶数位置 0、2、...、126 上的 64 个元素。
- 输入为 `half` 时，执行 mask 的第 `i` 位对应两个输入的第 `2 * i` 个 `half` 元素和
  第 `i` 个 `float` 结果。
- 两种底层 C API 均将执行 mask 无效位置的目的 lane 置零。返回对象仍携带 `src0.mask`，
  后续计算和存储应继续遵循该 mask。
- `T` 不是 `half` 或 `float` 时，编译失败，并提示 `exp_sub supports half and float`。
- 两个输入寄存器的元素类型不一致时，找不到匹配的 `exp_sub` 重载，编译失败。
- 指数结果超出 `float` 上限时返回 `+inf`，结果过小时可能下溢为 `0`。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，
再执行计算并将结果写回 UB。

同类 Tensor API 样例的 Kernel、Host 调用和数据验证流程请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/07_tensor_api/reg_vector_compute/composite_compute/)。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void exp_sub_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    auto mask = asc::te::all_mask<float>();
    auto src0_reg = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_reg = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto result = asc::te::experimental::exp_sub(src0_reg, src1_reg);
    output.store(asc::te::make_coord(0, 0), result);
}
```
