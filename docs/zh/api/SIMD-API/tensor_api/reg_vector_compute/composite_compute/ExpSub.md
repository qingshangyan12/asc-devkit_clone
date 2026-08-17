# exp_sub

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/composite_compute.h"`。

逐元素计算两个输入之差的自然指数：

$$
dst_i = e^{(src0_i - src1_i)}
$$

输入为 `half` 时，计算结果提升为 `float`。

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline decltype(auto) exp_sub(
    const reg_tensor<T>& src0,
    const reg_tensor<T>& src1);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `src0` | 输入 | 被减数，同时提供结果 mask。 |
| `src1` | 输入 | 减数。 |

## 数据类型

| `src0` | `src1` | 返回值 |
| --- | --- | --- |
| `reg_tensor<half>` | `reg_tensor<half>` | `reg_tensor<float>` |
| `reg_tensor<float>` | `reg_tensor<float>` | `reg_tensor<float>` |

## 返回值说明

返回 `reg_tensor<float>`，mask 与 `src0.mask` 一致。

## 约束说明

- 两个输入寄存器的元素类型必须一致。
- 输入和输出的寄存器排布由接口内部按照当前设备能力处理。
- 接口不检查指数计算的上溢或下溢。

## 调用示例

完整可运行样例请参考[复合计算Tensor API样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/composite_compute_tensor_api/README.md)。

```cpp
auto result = asc::te::exp_sub(src0, src1);
```
