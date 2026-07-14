# asc_set_mmad_direction_m

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

## 功能说明

设置Mmad计算时优先通过M/N中的M方向生成结果，然后通过N方向产生结果，M为矩阵的行，N为矩阵的列。

## 函数原型

```cpp
__aicore__ inline void asc_set_mmad_direction_m()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
asc_set_mmad_direction_m();
```
