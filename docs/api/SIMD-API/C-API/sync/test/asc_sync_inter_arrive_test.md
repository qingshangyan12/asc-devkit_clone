# asc_sync_inter_arrive

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
- 核心功能：和[asc_sync_inter_wait](asc_sync_inter_wait.md)配合使用（通过flag_id关联），用于组间block的信号同步。
- 实现原理：通过向核间同步寄存器发送同步信号，标识当前pipeline已到达flag_id对应的同步点。每个flag_id对应一个初始值为0的计数器，执行asc_sync_inter_arrive后flag_id对应的计数器增加1；执行asc_sync_inter_wait时如果对应的计数器数值为0则阻塞不执行；如果对应的计数器大于0，则计数器减一，同时后续指令开始执行。

## 函数原型

```cpp
__aicore__ inline void asc_sync_inter_arrive(pipe_t pipe, int64_t flag_id)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 设置这条指令所在的流水类型。|
| flag_id | 输入 | 事件标号，用于标识同一组同步信号。取值范围为[0,15]。|

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：无
- 同步资源占用：每个flag_id对应一个计数器，计数器计数范围为0-15。
- 资源占用与参数/数据量关系：计数器资源占用与flag_id的使用数量相关，同一flag_id的计数器最多设置15次。

## 约束说明

- 不建议开发者同时使用该接口和Matmul高阶API，否则会有flag_id冲突的风险。
- 同一flag_id的计数器最多设置15次。
- 必须保证配套使用asc_sync_inter_arrive和[asc_sync_inter_wait](asc_sync_inter_wait.md)，否则会出现未定义行为。
- 需要保证每个flag_id的计数器不溢出，否则将引发异常。

## 调用示例

<!-- npu="A3,910b" id8 -->
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品、Atlas A2 训练系列产品/Atlas A2 推理系列产品：

```cpp
// Host侧调用接口aclrtGetHardwareSyncAddr获取核间同步基地址ffts_addr
uint64_t config = *(__gm__ uint64_t*)ffts_addr;
asc_set_ffts_base_addr(config);
int64_t flag_id = 1;
asc_sync_inter_arrive(PIPE_S, flag_id);
asc_sync_inter_wait(PIPE_S, flag_id);  
```
<!-- end id8 -->

<!-- npu="950" id9 -->
针对Ascend 950PR/Ascend 950DT：

```cpp
// 在Ascend 950PR/Ascend 950DT产品上，无需进行核间同步基地址ffts_addr的设置
int64_t flag_id = 1;
asc_sync_inter_arrive(PIPE_S, flag_id);
asc_sync_inter_wait(PIPE_S, flag_id);  
```
<!-- end id9 -->
