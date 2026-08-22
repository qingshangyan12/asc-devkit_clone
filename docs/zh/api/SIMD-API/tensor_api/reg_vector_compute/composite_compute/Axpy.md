# axpy

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

逐元素将 `src` 与标量相乘，再与 `dst` 原值相加：

$$
result_i = scalar \times src_i + dst_i
$$

## 3. 函数原型

```cpp
namespace asc::te::experimental {

template <typename T, typename scalar_type>
__simd_callee__ inline decltype(auto) axpy(
    const reg_tensor<T>& dst,
    const reg_tensor<T>& src,
    const scalar_type& scalar);

} // namespace asc::te::experimental
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `dst` | 输入 | 累加初值，同时提供结果 mask。 |
| `src` | 输入 | 与标量相乘的输入寄存器。 |
| `scalar` | 输入 | 标量乘数，类型必须与 `T` 完全一致。 |

**支持的数据类型**

| `dst` | `src` | `scalar` | 返回值 |
| --- | --- | --- | --- |
| `reg_tensor<half>` | `reg_tensor<half>` | `half` | `reg_tensor<half>` |
| `reg_tensor<float>` | `reg_tensor<float>` | `float` | `reg_tensor<float>` |

## 5. 返回值说明

返回 `reg_tensor<T>`：

- `reg`：有效 lane 保存 `dst.reg + scalar * src.reg`；底层 `asc_axpy` 采用 ZEROING 模式，无效 lane 置零。
- `mask`：等于 `dst.mask`；`src.mask` 不参与执行 mask 的选择。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，须在 AIV 的 SIMD 调用链中使用，通常由
  `__simd_vf__` 函数直接或间接调用；
  底层调用 `asc_axpy`。
- 作为执行 mask 的第一个寄存器操作数 `.mask` 必须在调用前初始化，通常使用
  `load(...).with_mask(m)`。`m` 可由 `update_mask<T>(remain)`、`make_mask<pattern, T>()` 或 `all_mask<T>()` 生成；
  未初始化的 mask 内容不确定。
- 单个矢量寄存器宽度为 `asc_get_vf_len()` 字节；`T` 类型对应的最大 lane 数为
  `asc_get_vf_len() / sizeof(T)`。
- 底层 C API 使用 ZEROING 模式：执行 mask 无效的目的 lane 置零。返回对象仍携带相同
  `mask`，后续计算和存储应继续遵循该 mask。
- `T` 不是 `half` 或 `float` 时，编译失败，并提示 `axpy supports half and float`。
- `dst` 与 `src` 的元素类型不一致时，找不到匹配的 `axpy` 重载，编译失败。
- `scalar` 的类型与 `T` 不完全一致时，编译失败，并提示标量与寄存器元素类型必须匹配；例如 `T` 为 `float`
  时应传入 `0.25f`，不能传入 `0.25`。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，
再执行计算并将结果写回 UB。

完整的 Kernel、Host 调用和数据验证代码请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/07_tensor_api/reg_vector_compute/composite_compute/)。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void axpy_example(
    InputTensor initial_input, InputTensor src_input, OutputTensor output)
{
    auto mask = asc::te::all_mask<float>();
    auto initial_reg = initial_input.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src_reg = src_input.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto result = asc::te::experimental::axpy(initial_reg, src_reg, 0.25f);
    output.store(asc::te::make_coord(0, 0), result);
}
```
