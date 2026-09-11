# asc_int4x22bfloat16

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

头文件路径为：`"c_api/reg_compute/reg_convert.h"`。

根据`mask`将`src`中参与计算的`int4x2_t`类型元素转换为`bfloat16_t`类型，结果写入`dst`。由于源操作数与目的操作数的类型位宽比为1:4，`src`中的元素按索引分为四组，参数`src_quarter_pos`从`src`中离散选取每隔4个元素的索引位置读取数据，具体为索引0、4、8、…，索引1、5、9、…，索引2、6、10、…或索引3、7、11、…。

本接口为`reg`矢量计算接口，仅在AIV上生效。

## 函数原型

```cpp
__simd_callee__ inline void asc_int4x22bfloat16(
    vector_bfloat16_t& dst,
    vector_int4x2_t src,
    vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, <quarter_pos>> src_quarter_pos)
```

`<quarter_pos>`支持`ASC_DISPERSE_FIRST_QUARTER`、`ASC_DISPERSE_SECOND_QUARTER`、`ASC_DISPERSE_THIRD_QUARTER`和`ASC_DISPERSE_FOURTH_QUARTER`。

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| ------ | --------- | ---- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 掩码寄存器，用于控制各元素是否参与计算。`mask`中与元素对应的比特位为1时，该元素参与计算；为0时，该元素不参与计算。 |
| src_quarter_pos | 输入 | 四分之一位置选择标签（编译期标签分发，通过编译期重载选择对应实现），用于选择源操作数的读取位置。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 返回值说明

无。

## 流水类型

`PIPE_V`

## 约束说明

- 位置选择标签参数仅能使用上述编译期常量。
- 本接口仅在AIV上生效，并在Vector Function（`__simd_vf__`标记的函数）内调用。
- `mask`需通过掩码设置接口预先赋值后再传入；`mask`掩码位为0时，`dst`对应元素置0。

## 调用示例

```cpp
vector_int4x2_t src;
vector_bfloat16_t dst;
vector_bool mask = asc_create_mask_b8(PAT_ALL);
asc_int4x22bfloat16(dst, src, mask, ASC_DISPERSE_FIRST_QUARTER);
```
