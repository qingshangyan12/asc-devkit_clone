# asc_sync_inter_wait

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
<!-- npu="310b" id5 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id6 -->
<!-- npu="310p" id7 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id7 -->
<!-- npu="910" id8 -->
- Atlas 训练系列产品：不支持
<!-- end id8 -->

## 功能说明

头文件路径：`"c_api/sync/sync.h"`。

核间同步等待接口，与`asc_sync_inter_arrive`配对使用，用于两个核（AIC与AIV，或两个AIV之间）之间的同步：被等待核执行`asc_sync_inter_arrive`在其前序流水任务完成后向FFTS（Fast Task Flow Synchronizer）发送到达通知并递增对应flag_id的计数器；本接口在等待核上执行，阻塞`pipe`流水线的后续指令，直到对应flag_id的FFTS计数器非零时将计数器减一、放行后续指令。两个接口通过相同的`flag_id`关联，构成一对核间同步原语。本接口与`asc_sync_inter_arrive`配套使用。

## 函数原型

```c
__aicore__ inline void asc_sync_inter_wait(pipe_t pipe,
                                           uint8_t flag_id)

__aicore__ inline void asc_sync_inter_wait(pipe_t pipe,
                                           int64_t flag_id)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 需要执行同步指令的流水类型。|
| flag_id | 输入 | 事件标号，取值范围[0,15]。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- flag_id的取值范围为0至15，每个flag_id的计数器范围为0至15。
- 必须保证每个flag_id的计数器不溢出，否则将引发异常。
- 必须保证相同的ID在同一时间仅被一条流水线等待。

- 必须保证配套使用[asc_sync_inter_arrive](asc_sync_inter_arrive.md)和asc_sync_inter_wait，避免计算核一直处于阻塞阶段。
- pipe支持的流水类型为PIPE_V、PIPE_M、PIPE_MTE1、PIPE_MTE2、PIPE_MTE3、PIPE_FIX，不支持PIPE_ALL和PIPE_S。
- flag_id的值超出取值范围时，会截取最低4位为准。
- 使用该接口时，建议开启batchmode模式，使算子独占全部所需核资源，否则在多流并发场景下（≥2条执行流、≥2个算子并发、核数总和超过物理核数）可能导致死锁。

## 调用示例

`asc_sync_inter_wait`需与`asc_sync_inter_arrive`配对使用，具体调用示例请参考[asc_sync_inter_arrive调用示例](asc_sync_inter_arrive.md#调用示例)。
