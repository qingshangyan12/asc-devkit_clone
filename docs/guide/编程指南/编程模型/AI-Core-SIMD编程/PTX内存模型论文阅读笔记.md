# PTX内存模型论文阅读笔记

本文是对论文[A Formal Analysis of the NVIDIA PTX Memory Consistency Model](https://d1qx31qr3h6wln.cloudfront.net/publications/ASPLOS_2019_PTXMemoryModel.pdf)的中文阅读笔记，不是论文全文翻译。

## 1. 论文关注的问题

GPU程序包含大量并行线程，线程之间可能通过共享内存、全局内存、原子操作和同步指令交换数据。为了让编译器、硬件和程序员对这些并发访问有一致理解，PTX需要定义哪些读写结果是允许的，哪些结果违反内存模型。

论文指出，PTX内存模型并不是简单的全局顺序模型，而是带作用域的弱内存模型。不同线程、不同CTA、不同GPU以及系统级参与者之间，不一定默认共享同一个同步范围。程序员需要通过合适的同步作用域建立可见性。

## 2. 论文的组织方式

论文的核心写法是先定义模型对象，再定义对象之间的关系，最后用这些关系约束合法执行结果。

常见对象包括：

- 内存事件：读、写、原子操作、Fence等。
- 位置：事件访问的内存地址或地址范围。
- 作用域：同步或内存操作影响的范围，例如CTA、GPU、System。
- 程序顺序：同一线程内指令的先后关系。
- 读写关系：一次读从哪一次写获得值。
- 同步关系：哪些同步操作能够让某个写对另一个读可见。

这种写法的特点是：内存模型不是直接列举所有场景，而是把场景拆成事件和关系。只要能判断两个事件是否访问同一位置、作用域是否相交、是否存在同步关系，就能推导该访问是否有确定的可见性。

## 3. 对NPU文档的参考价值

这篇论文不能作为NPU硬件行为的直接依据，但它的写法适合参考：

- 先声明本文采用的抽象对象，例如访问事件、访问路径、作用域、同步关系。
- 再讨论每类存储层级中，哪些数据副本需要维护一致性。
- 对每类访问路径分别说明：读从哪里来，写到哪里去，什么时候对其他执行单元可见。
- 把同步和缓存控制写成建立可见性的操作，而不是只写成孤立API说明。

## 4. 映射到NPU内存一致性文档时的取舍

NPU SIMD/SIMT资料中公开说明的是具体硬件单元和API行为，例如L2 Cache、DCache、ICache、DataCacheCleanAndInvalid、SetL2CacheHint、asc_threadfence等。本文档可以借用PTX论文的“事件-关系-作用域”写法，但不应把PTX里的CTA/GPU/System作用域、morally strong等术语直接套到NPU上。

更合适的做法是使用NPU自己的对象：

- 执行单元：Scalar、MTE2、MTE3、FixPipe、Vector、SIMT线程。
- 存储位置：GM、L2 Cache、DCache、ICache、UB、L1 Buffer、L0 Buffer。
- 可见性范围：单流水、单AI Core、单Device、多Device通信。
- 建立可见性的手段：PipeBarrier、SetFlag/WaitFlag、CrossCoreSetFlag/CrossCoreWaitFlag、SyncAll、DataCacheCleanAndInvalid、ReadGmByPassDCache、WriteGmByPassDCache、asc_threadfence等。

## 5. 写作建议

NPU文档如果模仿该论文，可以采用如下结构：

1. 先定义一次内存访问事件包含哪些信息：执行者、操作类型、地址范围、访问路径、作用域。
2. 再定义一致性问题来自哪里：同一地址存在多个缓存副本，或者跨执行单元没有建立顺序。
3. 对每种缓存路径描述读写规则：读命中哪里，写先进入哪里，何时写回。
4. 对每种同步或缓存控制接口描述它建立了什么关系：顺序关系、写回关系、失效关系、绕过缓存关系。
5. 最后给出开发者义务：当存在生产者-消费者关系时，必须选择合适的同步和缓存控制接口。

