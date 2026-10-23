# block、intra、inter和subblock同步

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas训练系列产品：不支持
<!-- end id7 -->

## 功能说明

头文件路径为：`tensor_api/tensor.h`。

本组接口提供四类到达/等待同步能力：

- block同步：实现单个AI Core内AIC与全部AIV之间的同步。
- intra同步：实现单个AI Core内AIC与单个AIV之间的同步。
- inter同步：实现不同AI Core之间的同步。
- subblock同步：实现单个AI Core内subblock（一个group内的从核）之间的同步。

每类同步均提供`arrive`和`wait`两个接口。

## 函数原型

```cpp
__aicore__ inline void sync_block_arrive(pipe_t pipe, int64_t flag_id)
__aicore__ inline void sync_block_wait(pipe_t pipe, int64_t flag_id)

__aicore__ inline void sync_intra_arrive(pipe_t pipe, uint64_t sync_id)
__aicore__ inline void sync_intra_wait(pipe_t pipe, uint64_t sync_id)

__aicore__ inline void sync_inter_arrive(pipe_t pipe, int64_t flag_id)
__aicore__ inline void sync_inter_wait(pipe_t pipe, int64_t flag_id)

__aicore__ inline void sync_subblock_arrive(pipe_t pipe, int64_t flag_id)
__aicore__ inline void sync_subblock_wait(pipe_t pipe, int64_t flag_id)
```

## 参数说明

**表1**  参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| pipe | 输入 | 同步关联的流水，类型为`pipe_t`。 |
| flag_id | 输入 | block、inter或subblock同步标记，用于标识同一组同步信号。取值范围为`[0, 15]`，每个`flag_id`各自拥有独立的4位计数器。 |
| sync_id | 输入 | intra同步标记，用于标识同一组同步信号。每个`sync_id`各自拥有独立的4位计数器。AIC侧拥有32个`sync_id`（0~31），每个AIV侧各拥有16个`sync_id`（0~15）。 |

## 返回值说明

无

## 约束说明

- `sync_block_arrive`与`sync_block_wait`、`sync_inter_arrive`与`sync_inter_wait`、`sync_subblock_arrive`与`sync_subblock_wait`配对使用时，`flag_id`必须完全一致。
- `sync_intra_arrive`与`sync_intra_wait`配对使用时，不要求二者传入相同的`sync_id`，而是要求符合跨核ID映射关系，否则会出现未定义行为。
- 核函数不能使用`__cube__`或`__vector__`函数执行空间限定符，否则硬件不会开启调度模块，无法正常进行核间同步。
- `sync_block_arrive`与`sync_block_wait`、`sync_inter_arrive`与`sync_inter_wait`、`sync_subblock_arrive`与`sync_subblock_wait`支持`flag_id`取值范围为`[0, 15]`，超出范围的值会被按位宽截断处理为低4位。
- `sync_intra_arrive`与`sync_intra_wait`中，AIC的`sync_id`取值范围为`[0, 31]`，超出范围的值会被按位宽截断处理为低5位；AIV的`sync_id`取值范围为`[0, 15]`，超出范围的值会被按位宽截断处理为低4位。
- 每个计数器最多连续累加15次，必须保证计数器的值不超过15，否则触发异常。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

using namespace asc::te;

__global__ __mix__(1, 2) void block_sync_example()
{
    if ASC_IS_AIV {
        // AIV侧数据准备完成后发送同步通知。
        sync_block_arrive(pipe_t::PIPE_MTE3, 0);
    }
    if ASC_IS_AIC {
        // AIC侧等待AIV同步通知。
        sync_block_wait(pipe_t::PIPE_MTE1, 0);
    }
}
```