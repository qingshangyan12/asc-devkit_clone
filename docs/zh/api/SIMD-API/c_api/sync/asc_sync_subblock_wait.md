# asc_sync_subblock_wait

## 产品支持情况

<!-- npu="950" id13 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id13 -->
<!-- npu="A3" id14 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：支持
<!-- end id14 -->
<!-- npu="910b" id15 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：支持
<!-- end id15 -->
<!-- npu="310b" id16 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id16 -->
<!-- npu="310p" id17 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id17 -->
<!-- npu="310p" id18 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id18 -->
<!-- npu="910" id19 -->
- Atlas 训练系列产品：不支持
<!-- end id19 -->

## 功能说明

本接口与[asc_sync_subblock_arrive](asc_sync_subblock_arrive.md)配对使用，实现同一AI Core内全部AIV（[subblock](inter_core_sync_overview.md#zh-cn_topic_0000002542605351_fig52727114018)）之间的同步（[四种核间同步模式](system_sync_overview.md#fig_sync_control_mode)中的模式1），核间同步实现的原理如下：

- 同一AI Core内所有AIV（subblock）都执行`asc_sync_subblock_arrive`后向调度模块发送通知，接着调度模块将各AIV（subblock）对应`flag_id`的计数器增加1。
- 各AIV（subblock）上配对的`asc_sync_subblock_wait`检测到对应`flag_id`的计数器非0后解除阻塞并将计数器减1。

## 函数原型

```c
__aicore__ inline void asc_sync_subblock_wait(pipe_t pipe,
                                              uint8_t flag_id)

__aicore__ inline void asc_sync_subblock_wait(pipe_t pipe,
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

- 必须保证配套使用[asc_sync_subblock_arrive](asc_sync_subblock_arrive.md)和asc_sync_subblock_wait，避免计算核一直处于阻塞阶段。
- pipe支持的流水类型为PIPE_V、PIPE_M、PIPE_MTE1、PIPE_MTE2、PIPE_MTE3、PIPE_FIX，不支持PIPE_ALL和PIPE_S。
- flag_id的值超出取值范围时，会截取最低4位为准。

## 调用示例

`asc_sync_subblock_wait`需与`asc_sync_subblock_arrive`配对使用，具体调用示例请参考[asc_sync_subblock_arrive调用示例](asc_sync_subblock_arrive.md#调用示例)。
