# operator!

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

对普通元素执行逻辑非：输入为 0 时结果为 1，输入非 0 时结果为 0。

$$
dst_i = (src_i == 0) ? 1 : 0
$$

对 `reg_tensor<bool>` 执行逐 bit 布尔取反。

## 3. 函数原型

```cpp
namespace asc::te {

template <typename T>
__simd_callee__ inline decltype(auto) operator!(const reg_tensor<T>& src);

} // namespace asc::te
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型。 |
| `src` | 输入 | 待执行逻辑非的寄存器。`reg` 保存输入数据，`mask` 决定有效 lane。 |

**支持的数据类型**

`T` 支持 `bool`、`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

## 5. 返回值说明

返回 `reg_tensor<T>`：

- `T` 非 `bool` 时，`reg` 的有效 lane 在输入为 `0` 时取同类型的 `1`，否则取 `0`。
- `T` 为 `bool` 时，`reg` 中每个有效 predicate bit 为输入对应 bit 的反值。
- `mask`：等于 `src.mask`。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，并且只能在 AIV 的 `__simd_vf__` 函数中调用；
  底层调用 `asc_not`、`asc_eq_scalar` 和
  `asc_duplicate_scalar`。
- 作为执行 mask 的第一个寄存器操作数 `.mask` 必须在调用前初始化，通常使用
  `load(...).with_mask(m)`。`m` 可由 `update_mask<T>`、`make_mask` 或 `all_mask<T>` 生成；
  未初始化的 mask 内容不确定。
- 单个矢量寄存器宽度为 `asc_get_vf_len()` 字节；`T` 类型对应的最大 lane 数为
  `asc_get_vf_len() / sizeof(T)`。
- 普通矢量类型使用 ZEROING 模式；`reg_tensor<bool>` 对应 predicate 指令没有 ZEROING
  模式参数。无论类型如何，后续计算和存储都应以返回对象的 `mask` 为准。
- `T` 不在“支持的数据类型”列表中时，编译失败，并提示 `operator! does not support this element type`。
- 本接口是 C++ 逻辑非语义，不等价于 Basic API 对数值数据执行的按位 `Not`；例如输入整数 `2`
  时结果为 `0`，而不是 `~2`。
- `T` 为 `bool` 时，`src.reg` 必须已经包含 predicate 数据，`src.mask` 必须已经绑定有效 lane。
  构造 `reg_tensor<bool>{maskValue}` 只初始化 `mask`，不会写入 `reg`。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，再执行计算并将结果写回 UB。

完整的 Kernel、Host 调用和数据验证代码请参见[可运行样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/logical_compute_tensor_api/logical_compute_tensor_api.asc)。

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/arch/vector/experimental/reg_tensor.h"
#include "tensor_api/experimental/reg/logical_compute.h"

__simd_vf__ inline void logical_not_example(__ubuf__ int32_t* src, __ubuf__ int32_t* dst)
{
    auto layout =
        asc::te::make_frame_layout<asc::te::nd_ext_layout_ptn>(asc::te::_1 {}, asc::te::_64 {});
    auto input = asc::te::make_tensor(asc::te::make_mem_ptr(src), layout);
    auto output = asc::te::make_tensor(asc::te::make_mem_ptr(dst), layout);
    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<int32_t>());

    auto is_zero = !src_reg;
    output.store(asc::te::make_coord(0, 0), is_zero);
}
```
