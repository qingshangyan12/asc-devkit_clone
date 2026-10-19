# block、intra、inter和subblock同步

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR&950DT系列产品：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3系列产品：支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2系列产品：支持
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

头文件路径为：`"tensor_api/arch/sync/block_sync.h"`，命名空间为`asc::te`。

本组接口提供block、intra、inter和subblock四类到达/等待同步能力。每个同步范围均提供`arrive`和`wait`两个高度对称的接口。

接口与C API的对应关系如下：

**表1** 接口与C API对应关系

| Tensor API | C API |
| :--- | :--- |
| `sync_block_arrive` | `asc_sync_block_arrive` |
| `sync_block_wait` | `asc_sync_block_wait` |
| `sync_intra_arrive` | `asc_sync_intra_arrive` |
| `sync_intra_wait` | `asc_sync_intra_wait` |
| `sync_inter_arrive` | `asc_sync_inter_arrive` |
| `sync_inter_wait` | `asc_sync_inter_wait` |
| `sync_subblock_arrive` | `asc_sync_subblock_arrive` |
| `sync_subblock_wait` | `asc_sync_subblock_wait` |

## 函数原型

```cpp
namespace asc {
namespace te {
__aicore__ inline void sync_block_arrive(pipe_t pipe, int64_t flag_id);
__aicore__ inline void sync_block_wait(pipe_t pipe, int64_t flag_id);

__aicore__ inline void sync_intra_arrive(pipe_t pipe, uint64_t sync_id);
__aicore__ inline void sync_intra_wait(pipe_t pipe, uint64_t sync_id);

__aicore__ inline void sync_inter_arrive(pipe_t pipe, int64_t flag_id);
__aicore__ inline void sync_inter_wait(pipe_t pipe, int64_t flag_id);

__aicore__ inline void sync_subblock_arrive(pipe_t pipe, int64_t flag_id);
__aicore__ inline void sync_subblock_wait(pipe_t pipe, int64_t flag_id);
}
}
```

## 参数说明

**表2** 参数说明

| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| pipe | 输入 | 同步关联的流水类型，类型为`pipe_t`。 |
| flag_id | 输入 | block、inter或subblock同步标记，类型为`int64_t`。合法范围和截断行为继承对应C API。 |
| sync_id | 输入 | intra同步标记，类型为`uint64_t`。合法范围继承对应C API。 |

## 返回值说明

无

## 流水类型

`PIPE_S`

## 约束说明

- `arrive`与`wait`必须按同步协议配对使用，并使用一致的同步标记。
- 产品支持的核函数类型、pipe取值、同步计数器范围和内存可见性约束均继承对应C API。
- Tensor API封装不改变底层同步行为，也不额外插入流水同步。

## 调用示例

```cpp
#include "tensor_api/tensor.h"

__global__ __mix__(1, 2) void block_sync_example()
{
    if ASC_IS_AIV {
        // AIV侧数据准备完成后发送同步通知。
        asc::te::sync_block_arrive(pipe_t::PIPE_MTE3, 0);
    }
    if ASC_IS_AIC {
        // AIC侧等待AIV同步通知。
        asc::te::sync_block_wait(pipe_t::PIPE_MTE1, 0);
    }
}
```

