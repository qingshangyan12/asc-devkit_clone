# asc_half2e4m3

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

根据`mask`将`src`中参与计算的`half`类型元素转换为`fp8_e4m3fn_t`类型，结果写入`dst`。由于源操作数与目的操作数的类型位宽比为2:1，`dst`中的元素按索引分为偶数位置和奇数位置，参数`dst_pos`选择转换结果的写入位置。非合并写入版本将`mask`对应位为0或未选中的位置置零，合并写入版本保留这些位置的原值。

本接口使用RN舍入模式，支持饱和和非饱和模式，并提供合并写入版本。`asc_cast`和`asc_cast_merge`提供与本接口对应的编译期重载：前者将未选中位置清零，后者保留未选中位置的原值。

本接口为Reg矢量计算接口，仅在AIV上生效。

## 函数原型

```cpp
__simd_callee__ inline void asc_half2e4m3_rn(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
```

```cpp
__simd_callee__ inline void asc_half2e4m3_rn_sat(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
```

```cpp
__simd_callee__ inline void asc_half2e4m3_rn(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
```

```cpp
__simd_callee__ inline void asc_half2e4m3_rn_sat(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
```

```cpp
__simd_callee__ inline vector_fp8_e4m3fn_t asc_half2e4m3_rn(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
```

```cpp
__simd_callee__ inline vector_fp8_e4m3fn_t asc_half2e4m3_rn_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
```

```cpp
__simd_callee__ inline vector_fp8_e4m3fn_t asc_half2e4m3_rn(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
```

```cpp
__simd_callee__ inline vector_fp8_e4m3fn_t asc_half2e4m3_rn_sat(
    vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
```

```cpp
__simd_callee__ inline void asc_half2e4m3_rn_v2(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_half2e4m3_rn_sat_v2(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask)
```

```cpp
__simd_callee__ inline void asc_half2e4m3_rn_merge(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
```

```cpp
__simd_callee__ inline void asc_half2e4m3_rn_sat_merge(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
```

```cpp
__simd_callee__ inline void asc_half2e4m3_rn_merge(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
```

```cpp
__simd_callee__ inline void asc_half2e4m3_rn_sat_merge(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
```

```cpp
__simd_callee__ inline void asc_cast(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
```

```cpp
__simd_callee__ inline void asc_cast(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
```

```cpp
__simd_callee__ inline void asc_cast(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
```

```cpp
__simd_callee__ inline void asc_cast(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
```

```cpp
__simd_callee__ inline void asc_cast_merge(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
```

```cpp
__simd_callee__ inline void asc_cast_merge(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
```

```cpp
__simd_callee__ inline void asc_cast_merge(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
```

```cpp
__simd_callee__ inline void asc_cast_merge(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
```

**接口说明如下：**

- `asc_half2e4m3_rn`和`asc_half2e4m3_rn_sat`分别为非饱和和饱和版本。
- `asc_half2e4m3_rn_merge`和`asc_half2e4m3_rn_sat_merge`各包含`EVEN/ODD`两个位置重载。
- `asc_cast`和`asc_cast_merge`各包含`NO_SAT/SAT`与`EVEN/ODD`的全部组合重载。
- `asc_half2e4m3_rn_v2`和`asc_half2e4m3_rn_sat_v2`等价于使用`ODD`位置的引用输出版本。

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| ------ | --------- | ---- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 掩码寄存器，用于控制各元素是否参与计算。 |
| round_mode | 输入 | 舍入模式，固定为`ASC_CAST_RN`。具名接口固定为RN舍入。 |
| saturation_mode | 输入 | 饱和模式，取`ASC_CAST_NO_SAT`或`ASC_CAST_SAT`。 |
| dst_pos | 输入 | 位置选择标签。选择将结果写入目的操作数的偶数或奇数位置。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../../defs/type/data_type_definition.md)。

## 数据类型

| 源数据类型 | 目的数据类型 |
| --- | --- |
| `vector_half` | `vector_fp8_e4m3fn_t` |

## 返回值说明

- 引用输出版本无返回值。
- 返回值版本返回`vector_fp8_e4m3fn_t`类型的转换结果。
- `asc_cast`和`asc_cast_merge`为引用输出接口，无返回值。

## 约束说明

- Reg矢量计算C API通用约束请参见[通用约束](../overview.md#通用约束)。
- 位置选择标签仅能使用编译期常量。
- `asc_half2e4m3_rn`、`asc_half2e4m3_rn_sat`及其`_v2`变体和`asc_cast`在`mask`对应位为0或未选中位置时将结果置零；`asc_half2e4m3_rn_merge`、`asc_half2e4m3_rn_sat_merge`和`asc_cast_merge`保留这些位置的原值。
- 使用饱和或非饱和模式时，需要配置`ctrl`寄存器，详细说明请参见[asc_set_ctrl](../../spr/asc_set_ctrl.md)。

## 调用示例

```cpp
__simd_vf__ inline void convert(vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask)
{
    // 具名接口：RN舍入、饱和模式，写入偶数位置。
    asc_half2e4m3_rn_sat(dst, src, mask, ASC_POSITION_EVEN);
    // _v2固定写入奇数位置。
    asc_half2e4m3_rn_sat_v2(dst, src, mask);
    // merge接口保留未选中位置的原值。
    asc_half2e4m3_rn_merge(dst, src, mask, ASC_POSITION_ODD);
    // 通用cast接口使用编译期标签选择舍入、饱和和位置模式。
    asc_cast(dst, src, mask, ASC_CAST_RN, ASC_CAST_SAT, ASC_POSITION_EVEN);
    asc_cast_merge(dst, src, mask, ASC_CAST_RN, ASC_CAST_NO_SAT, ASC_POSITION_ODD);
}
```
