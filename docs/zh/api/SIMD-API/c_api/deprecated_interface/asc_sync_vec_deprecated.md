# asc_sync_vec（废弃）

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

头文件路径为：`"c_api/sync/sync.h"`。

**无入参原型`asc_sync_vec()`已废弃，请使用[asc_sync](../sync/asc_sync.md)执行针对所有流水的同步操作。**

等待`PIPE_V`中的前序指令完成后，再执行其他流水的后续指令。

## 函数原型

```c
__aicore__ inline void asc_sync_vec()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

本接口只能在AIV中调用。

## 调用示例

```cpp
asc_sync();
```
