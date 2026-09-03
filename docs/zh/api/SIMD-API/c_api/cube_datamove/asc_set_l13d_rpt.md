# asc_set_l13d_rpt

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
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

用于设置Load3Dv2接口的repeat参数。

## 函数原型

```cpp
__aicore__ inline void asc_set_l13d_rpt(asc_load3d_v2_config& config)
__aicore__ inline void asc_set_l13d_rpt(uint16_t repeat_stride, uint8_t repeat_times,
    asc_l13d_repeat_direction repeat_direction, uint16_t dst_stride_k, uint16_t dst_start_pos_m)
```

## 参数说明

**表1** 参数说明

| 参数名  | 输入/输出 | 描述 |
| :----- | :------- | :------- |
| config | 输入 | 用于设置Load3Dv2接口的repeat参数，详细说明请参考[asc_load3d_v2_config.md](../defs/union/asc_load3d_v2_config.md)。 |
| repeat_stride | 输入 | 重复步长。 |
| repeat_times | 输入 | 重复次数。 |
| repeat_direction | 输入 | 重复方向。 |
| dst_stride_k | 输入 | 输出矩阵K方向步长。 |
| dst_start_pos_m | 输入 | 输出矩阵M方向起始位置。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 需配合[asc_load3d_v2_config.md](../defs/union/asc_load3d_v2_config.md)使用，用于设置Load3Dv2接口的repeat参数。

## 调用示例

```cpp
asc_load3d_v2_config config;
asc_set_l13d_rpt(config); // 设置Load3D的repeat相关参数为默认值rpt_stride：0，rpt_time：1，rpt_mode：0
```
