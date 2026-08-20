# operator<<

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

按照寄存器或标量给出的移位量，对 `src` 逐元素左移。

## 3. 函数原型

```cpp
namespace asc::te {

template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator<<(
    const reg_tensor<T>& src, const U& shift);

} // namespace asc::te
```

## 4. 参数说明

**表1 参数说明**

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src` | 输入 | 待左移的整数寄存器，同时提供执行 mask。 |
| `shift` | 输入 | 逐元素移位寄存器或 `int16_t` 标量；寄存器形式的 `shift.mask` 不参与执行 mask 的选择。 |

**支持的数据类型**

寄存器移位支持以下组合：

| `src` 元素类型 | `shift` 寄存器元素类型 |
| --- | --- |
| `int8_t`、`uint8_t` | `int8_t` |
| `int16_t`、`uint16_t` | `int16_t` |
| `int32_t`、`uint32_t` | `int32_t` |

标量移位支持上述全部 `src` 类型，`shift` 必须为 `int16_t`。

## 5. 返回值说明

返回 `reg_tensor<T>`：

- `reg`：有效 lane 保存逐元素左移结果，无效 lane 置零。
- `mask`：等于 `src.mask`；`shift` 为寄存器时，其 `mask` 不参与执行 mask 的选择。

## 6. 约束说明

- 本接口仅支持 `__NPU_ARCH__ == 3510`，并且只能在 AIV 的 `__simd_vf__` 函数中调用；
  底层调用 `asc_shiftleft` 或 `asc_shiftleft_scalar`。
- 作为执行 mask 的第一个寄存器操作数 `.mask` 必须在调用前初始化，通常使用
  `load(...).with_mask(m)`。`m` 可由 `update_mask<T>`、`make_mask` 或 `all_mask<T>` 生成；
  未初始化的 mask 内容不确定。
- 单个矢量寄存器宽度为 `asc_get_vf_len()` 字节；`T` 类型对应的最大 lane 数为
  `asc_get_vf_len() / sizeof(T)`。
- 底层 C API 使用 ZEROING 模式：执行 mask 无效的目的 lane 置零。返回对象仍携带相同
  `mask`，后续计算和存储应继续遵循该 mask。
- `src` 元素类型不在“支持的数据类型”列表中时，编译失败，并提示 `operator<< does not support this element type`。
- 寄存器移位量不是与 `src` 等位宽的有符号整数时，编译失败；标量移位量不是 `int16_t` 时，编译失败。
- 移位量为负数时行为未定义；移位量大于等于 `src` 元素类型位宽时，结果为 `0`。
- 左移结果超出 `T` 位宽的高位被丢弃，只保留低位结果。

## 7. 调用示例

以下示例展示接口在 AIV 计算函数中的典型调用方式。示例先为加载结果绑定 mask，再执行计算并将结果写回 UB。

完整的 Kernel、Host 调用和数据验证代码请参见[可运行样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/logical_compute_tensor_api/logical_compute_tensor_api.asc)。

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/arch/vector/experimental/reg_tensor.h"
#include "tensor_api/experimental/reg/logical_compute.h"

__simd_vf__ inline void shift_left_example(__ubuf__ int32_t* src, __ubuf__ int32_t* dst)
{
    auto layout =
        asc::te::make_frame_layout<asc::te::nd_ext_layout_ptn>(asc::te::_1 {}, asc::te::_64 {});
    auto input = asc::te::make_tensor(asc::te::make_mem_ptr(src), layout);
    auto output = asc::te::make_tensor(asc::te::make_mem_ptr(dst), layout);
    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<int32_t>());

    auto result = src_reg << static_cast<int16_t>(2);
    output.store(asc::te::make_coord(0, 0), result);
}
```
