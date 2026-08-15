# relu

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

逐元素执行 ReLU：

$$
dst_i = \max(src_i, 0)
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline decltype(auto) relu(reg_tensor<T> src_reg);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型。 |
| `src_reg` | 输入 | 待计算的 `reg_tensor<T>`。 |

## 数据类型

`T` 支持 `half`、`int32_t`、`float`。

## 返回值说明

返回 `reg_tensor<T>`，mask 与 `src_reg.mask` 一致。

## 约束说明

不支持的元素类型会在编译期报错。

## 调用示例

```cpp
auto dst_reg = asc::te::relu(src_reg);
```
