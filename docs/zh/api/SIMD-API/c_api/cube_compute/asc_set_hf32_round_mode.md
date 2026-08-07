# asc_set_hf32_round_mode

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

本接口用于设置HF32模式舍入方式，使用该接口前需要先使用[asc_enable_hf32](./asc_enable_hf32.md)开启HF32模式。

## 函数原型

```c
__aicore__ inline void asc_set_hf32_round_mode(asc_hf32_round_mode hf32_round_mode)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
|:-------|:----------|:------|
| hf32_round_mode | 输入 | HF32舍入模式控制入参，[asc_hf32_round_mode](../enum/asc_hf32_round_mode.md)枚举类型，支持如下2种枚举值：<br>&nbsp;&nbsp;&bull; NEAREST_ZERO：FP32将以向最接近的值舍入，平局时远离零的方式舍入为HF32。<br>&nbsp;&nbsp;&bull; NEAREST_EVEN：FP32将以向最接近的值舍入，平局时向偶数舍入的方式舍入为HF32。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 本接口需在矩阵乘加指令（[asc_mmad](./asc_mmad.md)）执行前调用，确保舍入模式配置在矩阵乘加结果生成前生效。
- 本接口配置的舍入模式仅在HF32模式开启期间生效，需先开启HF32模式再调用本接口，否则舍入模式配置不产生实际作用。
- 舍入模式配置后会持续生效，HF32模式关闭后再次开启仍将沿用上次的舍入模式配置，如需切换舍入模式，请重新调用本接口。
- hf32_round_mode仅支持如下2种枚举值：

  - asc_hf32_round_mode::NEAREST_ZERO
  - asc_hf32_round_mode::NEAREST_EVEN

## 调用示例

```cpp
asc_hf32_round_mode hf32_round_mode = asc_hf32_round_mode::NEAREST_EVEN;
asc_set_hf32_round_mode(hf32_round_mode);
```
