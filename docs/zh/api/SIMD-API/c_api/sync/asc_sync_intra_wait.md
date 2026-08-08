# asc_sync_intra_wait

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

在分离模式下，AIC（Cube Core）与AIV（Vector Core）按group划分，一个group由1个block（主核）和N个subblock（从核）组成，比例1:N。block内同步指同一block内block（AIC）与单个subblock（AIV）之间的同步。

block内同步指令，与[asc_sync_intra_arrive](asc_sync_intra_arrive.md)配合使用，是block内不同Core的流水线同步指令，用于向每个Core的同步寄存器发送同步信号。每个sync_id对应Vector Core与Cube Core中的一个初始值为0的计数器。asc_sync_intra_arrive指令后其他CORE对应的计数器增加1；执行asc_sync_intra_wait时如果对应的计数器数值为0则阻塞不执行；如果对应的计数器大于0，则计数器减一，同时后续指令开始执行。

## 函数原型

```cpp
__aicore__ inline void asc_sync_intra_wait(pipe_t pipe, uint8_t sync_id)
__aicore__ inline void asc_sync_intra_wait(pipe_t pipe, uint64_t sync_id)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 该指令的执行流水线。 |
| sync_id | 输入 | 事件标号。AIC的取值范围为[0, 31]，AIV的取值范围为[0, 15]。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 用户需要确保配套使用（`sync_id`必须按照映射规则对应）asc_sync_intra_arrive和asc_sync_intra_wait，否则会出现未定义行为。
- 由于当使用__vector__和__cube__修饰核函数时，硬件不会开启调度模块，也就无法正常进行核间同步。因此即使是在纯Vector或纯Cube场景下，也不能用__vector__或__cube__修饰核函数。
- pipe支持的流水类型为PIPE_V、PIPE_M、PIPE_MTE1、PIPE_MTE2、PIPE_MTE3、PIPE_FIX、PIPE_S，不支持PIPE_ALL。
- 一个核连续发出的asc_sync_intra_arrive，硬件不保证执行顺序。
- `pipe`标识本指令在本核的执行流水，本接口不阻塞pipe流水中的后续指令。
- 计数器取值范围为[0, 15]，软件须保证每个flag ID的计数器不会溢出，否则触发异常。
- CUBECORE侧共32个sync_id（0~15映射到第一个Vector单元、16~31映射到第二个Vector单元），Vector单元侧各16个sync_id，以上各个核的sync_id映射规则如下：
    - sync_id为0~15的Vector Core 0的asc_sync_intra_arrive指令会被映射到Cube Core的0~15的sync_id。 
    - sync_id为0~15的Vector Core 1的asc_sync_intra_arrive指令会被映射到Cube Core的16~31的sync_id。 
    - sync_id为0~15的Cube Core的asc_sync_intra_arrive指令会被映射到Vector Core 0的0~15的sync_id。 
    - sync_id为16~31的Cube Core的asc_sync_intra_arrive指令会被映射到Vector Core 1的0~15的sync_id。 

## 调用示例

```cpp
// 场景：AIC执行asc_sync_intra_wait等待1个AIV，对应AIV执行asc_sync_intra_arrive通知。
// 注意：asc_sync_intra_wait与asc_sync_intra_arrive必须分别在不同Core上调用，同一Core连续调用无法实现跨核同步。

// ---- AIC核代码 ----
uint64_t sync_id = 1;
asc_sync_intra_wait(PIPE_FIX, sync_id);   // AIC执行wait，等待AIV的arrive使计数器+1后解除阻塞

// ---- AIV核代码 ----
asc_sync_intra_arrive(PIPE_V, sync_id);   // AIV执行arrive，AIC对应sync_id计数器+1
```
