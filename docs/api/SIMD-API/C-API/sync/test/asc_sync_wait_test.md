# asc_sync_wait

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
- 核心功能：等待特定的同步标志，阻塞当前流水线直到条件满足
- 实现原理：对应基础API WaitFlag，当目的流水执行到该指令时，如果发现硬件中对应标志位为0，目的流水的后续指令将一直被阻塞；如果发现硬件中对应标志位为1，则将硬件中对应标志位设置为0，同时目的流水的后续指令开始执行。

## 函数原型

```cpp
__aicore__ inline void asc_sync_wait(pipe_t pipe, pipe_t tpipe, event_t id)
```

## 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| pipe | 输入 | 源流水线类型。需传入编译期常量。 |
| tpipe | 输入 | 目标流水线类型。需传入编译期常量。 |
| id | 输入 | 同步ID。取值范围为[0, 7]。 |

## 返回值说明

无

## 流水类型

PIPE_S

## 资源占用情况

- 临时空间占用：无
- 同步资源占用：占用一个event ID资源
- 资源占用与参数/数据量关系：不涉及

## 约束说明

- asc_sync_notify和asc_sync_wait必须成对使用，且asc_sync_notify和asc_sync_wait的参数必须完全一致（包括pipe、tpipe和id）。如果不匹配，会引发timeout问题。
- asc_sync_wait会阻塞目的流水的后续指令，直到硬件中对应标志位为1。
- 相同流水、相同id下，连续使用asc_sync_notify会引发未定义行为，此时再执行asc_sync_pipe会出现卡死现象。

## 调用示例

```cpp
// 本例中total_length指参与计算的数据总长度。src0_gm，src1_gm，dst_gm是外部输入的float类型的源操作数、目的操作数，指向GM内存空间。
constexpr uint32_t total_length = 128;
__ubuf__ float src0[total_length];
__ubuf__ float src1[total_length];
__ubuf__ float dst[total_length];

asc_copy_gm2ub(src0, src0_gm, total_length * sizeof(float));
asc_copy_gm2ub(src1, src1_gm, total_length * sizeof(float));

// 同步操作：数据搬运操作（GM到UB，PIPE_MTE2流水）完成后才能启动计算操作（PIPE_V流水）。
asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);  // EVENT_ID0为外部传入的同步ID。
asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);  // EVENT_ID0为外部传入的同步ID。

asc_add(dst, src1, src0, total_length);

// 同步操作：计算操作（PIPE_V流水）完成后才能启动数据搬运操作（UB到GM，PIPE_MTE3流水）。
asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);  // EVENT_ID0为外部传入的同步ID。
asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);  // EVENT_ID0为外部传入的同步ID。

asc_copy_ub2gm(dst_gm, dst, total_length * sizeof(float));
```
