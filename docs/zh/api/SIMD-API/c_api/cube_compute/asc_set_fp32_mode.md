# asc_set_fp32_mode

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

本接口用于启用矩阵乘加（Mmad）计算的FP32模式。调用该接口会关闭HF32模式，其作用与[asc_enable_hf32](./asc_enable_hf32.md)相反，两个接口不同时生效。启用FP32模式后，L0A Buffer与L0B Buffer中的`float`数据在参与矩阵乘加计算之前不做舍入处理。

## 函数原型

```c
__aicore__ inline void asc_set_fp32_mode()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口需在矩阵乘加指令（[asc_mmad](./asc_mmad.md)）执行前调用，确保模式配置在矩阵乘加结果生成前生效。
- FP32模式启用后会持续生效，不会自动关闭。后续矩阵乘加指令若不显式重新配置，将沿用当前模式。如需开启HF32模式，请重新调用[asc_enable_hf32](./asc_enable_hf32.md)接口。

## 调用示例

```cpp
asc_set_fp32_mode();
```
