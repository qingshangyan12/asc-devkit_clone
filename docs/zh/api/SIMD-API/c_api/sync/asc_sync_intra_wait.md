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

头文件路径：`"c_api/sync/sync.h"`。

块内（intra-block）跨核同步等待原语，与配套投递接口配对使用，用于在mix-mode block内部建立CubeCore与VecCore之间的流水同步依赖。本接口阻塞`pipe`指定流水的后续指令，直到对端核通过配套投递接口投递到对应同步ID上的计数器值大于0；命中后将该计数器减1并放行后续指令。

本接口为非阻塞语义的反向操作——配套投递接口在其前序指令提交后将对端核对应ID的4位计数器加1且不阻塞自身流水，本接口则在对应计数器为0时阻塞本流水的后续指令，直到对端投递使计数器变为非零，随后将计数器减1放行，从而构成一对跨核"投递-等待"同步闭环。需要与配套投递接口配合使用。

## 函数原型

```c
__aicore__ inline void asc_sync_intra_wait(pipe_t pipe,
                                           uint8_t sync_id)
__aicore__ inline void asc_sync_intra_wait(pipe_t pipe,
                                           uint64_t sync_id)
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

- 需要保证每一个ID的4位计数器不会溢出，否则会出现异常。
- 需要保证相同的ID只会被一个流水线控制。
- 每个ID对应一个4位计数器，特定CORE的同步ID计数器可通过其他CORE发起的asc_sync_intra_arrive递增；也可通过本身发起的asc_sync_intra_wait递减。 
- 三个CORE的ID映射规则如下：
    - ID为0~15的Vector Core 0的asc_sync_intra_arrive指令会被映射到Cube Core的0~15的ID。 
    - ID为0~15的Vector Core 1的asc_sync_intra_arrive指令会被映射到Cube Core的16~31的ID。 
    - ID为0~15的Cube Core的asc_sync_intra_arrive指令会被映射到Vector Core 0的0~15的ID。 
    - ID为16~31的Cube Core的asc_sync_intra_arrive指令会被映射到Vector Core 1的0~15的ID。 

- 必须保证配套使用[asc_sync_intra_arrive](asc_sync_intra_arrive.md)和asc_sync_intra_wait，避免计算核一直处于阻塞阶段。
- pipe支持的流水类型为PIPE_V、PIPE_M、PIPE_MTE1、PIPE_MTE2、PIPE_MTE3、PIPE_FIX、PIPE_S，不支持PIPE_ALL。

## 调用示例

`asc_sync_intra_wait`需与`asc_sync_intra_arrive`配对使用，具体调用示例请参考[asc_sync_intra_arrive调用示例](asc_sync_intra_arrive.md#调用示例)。
