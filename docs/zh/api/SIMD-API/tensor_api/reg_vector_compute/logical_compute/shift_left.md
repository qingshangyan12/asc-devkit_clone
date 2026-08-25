# operator<<

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3训练系列产品/Atlas A3推理系列产品：不支持
- Atlas A2训练系列产品/Atlas A2推理系列产品：不支持
- Atlas 200I/500 A2推理产品：不支持
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
- Atlas训练系列产品：不支持

## 功能说明

头文件路径：`"tensor_api/tensor.h"`。

命名空间：`asc::te::experimental`。

按照寄存器或标量给出的移位量，对`src`逐元素左移。

$$
dst_i = src_i \ll shift_i
$$

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline reg_tensor<T> operator<<(
    const reg_tensor<T>& src, const U& shift)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src` | 输入 | 待左移的源操作数。`src.mask`用于标记参与计算的元素，对应位置为`1`时参与计算，为`0`时不参与计算。 |
| `shift` | 输入 | 逐元素移位寄存器或`int16_t`标量。寄存器形式的`shift.mask`不用于本次计算。 |

## 支持的数据类型

| 移位形式 | `src`元素类型 | `shift`类型 |
| --- | --- | --- |
| 寄存器移位 | `int8_t`、`uint8_t` | `reg_tensor<int8_t>` |
| 寄存器移位 | `int16_t`、`uint16_t` | `reg_tensor<int16_t>` |
| 寄存器移位 | `int32_t`、`uint32_t` | `reg_tensor<int32_t>` |
| 标量移位 | `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`int32_t`、`uint32_t` | `int16_t` |

## 返回值说明

返回`reg_tensor<T>`。`reg`保存逐元素左移结果，`src.mask`对应位置为`0`时，`reg`的对应位置置零。返回值的`mask`与`src.mask`相同。

## 约束说明

- 移位量为负数时行为未定义。移位量大于等于`src`元素类型位宽时，结果为`0`。
- 左移结果超出`T`位宽的高位被丢弃，只保留低位结果。

## 调用示例

以下示例展示接口在AIV计算函数中的典型调用方式。示例先为加载结果绑定mask，
再执行计算并将结果写回UB。

```cpp
#include "tensor_api/tensor.h"

template <typename InputTensor, typename OutputTensor>
__simd_vf__ inline void shift_left_example(InputTensor input, OutputTensor output)
{
    using asc::te::experimental::operator<<;

    auto src_reg = input.load(asc::te::make_coord(0, 0)).with_mask(asc::te::all_mask<int32_t>());

    auto result = src_reg << static_cast<int16_t>(2);
    output.store(asc::te::make_coord(0, 0), result);
}
```
