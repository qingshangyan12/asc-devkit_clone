# operator*

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

逐元素计算两个操作数的乘积，支持寄存器与寄存器、寄存器与标量、标量与寄存器三种组合。

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator*(const T& a, const U& b);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `a`、`b` | 输入 | 至少一个操作数必须是 `reg_tensor`；双寄存器形式要求元素类型一致。 |

## 数据类型

| 操作数形式 | 寄存器元素类型 |
| --- | --- |
| `reg * reg` | `int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float` |
| `reg * scalar`、`scalar * reg` | `int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float` |

## 返回值说明

返回与寄存器操作数元素类型相同的 `reg_tensor<T>`，mask 跟随寄存器输入。

## 约束说明

- 普通整型乘法仅保留目标元素类型能够表示的结果。
- `bfloat16_t` 当前仅支持双寄存器形式。

## 调用示例

```cpp
auto product0 = src0 * src1;
auto product1 = src0 * 2.0f;
auto product2 = 2.0f * src0;
```
