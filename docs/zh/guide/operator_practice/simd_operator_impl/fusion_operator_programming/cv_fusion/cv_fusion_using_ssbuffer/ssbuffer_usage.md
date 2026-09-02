# SSBuffer使用说明

## 概述

SSBuffer是Ascend 950PR/Ascend 950DT AI Core内用于AIC和AIV间通信的物理存储单元。它适合承载短小的控制消息、状态位和地址描述符，使AIV与AIC在同一个Mix任务中交换控制信息时无需经过GM。

SSBuffer不是GM、UB或L1 Buffer的替代品，也不适合保存大块中间计算结果。对UB到L1 Buffer的数据搬运，`ENABLE_CV_COMM_VIA_SSBUF`开启后会选择硬件通道；该过程由基础API内部实现，用户通常不需要也不应把待搬运的数据手工放入`__ssbuf__`地址空间。

## 规格和约束

| 项目 | 说明 |
| --- | --- |
| 支持产品 | Ascend 950PR、Ascend 950DT（`dav-3510`） |
| 总容量 | 3KB |
| 非Mix模式 | AIC、AIV0、AIV1各自独占1KB |
| `__mix__(1, 2)`模式 | AIC、AIV0、AIV1共享全部3KB |
| 对齐要求 | 仅以32B对齐地址进行读写 |
| 初始内容 | 不保证为0，使用前必须由协议指定的唯一初始化方清理 |
| 任务约束 | AIC和AIV必须启动同一个任务；不同任务不能访问同一SSBuffer |
| 越界访问 | 超过3KB末端会产生异常，不能用探测式访问确认容量 |

基础API通过`AscendC::GetSsbufBaseAddr()`取得基地址。NPU侧返回的是`__ssbuf__`地址空间中的零基址，不能把该数值当作普通GM地址；必须保留`__ssbuf__`指针限定符。C API没有同名获取接口，NPU侧可将`__ssbuf__`地址空间的零地址解释为SSBuffer基址，但仍须遵守相同约束。

```cpp
__ssbuf__ void* base = AscendC::GetSsbufBaseAddr();
__ssbuf__ uint8_t* bytes = reinterpret_cast<__ssbuf__ uint8_t*>(base);
```

## 使用场景

### 使用显式SSBuffer协议

当一个Mix任务需要在AIV和AIC之间传递少量自定义控制消息，例如分片就绪通知、短队列命令或地址/长度描述符时，可建立自定义SSBuffer协议。协议必须明确：存储分区、发布者、消费者、消息有效位、确认方式和退出条件。

配套[AIV/AIC环形队列样例](../../../../../../../../examples/01_simd_cpp_api/03_basic_api/05_sync_control/ssbuf_aiv_aic_comm)使用两条128B槽位队列：每个AIV独占8个槽，AIV写入消息体后置位`head`中的valid位，AIC消费后清零消息头作为ACK。

### 使用框架内置的SSBuffer路径

以下场景应通过公开API和编译选项使用SSBuffer，而不是直接包含内部KFC头文件或构造KFC消息：

- 在Ascend 950PR/Ascend 950DT上使用UB到L1 Buffer的`DataCopy`硬通道。
- 使用内部依赖SSBuffer的Matmul高阶API新增功能。

此类场景在构建时设置`-DENABLE_CV_COMM_VIA_SSBUF=true`。该开关选择框架内部通信/数据通路；它不是直接调用`GetSsbufBaseAddr()`的前提，也不能为自定义协议分配额外空间。

## 调用流程

以下代码节选自环形队列样例，展示一个最小的AIV发布、AIC消费流程。示例以`__mix__(1, 2)`运行，因此两个AIV和一个AIC共享3KB空间。

### 1. 规划32B对齐布局

消息槽按128B对齐，工作区为2KB，保留其余空间不使用。`static_assert`在编译期阻止槽位大小或总空间意外变化。

```cpp
struct alignas(128) CommMessage {
    volatile uint32_t head;
    uint32_t reserved0;
    uint64_t sequence;
    uint64_t value;
    uint64_t checksum;
    uint64_t reserved[12];
};

struct SsbufWorkspace {
    CommMessage queue[2][8];
};

static_assert(sizeof(CommMessage) == 128);
static_assert(sizeof(SsbufWorkspace) == 2048);
```

### 2. 清理并建立启动依赖

SSBuffer可能遗留有效位。AIC作为唯一初始化方清除所有消息头，再以mode 2核间同步放行两个AIV。AIV不能在`CrossCoreWaitFlag`返回前检查或写入队列。

```cpp
if ASCEND_IS_AIC {
    InitQueues(workspace); // 将每个消息槽的head清零
    AscendC::CrossCoreSetFlag<2, PIPE_MTE1>(READY_FLAG_ID);
}

if ASCEND_IS_AIV {
    AscendC::CrossCoreWaitFlag<2>(READY_FLAG_ID);
}
```

该同步只建立任务启动顺序。它不能替代后续的valid/ACK轮询，也不能替代数据搬运的流水同步。

### 3. AIV发布消息

发送方先等待所属槽位空闲，写入所有消息体字段，插入编译器屏障，最后写入带valid位的`head`。最后一次写入是发布动作，不能提前到消息体写入之前。

```cpp
while ((message->head & VALID_MASK) != 0) {
    CompilerBarrier();
}

message->sequence = sequence;
message->value = value;
message->checksum = checksum;
CompilerBarrier();
message->head = (FUNCTION_ID << 16) | VALID_MASK | sender;
```

`CompilerBarrier()`只限制编译器重排，不产生核间同步或缓存刷写；AIV/AIC的共享可见性仍依赖SSBuffer语义和协议中的发布顺序。

### 4. AIC消费并确认

AIC轮询valid位。观察到valid后读取消息体，完成处理后清除消息头的前8B。清零是ACK，发送方仅能在观察到槽位无valid后复用该槽。

```cpp
uint32_t head = message->head;
while ((head & VALID_MASK) == 0) {
    CompilerBarrier();
    head = message->head;
}

const uint64_t value = message->value;
// 使用value完成AIC侧处理。
CompilerBarrier();
*reinterpret_cast<__ssbuf__ uint64_t*>(message) = 0;
```

## 设计检查表

1. 仅在支持的`dav-3510`产品和同一个Mix任务内使用显式SSBuffer协议。
2. 定义不超过可用分区的32B对齐数据布局，并用`alignas`和`static_assert`固化大小。
3. 指定唯一初始化方，在每次任务启动时清理valid/ready状态。
4. 使用核间同步建立初始化完成的启动顺序；对持续通信另行设计valid和ACK。
5. 发布者先写body、后发布状态；消费者先确认状态、后读body、最后确认释放。
6. 不要与KFC、Matmul高阶API或其他占用SSBuffer的协议混用同一任务。
7. 不要通过越界、未对齐访问或让AIC/AIV启动不同任务来测试限制；这些行为无定义且可能影响设备任务。

## 边界验证样例

[SSBuffer边界场景样例](../../../../../../../../examples/01_simd_cpp_api/03_basic_api/05_sync_control/ssbuf_boundary_cases)验证以下安全边界：

- 偏移`0`处的首个32B记录。
- 偏移`3040`处的最后一个32B记录，恰好位于3KB空间的末端。
- AIV0/AIV1写入互不重叠的首尾分区，AIC分别消费。
- 连续16次启动同一kernel，在每轮初始化后复用相同首尾记录。

样例不会执行越界或非32B对齐访问。实际构建和上板结果记录在工作区根目录的`res.md`。
