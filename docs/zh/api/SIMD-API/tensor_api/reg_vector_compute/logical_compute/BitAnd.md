# operator&

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持。
- 其他产品：不支持。

## 功能说明

头文件路径：`"tensor_api/experimental/reg/logical_compute.h"`。

逐元素执行按位与运算，支持寄存器与寄存器、寄存器与标量、标量与寄存器三种形式。

## 函数原型

```cpp
template <typename T, typename U>
__simd_callee__ inline decltype(auto) operator&(const T& a, const U& b);
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `a`、`b` | 输入 | 按位与操作数，至少一个必须是 `reg_tensor`。 |

## 数据类型

寄存器元素类型支持 `bool`、`int8_t`、`uint8_t`、`fp8_e5m2_t`、`fp8_e4m3fn_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。

## 返回值说明

返回与寄存器操作数元素类型相同的 `reg_tensor`。普通类型的结果 mask 跟随寄存器输入。

## 约束说明

- 双寄存器形式的元素类型必须一致。
- `bool` 仅支持两个 `reg_tensor<bool>` 操作数，不支持 bool 标量组合。

## 调用示例

完整可运行样例请参考[逻辑计算Tensor API样例](../../../../../../../examples/01_simd_cpp_api/02_features/03_basic_api/02_vector_compute/logical_compute_tensor_api/README.md)。

```cpp
auto result0 = src0 & src1;
auto result1 = src0 & 0xff;
```
