# leaky_relu

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

使用标量斜率执行 Leaky ReLU：

$$
dst_i = \begin{cases}
src_i, & src_i \ge 0 \\
src_i \times slope, & src_i < 0
\end{cases}
$$

## 函数原型

```cpp
template <typename T, typename scalar_type>
__simd_callee__ inline decltype(auto) leaky_relu(
    const reg_tensor<T>& src_reg, const scalar_type& slope);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型。 |
| `scalar_type` | 模板参数 | 标量斜率类型，必须可转换为 `T`。 |
| `src_reg` | 输入 | 待计算的输入寄存器。 |
| `slope` | 输入 | 负数区间使用的标量斜率。 |

## 数据类型

`T` 支持 `half`、`float`。

## 返回值说明

返回 `reg_tensor<T>`，mask 与 `src_reg.mask` 一致。

## 约束说明

`slope` 必须是可转换为 `T` 的标量，不能是 `reg_tensor`。

## 调用示例

完整可运行样例请参考[基础计算Tensor API样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/basic_arithmetic_tensor_api/README.md)。

```cpp
auto dst_reg = asc::te::leaky_relu(src_reg, 0.1f);
```
