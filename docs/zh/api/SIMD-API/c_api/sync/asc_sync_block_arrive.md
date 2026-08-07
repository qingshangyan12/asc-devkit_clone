# asc_sync_block_arrive

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

在分离模式下，AIC（Cube Core）与AIV（Vector Core）按group划分，一个group由1个block（主核）和N个subblock（从核）组成，比例1:N。组内block和subblock间同步指同一group内block（AIC）与subblock（AIV）之间的同步。

和[asc_sync_block_wait](asc_sync_block_wait.md)配合使用（通过flag_id关联），用于组内block和subblock间的信号同步。通过向核间同步寄存器发送同步信号，标识当前pipeline已到达flag_id对应的同步点。每个flag_id对应一个初始值为0的计数器，执行asc_sync_block_arrive后flag_id对应的计数器增加1；执行asc_sync_block_wait时如果对应的计数器数值为0则阻塞不执行；如果对应的计数器大于0，则计数器减一，同时后续指令开始执行。

## 函数原型

```cpp
__aicore__ inline void asc_sync_block_arrive(pipe_t pipe, int64_t flag_id)
```

## 参数说明

**表1** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :---  | :--- | :--- |
| pipe | 输入 | 设置这条指令所在的流水类型。|
| flag_id | 输入 | 事件标号，用于标识同一组同步信号。取值范围为[0,15]。|

## 返回值说明

无

## 流水类型

PIPE_S

## 约束说明

- 由于当使用__vector__和__cube__修饰核函数时，硬件不会开启调度模块，也就无法正常进行核间同步。因此即使是在纯Vector/Cube场景下，也不能用__vector__和__cube__修饰核函数。

- 同一flag_id的计数器最多设置15次。
- pipe支持的流水类型为PIPE_V、PIPE_M、PIPE_MTE1、PIPE_MTE2、PIPE_MTE3、PIPE_FIX，不支持PIPE_ALL和PIPE_S。
- flag_id的值超出取值范围时，会截取最低4位为准。
- 一个核连续发出的asc_sync_block_arrive，硬件不保证执行顺序。

- 以下

- 本接口需与`asc_sync_block_wait`配对使用，配对的两条调用其`flag_id`必须完全一致，否则同步关系不成立。本指令在到达核设置同步标志，`asc_sync_block_wait`在等待核阻塞直至对应`flag_id`的同步点达成，二者构成跨核软同步的执行期闭环。
- 本接口固定实现单AI Core内AIC与全部AIV之间的全核同步（mode 2）。在该模式下，AIC 1次调用本接口对应M个AIV各1次配对的`asc_sync_block_wait`；M个AIV各1次调用本接口对应AIC 1次配对的`asc_sync_block_wait`。到达次数与等待次数需按上述语义对齐，否则同步计数器残留，后续同步语义错乱。
- `pipe`不可取`PIPE_S`与`PIPE_ALL`，否则触发异常。AIC中合法取值为`PIPE_M`、`PIPE_MTE1`、`PIPE_MTE2`、`PIPE_MTE3`、`PIPE_FIX`，AIV中合法取值为`PIPE_MTE2`、`PIPE_MTE3`，需按当前核类型选择合法流水，落入非法枚举触发异常。
- `pipe`标识本指令在本核的执行流水，本接口不阻塞同一流水中的后续指令，仅设置一个同步标志，待本指令之前的所有指令提交后才触发对端核配对的`asc_sync_block_wait`。
- `flag_id`取值范围为[0, 15]，需与配对的`asc_sync_block_wait`调用中的`flag_id`完全一致，否则两端的同步计数器无法对齐，同步阻塞无法释放。
- `flag_id`传入大于15的值会被按位宽截断处理为低4位（如16→0、17→1），硬件不报错。截断后实际生效值与预期不符，导致同步关系错乱，可能与其他`flag_id`的同步点冲突或使后续指令提前放行。参数类型为`uint16_t`（0~2^16−1），仅截取低4位。
- 本接口仅在AI Core内部建立AIC与AIV之间的同步依赖，不涉及Global Memory读写，无地址对齐与容量上限约束。

### 流水组合约束

- `pipe`的合法取值由当前核类型（AIC或AIV）决定，需选择硬件支持的流水，否则`pipe`取值落入非法枚举触发异常。AIC与AIV中可用的核间同步流水组合参见核间同步能力概述，需按当前核类型选择合法组合。
- 编译器在C API编程方式下不会自动插入核间同步指令，跨核数据依赖需开发者通过本接口与`asc_sync_block_wait`手动建立同步点。

## 调用示例

```cpp
// Host侧调用接口aclrtGetHardwareSyncAddr获取核间同步基地址ffts_addr
uint64_t config = *(__gm__ uint64_t*)ffts_addr;
asc_set_ffts_base_addr(config);
int64_t flag_id = 1;
asc_sync_block_arrive(PIPE_V, flag_id);
asc_sync_block_wait(PIPE_V, flag_id);  
```

<!-- npu="950" id10 -->

```cpp
// 在Ascend 950PR/Ascend 950DT产品上，无需进行核间同步基地址ffts_addr的设置
int64_t flag_id = 1;
asc_sync_block_arrive(PIPE_V, flag_id);
asc_sync_block_wait(PIPE_V, flag_id);  
```
<!-- end id10 -->
