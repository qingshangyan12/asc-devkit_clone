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

- 接口声明文件：`"c_api/sync/sync.h"`
- 核心功能：和asc_sync_block_arrive配合使用（通过flag_id关联），用于等待所有同步对象到达flag_id对应的同步点
- 实现原理：对应基础API SyncAll/IBWait，如果flag_id所对应的计数器数值为0则阻塞不执行；如果对应的计数器大于0，则计数器减一，同时后续指令开始执行。

## 函数原型

```cpp
__aicore__ inline void asc_sync_block_wait(pipe_t pipe, uint8_t flag_id)
__aicore__ inline void asc_sync_block_wait(pipe_t pipe, int64_t flag_id)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 需要执行同步指令的流水类型。|
| flag_id | 输入 | 事件标号，取值范围[0,15]。|

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：无
- 同步资源占用：占用一个flag_id资源（取值范围[0,15]，每个flag_id计数器范围为[0,15]）
- 资源占用与参数/数据量关系：不涉及

## 约束说明

- flag_id的取值范围为0至15，每个flag_id的计数器范围为0至15。
- 必须保证每个flag_id的四位计数器不溢出，否则将引发异常。
- 必须保证相同的flag_id在同一时间仅被一条流水线使用。
- 使用该接口进行多核控制时，算子调用时指定的逻辑AI Core核数numBlocks必须保证不大于实际运行该算子的AI处理器核数，否则框架进行多轮调度时会插入异常同步，导致Kernel"卡死"现象。

## 调用示例

<!-- npu="950" id8 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// 在Ascend 950PR/Ascend 950DT产品上，无需进行核间同步基地址ffts_addr的设置
int64_t flag_id = 1;
asc_sync_block_arrive(PIPE_S, flag_id);
asc_sync_block_wait(PIPE_S, flag_id);  
```
<!-- end id8 -->
<!-- npu="A3" id9 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品：

```cpp
// Host侧调用接口aclrtGetHardwareSyncAddr获取核间同步基地址ffts_addr
uint64_t config = *(__gm__ uint64_t*)ffts_addr;
asc_set_ffts_base_addr(config);
int64_t flag_id = 1;
asc_sync_block_arrive(PIPE_S, flag_id);
asc_sync_block_wait(PIPE_S, flag_id);  
```
<!-- end id9 -->
<!-- npu="910b" id10 -->
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
// Host侧调用接口aclrtGetHardwareSyncAddr获取核间同步基地址ffts_addr
uint64_t config = *(__gm__ uint64_t*)ffts_addr;
asc_set_ffts_base_addr(config);
int64_t flag_id = 1;
asc_sync_block_arrive(PIPE_S, flag_id);
asc_sync_block_wait(PIPE_S, flag_id);  
```
<!-- end id10 -->
