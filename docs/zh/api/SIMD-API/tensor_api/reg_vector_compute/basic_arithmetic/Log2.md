# log2

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

逐元素计算以 2 为底的对数：

$$
dst_i = \log_2(src_i)
$$

接口通过自然对数换底实现。

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline decltype(auto) log2(reg_tensor<T> src_reg);
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

- 接口不检查数学定义域。
- 结果包含自然对数计算和换底乘法产生的舍入误差。

## 调用示例

```cpp
auto dst_reg = asc::te::log2(src_reg);
```
