# asc_e8m02bfloat16

## 产品支持情况

<!-- npu="960" id1 -->
- Ascend 960PR&960DT系列产品：支持
<!-- end id1 -->
<!-- npu="950" id8 -->
- Ascend 950PR&950DT系列产品：不支持
<!-- end id8 -->
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

头文件路径为：`"c_api/reg_compute/reg_convert.h"`。

本接口仅在Ascend 960PR&960DT系列产品的`dav-9201`架构上支持。

根据`mask`将`src`中参与计算的`fp8_e8m0_t`类型元素转换为`bfloat16_t`类型，结果写入`dst`。由于源操作数与目的操作数的类型位宽比为1:2，参数`src_pos`选择从源操作数的偶数或奇数位置读取数据，未选中的源位置不参与计算。

本接口提供引用输出、返回值和`_v2`变体。`_v2`变体固定读取源操作数的奇数位置。`asc_cast`提供与该转换对应的`EVEN/ODD`位置重载。

本接口为Reg矢量计算接口，仅在AIV上生效。

## 函数原型

```cpp
__simd_callee__ inline void asc_e8m02bfloat16(
    vector_bfloat16_t& dst, vector_fp8_e8m0_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, <position>> src_pos)

__simd_callee__ inline vector_bfloat16_t asc_e8m02bfloat16(
    vector_fp8_e8m0_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, <position>> src_pos)

__simd_callee__ inline void asc_e8m02bfloat16_v2(
    vector_bfloat16_t& dst, vector_fp8_e8m0_t src, vector_bool mask)

__simd_callee__ inline void asc_cast(
    vector_bfloat16_t& dst, vector_fp8_e8m0_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, <position>> src_pos)

__simd_callee__ inline void asc_cast(
    vector_fp8_e8m0_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, <round_mode>> round_mode,
    std::integral_constant<asc_cast_saturation_mode, <sat_mode>> saturation_mode,
    std::integral_constant<asc_position_mode, <position>> dst_pos)
```

**占位符说明如下：**

- `<position>`支持`EVEN`和`ODD`，对应常量`ASC_POSITION_EVEN`和`ASC_POSITION_ODD`。
- `asc_e8m02bfloat16_v2`等价于使用`ODD`位置的引用输出版本。
- `asc_cast`包含`<position>`的两个重载，不支持饱和模式和合并写入版本。
- `vector_bfloat16_t`到`vector_fp8_e8m0_t`的`asc_cast`支持`RU/RZ`舍入、`NO_SAT/SAT`饱和模式以及`EVEN/ODD`位置的全部组合。

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| ------ | --------- | ---- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 掩码寄存器，用于控制各元素是否参与计算。 |
| src_pos | 输入 | 源数据位置选择标签，选择从源操作数的偶数或奇数位置读取数据。 |
| round_mode | 输入 | `bfloat16_t`到`fp8_e8m0_t`的`asc_cast`舍入模式，支持`ASC_CAST_RU`和`ASC_CAST_RZ`。 |
| saturation_mode | 输入 | `bfloat16_t`到`fp8_e8m0_t`的`asc_cast`饱和模式，支持`ASC_CAST_NO_SAT`和`ASC_CAST_SAT`。 |
| dst_pos | 输入 | `bfloat16_t`到`fp8_e8m0_t`的`asc_cast`目的数据位置选择标签。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 数据类型

| 源数据类型 | 目的数据类型 |
| --- | --- |
| `vector_fp8_e8m0_t` | `vector_bfloat16_t` |
| `vector_bfloat16_t` | `vector_fp8_e8m0_t` |

## 返回值说明

- 引用输出版本和`_v2`版本无返回值。
- 返回值版本返回`vector_bfloat16_t`类型的转换结果。
- `asc_cast`为引用输出接口，无返回值。

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 位置选择标签仅能使用编译期常量。
- `mask`掩码位为0时，`dst`对应元素置0。

## 调用示例

```cpp
__simd_vf__ inline void convert(vector_bfloat16_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    // 具名接口：从源操作数偶数位置读取e8m0。
    asc_e8m02bfloat16(dst, src, mask, ASC_POSITION_EVEN);
    // _v2固定从源操作数奇数位置读取e8m0。
    asc_e8m02bfloat16_v2(dst, src, mask);
    // e8m0到bfloat16的通用cast。
    asc_cast(dst, src, mask, ASC_POSITION_ODD);
}
```
