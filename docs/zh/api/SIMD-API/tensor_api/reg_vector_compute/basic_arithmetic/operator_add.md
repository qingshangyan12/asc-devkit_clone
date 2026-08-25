# operator+

## 产品支持情况

- Ascend 950PR/Ascend 950DT：支持
- Atlas A3训练系列产品/Atlas A3推理系列产品：不支持
- Atlas A2训练系列产品/Atlas A2推理系列产品：不支持
- Atlas 200I/500 A2推理产品：不支持
- Atlas推理系列产品AI Core：不支持
- Atlas推理系列产品Vector Core：不支持
- Atlas训练系列产品：不支持

## 功能说明

头文件路径：`"tensor_api/tensor.h"`。

命名空间：`asc::te::experimental`。

逐元素执行加法。支持寄存器与寄存器、寄存器与标量、标量与寄存器三种操作数组合。

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1)

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(
    const reg_tensor<T>& src, const T& scalar)

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(
    const T& scalar, const reg_tensor<T>& src)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 模板参数 | 输入和返回值的元素类型，支持`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`。 |
| `src0`、`src1` | 输入 | 寄存器/寄存器形式的两个加数，类型均为`reg_tensor<T>`。 |
| `src` | 输入 | 标量形式中的寄存器加数，类型为`reg_tensor<T>`。 |
| `scalar` | 输入 | 标量加数，类型为`T`。 |

## 返回值说明

返回与寄存器操作数元素类型相同的`reg_tensor`。结果mask等于第一个寄存器操作数的mask。

## 约束说明

- 标量类型必须与寄存器元素类型一致。
- 整型结果超出元素类型表示范围时采用非饱和截断。

## 调用示例

```cpp
using asc::te::experimental::operator+;

auto mask = asc::te::all_mask<float>();
auto src = input.load(asc::te::make_coord(0, 0)).with_mask(mask);
auto result = src + 1.0f;
```
