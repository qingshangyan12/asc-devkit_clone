# asc_and浮点类型接口（废弃）

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
<!-- npu="910" id4 -->
- Atlas 训练系列产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="310b" id7 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id7 -->

## 功能说明

**`asc_and`的浮点类型重载自9.2.0起废弃，浮点数据类型不再作为按位与接口的支持类型。**

该接口不解释浮点数值，仅根据数据位宽对`src0`和`src1`的底层比特执行逐位与操作，并将结果写入`dst`。原接口和产品支持范围仍保留以维持源码兼容性，调用时会产生编译告警。按当前版本策略，计划删除版本为9.2.0；本次仅标记废弃，未删除接口。

没有浮点数据类型的等价替代接口。若业务确实需要处理位模式，应按相同位宽选择整数重载：FP8选择`vector_uint8_t`，FP16/BF16选择`vector_uint16_t`，FP32选择`vector_uint32_t`。迁移前需确认重解释不会改变业务语义、掩码行为和数据布局，该方式不是浮点数值运算替代。

整数和掩码重载仍通过[asc_and](../reg/logic_compute/asc_and.md)提供。

## 函数原型

```cpp
[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src0, vector_fp8_e4m3fn_t src1, vector_bool mask);

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(
    vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src0, vector_fp8_e5m2_t src1, vector_bool mask);

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(
    vector_half& dst, vector_half src0, vector_half src1, vector_bool mask);

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(
    vector_bfloat16_t& dst, vector_bfloat16_t src0, vector_bfloat16_t src1, vector_bool mask);

[[deprecated("NOTICE: asc_and floating-point overloads are deprecated since 9.2.0. Floating-point data types are not supported; use the same-width integer overload only when bitwise interpretation is intended.")]]
__simd_callee__ inline void asc_and(
    vector_float& dst, vector_float src0, vector_float src1, vector_bool mask);
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :----- | :------- | :--- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src0 | 输入 | 源操作数（矢量数据寄存器）。 |
| src1 | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 源操作数掩码（掩码寄存器）。对应位置为1时参与计算，为0时不参与计算，未参与计算的元素在输出中置0。 |

## 返回值说明

无

## 约束说明

- 本接口在非AIV上调用直接返回。
- `mask`需通过[掩码设置接口](../reg/reg_data_types/data_type_definition.md#掩码寄存器)预先赋值后再传入；未赋值的掩码寄存器内容不确定，会导致有效元素位置错误。
- 参与计算的元素个数为VL除以`sizeof(dtype)`。
- `mask`比特位为0时，`dst`对应比特位写0。

## 调用示例

以下示例按16位位宽使用整数重载。调用方需先确认`src0`和`src1`保存的是需要执行按位与的16位位模式。

```cpp
vector_uint16_t dst;
vector_uint16_t src0;
vector_uint16_t src1;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_and(dst, src0, src1, mask);
```
