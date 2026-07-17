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

## 功能说明

- 接口声明文件：`"c_api/cache_ctrl/cache_ctrl.h"`
- 核心功能：数据缓存失效，该指令用于使整个数据缓存无效化，且不会对处于"脏"状态的缓存行执行写回操作。换言之，所有缓存行将被直接标记为无效，其修改过但尚未同步至主存的数据将被丢弃。
- 实现原理/公式：将整个Data Cache中所有Cache Line标记为无效状态，丢弃所有脏数据，不执行写回操作。与asc_dcci的区别在于：asc_dcci会先写回脏数据再失效，而asc_dci直接失效不写回。

## 函数原型

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline void asc_dci()
```

<!-- end id4 -->

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：不占用同步资源。
- 资源占用与参数/数据量关系：无。

## 约束说明

- 在调用asc_dci之前，需调用[asc_sync_data_barrier](../sync/asc_sync_data_barrier.md)插入DSB_ALL指令，等待所有内存访问指令执行结束。
- 该接口会丢弃所有脏数据，调用前请确保不需要保留缓存中未写回的数据，或已通过asc_dcci完成脏数据写回。

## 调用示例

<!-- npu="950" id5 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// 在调用asc_dci之前，先插入数据同步屏障，确保所有内存访问指令执行结束。
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);

// 使整个数据缓存失效化，丢弃所有脏数据。
asc_dci();
```

- 完整样例链接：`<TODO>`

<!-- end id5 -->
