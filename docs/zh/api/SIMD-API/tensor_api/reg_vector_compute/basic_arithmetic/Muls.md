# operator*（寄存器与标量）

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

逐元素计算寄存器与标量的乘积，支持寄存器与标量、标量与寄存器两种操作数顺序：

$$
dst_i = src_i \times scalar
$$

Tensor API 的公开接口为 `operator*`，该标量重载底层调用 `asc_mul_scalar`。它与
`muls_cast` 不同：`muls_cast` 调用 `asc_muls`，同时完成 `float` 到 `half` 的转换。

## 3. 函数原型

```cpp
namespace asc {
namespace te {
namespace experimental {

template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator*(const T& a, const U& b);

} // namespace experimental
} // namespace te
} // namespace asc
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `a`、`b` | 输入 | `reg_tensor` 与可转换标量；寄存器操作数提供执行 mask。 |

**支持的数据类型**

以下表格中的 `T` 表示寄存器元素类型。

| `T` | 标量类型 | 返回值 |
| --- | --- | --- |
| `int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float` | 可转换为 `T` 的标量 | `reg_tensor<T>` |

## 5. 返回值说明

返回与寄存器操作数元素类型相同的 `reg_tensor<T>`：

- `reg`：有效 lane 保存寄存器对应元素与标量的乘积；无效 lane 置零。
- `mask`：等于寄存器操作数的 `mask`。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，须在 AIV 的 SIMD 调用链中使用，通常由
  `__simd_vf__` 函数直接或间接调用；底层调用 `asc_mul_scalar`。非 AIV 分支不执行矢量计算，
  不应使用其返回结果。
- 寄存器操作数的 `.mask` 必须在调用前初始化，通常使用 `load(...).with_mask(m)`。`m` 可由
  `update_mask<T>(remain)`、`make_mask<pattern, T>()` 或 `all_mask<T>()` 生成；未初始化的
  mask 内容不确定。
- 3510 架构的单个矢量寄存器宽度为 `asc_get_vf_len()` 字节，即 256 字节。`T` 类型对应的
  最大 lane 数为 `asc_get_vf_len() / sizeof(T)`。
- 底层 `asc_mul_scalar` 使用 ZEROING 模式，执行 mask 无效的目的 lane 置零。返回对象的
  `mask` 与寄存器操作数的 `mask` 相同，后续计算和存储继续使用该 mask。
- 标量不能转换为寄存器元素类型时，在标量转换处编译失败。
- 寄存器元素类型为 `bfloat16_t` 时，编译失败，并提示
  `scalar operator* does not support this element type`。
- 整型乘积超出元素类型表示范围时采用非饱和截断，仅保留目标元素类型位宽内的结果。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，
再执行计算并将结果写回 UB。

同类 Tensor API 样例的 Kernel、Host 调用和数据验证流程请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/07_tensor_api/reg_vector_compute/basic_arithmetic/)。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void mul_scalar_example(InputTensor input, OutputTensor output)
{
    using asc::te::experimental::operator*;

    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<float>());

    auto product = src_reg * 2.0f;
    output.store(asc::te::make_coord(0, 0), product);
}
```
