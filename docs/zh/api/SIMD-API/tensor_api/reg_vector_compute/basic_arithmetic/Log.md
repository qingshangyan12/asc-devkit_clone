# log

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

逐元素计算自然对数：

$$
dst_i = \ln(src_i)
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline decltype(auto) log(const reg_tensor<T>& src_reg);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型。 |
| `src_reg` | 输入 | 待计算的 `reg_tensor<T>`。 |

## 数据类型

`T` 支持 `half`、`float`。

## 返回值说明

返回 `reg_tensor<T>`，mask 与 `src_reg.mask` 一致。

## 约束说明

- 接口不检查数学定义域，调用方应保证有效元素满足输入要求。
- 当前接口直接复用 `asc_ln`，不提供独立的 Subnormal 算法模式参数。

## 调用示例

完整可运行样例请参考[基础计算Tensor API样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/basic_arithmetic_tensor_api/README.md)。

```cpp
auto src_reg = input.load(MakeCoord(0, 0)).with_mask(asc::te::ones<float>());
auto dst_reg = asc::te::log(src_reg);
```
