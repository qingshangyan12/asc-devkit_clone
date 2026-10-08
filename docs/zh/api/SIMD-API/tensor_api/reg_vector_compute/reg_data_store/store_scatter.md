# store_scatter

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`"tensor_api/experimental/arch/vector/reg_data_store.h"`，命名空间为`asc::te::experimental`。

`store_scatter`根据索引寄存器，将[reg_tensor](../reg_tensor/reg_tensor.md)中的有效元素分散写入Unified Buffer（UB）Tensor。

传入`coord`时，以`tensor.engine() + tensor.layout()(coord)`为基地址；不传入`coord`时，以`tensor`起始地址为基地址。

## 函数原型

```cpp
template <typename Tensor, typename T, typename Index>
__simd_callee__ inline void store_scatter(
    Tensor& tensor,
    const reg_tensor<T>& src,
    const reg_tensor<Index>& index)

template <typename Tensor, typename Coord, typename T, typename Index>
__simd_callee__ inline void store_scatter(
    Tensor& tensor,
    const Coord& coord,
    const reg_tensor<T>& src,
    const reg_tensor<Index>& index)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `tensor` | 输出 | 位于UB的目的Tensor。 |
| `coord` | 输入 | 基地址在`tensor`逻辑布局中的坐标。 |
| `src` | 输入 | 源寄存器。`src.mask`逐元素指示是否写入。 |
| `index` | 输入 | `src`中各元素相对于基地址的目的位置，单位为元素。 |

## 数据类型

Tensor元素类型与`src`元素类型必须一致，支持`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`和`uint64_t`。16bit及以下元素使用`uint16_t`索引，32bit元素使用`uint32_t`索引，64bit元素使用`uint32_t`或`uint64_t`索引。

## 返回值说明

无。

## 约束说明

- 仅支持位于UB的Tensor，基地址必须32B对齐。
- Tensor元素类型必须与`src`元素类型一致。
- 索引对应的数据必须在UB有效地址范围内。
- `index`中的有效索引必须唯一。存在重复索引时，无法确定最终写入的数据。
- 元素类型为`int8_t`或`uint8_t`时，仅`src`中的偶数位置元素有效。
- 传入`coord`时，坐标维数必须与Tensor布局匹配，且坐标不能越界。
- 寄存器数据搬出应在`__simd_vf__`函数或`__simd_callee__`函数中完成。

## 调用示例

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/experimental/arch/vector/reg_data_store.h"

template <typename DstTensor, typename T>
__simd_vf__ inline void ScatterData(
    DstTensor& dst,
    const asc::te::experimental::reg_tensor<T>& value,
    const asc::te::experimental::reg_tensor<uint32_t>& index)
{
    asc::te::experimental::store_scatter(
        dst, asc::te::make_coord(0), value, index);
}
```
