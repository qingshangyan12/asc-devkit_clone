# 基础知识

>[!NOTE]说明
>学习本节前，请先了解[CV融合算子](../basics.md)以及[地址空间限定符](../../../../../programming_guide/language_extension/simd_builtin_keywords.md#地址空间限定符)。

## SSBuffer

SSBuffer是AI Core内部的物理存储单元。在分离模式下，AIC和AIV可以通过Scalar访问SSBuffer，并在同一个Mix任务中交换控制信息。与先把信息写入Global Memory（GM）再由另一侧读取相比，SSBuffer更适合传递数据准备完成通知、计算任务消息、地址和长度描述符等小消息。

SSBuffer容量有限，不用于保存大块中间计算结果，也不能替代GM、Unified Buffer（UB）或L1 Buffer。业务数据仍应根据算子的数据流放在对应存储空间中，SSBuffer只负责传递双方约定的消息。

## 使用场景和优势

在CV融合算子中，AIC和AIV通常各自完成一部分计算。当一侧需要把少量状态或下一步任务告诉另一侧时，可以使用SSBuffer。常见场景包括：

- AIV通知AIC输入数据已经准备完成。
- AIC向AIV下发下一轮计算需要的地址和长度。
- AIV与AIC通过消息队列交换计算任务，并确认消息已经处理完成。

使用SSBuffer后，小消息不需要绕行GM，可以减少不必要的数据搬运。不过，开发者需要自行约定空间如何划分、消息何时生效以及双方何时退出。AIC/AIV通信有两种相关的使用方式，二者的接口边界和消息空间不同，不能混为一谈。

| 使用方式 | 适用场景 | 开发者需要处理的内容 |
| --- | --- | --- |
| 显式SSBuffer通信 | AIV和AIC需要交换自定义控制消息。 | 直接访问`__ssbuf__`地址空间，自行设计消息队列、消息状态、核间同步和退出条件。 |
| KFC调用 | AIV以Client身份向AIC Server下发Cube计算任务，用于配合Matmul高阶API使用。 | 使用公开的`KfcWorkspace`、`CreateCubeResGroup`和`CubeResGroupHandle`接口管理消息队列，不直接访问内部KFC消息结构。 |

## 规格和约束

当前SSBuffer的规格如下。

| 项目 | 说明 |
| --- | --- |
| 支持产品 | Ascend 950PR/Ascend 950DT（`dav-3510`） |
| 总容量 | 3KB |
| 非Mix模式 | AIC、AIV0、AIV1各自独占1KB |
| `__mix__(1, 2)`模式 | AIC、AIV0、AIV1共享3KB |
| 对齐要求 | 按32B对齐要求规划读写地址和数据布局。 |
| 初始内容 | 不保证为0，每次任务启动时都需要由唯一的初始化方清理消息状态。 |
| 任务约束 | AIC和AIV必须启动同一个任务，不同任务不能访问同一SSBuffer。 |
| 越界访问 | 超过3KB末端会产生异常。 |

### 基础 API下获取SSBuffer地址

使用基础API时，通过[`GetSsbufBaseAddr`](../../../../../../api/SIMD-API/basic_api/tool_interface/system_resources_and_variables/GetSsbufBaseAddr.md)获取SSBuffer基地址。返回值属于`__ssbuf__`地址空间，后续指针必须保留`__ssbuf__`限定，不能作为普通GM地址使用。

```cpp
__ssbuf__ void* base = AscendC::GetSsbufBaseAddr(); // __ssbuf__表示指针指向SSBuffer地址空间。
__ssbuf__ uint8_t* bytes = reinterpret_cast<__ssbuf__ uint8_t*>(base); // 保留SSBuffer地址空间限定。
```

### C API下获取SSBuffer地址

C API当前没有与`GetSsbufBaseAddr`对应的接口。在`dav-3510` NPU上，`__ssbuf__`地址空间的零地址表示SSBuffer基地址，因此可以把零地址转换为带`__ssbuf__`限定的工作区指针。这里的零地址属于特定地址空间，不是GM中的空地址。

在本样例使用的`dav-3510`编译环境中，不支持直接声明带`__ssbuf__`限定的数组，也不支持让`__ssbuf__`修饰指向数组类型的指针。下面两种写法都会报错，因此不能用它们“申请”SSBuffer空间。

```cpp
__ssbuf__ uint8_t ssbuffer[3 * 1024]; // 不支持：数组对象只能位于默认的Local Memory。

using SsbufByteArray = uint8_t[3 * 1024];
__ssbuf__ SsbufByteArray* ssbuffer; // 不支持：指向数组类型的指针不能带该地址空间限定符。
```

可以把数组作为普通结构体的成员，用结构体描述消息队列布局，再让带`__ssbuf__`限定的结构体指针指向SSBuffer基地址。结构体声明只定义布局，并不会另外分配存储空间。

```cpp
struct alignas(128) CommMessage {
    volatile uint32_t head;
    volatile uint32_t reserved0;
    volatile uint64_t sequence;
    volatile uint64_t value;
    volatile uint64_t checksum;
    uint64_t reserved[12];
};

struct SsbufWorkspace {
    CommMessage queue[2][8]; // 两个AIV各使用一条深度为8的消息队列。
};

__aicore__ inline __ssbuf__ SsbufWorkspace* get_workspace() // __aicore__表示函数在AI Core上执行。
{
    return reinterpret_cast<__ssbuf__ SsbufWorkspace*>(0); // __ssbuf__表示指针指向SSBuffer。
}
```

跨核读写的字段使用`volatile`，避免编译器缓存字段值或改变这些访问的先后关系。AIV和AIC仍需通过核间同步接口交接消息，`volatile`本身不是同步机制。

如果希望用数组统一管理多条消息队列，可以声明一个位于默认Local Memory的**指针数组**，数组中的每个元素分别指向SSBuffer中的一段`CommMessage`空间。这种写法可以编译，但消息队列起始偏移需要开发者自行计算。

```cpp
__ssbuf__ CommMessage* queues[2] = {
    reinterpret_cast<__ssbuf__ CommMessage*>(0),
    reinterpret_cast<__ssbuf__ CommMessage*>(8 * sizeof(CommMessage))
};

queues[sender][queueIndex].head = make_head(sender);
```

这里的`queues`数组本身不在SSBuffer中，只有数组元素指向的`CommMessage`位于SSBuffer。与这种写法相比，`SsbufWorkspace::queue[2][8]`把结构体字段和队列偏移放在同一份类型定义中，不需要手动维护各消息队列的起始偏移，本样例采用结构体成员数组方式。

## 编程范式

显式SSBuffer通信需要先定好一套双方都遵守的规则。一个完整规则至少要回答六个问题：哪一段消息空间属于谁、谁负责写、谁负责读、怎样表示消息有效、怎样确认消息已处理以及双方何时结束。

以AIV向AIC发送消息为例，推荐按以下步骤实现。

1. 规划空间。为不同AIV划分互不重叠的消息队列，明确消息空间的对齐方式、结构大小和总占用空间。
2. 清理状态。由AIC作为唯一初始化方清除所有消息头，避免把SSBuffer中的旧内容当成新消息。
3. 建立启动顺序。AIC清理完成后，分别向AIV0和AIV1发送核间同步通知，两个AIV收到通知后才能访问各自的消息队列。
4. 发布消息。AIV写完消息体和消息头后，通过核间同步接口发送消息准备完成通知。
5. 消费消息。AIC等待通知到达，再读取和检查消息；处理完成后清除消息头，并向发送方返回ACK通知。
6. 复用或退出。AIV只有收到ACK后才能复用对应的消息空间；发送结束后，还要等待所有未返回的ACK再退出。

通信过程如下。

```text
AIC：清理消息头 -> 通知AIV启动 -> 等待消息通知 -> 读取消息 -> 清消息头 -> 返回ACK
                         |                  ^                              |
                         v                  |                              v
AIV：                等待启动通知 -> 写消息体和消息头 -> 发送消息通知 -> 等待ACK或复用消息空间
```

下一节将以[基于SSBuffer的AIV/AIC通信算子](./operator_impl.md)为例，说明这套编程范式如何落到消息布局和Device侧代码中。
