

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

数据缓存失效，该指令用于使整个数据缓存无效化，且不会对处于“脏”状态的缓存行执行写回操作。换言之，所有缓存行将被直接标记为无效，其修改过但尚未同步至主存的数据将被丢弃。

<font color="red">

头文件路径：`"c_api/cache_ctrl/cache_ctrl.h"`。

数据缓存无效化接口，将整个Data Cache的所有Cache Line标记为无效，使其从缓存中移除。本接口不检查缓存行是否为脏（dirty，表示数据已被修改但尚未写回到GM），若存在脏数据将被直接丢弃，不执行回写。调用后，下一次访问这些内存地址时会从GM重新加载数据，而不是使用可能过期的缓存数据。

本接口无参数，调用即触发整个Data Cache的全量失效操作。

</font>

## 函数原型

```cpp
__aicore__ inline void asc_dci()
```

## 参数说明

无

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

在调用asc_dci之前，需调用[asc_sync_data_barrier](../sync/asc_sync_data_barrier.md)插入DSB_ALL指令，等待所有内存访问指令执行结束。
- 在调用`asc_dci`之前，必须调用[asc_sync_data_barrier](../sync/asc_sync_data_barrier.md)，并将参数设置为`mem_dsb_t::DSB_ALL`，等待此前的内存访问指令执行结束，再使整个数据缓存失效。调用顺序如下：

    ```cpp
    asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
    asc_dci();
    ```

  `asc_dci`不会等待此前尚未完成的内存访问。若未调用`asc_sync_data_barrier`，此前的内存访问可能与`asc_dci`并行执行，并在缓存失效后再次填充或修改缓存，导致无法保证`asc_dci`执行后的缓存状态符合预期。

<font color="red">

- 调用本接口前需先插入数据同步屏障指令（`asc_sync_data_barrier`），保证此前所有对GM的写操作已对外可见，避免本接口将尚未写回的脏数据直接丢弃导致数据丢失。
- 本接口执行强串行化语义：调用后会阻塞后续所有Data Cache请求，直到全量失效操作完成后才放行。在使用本接口的场景中，应避免在性能敏感路径上频繁调用。
- 本接口将整个Data Cache全量失效，不支持按地址范围或内存域过滤。若仅需失效指定地址范围的缓存，应使用对应的单地址或指定范围失效接口。
- 本接口不回写脏数据：调用前若Data Cache中存在尚未写回到GM的脏数据，这些数据将被直接丢弃。若需保留脏数据，应先执行Clean操作（清理并回写）再调用本接口，或改用清理并失效的接口。

</font>

## 调用示例

```cpp
asc_sync_data_barrier(mem_dsb_t::DSB_ALL);
asc_dci();
```
