# C++ Tensor编程概述

C++ Tensor编程是面向LocalTensor与GlobalTensor的编程范式。核心设计理念是将内存地址抽象为Tensor对象，开发者通过Tensor进行数据搬运、计算操作、同步管理。相比C指针编程的直接地址操作，Tensor编程隐藏内存地址细节，提供更高级的抽象层次，降低开发复杂度。Ascend C提供基础能力接口，支持LocalTensor/GlobalTensor定义、数据搬运、矢量计算、矩阵乘、同步等核心功能。在此基础上，对LocalTensor进行扩展，引入Layout概念描述数据布局，实现类型安全、编译期确定的张量操作，并提供Copy/Mmad Atom、零开销切片等扩展能力。本章旨在帮助开发者建立对Ascend C编程范式的整体认知。全文遵循「设计理念—Tensor抽象—基础能力—扩展能力—接口选择」的递进逻辑展开。

## 设计理念：面向Tensor的编程抽象
### Tensor编程的核心思想
Tensor编程的核心思想是将原始内存地址抽象为Tensor对象，实现从"地址操作"到"对象管理"的编程层次提升。Tensor体系分为两类：LocalTensor对应片上内存体系（包括UB、L1、L0A、L0B、L0C等多级缓冲区），承载计算过程中的输入、输出、中间数据；GlobalTensor对应全局内存（GM），作为算子与外部数据交互的门户。基础能力的Tensor对象携带数据类型和内存位置两类属性，接口调用需手动传递操作参数。扩展能力的Tensor在此基础上引入Layout概念，携带Shape和Stride信息，成为"自描述数据容器"。接口调用时编译器自动推断搬运长度、计算元素数等参数，实现"一次定义、自动推断"的便捷开发体验。
```cpp
// ===== 基础能力的Tensor =====
// 定义：携带数据类型 + 内存位置，需手动传参
AscendC::LocalMemAllocator<AscendC::Hardware::UB> ubAllocator;
LocalTensor<half> ubTensor = ubAllocator.Alloc<half, 256>();
GlobalTensor<half> gmTensor;
gmTensor.SetGlobalBuffer((__gm__ half*)gmBuffer, bufferSize);
// 操作：需手动传递size/count参数
DataCopy(ubTensor, gmTensor, 256);
Add(dstTensor, src0Tensor, src1Tensor, 256);

// ===== 扩展能力的Tensor (Tensor API) =====
// 定义：通过MakeTensor创建，携带Shape和Stride
auto gmTensor = MakeTensor(gmPointer, MakeShape(128, 64));
auto ubTensor = MakeTensor(ubPointer, MakeShape(128, 64));
// 操作：无需手动设置size/count，Copy接口根据Tensor信息自动推导
Copy(ubPointer, gmPointer);
```

### Tensor编程的特征
- **降低开发复杂度**：隐藏内存地址细节，Tensor携带数据类型和形状信息，接口自动推断操作参数。
- **提升代码可读性**：Tensor命名表达数据语义，接口调用直观，参数通过Tensor对象传递。
- **扩展能力优化**：Layout编译期确定实现类型安全，Slice/Tile零开销切片，Atom设计简化参数配置。

## 编程范式: 面向C++ Tensor的SPMD编程范式

### Kernel函数及函数修饰符

核函数（Kernel Function）是Ascend C算子设备侧实现的入口。Ascend C允许用户使用C/C++函数的语法扩展来编写设备端的运行代码，用户在核函数中进行数据访问和计算操作，由此实现该算子的所有功能。区别于普通的C++函数调用时仅执行一次，当核函数被调用时，多个核都执行相同的核函数代码，具有相同的函数入参，并行执行。

