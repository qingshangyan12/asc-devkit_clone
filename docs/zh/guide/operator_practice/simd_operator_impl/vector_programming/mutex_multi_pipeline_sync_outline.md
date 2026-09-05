# 基于Mutex实现多流水同步文档大纲

## 行文目标

以双缓冲Add算子为载体，先说明Buffer复用为什么需要反向同步，再解释Mutex的阻塞原理，最后通过与SetFlag/WaitFlag的对比说明Mutex如何减少同步代码和架构相关配置。

## 1. 开篇：给出核心结论和适用范围

- 明确适用产品为Ascend 950PR/Ascend 950DT。
- 概述数据搬入、计算、数据搬出三阶段，以及Buffer复用产生的同步需求。
- 直接给出核心结论：`Lock<当前pipe>(mutexId)`只需要指定当前流水；此前所有流水中相同MutexID的Mutex均已Unlock后，当前流水才会继续执行。
- 说明该机制使开发者无需识别或传入前序流水类型。

## 2. 前置说明：限定概念和接口边界

- 链接双缓冲和内存一致性相关概念。
- 说明MutexID的申请和释放方式。
- 明确Mutex与同流水内排序场景的边界。

## 3. 适用场景：从Buffer生命周期引出MutexID

- 描述`PIPE_MTE2 -> PIPE_V -> PIPE_MTE3`的典型矢量算子流水。
- 说明双缓冲使相邻tile重叠，但同一Buffer复用前必须等待前一次搬出完成。
- 将一个MutexID绑定一个Buffer，建立“MutexID表示Buffer完整生命周期”的认知。
- 用时序图展示第`i`轮使用buffer0、第`i + 1`轮使用buffer1、第`i + 2`轮复用buffer0的关系。

## 4. MutexID管理：保证资源与同步状态一一对应

- 给出TPipe-TQue和静态Tensor两种编程范式下的MutexID管理方式。
- 说明双缓冲应使用两个MutexID，避免不同Buffer之间错误串行化。

## 5. 算子实现：用核心代码建立正向与反向依赖

- 先给出循环轮次、Buffer和MutexID的对应表。
- 展示MTE2、V、MTE3三个阶段在同一MutexID上的Lock/Unlock代码。
- 解释正向依赖由同一tile内的MTE2、V、MTE3顺序形成。
- 解释反向依赖由第`i + 2`轮再次Lock同一MutexID形成。

## 6. 机制重点：为什么Lock不需要关心前序流水

- 明确Lock阻塞的是当前流水的后续指令。
- 明确解除阻塞的判定对象是此前所有流水中相同MutexID的Mutex，而不是某一条指定源流水。
- 说明第`i + 2`轮的`Lock<PIPE_MTE2>`自动包含对第`i`轮`PIPE_MTE3`完成的等待，但代码中不需要写出`PIPE_MTE3 -> PIPE_MTE2`。

## 7. 与SetFlag/WaitFlag对比：说明简化来源

- 对比依赖表达和解除阻塞条件：Mutex按资源状态聚合，SetFlag/WaitFlag按源、目的流水和事件ID配对。
- 说明反向同步中，SetFlag/WaitFlag需要显式维护`PIPE_MTE3 -> PIPE_MTE2`事件；Mutex只需复用同一MutexID。
- 说明SetFlag/WaitFlag还要按循环次数处理首轮无等待、末轮无通知等边界分支；Mutex不需要这些反向同步分支。
- 说明SetFlag/WaitFlag依赖HardEvent，源、目的流水组合在不同硬件架构上可能不同；Mutex无需选择HardEvent，但产品支持范围仅限支持Mutex的产品。
- 说明MutexID和事件ID的数量及资源规划差异。

## 8. 使用约束与延伸阅读

- 列出Lock/Unlock成对使用、禁止嵌套、释放MutexID等约束。
- 明确同一流水内排序应使用PipeBarrier。
- 链接Mutex、Lock、Unlock、C API同步接口和完整样例。
