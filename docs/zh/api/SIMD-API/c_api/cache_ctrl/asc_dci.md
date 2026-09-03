# asc_dci

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

数据缓存无效化接口，将整个DCache的所有Cache Line标记为无效，使其从缓存中移除。本接口不检查缓存行是否为脏（dirty，表示数据已被修改但尚未写回到GM），若存在脏数据将被直接丢弃，不执行回写。调用后，下一次访问这些内存地址时会从GM重新加载数据，而不是使用可能过期的缓存数据。

本接口无参数，调用即触发整个DCache的全量失效操作。

## 函数原型

```c
__aicore__ inline void asc_dci()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

在调用`asc_dci`之前，必须调用[asc_sync_data_barrier](../sync/asc_sync_data_barrier.md)，并将参数设置为`mem_dsb_t::DSB_ALL`，等待此前的内存访问指令执行结束，再使整个数据缓存失效。调用顺序如下：

```cpp
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
asc_dci();
```

## 调用示例

```cpp
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
asc_dci();
```