使用__global__函数类型限定符来标识它是一个核函数；使用__vector__限定符标识该核函数在Vector核执行；其指针入参变量需要增加变量类型限定符__gm__，表明该指针变量指向Global Memory上某处内存地址；使用<<<...>>>内核调用符调用执行核函数，并指定调用时的执行核数。
- `__global_`: 执行空间限定符，声明一个Kernel函数。
- `__vector__`: 标识该核函数仅在Vector核执行。针对耦合模式的硬件架构，该修饰符不生效。
- `__cube__`: 标识该核函数仅在Cube核执行。针对耦合模式的硬件架构，该修饰符不生效。
- `__mix__`: 标识该核函数同时在Cube核和Vector核上执行。(cube, vec)分别表示核函数启动的Cube核和Vector核的配比，支持的配比为(1, 0)，(0, 1)，(1, 1)， (1, 2)。针对耦合模式的硬件架构，该修饰符不生效。
```cpp
// 实现核函数
__vector__ __global__ void add_custom(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    uint32_t blockLength = TILE_LENGTH * NUM_BLOCKS / asc_get_block_num();
}

// 调用核函数
void add_custom_do(uint32_t numBlocks, void* l2ctrl, void* stream, uint8_t* x, uint8_t* y, uint8_t* z)
{
    add_custom<<<numBlocks, l2ctrl, stream>>>(x, y, z);
}
```

另外，通过`__aicore__`和`__host__`限定符来声明函数执行空间：
- **__aicore__**: 执行空间限定符，声明一个在Device侧执行的函数, 该函数只能被__global__函数，或者其他__aicore__函数调用。
```cpp
// 定义一个在Device侧执行的函数
__aicore__ void bar() {}

// 定义核函数, 当前不通过__vector__、__cube__、__mix__等指定具体的执行核, 标识__global__ __aicore__告知编译框架自行推导
__global__ __aicore__ void foo() {
  bar();
}
```
- **__host__**: 执行空间限定符, 声明一个在Host侧执行的函数，__global__ 和__host__不能一起使用，__host__限定符是可选项，无函数执行空间限定符定义的函数，默认是host函数。
```cpp
// 不指定执行空间限定符, 默认定义Host侧执行函数
int foo() {}

// 指定__host__空间限定符, 定义Host侧执行函数
__host__ int bar() {
  foo();
}

// __global__ 和__host__不能一起使用
__global__ __host__ void kfunc() {}
```

**学习路径**：详见 [核函数](../核函数.md) [SIMD Builtin关键字](../../../语言扩展层/SIMD-BuiltIn关键字.md)

### 内存层级

AI Core采用分级存储架构，不同计算单元的编程视角有所差异：**Vector矢量计算**采用「Global Memory → UB → Register」三级层级，**Global Memory**存放输入输出数据，**UB**作为矢量计算数据枢纽，**Register**（Ascend 950PR/Ascend 950DT引入）位于Vector计算单元最内层，具备最低访存延迟与最高带宽；**Cube矩阵计算**采用「Global Memory → L1/L0系列Buffer」两级层级，L1 Buffer、L0A/L0B Buffer、L0C Buffer服务矩阵计算。

C++ Tensor编程将物理内存抽象为Tensor对象，开发者无需直接操作指针，Tensor作为编程接口的载体承载内存操作：
- **GlobalTensor**：用于存储Global Memory数据，存放算子输入/输出数据，开发者通过SetGlobalBuffer设置地址和大小，可配置L2缓存策略。
- **LocalTensor**：用于存储片上内存数据，包括UB、L1 Buffer、L0A/L0B/L0C Buffer等，服务于矢量计算和矩阵计算。自Ascend 950PR/Ascend 950DT起，扩展Tensor引入Layout概念，实现编译期类型安全和自动参数推断。
- **RegTensor**：用于存储寄存器变量（Ascend 950PR/Ascend 950DT引入），是矢量计算的基本存储单元，位宽为VL（Vector Length），可存储VL/sizeof(T)个数据元素。Vector计算单元通过Load-Compute-Store与UB协作。

> 📌 寄存器可编程能力从**Ascend 950PR/Ascend 950DT**引入，构建三级内存架构，可获得接近峰值的计算性能。

