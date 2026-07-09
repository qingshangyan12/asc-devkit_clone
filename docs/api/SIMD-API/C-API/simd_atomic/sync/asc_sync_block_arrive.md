# asc_sync_block_arrive

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
- 核心功能：和[asc_sync_block_wait](asc_sync_block_wait.md)配合使用（通过flag_id关联），用于组内block和subblock间的信号同步。通过向核间同步寄存器发送同步信号，标识当前pipeline已到达flag_id对应的同步点。每个flag_id对应一个初始值为0的计数器，执行asc_sync_block_arrive后flag_id对应的计数器增加1；执行asc_sync_block_wait时如果对应的计数器数值为0则阻塞不执行；如果对应的计数器大于0，则计数器减一，同时后续指令开始执行。
- 实现原理/公式：对应CrossCoreSetFlag的模式2——单个AI Core内AIC与全部AIV之间的同步，arrive对应SetFlag。AI Core中的AIC/AIV在完成本核前置位的pipe流水任务后，调用该接口向调度模块上报本核任务已完成。

## 函数原型

<!-- npu="950" id4 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
__aicore__ inline void asc_sync_block_arrive(pipe_t pipe, int64_t flag_id)
```

<!-- end id4 -->
<!-- npu="A3" id5 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
__aicore__ inline void asc_sync_block_arrive(pipe_t pipe, int64_t flag_id)
```

<!-- end id5 -->
<!-- npu="910b" id6 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
__aicore__ inline void asc_sync_block_arrive(pipe_t pipe, int64_t flag_id)
```

<!-- end id6 -->

## 参数说明

**表 1**  参数说明

<!-- npu="950" id7 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 设置这条指令所在的流水类型。 |
| flag_id | 输入 | 事件标号，用于标识同一组同步信号。取值范围为[0,15]。 |

<!-- end id7 -->
<!-- npu="A3" id8 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 设置这条指令所在的流水类型。 |
| flag_id | 输入 | 事件标号，用于标识同一组同步信号。取值范围为[0,15]。 |

<!-- end id8 -->
<!-- npu="910b" id9 -->

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 设置这条指令所在的流水类型。 |
| flag_id | 输入 | 事件标号，用于标识同一组同步信号。取值范围为[0,15]。 |

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
- 使用该同步接口时，需要按照如下规则设置Kernel类型：
  - 在纯Vector/Cube场景下，需设置Kernel类型为KERNEL_TYPE_MIX_AIV_1_0或KERNEL_TYPE_MIX_AIC_1_0。
  - 对于Vector和Cube混合场景，需根据实际情况灵活配置Kernel类型。
- 不建议开发者同时使用该接口和Matmul高阶API，否则会有flag_id冲突的风险。
- 同一flag_id的计数器最多设置15次。

## 调用示例

<!-- npu="950" id13 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// 在Ascend 950PR/Ascend 950DT产品上，无需进行核间同步基地址ffts_addr的设置
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
