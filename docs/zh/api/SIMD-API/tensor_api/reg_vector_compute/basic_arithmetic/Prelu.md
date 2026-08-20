# prelu

> **命名空间** `asc::te` · **头文件** `tensor_api/experimental/reg/basic_arithmetic.h`

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

使用逐元素斜率执行 PReLU：

$$
dst_i = \begin{cases}
src_i, & src_i \ge 0 \\
src_i \times slope_i, & src_i < 0
\end{cases}
$$

## 3. 函数原型

```cpp
namespace asc::te {

template <typename T>
__simd_callee__ inline decltype(auto) prelu(
    const reg_tensor<T>& src_reg, const reg_tensor<T>& slope);

} // namespace asc::te
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入、斜率和返回值的元素类型。 |
| `src_reg` | 输入 | 待计算的输入寄存器，同时提供执行 mask。 |
| `slope` | 输入 | 负数区间使用的逐元素斜率寄存器；`slope.mask` 不参与执行 mask 的选择。 |

**支持的数据类型**

`T` 支持 `half`、`float`。

## 5. 返回值说明

返回 `reg_tensor<T>`：

- `reg`：有效 lane 在 `src_reg.reg >= 0` 时取原值，否则取 `src_reg.reg * slope.reg`。
- `mask`：等于 `src_reg.mask`；`slope.mask` 不参与本次执行 mask 的选择。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，并且只能在 AIV 的 `__simd_vf__` 函数中调用；
  底层调用 `asc_prelu`。
- 作为执行 mask 的第一个寄存器操作数 `.mask` 必须在调用前初始化，通常使用
  `load(...).with_mask(m)`。`m` 可由 `update_mask<T>`、`make_mask` 或 `all_mask<T>` 生成；
  未初始化的 mask 内容不确定。
- 单个矢量寄存器宽度为 `asc_get_vf_len()` 字节；`T` 类型对应的最大 lane 数为
  `asc_get_vf_len() / sizeof(T)`。
- 该底层 C API 未提供统一的 ZEROING 模式参数；不要依赖执行 mask 无效位置的数据，
  后续计算和存储应以返回对象的 `mask` 为准。
- `T` 不是 `half` 或 `float` 时，编译失败，并提示 `prelu supports half and float`。
- `src_reg` 和 `slope` 的元素类型不一致时，找不到匹配的 `prelu` 重载，编译失败。
- 仅 `src_reg.mask` 中的有效元素参与计算；`slope.mask` 不改变结果 mask。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，再执行计算并将结果写回 UB。

完整的 Kernel、Host 调用和数据验证代码请参见[可运行样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/basic_arithmetic_tensor_api/basic_arithmetic_tensor_api.asc)。

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/arch/vector/experimental/reg_tensor.h"
#include "tensor_api/experimental/reg/basic_arithmetic.h"

__simd_vf__ inline void prelu_example(
    __ubuf__ float* src, __ubuf__ float* slope, __ubuf__ float* dst)
{
    auto layout =
        asc::te::make_frame_layout<asc::te::nd_ext_layout_ptn>(asc::te::_1 {}, asc::te::_64 {});
    auto input = asc::te::make_tensor(asc::te::make_mem_ptr(src), layout);
    auto slope_input = asc::te::make_tensor(asc::te::make_mem_ptr(slope), layout);
    auto output = asc::te::make_tensor(asc::te::make_mem_ptr(dst), layout);
    auto mask = asc::te::all_mask<float>();
    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto slope_reg = slope_input.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto dst_reg = asc::te::prelu(src_reg, slope_reg);
    output.store(asc::te::make_coord(0, 0), dst_reg);
}
```
