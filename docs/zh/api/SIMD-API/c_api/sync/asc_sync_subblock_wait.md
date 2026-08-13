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

头文件路径：`"c_api/sync/sync.h"`。

亚块（subblock）间同步等待接口，与配套到达通知接口配合使用，用于同一group内N个亚块之间通过flag ID关联建立同步点。group由1个block与N个亚块组成（N≥1），亚块间比例为1：N。本接口在所在核的指定流水线上插入一个等待点：当对应flag ID的同步计数器为0时，本接口所在流水的后续指令被阻塞；当计数器变为非0时，本接口放行后续指令并将计数器减1。同步计数器由配套到达通知接口在参与同步的亚块执行到对应同步点时累加，调度模块感知到全部参与亚块到达后将计数器加1。

本接口需与`asc_sync_subblock_arrive`配合使用。配套到达通知接口负责发送同步信息（包含同步模式与flag ID）到核间同步调度模块并累加计数器，本接口负责阻塞等待计数器变为非0。两者通过相同的flag ID关联，同步模式由配套到达通知接口一侧指定为亚块间同步模式，本接口不传同步模式，仅通过flag ID与pipe与配套到达通知接口对齐。

本接口为同步原语，AIC与AIV上均可调用，无AIC/AIV守护限制。

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
