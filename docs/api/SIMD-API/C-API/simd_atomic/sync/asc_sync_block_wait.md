# asc_sync_block_wait

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

- 接口声明文件：`c_api/sync/sync.h`
- 核心功能：和[asc_sync_block_arrive](asc_sync_block_arrive.md)配合使用（通过flag_id关联），用于等待所有同步对象到达flag_id对应的同步点。如果flag_id所对应的计数器数值为0则阻塞不执行；如果对应的计数器大于0，则计数器减一，同时后续指令开始执行。
- 实现原理/公式：对应CrossCoreWaitFlag的模式2——单个AI Core内AIC与全部AIV之间的同步，wait对应WaitFlag。用于阻塞本核后续指令的下发（已下发指令可正常执行），直至调度模块确认所有应调用asc_sync_block_arrive的核均已上报完成，随即解除阻塞。

## 函数原型

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline void asc_sync_block_wait(pipe_t pipe, uint8_t flag_id)
__aicore__ inline void asc_sync_block_wait(pipe_t pipe, int64_t flag_id)
```

<!-- end id4 -->
<!-- npu="A3" id5 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
__aicore__ inline void asc_sync_block_wait(pipe_t pipe, uint8_t flag_id)
__aicore__ inline void asc_sync_block_wait(pipe_t pipe, int64_t flag_id)
```

<!-- end id5 -->
<!-- npu="910b" id6 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
__aicore__ inline void asc_sync_block_wait(pipe_t pipe, uint8_t flag_id)
__aicore__ inline void asc_sync_block_wait(pipe_t pipe, int64_t flag_id)
```

<!-- end id6 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950" id7 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 需要执行同步指令的流水类型。 |
| flag_id | 输入 | 事件标号，取值范围[0,15]。 |

<!-- end id7 -->
<!-- npu="A3" id8 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 需要执行同步指令的流水类型。 |
| flag_id | 输入 | 事件标号，取值范围[0,15]。 |

<!-- end id8 -->
<!-- npu="910b" id9 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 需要执行同步指令的流水类型。 |
| flag_id | 输入 | 事件标号，取值范围[0,15]。 |

<!-- end id9 -->

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：不占用临时空间。
- 同步资源占用：占用核间同步计数器资源，每个flag_id对应一个计数器，取值范围为[0,15]。
- 资源占用与参数/数据量关系：资源占用与数据量无关，与flag_id参数取值有关。

## 约束说明

<!-- npu="950" id10 -->
- 针对Ascend 950PR/Ascend 950DT：无需进行核间同步基地址ffts_addr的设置。
<!-- end id10 -->
<!-- npu="A3" id11 -->
- 针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：使用前需通过Host侧调用接口aclrtGetHardwareSyncAddr获取核间同步基地址ffts_addr，并通过asc_set_ffts_base_addr进行设置。
<!-- end id11 -->
<!-- npu="910b" id12 -->
- 针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：使用前需通过Host侧调用接口aclrtGetHardwareSyncAddr获取核间同步基地址ffts_addr，并通过asc_set_ffts_base_addr进行设置。
<!-- end id12 -->
- flag_id的取值范围为0至15，每个flag_id的计数器范围为0至15。
- 必须保证每个flag_id的四位计数器不溢出，否则将引发异常。
- 必须保证相同的flag_id在同一时间仅被一条流水线使用。

## 调用示例

<!-- npu="950" id13 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
int64_t flag_id = 1;
asc_sync_block_arrive(PIPE_S, flag_id);
asc_sync_block_wait(PIPE_S, flag_id);
```

<!-- end id13 -->
<!-- npu="A3" id14 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
// Host侧调用接口aclrtGetHardwareSyncAddr获取核间同步基地址ffts_addr
uint64_t config = *(__gm__ uint64_t*)ffts_addr;
asc_set_ffts_base_addr(config);
int64_t flag_id = 1;
asc_sync_block_arrive(PIPE_S, flag_id);
asc_sync_block_wait(PIPE_S, flag_id);
```

<!-- end id14 -->
<!-- npu="910b" id15 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
// Host侧调用接口aclrtGetHardwareSyncAddr获取核间同步基地址ffts_addr
uint64_t config = *(__gm__ uint64_t*)ffts_addr;
asc_set_ffts_base_addr(config);
int64_t flag_id = 1;
asc_sync_block_arrive(PIPE_S, flag_id);
asc_sync_block_wait(PIPE_S, flag_id);
```

<!-- end id15 -->
