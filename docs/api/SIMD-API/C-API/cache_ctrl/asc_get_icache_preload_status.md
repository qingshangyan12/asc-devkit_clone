# asc_get_icache_preload_status

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

获取ICache的Preload的状态。

## 函数原型

```cpp
__aicore__ inline int64_t asc_get_icache_preload_status()
```

## 参数说明

无

## 返回值说明

int64_t类型，0表示空闲，1表示忙。

## 流水类型

PIPE_S

## 约束说明

无

## 调用示例

```cpp
int64_t status = asc_get_icache_preload_status();
```
