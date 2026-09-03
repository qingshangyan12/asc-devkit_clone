# asc_bfloat162float（废弃）

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

**该接口通过接口后缀控制源操作数读取位置的三参数原型已废弃，请使用[asc_bfloat162float](../reg_compute/reg_convert/asc_bfloat162float.md)的显式位置参数重载替代。**

根据`mask`将`src`中参与计算的`bfloat16_t`类型元素转换为`float`类型，结果写入`dst`。旧原型通过无后缀或`_v2`后缀选择读取源操作数索引为偶数或奇数的位置。

## 函数原型

```c
__simd_callee__ inline void asc_bfloat162float(vector_float& dst,
                                               vector_bfloat16_t src,
                                               vector_bool mask)

__simd_callee__ inline void asc_bfloat162float_v2(vector_float& dst,
                                                  vector_bfloat16_t src,
                                                  vector_bool mask)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| ------ | --------- | ---- |
| dst | 输出 | 目的操作数（矢量数据寄存器）。 |
| src | 输入 | 源操作数（矢量数据寄存器）。 |
| mask | 输入 | 掩码寄存器，用于控制各元素是否参与计算。 |

矢量数据寄存器和掩码寄存器的详细说明请参见[reg数据类型定义](../defs/type/data_type_definition.md)。

## 返回值说明

无

## 约束说明

- 无位置后缀的旧原型对应新接口的`ASC_POSITION_EVEN`。
- `_v2`旧原型对应新接口的`ASC_POSITION_ODD`。

## 调用示例

```c
vector_bfloat16_t src;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
vector_float dst;
asc_bfloat162float(dst, src, mask); // 已废弃
```
