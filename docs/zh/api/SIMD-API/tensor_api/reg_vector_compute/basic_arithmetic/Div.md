# operator/

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

逐元素执行除法，支持寄存器与寄存器、寄存器与标量、标量与寄存器三种组合。

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator/(const T& a, const U& b);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `a` | 输入 | 被除数。 |
| `b` | 输入 | 除数。 |

至少一个操作数必须为 `reg_tensor`；双寄存器形式要求元素类型一致。

## 数据类型

寄存器元素类型支持 `int16_t`、`uint16_t`、`half`、`int32_t`、`uint32_t`、`float`。

## 返回值说明

返回与寄存器操作数元素类型相同的 `reg_tensor<T>`，mask 跟随寄存器输入。

## 约束说明

- 接口不检查除数是否为零。
- 标量位于左侧时会先广播，再执行 `scalar / reg`，操作数顺序不会交换。

## 调用示例

```cpp
auto quotient0 = src0 / src1;
auto quotient1 = src0 / 2.0f;
auto quotient2 = 2.0f / src0;
```
