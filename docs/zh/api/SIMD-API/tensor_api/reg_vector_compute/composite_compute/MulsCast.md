# muls_cast

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/composite_compute.h"`。

将 `float` 输入与标量逐元素相乘，再将结果转换为 `half`：

$$
dst_i = cast\_to\_half(src_i \times scalar)
$$

## 函数原型

```cpp
__simd_callee__ inline decltype(auto) muls_cast(
    const reg_tensor<float>& src,
    const float& scalar);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src` | 输入 | `reg_tensor<float>` 输入。 |
| `scalar` | 输入 | `float` 标量乘数。 |

## 数据类型

输入为 `reg_tensor<float>` 和 `float` 标量，返回 `reg_tensor<half>`。

## 返回值说明

返回乘法和类型转换后的 `reg_tensor<half>`，mask 与 `src.mask` 一致。

## 约束说明

- 输入和输出的寄存器排布由接口内部按照当前设备能力处理。
- 类型转换和舍入行为遵循底层 `asc_muls`。

## 调用示例

完整可运行样例请参考[复合计算Tensor API样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/composite_compute_tensor_api/README.md)。

```cpp
auto result = asc::te::muls_cast(src_float, 1.5f);
```
