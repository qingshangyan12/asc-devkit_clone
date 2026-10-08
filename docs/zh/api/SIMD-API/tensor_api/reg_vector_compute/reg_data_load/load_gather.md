# load_gather

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

头文件路径为：`"tensor_api/experimental/arch/vector/reg_data_load.h"`，命名空间为`asc::te::experimental`。

`load_gather`根据索引寄存器，从Unified Buffer（UB）Tensor的离散位置收集数据到[reg_tensor](../reg_tensor/reg_tensor.md)。`gather_mode::element`按元素收集，`gather_mode::data_block`按DataBlock（32B）收集。

传入`coord`时，以`tensor.engine() + tensor.layout()(coord)`为基地址；不传入`coord`时，以`tensor`起始地址为基地址。

## 函数原型

```cpp
template <gather_mode mode = gather_mode::element,
          typename Tensor,
          typename Coord,
          typename Index>
__simd_callee__ inline decltype(auto) load_gather(
    const Tensor& tensor,
    const Coord& coord,
    const reg_tensor<Index> index,
    const reg_tensor<bool>& gather_mask)

template <gather_mode mode = gather_mode::element,
          typename Tensor,
          typename Index>
__simd_callee__ inline decltype(auto) load_gather(
    const Tensor& tensor,
    const reg_tensor<Index> index,
    const reg_tensor<bool>& gather_mask)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| `mode` | 输入 | 收集模式。默认值为`gather_mode::element`。 |
| `tensor` | 输入 | 位于UB的源Tensor。 |
| `coord` | 输入 | 基地址在`tensor`逻辑布局中的坐标。 |
| `index` | 输入 | 相对于基地址的索引。元素模式下单位为元素；DataBlock模式下单位为字节。 |
| `gather_mask` | 输入 | 收集操作的有效位Mask。元素模式下逐元素生效；DataBlock模式下按b32格式解释，仅bit 0、4、8、12、16、20、24、28有效。 |

## 数据类型

- `gather_mode::element`：Tensor元素类型支持`int8_t`、`uint8_t`、`int16_t`、`uint16_t`、`half`、`bfloat16_t`、`int32_t`、`uint32_t`、`float`、`int64_t`和`uint64_t`。16bit及以下元素使用`uint16_t`索引，32bit元素使用`uint32_t`索引，64bit元素使用`uint32_t`或`uint64_t`索引。
- `gather_mode::data_block`：Tensor元素类型支持8bit、16bit、32bit和64bit类型，索引类型为`uint32_t`。

## 返回值说明

返回`reg_tensor<typename Tensor::data_type>`，其`reg`成员保存收集结果。接口不初始化返回对象的`mask`成员。

## 约束说明

- 仅支持位于UB的Tensor，基地址必须32B对齐。
- 索引对应的数据必须在UB有效地址范围内。
- `gather_mode::data_block`仅使用`index`的前8个元素，且每个索引必须32B对齐。
- 传入`coord`时，坐标维数必须与Tensor布局匹配，且坐标不能越界。
- 寄存器数据搬入应在`__simd_vf__`函数或`__simd_callee__`函数中完成。

## 调用示例

```cpp
#include "tensor_api/tensor.h"
#include "tensor_api/experimental/arch/vector/reg_data_load.h"

template <typename SrcTensor>
__simd_vf__ inline void GatherData(
    const SrcTensor& src,
    const asc::te::experimental::reg_tensor<uint32_t>& index,
    const asc::te::experimental::reg_tensor<bool>& mask)
{
    auto value = asc::te::experimental::load_gather(
        src, asc::te::make_coord(0), index, mask);
    value.with_mask(mask);
    // 使用value完成后续寄存器计算。
}
```
