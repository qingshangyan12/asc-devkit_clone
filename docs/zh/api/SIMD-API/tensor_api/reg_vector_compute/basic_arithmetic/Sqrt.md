# sqrt

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

逐元素计算平方根：

$$
dst_i = \sqrt{src_i}
$$

## 3. 函数原型

```cpp
namespace asc {
namespace te {
namespace experimental {

template <typename T>
__simd_callee__ inline decltype(auto) sqrt(const reg_tensor<T>& src_reg);

} // namespace experimental
} // namespace te
} // namespace asc
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型。 |
| `src_reg` | 输入 | 待计算的 `reg_tensor<T>`。 |

**支持的数据类型**

`T` 支持 `half`、`float`。

## 5. 返回值说明

返回 `reg_tensor<T>`：

- `reg`：有效 lane 为 `sqrt(src_reg.reg)`；底层 `asc_sqrt` 采用 ZEROING 模式，无效 lane 置零。
- `mask`：等于 `src_reg.mask`。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，须在 AIV 的 SIMD 调用链中使用，通常由
  `__simd_vf__` 函数直接或间接调用；
  底层调用 `asc_sqrt`。
- 作为执行 mask 的第一个寄存器操作数 `.mask` 必须在调用前初始化，通常使用
  `load(...).with_mask(m)`。`m` 可由 `update_mask<T>(remain)`、`make_mask<pattern, T>()` 或 `all_mask<T>()` 生成；
  未初始化的 mask 内容不确定。
- 单个矢量寄存器宽度为 `asc_get_vf_len()` 字节；`T` 类型对应的最大 lane 数为
  `asc_get_vf_len() / sizeof(T)`。
- 底层 C API 使用 ZEROING 模式：执行 mask 无效的目的 lane 置零。返回对象仍携带相同
  `mask`，后续计算和存储应继续遵循该 mask。
- `T` 不是 `half` 或 `float` 时，编译失败，并提示 `sqrt supports half and float`。
- 有效输入元素为负数时返回 `nan`；输入为 `-0` 时返回 `-0`。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，
再执行计算并将结果写回 UB。

同类 Tensor API 样例的 Kernel、Host 调用和数据验证流程请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/07_tensor_api/reg_vector_compute/basic_arithmetic/)。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void sqrt_example(InputTensor input, OutputTensor output)
{
    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<float>());

    auto dst_reg = asc::te::experimental::sqrt(src_reg);
    output.store(asc::te::make_coord(0, 0), dst_reg);
}
```
