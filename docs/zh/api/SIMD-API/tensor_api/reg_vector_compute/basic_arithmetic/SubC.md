# sub_c

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/basic_arithmetic.h"`。

逐元素执行带进位输入的 32 位整数补码减法，并返回新的进位信息。

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline decltype(auto) sub_c(
    reg_tensor<bool>& carry,
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1,
    const reg_tensor<bool>& carry_src);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `carry` | 输出 | 逐元素进位输出。 |
| `src0` | 输入 | 被减数，类型为 `reg_tensor<T>`。 |
| `src1` | 输入 | 减数，类型为与 `src0` 相同的 `reg_tensor<T>`。 |
| `carry_src` | 输入 | 逐元素进位输入。 |

## 数据类型

`T` 支持 `int32_t`、`uint32_t`；进位输入和输出为 `reg_tensor<bool>`。

## 返回值说明

返回 `reg_tensor<T>`，表示减法低 32 位结果；mask 跟随 `src0.mask`。

## 约束说明

接口用于多字整数的分段减法，调用方负责按照从低位到高位的顺序传递进位。

## 调用示例

完整可运行样例请参考[基础计算Tensor API样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/basic_arithmetic_tensor_api/README.md)。

```cpp
asc::te::reg_tensor<bool> carry_out;
auto high = asc::te::sub_c(carry_out, high0, high1, carry_in);
```
