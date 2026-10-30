# compress

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

头文件路径为：`"tensor_api/experimental/arch/vector/reg_compute_sel.h"`，命名空间为`asc::te::experimental`。

根据`src.mask`筛选源寄存器中的元素，将被选中的元素按原顺序连续排列在结果寄存器的低位，剩余元素置0。例如，源数据的前四个元素为`{10, 20, 30, 40}`，`src.mask`选中第0、2、3个元素且不选中其余元素，结果为`{10, 30, 40, 0, ...}`。

本接口仅在AIV上生效。

## 函数原型

```cpp
template <typename T>
__simd_callee__ inline reg_tensor<T> compress(reg_tensor<T> src);
```

## 参数说明

**表1** 模板参数说明

| 参数名 | 描述 |
| --- | --- |
| T | 源寄存器及返回结果的元素类型，支持的类型请参见[数据类型](#数据类型)。 |

**表2** 函数参数说明

| 参数名 | 输入/输出 | 描述 |
| --- | --- | --- |
| src | 输入 | 源操作数，类型为[reg_tensor&lt;T&gt;](../reg_tensor/reg_tensor.md)。`src.mask`为筛选条件，使用其中与T元素位宽对应的比特位，1表示选中对应源元素，0表示不选中。 |

## 数据类型

T支持：int8_t、uint8_t、hifloat8_t、fp8_e8m0_t、fp8_e5m2_t、fp8_e4m3fn_t、int16_t、uint16_t、half、bfloat16_t、int32_t、uint32_t、float。

## 返回值说明

返回`reg_tensor<T>`，`reg`保存压缩结果，`mask`直接复制`src.mask`，不返回被选中元素的数量。

## 约束说明

- `src.mask`须按T的元素位宽构造并初始化，且须保证被选中的源元素已初始化。可通过[with_mask](../reg_tensor/reg_tensor.md#with_mask)绑定筛选掩码。
- 返回对象的`mask`直接继承`src.mask`。若后续计算或搬出只需处理被选中的元素，须先通过[with_mask](../reg_tensor/reg_tensor.md#with_mask)重新绑定结果低位连续的有效元素掩码，不能直接沿用原筛选掩码。

## 调用示例

以下代码片段在VF中调用。`src.reg`及`src.mask`已初始化，其中`src.mask`为筛选掩码；`output_mask.reg`按T的元素位宽构造，仅选中低位连续的有效元素，其有效元素个数等于`src.mask`中选中的元素个数。

```cpp
template <typename T>
__simd_callee__ inline asc::te::experimental::reg_tensor<T> compress_example(
    asc::te::experimental::reg_tensor<T> src,
    const asc::te::experimental::reg_tensor<bool>& output_mask)
{
    auto result = asc::te::experimental::compress(src).with_mask(output_mask);
    return result;
}
```
