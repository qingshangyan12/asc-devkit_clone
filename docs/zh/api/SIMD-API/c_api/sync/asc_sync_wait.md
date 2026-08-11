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

与`asc_sync_notify`配对使用，用于实现AI Core内部不同流水之间的同步控制，asc_sync_notify和asc_sync_wait各自的功能如下：
    - asc_sync_notify：当源流水的前序指令的所有读写操作都完成之后，当前指令开始执行，并将硬件中的对应标志位设置为1。asc_sync_notify只是设置硬件中的对应标志位，并不会阻塞源流水中的下一个指令。
    - asc_sync_wait：当目的流水执行到该指令时，如果发现硬件中对应标志位为0，目的流水的后续指令将一直被阻塞；如果发现硬件中对应标志位为1，则将硬件中对应标志位设置为0，同时目的流水的后续指令开始执行。

## 函数原型

```c
__aicore__ inline void asc_sync_wait(pipe_t pipe,
                                     pipe_t tpipe,
                                     event_t id)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
|---|---|---|
| `pipe` | 输入 | 源流水类型，即"等待哪条流水的前序指令完成"。取值范围为`pipe_t`枚举：`PIPE_S`、`PIPE_V`、`PIPE_M`、`PIPE_MTE1`、`PIPE_MTE2`、`PIPE_MTE3`、`PIPE_FIX`。不可取`PIPE_ALL`，否则触发异常。 |
| `tpipe` | 输入 | 目标流水类型，即"解除哪条流水的`asc_sync_wait`阻塞"。取值范围与`pipe`相同，为`pipe_t`枚举。不可取`PIPE_ALL`，否则触发异常。 |
| `id` | 输入 | 同步事件ID，每对`pipe`与`tpipe`组合各自拥有0~7共8个独立的同步事件ID。取值范围为`EVENT_ID0`~`EVENT_ID7`（即0~7）。 |

针对不同产品，AIC与AIV中支持的“源流水-目的流水”的组合不同，具体请参考[核内同步概述](intra_core_sync_overview.md)。

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- AIC与AIV中均可调用本接口。
- 相同源流水、相同目标流水、相同id下，连续使用asc_sync_notify会引发未定义行为。
- 本接口需与`asc_sync_notify`配对使用，配对的两条调用其`pipe`、`tpipe`、`id`三个参数必须完全一致，否则同步关系不成立。
- `pipe`与`tpipe`均不可取`PIPE_ALL`，否则触发异常。
- `id`取值范围为0~7，每对`pipe`与`tpipe`组合各自拥有0~7共8个独立的同步事件ID。例如`PIPE_M`与`PIPE_V`的组合和`PIPE_V`与`PIPE_MTE3`的组合可同时使用相同的`id`值而互不干扰。
- 本接口仅在AI Core内部建立跨流水依赖，不涉及Global Memory读写，无地址对齐与容量上限约束。。<span style="color:red">**（莫名其妙的约束）**</span>
- 本接口只阻塞其所在流水线（`tpipe`）的后续指令，不影响其他流水线指令的执行；阻塞期间 `pipe` 流水线可继续执行后续指令。
<!-- npu="950" id8 -->
- 在[NPU架构3510](../../../../guide/编程指南/语言扩展层/SIMD-BuiltIn关键字.md#npu-arch)上，`asc_lock`与`asc_unlock`组合也可实现多流水同步。与`asc_sync_notify`和`asc_sync_wait`相比，该组合内聚性更强、与其它流水线解耦，可简化反向同步逻辑。
<!-- end id8 -->

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
