# asc_int322uint8

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->


## 功能说明

根据`mask`将`src`中参与计算的`int32_t`类型元素转换为`uint8_t`类型，结果写入`dst`。由于源操作数与目的操作数的类型位宽比为4:1，一个矢量寄存器需要按四个位置分块处理，根据`dst_quarter_pos`选择将转换结果写入`dst`的四分之一位置，其他位置置零。

关于舍入模式和饱和/非饱和模式的详细说明，请参见[舍入模式与饱和模式](rounding_mode.md)。

本接口为`reg`矢量计算接口，仅在AIV上生效。

## 函数原型（占位符形式）

```c
// 通过引用参数输出结果
__simd_callee__ inline void asc_int322uint8<sat_mode>(vector_uint8_t& dst,
                                     vector_int32_t src,
                                     vector_bool mask,
                                     std::integral_constant<asc_position_quarter_mode, <quarter_pos>> dst_quarter_pos)

// 通过函数返回值返回结果
__simd_callee__ inline vector_uint8_t asc_int322uint8<sat_mode>(vector_int32_t src,
                                     vector_bool mask,
                                     std::integral_constant<asc_position_quarter_mode, <quarter_pos>> dst_quarter_pos)
```

- `<sat_mode>`表示饱和/非饱和模式，为空时表示非饱和模式，取值为`_sat`时表示饱和模式。

- `<quarter_pos>`支持`asc_position_quarter_mode::DISPERSE_FIRST_QUARTER`、`asc_position_quarter_mode::DISPERSE_SECOND_QUARTER`、`asc_position_quarter_mode::DISPERSE_THIRD_QUARTER`和`asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER`，调用时依次使用`ASC_DISPERSE_FIRST_QUARTER`、`ASC_DISPERSE_SECOND_QUARTER`、`ASC_DISPERSE_THIRD_QUARTER`和`ASC_DISPERSE_FOURTH_QUARTER`。

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| ------ | --------- | ---- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |
| dst_quarter_pos | 输入 | 四分之一位置选择标签（编译器标签类型），用于选择目的操作数的写入位置。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

- 通过引用参数输出结果的函数原型无返回值。
- 通过函数返回值输出结果的函数原型返回计算结果，返回值类型与对应引用输出函数原型中`dst`参数的类型一致（去除引用）。

## 约束说明

- 通过引用参数输出结果的函数原型在非AIV上调用时直接返回。
- 通过函数返回值输出结果的函数原型在非AIV上调用时返回对应矢量类型的默认构造值。
- 本接口在Vector Function（`__simd_vf__`标记的函数）内调用。
- `mask`需通过掩码设置接口预先赋值后再传入，未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- `mask`掩码位为0时，`dst`对应元素置0。
- 使用饱和或非饱和模式时，需要配置`ctrl`寄存器，详细说明请参见[asc_set_ctrl](../../spr/asc_set_ctrl.md)。

## 调用示例

以下示例使用函数返回值获取转换结果，并选择第一个四分之一位置：

```c
__simd_vf__ inline vector_uint8_t convert(vector_int32_t src)
{
    vector_bool mask = asc_create_mask_b32(PAT_ALL);
    return asc_int322uint8(src, mask, ASC_DISPERSE_FIRST_QUARTER);
}
```
