# prelu

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

使用逐元素斜率执行 PReLU：

$$
dst_i = \begin{cases}
src_i, & src_i \ge 0 \\
src_i \times slope_i, & src_i < 0
\end{cases}
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline decltype(auto) prelu(
    const reg_tensor<T>& src_reg, const reg_tensor<T>& slope);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入、斜率和返回值的元素类型。 |
| `src_reg` | 输入 | 待计算的输入寄存器。 |
| `slope` | 输入 | 负数区间使用的逐元素斜率寄存器。 |

## 数据类型

`T` 支持 `half`、`float`。

## 返回值说明

返回 `reg_tensor<T>`，mask 与 `src_reg.mask` 一致。

## 约束说明

- `src_reg` 和 `slope` 的元素类型必须一致。
- 计算有效范围由 `src_reg.mask` 决定。

## 调用示例

```cpp
auto dst_reg = asc::te::prelu(src_reg, slope_reg);
```
