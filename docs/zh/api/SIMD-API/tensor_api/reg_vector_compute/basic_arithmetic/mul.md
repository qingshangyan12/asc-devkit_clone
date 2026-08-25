# operator*

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3训练系列产品/Atlas A3推理系列产品：不支持
- Atlas A2训练系列产品/Atlas A2推理系列产品：不支持
- Atlas 200I/500 A2推理产品：不支持
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
- Atlas训练系列产品：不支持

## 功能说明

逐元素执行乘法。支持寄存器与寄存器、寄存器与标量、标量与寄存器三种操作数组合。

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator*(const T& a, const U& b)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `a` | 输入 | 左操作数，可以是`reg_tensor`或标量。 |
| `b` | 输入 | 右操作数，可以是与寄存器元素类型相同的`reg_tensor`或可转换标量。 |

至少一个操作数必须为`reg_tensor`。两个寄存器操作数的元素类型必须一致。

## 支持的数据类型

| 操作数组合 | 寄存器元素类型 |
| --- | --- |
| 寄存器/寄存器 | `int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float` |
| 寄存器/标量、标量/寄存器 | `int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float` |

## 返回值说明

返回与寄存器操作数元素类型相同的`reg_tensor`。结果mask等于第一个寄存器操作数的mask。

## 约束说明

- 标量必须能够转换为寄存器元素类型。
- 整型结果只保留目标元素类型位宽内的数据。
- mask初始化和传导规则请参见[mask使用与传导](../reg_vector_compute.md#mask使用与传导)。

## 调用示例

```cpp
using asc::te::experimental::operator*;

auto mask = asc::te::all_mask<float>();
auto src = input.load(asc::te::make_coord(0, 0)).with_mask(mask);
auto result = src * 2.0f;
```
