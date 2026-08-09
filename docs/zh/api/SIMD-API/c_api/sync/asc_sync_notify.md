# asc_sync_notify

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

头文件路径：`"c_api/sync/sync.h"`。

与`asc_sync_wait`配对使用，用于实现AI Core内部不同流水之间的同步控制，asc_sync_notify和asc_sync_wait各自的功能如下：
    - asc_sync_notify：当源流水的前序指令的所有读写操作都完成之后，当前指令开始执行，并将硬件中的对应标志位设置为1。asc_sync_notify只是设置硬件中的对应标志位，并不会阻塞源流水中的下一个指令。
    - asc_sync_wait：当目的流水执行到该指令时，如果发现硬件中对应标志位为0，目的流水的后续指令将一直被阻塞；如果发现硬件中对应标志位为1，则将硬件中对应标志位设置为0，同时目的流水的后续指令开始执行。

## 函数原型

```c
__aicore__ inline void asc_sync_notify(pipe_t pipe,
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

针对不同产品，AIC与AIV中支持的“源流水-目的流水”的组合不同，具体请参考[支持的多流水同步组合]()。

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 相同源流水、相同目标流水、相同id下，连续使用asc_sync_notify会引发未定义行为。
- 本接口需与`asc_sync_wait`配对使用，配对的两条调用其`pipe`、`tpipe`、`id`三个参数必须完全一致，否则同步关系不成立。
- `pipe`与`tpipe`均不可取`PIPE_ALL`，否则触发异常。
- `id`取值范围为0~7，每对`pipe`与`tpipe`组合各自拥有0~7共8个独立的同步事件ID。例如`PIPE_M`与`PIPE_V`的组合和`PIPE_V`与`PIPE_MTE3`的组合可同时使用相同的`id`值而互不干扰。





**已验证：1、2、3、6、9**
- 本接口需与`asc_sync_wait`配对使用，配对的两条调用其`pipe`、`tpipe`、`id`三个参数必须完全一致，否则同步关系不成立。本指令设置标志位，`asc_sync_wait`消费标志位，二者构成跨流水软同步的执行期闭环。<span style="color:green">**（pass）**</span>
- `pipe`与`tpipe`均不可取`PIPE_ALL`，否则触发异常。<span style="color:green">**（pass，这个描述是在isa中有对应的）**</span>
- `id`取值范围为0~7，每对`pipe`与`tpipe`组合各自拥有0~7共8个独立的同步事件ID<span style="color:green">**（这个描述是在isa中有对应的）**</span>。例如`PIPE_M`与`PIPE_V`的组合和`PIPE_V`与`PIPE_MTE3`的组合可同时使用相同的`id`值而互不干扰。<span style="color:green">**（pass）**</span>
- 不允许出现两个相同`pipe`、`tpipe`、`id`三元组的`asc_sync_notify`调用却没有对应的`asc_sync_wait`消费，否则同步标志在析构期残留，使用错误。。<span style="color:red">**（需重点关注，手动设计样例）**</span><span style="color:green">**（这个描述是在isa中有对应的，之前洋哥在字节现场遇到过的问题是连续调用相同的pipe、id是未定义行为，可能触发卡死）**</span>
- AIC与AIV之间不支持使用本接口与`asc_sync_wait`进行同步，可能触发硬件未定义行为。AIC与AIV之间的同步属于核间同步，需使用`asc_sync_block_arrive`与`asc_sync_block_wait`完成。
- 本接口仅在AI Core内部建立跨流水依赖，不涉及Global Memory读写，无地址对齐与容量上限约束。。<span style="color:red">**（莫名其妙的约束）**</span>
- 本接口对同一流水中的后续指令不产生阻塞，仅设置一个同步标志位，待本指令之前的所有指令提交后才触发对应的`asc_sync_wait`。<span style="color:green">**（isa中的描述）**</span>

### 流水组合约束

- 源流水与目标流水的合法组合由当前核类型（AIC或AIV）决定，需选择硬件支持的流水对，否则`pipe`或`tpipe`取值落入非法枚举触发异常。AIC中可用的核内同步流水组合参见核内同步能力概述。<span style="color:red">**（需加跳转）**</span>，AIV中可用的核内同步流水组合参见同一文档的AIV同步组合表。<span style="color:green">**（pass）**</span>
- 在NPU架构3510下，本接口仍可用于兼容场景的跨流水依赖同步；多流水依赖也可改用`asc_lock`与`asc_unlock`按"锁定-释放"方式处理。<span style="color:green">**（pass）**</span>
- 编译器在C API编程方式下会自动插入部分流水同步（具体范围见核内同步关键特性说明），开发者不应重复插入编译器已自动插入的同步指令，否则可能导致程序逻辑异常或性能下降。。<span style="color:red">**（需重点关注，手动设计样例）**</span>

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
