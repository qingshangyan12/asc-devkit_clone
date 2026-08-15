# absdiff

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/composite_compute.h"`。

逐元素计算两个输入之差的绝对值：

$$
dst_i = |src0_i - src1_i|
$$

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline decltype(auto) absdiff(
    reg_tensor<T> src0,
    reg_tensor<T> src1);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src0` | 输入 | 被减数，同时提供结果 mask。 |
| `src1` | 输入 | 减数。 |

## 数据类型

`T` 支持 `half`、`float`，输入和返回值类型一致。

## 返回值说明

返回 `reg_tensor<T>`，mask 与 `src0.mask` 一致。

## 约束说明

两个输入寄存器的元素类型必须一致。

## 调用示例

```cpp
auto result = asc::te::absdiff(src0, src1);
```
