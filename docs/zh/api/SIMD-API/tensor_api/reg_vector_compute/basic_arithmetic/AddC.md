# add_c

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

逐元素计算两个 32 位整数输入与进位输入之和，并同时返回新的进位：

$$
dst_i = src0_i + src1_i + carrySrc_i
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline decltype(auto) add_c(
    reg_tensor<bool>& carry,
    reg_tensor<T> src0,
    reg_tensor<T> src1,
    reg_tensor<bool> carry_src);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `carry` | 输出 | 逐元素进位输出。 |
| `src0`、`src1` | 输入 | 两个相同元素类型的 32 位整数加数。 |
| `carry_src` | 输入 | 逐元素进位输入。 |

## 数据类型

`T` 支持 `int32_t`、`uint32_t`；`carry` 和 `carry_src` 为 `reg_tensor<bool>`。

## 返回值说明

返回 `reg_tensor<T>`，表示加法低 32 位结果；mask 跟随 `src0.mask`。

## 约束说明

接口用于多字整数的分段加法，调用方负责按照从低位到高位的顺序传递进位。

## 调用示例

```cpp
asc::te::reg_tensor<bool> carry_out;
auto high = asc::te::add_c(carry_out, high0, high1, carry_in);
```
