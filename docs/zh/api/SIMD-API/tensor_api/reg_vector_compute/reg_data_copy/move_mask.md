# move_mask

## 产品支持情况

- Ascend 950PR&950DT系列产品：支持
- 其他产品：不支持

## 功能说明

头文件路径为：`"tensor_api/experimental/arch/vector/reg_data_copy.h"`，接口命名空间为`asc::te::experimental`。

从源寄存器的指定数据块中提取bit并扩展，生成掩码寄存器。

## 函数原型

```cpp
template <int16_t Offset, typename T>
__simd_callee__ inline reg_tensor<bool> move_mask(const reg_tensor<T>& src)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `Offset` | 输入 | 模板参数，待提取的数据块索引。`T`为`uint16_t`时取值范围为[0, 15]；`T`为`uint32_t`时取值范围为[0, 31]。 |
| `T` | 输入 | 模板参数，源寄存器的数据类型，支持`uint16_t`和`uint32_t`。 |
| `src` | 输入 | 源寄存器，类型为`reg_tensor<T>`。 |

## 返回值说明

返回`reg_tensor<bool>`，其`reg`成员保存提取并扩展后的掩码，`mask`成员为全有效掩码。

## 约束说明

`Offset`必须是编译期常量，并满足对应数据类型的取值范围。

## 调用示例

```cpp
asc::te::experimental::reg_tensor<uint16_t> src;
auto mask = asc::te::experimental::move_mask<0>(src);
```
