# abs

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

对 `src_reg` 中 mask 有效的元素逐个取绝对值，并返回计算结果：

$$
dst_i = |src_i|
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline decltype(auto) abs(const reg_tensor<T>& src_reg);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型。 |
| `src_reg` | 输入 | 待计算的 `reg_tensor<T>`，同时携带有效元素 mask。 |

## 数据类型

`T` 支持 `int8_t`、`int16_t`、`half`、`int32_t`、`float`。

## 返回值说明

返回 `reg_tensor<T>`。返回值 mask 与 `src_reg.mask` 一致。

## 约束说明

- 接口不会扩展整数结果的类型。对有符号整数的最小值取绝对值时，数学结果超出原类型的表示范围，返回值仍为该最小值。例如，`abs(int8_t(-128))` 返回 `int8_t(-128)`。

## 调用示例

```cpp
auto src_reg = input.load(make_coord(0, 0)).with_mask(asc::te::all_mask<float>());
auto dst_reg = asc::te::abs(src_reg);
output.store(make_coord(0, 0), dst_reg);
```
