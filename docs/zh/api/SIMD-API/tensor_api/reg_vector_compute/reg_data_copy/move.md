# move

## 产品支持情况

- Ascend 950PR&950DT系列产品：支持
- 其他产品：不支持

## 功能说明

头文件路径为：`"tensor_api/experimental/arch/vector/reg_data_copy.h"`，接口命名空间为`asc::te::experimental`。

根据掩码将源寄存器中的有效元素复制到目的寄存器。未指定掩码时，复制全部元素。

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline void move(reg_tensor<T>& dst, const reg_tensor<T>& src,
                                 const reg_tensor<bool>& mask = all_mask<T>())
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `T` | 输入 | 模板参数，源、目的寄存器的数据类型。 |
| `dst` | 输出 | 目的寄存器，类型为`reg_tensor<T>`。 |
| `src` | 输入 | 源寄存器，类型为`reg_tensor<T>`。 |
| `mask` | 输入 | 参与复制的元素掩码。默认全部元素有效。 |

## 数据类型

`T`支持`bool`、`uint8_t`、`int8_t`、`uint16_t`、`int16_t`、`half`、`bfloat16_t`、`uint32_t`、`int32_t`和`float`。

## 返回值说明

无。

## 调用示例

```cpp
asc::te::experimental::reg_tensor<float> src;
asc::te::experimental::reg_tensor<float> dst;
auto mask = asc::te::experimental::all_mask<float>();
asc::te::experimental::move(dst, src, mask);
```
