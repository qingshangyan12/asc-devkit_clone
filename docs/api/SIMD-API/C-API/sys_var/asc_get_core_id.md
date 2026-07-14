# asc_get_core_id

## 产品支持情况

<!-- npu="A3" id1 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id1 -->
<!-- npu="910b" id2 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id2 -->
<!-- npu="950" id3 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id3 -->

## 功能说明

获取当前核的编号。

## 函数原型

```cpp
__aicore__ inline int64_t asc_get_core_id()
```

## 参数说明

无

## 返回值说明

返回当前核的编号。

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

无