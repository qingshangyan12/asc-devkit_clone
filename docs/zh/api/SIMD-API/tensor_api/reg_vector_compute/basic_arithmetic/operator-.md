# operator-

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

二元`operator-`逐元素执行不带借位输出的减法，支持寄存器与寄存器、寄存器与标量、标量与寄存器三种操作数组合。

$$
dst_i = a_i - b_i
$$

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator-(const T& a, const U& b)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T`、`U` | 模板参数 | 至少一个为`reg_tensor`类型。寄存器元素类型支持`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`；标量须可转换为寄存器元素类型。 |
| `a` | 输入 | 被减数，可以是`reg_tensor`或标量；`a`为寄存器时优先使用`a.mask`。 |
| `b` | 输入 | 减数，可以是同元素类型寄存器或可转换标量；仅`b`为寄存器时使用`b.mask`。 |

## 返回值说明

返回与寄存器操作数元素类型相同的`reg_tensor`。结果mask等于第一个寄存器操作数的mask。

## 约束说明

- 整型结果超出元素类型表示范围时采用非饱和截断。
- 标量位于左侧时执行`scalar - reg`，操作数顺序不会交换。

## 调用示例

```cpp
using asc::te::experimental::operator-;

auto mask = asc::te::all_mask<uint32_t>();
auto src0 = input0.load(asc::te::make_coord(0, 0)).with_mask(mask);
auto src1 = input1.load(asc::te::make_coord(0, 0)).with_mask(mask);
auto result = src0 - src1;
```
