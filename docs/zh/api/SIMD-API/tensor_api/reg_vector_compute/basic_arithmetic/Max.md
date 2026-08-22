# max

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

逐元素返回两个操作数中的较大值，支持寄存器与寄存器、寄存器与标量、标量与寄存器三种组合。

## 3. 函数原型

```cpp
namespace asc::te::experimental {

template <typename T, typename U>
__simd_callee__ inline decltype(auto) max(const T& a, const U& b);

} // namespace asc::te::experimental
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `a` | 输入 | 左操作数，可以是 `reg_tensor` 或标量；`a` 为寄存器时优先使用 `a.mask`。 |
| `b` | 输入 | 右操作数，可以是同元素类型寄存器或可转换标量；仅 `b` 为寄存器时使用 `b.mask`。 |

**支持的数据类型**

寄存器元素类型支持 `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

## 5. 返回值说明

返回与寄存器操作数元素类型相同的 `reg_tensor`：

- `reg`：有效 lane 保存两个操作数中的较大值，无效 lane 置零。
- `mask`：等于第一个 `reg_tensor` 操作数的 `mask`。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，须在 AIV 的 SIMD 调用链中使用，通常由
  `__simd_vf__` 函数直接或间接调用；
  底层调用 `asc_max` 或 `asc_max_scalar`。
- 作为执行 mask 的第一个寄存器操作数 `.mask` 必须在调用前初始化，通常使用
  `load(...).with_mask(m)`。`m` 可由 `update_mask<T>(remain)`、`make_mask<pattern, T>()` 或 `all_mask<T>()` 生成；
  未初始化的 mask 内容不确定。
- 单个矢量寄存器宽度为 `asc_get_vf_len()` 字节；`T` 类型对应的最大 lane 数为
  `asc_get_vf_len() / sizeof(T)`。
- 底层 C API 使用 ZEROING 模式：执行 mask 无效的目的 lane 置零。返回对象仍携带相同
  `mask`，后续计算和存储应继续遵循该 mask。
- 双寄存器元素类型不一致时，编译失败，并提示寄存器操作数必须具有相同的元素类型。
- 标量不能转换为寄存器元素类型时，在标量转换处编译失败。
- 浮点输入分别为 `-0` 和 `+0` 时，返回 `+0`。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，
再执行计算并将结果写回 UB。

同类 Tensor API 样例的 Kernel、Host 调用和数据验证流程请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/07_tensor_api/reg_vector_compute/basic_arithmetic/)。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void max_example(
    InputTensor input0, InputTensor input1, OutputTensor output)
{
    auto mask = asc::te::all_mask<float>();
    auto src0_reg = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_reg = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto result = asc::te::experimental::max(src0_reg, src1_reg);
    output.store(asc::te::make_coord(0, 0), result);
}
```
