# C++ Tensor编程概述

如概述章节所述，本章节重点描述如何基于Ascend C提供的Tensor编程接口实现业界Tensor/Tile编程。其特点基于C++ Tensor编程接口，提供C++语言基础完备编程能力，支持自主管理同步与内存布局，匹配C++ Tensor开发习惯，开放全部底层硬件能力，支撑精细化调优与极致性能实现。

## 设计理念：面向Tensor的编程抽象

### Tensor编程的核心思想

Tensor本质是对内存地址的抽象封装，通过数组方式来管理和分配内存，以及执行相关计算，更匹配SIMD的硬件执行模型。Tensor编程的核心思想是将原始内存地址抽象为Tensor对象，实现从"地址操作"到"对象管理"的编程层次提升。

### Tensor的定义与分配管理

根据芯片分级存储架构，Tensor分为两类：GlobalTensor对应全局内存（GM），存放算子输入/输出数据，作为算子与外部数据交互的门户；LocalTensor对应片上内存（UB、L1、L0A/L0B/L0C等缓冲区），承载计算过程中的输入、输出和中间数据。Tensor体系在演进过程中逐步增强能力：基础Tensor仅携带数据指针和大小信息，需手动传递操作参数；扩展Tensor引入Layout概念，携带Shape和Stride信息，接口可根据Layout自动推导搬运长度、计算元素数等参数，大幅降低编程复杂度。

- 基础Tensor(不带Layout)的定义和使用
```cpp
// Definition: carries data type + memory location, requires manual parameter passing
AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
LocalTensor<half> ubTensor = ubAllocator.Alloc<half, 256>();
GlobalTensor<half> gmTensor;
gmTensor.SetGlobalBuffer((__gm__ half*)gmBuffer, bufferSize);
// Operation: requires manual size/count parameter passing
DataCopy(ubTensor, gmTensor, 256);
Add(dstTensor, src0Tensor, src1Tensor, 256);
```

- 扩展Tensor(带有Layout)的定义和使用
```cpp
// ===== Extended Tensor (Tensor API) =====
// Definition: created via MakeTensor, carries Shape and Stride
auto gmTensor = MakeTensor(gmPointer, MakeShape(128, 64));
auto ubTensor = MakeTensor(ubPointer, MakeShape(128, 64));
// Operation: no manual size/count setting, Copy interface automatically infers from Tensor info
Copy(ubTensor, gmTensor);
```

## 编程范式: 面向C++ Tensor的SPMD编程范式

### Kernel函数及函数修饰符

核函数是Ascend C算子设备侧实现的入口。C++ Tensor编程在核函数定义与调用方面遵循SIMD编程的通用规范，无特殊语法规则，完整规则详见[核函数](../核函数.md)。

### 内存层级

C++ Tensor编程将物理内存抽象为Tensor对象，开发者无需直接操作指针，Tensor作为编程接口的载体承载内存操作，携带数据类型、形状、内存位置等属性，搬运计算接口直接操作Tensor对象即可完成数据流转：
- **GlobalTensor**：用于存储Global Memory数据，作为算子与外部数据交互的门户，开发者通过SetGlobalBuffer设置地址和大小，可配置L2缓存策略。
- **LocalTensor**：用于存储UB、L1 Buffer、L0A/L0B/L0C Buffer等片上内存数据，承载计算过程中的输入、输出和中间数据。自Ascend 950PR/Ascend 950DT起，扩展Tensor引入Layout概念，实现编译期类型安全和自动参数推断。
- **RegTensor**：用于存储寄存器变量，是矢量计算的基本存储单元，位宽为VL（Vector Length），可存储VL/sizeof(T)个数据元素，配合Load-Compute-Store模式与UB协作完成计算。

> 📌 寄存器可编程能力从**Ascend 950PR/950DT**引入，构建三级内存架构，可获得接近峰值的计算性能。

### 同步机制

C++ Tensor编程针对AI Core同步机制分别提供了核内同步和核间同步接口。核内同步包括：Mutex互斥锁保护共享资源并发访问(Ascend 950PR/Ascend 950DT引入)，SetFlag/WaitFlag实现多流水线间同步（生产者发信号、消费者等待），PipeBarrier约束单流水线内执行顺序。核间同步采用计数器机制，通过CrossCoreSetFlag/WaitFlag协调多核执行进度。
- **Mutex互斥锁**用于核内异步流水指令之间的同步处理，其功能类似于传统CPU中的锁机制。通过锁定指定流水线再释放流水线来完成流水线间的同步。
- **SetFlag/WaitFlag（多流水同步）**采用**信号通知-阻塞等待**模式，通过流水线间的主动通知与被动等待实现执行顺序的严格约束。
- **PipeBarrier（单流水同步）**采用**阻塞等待**模式，保证前序指令中所有数据的读写工作全部完成，后序指令才能执行。用于在同一流水线内部约束执行顺序。
- **核间同步（多AI Core协作）**采用**计数器递增/递减**模式，通过flag_id对应的计数器协调多核执行进度。每个flag_id对应一个初始值为0的计数器，arrive指令递增计数器表示"当前核到达同步点"，wait指令检测计数器大于0则递减并继续执行、等于0则阻塞等待。


### 数据搬运与计算接口

AI Core采用「标量控制单元+Vector矢量计算单元+Cube矩阵计算单元+分级本地存储（UB/L1/L0/Reg）」的异构架构，各组件分工明确、协同联动，硬件能力边界直接决定算子的数据搬运路径与计算形态。数据搬运与计算是AI Core算子开发的两大核心操作，共同构成算子执行的数据流转与计算处理闭环——数据搬运解决「数据从哪里来、到哪里去」的问题，计算解决「数据如何处理」的问题，二者协同配合完成算子功能实现。

C++ Tensor编程针对AI Core的架构，提供了面向Tensor编程能力的搬运和计算接口。搬运接口分为**矢量搬运接口**（GM↔UB）和**矩阵搬运接口**（GM→L1→L0A/L0B→L0C→GM），基础搬运手动传参、显式同步。计算接口分为矢量计算和矩阵计算两大类，矢量计算覆盖算术、比较、转换等场景；Ascend 950PR/950DT引入寄存器后，矢量计算接口拆分为**Memory矢量计算接口**和**Reg矢量计算接口**，**矩阵计算接口**以Mmad为核心执行矩阵乘加运算。此外，Ascend 950PR/950DT还引入了Layout扩展Tensor，并基于扩展Tensor提供了一套简化参数传入的矩阵搬运和计算接口。


本章阐述了Ascend C编程体系中的C++ Tensor编程范式：将内存地址抽象为Tensor对象，隐藏地址细节。基础能力接口提供Tensor定义、数据搬运、计算、同步等核心功能。扩展能力引入Layout概念，通过Shape和Stride编译期确定实现类型安全，支持平铺布局、层次化布局、分形布局等多种布局模式。开发者可根据场景选择基础能力或扩展能力，复杂场景可混合使用。

后续章节将逐层深入、循序渐进地详解内存层级管理、同步机制、数据搬运与计算接口、编程示例，帮助开发者熟练掌握C++ Tensor编程的核心能力。