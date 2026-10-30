# move

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

头文件路径为：`"tensor_api/experimental/arch/vector/reg_data_copy.h"`，接口命名空间为`asc::te::experimental`。

完成寄存器之间的数据搬运。将`src`中被`src.mask`筛选的有效元素复制到`dst`对应位置。

- `T`为非`bool`类型时，未被`src.mask`筛选的位置采用Merging模式，保持`dst.reg`中的原值。
- `T`为`bool`类型时，将`src.reg`中被`src.mask`筛选的bit复制到`dst.reg`，未被`src.mask`筛选的bit在`dst.reg`中置0。

完成数据搬运后，接口会将`src.mask`同步写入`dst.mask`。

本接口为Reg矢量搬运接口，仅在AIV上生效。

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline void move(reg_tensor<T>& dst, const reg_tensor<T>& src)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| T | 输入 | 模板参数，源、目的寄存器的数据类型。 |
| dst | 输出 | 目的寄存器，类型为`reg_tensor<T>`，数据类型须与`src`完全一致。`T`为非`bool`类型且`src.mask`部分有效时，`dst.reg`需提前初始化，否则未被筛选的位置将出现未知值。接口会将`src.mask`写入`dst.mask`。 |
| src | 输入 | 源寄存器，类型为`reg_tensor<T>`，数据类型须与`dst`完全一致。其`mask`成员用于指示参与复制的元素。对应位置有效时复制`src.reg`中的元素；`T`为非`bool`类型时，对应位置无效的元素保留`dst.reg`中的原值；`T`为`bool`类型时，对应位置无效的bit在`dst.reg`中置0。 |

## 数据类型

支持的数据类型为：bool、uint8_t、int8_t、uint16_t、int16_t、half、bfloat16_t、uint32_t、int32_t、float。

## 返回值说明

无。

## 约束说明

- `dst`与`src`必须为数据类型相同的`reg_tensor`。
- 调用接口前必须初始化`src.mask`，用于指定参与复制的元素。
- `T`为非`bool`类型时，`src.mask`未筛选的位置保持`dst.reg`中的原值。`src.mask`部分有效时，必须在调用接口前初始化`dst.reg`。
- `T`为`bool`类型时，`src.mask`未筛选的bit在`dst.reg`中置0。
- 对所有支持的数据类型，接口都会将`src.mask`写入`dst.mask`。
- 寄存器数据搬运应在`__simd_vf__`函数或`__simd_callee__`函数中完成。

## 调用示例

```cpp
template <typename SrcTensor, typename DstTensor>
__simd_vf__ inline void MoveData(SrcTensor src, DstTensor dst,
    uint32_t repeat_times, uint32_t total_length,
    uint32_t one_repeat_size)
{
    using data_type = typename SrcTensor::data_type;
    uint32_t remain = total_length;
    for (uint16_t i = 0; i < repeat_times; ++i) {
        auto mask = asc::te::experimental::update_mask<data_type>(remain);
        uint32_t offset = i * one_repeat_size;
        const auto coord = asc::te::make_coord(offset);
        auto src_reg = asc::te::experimental::load(src, coord).with_mask(mask);
        auto dst_reg = asc::te::experimental::load(dst, coord);
        asc::te::experimental::move(dst_reg, src_reg);
        asc::te::experimental::store(dst, coord, dst_reg);
    }
}
```
