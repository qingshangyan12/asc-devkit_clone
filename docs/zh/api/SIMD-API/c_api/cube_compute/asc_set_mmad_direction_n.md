# asc_set_mmad_direction_n

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

本接口用于设置矩阵乘加（Mmad）计算先沿列方向、再沿行方向生成矩阵计算结果分形。该配置只改变L0C Buffer上结果分形的生成顺序，不改变计算结果、乘加次数或输出格式。

为获得更好的性能表现，本接口可用于以下场景：

- UnitFlag开启且搬出时开启Nz2ND随路格式转换。
- UnitFlag开启且搬出时不进行随路格式转换，且开启B8/B4量化触发Channel Merge功能。

## 函数原型

```c
__aicore__ inline void asc_set_mmad_direction_n()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口需在矩阵乘加指令执行前调用，确保模式配置在矩阵乘加结果生成前生效。
- 方向配置一旦写入会持续生效，后续矩阵乘加指令若不显式重新配置，将沿用当前方向配置。如需切换为先沿行方向、再沿列方向生成矩阵计算结果分形，请重新调用[asc_set_mmad_direction_m](./asc_set_mmad_direction_m.md)接口。

## 调用示例

```cpp
asc_set_mmad_direction_n();
```
