# axpy

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/composite_compute.h"`。

逐元素将 `src` 与标量相乘，再与 `dst` 原值相加：

$$
result_i = scalar \times src_i + dst_i
$$

## 函数原型

```cpp
template <typename T, typename scalar_type>
__simd_callee__ inline decltype(auto) axpy(
    reg_tensor<T> dst,
    reg_tensor<T> src,
    scalar_type scalar);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `dst` | 输入 | 累加初值，同时提供结果 mask。 |
| `src` | 输入 | 与标量相乘的输入寄存器。 |
| `scalar` | 输入 | 标量乘数，类型必须与 `T` 完全一致。 |

## 数据类型

| `dst` | `src` | `scalar` | 返回值 |
| --- | --- | --- | --- |
| `reg_tensor<half>` | `reg_tensor<half>` | `half` | `reg_tensor<half>` |
| `reg_tensor<float>` | `reg_tensor<float>` | `float` | `reg_tensor<float>` |

## 返回值说明

返回计算后的 `reg_tensor<T>`，mask 与 `dst.mask` 一致。

## 约束说明

`dst`、`src` 和 `scalar` 的元素类型必须一致。

## 调用示例

```cpp
auto result = asc::te::axpy(dst_reg, src_reg, 0.25f);
```
