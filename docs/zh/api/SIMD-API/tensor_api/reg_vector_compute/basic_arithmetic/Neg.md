# operator-（一元）

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

对输入寄存器执行逐元素取负：

$$
dst_i = -src_i
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline decltype(auto) operator-(const reg_tensor<T>& src_reg);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型。 |
| `src_reg` | 输入 | 待取负的 `reg_tensor<T>`。 |

## 数据类型

`T` 支持 `int8_t`、`int16_t`、`half`、`int32_t`、`float`。

## 返回值说明

返回 `reg_tensor<T>`，mask 与 `src_reg.mask` 一致。

## 约束说明

接口不会扩展整数结果的类型。对有符号整数的最小值取负时，数学结果超出原类型的表示范围，返回值仍为该最小值。例如，`-int8_t(-128)` 的结果为 `int8_t(-128)`。

## 调用示例

```cpp
auto dst_reg = -src_reg;
```
