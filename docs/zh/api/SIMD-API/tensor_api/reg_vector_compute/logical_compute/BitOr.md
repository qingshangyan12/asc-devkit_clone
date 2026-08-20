# operator|

> **命名空间** `asc::te` · **头文件** `tensor_api/experimental/reg/logical_compute.h`

## 1. 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
- Atlas 200I/500 A2 推理产品：不支持
- Atlas 推理系列产品AI Core：不支持
- Atlas 推理系列产品Vector Core：不支持
- Atlas 训练系列产品：不支持

## 2. 功能说明

头文件路径：`"tensor_api/experimental/reg/logical_compute.h"`。

逐元素执行按位或运算，支持寄存器与寄存器、寄存器与标量、标量与寄存器三种形式。

## 3. 函数原型

```cpp
namespace asc::te {

template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator|(const T& a, const U& b);

} // namespace asc::te
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `a` | 输入 | 左操作数。非 `bool` 类型可以是寄存器或标量；为寄存器时优先使用 `a.mask`。 |
| `b` | 输入 | 右操作数。非 `bool` 类型可以是同类型寄存器或可转换标量；仅 `b` 为寄存器时使用 `b.mask`。 |

`bool` 类型要求 `a`、`b` 均为 `reg_tensor<bool>`：`reg` 保存 predicate 数据，执行 mask 取 `a.mask`。

**支持的数据类型**

寄存器元素类型支持 `bool`、`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float`。

## 5. 返回值说明

返回与寄存器操作数元素类型相同的 `reg_tensor`：

- `reg`：有效 lane 保存两个操作数逐 bit 或的结果。普通矢量类型的无效 lane 置零；
  `bool` 使用 predicate 指令。
- `mask`：包括 `reg_tensor<bool>` 在内，均等于第一个寄存器操作数的 `mask`。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，并且只能在 AIV 的 `__simd_vf__` 函数中调用；
  底层调用 `asc_or`。
- 作为执行 mask 的第一个寄存器操作数 `.mask` 必须在调用前初始化，通常使用
  `load(...).with_mask(m)`。`m` 可由 `update_mask<T>(remain)`、`make_mask<pattern, T>()` 或 `all_mask<T>()` 生成；
  未初始化的 mask 内容不确定。
- 单个矢量寄存器宽度为 `asc_get_vf_len()` 字节；`T` 类型对应的最大 lane 数为
  `asc_get_vf_len() / sizeof(T)`。
- 普通矢量类型使用 ZEROING 模式；`reg_tensor<bool>` 对应 predicate 指令没有 ZEROING
  模式参数。无论类型如何，后续计算和存储都应以返回对象的 `mask` 为准。
- 双寄存器元素类型不一致时，编译失败，并提示寄存器操作数必须具有相同的元素类型。
- 元素类型不在“支持的数据类型”列表中时，编译失败，并提示 `operator| does not support this element type`。
- `bool` 与标量组合时，编译失败，并提示 bool 按位运算需要两个 `reg_tensor<bool>` 操作数。
- 构造 `reg_tensor<bool>{maskValue}` 只初始化对象的 `mask`，不会生成 predicate 数据；作为操作数前，
  `reg` 必须已经由 mask 生成、加载或其他 predicate 计算接口写入。
- 对浮点类型执行的是底层二进制编码的按位或，不是数值逻辑或；结果可能为 `nan` 或其他特殊浮点值。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，
再执行计算并将结果写回 UB。

完整的 Kernel、Host 调用和数据验证代码请参见
[可运行样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/logical_compute_tensor_api/)。

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/arch/vector/experimental/reg_tensor.h"
#include "tensor_api/experimental/reg/logical_compute.h"

__simd_vf__ inline void bit_or_example(
    __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, __ubuf__ int32_t* dst)
{
    auto layout =
        asc::te::make_frame_layout<asc::te::nd_ext_layout_ptn>(asc::te::_1 {}, asc::te::_64 {});
    auto input0 = asc::te::make_tensor(asc::te::make_mem_ptr(src0), layout);
    auto input1 = asc::te::make_tensor(asc::te::make_mem_ptr(src1), layout);
    auto output = asc::te::make_tensor(asc::te::make_mem_ptr(dst), layout);
    auto mask = asc::te::all_mask<int32_t>();
    auto src0_reg = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
    auto src1_reg = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);

    auto result = src0_reg | src1_reg;
    output.store(asc::te::make_coord(0, 0), result);
}
```