**学习路径**：详见[内存层级](./内存层级.md)

### 同步机制

AI Core采用多流水线异步并行架构。MTE2流水线负责数据搬入（GM→片上存储），MTE3流水线负责数据搬出（片上存储→GM），Vector流水线执行矢量计算，Cube流水线执行矩阵乘运算。流水线异步执行带来显著性能收益，但缺乏自动协调，需通过同步机制确保数据依赖关系满足。

C++ Tensor编程针对AI Core同步机制分别提供了核内同步和核间同接口。核内同步包括：Mutex互斥锁保护共享资源并发访问(Ascend 950PR/Ascend 950DT引入)，SetFlag/WaitFlag实现多流水线间同步（生产者发信号、消费者等待），PipeBarrier约束单流水线内执行顺序。核间同步采用计数器机制，通过CrossCoreSetFlag/WaitFlag协调多核执行进度。
- **Mutex互斥锁**用于核内异步流水指令之间的同步处理，其功能类似于传统CPU中的锁机制。通过锁定指定流水线再释放流水线来完成流水线间的同步。
- **SetFlag/WaitFlag（多流水同步）**采用**信号通知-阻塞等待**模式，通过流水线间的主动通知与被动等待实现执行顺序的严格约束。
- **PipeBarrier（单流水同步）**采用**阻塞等待**模式，保证前序指令中所有数据的读写工作全部完成，后序指令才能执行。用于在同一流水线内部约束执行顺序。
- **核间同步（多AI Core协作）**采用**计数器递增/递减**模式，通过flag_id对应的计数器协调多核执行进度。每个flag_id对应一个初始值为0的计数器，arrive指令递增计数器表示"当前核到达同步点"，wait指令检测计数器大于0则递减并继续执行、等于0则阻塞等待。

**学习路径**：详见[同步机制](./同步机制.md)

### 数据搬运与计算接口

AI Core采用「标量控制单元+Vector矢量计算单元+Cube矩阵计算单元+分级本地存储（UB/L1/L0/Reg）」的异构架构，各组件分工明确、协同联动，硬件能力边界直接决定算子的数据搬运路径与计算形态。数据搬运与计算是AI Core算子开发的两大核心操作，共同构成算子执行的数据流转与计算处理闭环——数据搬运解决「数据从哪里来、到哪里去」的问题，计算解决「数据如何处理」的问题，二者协同配合完成算子功能实现。

C++ Tensor编程针对AI Core的架构， 提供了面向Tensor编程能力的搬运和计算接口，搬运接口分为矢量搬运（GM↔UB）和矩阵搬运（GM→L1→L0A/L0B→L0C→GM），基础搬运手动传参、显式同步。计算接口分为矢量计算和矩阵计算两大类，矢量计算覆盖算术、比较、转换等场景，Ascend 950PR/950DT引入寄存器，矢量计算接口拆分为Memory矢量计算接口和Reg矢量计算接口，矩阵计算以Mmad为核心执行矩阵乘加运算。另外 Ascend 950PR/950DT还引入了Layout对Tensor进行扩展，还基于扩展Tensor提供了一套简化参数传入的矩阵搬运和计算接口；

**学习路径**：详见[数据搬运与计算](./数据搬运与计算.md)

本章阐述了Ascend C编程体系中的C++ Tensor编程范式：将内存地址抽象为Tensor对象，隐藏地址细节。基础能力接口提供Tensor定义、数据搬运、计算、同步等核心功能。扩展能力引入Layout概念，通过Shape和Stride编译期确定实现类型安全，支持平铺布局、层次化布局、分形布局等多种布局模式。开发者可根据场景选择基础能力或扩展能力，复杂场景可混合使用。

后续章节将逐层深入、循序渐进地详解内存层级管理、同步机制、数据搬运与计算接口、编程示例，帮助开发者熟练掌握C++ Tensor编程的核心能力。