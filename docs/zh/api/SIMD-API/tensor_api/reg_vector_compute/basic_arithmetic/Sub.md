# operator- / sub

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

二元 `operator-` 执行逐元素减法，支持寄存器与寄存器、寄存器与标量、标量与寄存器三种组合。

`sub` 用于 32 位整数寄存器减法，并通过引用参数额外返回补码减法的进位信息。

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator-(const T& a, const U& b);

template <typename T>
__simd_callee__ inline decltype(auto) sub(
    reg_tensor<bool>& carry,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `a` | 输入 | 被减数，必须与 `b` 中至少一个为 `reg_tensor`。 |
| `b` | 输入 | 减数，可以是相同元素类型的寄存器或可转换标量。 |
| `carry` | 输出 | `sub` 返回的逐元素进位信息。 |
| `src0`、`src1` | 输入 | `sub` 的两个同类型 32 位整数寄存器输入。 |

## 数据类型

二元 `operator-` 支持 `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

`sub` 支持 `int32_t`、`uint32_t`。

## 返回值说明

返回与寄存器操作数元素类型相同的 `reg_tensor<T>`。`sub` 另通过 `carry` 返回进位；结果 mask 跟随参与计算的寄存器输入。

## 约束说明

- 双寄存器形式的元素类型必须一致。
- 标量位于左侧时会先广播，再执行 `scalar - reg`，操作数顺序不会交换。

## 调用示例

```cpp
auto diff0 = src0 - src1;
auto diff1 = src0 - 1.0f;
auto diff2 = 1.0f - src0;

asc::te::reg_tensor<bool> carry;
auto diff3 = asc::te::sub(carry, src_u32_0, src_u32_1);
```
