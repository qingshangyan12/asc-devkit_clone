# operator+ / add

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

`operator+` 对两个操作数执行逐元素加法。操作数支持寄存器与寄存器、寄存器与标量、标量与寄存器三种组合。

`add` 用于 32 位整数寄存器加法，并通过引用参数额外返回每个元素的进位信息。

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator+(T a, U b);

template <typename T>
__simd_callee__ inline decltype(auto) add(
    reg_tensor<bool>& carry,
    reg_tensor<T> src0,
    reg_tensor<T> src1);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `a`、`b` | 输入 | 至少一个操作数必须是 `reg_tensor<T>`；另一个可以是相同元素类型的寄存器或可转换标量。 |
| `carry` | 输出 | `add` 返回的逐元素进位信息。 |
| `src0`、`src1` | 输入 | `add` 的两个同类型 32 位整数寄存器输入。 |

## 数据类型

`operator+` 的寄存器元素类型支持 `int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

`add` 支持 `int32_t`、`uint32_t`。

## 返回值说明

- `operator+` 返回与寄存器操作数元素类型相同的 `reg_tensor<T>`。
- `add` 返回加法低 32 位结果，并通过 `carry` 返回进位。
- 返回值 mask 跟随参与计算的寄存器操作数；双寄存器形式要求元素类型一致。

## 约束说明

- 至少一个操作数必须是 `reg_tensor`。
- 双寄存器形式的元素类型必须一致。
- 普通 `operator+` 不单独返回整数溢出信息。

## 调用示例

```cpp
auto sum0 = src0 + src1;
auto sum1 = src0 + 2.0f;

asc::te::reg_tensor<bool> carry;
auto sum2 = asc::te::add(carry, src_u32_0, src_u32_1);
```
